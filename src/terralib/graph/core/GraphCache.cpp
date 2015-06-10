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
  \file GraphCache.cpp

  \brief Class used to manager the graph data elements.
        This class uses a cache policy to control the elements in memory.
        If a element was requested and not found  in cache, the 
        GraphDataManager is used to loaded a new GraphData.
*/

// Terralib Includes
#include "../../common/STLUtils.h"
#include "../cache/AbstractCachePolicy.h"
#include "../cache/AbstractCachePolicyFactory.h"
#include "../loader/AbstractGraphLoaderStrategy.h"
#include "GraphCache.h"
#include "GraphData.h"
#include "GraphDataManager.h"

// STL Includes
#include <iostream>

te::graph::GraphCache::GraphCache(AbstractCachePolicy* cp, GraphDataManager* dm): m_policy(cp),m_dataManager(dm)
{
  m_graphDataCounter = 0; // initializate the graph data counter

  m_metadata = m_dataManager->getLoaderStrategy()->getMetadata();

  if(m_policy == 0)
  {
    m_policy = te::graph::AbstractCachePolicyFactory::make(TE_DEFAULT_CACHE_POLICY_TYPE);
  }
}

te::graph::GraphCache::~GraphCache()
{
  clearCache();

  delete m_policy;
}

te::graph::GraphData* te::graph::GraphCache::getGraphDataByVertexId(int id)
{
  //check local cache
  std::map<int, GraphData*>::iterator itMap = m_graphDataMap.begin();

  while(itMap != m_graphDataMap.end())
  {
    te::graph::GraphData* d = itMap->second;

    te::graph::GraphData::VertexMap::iterator it = d->getVertexMap().find(id);

    if(it != d->getVertexMap().end())
    {
      m_policy->accessed(d->getId());

      return d;
    }

    ++itMap;
  }

  //if not found
  if(m_dataManager != 0)
  {
    m_dataManager->loadGraphDataByVertexId(id, this);

    return checkCacheByVertexId(id);
  }

  return 0;
}

te::graph::GraphData* te::graph::GraphCache::getGraphDataByEdgeId(int id)
{
  //check local cache
  std::map<int, GraphData*>::iterator itMap = m_graphDataMap.begin();

  while(itMap != m_graphDataMap.end())
  {
    te::graph::GraphData* d = itMap->second;

    te::graph::GraphData::EdgeMap::iterator it = d->getEdgeMap().find(id);

    if(it != d->getEdgeMap().end())
    {
      m_policy->accessed(d->getId());

      return d;
    }

    ++itMap;
  }

  //if not found
  if(m_dataManager != 0)
  {
    m_dataManager->loadGraphDataByEdgeId(id, this);

    return checkCacheByEdgeId(id);
  }

  return 0;
}

te::graph::GraphData* te::graph::GraphCache::getGraphData()
{
  //cache is empty, return a new graph data
  if(m_graphDataMap.empty())
  {
    return createGraphData();
  }

  //verify if has a incomplete graph data in cache and return
  std::map<int, GraphData*>::iterator itMap = m_graphDataMap.begin();

  int gdId = -1;
  size_t maxSize = m_metadata->m_maxCacheSize - 1;

  while(itMap != m_graphDataMap.end())
  {
    te::graph::GraphData* d = itMap->second;

    if(d->getVertexMap().size() < maxSize && d->getEdgeMap().size() < maxSize)
    {
      gdId = d->getId();

      maxSize = d->getVertexMap().size();
    }
    ++itMap;
  }

  if(gdId != -1)
  {
    m_policy->update(gdId);

    return m_graphDataMap[gdId];
  }

  //if cache is not full create a new graph data... add to cache and return
  if(m_graphDataMap.size() < m_metadata->m_maxVecCacheSize)
  {
    return createGraphData();
  }

  //remove the a graph data following the cache policy and create a new graph data
  if(m_graphDataMap.size() == m_metadata->m_maxVecCacheSize)
  {
    int idxToRemove = 0;

    m_policy->toRemove(idxToRemove);

    te::graph::GraphData* d = m_graphDataMap[idxToRemove];

    saveGraphData(d);

    removeGraphData(d->getId());

    return createGraphData();
  }

  return 0;
}

te::graph::GraphData* te::graph::GraphCache::createGraphData()
{
  if(m_graphDataMap.size() == m_metadata->m_maxVecCacheSize)
  {
    return 0;
  }

  te::graph::GraphData* d = new te::graph::GraphData(getGraphDataId());

  m_graphDataMap.insert(std::map<int, GraphData*>::value_type(d->getId(), d));

  m_policy->added(d->getId());

  return d;
}

void te::graph::GraphCache::removeGraphData(int idx)
{
  std::map<int, GraphData*>::iterator it = m_graphDataMap.find(idx);

  if(it == m_graphDataMap.end())
  {
    return;
  }

  te::graph::GraphData* data =it->second;
  delete data;

  m_graphDataMap.erase(it);
}

void te::graph::GraphCache::saveGraphData(GraphData* data)
{
  if(m_dataManager)
  {
    m_dataManager->saveGraphData(data);
  }
}

void te::graph::GraphCache::clearCache()
{
  std::map<int, GraphData*>::iterator it = m_graphDataMap.begin();

  while(it != m_graphDataMap.end())
  {
    te::graph::GraphData* d = it->second;

    if(d->isDirty())
    {
      if(m_dataManager)
      {
        m_dataManager->saveGraphData(d);
      }
    }

    ++it;
  }

  te::common::FreeContents(m_graphDataMap);

  m_graphDataMap.clear();
}

te::graph::GraphData* te::graph::GraphCache::checkCacheByVertexId(int id)
{
  //if not found check in cache
  std::map<int, GraphData*>::iterator itMap = m_graphDataMap.begin();

  while(itMap != m_graphDataMap.end())
  {
    te::graph::GraphData* d = itMap->second;

    te::graph::GraphData::VertexMap::iterator it = d->getVertexMap().find(id);

    if(it != d->getVertexMap().end())
    {
      m_policy->accessed(d->getId());

      return d;
    }

    ++itMap;
  }

  return 0;
}

te::graph::GraphData* te::graph::GraphCache::checkCacheByEdgeId(int id)
{
  //if not found check in cache
  std::map<int, GraphData*>::iterator itMap = m_graphDataMap.begin();

  while(itMap != m_graphDataMap.end())
  {
    te::graph::GraphData* d = itMap->second;

    te::graph::GraphData::EdgeMap::iterator it = d->getEdgeMap().find(id);

    if(it != d->getEdgeMap().end())
    {
      m_policy->accessed(d->getId());

      return d;
    }

    ++itMap;
  }

  return 0;
}

int te::graph::GraphCache::getGraphDataId()
{
  int id = m_graphDataCounter;

  ++m_graphDataCounter;

  return id;
}
