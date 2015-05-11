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
  \file Object.cpp

  \brief This file contains a class to represent an object.
*/

// TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimePeriod.h"
#include "../../../datatype/DateTimeInstant.h"
#include "../../../datatype/AbstractData.h"
#include "../../../geometry/Geometry.h"

//ST
#include "../timeseries/TimeSeries.h"
#include "../trajectory/Trajectory.h"
#include "Object.h"


te::st::Object::Object(const std::string& id, TimeSeries* ts, Trajectory* tj)
  : m_id(id),
    m_trajectory(tj),
    m_timeseries(ts)
{
}

te::st::Object::Object(const std::string& id, TimeSeries* ts)
  : m_id(id),
    m_trajectory(),
    m_timeseries(ts)
{
}

te::st::Object::Object(const std::string& id, Trajectory* tj)
  : m_id(id),
    m_trajectory(tj),
    m_timeseries()
{
}

te::st::Object::Object(const Object& obj)
  : m_id(obj.m_id)
{
  if(obj.m_timeseries.get())
    m_timeseries.reset(obj.m_timeseries->clone());
  if(obj.m_trajectory.get())
    m_trajectory.reset(obj.m_trajectory->clone());
}

te::st::Object& te::st::Object::operator=(const Object& other)
{
  if(this != &other)
  {
    m_id = other.m_id;
    if(other.m_timeseries.get())
      m_timeseries.reset(other.m_timeseries->clone());
    if(other.m_trajectory.get())
      m_trajectory.reset(other.m_trajectory->clone());
  }
  return *this;
}

te::st::Object* te::st::Object::clone() const
{
  return new Object(*this);
}

std::string te::st::Object::getId() const
{
  return m_id;
}       

void te::st::Object::setId(const std::string& id)
{
  m_id = id;
} 

te::st::TimeSeries* te::st::Object::getTimeSeries() const
{
  return m_timeseries.get();
}

void te::st::Object::setTimeSeries(TimeSeries* ts)
{
  m_timeseries.reset(ts);
}

te::st::Trajectory* te::st::Object::getTrajectory() const
{
  return m_trajectory.get();
}

void te::st::Object::setTrajectory(Trajectory* tj)
{
  m_trajectory.reset(tj);
}

std::pair<te::dt::AbstractData*,te::gm::Geometry*> 
te::st::Object::getState(te::dt::DateTime* t) const
{
  std::auto_ptr<te::dt::AbstractData> v;
  std::auto_ptr<te::gm::Geometry> g; 

  if(m_timeseries.get()!=0)
    v = m_timeseries->getValue(t);

  if(m_trajectory.get()!=0)
    g = m_trajectory->getGeometry(t);

  std::pair<te::dt::AbstractData*,te::gm::Geometry*> result(v.release(),g.release());
  return result;  
}

te::st::Object::~Object()
{
}


