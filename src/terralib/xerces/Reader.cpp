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
  \file Reader.cpp
   
  \brief A class that models a XML reader object built on top of Xerces-C++.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "ErrorHandler.h"
#include "Exception.h"
#include "Reader.h"
#include "ReaderHandler.h"
#include "StrToXMLCh.h"
#include "Utils.h"

// Xerces-C++
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

te::xerces::Reader::Reader()
  : m_parser(0),
    m_readerH(0),
    m_errH(0),
    m_token(0),
    m_ignoreWhiteSpaces(false)
{
  m_parser = xercesc::XMLReaderFactory::createXMLReader();
  m_readerH = new ReaderHandler;
  m_errH = new ErrorHandler;

  if(m_parser)
  {
    m_parser->setContentHandler(m_readerH);
    m_parser->setErrorHandler(m_errH);
    m_parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);
    m_parser->setFeature(xercesc::XMLUni::fgXercesSchema, true);
    m_parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
    m_parser->setFeature(xercesc::XMLUni::fgXercesUseCachedGrammarInParse, true);
    m_parser->setFeature(xercesc::XMLUni::fgXercesCacheGrammarFromParse, true);
    m_parser->setInputBufferSize(TE_XERCES_READER_MAX_BUFFSIZE);
  }
}

te::xerces::Reader::~Reader()
{
  reset();

  delete m_parser;
  delete m_readerH;
  delete m_errH;
  delete m_token;
}

void te::xerces::Reader::setDoNamespaces(bool d)
{
  m_parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, d);
}

void te::xerces::Reader::setDoSchema(bool d)
{
  m_parser->setFeature(xercesc::XMLUni::fgXercesSchema, d);
}

void te::xerces::Reader::setValidationScheme(bool d)
{
  m_parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, d);
}

void te::xerces::Reader::setUseCachedGrammarInParse(bool d)
{
  m_parser->setFeature(xercesc::XMLUni::fgXercesUseCachedGrammarInParse, d);
}

void te::xerces::Reader::setCacheGrammarFromParse(bool d)
{
  m_parser->setFeature(xercesc::XMLUni::fgXercesCacheGrammarFromParse, d);
}

void te::xerces::Reader::setIgnoreWhiteSpaces(bool d)
{
  m_ignoreWhiteSpaces = d;
}

void te::xerces::Reader::read(const std::string& fileURI)
{
// reset parser if it is in use! 
  if(m_token)
    reset();

// create a new state for the progressive parser
  delete m_token;
  m_token = new xercesc::XMLPScanToken;

  try
  {
    if(!m_parser->parseFirst(fileURI.c_str(), *m_token))
    {
      ErrorHandler* errH = static_cast<ErrorHandler*>(m_parser->getErrorHandler());

      std::string m  = TE_TR("Could not start reading the file due to the following problem: ");
                  m += errH->getErrors();
                  m += ".";

      throw Exception(m);
    }

    if(m_readerH->getNodeType() ==  te::xml::UNKNOWN)
    {
      next();
    }
  }
  catch(const xercesc::XMLException& e)
  {
    std::string m  = TE_TR("Error when reading the XML document. Exception messages is: ");
                m += ToString(e.getMessage());

    throw Exception(m);
  }
  catch(...)
  {
    throw Exception(TE_TR("Unexpected error reading the XML document!"));
  }
}

bool te::xerces::Reader::next()
{
  assert(m_token);

  m_readerH->reset();

  bool parserStatus = true;

  try
  {
    while(true && parserStatus)
    {
      if(m_readerH->isInContractedForm())
      {
        m_readerH->setNodeType(te::xml::END_ELEMENT);
        m_readerH->setInContractedForm(false);
        return true;
      }
      else
        parserStatus = m_parser->parseNext(*m_token);

      if(m_parser->getErrorCount() != 0)
      {
        std::string errmsg = m_errH->getErrors();
        throw Exception(errmsg);
      }

      if(m_readerH->isInContractedForm())
      {
        m_readerH->setNodeType(te::xml::START_ELEMENT);
      }

      if(m_ignoreWhiteSpaces && getNodeType() == te::xml::WHITESPACE)
        continue;

      if(m_readerH->getNodeType() !=  te::xml::UNKNOWN)
        return true;
    }
  }
  catch(const xercesc::XMLException& e)
  {
    std::string m  = TE_TR("Error parsing the XML document. Exception messages is: ");
                m += ToString(e.getMessage());

    throw Exception(m);
  }
  catch(const std::exception& /*e*/)
  {
    throw;
  }
  catch(...)
  {
    throw Exception(TE_TR("Unexpected error in parse next!"));
  }

  return false;
}

