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
  \file TimeSeries.cpp

  \brief This file contains a class to represent a time series.
*/

// TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimePeriod.h"
#include "../../../datatype/DateTimeInstant.h"
#include "../../../datatype/DateTimeUtils.h"
#include "../../../datatype/SimpleData.h"
#include "../../../datatype/AbstractData.h"
#include "../../../geometry/Geometry.h"

//ST
#include "TimeSeries.h"
#include "TimeSeriesObservation.h"
#include "../interpolator/AbstractTimeSeriesInterp.h"
#include "../interpolator/NearestValueAtTimeInterp.h"

te::st::TimeSeries::TimeSeries()
  : m_observations(),
    m_interpolator(&NearestValueAtTimeInterp::getInstance()),
    m_location(),
    m_id()
{  
}

te::st::TimeSeries::TimeSeries(const std::string& id)
  : m_observations(),
    m_interpolator(&NearestValueAtTimeInterp::getInstance()),
    m_location(),
    m_id(id)
{  
}

te::st::TimeSeries::TimeSeries(AbstractTimeSeriesInterp* interp, const std::string& id)
  : m_observations(),
    m_interpolator(interp),
    m_location(),
    m_id(id)
{  
}

te::st::TimeSeries::TimeSeries( AbstractTimeSeriesInterp* interp, const std::string& id, 
                                te::gm::Geometry* geom)
  : m_observations(),
    m_interpolator(interp),
    m_location(geom),
    m_id(id)
{  
}

te::st::TimeSeries::TimeSeries( const TimeSeriesObservationSet& obs, AbstractTimeSeriesInterp* interp, 
                                const std::string& id, te::gm::Geometry* geom)
  : m_observations(obs),
    m_interpolator(interp),
    m_location(geom),
    m_id(id)
{  
}

te::st::TimeSeries::TimeSeries(const TimeSeriesPatch& patch, AbstractTimeSeriesInterp* interp, 
                   const std::string& id, te::gm::Geometry* geom)
  : m_observations(),
    m_interpolator(interp),
    m_location(geom),
    m_id(id)
{
  TimeSeriesIterator it = patch.begin();
  while(it!=patch.end())
  {
    //add shared pointers 
    m_observations.insert(*it);  
    ++it;
  }
}
te::st::TimeSeries::TimeSeries(const TimeSeries& ts)
  : m_observations(ts.m_observations),
    m_interpolator(ts.m_interpolator),
    m_location(static_cast<te::gm::Geometry*>(ts.m_location->clone())),
    m_id(ts.m_id)
{
}

te::st::TimeSeries& te::st::TimeSeries::operator=(const TimeSeries& other)
{
  if(this != &other)
  {
    m_observations = other.m_observations;
    m_interpolator = other.m_interpolator;
    m_location.reset(static_cast<te::gm::Geometry*>(other.m_location->clone()));
    m_id = other.m_id;
  }
  return *this;
}

te::st::TimeSeries* te::st::TimeSeries::clone() const
{
  TimeSeries* result = new TimeSeries(m_interpolator, m_id, static_cast<te::gm::Geometry*>(m_location->clone()));
  TimeSeriesObservationSet::const_iterator it = m_observations.begin();
  TimeSeriesObservationSet::const_iterator ite = m_observations.end();
  while(it != ite)
  {
    te::dt::DateTime* t = static_cast<te::dt::DateTime*>(it->getTime()->clone());
    te::dt::AbstractData* v = it->getValue()->clone();
    result->add(t,v);
    ++it;
  }
  return result;
}

const te::st::TimeSeriesObservationSet& te::st::TimeSeries::getObservations() const
{
  return m_observations;
}

te::st::AbstractTimeSeriesInterp* te::st::TimeSeries::getInterpolator() const
{
  return m_interpolator;
}

void te::st::TimeSeries::setInterpolator(AbstractTimeSeriesInterp* interp)
{
  m_interpolator = interp;
}

std::string te::st::TimeSeries::getId() const
{
  return m_id;
}
        
void te::st::TimeSeries::setId(const std::string& id)
{
  m_id = id;
}

te::gm::Geometry* te::st::TimeSeries::getLocation() const
{
  return m_location.get();
} 

void te::st::TimeSeries::setLocation(te::gm::Geometry* geom)
{
  m_location.reset(geom);
}

