/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file ComplexType.cpp

  \brief It models a XML Schema Complex Type definition.
*/

// TerraLib
#include "AbstractAttribute.h"
#include "AnyAttribute.h"
#include "ComplexContent.h"
#include "ComplexType.h"
#include "Content.h"
#include "SimpleContent.h"

te::xsd::ComplexType::ComplexType(Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_name(0),
    m_abstract(false),
    m_mixed(false),
    m_block(UNKNOWN),
    m_final(UNKNOWN),
    m_sContent(0),
    m_cContent(0),
    m_content(0),
    m_anyAttr(0)
{
}

te::xsd::ComplexType::ComplexType(const ComplexType& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_name(0),
    m_abstract(false),
    m_mixed(false),
    m_block(UNKNOWN),
    m_final(UNKNOWN),
    m_sContent(0),
    m_cContent(0),
    m_content(0),
    m_anyAttr(0)
{
}

te::xsd::ComplexType::~ComplexType()
{
  delete m_name;
  delete m_sContent;
  delete m_cContent;
  delete m_content;
  delete m_anyAttr;
}

te::xsd::ComplexType& te::xsd::ComplexType::operator=(const ComplexType& rhs)
{
  return *this;
}

std::string* te::xsd::ComplexType::getName() const
{
  return m_name;
}

bool te::xsd::ComplexType::isAbstract()
{
  return m_abstract;
}

bool te::xsd::ComplexType::isMixed()
{
  return m_mixed;
}

int te::xsd::ComplexType::getBlock()
{
  return m_block;
}

int te::xsd::ComplexType::getFinal()
{
  return m_final;
}

te::xsd::SimpleContent* te::xsd::ComplexType::getSimpleContent() const
{
  return m_sContent;
}

te::xsd::ComplexContent* te::xsd::ComplexType::getComplexContent() const
{
  return m_cContent;
}

te::xsd::Content* te::xsd::ComplexType::getContent()
{
  return m_content;
}

const boost::ptr_vector<te::xsd::AbstractAttribute>& te::xsd::ComplexType::getAttributes() const
{
  return m_attributeVec;
}

te::xsd::AnyAttribute* te::xsd::ComplexType::getAnyAttribute() const
{
  return m_anyAttr;
}

void te::xsd::ComplexType::setName(std::string* name)
{
  delete m_name;
  m_name = name;
}

void te::xsd::ComplexType::setAsAbstract(bool v)
{
  m_abstract = v;
}

void te::xsd::ComplexType::setAsMixed(bool v)
{
  m_mixed = v;
}

void te::xsd::ComplexType::setBlock(int v)
{
  m_block = v;
}

void te::xsd::ComplexType::setFinal(int v)
{
  m_final = v;
}

void te::xsd::ComplexType::setSimpleContent(te::xsd::SimpleContent* sc)
{
  delete m_sContent;
  m_sContent = sc;

  // Clears mutually exclusive members
  delete m_cContent;
  m_cContent = 0;

  delete m_content;
  m_content = 0;

  m_attributeVec.clear();

  delete m_anyAttr;
  m_anyAttr = 0;
}

void te::xsd::ComplexType::setComplexContent(te::xsd::ComplexContent* cc)
{
  delete m_cContent;
  m_cContent = cc;

  // Clears mutually exclusive members
  delete m_sContent;
  m_sContent = 0;

  delete m_content;
  m_content = 0;

  m_attributeVec.clear();

  delete m_anyAttr;
  m_anyAttr = 0;
}

void te::xsd::ComplexType::setContent(te::xsd::Content* c)
{
  delete m_content;
  m_content = c;

  // Clears mutually exclusive members
  delete m_sContent;
  m_sContent = 0;

  delete m_cContent;
  m_cContent = 0;
}

void te::xsd::ComplexType::addAttribute(te::xsd::AbstractAttribute* a)
{
  // Clears mutually exclusive members
  delete m_sContent;
  m_sContent = 0;

  delete m_cContent;
  m_cContent = 0;

  m_attributeVec.push_back(a);
}

void te::xsd::ComplexType::setAnyAttribute(te::xsd::AnyAttribute* a)
{
  delete m_anyAttr;
  m_anyAttr = a;
  
  // Clears mutually exclusive members
  delete m_sContent;
  m_sContent = 0;

  delete m_cContent;
  m_cContent = 0;
}

te::xsd::Type* te::xsd::ComplexType::clone() const
{
  return new ComplexType(*this);
}
