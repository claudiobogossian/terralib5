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
  \file Observation.cpp

  \brief A class to represent an observation.
 */

// TerraLib
#include "../../../datatype/AbstractData.h"
#include "../../../datatype/DateTime.h"
#include "../../../datatype/TimeInstant.h"
#include "../../../datatype/TimePeriod.h"
#include "../../../geometry/Geometry.h"

//ST
#include "Observation.h"

te::st::Observation::Observation()
  : m_phTime(),
    m_resultTime(),
    m_validTime(),
    m_geometry()
{  
}

te::st::Observation::Observation(te::dt::DateTime* phTime) 
  : m_phTime(phTime),
    m_resultTime(),
    m_validTime(),
    m_geometry()
{
}

te::st::Observation::Observation(te::dt::DateTime* phTime, te::dt::AbstractData* obsValue)
  : m_phTime(phTime),
    m_resultTime(),
    m_validTime(),
    m_geometry()
{
  m_observedValues.push_back(obsValue);
}

te::st::Observation::Observation(te::dt::DateTime* phTime, te::gm::Geometry* geom, 
                                 const boost::ptr_vector<te::dt::AbstractData>& obsValues)
  : m_phTime(phTime),
    m_resultTime(),
    m_validTime(),
    m_observedValues(obsValues),
    m_geometry(geom)
 {
 }

te::st::Observation::Observation(te::dt::DateTime* phTime, te::dt::DateTimeInstant* resTime, 
                    te::dt::DateTimePeriod* valTime, te::gm::Geometry* geom,
                    const boost::ptr_vector<te::dt::AbstractData>& obsValues) 
  : m_phTime(phTime),
    m_resultTime(resTime),
    m_validTime(valTime),
    m_observedValues(obsValues),
    m_geometry(geom)
{
}

te::st::Observation::Observation(const Observation& obs)
{
  *this = obs;
}

const te::st::Observation& te::st::Observation::operator=(const Observation& rhs)
{
  if(this == &rhs) 
    return *this; 
    
  m_phTime.reset();
  m_resultTime.reset();
  m_validTime.reset();
  m_geometry.reset();
  m_observedValues.clear();

  if(rhs.m_phTime.get())
    m_phTime.reset(dynamic_cast<te::dt::DateTime*>(rhs.m_phTime->clone()));
  if(rhs.m_resultTime.get())
    m_resultTime.reset(dynamic_cast<te::dt::DateTimeInstant*>(rhs.m_resultTime->clone()));
  if(rhs.m_validTime.get())
    m_validTime.reset(dynamic_cast<te::dt::DateTimePeriod*>(rhs.m_validTime->clone()));
  if(rhs.m_geometry.get())
    m_geometry.reset(dynamic_cast<te::gm::Geometry*>(rhs.m_geometry->clone()));
  
  m_observedValues = rhs.m_observedValues.clone();
  return *this; 
}

te::dt::DateTime* te::st::Observation::getTime() const
{
  return m_phTime.get();
}
        
void te::st::Observation::setTime(te::dt::DateTime* phTime)
{
  m_phTime.reset(phTime);
}

te::dt::DateTimeInstant* te::st::Observation::getResultTime() const
{
  return m_resultTime.get();
}
        
void te::st::Observation::setResultTime(te::dt::DateTimeInstant* resTime)
{
  m_resultTime.reset(resTime);
}

te::dt::DateTimePeriod* te::st::Observation::getValidTime() const
{
  return m_validTime.get();
}
        
void te::st::Observation::setValidTime(te::dt::DateTimePeriod* valTime)
{
  m_validTime.reset(valTime);
}

boost::ptr_vector<te::dt::AbstractData>& te::st::Observation::getObservedValues()
{
  return m_observedValues;
}

const te::dt::AbstractData* te::st::Observation::getObservedValue(int idx) const
{
  return &m_observedValues[idx];
}

void te::st::Observation::addValue(te::dt::AbstractData* value)
{
  m_observedValues.push_back(value);
}

void te::st::Observation::setValues(const boost::ptr_vector<te::dt::AbstractData>& values)
{
  m_observedValues.clear();
  m_observedValues = values; //deep copy?
}

te::gm::Geometry* te::st::Observation::getGeometry() const
{
  return m_geometry.get();
}

void te::st::Observation::setGeometry(te::gm::Geometry* geom)
{
  m_geometry.reset(geom);
}

te::st::Observation* te::st::Observation::clone() const
{
  return new Observation(*this);
}
  
te::st::Observation::~Observation()
{
}


