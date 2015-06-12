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
  \file AttributeGroup.cpp

  \brief It models a XML Schema attribute group.
*/

// TerraLib
#include "AnyAttribute.h"
#include "AttributeGroup.h"
#include "QName.h"

te::xsd::AttributeGroup::AttributeGroup(Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_name(0),
    m_ref(0),
    m_anyAttr(0)
{
}

te::xsd::AttributeGroup::AttributeGroup(const AttributeGroup& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_name(0),
    m_ref(0),
    m_anyAttr(0)
{
  m_name = rhs.m_name ? new std::string(*rhs.m_name) : 0;
  m_ref = rhs.m_ref ? new QName(*rhs.m_ref) : 0;

  for(std::size_t i = 0; i < rhs.m_attributeVec.size(); ++i)
    m_attributeVec.push_back(rhs.m_attributeVec[i].clone());

  m_anyAttr = rhs.m_anyAttr ? new AnyAttribute(*rhs.m_anyAttr) : 0;
}

te::xsd::AttributeGroup::~AttributeGroup()
{
  delete m_name;
  delete m_ref;
  delete m_anyAttr;
}

te::xsd::AttributeGroup& te::xsd::AttributeGroup::operator=(const AttributeGroup& rhs)
{
  if(this != &rhs)
  {
    Identifiable::operator=(rhs);

    Annotated::operator=(rhs);

    delete m_name;

    m_name = rhs.m_name ? new std::string(*rhs.m_name) : 0;

    delete m_ref;

    m_ref = rhs.m_ref ? new QName(*rhs.m_ref) : 0;

    for(std::size_t i = 0; i < rhs.m_attributeVec.size(); ++i)
      m_attributeVec.push_back(rhs.m_attributeVec[i].clone());

    delete m_anyAttr;

    m_anyAttr = rhs.m_anyAttr ? new AnyAttribute(*rhs.m_anyAttr) : 0;
  }

  return *this;
}

std::string* te::xsd::AttributeGroup::getName() const
{
  return m_name;
}

te::xsd::QName* te::xsd::AttributeGroup::getRef() const
{
  return m_ref;
}

te::xsd::AnyAttribute* te::xsd::AttributeGroup::getAnyAttribute() const
{
  return m_anyAttr;
}

void te::xsd::AttributeGroup::setName(std::string* name)
{
  delete m_name;
  m_name = name;

  delete m_ref;
  m_ref = 0;
}

void te::xsd::AttributeGroup::setRef(te::xsd::QName* ref)
{
  delete m_ref;
  m_ref = ref;

  delete m_name;
  m_name = 0;
}

void te::xsd::AttributeGroup::addAttribute(te::xsd::AbstractAttribute* a)
{
  m_attributeVec.push_back(a);
}

void te::xsd::AttributeGroup::setAnyAttribute(te::xsd::AnyAttribute* a)
{
  delete m_anyAttr;
  m_anyAttr = a;
}

te::xsd::AbstractAttribute* te::xsd::AttributeGroup::clone() const
{
  return new AttributeGroup(*this);
}
