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
  \file Event.cpp

  \brief This file contains a class to represent an event.
*/

// TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimePeriod.h"
#include "../../../datatype/DateTimeInstant.h"
#include "../../../datatype/SimpleData.h"
#include "../../../datatype/AbstractData.h"
#include "../../../geometry/Geometry.h"
#include "../../../common/STLUtils.h"

//ST
#include "Event.h"
#include "Object.h"


te::st::Event::Event(const std::string& id, te::dt::DateTime* t, te::gm::Geometry* g)
  : m_id(id),
    m_time(t),
    m_location(g)
{
}

te::st::Event::Event( const std::string& id, te::dt::DateTime* t, te::gm::Geometry* g, 
                      std::vector<te::st::Object*> objs)
  : m_id(id),
    m_time(t),
    m_location(g),
    m_objects(objs)
{
}
     
te::st::Event::Event(const Event& ev)
  : m_id(ev.m_id)
{
  if(ev.m_time.get()==0)
    m_time.reset(static_cast<te::dt::DateTime*>(ev.m_time->clone()));
  if(ev.m_location.get()==0)
    m_location.reset(static_cast<te::gm::Geometry*>(ev.m_location->clone()));
  for(unsigned int i=0; i<ev.m_objects.size(); ++i)
    m_objects.push_back(ev.m_objects[i]->clone());
}

te::st::Event& te::st::Event::operator=(const Event& other)
{
  if(this != &other)
  {    
    m_id = other.m_id;
    if(other.m_time.get()==0)
      m_time.reset(static_cast<te::dt::DateTime*>(other.m_time->clone()));
    if(other.m_location.get()==0)
      m_location.reset(static_cast<te::gm::Geometry*>(other.m_location->clone()));
    te::common::FreeContents(m_objects);
    for(unsigned int i=0; i<other.m_objects.size(); ++i)
      m_objects.push_back(other.m_objects[i]->clone());
  }
  return *this;
}

te::st::Event* te::st::Event::clone() const
{
  return new Event(*this);
}

std::string te::st::Event::getId() const
{
  return m_id;
}

void te::st::Event::setId(const std::string& id)
{
  m_id = id;
}

te::dt::DateTime* te::st::Event::getTime() const
{
  return m_time.get();
}

void te::st::Event::setTime(te::dt::DateTime* t)
{
  m_time.reset(t);
}

te::gm::Geometry* te::st::Event::getLocation() const
{
  return m_location.get();
}

void te::st::Event::setLocation(te::gm::Geometry* g)
{
  m_location.reset(g);
}

void te::st::Event::getObjects(std::vector<te::st::Object*>& output) const
{
  output = m_objects;
}

void te::st::Event::setObjects(std::vector<te::st::Object*>& objs)
{
  m_objects.clear();
  m_objects = objs;
}

te::st::Event::~Event()
{
  m_objects.clear();
}


