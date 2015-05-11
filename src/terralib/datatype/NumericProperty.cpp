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
  \file terralib/datatype/NumericProperty.cpp

  \brief The type for arbitrary precision numbers, like numeric(p, q).
*/

// TerraLib
#include "Enums.h"
#include "NumericProperty.h"

te::dt::NumericProperty::NumericProperty(const std::string& name,
                                         unsigned int precision,
                                         unsigned int scale,
                                         bool isRequired,
                                         std::string* defaultValue,
                                         unsigned int id,
                                         Property* parent)
  : SimpleProperty(name, NUMERIC_TYPE, isRequired, defaultValue, id, parent),
    m_precision(precision),
    m_scale(scale)
{
}

te::dt::NumericProperty::NumericProperty(const NumericProperty& rhs)
  : SimpleProperty(rhs),
    m_precision(rhs.m_precision),
    m_scale(rhs.m_scale)
{
}

te::dt::NumericProperty& te::dt::NumericProperty::operator=(const NumericProperty& rhs)
{
  if(this != &rhs)
  {
    SimpleProperty::operator=(rhs);

    m_precision = rhs.m_precision;

    m_scale = rhs.m_scale;
  }

  return *this;
}

te::dt::Property* te::dt::NumericProperty::clone() const
{
  return new NumericProperty(*this);
}

