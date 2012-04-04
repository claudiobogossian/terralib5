/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file ObservationSetIterator.cpp

  \brief This file contains an iterator over observation set. 
*/

// TerraLib
#include "../../datatype/DateTime.h"

//DyGeo
#include "ObservationSetIterator.h"
#include "Observation.h"

te::st::ObservationSetIterator::ObservationSetIterator() 
{   
}

te::st::ObservationSetIterator::ObservationSetIterator(TimeToObservationMap::iterator it) :
  m_it(it)
{
}

const te::st::ObservationSetIterator& 
te::st::ObservationSetIterator::operator=(const ObservationSetIterator& rhs)
{
  if(this == &rhs) 
    return *this; 

  m_it = rhs.m_it;
  return *this; 
}

bool te::st::ObservationSetIterator::operator==(const ObservationSetIterator& rhs)
{
  return m_it == rhs.m_it;
}

bool te::st::ObservationSetIterator::operator!=(const ObservationSetIterator& rhs)
{
  return m_it != rhs.m_it;
}

te::st::ObservationSetIterator& te::st::ObservationSetIterator::operator++()
{
  ++m_it; 
  return *this;
}

te::st::ObservationSetIterator te::st::ObservationSetIterator::operator++(int /*aux*/)
{
  te::st::ObservationSetIterator result = *this;
  ++m_it; 
  return result;
}

te::st::ObservationSetIterator& te::st::ObservationSetIterator::operator--()
{
  --m_it; 
  return *this;
}

te::st::ObservationSetIterator te::st::ObservationSetIterator::operator--(int /*aux*/)
{
  te::st::ObservationSetIterator result = *this;
  --m_it; 
  return result;
}

te::st::Observation* te::st::ObservationSetIterator::getObservation()
{
  return m_it->second;
}

te::dt::DateTime* te::st::ObservationSetIterator::getDateTime()
{
  return m_it->first;
}

std::vector<te::dt::AbstractData*> te::st::ObservationSetIterator::getObservedValues()
{
  return m_it->second->getObservedValues();
}

te::dt::AbstractData* te::st::ObservationSetIterator::getObservedValue(int idx)
{
  return m_it->second->getObservedValue(idx);
}    