void te::st::TimeSeries::add(te::dt::DateTime* time, te::dt::AbstractData* value)
{
  TimeSeriesObservation item(time, value);
  m_observations.insert(item);
}   

void te::st::TimeSeries::add(te::dt::DateTime* time, double value)
{
  te::dt::Double* v = new te::dt::Double(value);
  TimeSeriesObservation item(time, v);
  m_observations.insert(item);
}

void te::st::TimeSeries::add(te::dt::DateTime* time, int value)
{
  te::dt::Int32* v = new te::dt::Int32(value);
  TimeSeriesObservation item(time, v);
  m_observations.insert(item);
}

void te::st::TimeSeries::add(te::dt::DateTime* time, const std::string& value)
{
  te::dt::String* v = new te::dt::String(value);
  TimeSeriesObservation item(time, v);
  m_observations.insert(item);
}

void te::st::TimeSeries::add(const TimeSeriesObservation& item)
{
  m_observations.insert(item);
}

std::size_t te::st::TimeSeries::size() const
{
  return m_observations.size();
}

te::st::TimeSeriesIterator te::st::TimeSeries::begin() const
{
  TimeSeriesObservationSet::const_iterator it = m_observations.begin();
  return TimeSeriesIterator(it);
}

te::st::TimeSeriesIterator te::st::TimeSeries::end() const
{
  TimeSeriesObservationSet::const_iterator it = m_observations.end();
  return TimeSeriesIterator(it);
}

te::st::TimeSeriesIterator te::st::TimeSeries::at(te::dt::DateTime* t) const
{
  TimeSeriesObservation item(t,0);
  TimeSeriesObservationSet::const_iterator it = m_observations.find(item);
  return TimeSeriesIterator(it);
}

std::auto_ptr<te::dt::AbstractData> te::st::TimeSeries::getValue(te::dt::DateTime* t) const
{
  TimeSeriesObservation item(t,0);
  TimeSeriesObservationSet::const_iterator it = m_observations.find(item);
  TimeSeriesObservationSet::const_iterator ite = m_observations.end();
  if(it!=ite)
    return std::auto_ptr<te::dt::AbstractData>(it->getValue()->clone());
  return std::auto_ptr<te::dt::AbstractData>(m_interpolator->estimate(*this,t));
}

double te::st::TimeSeries::getDouble(te::dt::DateTime* t) const
{
  std::auto_ptr<te::dt::AbstractData> v(getValue(t));
  return atof(v->toString().c_str());
}

int te::st::TimeSeries::getInt(te::dt::DateTime* t) const
{
  std::auto_ptr<te::dt::AbstractData> v(getValue(t));
  return atoi(v->toString().c_str());
}

std::string te::st::TimeSeries::getString(te::dt::DateTime* t) const
{
  std::auto_ptr<te::dt::AbstractData> v(getValue(t));
  return v->toString();
} 

std::auto_ptr<te::dt::DateTimePeriod> te::st::TimeSeries::getTemporalExtent() const
{
  TimeSeriesObservationSet::iterator it1 = m_observations.begin();
  TimeSeriesObservationSet::reverse_iterator it2 = m_observations.rbegin();

  te::dt::DateTime* bt = it1->getTime();
  te::dt::DateTime* et = it2->getTime();
  //This function does not take the ownership of the given times
  return std::auto_ptr<te::dt::DateTimePeriod> (te::dt::GetTemporalExtent(bt, et)); 
}

void te::st::TimeSeries::getValueExtent(double& minValue, double& maxValue)
{
  minValue = m_observations.get<1>().begin()->getDouble();
  minValue = m_observations.get<1>().rbegin()->getDouble();
  return;
}

void te::st::TimeSeries::getValueExtent(int& minValue, int& maxValue)
{
  minValue = m_observations.get<1>().begin()->getInt();
  minValue = m_observations.get<1>().rbegin()->getInt();
}

void te::st::TimeSeries::getValueExtent(std::string& minValue, std::string& maxValue)
{
  minValue = m_observations.get<2>().begin()->getString();
  minValue = m_observations.get<2>().rbegin()->getString();
}

