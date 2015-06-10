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
  \file terralib/datatype/DatePeriod.cpp

  \brief A class to represent date period. 
*/

// TerraLib
#include "Date.h"
#include "DatePeriod.h"

te::dt::DatePeriod::DatePeriod()
  : m_datePeriod(boost::gregorian::date(), boost::gregorian::date())
{  
}   

te::dt::DatePeriod::DatePeriod(const Date& initialDate, const Date& finalDate)
  : m_datePeriod(initialDate.getDate(), finalDate.getDate())
{  
}       
        
te::dt::DatePeriod::DatePeriod(const boost::gregorian::date_period& dp) 
  : m_datePeriod(dp) 
{ 
}

te::dt::DatePeriod::DatePeriod(const boost::gregorian::date& begin, const boost::gregorian::date& end)
  : m_datePeriod(begin, end)
{
}

te::dt::Date te::dt::DatePeriod::getInitialDate() const
{ 
  return Date(m_datePeriod.begin());
}

te::dt::Date te::dt::DatePeriod::getFinalDate() const
{ 
  return Date(m_datePeriod.end());
} 

te::dt::DateTimeInstant* te::dt::DatePeriod::getInitialInstant() const
{ 
  return new Date(m_datePeriod.begin());
}

te::dt::DateTimeInstant* te::dt::DatePeriod::getFinalInstant() const
{ 
  return new Date(m_datePeriod.end());
} 

bool te::dt::DatePeriod::operator==(const DateTime& rhs) const
{
  const te::dt::DatePeriod* t = dynamic_cast<const te::dt::DatePeriod*>(&rhs);
  return m_datePeriod == t->m_datePeriod;
}
        
bool te::dt::DatePeriod::operator!=(const DateTime& rhs) const
{
  const te::dt::DatePeriod* t = dynamic_cast<const te::dt::DatePeriod*>(&rhs);
  return m_datePeriod != t->m_datePeriod;
}
       
bool te::dt::DatePeriod::operator<(const DateTime& rhs) const
{
  const te::dt::DatePeriod* t = dynamic_cast<const te::dt::DatePeriod*>(&rhs);
  return m_datePeriod < t->m_datePeriod;
}
        
bool te::dt::DatePeriod::operator>(const DateTime& rhs) const
{
  const te::dt::DatePeriod* t = dynamic_cast<const te::dt::DatePeriod*>(&rhs);
  return m_datePeriod > t->m_datePeriod;
}

te::dt::AbstractData* te::dt::DatePeriod::clone() const
{
  return new te::dt::DatePeriod(*this);
}

std::string te::dt::DatePeriod::toString() const
{
  return boost::gregorian::to_simple_string(m_datePeriod);
  //return boost::gregorian::to_iso_extended_string(m_datePeriod);
  //return boost::gregorian::to_iso_string(m_datePeriod);
}

te::dt::DatePeriod::~DatePeriod()
{
}



