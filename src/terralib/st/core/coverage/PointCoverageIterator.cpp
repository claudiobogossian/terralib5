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
  \file PointCoverageIterator.cpp

   \brief  This file contains an iterator that is able to traverse all observations
          of a point coverage.  
*/

// TerraLib
#include "../../../geometry/Point.h"
#include "../../../datatype/AbstractData.h"

//ST
#include "PointCoverageIterator.h"

te::st::PointCoverageIterator::PointCoverageIterator() 
{   
}

te::st::PointCoverageIterator::PointCoverageIterator(const PointCoverageObservationSet::const_iterator& it) :
  m_it(it)
{
}

const te::st::PointCoverageIterator& 
te::st::PointCoverageIterator::operator=(const PointCoverageIterator& rhs)
{
  if(this == &rhs) 
    return *this; 

  m_it = rhs.m_it;
  return *this; 
}

bool te::st::PointCoverageIterator::operator==(const PointCoverageIterator& rhs)
{
  return m_it == rhs.m_it;
}

bool te::st::PointCoverageIterator::operator!=(const PointCoverageIterator& rhs)
{
  return m_it != rhs.m_it;
}

te::st::PointCoverageIterator& te::st::PointCoverageIterator::operator++()
{
  ++m_it; 
  return *this;
}

te::st::PointCoverageIterator te::st::PointCoverageIterator::operator++(int /*aux*/)
{
  te::st::PointCoverageIterator result = *this;
  ++m_it; 
  return result;
}

te::st::PointCoverageIterator& te::st::PointCoverageIterator::operator--()
{
  --m_it; 
  return *this;
}

te::st::PointCoverageIterator te::st::PointCoverageIterator::operator--(int /*aux*/)
{
  te::st::PointCoverageIterator result = *this;
  --m_it; 
  return result;
}

te::st::PointCoverageObservation te::st::PointCoverageIterator::operator*() const
{
  return *m_it;
}

te::gm::Point& te::st::PointCoverageIterator::getLocation() const
{
  return m_it->get()->first; 
}

te::dt::AbstractData* te::st::PointCoverageIterator::getValue(int i) const
{
  PointCoverageItem* aux = m_it->get();
  return &aux->second.operator[](i); 
}

te::dt::AbstractData* te::st::PointCoverageIterator::getValue() const
{
  PointCoverageItem* aux = m_it->get();
  return &aux->second.operator[](0); 
}

double te::st::PointCoverageIterator::getDouble(int i) const
{
  PointCoverageItem* aux = m_it->get();
  return atof(aux->second.operator[](i).toString().c_str()); 
}

double te::st::PointCoverageIterator::getDouble() const
{
  PointCoverageItem* aux = m_it->get();
  return atof(aux->second.operator[](0).toString().c_str()); 
}

int te::st::PointCoverageIterator::getInt(int i) const
{
  PointCoverageItem* aux = m_it->get();
  return atoi(aux->second.operator[](i).toString().c_str()); 
}

int te::st::PointCoverageIterator::getInt() const
{
  PointCoverageItem* aux = m_it->get();
  return atoi(aux->second.operator[](0).toString().c_str()); 
}

te::st::PointCoverageIterator::~PointCoverageIterator() 
{   
}