te::st::TimeSeriesPatch te::st::TimeSeries::getPatch(const te::dt::DateTime& dt, 
                                   te::dt::TemporalRelation r) const
{
  //Note: the end iterator of a patch points to the position AFTER the last required observation 
  TimeSeriesObservationSet::const_iterator itb = m_observations.end();
  TimeSeriesObservationSet::const_iterator ite = m_observations.end();
    
  if(r==te::dt::AFTER) //2
  {
    TimeSeriesObservation item(static_cast<te::dt::DateTime*>(dt.clone()),0);
    itb = m_observations.upper_bound(item);
    return TimeSeriesPatch(itb,ite);
  }
  if(r==(te::dt::AFTER | te::dt::EQUALS)) // 2 OU 8 = 10 
  {
    TimeSeriesObservation item(static_cast<te::dt::DateTime*>(dt.clone()),0);
    itb = m_observations.find(item);
    if(itb==m_observations.end())
      itb = m_observations.upper_bound(item);
    return TimeSeriesPatch(itb,ite);
  }
  if(r==te::dt::BEFORE) // 1
  {
    TimeSeriesObservation item(static_cast<te::dt::DateTime*>(dt.clone()),0);
    itb = m_observations.begin();
    ite = m_observations.find(item);
    if(ite==m_observations.end())
      ite = m_observations.upper_bound(item);
    return TimeSeriesPatch(itb,ite);
  }
  if(r==(te::dt::BEFORE | te::dt::EQUALS)) // 1 OU 8 = 9
  {
    TimeSeriesObservation item(static_cast<te::dt::DateTime*>(dt.clone()),0);
    itb = m_observations.begin();
    ite = m_observations.upper_bound(item); 
    return TimeSeriesPatch(itb,ite);
  }
  if(r==te::dt::DURING) //4
  {
    std::auto_ptr<te::dt::DateTimePeriod> period(static_cast<te::dt::DateTimePeriod*>(dt.clone()));
    TimeSeriesObservation item1(period->getInitialInstant(),0);
    TimeSeriesObservation item2(period->getInitialInstant(),0);
    itb = m_observations.find(item1);
    if(itb==m_observations.end())
      itb = m_observations.upper_bound(item1);
    ite = m_observations.upper_bound(item2); 
    return TimeSeriesPatch(itb,ite);  
  }
  if(r==te::dt::EQUALS) //8
  {
    TimeSeriesObservation item(static_cast<te::dt::DateTime*>(dt.clone()),0);
    std::pair<TimeSeriesObservationSet::const_iterator, TimeSeriesObservationSet::const_iterator> itPair;
    itPair = m_observations.equal_range(item);
    itb = itPair.first;
    ite = itPair.second;
    if(ite!= m_observations.end())
      ++ite;
    return TimeSeriesPatch(itb,ite);
  }
  
  return TimeSeriesPatch(itb,ite);  
}

void te::st::TimeSeries::getPatches( const double& v, te::dt::BasicRelation r, 
                          std::vector<TimeSeriesPatch>& result) const
{
  //Note: the end iterator of a patch points to the position AFTER the last required observation 
  std::map<te::dt::DateTime*, te::dt::AbstractData*, te::dt::CompareDateTime> resultTS;
  getResultOrderedByTime(v, r, resultTS);
   
  std::map<te::dt::DateTime*, te::dt::AbstractData*, 
      te::dt::CompareDateTime>::const_iterator itResultBegin = resultTS.begin();
  std::map<te::dt::DateTime*, te::dt::AbstractData*, 
      te::dt::CompareDateTime>::const_iterator itResultEnd = resultTS.end();
  
  TimeSeriesObservation item(static_cast<te::dt::DateTime*>(itResultBegin->first->clone()),0);
  TimeSeriesObservationSet::const_iterator itOriginBegin = m_observations.find(item);
  TimeSeriesObservationSet::const_iterator itOriginEnd = m_observations.end();

  TimeSeriesObservationSet::const_iterator patchBegin;
  TimeSeriesObservationSet::const_iterator patchEnd;

  while(itResultBegin!=itResultEnd && itOriginBegin!=itOriginEnd)
  {
     patchBegin = itOriginBegin; //The beginning of a patch

     //Enquanto os ponteiros forem iguais estamos no meio de um patch
     while( (itResultBegin!=itResultEnd) && (itOriginBegin!=itOriginEnd) && 
            (itResultBegin->first == itOriginBegin->getTime())) 
     {
        patchEnd = itOriginBegin;
        ++itResultBegin;
        ++itOriginBegin;
     }

     //get the patch
     //the end of a patch MUST point to the position AFTER the required observations 
     if(patchEnd!=itOriginEnd)
      ++patchEnd;
     TimeSeriesPatch r(patchBegin, patchEnd);
     result.push_back(r);

     //update the origin begin
     if(itResultBegin!=itResultEnd)
     {
       item.setTime(static_cast<te::dt::DateTime*>(itResultBegin->first->clone()));
       itOriginBegin = m_observations.find(item);
     }
  } //while
  
  return;  
}

