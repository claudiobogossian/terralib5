#include "QueryGraphBuilder.h"

#include "AbstractGraphFactory.h"
#include "Config.h"
#include "Edge.h"
#include "Exception.h"
#include "Graph.h"
#include "GraphMetadata.h"
#include "QueryIterator.h"
#include "Vertex.h"
#include "VertexProperty.h"

// TerraLib
#include "../../common/Translator.h"
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/query/Expression.h"

// STL
#include <cassert>
#include <cstdlib>
#include <iostream>


te::graph::QueryGraphBuilder::QueryGraphBuilder() : AbstractGraphBuilder()
{

}

te::graph::QueryGraphBuilder::~QueryGraphBuilder()
{
}

bool te::graph::QueryGraphBuilder::build(te::graph::AbstractGraph* g, te::da::Expression* eEdge, te::da::Expression* eVertex, const std::map<std::string, std::string>& dsInfo, const std::string& graphType, const std::map<std::string, std::string>& gInfo)
{
  //create output graph
  m_graph = te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo);

  assert(m_graph);

  //copy attributes
  for(int t = 0; t < g->getEdgePropertySize(); ++ t)
  {
    te::dt::Property* p = g->getMetadata()->getEdgeProperty(t)->clone();
    p->setParent(0);

    m_graph->addEdgeProperty(p);
  }

  for(int t = 0; t < g->getVertexPropertySize(); ++ t)
  {
    te::dt::Property* p = g->getMetadata()->getVertexProperty(t)->clone();
    p->setParent(0);

    m_graph->addVertexProperty(p);
  }

  //create iterator
  te::graph::AbstractIterator* oldIt = g->getIterator();

  {
    te::graph::QueryIterator* it = new te::graph::QueryIterator(g, eVertex);

    g->setIterator(it);

    te::graph::Vertex* vertex = g->getFirstVertex();

    te::common::TaskProgress t;

    t.setTotalSteps(it->getVertexInteratorCount());
    t.setMessage(TR_GRAPH("Query Graph Builder - Vertex..."));
    t.useTimer(true);


    //copy elements
    while(it->isVertexIteratorAfterEnd() == false)
    {
      if(vertex)
      {
        te::graph::Vertex* vNew = new te::graph::Vertex(vertex);

        m_graph->add(vNew);
      }

      vertex = g->getNextVertex();

      t.pulse();
    }

    delete it;
  }

  {
    te::graph::QueryIterator* it = new te::graph::QueryIterator(g, eEdge);

    g->setIterator(it);

    te::graph::Edge* edge = g->getFirstEdge();

    te::common::TaskProgress t;

    t.setTotalSteps(it->getEdgeInteratorCount());
    t.setMessage(TR_GRAPH("Query Graph Builder - Edges..."));
    t.useTimer(true);


    //copy elements
    while(it->isEdgeIteratorAfterEnd() == false)
    {
      if(edge)
      {
        te::graph::Edge* eNew = new te::graph::Edge(edge);

        m_graph->add(eNew);
      }

      edge = g->getNextEdge();

      t.pulse();
    }

    delete it;

  }

  g->setIterator(oldIt);

  return true;
}