te::xml::NodeType te::xerces::Reader::getNodeType() const
{
  return m_readerH->getNodeType();
}

std::string te::xerces::Reader::getElementURI() const
{
  return ToString(m_readerH->getElementURI());
}

std::string te::xerces::Reader::getElementLocalName() const
{
  return ToString(m_readerH->getElementLocalName());
}

std::string te::xerces::Reader::getElementQName() const
{
  return ToString(m_readerH->getElementQName());
}

std::string te::xerces::Reader::getElementValue() const
{
  return ToString(m_readerH->getElementValue());
}

std::size_t te::xerces::Reader::getElementDataLen() const
{
  return m_readerH->getDataLen();
}

bool te::xerces::Reader::hasAttrs() const
{
  return (m_readerH->getElementAttrs() != 0) && (m_readerH->getElementAttrs()->getLength() > 0);
}

std::size_t te::xerces::Reader::getNumberOfAttrs() const
{
  return (hasAttrs() ? m_readerH->getElementAttrs()->getLength() : 0);
}

std::string te::xerces::Reader::getAttr(const std::string& name) const
{
  assert(m_readerH->getElementAttrs());

  const XMLCh* value = m_readerH->getElementAttrs()->getValue(StrToXMLCh(name).getXMLCh());

  return ToString(value);  
}

std::string te::xerces::Reader::getAttr(std::size_t i) const
{
  assert(m_readerH->getElementAttrs());
  assert(i < m_readerH->getElementAttrs()->getLength());

  const XMLCh* value = m_readerH->getElementAttrs()->getValue(i);

  return ToString(value);  
}

std::string te::xerces::Reader::getAttrLocalName(std::size_t i) const
{
  assert(m_readerH->getElementAttrs());
  assert(i < m_readerH->getElementAttrs()->getLength());

  const XMLCh* name = m_readerH->getElementAttrs()->getLocalName(i);

  return ToString(name);
}

std::string te::xerces::Reader::getAttrQName(std::size_t i) const
{
  assert(m_readerH->getElementAttrs());
  assert(m_readerH->getElementAttrs()->getLength() > i);

  const XMLCh* name = m_readerH->getElementAttrs()->getQName(i);

  return ToString(name);
}

std::string te::xerces::Reader::getAttrURI(std::size_t i) const
{
  assert(m_readerH->getElementAttrs());
  assert(m_readerH->getElementAttrs()->getLength() > i);

  const XMLCh* name = m_readerH->getElementAttrs()->getURI(i);

  return ToString(name);
}

std::size_t te::xerces::Reader::getAttrPosition(const std::string& name) const
{
  assert(m_readerH->getElementAttrs());

  return m_readerH->getElementAttrs()->getIndex(StrToXMLCh(name).getXMLCh());
}

std::size_t te::xerces::Reader::getNumberOfNamespaces() const
{
  return m_readerH->getNumberOfNamespaces();
}

void te::xerces::Reader::getNamespace(std::size_t i, std::pair<std::string, std::string>& ns) const
{
  const std::pair<const XMLCh*, const XMLCh*>& nns = m_readerH->getNamespace(i);

  ns.first = ToString(nns.first);
  ns.second = ToString(nns.second);
}

void te::xerces::Reader::setInternalBufferSize(const std::size_t size)
{
  m_parser->setInputBufferSize(size);
}

void te::xerces::Reader::reset()
{
  if(m_token == 0)
    return;

  try
  {
    m_parser->parseReset(*m_token);
  }
  catch(const xercesc::XMLException& e)
  {
    std::string m  = TE_TR("Error reseting the parser. Exception messages is: ");
                m += ToString(e.getMessage());

    throw Exception(m);
  }
  catch(...)
  {
    throw Exception(TE_TR("Unexpected error reseting the parser!"));
  }
}

