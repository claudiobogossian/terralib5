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
  \file Trajectory.cpp

  \brief This file contains a class to represent a trajectory.
*/

// TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimePeriod.h"
#include "../../../datatype/DateTimeInstant.h"
#include "../../../datatype/DateTimeUtils.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Utils.h"
#include "../../../sam/rtree/Index.h"

//ST
#include "Trajectory.h"
#include "../timeseries/TimeSeries.h"
#include "../interpolator/AbstractTrajectoryInterp.h"
#include "../interpolator/NearestGeometryAtTimeInterp.h"

//STL
#include <algorithm>

te::st::Trajectory::Trajectory()
  : m_observations(),
    m_interpolator(&NearestGeometryAtTimeInterp::getInstance()),
    m_id("-1"),
    m_rtree()
{  
  //create the internal empty RTree 
  m_rtree.reset(new te::sam::rtree::Index<te::dt::DateTime*>());
}

te::st::Trajectory::Trajectory(const std::string& id)
  : m_observations(),
    m_interpolator(&NearestGeometryAtTimeInterp::getInstance()),
    m_id(id),
    m_rtree()
{  
  //create the internal empty RTree 
  m_rtree.reset(new te::sam::rtree::Index<te::dt::DateTime*>());
}

te::st::Trajectory::Trajectory(AbstractTrajectoryInterp* interp, const std::string& id)
  : m_observations(),
    m_interpolator(interp),
    m_id(id),
    m_rtree()
{  
  //create the internal empty RTree 
  m_rtree.reset(new te::sam::rtree::Index<te::dt::DateTime*>());
}

te::st::Trajectory::Trajectory(const TrajectoryObservationSet& obs, const std::string& id)
  : m_observations(obs),
    m_interpolator(&NearestGeometryAtTimeInterp::getInstance()),
    m_id(id),
    m_rtree()
{  
  //create the internal empty RTree 
  m_rtree.reset(new te::sam::rtree::Index<te::dt::DateTime*>());
}

te::st::Trajectory::Trajectory( const TrajectoryObservationSet& obs, AbstractTrajectoryInterp* interp, 
                                const std::string& id)
  : m_observations(obs),
    m_interpolator(interp),
    m_id(id),
    m_rtree()
{
  //create the internal empty RTree 
  m_rtree.reset(new te::sam::rtree::Index<te::dt::DateTime*>());
}

te::st::Trajectory::Trajectory(const TrajectoryPatch& patch, AbstractTrajectoryInterp* interp, 
                    const std::string& id)
  : m_observations(),
    m_interpolator(interp),
    m_id(id),
    m_rtree()
{
  //create the internal empty RTree 
  m_rtree.reset(new te::sam::rtree::Index<te::dt::DateTime*>());
  
  TrajectoryIterator it = patch.begin();
  while(it!=patch.end())
  {
    //add shared pointers and creates the internal RTree
    m_observations.insert(*it);  
    ++it;
  }
}

te::st::Trajectory::Trajectory(const Trajectory& ph)
  : m_observations(ph.m_observations),
    m_interpolator(ph.m_interpolator),
    m_id(ph.m_id),
    m_rtree(ph.m_rtree)
{
}

te::st::Trajectory& te::st::Trajectory::operator=(const Trajectory& other)
{
  if(this != &other)
  {
    m_observations = other.m_observations;
    m_interpolator = other.m_interpolator;
    m_id = other.m_id;
    m_rtree = other.m_rtree; //shared pointer
  }
  return *this;
}

te::st::Trajectory* te::st::Trajectory::clone() const
{
  Trajectory* result = new Trajectory(m_interpolator, m_id);
  TrajectoryObservationSet::const_iterator it = m_observations.begin();
  while(it != m_observations.end())
  {
    te::dt::DateTime* t = static_cast<te::dt::DateTime*>(it->first->clone());
    te::gm::Geometry* g = static_cast<te::gm::Geometry*>(it->second->clone());
    result->add(t,g);
    ++it;
  }
  return result;
}   

const te::st::TrajectoryObservationSet& te::st::Trajectory::getObservations() const
{
  return m_observations;
}

te::st::AbstractTrajectoryInterp* te::st::Trajectory::getInterpolator() const
{
  return m_interpolator;
}

void te::st::Trajectory::setInterpolator(te::st::AbstractTrajectoryInterp* interp)
{
  m_interpolator = interp;
}

std::string te::st::Trajectory::getId() const
{
  return m_id;
}       

void te::st::Trajectory::setId(const std::string& id)
{
  m_id = id;
} 

