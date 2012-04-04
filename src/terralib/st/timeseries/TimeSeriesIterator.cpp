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
  \file TimeSeriesIterator.cpp

  \brief This file contains a time series iterator. 
*/

// TerraLib
#include "../../datatype/DateTime.h"

//ST
#include "TimeSeriesIterator.h"
#include "../observation/Observation.h"

te::st::TimeSeriesIterator::TimeSeriesIterator() :
  m_propIdx(0)
{   
}

te::st::TimeSeriesIterator::TimeSeriesIterator(TimeToObservationMap::iterator it, int propIdx) :
  m_it(it),
  m_propIdx(propIdx)
{
}

const te::st::TimeSeriesIterator& 
te::st::TimeSeriesIterator::operator=(const TimeSeriesIterator& rhs)
{
  if(this == &rhs) 
    return *this; 

  m_propIdx = rhs.m_propIdx;
  m_it = rhs.m_it;
  return *this; 
}

bool te::st::TimeSeriesIterator::operator==(const TimeSeriesIterator& rhs)
{
  return m_it == rhs.m_it;
}

bool te::st::TimeSeriesIterator::operator!=(const TimeSeriesIterator& rhs)
{
  return m_it != rhs.m_it;
}

te::st::TimeSeriesIterator& te::st::TimeSeriesIterator::operator++()
{
  ++m_it; 
  return *this;
}

te::st::TimeSeriesIterator te::st::TimeSeriesIterator::operator++(int /*aux*/)
{
  te::st::TimeSeriesIterator result = *this;
  ++m_it; 
  return result;
}

te::st::TimeSeriesIterator& te::st::TimeSeriesIterator::operator--()
{
  --m_it; 
  return *this;
}

te::st::TimeSeriesIterator te::st::TimeSeriesIterator::operator--(int /*aux*/)
{
  te::st::TimeSeriesIterator result = *this;
  --m_it; 
  return result;
}

te::dt::DateTime* te::st::TimeSeriesIterator::getDateTime()
{
  return m_it->first; 
}

te::dt::AbstractData* te::st::TimeSeriesIterator::getObservedValue()
{
  return m_it->second->getObservedValue(m_propIdx);
}







