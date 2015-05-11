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
  \file terralib/datatype/OrdinalPeriod.cpp

  \brief A class to represent ordinal time period. 
*/

// TerraLib
#include "OrdinalPeriod.h"

te::dt::OrdinalPeriod::OrdinalPeriod()
  : m_initialInstant(),
    m_finalInstant()    
{
}

te::dt::OrdinalPeriod::OrdinalPeriod(Int64 it, Int64 ft)
   : m_initialInstant(it),
     m_finalInstant(ft)    
{
}

te::dt::DateTimeInstant* te::dt::OrdinalPeriod::getInitialInstant() const
{ 
  return new te::dt::OrdinalInstant(m_initialInstant);
}

te::dt::DateTimeInstant* te::dt::OrdinalPeriod::getFinalInstant() const
{ 
  return new te::dt::OrdinalInstant(m_finalInstant);
} 

bool te::dt::OrdinalPeriod::operator==(const DateTime& rhs) const
{
  const te::dt::OrdinalPeriod* t = dynamic_cast<const te::dt::OrdinalPeriod*>(&rhs);
  return (m_initialInstant == t->m_initialInstant &&
          m_finalInstant == t->m_finalInstant);
}
        
bool te::dt::OrdinalPeriod::operator!=(const DateTime& rhs) const
{
  const te::dt::OrdinalPeriod* t = dynamic_cast<const te::dt::OrdinalPeriod*>(&rhs);
  return (m_initialInstant != t->m_initialInstant ||
          m_finalInstant != t->m_finalInstant);
}
       
bool te::dt::OrdinalPeriod::operator<(const DateTime& rhs) const
{
  const te::dt::OrdinalPeriod* t = dynamic_cast<const te::dt::OrdinalPeriod*>(&rhs);
  return m_finalInstant < t->m_initialInstant;
}
        
bool te::dt::OrdinalPeriod::operator>(const DateTime& rhs) const
{
  const te::dt::OrdinalPeriod* t = dynamic_cast<const te::dt::OrdinalPeriod*>(&rhs);
  return m_initialInstant > t->m_finalInstant;
}

te::dt::AbstractData* te::dt::OrdinalPeriod::clone() const
{
  return new OrdinalPeriod(*this);
}

std::string te::dt::OrdinalPeriod::toString() const 
{ 
  return std::string(m_initialInstant.toString() + "-" + m_finalInstant.toString()); 
}