void te::st::Trajectory::add(te::dt::DateTime* time, te::gm::Geometry* geom)
{
  te::dt::DateTimeShrPtr t(time);
  te::gm::GeometryShrPtr g(geom);

  std::pair<te::dt::DateTimeShrPtr, te::gm::GeometryShrPtr> pr(t,g);
  add(pr);
}

void te::st::Trajectory::add(const TrajectoryObservation& p)
{
  m_observations.insert(p);
  const te::gm::Envelope* env = p.second->getMBR();
  m_rtree->insert(*env, p.first.get());
}

std::size_t te::st::Trajectory::size() const
{
  return m_observations.size();
}

te::st::TrajectoryIterator te::st::Trajectory::begin() const
{
  TrajectoryObservationSet::const_iterator it = m_observations.begin();
  return TrajectoryIterator(it);
}

te::st::TrajectoryIterator te::st::Trajectory::end() const
{
  TrajectoryObservationSet::const_iterator it = m_observations.end();
  return TrajectoryIterator(it);
}

te::st::TrajectoryIterator te::st::Trajectory::at(te::dt::DateTime* t) const
{
  te::dt::DateTimeShrPtr aux(static_cast<te::dt::DateTime*>(t->clone()));
  TrajectoryObservationSet::const_iterator it = m_observations.find(aux);
  return TrajectoryIterator (it);
}

std::auto_ptr<te::gm::Geometry> te::st::Trajectory::getGeometry(te::dt::DateTime* t) const
{
  te::dt::DateTimeShrPtr aux(static_cast<te::dt::DateTime*>(t->clone()));
  TrajectoryObservationSet::const_iterator it = m_observations.find(aux);
  if(it!=m_observations.end())
    return std::auto_ptr<te::gm::Geometry>(static_cast<te::gm::Geometry*>(it->second->clone()));

  return std::auto_ptr<te::gm::Geometry>(m_interpolator->estimate(*this,t)); 
}

std::auto_ptr<te::dt::DateTimePeriod> te::st::Trajectory::getTemporalExtent() const
{
  te::dt::DateTime* bt = m_observations.begin()->first.get();
  te::dt::DateTime* et = m_observations.rbegin()->first.get();
  //This function does not take the ownership of the given times
  return std::auto_ptr<te::dt::DateTimePeriod>(te::dt::GetTemporalExtent(bt, et)); 
}

te::gm::Envelope te::st::Trajectory::getSpatialExtent() const
{
  return m_rtree->getMBR();
}

te::st::TrajectoryPatch 
te::st::Trajectory::getPatch(const te::dt::DateTime& dt, 
                                  te::dt::TemporalRelation r) const
{
  //Note: the end iterator of a patch points to the position AFTER the last required observation 
  TrajectoryObservationSet::const_iterator itb = m_observations.end();
  TrajectoryObservationSet::const_iterator ite = m_observations.end();

  te::dt::DateTimeShrPtr shrdt(static_cast<te::dt::DateTime*>(dt.clone()));

  if(r==te::dt::AFTER) //2
  {
    itb = m_observations.upper_bound(shrdt);
    return TrajectoryPatch(itb,ite);
  }
  if(r==(te::dt::AFTER | te::dt::EQUALS)) // 2 OU 8 = 10 
  {
    itb = m_observations.find(shrdt);
    if(itb==m_observations.end())
      itb = m_observations.upper_bound(shrdt);
    return TrajectoryPatch(itb,ite);
  }
  if(r==te::dt::BEFORE) // 1
  {
    itb = m_observations.begin();
    ite = m_observations.find(shrdt);
    if(ite==m_observations.end())
      ite = m_observations.upper_bound(shrdt);
    return TrajectoryPatch(itb,ite);
  }
  if(r==(te::dt::BEFORE | te::dt::EQUALS)) // 1 OU 8 = 9
  {
    itb = m_observations.begin();
    ite = m_observations.upper_bound(shrdt); 
    return TrajectoryPatch(itb,ite);
  }
  if(r==te::dt::DURING) //4
  {
    te::dt::DateTimeShrPtr t1(static_cast<te::dt::DateTimePeriod*>(shrdt.get())->getInitialInstant());
    te::dt::DateTimeShrPtr t2(static_cast<te::dt::DateTimePeriod*>(shrdt.get())->getFinalInstant());
    itb = m_observations.find(t1);
    if(itb==m_observations.end())
      itb = m_observations.upper_bound(t1);
    ite = m_observations.upper_bound(t2); 
    return TrajectoryPatch(itb,ite);  
  }
  if(r==te::dt::EQUALS) //8
  {
    std::pair<TrajectoryObservationSet::const_iterator, TrajectoryObservationSet::const_iterator> itPair;
    itPair = m_observations.equal_range(shrdt);
    itb = itPair.first;
    ite = itPair.second;
    if(ite!= m_observations.end())
      ++ite;
    return TrajectoryPatch(itb,ite);
  }
  
  return TrajectoryPatch(itb,ite);  
}

