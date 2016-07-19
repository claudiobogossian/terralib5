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
  \file Restriction4ComplexContent.cpp

  \brief The restriction class can be used to define restrictions on a complexContent.
*/

// TerraLib
#include "AbstractAttribute.h"
#include "AnyAttribute.h"
#include "Content.h"
#include "Restriction4ComplexContent.h"
#include "QName.h"

// STL
#include <cassert>

te::xsd::Restriction4ComplexContent::Restriction4ComplexContent(std::string* id, QName* base)
  : Identifiable(id),
    Annotated(0),
    m_base(base),
    m_content(0),
    m_anyAttr(0)
{
  assert(base);
}

te::xsd::Restriction4ComplexContent::~Restriction4ComplexContent()
{
  delete m_base;
  delete m_content;
  delete m_anyAttr;
}

te::xsd::QName* te::xsd::Restriction4ComplexContent::getBase() const
{
  return m_base;
}

te::xsd::Content* te::xsd::Restriction4ComplexContent::getContent() const
{
  return m_content;
}

te::xsd::AnyAttribute* te::xsd::Restriction4ComplexContent::getAnyAttribute() const
{
  return m_anyAttr;
}

void te::xsd::Restriction4ComplexContent::setBase(te::xsd::QName* base)
{
  delete m_base;
  m_base = base;
}

void te::xsd::Restriction4ComplexContent::setContent(te::xsd::Content* c)
{
  delete m_content;
  m_content = c;
}

void te::xsd::Restriction4ComplexContent::addAttribute(te::xsd::AbstractAttribute* a)
{
  m_attributeVec.push_back(a);
}

void te::xsd::Restriction4ComplexContent::setAnyAttribute(te::xsd::AnyAttribute* a)
{
  delete m_anyAttr;
  m_anyAttr = a;
}
