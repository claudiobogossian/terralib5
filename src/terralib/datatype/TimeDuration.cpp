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
  \file terralib/datatype/TimeDuration.cpp

  \brief A class to represent time duration. 
*/

// TerraLib
#include "TimeDuration.h"

te::dt::TimeDuration::TimeDuration(long hours, long minutes, long seconds, boost::int64_t fracSeconds)
  : m_timeDuration(hours, minutes, seconds, fracSeconds)
{
}

te::dt::TimeDuration::TimeDuration(boost::posix_time::time_duration& t) 
  : m_timeDuration(t) 
{ 
}

bool te::dt::TimeDuration::operator==(const DateTime& rhs) const
{
  const te::dt::TimeDuration* t = dynamic_cast<const te::dt::TimeDuration*>(&rhs);
  return m_timeDuration == t->m_timeDuration;
}
        
bool te::dt::TimeDuration::operator!=(const DateTime& rhs) const
{
  const te::dt::TimeDuration* t = dynamic_cast<const te::dt::TimeDuration*>(&rhs);
  return m_timeDuration != t->m_timeDuration;
}
       
bool te::dt::TimeDuration::operator<(const DateTime& rhs) const
{
  const te::dt::TimeDuration* t = dynamic_cast<const te::dt::TimeDuration*>(&rhs);
  return m_timeDuration < t->m_timeDuration;
}
        
bool te::dt::TimeDuration::operator>(const DateTime& rhs) const
{
  const te::dt::TimeDuration* t = dynamic_cast<const te::dt::TimeDuration*>(&rhs);
  return m_timeDuration > t->m_timeDuration;
}

long te::dt::TimeDuration::operator-(const TimeDuration& rhs) const
{
  boost::posix_time::time_duration td(m_timeDuration - rhs.m_timeDuration);

  return td.total_seconds();
}

te::dt::AbstractData* te::dt::TimeDuration::clone() const
{
  return new TimeDuration(*this);
}

std::string te::dt::TimeDuration::toString() const 
{ 
  return boost::posix_time::to_simple_string(m_timeDuration); 
  //return boost::posix_time::to_iso_extended_string(m_timeDuration); 
  //return boost::posix_time::to_iso_string(m_timeDuration); 
}

te::dt::TimeDuration::~TimeDuration()
{
}

