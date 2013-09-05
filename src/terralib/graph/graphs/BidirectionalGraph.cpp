#include "BidirectionalGraph.h"
#include "GraphCache.h"
#include "GraphData.h"
#include "GraphDataManager.h"
#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

//STL
#include <iterator>

// Terralib Includes
#include "../common/STLUtils.h"

te::graph::BidirectionalGraph::BidirectionalGraph() :Graph()
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
  if(m_graphCache->checkCacheByVertexId(e->getIdFrom()))
  {
    te::graph::Vertex* vFrom = getVertex(e->getIdFrom());
    if(vFrom)
      vFrom->getSuccessors().insert(e->getId());
  }

  if(m_graphCache->checkCacheByVertexId(e->getIdTo()))
  {
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
