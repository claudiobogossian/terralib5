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
  \file ReaderHandler.cpp
   
  \brief This class implements Xerces interface for a handler that receives general document events.
 */

// TerraLib
#include "ReaderHandler.h"

// Xerces-C++
#include <xercesc/sax/Locator.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLChar.hpp>
#include <xercesc/util/XMLString.hpp>

te::xerces::ReaderHandler::ReaderHandler()
  : m_uri(0),
    m_localname(0),
    m_qname(0),
    m_attrs(0),
    m_value(0),
    m_len(0),
    /*m_locator(0),*/
    m_nodeType(te::xml::UNKNOWN),
    m_isInContractedForm(false)
{
}

void te::xerces::ReaderHandler::reset()
{
  m_nodeType = te::xml::UNKNOWN;
  m_nspaces.clear();
}

void te::xerces::ReaderHandler::characters(const XMLCh* const chars, const XMLSize_t length)
{
  xercesc_3_1::XMLChar1_0::isAllSpaces(chars, length) ? m_nodeType = te::xml::WHITESPACE : m_nodeType = te::xml::VALUE;
  m_value = chars;
  m_len = length;
}

void te::xerces::ReaderHandler::endDocument()
{
  m_nodeType = te::xml::END_DOCUMENT;
}

void te::xerces::ReaderHandler::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
{
  if(m_nodeType == te::xml::START_ELEMENT)
  {
    m_isInContractedForm = true;
    return;
  }

  m_nodeType = te::xml::END_ELEMENT;
  m_uri = uri;
  m_localname = localname;
  m_qname = qname;
}

void te::xerces::ReaderHandler::ignorableWhitespace(const XMLCh* const /*chars*/, const XMLSize_t /*length*/)
{
  m_nodeType = te::xml::UNKNOWN;
  //m_value = chars;
  //m_len = length;
}

void te::xerces::ReaderHandler::processingInstruction(const XMLCh* const target, const XMLCh* const data)
{
  m_nodeType = te::xml::UNKNOWN;
}

void te::xerces::ReaderHandler::setDocumentLocator(const xercesc::Locator* const /*locator*/)
{
  //m_locator = locator;
}

void te::xerces::ReaderHandler::startDocument()
{
  m_nodeType = te::xml::START_DOCUMENT;
}

void te::xerces::ReaderHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const xercesc::Attributes& attrs)
{
  m_nodeType = te::xml::START_ELEMENT;
  m_uri = uri;
  m_localname = localname;
  m_qname = qname;
  m_attrs = &attrs;
}

void te::xerces::ReaderHandler::startPrefixMapping(const XMLCh* const prefix, const XMLCh* const uri)
{
  m_nodeType = te::xml::UNKNOWN;

  m_nspaces.push_back(std::pair<const XMLCh*, const XMLCh*>(prefix, uri));
}

void te::xerces::ReaderHandler::endPrefixMapping(const XMLCh* const prefix)
{
  m_nodeType = te::xml::UNKNOWN;
}

void te::xerces::ReaderHandler::skippedEntity(const XMLCh* const /*name*/)
{
  m_nodeType = te::xml::UNKNOWN;
}

te::xml::NodeType te::xerces::ReaderHandler::getNodeType() const
{
  return m_nodeType;
}

void te::xerces::ReaderHandler::setNodeType(te::xml::NodeType type)
{
  m_nodeType = type;
}

bool te::xerces::ReaderHandler::isInContractedForm() const
{
  return m_isInContractedForm;
}

void te::xerces::ReaderHandler::setInContractedForm(bool d)
{
  m_isInContractedForm = d;
}
