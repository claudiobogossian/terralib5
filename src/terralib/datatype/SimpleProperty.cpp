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
  \file terralib/datatype/SimpleProperty.cpp

  \brief An atomic property like an integer or double.
*/

// TerraLib
#include "SimpleProperty.h"

te::dt::SimpleProperty::SimpleProperty(const std::string& name,
                                       int datatype,
                                       bool isRequired,
                                       std::string* defaultValue,
                                       unsigned int id,
                                       Property* parent)
  : Property(name, datatype, id, parent),
    m_isRequired(isRequired),
    m_isAutoNumber(false),
    m_defaultValue(defaultValue)
{
}

te::dt::SimpleProperty::SimpleProperty(const SimpleProperty& rhs)
  : Property(rhs),    
    m_isRequired(rhs.m_isRequired),
    m_isAutoNumber(rhs.m_isAutoNumber),
    m_defaultValue(0)
{
  m_defaultValue = rhs.m_defaultValue ? new std::string(*(rhs.m_defaultValue)) : 0;
}

te::dt::SimpleProperty::~SimpleProperty()
{
  delete m_defaultValue;
}

te::dt::SimpleProperty& te::dt::SimpleProperty::operator=(const SimpleProperty& rhs)
{
  if(this != &rhs)
  {
    Property::operator=(rhs);

    m_isRequired = rhs.m_isRequired;

    m_isAutoNumber = rhs.m_isAutoNumber;

    delete m_defaultValue;

    m_defaultValue = rhs.m_defaultValue ? new std::string(*(rhs.m_defaultValue)) : 0;
  }

  return *this;
}

void te::dt::SimpleProperty::setDefaultValue(std::string* d)
{
  delete m_defaultValue;
  m_defaultValue = d;
}

bool te::dt::SimpleProperty::has(Property* /*p*/) const
{
  return false;
}

te::dt::Property* te::dt::SimpleProperty::clone() const
{
  return new SimpleProperty(*this);
}

