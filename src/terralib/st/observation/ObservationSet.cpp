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
  \file ObservationSet.cpp

  \brief It contains a class to represent an observation set.
 */

// TerraLib
#include "../../datatype/Property.h"
#include "../../datatype/DateTimeUtils.h"
#include "../../datatype/DateTimePeriod.h"
#include "../../common/STLUtils.h"

//ST
#include "Observation.h"
#include "ObservationSet.h"
#include "ObservationSetIterator.h"

te::st::ObservationSet::ObservationSet()
{  
}

te::st::ObservationSet::ObservationSet(const std::vector<Observation*>& obs, 
                                          const std::vector<te::dt::Property*>& props)
  : m_properties(props)
{ 
  //create a internal time ordenation
  for(unsigned int i=0; i<obs.size(); ++i)
    addObservation(obs[i]); 
}

te::st::ObservationSet::ObservationSet(const std::vector<te::dt::Property*>& props)
  : m_properties(props)
{
}

te::st::TimeToObservationMap& te::st::ObservationSet::getObservations() 
{
  return m_observations;
}

te::st::ObservationSetIterator te::st::ObservationSet::begin() 
{
  return ObservationSetIterator(m_observations.begin());
}

te::st::ObservationSetIterator te::st::ObservationSet::end() 
{
  return ObservationSetIterator(m_observations.end());
}

void te::st::ObservationSet::setObservations(const std::vector<Observation*>& obs)
{
  clear();
  //create a internal time ordenation
  for(unsigned int i=0; i<obs.size(); ++i)
    addObservation(obs[i]); 
}

void te::st::ObservationSet::clear()
{
  TimeToObservationMap::const_iterator itb, ite;
  itb = m_observations.begin();
  ite = m_observations.end();
  while(itb!=ite)
  {
    delete itb->second;
    ++itb;
  }
  m_observations.clear();
}

std::size_t te::st::ObservationSet::size() const
{
  return m_observations.size();
}

void te::st::ObservationSet::addObservation(Observation* ob)
{
  m_observations.insert(std::pair<te::dt::DateTime*, te::st::Observation*>(ob->getTime(), ob)); 
  ob->setParent(this);
}

void te::st::ObservationSet::addProperty(te::dt::Property* prop)
{
  m_properties.push_back(prop);
}

std::vector<te::dt::Property*>& te::st::ObservationSet::getProperties() 
{
  return m_properties;
}

te::dt::Property* te::st::ObservationSet::getProperty(int idx)
{
  return m_properties[idx];
}

void te::st::ObservationSet::setProperties(const std::vector<te::dt::Property*>& props)
{
  te::common::FreeContents(m_properties);
  m_properties = props;
}

te::dt::DateTimePeriod* te::st::ObservationSet::temporalExtent()
{
  te::dt::DateTime* tbegin =  m_observations.begin()->first;
  te::dt::DateTime* tend =  m_observations.rbegin()->first;
  te::dt::DateTimePeriod* output = te::dt::GetTemporalExtent(tbegin, tend);
  return output;
}

void te::st::ObservationSet::after(te::dt::DateTime* dt, std::vector<Observation*>& result)
{
  TimeToObservationMap::const_iterator itb, ite;
  itb = m_observations.upper_bound(dt);
  ite = m_observations.end();
  
  //Copy the observations
  while(itb!=ite)
  {
    Observation* ob = itb->second;
    result.push_back(ob); 
    ++itb;
  } 
}

void te::st::ObservationSet::before(te::dt::DateTime* dt, std::vector<Observation*>& result)
{
  TimeToObservationMap::const_iterator itb, ite;
  itb = m_observations.begin();
  ite = m_observations.lower_bound(dt);
  
  //Copy the observations
  while(itb!=ite)
  {
    Observation* ob = itb->second;
    result.push_back(ob); 
    ++itb;
  } 
}

void te::st::ObservationSet::during(te::dt::DateTime* initialTime, te::dt::DateTime* finalTime, std::vector<Observation*>& result)
{
  TimeToObservationMap::const_iterator itb, ite;
  itb = m_observations.lower_bound(initialTime);
  ite = m_observations.upper_bound(finalTime);

  while(itb!=ite)
  {
    Observation* ob = itb->second;
    result.push_back(ob); 
    ++itb;
  }
}

void te::st::ObservationSet::at(te::dt::DateTime* dt, std::vector<Observation*>& result)
{
  TimeToObservationMap::const_iterator it;
  std::pair<TimeToObservationMap::const_iterator, TimeToObservationMap::const_iterator> itPair;
  itPair = m_observations.equal_range(dt);

  //Copy the observations
  for(it=itPair.first; it!=itPair.second; ++it)
  {
    Observation* ob = it->second;
    result.push_back(ob); 
  }
}

te::st::ObservationSet::~ObservationSet()
{
  clear();
  te::common::FreeContents(m_properties);
}


