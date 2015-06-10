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
  \file terralib/datatype/TimePeriodTZ.cpp

  \brief A class to represent time period with time zone. 
*/

// TerraLib
#include "TimeInstantTZ.h"
#include "TimePeriodTZ.h"

te::dt::TimePeriodTZ::TimePeriodTZ(const TimeInstantTZ& initialTime, const TimeInstantTZ& finalTime)
  : m_timePeriodTZ(initialTime.getTimeInstantTZ(), finalTime.getTimeInstantTZ())
{
} 

te::dt::TimePeriodTZ::TimePeriodTZ(const boost::local_time::local_time_period& t)
  : m_timePeriodTZ(t)
{
}

te::dt::TimeInstantTZ te::dt::TimePeriodTZ::getInitialTimeInstant() const
{ 
  return TimeInstantTZ(m_timePeriodTZ.begin());
}

te::dt::TimeInstantTZ te::dt::TimePeriodTZ::getFinalTimeInstant() const
{ 
  return TimeInstantTZ(m_timePeriodTZ.end());
} 

te::dt::DateTimeInstant* te::dt::TimePeriodTZ::getInitialInstant() const
{ 
  return new TimeInstantTZ(m_timePeriodTZ.begin());
}

te::dt::DateTimeInstant* te::dt::TimePeriodTZ::getFinalInstant() const
{ 
  return new TimeInstantTZ(m_timePeriodTZ.end());
} 

bool te::dt::TimePeriodTZ::operator==(const DateTime& rhs) const
{
  const te::dt::TimePeriodTZ* t = dynamic_cast<const te::dt::TimePeriodTZ*>(&rhs);
  return m_timePeriodTZ == t->m_timePeriodTZ;
}
        
bool te::dt::TimePeriodTZ::operator!=(const DateTime& rhs) const
{
  const te::dt::TimePeriodTZ* t = dynamic_cast<const te::dt::TimePeriodTZ*>(&rhs);
  return m_timePeriodTZ != t->m_timePeriodTZ;
}
       
bool te::dt::TimePeriodTZ::operator<(const DateTime& rhs) const
{
  const te::dt::TimePeriodTZ* t = dynamic_cast<const te::dt::TimePeriodTZ*>(&rhs);
  return m_timePeriodTZ < t->m_timePeriodTZ;
}
        
bool te::dt::TimePeriodTZ::operator>(const DateTime& rhs) const
{
  const te::dt::TimePeriodTZ* t = dynamic_cast<const te::dt::TimePeriodTZ*>(&rhs);
  return m_timePeriodTZ > t->m_timePeriodTZ;
}

te::dt::AbstractData* te::dt::TimePeriodTZ::clone() const
{
  return new te::dt::TimePeriodTZ(*this);
}

std::string te::dt::TimePeriodTZ::toString() const
{
  std::string begin(boost::posix_time::to_simple_string(m_timePeriodTZ.begin().utc_time()));
  //std::string begin(boost::posix_time::to_iso_extended_string(m_timePeriodTZ.begin().utc_time()));
  //std::string begin(boost::posix_time::to_iso_string(m_timePeriodTZ.begin().utc_time()));
  begin += m_timePeriodTZ.begin().zone()->to_posix_string();

  std::string end(boost::posix_time::to_simple_string(m_timePeriodTZ.last().utc_time()));
  //std::string end(boost::posix_time::to_iso_extended_string(m_timePeriodTZ.last().utc_time()));
  //std::string end(boost::posix_time::to_iso_string(m_timePeriodTZ.last().utc_time()));
  end += m_timePeriodTZ.begin().zone()->to_posix_string();

  return begin + "/" + end;
}

te::dt::TimePeriodTZ::~TimePeriodTZ()
{
}

