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
  \file terralib/datatype/OrdinalInstant.cpp

  \brief A class to represent ordinal time instant. 
*/

// TerraLib
#include "OrdinalInstant.h"

te::dt::OrdinalInstant::OrdinalInstant()
  : m_timeInstant(0)
{
}

te::dt::OrdinalInstant::OrdinalInstant(Int64 t)
  : m_timeInstant(t)
{
}

bool te::dt::OrdinalInstant::operator==(const DateTime& rhs) const
{
  const te::dt::OrdinalInstant* t = dynamic_cast<const te::dt::OrdinalInstant*>(&rhs);
  return m_timeInstant.getValue() == t->m_timeInstant.getValue();
}
        
bool te::dt::OrdinalInstant::operator!=(const DateTime& rhs) const
{
  const te::dt::OrdinalInstant* t = dynamic_cast<const te::dt::OrdinalInstant*>(&rhs);
  return m_timeInstant.getValue() != t->m_timeInstant.getValue();
}
       
bool te::dt::OrdinalInstant::operator<(const DateTime& rhs) const
{
  const te::dt::OrdinalInstant* t = dynamic_cast<const te::dt::OrdinalInstant*>(&rhs);
  return m_timeInstant.getValue() < t->m_timeInstant.getValue();
}
        
bool te::dt::OrdinalInstant::operator>(const DateTime& rhs) const
{
  const te::dt::OrdinalInstant* t = dynamic_cast<const te::dt::OrdinalInstant*>(&rhs);
  return m_timeInstant.getValue() > t->m_timeInstant.getValue();
}

te::dt::AbstractData* te::dt::OrdinalInstant::clone() const
{
  return new OrdinalInstant(*this);
}

std::string te::dt::OrdinalInstant::toString() const 
{ 
  return m_timeInstant.toString(); 
}

