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
  \file terralib/datatype/TimePeriod.cpp

  \brief A class to represent time period. 
*/

// TerraLib
#include "TimePeriod.h"
#include "TimeInstant.h"

te::dt::TimePeriod::TimePeriod()
  : m_timePeriod(boost::posix_time::ptime(), boost::posix_time::ptime())
{  
}   

te::dt::TimePeriod::TimePeriod(const TimeInstant& initialTime, const TimeInstant& finalTime)
  : m_timePeriod(initialTime.getTimeInstant(), finalTime.getTimeInstant())
{
} 

te::dt::TimePeriod::TimePeriod(const boost::posix_time::time_period& t) 
  : m_timePeriod(t) 
{ 
}

te::dt::TimeInstant te::dt::TimePeriod::getInitialTimeInstant() const
{ 
  return TimeInstant(m_timePeriod.begin());
}

te::dt::TimeInstant te::dt::TimePeriod::getFinalTimeInstant() const
{ 
  return TimeInstant(m_timePeriod.end());
} 

te::dt::DateTimeInstant* te::dt::TimePeriod::getInitialInstant() const
{ 
  return new TimeInstant(m_timePeriod.begin());
}

te::dt::DateTimeInstant* te::dt::TimePeriod::getFinalInstant() const
{ 
  return new TimeInstant(m_timePeriod.end());
} 

bool te::dt::TimePeriod::operator==(const DateTime& rhs) const
{
  const te::dt::TimePeriod* t = dynamic_cast<const te::dt::TimePeriod*>(&rhs);
  return m_timePeriod == t->m_timePeriod;
}
        
bool te::dt::TimePeriod::operator!=(const DateTime& rhs) const
{
  const te::dt::TimePeriod* t = dynamic_cast<const te::dt::TimePeriod*>(&rhs);
  return m_timePeriod != t->m_timePeriod;
}
       
bool te::dt::TimePeriod::operator<(const DateTime& rhs) const
{
  const te::dt::TimePeriod* t = dynamic_cast<const te::dt::TimePeriod*>(&rhs);
  return m_timePeriod < t->m_timePeriod;
}
        
bool te::dt::TimePeriod::operator>(const DateTime& rhs) const
{
  const te::dt::TimePeriod* t = dynamic_cast<const te::dt::TimePeriod*>(&rhs);
  return m_timePeriod > t->m_timePeriod;
}

te::dt::AbstractData* te::dt::TimePeriod::clone() const
{
  return new te::dt::TimePeriod(*this);
}

std::string te::dt::TimePeriod::toString() const
{
  std::string begin(boost::posix_time::to_simple_string(m_timePeriod.begin()));
  std::string end(boost::posix_time::to_simple_string(m_timePeriod.end()));
  //std::string begin(boost::posix_time::to_iso_extended_string(m_timePeriod.begin()));
  //std::string end(boost::posix_time::to_iso_extended_string(m_timePeriod.end()));
  //std::string begin(boost::posix_time::to_iso_string(m_timePeriod.begin()));
  //std::string end(boost::posix_time::to_iso_string(m_timePeriod.end()));
  return std::string(begin + "/" + end);
}

te::dt::TimePeriod::~TimePeriod()
{
}


