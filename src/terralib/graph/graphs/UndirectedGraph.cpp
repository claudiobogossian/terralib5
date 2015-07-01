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
  \file UndirectedGraph.cpp

  \brief This is a implementation of a UndirectedGraph Graph.
         By definition a undirected graph has no direction 
         information about his edges.
*/

// Terralib Includes
#include "../../common/STLUtils.h"
#include "../core/Edge.h"
#include "../core/GraphCache.h"
#include "../core/GraphData.h"
#include "../core/GraphDataManager.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../graphs/Graph.h"
#include "UndirectedGraph.h"


te::graph::UndirectedGraph::UndirectedGraph() : Graph()
{
}

te::graph::UndirectedGraph::UndirectedGraph(GraphMetadata* metadata) :
  Graph(metadata)
{
}

te::graph::UndirectedGraph::UndirectedGraph(AbstractCachePolicy* cp, AbstractGraphLoaderStrategy* ls) :
  Graph(cp, ls)
{
}

te::graph::UndirectedGraph::~UndirectedGraph()
{
}

std::vector<te::graph::Vertex*> te::graph::UndirectedGraph::getVertexNeighborhood(int id)
{
  std::vector<te::graph::Vertex*> vec;

  te::graph::Vertex* v = getVertex(id);

  if(v)
  {
    std::set<int>::iterator it =  v->getNeighborhood().begin();

    while(it != v->getNeighborhood().end())
    {
      te::graph::Edge* e = getEdge(*it);

      if(e)
      {
        te::graph::Vertex* vNeighbor = 0;

        if(e->getIdFrom() == id)
        {
          vNeighbor = getVertex(e->getIdTo());
        }
        else
        {
          vNeighbor = getVertex(e->getIdFrom());
        }

        if(vNeighbor)
        {
          vec.push_back(vNeighbor);
        }
      }
      ++it;
    }
  }

  return vec;
}

bool te::graph::UndirectedGraph::isIsolateVertex(int id, bool& flag)
{
  te::graph::Vertex* v = getVertex(id);

  if(v)
  {
    if(v->getNeighborhood().empty())
    {
      flag = true;
    }
    else
    {
      flag = false;
    }
    return true;
  }

  return false;
}

void te::graph::UndirectedGraph::add(Edge* e)
{
  bool hasVFrom = false;

  if(!m_metadata->m_memoryGraph)
  {
    m_graphData =  m_graphCache->checkCacheByVertexId(e->getIdFrom());
  }

  if(m_graphData)
  {
    //set successor information
    te::graph::Vertex* vFrom = getVertex(e->getIdFrom());
    if(vFrom)
    {
      vFrom->getNeighborhood().insert(e->getId());
      hasVFrom = true;
    }
  }

  bool hasVTo = false;

  if(!m_metadata->m_memoryGraph)
  {
    m_graphData = m_graphCache->checkCacheByVertexId(e->getIdTo());
  }

  if(m_graphData)
  {
    //set predecessor information
    te::graph::Vertex* vTo = getVertex(e->getIdTo());
    if(vTo)
    {
      vTo->getNeighborhood().insert(e->getId());
      hasVTo = true;
    }
  }

  if(hasVFrom && hasVTo)
  {
    te::graph::Graph::add(e);
  }
}

void te::graph::UndirectedGraph::removeEdge(int id)
{
  te::graph::Edge* e = getEdge(id);

  if(e == 0)
  {
    return;
  }

  //remove id from neighborhood information from vFrom
  te::graph::Vertex* vFrom = getVertex(e->getIdFrom());

  if(vFrom)
  {
    std::set<int>::iterator it = vFrom->getNeighborhood().begin();

    while(it != vFrom->getNeighborhood().end())
    {
      if(*it == id)
      {
        vFrom->getNeighborhood().erase(it);
        break;
      }

      ++it;
    }
  }

  //remove id from neighborhood information from vTo
  te::graph::Vertex* vTo = getVertex(e->getIdTo());

  if(vTo)
  {
    std::set<int>::iterator it = vTo->getNeighborhood().begin();

    while(it != vTo->getNeighborhood().end())
    {
      if(*it == id)
      {
        vTo->getNeighborhood().erase(it);
        break;
      }

      ++it;
    }
  }

  te::graph::Graph::removeEdge(id);
}

std::vector<te::graph::Edge*> te::graph::UndirectedGraph::getEdges(int vId)
{
  std::vector<te::graph::Edge*> vec;

  te::graph::Vertex* v = getVertex(vId);

  if(v)
  {
    std::set<int>::iterator it = v->getNeighborhood().begin();

    while(it != v->getNeighborhood().end())
    {
      te::graph::Edge* e = getEdge(*it);

      if(e)
      {
        vec.push_back(e);
      }

      ++it;
    }
  }

  return vec;
}
