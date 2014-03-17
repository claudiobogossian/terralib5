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
  \file QueryGraphBuilder.cpp

  \brief This class defines the Query strategy to build a graph having
         a exist graph as input parameters.

        The query must be defined using only the terralib Query module
        elements.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/query/Expression.h"
#include "../core/AbstractGraphFactory.h"
#include "../core/Edge.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../graphs/Graph.h"
#include "../iterator/QueryIterator.h"
#include "../Config.h"
#include "../Exception.h"
#include "QueryGraphBuilder.h"

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
  m_graph.reset(te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo));

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
  {
    te::graph::QueryIterator* it = new te::graph::QueryIterator(g, eVertex);

    te::graph::Vertex* vertex = it->getFirstVertex();

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

      vertex = it->getNextVertex();

      t.pulse();
    }

    delete it;
  }

  {
    te::graph::QueryIterator* it = new te::graph::QueryIterator(g, eEdge);

    te::graph::Edge* edge = it->getFirstEdge();

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

      edge = it->getNextEdge();

      t.pulse();
    }

    delete it;

  }

  return true;
}

