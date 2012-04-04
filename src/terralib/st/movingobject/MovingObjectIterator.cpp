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
  \file MovingObjectIterator.cpp

  \brief This file contains a moving object iterator. 
*/

// TerraLib
#include "../../datatype/DateTime.h"
#include "../../geometry/Geometry.h"

//ST
#include "MovingObjectIterator.h"
#include "../observation/Observation.h"

te::st::MovingObjectIterator::MovingObjectIterator() :
  m_geomPropIdx(0)
{   
}

te::st::MovingObjectIterator::MovingObjectIterator(TimeToObservationMap::iterator it, int geomPropIdx) :
  m_it(it),
  m_geomPropIdx(geomPropIdx)
{
}

const te::st::MovingObjectIterator& 
te::st::MovingObjectIterator::operator=(const MovingObjectIterator& rhs)
{
  if(this == &rhs) 
    return *this; 

  m_geomPropIdx = rhs.m_geomPropIdx;
  m_it = rhs.m_it;
  return *this; 
}

bool te::st::MovingObjectIterator::operator==(const MovingObjectIterator& rhs)
{
  return m_it == rhs.m_it;
}

bool te::st::MovingObjectIterator::operator!=(const MovingObjectIterator& rhs)
{
  return m_it != rhs.m_it;
}

te::st::MovingObjectIterator& te::st::MovingObjectIterator::operator++()
{
  ++m_it; 
  return *this;
}

te::st::MovingObjectIterator te::st::MovingObjectIterator::operator++(int /*aux*/)
{
  te::st::MovingObjectIterator result = *this;
  ++m_it; 
  return result;
}

te::st::MovingObjectIterator& te::st::MovingObjectIterator::operator--()
{
  --m_it; 
  return *this;
}

te::st::MovingObjectIterator te::st::MovingObjectIterator::operator--(int /*aux*/)
{
  te::st::MovingObjectIterator result = *this;
  --m_it; 
  return result;
}

te::gm::Geometry* te::st::MovingObjectIterator::getGeometry()
{
  return dynamic_cast<te::gm::Geometry*>(m_it->second->getObservedValue(m_geomPropIdx));
}

te::dt::DateTime* te::st::MovingObjectIterator::getDateTime()
{
  return m_it->first; 
}