void 
te::st::Trajectory::getPatches(const te::gm::Envelope& g, te::gm::SpatialRelation r, 
                                  std::vector<TrajectoryPatch>& result) const
{
  //Note: the end iterator of a patch points to the position AFTER the last required observation 
  if(r!=te::gm::INTERSECTS)
    return;

  //times when the trajectory intersects the envelope
  std::vector<te::dt::DateTime*> report;
  m_rtree->search(g, report);
  std::sort(report.begin(), report.end(), te::dt::CompareDateTime());

  std::vector<te::dt::DateTime*>::const_iterator itResultBegin = report.begin();
  std::vector<te::dt::DateTime*>::const_iterator itResultEnd = report.end();
  
  te::dt::DateTimeShrPtr shrdt(static_cast<te::dt::DateTime*>((*itResultBegin)->clone())); 
  TrajectoryObservationSet::const_iterator itOriginBegin = m_observations.find(shrdt);
  TrajectoryObservationSet::const_iterator itOriginEnd = m_observations.end();

  TrajectoryObservationSet::const_iterator pathBegin;
  TrajectoryObservationSet::const_iterator pathEnd;

  while(itResultBegin!=itResultEnd && itOriginBegin!=itOriginEnd)
  {
     pathBegin = itOriginBegin; //The beginning of a patch

     //Enquanto os ponteiros forem iguais estamos no meio de um patch
     while( (itResultBegin!=itResultEnd) && (itOriginBegin!=itOriginEnd) && 
            (*itResultBegin == itOriginBegin->first.get())) 
     {
        pathEnd = itOriginBegin;
        ++itResultBegin;
        ++itOriginBegin;
     }

     //get the patch
     //the end of a patch MUST point to the position AFTER the required observations 
     if(pathEnd!=itOriginEnd)
      ++pathEnd;
     TrajectoryPatch r(pathBegin, pathEnd);
     result.push_back(r);

     //update the origin begin
     if(itResultBegin!=itResultEnd)
     {
       shrdt.reset(static_cast<te::dt::DateTime*>((*itResultBegin)->clone()));
       itOriginBegin = m_observations.find(shrdt);
     }
  }
  
  return;
}

void 
te::st::Trajectory::getPatches(const te::gm::Geometry& geom, te::gm::SpatialRelation r, 
                                  std::vector<TrajectoryPatch>& result) const
{
  if(r!=te::gm::INTERSECTS)
    return;

  //times when the trajectory intersects the envelope
  std::vector<te::dt::DateTime*> report;
  m_rtree->search(*geom.getMBR(), report);
  
  std::sort(report.begin(), report.end(), te::dt::CompareDateTime());

  std::vector<te::dt::DateTime*>::const_iterator itResultBegin = report.begin();
  std::vector<te::dt::DateTime*>::const_iterator itResultEnd = report.end();

  te::dt::DateTimeShrPtr shrdt(static_cast<te::dt::DateTime*>((*itResultBegin)->clone()));
  TrajectoryObservationSet::const_iterator itOriginBegin = m_observations.find(shrdt);
  TrajectoryObservationSet::const_iterator itOriginEnd = m_observations.end();
  
  TrajectoryObservationSet::const_iterator pathBegin;
  TrajectoryObservationSet::const_iterator pathEnd;
   
  while(itResultBegin!=itResultEnd && itOriginBegin!=itOriginEnd)
  {
     if(!geom.intersects(itOriginBegin->second.get())) //looking for a patch
     {
        ++itResultBegin;
        if(itResultBegin!=itResultEnd)
        {
          shrdt.reset(static_cast<te::dt::DateTime*>((*itResultBegin)->clone()));
          itOriginBegin = m_observations.find(shrdt);
        }        
     }
     else //the beginning of a patch was found
     {
        pathBegin = itOriginBegin;
        //enquanto os datetimes forem sequenciais e as geometrias interceptam geom estamos no meio do patch! 
        while( itResultBegin!=itResultEnd && itOriginBegin!=itOriginEnd
            && (*itResultBegin == itOriginBegin->first.get())
            && geom.intersects(itOriginBegin->second.get()))
        {
            pathEnd = itOriginBegin;
            ++itResultBegin;
            if(itResultBegin!=itResultEnd)
            {
              shrdt.reset(static_cast<te::dt::DateTime*>((*itResultBegin)->clone()));
              itOriginBegin = m_observations.find(shrdt);
            }
        }

        //get the patch
        //the end of a patch MUST point to the position AFTER the required observations
        if(pathEnd!=itOriginEnd)
          ++pathEnd; 
        TrajectoryPatch r(pathBegin, pathEnd);
        result.push_back(r);  

     }//else
  }//while
     
  return;  
}


