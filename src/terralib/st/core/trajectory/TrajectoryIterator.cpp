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
  \file TrajectoryIterator.cpp

  \brief This file contains a trajetory iterator. 
*/

//ST
#include "TrajectoryIterator.h"

te::st::TrajectoryIterator::TrajectoryIterator()
{   
}

te::st::TrajectoryIterator::TrajectoryIterator(TrajectoryObservationSet::const_iterator& it) 
  : m_it(it)
{
}

const te::st::TrajectoryIterator& 
te::st::TrajectoryIterator::operator=(const TrajectoryIterator& rhs)
{
  if(this == &rhs) 
    return *this; 

  m_it = rhs.m_it;
  return *this; 
}

bool te::st::TrajectoryIterator::operator==(const TrajectoryIterator& rhs)
{
  return m_it == rhs.m_it;
}

bool te::st::TrajectoryIterator::operator!=(const TrajectoryIterator& rhs)
{
  return m_it != rhs.m_it;
}

te::st::TrajectoryIterator& te::st::TrajectoryIterator::operator++()
{
  ++m_it; 
  return *this;
}

te::st::TrajectoryIterator te::st::TrajectoryIterator::operator++(int /*aux*/)
{
  te::st::TrajectoryIterator result = *this;
  ++m_it; 
  return result;
}

te::st::TrajectoryIterator& te::st::TrajectoryIterator::operator--()
{
  --m_it; 
  return *this;
}

te::st::TrajectoryIterator te::st::TrajectoryIterator::operator--(int /*aux*/)
{
  te::st::TrajectoryIterator result = *this;
  --m_it; 
  return result;
}

te::st::TrajectoryObservation te::st::TrajectoryIterator::operator*() const
{
  return *m_it;
}

te::gm::Geometry* te::st::TrajectoryIterator::getGeometry() const
{
  return m_it->second.get();
}

te::dt::DateTime* te::st::TrajectoryIterator::getTime() const
{
  return m_it->first.get(); 
}

te::st::TrajectoryIterator::~TrajectoryIterator()
{   
}






