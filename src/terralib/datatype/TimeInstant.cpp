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
  \file terralib/datatype/TimeInstant.cpp

  \brief A class to represent time instant. 
*/

// TerraLib
#include "TimeInstant.h"

te::dt::TimeInstant::TimeInstant()
  : m_timeInstant()
{
}

te::dt::TimeInstant::TimeInstant(const Date& d, const TimeDuration& td)
  : m_timeInstant(d.getDate(), td.getTimeDuration())
{
}

te::dt::TimeInstant::TimeInstant(const boost::posix_time::ptime& t)
  :  m_timeInstant(t)
{
} 

te::dt::TimeInstant::TimeInstant(const std::string& dtime)
{
  m_timeInstant = boost::posix_time::from_iso_string(dtime);
}

te::dt::TimeDuration te::dt::TimeInstant::getTime() const 
{ 
  boost::posix_time::time_duration td = m_timeInstant.time_of_day();

  return te::dt::TimeDuration(td); 
}

bool te::dt::TimeInstant::operator==(const DateTime& rhs) const
{
  const te::dt::TimeInstant* t = dynamic_cast<const te::dt::TimeInstant*>(&rhs);
  return m_timeInstant == t->m_timeInstant;
}
        
bool te::dt::TimeInstant::operator!=(const DateTime& rhs) const
{
  const te::dt::TimeInstant* t = dynamic_cast<const te::dt::TimeInstant*>(&rhs);
  return m_timeInstant != t->m_timeInstant;
}
       
bool te::dt::TimeInstant::operator<(const DateTime& rhs) const
{
  const te::dt::TimeInstant* t = dynamic_cast<const te::dt::TimeInstant*>(&rhs);
  return m_timeInstant < t->m_timeInstant;
}
        
bool te::dt::TimeInstant::operator>(const DateTime& rhs) const
{
  const te::dt::TimeInstant* t = dynamic_cast<const te::dt::TimeInstant*>(&rhs);
  return m_timeInstant > t->m_timeInstant;
}

long te::dt::TimeInstant::operator-(const TimeInstant& rhs) const
{
  boost::posix_time::time_duration td(m_timeInstant - rhs.m_timeInstant);

  return td.total_seconds();
}

te::dt::AbstractData* te::dt::TimeInstant::clone() const
{
  return new TimeInstant(*this);
}

std::string te::dt::TimeInstant::toString() const 
{ 
  return boost::posix_time::to_simple_string(m_timeInstant); 
  //return boost::posix_time::to_iso_string(m_timeInstant); 
  //return boost::posix_time::to_iso_extended_string(m_timeInstant); 
}

te::dt::TimeInstant::~TimeInstant()
{
}

