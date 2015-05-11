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
  \file terralib/datatype/DateDuration.cpp

  \brief A class to represent date duration. 
*/

// TerraLib
#include "../common/StringUtils.h"
#include "DateDuration.h"

te::dt::DateDuration::DateDuration()
  : m_dateDuration()
{
}

te::dt::DateDuration::DateDuration(long dd)
  : m_dateDuration(dd)
{
}
        
te::dt::DateDuration::DateDuration(const boost::gregorian::date_duration& dd) 
  :  m_dateDuration(dd) 
{ 
}

bool te::dt::DateDuration::operator==(const DateTime& rhs) const
{
  const te::dt::DateDuration* t = dynamic_cast<const te::dt::DateDuration*>(&rhs);
  return m_dateDuration == t->m_dateDuration;
}
        
bool te::dt::DateDuration::operator!=(const DateTime& rhs) const
{
  const te::dt::DateDuration* t = dynamic_cast<const te::dt::DateDuration*>(&rhs);
  return m_dateDuration != t->m_dateDuration;
}
       
bool te::dt::DateDuration::operator<(const DateTime& rhs) const
{
  const te::dt::DateDuration* t = dynamic_cast<const te::dt::DateDuration*>(&rhs);
  return m_dateDuration < t->m_dateDuration;
}
        
bool te::dt::DateDuration::operator>(const DateTime& rhs) const
{
  const te::dt::DateDuration* t = dynamic_cast<const te::dt::DateDuration*>(&rhs);
  return m_dateDuration > t->m_dateDuration;
}

te::dt::AbstractData* te::dt::DateDuration::clone() const
{
  return new DateDuration(*this);
}

std::string te::dt::DateDuration::toString() const
{ 
  boost::int64_t days = m_dateDuration.days();

  return te::common::Convert2String(days);
}

te::dt::DateDuration::~DateDuration()
{
}


