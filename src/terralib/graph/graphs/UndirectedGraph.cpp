#include "UndirectedGraph.h"
#include "GraphCache.h"
#include "GraphData.h"
#include "GraphDataManager.h"
#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"


// Terralib Includes
#include "../common/STLUtils.h"

te::graph::UndirectedGraph::UndirectedGraph() : Graph()
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
  m_graphData = m_graphCache->checkCacheByVertexId(e->getIdFrom());
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
  m_graphData = m_graphCache->checkCacheByVertexId(e->getIdTo());
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
