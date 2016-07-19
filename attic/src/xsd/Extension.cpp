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
  \file Extension.cpp

  \brief This class models an extension element that can be used to extend an existing simpleType or complexType element.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "AbstractAttribute.h"
#include "AnyAttribute.h"
#include "Content.h"
#include "Extension.h"
#include "QName.h"

// STL
#include <cassert>

te::xsd::Extension::Extension(QName* base, Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_base(base),
    m_content(0),
    m_anyAttr(0)
{
}

te::xsd::Extension::Extension(const Extension& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_base(0),
    m_content(0),
    m_anyAttr(0)
{
}

te::xsd::Extension::~Extension()
{
  delete m_base;
  delete m_content;
  delete m_anyAttr;
}

te::xsd::Extension& te::xsd::Extension::operator=(const Extension& rhs)
{
  return *this;
}

te::xsd::QName* te::xsd::Extension::getBase() const
{
  return m_base;
}

te::xsd::Content* te::xsd::Extension::getContent() const
{
  return m_content;
}

const boost::ptr_vector<te::xsd::AbstractAttribute>& te::xsd::Extension::getAttributes() const
{
  return m_attributeVec;
}

te::xsd::AnyAttribute* te::xsd::Extension::getAnyAttribute() const
{
  return m_anyAttr;
}

void te::xsd::Extension::setBase(te::xsd::QName* base)
{
  delete m_base;
  m_base = base;
}

void te::xsd::Extension::setContent(te::xsd::Content* c)
{
  delete m_content;
  m_content = c;
}

void te::xsd::Extension::addAttribute(te::xsd::AbstractAttribute* a)
{
  m_attributeVec.push_back(a);
}

void te::xsd::Extension::setAnyAttribute(te::xsd::AnyAttribute* a)
{
  delete m_anyAttr;
  m_anyAttr = a;
}
