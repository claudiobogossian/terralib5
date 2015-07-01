/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file Writer.cpp
   
  \brief A class that models a XML writer object built on top of Xerces-C++.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "StrToXMLCh.h"
#include "XMLChToStr.h"
#include "Writer.h"

// Boost
#include <boost/lexical_cast.hpp>

// Xerces-C++
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>

te::xerces::Writer::Writer()
  : m_impl(0),
    m_doc(0),
    m_indice(0),
    m_topIndice(0)
{
  // TODO
}

te::xerces::Writer::~Writer()
{
  if(m_doc)
  {
    m_doc->release();
  }
}

void te::xerces::Writer::writeStartDocument(const std::string& encoding, const std::string& standalone)
{
  m_standalone = te::common::Convert2UCase(standalone) == "NO" ? false : true;
  m_encoding = encoding;
}

void te::xerces::Writer::writeToFile()
{
  // primeiro a declaracao dos objetos Xerces
#if XERCES_VERSION_MAJOR == 2
  xercesc::DOMWriter* theSerializer = ( (xercesc::DOMImplementationLS*)m_impl)->createDOMWriter();
  int* theOutput = 0; // for compat
#else // Xerces3
  xercesc::DOMLSSerializer* theSerializer = ((xercesc::DOMImplementationLS*)m_impl)->createLSSerializer();
  xercesc::DOMLSOutput* theOutput = 0;
#endif

  xercesc::XMLFormatTarget* myFormTarget = 0;

  try
  {
    // Se nao tem filename, eh stdout
    if( m_uri.empty() )
      myFormTarget = new xercesc::StdOutFormatTarget();
    else
      myFormTarget = new xercesc::LocalFileFormatTarget( m_uri.c_str() );

#if XERCES_VERSION_MAJOR == 2
    theSerializer->writeNode( myFormTarget, *doc_ );
#else // Xerces3
    theOutput = ( (xercesc::DOMImplementationLS*)m_impl)->createLSOutput();
    theOutput->setByteStream(myFormTarget);
    theSerializer->write(m_doc, theOutput );
#endif

    delete theOutput;
    delete theSerializer;
    delete myFormTarget;
  }
  catch( const xercesc::OutOfMemoryException& )
  {
    delete theOutput;
    delete theSerializer;
    delete myFormTarget;

    throw te::common::Exception(TE_TR("Out of memory"));
  }
  catch( const xercesc::DOMException& e )
  {
    delete theOutput;
    delete theSerializer;
    delete myFormTarget;

    std::string err = XMLChToStr(e.getMessage()).getStr();

    throw te::common::Exception(TE_TR("Exception message is: ") + err);
  }
  catch(...)
  {
    delete theSerializer;
    delete myFormTarget;

    throw te::common::Exception(TE_TR("An error occurred while outputing the xml document."));
  }
}

void te::xerces::Writer::writeStartElement(const std::string& qName)
{
  try
  {
    if(!m_doc)
    {
      m_impl = xercesc::DOMImplementationRegistry::getDOMImplementation(StrToXMLCh("Core").getXMLCh());

      if(m_impl)
      {
        try
        {
          if(m_rootNamespaceUri.empty())
            m_doc = m_impl->createDocument(0, StrToXMLCh(qName).getXMLCh(), 0);
          else
            m_doc = m_impl->createDocument(StrToXMLCh(m_rootNamespaceUri).getXMLCh(), StrToXMLCh(qName).getXMLCh(), 0);

          m_doc->setXmlStandalone(m_standalone);

          m_elementSet.push_back(m_doc->getDocumentElement());

          m_topIndice = 0;

          m_topElementSet.push_back(0);
        }
        catch(xercesc::DOMException& e)
        {
          std::string err = XMLChToStr(e.getMessage()).getStr();
        }
      }
      else
        throw te::common::Exception(TE_TR("DOM implementation fails"));
    }
    else
    {
      m_elementSet.push_back(m_doc->createElement(StrToXMLCh(qName).getXMLCh()));
      m_elementSet[m_topIndice]->appendChild(m_elementSet[++m_indice] );

      // ajusta o topElement do Elemento atual
      m_topElementSet.push_back(m_topIndice);

      // ele vira pai
      m_topIndice = m_indice;
    }
  }
  catch( const xercesc::OutOfMemoryException& ) 
  {
    throw te::common::Exception(TE_TR("Out of memory"));
  }
  catch( const xercesc::DOMException& e ) 
  {
    std::string err = XMLChToStr(e.getMessage()).getStr();
    throw te::common::Exception(TE_TR("Exception message is: ") + err );
  }
  catch(...)
  {
    throw te::common::Exception(TE_TR("An error occurred adding a element"));
  }
}

