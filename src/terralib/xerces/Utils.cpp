/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Utils.cpp
   
  \brief This file contains several utilities functions for dealing with Xerces in TerraLib.
*/

// TerraLib
#include "../common/Translator.h"
#include "ErrorHandler.h"
#include "Exception.h"
#include "Utils.h"

// Xerces-C++ 
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/XMLException.hpp>

// STL 
#include <cassert>
#include <memory>

xercesc::DOMDocument* te::xerces::readXML(const std::string& xmlFile,
                                       const bool doNamespace,
                                       const bool doSchema,
                                       const bool valid)
{
  assert(xmlFile.empty() == false);

  std::auto_ptr<xercesc::XercesDOMParser> parser(new xercesc::XercesDOMParser());
  std::auto_ptr<ErrorHandler> errHandler(new ErrorHandler);
  parser->setErrorHandler(errHandler.get());

  xercesc::XercesDOMParser::ValSchemes valScheme = valid ? xercesc::XercesDOMParser::Val_Always : xercesc::XercesDOMParser::Val_Never;

  //parser->setErrorHandler(errH);  
  
  parser->setDoNamespaces(doNamespace);
  parser->setDoSchema(doSchema);
  parser->setValidationScheme(valScheme);
  parser->cacheGrammarFromParse(true);
  //parser->setIncludeIgnorableWhitespace(false);

  try
  {
    parser->parse(xmlFile.c_str());
  }
  catch(const xercesc::SAXException& e)
  {
    std::string m =  TE_TR("Could not read the XML file: ");
                m += xmlFile;
                m += TE_TR(", due to the following error: ");
                m += ToString(e.getMessage());

    throw Exception(m);
  }
  catch(const xercesc::DOMException& e)
  {
    std::string m =  TE_TR("Could not read the XML file: ");
                m += xmlFile;
                m += TE_TR(", due to the following error: ");
                m += ToString(e.getMessage());

    throw Exception(m);
  }
  catch(const xercesc::XMLException& e)
  {
    std::string m =  TE_TR("Could not read the XML file: ");
                m += xmlFile;
                m += TE_TR(", due to the following error: ");
                m += ToString(e.getMessage());

    throw Exception(m);
  }
  
  if(parser->getErrorCount() > 0)
  {
    std::string m =  TE_TR("Could not read the XML file: ");
                m += xmlFile;
                m += ", due to the following problem: ";
                m += errHandler->getErrors();

    throw Exception(m);
  }  

// take the ownershipo of the document and release the parser
  xercesc::DOMDocument* doc = parser->adoptDocument();

  if(doc == 0)
    throw Exception(TE_TR("Unable to read the XML document!"));

  return doc;
}
