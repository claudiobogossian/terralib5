#include "DirectedGraph.h"
#include "GraphCache.h"
#include "GraphData.h"
#include "GraphDataManager.h"
#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

// Terralib Includes
#include "../common/STLUtils.h"

te::graph::DirectedGraph::DirectedGraph() : Graph()
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
  m_graphData =  m_graphCache->checkCacheByVertexId(e->getIdFrom());

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