void te::st::TimeSeries::getPatches( const double& v, te::dt::BasicRelation r, 
                            const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                            std::vector<TimeSeriesPatch>& result) const
{
  TimeSeriesPatch patch = getPatch(dt,tr);
  TimeSeries ts(patch,m_interpolator,"-1",0);
  std::vector<TimeSeriesPatch> patches;
  ts.getPatches(v,r,patches);

  std::vector<TimeSeriesPatch>::const_iterator it = patches.begin();
  TimeSeriesObservationSet::const_iterator ite = m_observations.end();

  while(it!=patches.end())
  {
    TimeSeriesIterator itAuxBegin = (*it).begin();
    TimeSeriesIterator itAuxEnd = (*it).end();

    TimeSeriesIterator itBegin(ite);
    TimeSeriesIterator itEnd(ite); 

    //Get the first position
    if(itAuxBegin!=ts.end())
    {
      te::dt::DateTime* t = itAuxBegin.getTime();
      itBegin = at(t);
    }

    //Get the last position
    if(itAuxEnd==ts.end())
      --itAuxEnd;

    te::dt::DateTime* t = itAuxEnd.getTime();
    itEnd = at(t);
    ++itEnd;

    //Get the patch
    if(itBegin!=itEnd)
    {
      TimeSeriesPatch p(itBegin, itEnd);
      result.push_back(p);
    }

    ++it;
  }
  
  return;
}

void te::st::TimeSeries::getResultOrderedByTime(const double& v, te::dt::BasicRelation r, 
                             std::map<te::dt::DateTime*, te::dt::AbstractData*, te::dt::CompareDateTime>& result) const
{
  TimeSeriesByValue::const_iterator itResultBeg = m_observations.get<1>().end();
  TimeSeriesByValue::const_iterator itResultEnd = m_observations.get<1>().end();
  
  if(r==te::dt::LESS) //1
  {
    itResultBeg = m_observations.get<1>().begin();
    itResultEnd = m_observations.get<1>().find(v);
    if(itResultEnd==m_observations.get<1>().end())
      itResultEnd = m_observations.get<1>().upper_bound(v);
  } 
  else if(r==te::dt::MORE) //2
  {
    itResultBeg = m_observations.get<1>().upper_bound(v);
  }
  else if(r==te::dt::EQUAL) //4
  {
    std::pair<TimeSeriesByValue::const_iterator, TimeSeriesByValue::const_iterator> itPair;
    itPair = m_observations.get<1>().equal_range(v);
    itResultBeg = itPair.first;
    itResultEnd = itPair.second;
    if(itResultEnd!= m_observations.get<1>().end())
      ++itResultEnd;
  }
  else if( r == (te::dt::LESS | te::dt::EQUAL)) // 1 (001) | 4 (100) = 5 (101)
  {
    itResultBeg = m_observations.get<1>().begin();
    itResultEnd = m_observations.get<1>().upper_bound(v); 
  }
  else if( r == (te::dt::MORE | te::dt::EQUAL)) // 2 (010) | 4 (100) = 6 (110)
  {
    itResultBeg = m_observations.get<1>().find(v);
    if(itResultBeg==m_observations.get<1>().end())
      itResultBeg = m_observations.get<1>().upper_bound(v);
  }
  
  //Put the result in a map orderded by time
  while(itResultBeg!=itResultEnd)
  {
    result.insert(std::pair<te::dt::DateTime*,te::dt::AbstractData*>(itResultBeg->getTime(), itResultBeg->getValue()));
    ++itResultBeg;
  }
  return;
}

te::st::TimeSeries::~TimeSeries()
{  
}


