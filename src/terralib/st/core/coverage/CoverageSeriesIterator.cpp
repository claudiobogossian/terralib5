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
  \file CoverageSeriesIterator.cpp

  \brief  This file contains an iterator that is able to traverse all observations
          of a coverage series.  
*/

//ST
#include "CoverageSeriesIterator.h"

te::st::CoverageSeriesIterator::CoverageSeriesIterator() 
{   
}

te::st::CoverageSeriesIterator::CoverageSeriesIterator(const CoverageSeriesObservationSet::const_iterator& it) :
  m_it(it)
{
}

const te::st::CoverageSeriesIterator& 
te::st::CoverageSeriesIterator::operator=(const CoverageSeriesIterator& rhs)
{
  if(this == &rhs) 
    return *this; 

  m_it = rhs.m_it;
  return *this; 
}

bool te::st::CoverageSeriesIterator::operator==(const CoverageSeriesIterator& rhs)
{
  return m_it == rhs.m_it;
}

bool te::st::CoverageSeriesIterator::operator!=(const CoverageSeriesIterator& rhs)
{
  return m_it != rhs.m_it;
}

te::st::CoverageSeriesIterator& te::st::CoverageSeriesIterator::operator++()
{
  ++m_it; 
  return *this;
}

te::st::CoverageSeriesIterator te::st::CoverageSeriesIterator::operator++(int /*aux*/)
{
  te::st::CoverageSeriesIterator result = *this;
  ++m_it; 
  return result;
}

te::st::CoverageSeriesIterator& te::st::CoverageSeriesIterator::operator--()
{
  --m_it; 
  return *this;
}

te::st::CoverageSeriesIterator te::st::CoverageSeriesIterator::operator--(int /*aux*/)
{
  te::st::CoverageSeriesIterator result = *this;
  --m_it; 
  return result;
}

te::st::CoverageSeriesObservation te::st::CoverageSeriesIterator::operator*() const
{
  return *m_it;
}

te::dt::DateTime* te::st::CoverageSeriesIterator::getTime() const
{
  return m_it->first.get(); 
}

te::st::Coverage* te::st::CoverageSeriesIterator::getCoverage() const
{
  return m_it->second.get(); 
}

te::st::CoverageSeriesIterator::~CoverageSeriesIterator() 
{   
}










