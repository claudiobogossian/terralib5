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
  \file DirectedGraph.cpp

  \brief This is a implementation of a Directed Graph.
         By convention a directed graph provides access to out-edges only.
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
#include "DirectedGraph.h"


te::graph::DirectedGraph::DirectedGraph() : Graph()
{
}

te::graph::DirectedGraph::DirectedGraph(GraphMetadata* metadata) :
  Graph(metadata)
{
}

te::graph::DirectedGraph::DirectedGraph(AbstractCachePolicy* cp, AbstractGraphLoaderStrategy* ls) :
  Graph(cp, ls)
{
}

te::graph::DirectedGraph::~DirectedGraph()
{
}

std::vector<te::graph::Vertex*> te::graph::DirectedGraph::getVertexNeighborhood(int id)
{
  std::vector<te::graph::Vertex*> vec;

  te::graph::Vertex* v = getVertex(id);

  if(v)
  {
    std::set<int>::iterator it =  v->getSuccessors().begin();

    while(it != v->getSuccessors().end())
    {
      te::graph::Edge* e = getEdge(*it);

      if(e)
      {
        te::graph::Vertex* vNeighbor = getVertex(e->getIdTo());

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


bool te::graph::DirectedGraph::isSinkVertex(int id, bool& flag)
{
  te::graph::Vertex* v = getVertex(id);

  if(v)
  {
    if(v->getSuccessors().empty())
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

void te::graph::DirectedGraph::add(Edge* e)
{
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

      te::graph::Graph::add(e); 
    }
  }
}

void te::graph::DirectedGraph::removeEdge(int id)
{
  te::graph::Edge* e = getEdge(id);

  if(e == 0)
  {
    return;
  }

  te::graph::Vertex* vFrom = getVertex(e->getIdFrom());

  if(vFrom)
  {
    //remove id from successor information
    std::set<int>::iterator it = vFrom->getSuccessors().begin();

    while(it != vFrom->getSuccessors().end())
    {
      if(*it == id)
      {
        vFrom->getSuccessors().erase(it);
        break;
      }

      ++it;
    }
  }

  te::graph::Graph::removeEdge(id);
}

std::vector<te::graph::Edge*> te::graph::DirectedGraph::getOutEdges(int vId)
{
  std::vector<te::graph::Edge*> vec;

  te::graph::Vertex* v = getVertex(vId);

  if(v)
  {
    std::set<int>::iterator it =  v->getSuccessors().begin();

    while(it != v->getSuccessors().end())
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
