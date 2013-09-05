#include "Graph.h"

#include "AbstractGraphLoaderStrategy.h"
#include "Config.h"
#include "Edge.h"
#include "EdgeProperty.h"
#include "Exception.h"
#include "GraphData.h"
#include "GraphDataManager.h"
#include "GraphCache.h"
#include "GraphMetadata.h"
#include "Vertex.h"
#include "VertexProperty.h"

// Terralib Includes
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"

#include <cassert>

te::graph::Graph::Graph() : AbstractGraph(), 
  m_graphData(0),
  m_dataManager(0),
  m_graphCache(0), 
  m_metadata(0)
{
}

te::graph::Graph::Graph(AbstractCachePolicy* cp, AbstractGraphLoaderStrategy* ls): 
  AbstractGraph(), 
  m_graphData(0),
  m_dataManager(0),
  m_graphCache(0), 
  m_metadata(0)
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

  m_graphData->getVertexMap().insert(te::graph::GraphData::VertexMap::value_type(v->getId(), v));

  if(v->isDirty() || v->isNew())
  {
    m_graphData->setDirty(true);
  }

  if(m_graphData->getVertexMap().size() >= m_metadata->m_maxCacheSize)
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
  te::graph::GraphData::VertexMap::iterator it = m_graphData->getVertexMap().find(id);

  if(it != m_graphData->getVertexMap().end())
  {
    m_graphData->getVertexMap().erase(it);
  }
  else
  {
    m_graphData = m_graphCache->getGraphDataByVertexId(id);

    if(m_graphData)
    {
      it = m_graphData->getVertexMap().find(id);

      if(it != m_graphData->getVertexMap().end())
      {
        m_graphData->getVertexMap().erase(it);
      }
    }
  }

  if(m_dataManager)
  {
    m_dataManager->removeVertex(id);
  }
}

te::graph::Vertex* te::graph::Graph::getVertex(int id)
{
  if(m_graphData)
  {
    te::graph::GraphData::VertexMap::iterator it = m_graphData->getVertexMap().find(id);

    if(it != m_graphData->getVertexMap().end())
    {
      return it->second;
    }
  }

  m_graphData = m_graphCache->getGraphDataByVertexId(id);

  if(m_graphData)
  {
    te::graph::GraphData::VertexMap::iterator it = m_graphData->getVertexMap().find(id);

    if(it != m_graphData->getVertexMap().end())
    {
      return it->second;
    }
  }

  return 0;
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

  m_graphData->getEdgeMap().insert(te::graph::GraphData::EdgeMap::value_type(e->getId(), e));

  if(e->isDirty() || e->isNew())
  {
    m_graphData->setDirty(true);
  }

  if(m_graphData->getEdgeMap().size() >= m_metadata->m_maxCacheSize)
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
  te::graph::GraphData::EdgeMap::iterator it = m_graphData->getEdgeMap().find(id);

  if(it != m_graphData->getEdgeMap().end())
  {
    m_graphData->getEdgeMap().erase(it);
  }
  else
  {
    m_graphData = m_graphCache->getGraphDataByEdgeId(id);

    if(m_graphData)
    {
      it = m_graphData->getEdgeMap().find(id);

      if(it != m_graphData->getEdgeMap().end())
      {
        m_graphData->getEdgeMap().erase(it);
      }
    }
  }

  if(m_dataManager)
  {
    m_dataManager->removeEdge(id);
  }
}

te::graph::Edge* te::graph::Graph::getEdge(int id)
{
  if(m_graphData)
  {
    te::graph::GraphData::EdgeMap::iterator it = m_graphData->getEdgeMap().find(id);

    if(it != m_graphData->getEdgeMap().end())
    {
      return it->second;
    }
  }

  m_graphData = m_graphCache->getGraphDataByEdgeId(id);

  if(m_graphData)
  {
    te::graph::GraphData::EdgeMap::iterator it = m_graphData->getEdgeMap().find(id);

    if(it != m_graphData->getEdgeMap().end())
    {
      return it->second;
    }
  }

  return 0;
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
  m_graphData = 0;

  m_graphCache->clearCache();
}
