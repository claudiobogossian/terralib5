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
  \file SpatioTemporalObservationSet.cpp

  \brief This file contains an observation set that has one geometry observed property. 
*/

// TerraLib
#include "../../geometry/Envelope.h"
#include "../../geometry/Geometry.h"


//ST
#include "SpatioTemporalObservationSet.h"
#include "Observation.h"


te::st::SpatioTemporalObservationSet::SpatioTemporalObservationSet() 
  : ObservationSet(),
    m_geomPropIdx(0)
{  
  m_RTree = new te::sam::rtree::Index<Observation*>();
}

te::st::SpatioTemporalObservationSet::SpatioTemporalObservationSet(const std::vector<Observation*>& obs, 
                                      const std::vector<te::dt::Property*>& props, int geomPropIdx)
  : ObservationSet(props),
    m_geomPropIdx(geomPropIdx)
{ 
  m_RTree = new te::sam::rtree::Index<Observation*>();
  
  //add the observations 
  for(unsigned int i=0; i<obs.size(); ++i)
    addObservation(obs[i]); 
}

te::st::SpatioTemporalObservationSet::SpatioTemporalObservationSet(const std::vector<te::dt::Property*>& props, 
                                                                      int geomPropIdx)
   : ObservationSet(props),
    m_geomPropIdx(geomPropIdx)
{
  m_RTree = new te::sam::rtree::Index<Observation*>();
}

int te::st::SpatioTemporalObservationSet::getGeometryPropIdx() const
{
  return m_geomPropIdx;
}

void te::st::SpatioTemporalObservationSet::setGeometryPropIdx(int idx)
{
  m_geomPropIdx = idx;
}    

void te::st::SpatioTemporalObservationSet::clear()
{
  m_RTree->clear();
  //// VERIFICAR SE ESTA CHAMANDO O PAI CORRETAMENTE!!!!
  dynamic_cast<ObservationSet*>(this)->clear();
}

void te::st::SpatioTemporalObservationSet::addObservation(te::st::Observation* ob)
{
  m_observations.insert(std::pair<te::dt::DateTime*, te::st::Observation*>(ob->getTime(), ob)); 
  ob->setParent(this);
  const te::gm::Envelope* env = dynamic_cast<te::gm::Geometry*>(ob->getObservedValue(m_geomPropIdx))->getMBR();
  m_RTree->insert(*env, ob);
}

te::gm::Envelope* te::st::SpatioTemporalObservationSet::spatialExtent()
{
  te::gm::Envelope* output = new te::gm::Envelope(m_RTree->getMBR());
  return output;
}

void te::st::SpatioTemporalObservationSet::intersection(te::gm::Geometry* geom, std::vector<Observation*>& result)
{
  std::vector<Observation*> report;
  m_RTree->search(*geom->getMBR(), report);
  for(unsigned int i=0; i<report.size(); ++i)
  {
    te::gm::Geometry* g = dynamic_cast<te::gm::Geometry*>(report[i]->getObservedValue(m_geomPropIdx));
    if(g->intersects(geom))
      result.push_back(report[i]);
  }
}

void te::st::SpatioTemporalObservationSet::difference(te::gm::Geometry* geom, std::vector<Observation*>& result)
{
  std::vector<Observation*> report;
  intersection(geom, report);
  
  TimeToObservationMap::const_iterator itOriginal = m_observations.begin();
  std::vector<Observation*>::iterator itInter; 
  bool find;
  while(itOriginal != m_observations.end())
  {
    find = false;
    itInter = report.begin();
    while(itInter != report.end())
    {
      if(itOriginal->second==*itInter)
      { 
        find = true;
        report.erase(itInter);
        break;
      }
      ++itInter;
    }
    if(!find)
      result.push_back(itOriginal->second);

    ++itOriginal;
  }
}

te::st::SpatioTemporalObservationSet::~SpatioTemporalObservationSet()
{
  delete m_RTree;
}


