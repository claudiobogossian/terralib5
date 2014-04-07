/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/StringProperty.cpp

  \brief The type for string types: FIXED_STRING, VAR_STRING or STRING.
*/

// TerraLib
#include "StringProperty.h"

// STL
#include <cassert>

te::dt::StringProperty::StringProperty(const std::string& name,
                                       StringType strType,
                                       std::size_t size,
                                       bool isRequired,
                                       std::string* defaultValue,
                                       unsigned int id,
                                       Property* parent)
  : SimpleProperty(name, STRING_TYPE, isRequired, defaultValue, id, parent),
    m_size(size),
    m_strSubType(strType),
    m_ce(te::common::UNKNOWN_CHAR_ENCODING)
{
}

te::dt::StringProperty::StringProperty(const StringProperty& rhs)
  : SimpleProperty(rhs),
    m_size(rhs.m_size),
    m_strSubType(rhs.m_strSubType)
{  
}

te::dt::StringProperty& te::dt::StringProperty::operator=(const StringProperty& rhs)
{
  if(this != &rhs)
  {
    SimpleProperty::operator=(rhs);

    m_size = rhs.m_size;

    m_strSubType = rhs.m_strSubType;
  }

  return *this;
}

te::dt::Property* te::dt::StringProperty::clone() const
{
  return new StringProperty(*this);
}

