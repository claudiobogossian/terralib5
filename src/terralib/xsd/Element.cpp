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
  \file Element.cpp

  \brief This class models the element of a XML Schema.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "Element.h"
#include "Enums.h"
#include "IdentityConstraint.h"
#include "Type.h"
#include "QName.h"

te::xsd::Element::Element(unsigned int minOccurs, unsigned int maxOccurs, Annotation* ann, std::string* id)
  : Occurs(minOccurs, maxOccurs),
    Identifiable(id),
    Annotated(ann),
    m_name(0),
    m_ref(0),
    m_type(0),
    m_substitutionGroup(0),
    m_default(0),
    m_fixed(0),
    m_form(Qualified),
    m_nillable(false),
    m_abstract(false),
    m_block(UNKNOWN),
    m_final(UNKNOWN),
    m_contentType(0)
{
}

te::xsd::Element::Element(const Element& rhs)
  : Occurs(rhs),
    Identifiable(rhs),
    Annotated(rhs),
    m_name(0),
    m_ref(0),
    m_type(0),
    m_substitutionGroup(0),
    m_default(0),
    m_fixed(0),
    m_form(Qualified),
    m_nillable(false),
    m_abstract(false),
    m_block(UNKNOWN),
    m_final(UNKNOWN),
    m_contentType(0)
{
}

te::xsd::Element::~Element()
{
  delete m_type;
  delete m_default;
  delete m_fixed;
  delete m_contentType;
}

te::xsd::Element& te::xsd::Element::operator=(const Element& rhs)
{
  return *this;
}

te::xsd::QName* te::xsd::Element::getType() const
{
  return m_type;
}

std::string* te::xsd::Element::getDefaultValue() const
{
  return m_default;
}

std::string* te::xsd::Element::getFixedValue() const
{
  return m_fixed;
}

bool te::xsd::Element::isNillable()
{
  return m_nillable;
}

bool te::xsd::Element::isAbstract()
{
  return m_abstract;
}

int te::xsd::Element::getBlock()
{
  return m_block;
}

void te::xsd::Element::setType(te::xsd::QName* t)
{
  delete m_type;
  m_type = t;
}

void te::xsd::Element::setDefaultValue(std::string* v)
{
  delete m_default;
  m_default = v;
}

void te::xsd::Element::setFixedValue(std::string* v)
{
  delete m_fixed;
  m_fixed = v;
}

void te::xsd::Element::setAsNillable(bool v)
{
  m_nillable = v;
}

void te::xsd::Element::setAsAbstract(bool v)
{
  m_abstract = v;
}

void te::xsd::Element::setBlock(int v)
{
  m_block = v;
}

std::string* te::xsd::Element::getName() const
{
  return m_name;
}

te::xsd::QName* te::xsd::Element::getRef() const
{
  return m_ref;
}

void te::xsd::Element::setName(std::string* name)
{
  delete m_name;
  m_name = name;

  delete m_ref;
  m_ref = 0;
}

void te::xsd::Element::setRef(te::xsd::QName* ref)
{
  delete m_ref;
  m_ref = ref;

  delete m_name;
  m_name = 0;
}


void te::xsd::Element::setSubstitutionGroup(te::xsd::QName* g)
{
  delete m_substitutionGroup;
  m_substitutionGroup = g;
}

void te::xsd::Element::setFinal(int v)
{
  m_final = v;
}

void te::xsd::Element::setContentType(te::xsd::Type* t)
{
  delete m_contentType;
  m_contentType = t;
}

void te::xsd::Element::addIdentityConstraint(IdentityConstraint* ic)
{
  m_identityConstraintVec.push_back(ic);
}
