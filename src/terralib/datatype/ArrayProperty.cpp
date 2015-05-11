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
  \file terralib/datatype/ArrayProperty.cpp

  \brief The type for variable-length multidimensional arrays.
*/

// TerraLib
#include "ArrayProperty.h"
#include "Enums.h"

// STL
#include <cassert>

te::dt::ArrayProperty::ArrayProperty(const std::string& name,
                                     Property* elementType,
                                     bool isRequired,
                                     std::string* defaultValue,
                                     unsigned int id,
                                     Property* parent)
  : SimpleProperty(name, ARRAY_TYPE, isRequired, defaultValue, id, parent),
    m_elementType(elementType)
{
  if(m_elementType)
    m_elementType->setParent(this);
}

te::dt::ArrayProperty::ArrayProperty(const ArrayProperty& rhs)
  : SimpleProperty(rhs),
    m_elementType(0)
{
  if(rhs.m_elementType)
  {
    m_elementType = rhs.m_elementType->clone();
    m_elementType->setParent(this);
  }
}

te::dt::ArrayProperty::~ArrayProperty()
{
  delete m_elementType;
}

te::dt::ArrayProperty& te::dt::ArrayProperty::operator=(const ArrayProperty& rhs)
{
  if(this != &rhs)
  {
    SimpleProperty::operator=(rhs);

    delete m_elementType;

    m_elementType = 0;

    if(rhs.m_elementType)
    {
      m_elementType = rhs.m_elementType->clone();

      m_elementType->setParent(this);
    }
  }

  return *this;
}

void te::dt::ArrayProperty::setElementType(Property* t)
{
  assert(t);

  delete m_elementType;
  m_elementType = t;
  m_elementType->setParent(this);
}

te::dt::Property* te::dt::ArrayProperty::clone() const
{
  return new ArrayProperty(*this);
}

