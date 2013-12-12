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
  \file Coverage.cpp

  \brief This file contains an abstract class to represent a coverage.
*/

//TerraLib
#include "../../../geometry/Polygon.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Utils.h"
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimeInstant.h"
#include "../../../datatype/DateTimeUtils.h"

//ST
#include "CoverageSeries.h"
#include "CoverageSeriesObservation.h"
#include "../interpolator/AbstractCoverageSeriesInterp.h"
#include "../interpolator/NearestCoverageAtTimeInterp.h"

te::st::CoverageSeries::CoverageSeries() :
  m_observations(), 
  m_interpolator(&NearestCoverageAtTimeInterp::getInstance()),
  m_cvtype(te::st::UNKNOWN),
  m_sextent(0)
{
}

te::st::CoverageSeries::CoverageSeries(const CoverageSeriesObservationSet& obs, 
                        AbstractCoverageSeriesInterp* interp, te::gm::Geometry* se, CoverageType t) :
  m_observations(obs), 
  m_interpolator(interp),
  m_cvtype(t),
  m_sextent(se)
{
}
        
const te::st::CoverageSeriesObservationSet& te::st::CoverageSeries::getObservations() const
{
  return m_observations;
}
        
te::st::CoverageType te::st::CoverageSeries::getType() const
{
  return m_cvtype;
}

te::gm::Geometry* te::st::CoverageSeries::getSpatialExtent() const
{
  return m_sextent.get();
}

std::auto_ptr<te::dt::DateTimePeriod> te::st::CoverageSeries::getTemporalExtent() const
{
  te::dt::DateTime* bt = m_observations.begin()->first.get();
  te::dt::DateTime* et = m_observations.rbegin()->first.get();
  //This function does not take the ownership of the given times
  return std::auto_ptr<te::dt::DateTimePeriod>(te::dt::GetTemporalExtent(bt, et)); 
}

void te::st::CoverageSeries::add(te::dt::DateTime* time, te::st::Coverage* cv)
{
  te::dt::DateTimeShrPtr t(time);
  CoverageShrPtr c(cv);
  CoverageSeriesObservation obs(t,c);
  add(obs);
}

void te::st::CoverageSeries::add(const te::st::CoverageSeriesObservation& o)
{
  m_observations.insert(o);
}
                
std::size_t te::st::CoverageSeries::size() const
{
  return m_observations.size();
}

te::st::CoverageSeriesIterator te::st::CoverageSeries::begin() const
{
  return te::st::CoverageSeriesIterator(m_observations.begin());
}

te::st::CoverageSeriesIterator te::st::CoverageSeries::end() const
{
  return te::st::CoverageSeriesIterator(m_observations.end());
}

te::st::CoverageSeriesIterator te::st::CoverageSeries::at(te::dt::DateTime* t) const
{
  te::dt::DateTimeShrPtr aux(static_cast<te::dt::DateTime*>(t->clone()));
  CoverageSeriesObservationSet::const_iterator itcs = m_observations.find(aux);
  CoverageSeriesIterator it(itcs); 
  return it;
}
        
std::auto_ptr<te::st::Coverage> te::st::CoverageSeries::getCoverage(te::dt::DateTime* t) const
{
  te::dt::DateTimeShrPtr aux(static_cast<te::dt::DateTime*>(t->clone()));
  CoverageSeriesObservationSet::const_iterator it = m_observations.find(aux);
  if(it!=m_observations.end())
    return std::auto_ptr<te::st::Coverage>(it->second->clone());

  return std::auto_ptr<te::st::Coverage>(m_interpolator->estimate(*this,t)); 
}
                
std::auto_ptr<te::st::TimeSeries> 
te::st::CoverageSeries::getTimeSeries(const te::gm::Point& l, unsigned int p) const
{
  std::auto_ptr<te::st::TimeSeries> result(new te::st::TimeSeries());
  result->setLocation(static_cast<te::gm::Geometry*>(l.clone()));

  CoverageSeriesObservationSet::const_iterator it = m_observations.begin();
  while(it!=m_observations.end())
  {
    te::dt::DateTime* dt = static_cast<te::dt::DateTime*>(it->first->clone());
    std::auto_ptr<te::dt::AbstractData> value(it->second->getValue(l,p));
    result->add(dt, value.release());
    ++it;
  }
  return result;
}

