#include "GraphDataManager.h"

#include "AbstractGraphLoaderStrategy.h"
#include "Config.h"
#include "Edge.h"
#include "Exception.h"
#include "Vertex.h"

// Terralib Includes
#include "../common/Translator.h"
#include "../common/StringUtils.h"



te::graph::GraphDataManager::GraphDataManager(te::graph::AbstractGraph* g) : m_loadStrategy(0), m_graph(g)
{
}

te::graph::GraphDataManager::~GraphDataManager()
{
  delete m_loadStrategy;
}

te::graph::AbstractGraphLoaderStrategy* te::graph::GraphDataManager::getLoaderStrategy()
{
  return m_loadStrategy;
}

void te::graph::GraphDataManager::setLoaderStrategy(AbstractGraphLoaderStrategy* loaderStrategy)
{
  m_loadStrategy = loaderStrategy;
}

void te::graph::GraphDataManager::loadGraphDataByVertexId(int vertexId, te::graph::GraphCache* gc)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->loadDataByVertexId(vertexId, m_graph, gc);
  }
}

void te::graph::GraphDataManager::loadGraphDataByEdgeId(int edgeId, te::graph::GraphCache* gc)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->loadDataByEdgeId(edgeId, m_graph, gc);
  }
}

void te::graph::GraphDataManager::saveGraphData(GraphData* data)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->saveData(data);
  }
}

void te::graph::GraphDataManager::removeEdge(int id)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->removeEdge(id);
  }
}

void te::graph::GraphDataManager::removeVertex(int id)
{
  if(m_loadStrategy)
  {
    m_loadStrategy->removeVertex(id);
  }
}
