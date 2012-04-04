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
  \file TimeSeries.cpp

  \brief This file contains a class to represent a time series.
*/

// TerraLib
#include "../../datatype/AbstractData.h"
#include "../../datatype/Property.h"
#include "../../datatype/DateTime.h"

//ST
#include "TimeSeries.h"
#include "TimeSeriesIterator.h"
#include "../observation/Observation.h"
#include "../observation/ObservationSet.h"
#include "../interpolator/AbstractInterpolator.h"

te::st::TimeSeries::TimeSeries()
  : m_propIdx(0),
    m_interpolator(0)
{  
  m_observations = new ObservationSet();
}

te::st::TimeSeries::TimeSeries(ObservationSet* obs, int propIdx)
  : m_observations(obs),
    m_propIdx(propIdx),
    m_interpolator(0)
{  
}

te::st::TimeSeries::TimeSeries(ObservationSet* obs, int propIdx, AbstractInterpolator* interp)
  : m_observations(obs),
    m_propIdx(propIdx),
    m_interpolator(interp)
{  
}

te::st::TimeSeries::TimeSeries(te::dt::Property* prop)
 : m_propIdx(0),
   m_interpolator(0)
{  
  m_observations = new ObservationSet();
  m_observations->addProperty(prop);
}

te::st::TimeSeriesIterator te::st::TimeSeries::begin() 
{
  return TimeSeriesIterator(m_observations->getObservations().begin(), m_propIdx);
}

te::st::TimeSeriesIterator te::st::TimeSeries::end() 
{
  return TimeSeriesIterator(m_observations->getObservations().end(), m_propIdx);
}

void te::st::TimeSeries::setObservationSet(ObservationSet* obs, int attrIdx)
{
  m_observations->clear();
  m_observations = obs;
  m_propIdx = attrIdx;
}

void te::st::TimeSeries::addObservation(te::dt::DateTime* phTime, te::dt::AbstractData* obsValue)
{
  Observation* ob = new Observation(phTime, obsValue);
  m_observations->addObservation(ob);
}

std::size_t te::st::TimeSeries::size() const
{
  return m_observations->size();
}

te::dt::AbstractData* te::st::TimeSeries::getValue(te::dt::DateTime* t)
{
  /* \TO DO: implementar um interpolador!!!!
  for(std::size_t i = 0; i < m_observations->size(); ++i)
  {
    Observation* obs = m_observations->getObservation(i); 
    if((*obs->getTime())==(*t))
      return obs->getObservedValue(m_propIdx);
  } 
  */
  return 0;
}

te::st::AbstractInterpolator* te::st::TimeSeries::getInterpolator() const
{
  return m_interpolator;
}

void te::st::TimeSeries::setInterpolator(te::st::AbstractInterpolator* interp)
{
  m_interpolator = interp;
}

te::dt::Property* te::st::TimeSeries::getProperty()
{
  return m_observations->getProperty(m_propIdx);
}

te::st::TimeSeries::~TimeSeries()
{
  m_observations->clear();
}