void te::st::CoverageSeries::getTimeSeries(const te::gm::Point& l, boost::ptr_vector<TimeSeries>& r) const
{
  CoverageSeriesObservationSet::const_iterator it = m_observations.begin();
  if(it==m_observations.end())
    return;

  unsigned int numts = it->second->getNumberOfProperties();
  for(unsigned int i=0; i<numts; ++i)
  {
    std::auto_ptr<te::st::TimeSeries> ts(new te::st::TimeSeries());
    ts->setLocation(static_cast<te::gm::Geometry*>(l.clone()));
    r.push_back(ts);
  }
  
  while(it!=m_observations.end())
  {
     boost::ptr_vector<te::dt::AbstractData> values;
     it->second->getValue(l,values);
     for(unsigned int i=0; i<numts; ++i)
       r[i].add(static_cast<te::dt::DateTime*>(it->first->clone()), &values[i]);
     values.release();
    ++it;
  }
  return;
}

void te::st::CoverageSeries::getTimeSeries(const te::gm::Polygon& l, unsigned int p, boost::ptr_vector<TimeSeries>& r) const
{
  CoverageSeriesObservationSet::const_iterator it = m_observations.begin();
  if(it==m_observations.end())
    return;

  //==== First iteration: creates the time series and add the first values
  //all values inside the polygon at time dt
  boost::ptr_vector<te::dt::AbstractData> values;
  it->second->getValue(l,p,values);

  for(unsigned int i=0; i<values.size(); ++i)
  {
    std::auto_ptr<te::st::TimeSeries> result(new te::st::TimeSeries());
    result->setLocation(static_cast<te::gm::Geometry*>(l.clone()));
    result->add(static_cast<te::dt::DateTime*>(it->first->clone()), &values[i]);
    r.push_back(result);
  }
  values.release();
  ++it;
  
  //==== Next iterations: add values into the time series
  while(it!=m_observations.end())
  {
    //all values inside the polygon at time dt
    it->second->getValue(l,p,values);

    for(unsigned int i=0; i<values.size(); ++i)
      r[i].add(static_cast<te::dt::DateTime*>(it->first->clone()), &values[i]);

    values.release();
    ++it;
  }
  return;
}

void te::st::CoverageSeries::getTimeSeries(const te::gm::Envelope& e, unsigned int p, 
                                            boost::ptr_vector<TimeSeries>& r) const
{
  std::auto_ptr<te::gm::Geometry> geom(te::gm::GetGeomFromEnvelope(&e, 0));
  te::gm::Polygon* pol = static_cast<te::gm::Polygon*>(geom.get());
  getTimeSeries(*pol, p, r); 
  return;
}

void te::st::CoverageSeries::getTimeSeries(const te::gm::Polygon& l, boost::ptr_vector<TimeSeries>& r) const
{
  CoverageSeriesObservationSet::const_iterator it = m_observations.begin();
  if(it==m_observations.end())
    return;

  //==== First iteration: creates the time series and add the first values
  //all values inside the polygon at time dt
  boost::ptr_vector<te::dt::AbstractData> values;
  it->second->getValue(l,values);

  for(unsigned int i=0; i<values.size(); ++i)
  {
    std::auto_ptr<te::st::TimeSeries> result(new te::st::TimeSeries());
    result->setLocation(static_cast<te::gm::Geometry*>(l.clone()));
    result->add(static_cast<te::dt::DateTime*>(it->first->clone()), &values[i]);
    r.push_back(result);
  }
  values.release();
  ++it;
  
  //==== Next iterations: add values into the time series
  while(it!=m_observations.end())
  {
    //all values inside the polygon at time dt
    it->second->getValue(l,values);

    for(unsigned int i=0; i<values.size(); ++i)
      r[i].add(static_cast<te::dt::DateTime*>(it->first->clone()), &values[i]);

    values.release();
    ++it;
  }
  return;
}

void te::st::CoverageSeries::getTimeSeries(const te::gm::Envelope& e, boost::ptr_vector<TimeSeries>& r) const
{
  std::auto_ptr<te::gm::Geometry> geom(te::gm::GetGeomFromEnvelope(&e, 0));
  te::gm::Polygon* pol = static_cast<te::gm::Polygon*>(geom.get());
  getTimeSeries(*pol, r); 
  return;
}

std::auto_ptr<te::st::TimeSeries> te::st::CoverageSeries::getTimeSeries(const te::gm::Point& l, const te::dt::DateTime& t,
                                                        te::dt::TemporalRelation tr, unsigned int p) const
{
  std::auto_ptr<te::st::CoverageSeries> aux = getPatch(t,tr);
  return aux->getTimeSeries(l,p);
}

