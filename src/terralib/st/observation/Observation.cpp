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
  \file Observation.cpp

  \brief A class to represent an observation.
 */

// TerraLib
#include "../../datatype/AbstractData.h"
#include "../../datatype/DateTime.h"
#include "../../datatype/TimeInstant.h"
#include "../../datatype/TimePeriod.h"
#include "../../common/STLUtils.h"

//DyGeo
#include "Observation.h"
#include "ObservationSet.h"

te::st::Observation::Observation()
  : m_phTime(0),
    m_resultTime(0),
    m_validTime(0),
    m_parent(0)
{  
}

te::st::Observation::Observation(const Observation& obs)
{
  *this = obs;
}

te::st::Observation::Observation(te::dt::DateTime* phTime, ObservationSet* parent) 
  : m_phTime(phTime),
    m_resultTime(0),
    m_validTime(0),
    m_parent(parent)
{
}

te::st::Observation::Observation(te::dt::DateTime* phTime, te::dt::AbstractData* obsValue, 
                                    ObservationSet* parent)
  : m_phTime(phTime),
    m_resultTime(0),
    m_validTime(0),
    m_parent(parent)
{
  m_observedValues.push_back(obsValue);
}

te::st::Observation::Observation(te::dt::DateTime* phTime, 
                                     const std::vector<te::dt::AbstractData*>& obsValues, 
                                     ObservationSet* parent)
  : m_phTime(phTime),
    m_resultTime(0),
    m_validTime(0),
    m_observedValues(obsValues),
    m_parent(parent)
 {
 }

te::st::Observation::Observation(te::dt::DateTime* phTime, te::dt::DateTimeInstant* resTime, 
                    te::dt::DateTimePeriod* valTime, const std::vector<te::dt::AbstractData*>& obsValues,
                    ObservationSet* parent) 
  : m_phTime(phTime),
    m_resultTime(resTime),
    m_validTime(valTime),
    m_observedValues(obsValues),
    m_parent(parent)
{
}

const te::st::Observation& te::st::Observation::operator=(const Observation& rhs)
{
  if(this == &rhs) 
    return *this; 

  m_parent = rhs.m_parent;
  m_phTime = 0;
  m_resultTime = 0;
  m_validTime = 0;
  if(rhs.m_phTime)
    m_phTime = dynamic_cast<te::dt::DateTime*>(rhs.m_phTime->clone());
  if(rhs.m_resultTime)
    m_resultTime = dynamic_cast<te::dt::DateTimeInstant*>(rhs.m_resultTime->clone());
  if(rhs.m_validTime)
    m_validTime = dynamic_cast<te::dt::DateTimePeriod*>(rhs.m_validTime->clone());
  
  te::common::Clone(rhs.m_observedValues, m_observedValues);  
  return *this; 
}

te::dt::DateTime* te::st::Observation::getTime() const
{
  return m_phTime;
}
        
void te::st::Observation::setTime(te::dt::DateTime* phTime)
{
  m_phTime = phTime;
}

te::dt::DateTimeInstant* te::st::Observation::getResultTime() const
{
  return m_resultTime;
}
        
void te::st::Observation::setResultTime(te::dt::DateTimeInstant* resTime)
{
  m_resultTime = resTime;
}

te::dt::DateTimePeriod* te::st::Observation::getValidTime() const
{
  return m_validTime;
}
        
void te::st::Observation::setValidTime(te::dt::DateTimePeriod* valTime)
{
  m_validTime = valTime;
}

std::vector<te::dt::AbstractData*>& te::st::Observation::getObservedValues()
{
  return m_observedValues;
}

te::dt::AbstractData* te::st::Observation::getObservedValue(int idx) const
{
  return m_observedValues[idx];
}

void te::st::Observation::addValue(te::dt::AbstractData* value)
{
  m_observedValues.push_back(value);
}

void te::st::Observation::setValues(const std::vector<te::dt::AbstractData*>& values)
{
  m_observedValues.clear();
  m_observedValues = values;
}

te::st::ObservationSet* te::st::Observation::getParent() const
{
  return m_parent;
}

void te::st::Observation::setParent(te::st::ObservationSet* parent)
{
  if(m_parent)
    delete m_parent;
  m_parent = parent;
}

te::st::Observation* te::st::Observation::clone() const
{
  return new Observation(*this);
}
  
te::st::Observation::~Observation()
{
  delete m_phTime;
  delete m_resultTime;
  delete m_validTime;
  te::common::FreeContents(m_observedValues);
}


