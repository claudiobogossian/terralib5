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
  \file terralib/datatype/Date.cpp

  \brief A base class for date data types.
*/

// TerraLib
#include "Date.h"

te::dt::Date::Date()
  : m_date()
{
} 

te::dt::Date::Date(boost::gregorian::greg_year year,
                   boost::gregorian::greg_month month,
                   boost::gregorian::greg_day day)
  : m_date(year, month, day)
{  
}

te::dt::Date::Date(const boost::gregorian::date& d) 
  : m_date(d) 
{ 
}

bool te::dt::Date::operator==(const DateTime& rhs) const
{
  const te::dt::Date* t = dynamic_cast<const te::dt::Date*>(&rhs);
  return m_date == t->getDate();
}
        
bool te::dt::Date::operator!=(const DateTime& rhs) const
{
  const te::dt::Date* t = dynamic_cast<const te::dt::Date*>(&rhs);
  return m_date != t->getDate();
}
       
bool te::dt::Date::operator<(const DateTime& rhs) const
{
  const te::dt::Date* t = dynamic_cast<const te::dt::Date*>(&rhs);
  return m_date < t->getDate();
}
        
bool te::dt::Date::operator>(const DateTime& rhs) const
{
  const te::dt::Date* t = dynamic_cast<const te::dt::Date*>(&rhs);
  return m_date > t->getDate();
}

long te::dt::Date::operator-(const Date& rhs) const
{
  boost::gregorian::date_duration dd(m_date - rhs.getDate());

  return dd.days();
}

te::dt::AbstractData* te::dt::Date::clone() const
{
  return new Date(*this);
}

std::string te::dt::Date::toString() const
{ 
  return boost::gregorian::to_simple_string(m_date); 
  //return boost::gregorian::to_iso_extended_string(m_date); 
  //return boost::gregorian::to_iso_string(m_date); 
}

te::dt::Date::~Date()
{
}


