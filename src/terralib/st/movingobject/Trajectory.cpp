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
  \file Trajectory.cpp

  \brief This file contains a class to represent a moving object trajectory.
*/

//TerraLib
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/Geometry.h"
#include "../../datatype/DateTime.h"

//ST
#include "MovingObject.h"
#include "Trajectory.h"

te::st::Trajectory::Trajectory()
  : m_parent(0)
{  
}

te::st::Trajectory::Trajectory(MovingObjectIterator itBegin, MovingObjectIterator itEnd, MovingObject* parent)
  : m_begin(itBegin),
    m_end(itEnd),
    m_parent(0)
{  
}

te::st::TrajectoryIterator te::st::Trajectory::begin() 
{
  return TrajectoryIterator(m_begin);
}

te::st::TrajectoryIterator te::st::Trajectory::end() 
{
  return TrajectoryIterator(m_end);
}

te::st::MovingObject* te::st::Trajectory::getParent()
{
  return m_parent;
}    

te::st::MovingObject* te::st::Trajectory::getMovingObject()
{
  te::gm::GeometryProperty* p = dynamic_cast<te::gm::GeometryProperty*>(m_parent->getProperty()->clone());

  te::st::MovingObject* result = new te::st::MovingObject(p, m_parent->getId()); 
  
  te::st::TrajectoryIterator it = begin();
  while(it != m_end)
  {
    te::dt::DateTime* dt = dynamic_cast<te::dt::DateTime*>(it.getDateTime()->clone());
    te::gm::Geometry* geom = dynamic_cast<te::gm::Geometry*>(it.getGeometry()->clone());
    result->addObservation(dt, geom);
    ++it;
  }
    
  return result;
}




