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
  \file terralib/datatype/ByteArrayProperty.cpp

  \brief Property for byte array types.
*/

// TerraLib
#include "ByteArrayProperty.h"
#include "Enums.h"

te::dt::ByteArrayProperty::ByteArrayProperty(const std::string& name,
                                             std::size_t nbytes,
                                             bool isRequired,
                                             std::string* defaultValue,
                                             unsigned int id,
                                             Property* parent)
  : SimpleProperty(name, BYTE_ARRAY_TYPE, isRequired, defaultValue, id, parent),
    m_nbytes(nbytes)
{
}

te::dt::ByteArrayProperty::ByteArrayProperty(const ByteArrayProperty& rhs)
  : SimpleProperty(rhs),
    m_nbytes(rhs.m_nbytes)
{  
}

te::dt::ByteArrayProperty& te::dt::ByteArrayProperty::operator=(const ByteArrayProperty& rhs)
{
  if(this != &rhs)
  {
    SimpleProperty::operator=(rhs);

    m_nbytes = rhs.m_nbytes;
  }

  return *this;
}

te::dt::Property* te::dt::ByteArrayProperty::clone() const
{
  return new ByteArrayProperty(*this);
}

