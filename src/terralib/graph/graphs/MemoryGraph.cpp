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
  \file MemoryGraph.cpp

  \brief  This is the memory graph implementation and do not uses a
          cache policy and a graph loader.
*/

// Terralib Includes
#include "../../common/STLUtils.h"
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../core/Edge.h"
#include "../core/EdgeProperty.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../Config.h"
#include "../Exception.h"
#include "MemoryGraph.h"

// STL Includes
#include <cassert>

te::graph::MemoryGraph::MemoryGraph() : AbstractGraph(), 
  m_metadata(0)
{
}

te::graph::MemoryGraph::MemoryGraph(GraphMetadata* metadata) : AbstractGraph(), 
  m_metadata(metadata)
{
}

te::graph::MemoryGraph::~MemoryGraph()
{
  flush();

  delete m_metadata;
}

void te::graph::MemoryGraph::add(Vertex*  v)
{
  m_vertexMap.insert(VertexMap::value_type(v->getId(), v));
}

void te::graph::MemoryGraph::update(Vertex*  v)
{
  v->setDirty(true);
}

void te::graph::MemoryGraph::removeVertex(int id)
{
  VertexMap::iterator it = m_vertexMap.find(id);

  if(it != m_vertexMap.end())
    m_vertexMap.erase(it);
}

te::graph::Vertex* te::graph::MemoryGraph::getVertex(int id)
{
  VertexMap::iterator it = m_vertexMap.find(id);

  if(it != m_vertexMap.end())
    return it->second;

  return 0;
}

void te::graph::MemoryGraph::addVertexProperty(te::dt::Property* p)
{
  if(m_metadata)
    m_metadata->addVertexProperty(p);
}

void te::graph::MemoryGraph::removeVertexProperty(int idx)
{
  if(m_metadata)
    m_metadata->removeVertexProperty(idx);
}

te::dt::Property* te::graph::MemoryGraph::getVertexProperty(int idx)
{
  if(m_metadata)
    return m_metadata->getVertexProperty(idx);

  return 0;
}

int te::graph::MemoryGraph::getVertexPropertySize()
{
  if(m_metadata)
    return m_metadata->getVertexPropertySize();

  return 0;
}

void te::graph::MemoryGraph::add(Edge* e)
{
  m_edgeMap.insert(EdgeMap::value_type(e->getId(), e));
}

void te::graph::MemoryGraph::update(Edge* e)
{
  e->setDirty(true);
}

void te::graph::MemoryGraph::removeEdge(int id)
{
  EdgeMap::iterator it = m_edgeMap.find(id);

  if(it != m_edgeMap.end())
    m_edgeMap.erase(it);
}

te::graph::Edge* te::graph::MemoryGraph::getEdge(int id)
{
  EdgeMap::iterator it = m_edgeMap.find(id);

  if(it != m_edgeMap.end())
    return it->second;


  return 0;
}

void te::graph::MemoryGraph::addEdgeProperty(te::dt::Property* p)
{
  if(m_metadata)
    m_metadata->addEdgeProperty(p);
}

void te::graph::MemoryGraph::removeEdgeProperty(int idx)
{
  if(m_metadata)
    m_metadata->removeEdgeProperty(idx);
}

te::dt::Property* te::graph::MemoryGraph::getEdgeProperty(int idx)
{
  if(m_metadata)
    return m_metadata->getEdgeProperty(idx);

  return 0;
}

int te::graph::MemoryGraph::getEdgePropertySize()
{
  if(m_metadata)
    return m_metadata->getEdgePropertySize();

  return 0;
}

te::graph::GraphMetadata* te::graph::MemoryGraph::getMetadata()
{
  return m_metadata;
}

void te::graph::MemoryGraph::flush()
{
  te::common::FreeContents(m_vertexMap);
  te::common::FreeContents(m_edgeMap);

  m_vertexMap.clear();
  m_edgeMap.clear();
}

te::graph::MemoryGraph::VertexMap& te::graph::MemoryGraph::getVertexMap()
{
  return m_vertexMap;
}

te::graph::MemoryGraph::EdgeMap& te::graph::MemoryGraph::getEdgeMap()
{
  return m_edgeMap;
}
