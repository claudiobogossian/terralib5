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
  \file BidirectionalGraph.cpp

  \brief This is a implementation of a Bidirectional Graph.
         By convention a bidirectional graph provides access to 
         out-and in edges.
*/

// Terralib Includes
#include "../../common/STLUtils.h"
#include "../core/GraphCache.h"
#include "../core/GraphData.h"
#include "../core/GraphDataManager.h"
#include "../core/GraphMetadata.h"
#include "../core/Edge.h"
#include "../core/Vertex.h"
#include "BidirectionalGraph.h"
#include "Graph.h"

//STL
#include <iterator>


te::graph::BidirectionalGraph::BidirectionalGraph() :Graph()
{
}

te::graph::BidirectionalGraph::BidirectionalGraph(GraphMetadata* metadata) :
  Graph(metadata)
{
}

te::graph::BidirectionalGraph::BidirectionalGraph(AbstractCachePolicy* cp, AbstractGraphLoaderStrategy* ls) :
  Graph(cp, ls)
{
}

te::graph::BidirectionalGraph::~BidirectionalGraph()
{
}

std::vector<te::graph::Vertex*> te::graph::BidirectionalGraph::getVertexNeighborhood(int id)
{
  std::vector<te::graph::Vertex*> vec;

  te::graph::Vertex* v = getVertex(id);

  if(v)
  {
    std::vector<int> edges;

    std::copy(v->getPredecessors().begin(), v->getPredecessors().end(), std::back_inserter(edges));
    std::copy(v->getSuccessors().begin(), v->getSuccessors().end(), std::back_inserter(edges));

    for(size_t i = 0; i < edges.size(); ++i)
    {
      te::graph::Edge* e = getEdge(edges[i]);

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
    }
  }

  return vec;
}

bool te::graph::BidirectionalGraph::isIsolateVertex(int id, bool& flag)
{
  te::graph::Vertex* v = getVertex(id);

  if(v)
  {
    if(v->getPredecessors().empty() && v->getSuccessors().empty())
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

bool te::graph::BidirectionalGraph::isSourceVertex(int id, bool& flag)
{
  te::graph::Vertex* v = getVertex(id);

  if(v)
  {
    if(v->getPredecessors().empty())
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

bool te::graph::BidirectionalGraph::isSinkVertex(int id, bool& flag)
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

void te::graph::BidirectionalGraph::add(Edge* e)
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
      vFrom->getSuccessors().insert(e->getId());
  }

  if(!m_metadata->m_memoryGraph)
  {
    m_graphData =  m_graphCache->checkCacheByVertexId(e->getIdTo());
  }

  if(m_graphData)
  {
    //set successor information
    te::graph::Vertex* vTo = getVertex(e->getIdTo());

    if(vTo)
      vTo->getPredecessors().insert(e->getId());
  }

  te::graph::Graph::add(e);
}

void te::graph::BidirectionalGraph::removeEdge(int id)
{
  te::graph::Edge* e = getEdge(id);

  if(e == 0)
  {
    return;
  }

  //remove id from successor information from vFrom
  te::graph::Vertex* vFrom = getVertex(e->getIdFrom());

  if(vFrom)
  {
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

  //remove id from predecessor information from vTo
  te::graph::Vertex* vTo = getVertex(e->getIdTo());

  if(vTo)
  {
    std::set<int>::iterator it = vTo->getPredecessors().begin();

    while(it != vTo->getPredecessors().end())
    {
      if(*it == id)
      {
        vTo->getPredecessors().erase(it);
        break;
      }

      ++it;
    }
  }


  te::graph::Graph::removeEdge(id);
}


std::vector<te::graph::Edge*> te::graph::BidirectionalGraph::getInEdges(int vId)
{
  std::vector<te::graph::Edge*> vec;

  te::graph::Vertex* v = getVertex(vId);

  if(v)
  {
    std::set<int>::iterator it = v->getPredecessors().begin();

    while(it != v->getPredecessors().end())
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

std::vector<te::graph::Edge*> te::graph::BidirectionalGraph::getOutEdges(int vId)
{
  std::vector<te::graph::Edge*> vec;

  te::graph::Vertex* v = getVertex(vId);

  if(v)
  {
    std::set<int>::iterator it = v->getSuccessors().begin();

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