void te::st::Trajectory::getPatches(const te::gm::Envelope& e, te::gm::SpatialRelation sr, 
                      const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                      std::vector<TrajectoryPatch>& result) const
{
  //first: find the patch that satisfy the temporal relation
  TrajectoryPatch res1 = getPatch(dt, tr);

  //second: find the patches using the spatial relation
  Trajectory tj(res1, m_interpolator, m_id);
  std::vector<TrajectoryPatch> res2;
  tj.getPatches(e, sr, res2); //sr: must be INTERSECTS for while
    
  std::vector<TrajectoryPatch>::const_iterator it = res2.begin();

  TrajectoryObservationSet::const_iterator itObsEnd = m_observations.end();

  while(it!=res2.end())
  {
    TrajectoryIterator itAuxBegin = (*it).begin();
    TrajectoryIterator itAuxEnd = (*it).end();

    TrajectoryIterator itBegin(itObsEnd);
    TrajectoryIterator itEnd(itObsEnd);

    //Get the first position
    if(itAuxBegin!=tj.end())
    {
      te::dt::DateTime* t = itAuxBegin.getTime();
      itBegin = at(t);
    }

    //Get the last position
    if(itAuxEnd==tj.end())
      --itAuxEnd;

    te::dt::DateTime* t = itAuxEnd.getTime();
    itEnd = at(t);
    ++itEnd;

    //Get the patch
    if(itBegin!=itEnd)
    {
      TrajectoryPatch p(itBegin, itEnd);
      result.push_back(p);
    }
    
    ++it;
  }
  
  return;
}

void te::st::Trajectory::getPatches(const te::gm::Geometry& geom, te::gm::SpatialRelation sr, 
                      const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                      std::vector<TrajectoryPatch>& result) const
{
  //first: find the patch that satisfy the temporal relation
  TrajectoryPatch res1 = getPatch(dt, tr);

  //second: find the patches using the spatial relation
  Trajectory tj(res1, m_interpolator, m_id);
  std::vector<TrajectoryPatch> res2;
  tj.getPatches(geom, sr, res2); //sr: must be INTERSECTS for while
    
  std::vector<TrajectoryPatch>::const_iterator it = res2.begin();

  TrajectoryObservationSet::const_iterator itObsEnd = m_observations.end();

  while(it!=res2.end())
  {
    TrajectoryIterator itAuxBegin = (*it).begin();
    TrajectoryIterator itAuxEnd = (*it).end();

    TrajectoryIterator itBegin(itObsEnd);
    TrajectoryIterator itEnd(itObsEnd);

    //Get the first position
    if(itAuxBegin!=tj.end())
    {
      te::dt::DateTime* t = itAuxBegin.getTime();
      itBegin = at(t);
    }

    //Get the last position
    if(itAuxEnd==tj.end())
      --itAuxEnd;

    te::dt::DateTime* t = itAuxEnd.getTime();
    itEnd = at(t);
    ++itEnd;

    //Get the patch
    if(itBegin!=itEnd)
    {
      TrajectoryPatch p(itBegin, itEnd);
      result.push_back(p);
    }
    
    ++it;
  }
  
  return;
}

void te::st::Trajectory::getPatches(const te::gm::Geometry& geom, te::st::SpatioTemporalRelation str, 
                    const std::vector<TrajectoryPatch>& result) const
{
}

std::auto_ptr<te::st::TimeSeries> te::st::Trajectory::getDistance(const Trajectory& other) const
{
  std::auto_ptr<te::st::TimeSeries> result(new te::st::TimeSeries());
  TrajectoryObservationSet::const_iterator it = m_observations.begin();
  //te::st::TimeSeries* result = 
  while(it!=m_observations.end())
  {
    te::dt::DateTime* t = it->first.get();
    std::auto_ptr<te::gm::Geometry> otherGeom(other.getGeometry(t));

    double dist = it->second->distance(otherGeom.get());
    
    //insert into the new time series!
    result->add(static_cast<te::dt::DateTime*>(t->clone()),dist);
    ++it;
  }
  return result;
}

std::auto_ptr<te::st::TimeSeries> te::st::Trajectory::getDistance(const te::gm::Geometry& geom)
{
  std::auto_ptr<te::st::TimeSeries> res;

  return res;
}

te::st::Trajectory::~Trajectory()
{
}


