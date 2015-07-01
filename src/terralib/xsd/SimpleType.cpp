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
  \file SimpleType.cpp

  \brief It models a XML Schema SimpleType element.
*/

// TerraLib
#include "SimpleType.h"
#include "SimpleTypeConstructor.h"

te::xsd::SimpleType::SimpleType(Annotation* ann, std::string* id)
  : Identifiable(id),
    Annotated(ann),
    m_name(0),
    m_constructor(0)
{
}

te::xsd::SimpleType::SimpleType(const SimpleType& rhs)
  : Identifiable(rhs),
    Annotated(rhs),
    m_name(0),
    m_constructor(0)
{
  m_name = rhs.m_name ? new std::string(*rhs.m_name) : 0;

  m_constructor = rhs.m_constructor ? rhs.m_constructor->clone() : 0;
}

te::xsd::SimpleType::~SimpleType()
{
  delete m_name;
  delete m_constructor;
}

te::xsd::SimpleType& te::xsd::SimpleType::operator=(const SimpleType& rhs)
{
  if(this != &rhs)
  {
    Identifiable::operator=(rhs);

    Annotated::operator=(rhs);

    delete m_name;

    m_name = rhs.m_name ? new std::string(*rhs.m_name) : 0;

    delete m_constructor;

    m_constructor = rhs.m_constructor ? rhs.m_constructor->clone() : 0;
  }

  return *this;
}

std::string* te::xsd::SimpleType::getName() const
{
  return m_name;
}

te::xsd::SimpleTypeConstructor* te::xsd::SimpleType::getConstructor() const
{
  return m_constructor;
}

void te::xsd::SimpleType::setName(std::string* name)
{
  delete m_name;
  m_name = name;
}

void te::xsd::SimpleType::setConstructor(te::xsd::SimpleTypeConstructor* c)
{
  delete m_constructor;
  m_constructor = c;
}

te::xsd::Type* te::xsd::SimpleType::clone() const
{
  return new SimpleType(*this);
}