void te::xerces::Writer::writeElement(const std::string& qName, const std::string& value)
{
  try
  {
    m_elementSet.push_back(m_doc->createElement(StrToXMLCh(qName).getXMLCh()));
    m_elementSet[m_topIndice]->appendChild(m_elementSet[++m_indice]);

    // ajusta o topElement do Elemento atual
    m_topElementSet.push_back(m_topIndice);

    // Colocando dados no elemento, se houver
    if(!value.empty())
      addText(value);
  }
  catch(const xercesc::DOMException& e)
  {
    throw te::common::Exception(TE_TR("Exception message is: ") + XMLChToStr(e.getMessage()).getStr());
  }
  catch(...)
  {
    throw te::common::Exception(TE_TR("An error occurred adding the element: ") + ( qName.empty() ? std::string( "[empty]" ) : qName ));
  }
}

void te::xerces::Writer::writeElement(const std::string& qName, const double& value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeElement(const std::string& qName, boost::int32_t value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeElement(const std::string& qName, boost::uint32_t value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeElement(const std::string& qName, boost::int64_t value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeElement(const std::string& qName, boost::uint64_t value)
{
  writeElement(qName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeAttribute(const std::string& attName, const std::string& value)
{
  try
  {
    m_elementSet[m_topIndice]->setAttribute(StrToXMLCh(attName).getXMLCh(), StrToXMLCh(value).getXMLCh());
  }
  catch(const xercesc::DOMException& e)
  {
    throw te::common::Exception(TE_TR("Exception message is: ") + XMLChToStr(e.getMessage()).getStr());
  }
  catch(...)
  {
    throw te::common::Exception(TE_TR("An error occurred adding the attribute: ") + ( attName.empty() ? std::string( "[empty]" ) : attName ));
  }
}

void te::xerces::Writer::writeAttribute(const std::string& attName, const double& value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeAttribute(const std::string& attName, boost::int32_t value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeAttribute(const std::string& attName, boost::uint32_t value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeAttribute(const std::string& attName, boost::int64_t value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeAttribute(const std::string& attName, boost::uint64_t value)
{
  writeAttribute(attName, boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeValue(const std::string& value)
{
  addText(value);
}

void te::xerces::Writer::writeValue(const double& value)
{
  addText(boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeValue(boost::int32_t value)
{
  addText(boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeValue(boost::uint32_t value)
{
  addText(boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeValue(boost::int64_t value)
{
  addText(boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeValue(boost::uint64_t value)
{
  addText(boost::lexical_cast<std::string>(value));
}

void te::xerces::Writer::writeEndElement(const std::string& qName)
{
  m_topIndice = m_topElementSet.at( m_topIndice );
}

void te::xerces::Writer::addText(const std::string& qValue)
{
  try
  {
    xercesc::DOMText* domText = m_doc->createTextNode(StrToXMLCh(qValue).getXMLCh());
    m_elementSet[m_indice]->appendChild(domText);
  }
  catch(const xercesc::DOMException& e)
  {
    throw te::common::Exception(TE_TR("Exception message is: ") + XMLChToStr(e.getMessage()).getStr());
  }
  catch(...)
  {
    throw te::common::Exception(TE_TR("An error occurred adding the value to the element"));
  }
}
