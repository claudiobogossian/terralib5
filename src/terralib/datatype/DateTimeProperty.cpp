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
  \file terralib/datatype/DateTimeProperty.cpp

  \brief The type for date and time types: TIME_STAMP, TIME_STAMP_TZ, DATE, DATETIME, TIME or TIME_TZ.
*/

// TerraLib
#include "DateTimeProperty.h"

te::dt::DateTimeProperty::DateTimeProperty(const std::string& name,
                                           DateTimeType dateType,
                                           bool isRequired,
                                           std::string* defaultValue,
                                           unsigned int id,
                                           Property* parent)
  : SimpleProperty(name, DATETIME_TYPE, isRequired, defaultValue, id, parent),
    m_dSubType(dateType),
    m_tempResolution(te::dt::TemporalResolution()),
    m_strFormat(UNKNOWN_STRING_FORMAT),
    m_ordinalType(USER_DEFINED),
    m_ordinalStartTime(te::dt::TimeInstant()) 
{
}

te::dt::DateTimeProperty::DateTimeProperty(const std::string& name,
                                           DateTimeType dateType,
                                           TemporalResolution tmpRes,
                                           bool isRequired,
                                           std::string* defaultValue,
                                           unsigned int id,
                                           Property* parent)
  : SimpleProperty(name, DATETIME_TYPE, isRequired, defaultValue, id, parent),
    m_dSubType(dateType),
    m_tempResolution(tmpRes),
    m_strFormat(UNKNOWN_STRING_FORMAT),
    m_ordinalType(USER_DEFINED),
    m_ordinalStartTime(te::dt::TimeInstant()) 
{
}


te::dt::DateTimeProperty::DateTimeProperty(const std::string& name,
                                           TemporalResolution tmpRes,
                                           DateTimeStringFormat strFormat,
                                           bool isRequired,
                                           std::string* defaultValue,
                                           unsigned int id,
                                           Property* parent)
  : SimpleProperty(name, STRING_TYPE, isRequired, defaultValue, id, parent),
    m_dSubType(STRING_TIME),
    m_tempResolution(tmpRes),
    m_strFormat(strFormat),
    m_ordinalType(USER_DEFINED),
    m_ordinalStartTime(te::dt::TimeInstant()) 
{
}

te::dt::DateTimeProperty::DateTimeProperty(const std::string& name,
                                           TemporalResolution tmpRes,
                                           DateTimeOrdinalType ordType,
                                           TimeInstant startTime,
                                           bool isRequired,
                                           std::string* defaultValue,
                                           unsigned int id,
                                           Property* parent)
  : SimpleProperty(name, UINT64_TYPE, isRequired, defaultValue, id, parent),
    m_dSubType(ORDINAL_TIME_INSTANT),  
    m_tempResolution(tmpRes),
    m_strFormat(UNKNOWN_STRING_FORMAT),
    m_ordinalType(ordType),
    m_ordinalStartTime(startTime) 
{
}

te::dt::DateTimeProperty::DateTimeProperty(const DateTimeProperty& rhs)
  : SimpleProperty(rhs),
    m_dSubType(rhs.m_dSubType),
    m_tempResolution(rhs.m_tempResolution),
    m_strFormat(rhs.m_strFormat),
    m_ordinalType(rhs.m_ordinalType),
    m_ordinalStartTime(rhs.m_ordinalStartTime) 
{  
}

te::dt::DateTimeProperty& te::dt::DateTimeProperty::operator=(const DateTimeProperty& rhs)
{
  if(this != &rhs)
  {
    SimpleProperty::operator=(rhs);

    m_dSubType = rhs.m_dSubType;
    m_tempResolution = rhs.m_tempResolution;
    m_strFormat = rhs.m_strFormat;
    m_ordinalType = rhs.m_ordinalType;
    m_ordinalStartTime = rhs.m_ordinalStartTime; 
  }

  return *this;
}

te::dt::Property* te::dt::DateTimeProperty::clone() const
{
  return new DateTimeProperty(*this);
}

