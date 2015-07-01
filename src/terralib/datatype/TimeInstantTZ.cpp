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
  \file terralib/datatype/TimeInstantTZ.cpp

  \brief A class to represent time instant with time zone. 
*/

// TerraLib
#include "TimeInstantTZ.h"

bool te::dt::TimeInstantTZ::operator==(const DateTime& rhs) const
{
  const te::dt::TimeInstantTZ* t = dynamic_cast<const te::dt::TimeInstantTZ*>(&rhs);
  return m_timeInstantTZ == t->m_timeInstantTZ;
}
        
bool te::dt::TimeInstantTZ::operator!=(const DateTime& rhs) const
{
  const te::dt::TimeInstantTZ* t = dynamic_cast<const te::dt::TimeInstantTZ*>(&rhs);
  return m_timeInstantTZ != t->m_timeInstantTZ;
}
       
bool te::dt::TimeInstantTZ::operator<(const DateTime& rhs) const
{
  const te::dt::TimeInstantTZ* t = dynamic_cast<const te::dt::TimeInstantTZ*>(&rhs);
  return m_timeInstantTZ < t->m_timeInstantTZ;
}
        
bool te::dt::TimeInstantTZ::operator>(const DateTime& rhs) const
{
  const te::dt::TimeInstantTZ* t = dynamic_cast<const te::dt::TimeInstantTZ*>(&rhs);
  return m_timeInstantTZ > t->m_timeInstantTZ;
}

long te::dt::TimeInstantTZ::operator-(const TimeInstantTZ& rhs) const
{
  boost::posix_time::time_duration td(m_timeInstantTZ - rhs.m_timeInstantTZ);
  return td.total_seconds();
}

te::dt::AbstractData* te::dt::TimeInstantTZ::clone() const
{
  return new TimeInstantTZ(*this);
}

std::string te::dt::TimeInstantTZ::toString() const
{
  std::string t(boost::posix_time::to_simple_string(m_timeInstantTZ.utc_time()));
  //std::string t(boost::posix_time::to_iso_extended_string(m_timeInstantTZ.utc_time()));
  //std::string t(boost::posix_time::to_iso_string(m_timeInstantTZ.utc_time()));
  t += m_timeInstantTZ.zone()->to_posix_string();

  return t;
}

te::dt::TimeInstantTZ::~TimeInstantTZ()
{
}
