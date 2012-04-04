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
  \file MovingObject.cpp

  \brief This file contains a class to represent a moving object.
*/

// TerraLib
#include "../../datatype/DateTime.h"
#include "../../datatype/DateTimePeriod.h"
#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/Geometry.h"

//ST
#include "MovingObject.h"
#include "Trajectory.h"
#include "MovingObjectIterator.h"
#include "../observation/SpatioTemporalObservationSet.h"
#include "../observation/Observation.h"
#include "../timeseries/TimeSeries.h"
#include "../interpolator/AbstractInterpolator.h"

te::st::MovingObject::MovingObject()
  : m_geomPropIdx(0),
    m_interpolator(0),
    m_id("-1")
{  
  m_observations = new SpatioTemporalObservationSet();
}

te::st::MovingObject::MovingObject(SpatioTemporalObservationSet* obs)
  : m_observations(obs),
    m_geomPropIdx(obs->getGeometryPropIdx()),
    m_interpolator(0),
    m_id("-1")
{  
}

te::st::MovingObject::MovingObject(SpatioTemporalObservationSet* obs, const std::string& id)
  : m_observations(obs),
    m_geomPropIdx(obs->getGeometryPropIdx()),
    m_interpolator(0),
    m_id(id)
{  
}

te::st::MovingObject::MovingObject(SpatioTemporalObservationSet* obs, const std::string& id, 
                                      AbstractInterpolator* interp)
  : m_observations(obs),
    m_geomPropIdx(obs->getGeometryPropIdx()),
    m_interpolator(interp),
    m_id(id)
{  
}

te::st::MovingObject::MovingObject(te::gm::GeometryProperty* prop, const std::string& id)
  : m_geomPropIdx(0),
    m_interpolator(0),
    m_id(id)
{
  m_observations = new SpatioTemporalObservationSet();
  m_observations->addProperty(prop);
}

te::st::MovingObjectIterator te::st::MovingObject::begin() 
{
  return MovingObjectIterator(m_observations->getObservations().begin(), m_geomPropIdx);
}

te::st::MovingObjectIterator te::st::MovingObject::end() 
{
  return MovingObjectIterator(m_observations->getObservations().end(), m_geomPropIdx);
}

te::gm::GeometryProperty* te::st::MovingObject::getProperty()
{
  return dynamic_cast<te::gm::GeometryProperty*>(m_observations->getProperty(m_geomPropIdx));
}

void te::st::MovingObject::setObservationSet(SpatioTemporalObservationSet* obs)
{
  m_observations->clear();
  m_observations = obs;
  m_geomPropIdx = obs->getGeometryPropIdx();
}

te::st::SpatioTemporalObservationSet* te::st::MovingObject::getObservationSet()
{
  return m_observations;
}

void te::st::MovingObject::addObservation(te::dt::DateTime* phTime, te::gm::Geometry* geom)
{
  Observation* ob = new Observation(phTime, geom);
  m_observations->addObservation(ob);
}

std::size_t te::st::MovingObject::size() const
{
  return m_observations->size();
}

te::gm::Geometry* te::st::MovingObject::getGeometry(te::dt::DateTime* t)
{
  te::gm::Geometry* result = 0;

  //Verify if there is an observation at the given time
  TimeToObservationMap::const_iterator it = m_observations->getObservations().find(t);
  if(it!=m_observations->getObservations().end())
  {
    result = dynamic_cast<te::gm::Geometry*>(it->second->getObservedValue(m_geomPropIdx));
    return dynamic_cast<te::gm::Geometry*>(result->clone());
  }
  
  if(m_interpolator==0)
    return result;

  //If not, use the interpolator
  std::vector<te::dt::AbstractData*> values; 
  m_interpolator->getValues(m_observations, t, values);
  result = dynamic_cast<te::gm::Geometry*>(values[m_geomPropIdx]);
  
  //delete 
  for(unsigned int i = 0; i<values.size(); ++i)
  {
    if(i!=m_geomPropIdx)
      delete values[i];
  }
  return result;
}

te::st::AbstractInterpolator* te::st::MovingObject::getInterpolator() const
{
  return m_interpolator;
}

void te::st::MovingObject::setInterpolator(te::st::AbstractInterpolator* interp)
{
  m_interpolator = interp;
}

std::string te::st::MovingObject::getId() const
{
  return m_id;
}       

void te::st::MovingObject::setId(const std::string& id)
{
  m_id = id;
} 

te::dt::DateTimePeriod* te::st::MovingObject::temporalExtent()
{
  return m_observations->temporalExtent();
}

