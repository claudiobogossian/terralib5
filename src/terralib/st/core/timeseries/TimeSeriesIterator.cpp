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
  \file TimeSeriesIterator.cpp

  \brief This file contains a time series iterator. 
*/

// TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/AbstractData.h"

//ST
#include "TimeSeriesIterator.h"

te::st::TimeSeriesIterator::TimeSeriesIterator() 
{   
}

te::st::TimeSeriesIterator::TimeSeriesIterator(TimeSeriesObservationSet::const_iterator& it) :
  m_it(it)
{
}

const te::st::TimeSeriesIterator& 
te::st::TimeSeriesIterator::operator=(const TimeSeriesIterator& rhs)
{
  if(this == &rhs) 
    return *this; 

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

te::st::TimeSeriesObservation te::st::TimeSeriesIterator::operator*() const
{
  return *m_it;
}

te::dt::DateTime* te::st::TimeSeriesIterator::getTime() const
{
  return m_it->getTime(); 
}

te::dt::AbstractData* te::st::TimeSeriesIterator::getValue() const
{
  return m_it->getValue(); 
}

double te::st::TimeSeriesIterator::getDouble() const
{
  return m_it->getDouble(); 
}

int te::st::TimeSeriesIterator::getInt() const
{
  return m_it->getInt(); 
}

std::string te::st::TimeSeriesIterator::getString() const
{
  return m_it->getString(); 
}

te::st::TimeSeriesIterator::~TimeSeriesIterator() 
{   
}









