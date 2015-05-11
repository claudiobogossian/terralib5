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
  \file Attribute.cpp

  \brief It models a XML Schema attribute.
*/

// TerraLib
#include "Attribute.h"
#include "QName.h"
#include "SimpleType.h"

te::xsd::Attribute::Attribute(Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_default(0),
    m_fixed(0),
    m_form(Unqualified),
    m_name(0),
    m_ref(0),
    m_type(0),
    m_use(OPTIONAL),
    m_innerType(0)/*,
    m_otherAttributes(0)*/
{
}

te::xsd::Attribute::Attribute(const Attribute& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_default(0),
    m_fixed(0),
    m_form(rhs.m_form),
    m_name(0),
    m_ref(0),
    m_type(0),
    m_use(rhs.m_use),
    m_innerType(0)
{
  m_default = rhs.m_default ? new std::string(*rhs.m_default) : 0;
  m_fixed = rhs.m_fixed ? new std::string(*rhs.m_fixed) : 0;
  m_name = rhs.m_name ? new std::string(*rhs.m_name) : 0;
  m_ref = rhs.m_ref ? new QName(*rhs.m_ref) : 0;
  m_type = rhs.m_type ? new QName(*rhs.m_type) : 0;
  m_innerType = rhs.m_innerType ? new SimpleType(*rhs.m_innerType) : 0;
}

te::xsd::Attribute::~Attribute()
{
  delete m_default;
  delete m_fixed;
  delete m_name;
  delete m_ref;
  delete m_type;
  delete m_innerType;
  //delete m_otherAttributes;
}

te::xsd::Attribute& te::xsd::Attribute::operator=(const Attribute& rhs)
{
  if(this != &rhs)
  {
    Identifiable::operator=(rhs);
    
    Annotated::operator=(rhs);
    
    delete m_default;

    m_default = rhs.m_default ? new std::string(*rhs.m_default) : 0;

    delete m_fixed;

    m_fixed = rhs.m_fixed ? new std::string(*rhs.m_fixed) : 0;

    m_form = rhs.m_form;

    delete m_name;

    m_name = rhs.m_name ? new std::string(*rhs.m_name) : 0;

    delete m_ref;

    m_ref = rhs.m_ref ? new QName(*rhs.m_ref) : 0;

    delete m_type;

    m_type = rhs.m_type ? new QName(*rhs.m_type) : 0;

    m_use = rhs.m_use;

    delete m_innerType;

    m_innerType = rhs.m_innerType ? new SimpleType(*rhs.m_innerType) : 0;  
  }

  return *this;
}


std::string* te::xsd::Attribute::getDefault() const
{
  return m_default;
}

std::string* te::xsd::Attribute::getFixed() const
{
  return m_fixed;
}

te::xsd::Form te::xsd::Attribute::getForm() const
{
  return m_form;
}

std::string* te::xsd::Attribute::getName() const
{
  return m_name;
}

te::xsd::QName* te::xsd::Attribute::getRef() const
{
  return m_ref;
}

te::xsd::QName* te::xsd::Attribute::getType() const
{
  return m_type;
}

te::xsd::AttributeUse te::xsd::Attribute::getUse() const
{
  return m_use;
}

te::xsd::SimpleType* te::xsd::Attribute::getInnerType() const
{
  return m_innerType;
}

//std::map<std::string, std::string>* te::xsd::Attribute::getOtherAttributes() const
//{
//  return m_otherAttributes;
//}

void te::xsd::Attribute::setDefault(std::string* def)
{
  delete m_default;
  m_default = def;

  delete m_fixed;
  m_fixed = 0;
}

void te::xsd::Attribute::setFixed(std::string* fix)
{
  delete m_fixed;
  m_fixed = fix;

  delete m_default;
  m_default = 0;
}

void te::xsd::Attribute::setForm(te::xsd::Form f)
{
  m_form = f;
}

void te::xsd::Attribute::setName(std::string* name)
{
  delete m_name;
  m_name = name;

  delete m_ref;
  m_ref = 0;
}

void te::xsd::Attribute::setRef(te::xsd::QName* ref)
{
  delete m_ref;
  m_ref = ref;

  delete m_name;
  m_name = 0;

  delete m_type;
  m_type = 0;

  delete m_innerType;
  m_innerType = 0;
}

void te::xsd::Attribute::setType(te::xsd::QName* type)
{
  delete m_type;
  m_type = type;

  delete m_ref;
  m_ref = 0;

  delete m_innerType;
  m_innerType = 0;
}

void te::xsd::Attribute::setUse(te::xsd::AttributeUse use)
{
  m_use = use;
}

void te::xsd::Attribute::setInnerType(te::xsd::SimpleType* iType)
{
  delete m_innerType;
  m_innerType = iType;

  delete m_ref;
  m_ref = 0;

  delete m_type;
  m_type = 0;
}

//void te::xsd::Attribute::addOtherAttribute(std::string key, std::string value)
//{
//  if(m_otherAttributes == 0)
//    m_otherAttributes = new std::map<std::string, std::string>;
//
//  m_otherAttributes->insert(std::pair<std::string, std::string>(key, value));
//}

te::xsd::AbstractAttribute* te::xsd::Attribute::clone() const
{
  return new Attribute(*this);
}