void te::st::CoverageSeries::getTimeSeries(const te::gm::Point& l, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const
{
  std::auto_ptr<te::st::CoverageSeries> aux = getPatch(t,tr);
  return aux->getTimeSeries(l,r);
}

void te::st::CoverageSeries::getTimeSeries(const te::gm::Polygon& l, unsigned int p, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const
{
  std::auto_ptr<te::st::CoverageSeries> aux = getPatch(t,tr);
  return aux->getTimeSeries(l,p,r);
}


void te::st::CoverageSeries::getTimeSeries(const te::gm::Envelope& e, unsigned int p, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const
{
  std::auto_ptr<te::st::CoverageSeries> aux = getPatch(t,tr);
  return aux->getTimeSeries(e,p,r);
}


void te::st::CoverageSeries::getTimeSeries(const te::gm::Polygon& l, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const
{
  std::auto_ptr<te::st::CoverageSeries> aux = getPatch(t,tr);
  return aux->getTimeSeries(l,r);
}

void te::st::CoverageSeries::getTimeSeries(const te::gm::Envelope& e, const te::dt::DateTime& t,
                           te::dt::TemporalRelation tr, boost::ptr_vector<TimeSeries>& r) const
{
  std::auto_ptr<te::st::CoverageSeries> aux = getPatch(t,tr);
  return aux->getTimeSeries(e,r);
}

std::auto_ptr<te::st::CoverageSeries> 
te::st::CoverageSeries::getPatch(const te::dt::DateTime& dt, te::dt::TemporalRelation r) const
{
  std::auto_ptr<te::st::CoverageSeries> cs (new CoverageSeries());
  //Note: the end iterator of a patch points to the position AFTER the last required observation 
  CoverageSeriesObservationSet::const_iterator itb = m_observations.end();
  CoverageSeriesObservationSet::const_iterator ite = m_observations.end();

  te::dt::DateTimeShrPtr shrdt(static_cast<te::dt::DateTime*>(dt.clone()));

  if(r==te::dt::AFTER) //2
  {
    itb = m_observations.upper_bound(shrdt);
  }
  else if(r==(te::dt::AFTER | te::dt::EQUALS)) // 2 OU 8 = 10 
  {
    itb = m_observations.find(shrdt);
    if(itb==m_observations.end())
      itb = m_observations.upper_bound(shrdt);
  }
  else if(r==te::dt::BEFORE) // 1
  {
    itb = m_observations.begin();
    ite = m_observations.find(shrdt);
    if(ite==m_observations.end())
      ite = m_observations.upper_bound(shrdt);
  }
  else if(r==(te::dt::BEFORE | te::dt::EQUALS)) // 1 OU 8 = 9
  {
    itb = m_observations.begin();
    ite = m_observations.upper_bound(shrdt); 
  }
  else if(r==te::dt::DURING) //4
  {
    te::dt::DateTimePeriod* auxt = static_cast<te::dt::DateTimePeriod*>(shrdt.get());
    te::dt::DateTimeShrPtr t1(auxt->getInitialInstant());
    te::dt::DateTimeShrPtr t2(auxt->getFinalInstant());
    itb = m_observations.find(t1);
    if(itb==m_observations.end())
      itb = m_observations.upper_bound(t1);
    ite = m_observations.upper_bound(t2); 
  }
  else if(r==te::dt::EQUALS) //8
  {
    std::pair<CoverageSeriesObservationSet::const_iterator, CoverageSeriesObservationSet::const_iterator> itPair;
    itPair = m_observations.equal_range(shrdt);
    itb = itPair.first;
    ite = itPair.second;
    if(ite!= m_observations.end())
      ++ite;
  }
  
  while(itb != ite)
  {
    cs->add(*itb);
    ++itb;
  }
  
  return cs;
}

std::auto_ptr<te::st::CoverageSeries> 
te::st::CoverageSeries::getPatch(const te::gm::Envelope& /*e*/, te::gm::SpatialRelation /*sr*/) const
{
  return std::auto_ptr<te::st::CoverageSeries>();
}

std::auto_ptr<te::st::CoverageSeries> 
te::st::CoverageSeries::getPatch(const te::gm::Geometry& /*e*/, te::gm::SpatialRelation /*sr*/) const
{
   return std::auto_ptr<te::st::CoverageSeries>();
}


std::auto_ptr<te::st::CoverageSeries> 
te::st::CoverageSeries::getPatch(const te::gm::Envelope& /*e*/, te::gm::SpatialRelation /*sr*/, 
                                 const te::dt::DateTime& /*dt*/, te::dt::TemporalRelation /*r*/) const
{
   return std::auto_ptr<te::st::CoverageSeries>();
}


std::auto_ptr<te::st::CoverageSeries> 
te::st::CoverageSeries::getPatch(const te::gm::Geometry& /*e*/, te::gm::SpatialRelation /*sr*/, 
                                 const te::dt::DateTime& /*dt*/, te::dt::TemporalRelation /*tr*/) const
{
   return std::auto_ptr<te::st::CoverageSeries>();
}

te::st::CoverageSeries::~CoverageSeries() 
{

}


