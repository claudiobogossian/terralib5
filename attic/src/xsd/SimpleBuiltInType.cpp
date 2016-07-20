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

// TerraLib
#include "SimpleBuiltInType.h"

std::string te::xsd::SimpleBuiltInType::m_typeNames[] = {std::string("TODO")};

te::xsd::SimpleBuiltInType::SimpleBuiltInType(BuiltInType type)
  : m_type(type)
{
}

te::xsd::SimpleBuiltInType::SimpleBuiltInType(const SimpleBuiltInType& rhs)
  : m_type(rhs.m_type)
{
}

te::xsd::SimpleBuiltInType::~SimpleBuiltInType()
{
}

te::xsd::SimpleBuiltInType& te::xsd::SimpleBuiltInType::operator=(const SimpleBuiltInType& rhs)
{
  if(this != &rhs)
  {
    m_type = rhs.m_type;
  }

  return *this;
}

te::xsd::BuiltInType te::xsd::SimpleBuiltInType::getType() const
{
  return m_type;
}

const std::string& te::xsd::SimpleBuiltInType::getName() const
{
  return m_typeNames[m_type];
}

te::xsd::Type* te::xsd::SimpleBuiltInType::clone() const
{
  return new SimpleBuiltInType(*this);
}