te::st::TimeSeries* te::st::MovingObject::distance(MovingObject* other)
{
  te::dt::Property* prop = new te::dt::SimpleProperty("distance",13); //double property
  te::st::TimeSeries* result = new te::st::TimeSeries(prop);
  
  TimeToObservationMap::const_iterator itb, ite;
  itb = m_observations->getObservations().begin();
  ite = m_observations->getObservations().end();
  
  while(itb!=ite)
  {
    te::gm::Geometry* geom1 = dynamic_cast<te::gm::Geometry*>(itb->second->getObservedValue(m_geomPropIdx));
    te::gm::Geometry* geom2 = other->getGeometry(itb->first);
    if(geom2!=0)
    {
      double distValue = geom1->distance(geom2);
      te::dt::Double* dist = new te::dt::Double(distValue); 
      te::dt::DateTime* datetime = dynamic_cast<te::dt::DateTime*>(itb->first->clone()); 
      delete geom2;

      //add in the time series
      result->addObservation(datetime, dist);
    }    
    ++itb;
  }
  return result;
}

void te::st::MovingObject::intersection(te::gm::Geometry* geom, std::vector<Trajectory*>& output)
{
  std::vector<Observation*> result;
  m_observations->intersection(geom, result);
  
  if(result.size()<1)
    return;

  //order result by time
  TimeToObservationMap ordResult;
  for(unsigned int i=0; i<result.size(); ++i)
    ordResult.insert(std::pair<te::dt::DateTime*, te::st::Observation*>(result[i]->getTime(), result[i])); 
    
  TimeToObservationMap::iterator itResultBegin = ordResult.begin();
  TimeToObservationMap::iterator itResultEnd = ordResult.end();
  TimeToObservationMap::iterator itObsFind = m_observations->getObservations().find(itResultBegin->first);
  TimeToObservationMap::iterator itObsEnd = m_observations->getObservations().end();
  TimeToObservationMap::iterator itTrajBegin, itTrajEnd;
  
  while((itObsFind!=itObsEnd) && (itResultBegin!=itResultEnd))
  {
    itTrajBegin = itObsFind;

    //TESTE
    std::string s1 = itObsFind->first->toString();
    std::string s2 = itResultBegin->first->toString();

    while((itObsFind!=itObsEnd) && (itResultBegin!=itResultEnd) && 
          (itObsFind->first == itResultBegin->first))
    {
      
      //TESTE
      s1 = itObsFind->first->toString();
      s2 = itResultBegin->first->toString();
      
      ++itObsFind;
      ++itResultBegin;
    }
    
    //finalize a trajectory
    itTrajEnd = --itResultBegin;
    ++itResultBegin;

    //TESTE
    s1 = itObsFind->first->toString();
    s2 = itResultBegin->first->toString();
   
   //TESTE
    std::string t1 = itTrajBegin->first->toString();
    std::string t2 = itTrajEnd->first->toString();

    Trajectory* traj = new Trajectory(MovingObjectIterator(itTrajBegin), 
                       MovingObjectIterator(itTrajEnd), this);    
    output.push_back(traj);

    if(itResultBegin!=itResultEnd)   
      itObsFind = m_observations->getObservations().find(itResultBegin->first);     
  }  
  return;
}

void te::st::MovingObject::difference(te::gm::Geometry* geom, std::vector<Trajectory*>& output)
{
  std::vector<Observation*> result;
  m_observations->difference(geom, result);
  
  if(result.size()<1)
    return;

   //order result by time
  TimeToObservationMap ordResult;
  for(unsigned int i=0; i<result.size(); ++i)
    ordResult.insert(std::pair<te::dt::DateTime*, te::st::Observation*>(result[i]->getTime(), result[i])); 
    
  TimeToObservationMap::iterator itResultBegin = ordResult.begin();
  TimeToObservationMap::iterator itResultEnd = ordResult.end();
  TimeToObservationMap::iterator itObsFind = m_observations->getObservations().find(itResultBegin->first);
  TimeToObservationMap::iterator itObsEnd = m_observations->getObservations().end();
  TimeToObservationMap::iterator itTrajBegin, itTrajEnd;
  
  while((itObsFind!=itObsEnd) && (itResultBegin!=itResultEnd))
  {
    itTrajBegin = itObsFind;

    //TESTE
    std::string s1 = itObsFind->first->toString();
    std::string s2 = itResultBegin->first->toString();

    while((itObsFind!=itObsEnd) && (itResultBegin!=itResultEnd) && 
          (itObsFind->first == itResultBegin->first))
    {
      
      //TESTE
      s1 = itObsFind->first->toString();
      s2 = itResultBegin->first->toString();
      
      ++itObsFind;
      ++itResultBegin;
    }
    
    //finalize a trajectory
    itTrajEnd = --itResultBegin;
    ++itResultBegin;

    //TESTE
    s1 = itObsFind->first->toString();
    s2 = itResultBegin->first->toString();
   
    Trajectory* traj = new Trajectory(MovingObjectIterator(itTrajBegin), 
                       MovingObjectIterator(itTrajEnd), this);    
    output.push_back(traj);

    if(itResultBegin!=itResultEnd)   
      itObsFind = m_observations->getObservations().find(itResultBegin->first);     
  }  
  return;
}

te::st::MovingObject::~MovingObject()
{
  delete m_observations;
  delete m_interpolator;
}


