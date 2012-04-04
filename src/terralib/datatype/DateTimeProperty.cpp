/*  Copyright (C) 2010-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/DateTimeProperty.cpp

  \brief The type for date and time types: TIME_STAMP, TIME_STAMP_TZ, DATE, DATETIME, TIME or TIME_TZ.
*/

// TerraLib
#include "DateTimeProperty.h"

te::dt::DateTimeProperty::DateTimeProperty(const std::string& name,
                                           DateTimeType dateType,
                                           DateTimeResolution dtResolution,
                                           bool isRequired,
                                           std::string* defaultValue,
                                           unsigned int id,
                                           Property* parent)
  : SimpleProperty(name, DATETIME_TYPE, isRequired, defaultValue, id, parent),
    m_dSubType(dateType),
    m_dtResolution(dtResolution)
{
}

te::dt::DateTimeProperty::DateTimeProperty(const DateTimeProperty& rhs)
  : SimpleProperty(rhs),
    m_dSubType(rhs.m_dSubType),
    m_dtResolution(rhs.m_dtResolution)
{  
}

te::dt::DateTimeProperty& te::dt::DateTimeProperty::operator=(const DateTimeProperty& rhs)
{
  if(this != &rhs)
  {
    SimpleProperty::operator=(rhs);

    m_dSubType = rhs.m_dSubType;
    m_dtResolution = rhs.m_dtResolution;
  }

  return *this;
}

te::dt::Property* te::dt::DateTimeProperty::clone() const
{
  return new DateTimeProperty(*this);
}

