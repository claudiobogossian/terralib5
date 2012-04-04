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
  \file LocatedTimeSeries.cpp

  \brief This file contains a class to represent a located time series
*/

// TerraLib
#include "../../geometry/Geometry.h"
#include "../../datatype/Property.h"
#include "../../common/STLUtils.h"

//ST
#include "TimeSeries.h"
#include "LocatedTimeSeries.h"

te::st::LocatedTimeSeries::LocatedTimeSeries()
  : m_spLocation(0),
    m_Id(-1)
{  
}

te::st::LocatedTimeSeries::LocatedTimeSeries(TimeSeries* timeSeries, 
                                te::gm::Geometry* spLocation, int id)
  : m_spLocation(spLocation),
    m_Id(id)
{  
  m_timeSeries.push_back(timeSeries);
}

te::st::LocatedTimeSeries::LocatedTimeSeries(const std::vector<TimeSeries*>& timeSeries, 
                        te::gm::Geometry* spLocation, int id)
  : m_timeSeries(timeSeries),
    m_spLocation(spLocation),
    m_Id(id)
{  
}

std::vector<te::st::TimeSeries*>& te::st::LocatedTimeSeries::getTimeSeriesSet()
{
  return m_timeSeries;
}

te::st::TimeSeries* te::st::LocatedTimeSeries::getTimeSeries(int idx) const
{
  return m_timeSeries[idx];
}

te::st::TimeSeries* te::st::LocatedTimeSeries::getTimeSeries(const std::string& name) const
{
  for(unsigned int i=0; i<m_timeSeries.size(); ++i)
  {
    if(m_timeSeries[i]->getProperty()->getName()==name)
      return m_timeSeries[i];
  }
  return 0;
}

void te::st::LocatedTimeSeries::setTimeSeries(const std::vector<TimeSeries*>& ts)
{
  te::common::FreeContents(m_timeSeries);
  m_timeSeries = ts;
}

void te::st::LocatedTimeSeries::addTimeSeries(TimeSeries* ts)
{
  m_timeSeries.push_back(ts);
}

te::gm::Geometry* te::st::LocatedTimeSeries::getSpatialLocation()
{
  return m_spLocation;
}

void te::st::LocatedTimeSeries::setSpatialLocation(te::gm::Geometry* spLocation)
{
  if(m_spLocation)
    delete m_spLocation;
  m_spLocation = spLocation;
}

int te::st::LocatedTimeSeries::getId()
{
  return m_Id;
}
        
void te::st::LocatedTimeSeries::setId(int id)
{
  m_Id = id;
}  

void te::st::LocatedTimeSeries::addObservation(const std::string& propName, 
                    te::dt::DateTime* phTime, te::dt::AbstractData* obsValue)
{
  for(unsigned int i=0; i<m_timeSeries.size(); ++i)
  {
    if(m_timeSeries[i]->getProperty()->getName()==propName)
      return m_timeSeries[i]->addObservation(phTime, obsValue);
  }
  return;  
}

te::st::LocatedTimeSeries::~LocatedTimeSeries()
{
  te::common::FreeContents(m_timeSeries);
  delete m_spLocation;
}


