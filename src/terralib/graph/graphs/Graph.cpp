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
  \file Graph.cpp

  \brief  This is the main graph implementation, that uses a
          cache policy anda graph loader to get all elements
          inside a data source.

          All methods to access a graph element (vertex or edge) 
          will use the GraphData instance, if not found the element, 
          the class GraphCache will be consulted.
*/

// Terralib Includes
#include "../../common/STLUtils.h"
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../core/Edge.h"
#include "../core/EdgeProperty.h"
#include "../core/GraphData.h"
#include "../core/GraphDataManager.h"
#include "../core/GraphCache.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../loader/AbstractGraphLoaderStrategy.h"
#include "../Config.h"
#include "../Exception.h"
#include "Graph.h"

// STL Includes
#include <cassert>

te::graph::Graph::Graph() : AbstractGraph(),
  m_dataManager(0),
  m_graphCache(0), 
  m_metadata(0),
  m_graphData(0)
{
}

te::graph::Graph::Graph(GraphMetadata* metadata): 
  AbstractGraph(), 
  m_dataManager(0),
  m_graphCache(0), 
  m_metadata(metadata)
{
  assert(metadata);

  if(m_metadata->m_memoryGraph)
    m_graphData = new te::graph::GraphData(0);
}

te::graph::Graph::Graph(AbstractCachePolicy* cp, AbstractGraphLoaderStrategy* ls): 
  AbstractGraph(),
  m_dataManager(0),
  m_graphCache(0), 
  m_metadata(0),
  m_graphData(0)
{
  assert(ls);

  m_dataManager = new te::graph::GraphDataManager(this);

  m_dataManager->setLoaderStrategy(ls);

  m_graphCache = new te::graph::GraphCache(cp, m_dataManager);

  m_metadata = ls->getMetadata();
}

te::graph::Graph::~Graph()
{
  flush();

  delete m_graphCache;
  delete m_dataManager;
}

void te::graph::Graph::add(Vertex*  v)
{
  if(!m_graphData)
  {
    m_graphData = m_graphCache->getGraphData();
  }

  m_graphData->addVertex(v);

  if(m_graphData->getVertexMap().size() >= m_metadata->m_maxCacheSize && !m_metadata->m_memoryGraph)
  {
    m_graphData = 0;
  }
}

void te::graph::Graph::update(Vertex*  v)
{
  v->setDirty(true);

  m_graphData->setDirty(true);
}

void te::graph::Graph::removeVertex(int id)
{
  bool res = m_graphData->removeVertex(id);

  if(!res && !m_metadata->m_memoryGraph)
  {
    m_graphData = m_graphCache->getGraphDataByVertexId(id);

    if(m_graphData)
      m_graphData->removeVertex(id);
  }

  if(m_dataManager)
  {
    m_dataManager->removeVertex(id);
  }
}

te::graph::Vertex* te::graph::Graph::getVertex(int id)
{
  te::graph::Vertex* v = 0;

  if(m_graphData)
  {
    v = m_graphData->getVertex(id);
  }

  if(!v && !m_metadata->m_memoryGraph)
  {
    m_graphData = m_graphCache->getGraphDataByVertexId(id);

    if(m_graphData)
      v = m_graphData->getVertex(id);
  }

  return v;
}

void te::graph::Graph::addVertexProperty(te::dt::Property* p)
{
  if(m_metadata)
  {
    m_metadata->addVertexProperty(p);
  }
}

void te::graph::Graph::removeVertexProperty(int idx)
{
  if(m_metadata)
  {
    m_metadata->removeVertexProperty(idx);
  }
}

te::dt::Property* te::graph::Graph::getVertexProperty(int idx)
{
  if(m_metadata)
  {
    return m_metadata->getVertexProperty(idx);
  }

  return 0;
}

int te::graph::Graph::getVertexPropertySize()
{
  if(m_metadata)
  {
    return m_metadata->getVertexPropertySize();
  }

  return 0;
}

void te::graph::Graph::add(Edge* e)
{
  if(!m_graphData)
  {
    m_graphData = m_graphCache->getGraphData();
  }

  m_graphData->addEdge(e);

  if(m_graphData->getEdgeMap().size() >= m_metadata->m_maxCacheSize && !m_metadata->m_memoryGraph)
  {
    m_graphData = 0;
  }
}

void te::graph::Graph::update(Edge* e)
{
  e->setDirty(true);

  m_graphData->setDirty(true);
}

void te::graph::Graph::removeEdge(int id)
{
  bool res = m_graphData->removeEdge(id);
  
  if(!res && !m_metadata->m_memoryGraph)
  {
    m_graphData = m_graphCache->getGraphDataByEdgeId(id);

    if(m_graphData)
      m_graphData->removeEdge(id);
  }

  if(m_dataManager)
  {
    m_dataManager->removeEdge(id);
  }
}

te::graph::Edge* te::graph::Graph::getEdge(int id)
{
  te::graph::Edge* e = 0;

  if(m_graphData)
  {
    e = m_graphData->getEdge(id);
  }

  if(!e && !m_metadata->m_memoryGraph)
  {
    m_graphData = m_graphCache->getGraphDataByEdgeId(id);

    if(m_graphData)
      e = m_graphData->getEdge(id);
  }

  return e;
}

void te::graph::Graph::addEdgeProperty(te::dt::Property* p)
{
  if(m_metadata)
  {
    m_metadata->addEdgeProperty(p);
  }
}

void te::graph::Graph::removeEdgeProperty(int idx)
{
  if(m_metadata)
  {
    m_metadata->removeEdgeProperty(idx);
  }
}

te::dt::Property* te::graph::Graph::getEdgeProperty(int idx)
{
  if(m_metadata)
  {
    return m_metadata->getEdgeProperty(idx);
  }

  return 0;
}

int te::graph::Graph::getEdgePropertySize()
{
  if(m_metadata)
  {
    return m_metadata->getEdgePropertySize();
  }

  return 0;
}

te::graph::GraphMetadata* te::graph::Graph::getMetadata()
{
  return m_metadata;
}

void te::graph::Graph::flush()
{
  if(m_metadata->m_memoryGraph)
  {
    delete m_graphData;
    m_graphData = 0;
  }
  else
  {
    m_graphData = 0;
    m_graphCache->clearCache();
  }
}
