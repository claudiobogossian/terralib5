#include "GraphData.h"

#include "Vertex.h"
#include "Edge.h"

// Terralib Includes
#include "../common/STLUtils.h"

te::graph::GraphData::GraphData(int id): 
  m_id(id),
  m_dirty(false)
{
}

te::graph::GraphData::~GraphData()
{
  te::common::FreeContents(m_vertexMap);
  te::common::FreeContents(m_edgeMap);

  m_vertexMap.clear();
  m_edgeMap.clear();
}

int te::graph::GraphData::getId()
{
  return m_id;
}

te::graph::GraphData::VertexMap& te::graph::GraphData::getVertexMap()
{
  return m_vertexMap;
}

void te::graph::GraphData::setVertexMap(const VertexMap& map)
{
  m_vertexMap = map;
}

te::graph::GraphData::EdgeMap& te::graph::GraphData::getEdgeMap()
{
  return m_edgeMap;
}

void te::graph::GraphData::setEdgeMap(const EdgeMap& map)
{
  m_edgeMap = map;
}

void te::graph::GraphData::setDirty(bool status)
{
  m_dirty = status;
}

bool te::graph::GraphData::isDirty()
{
  return m_dirty;
}
