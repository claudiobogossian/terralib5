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
  \file GetSubGraph.h

  \brief This class defines a function used to get a sub graph for a graph

*/

// Terralib
#include "../../common/Translator.h"
#include "../../common/progress/TaskProgress.h"
#include "../core/AbstractGraph.h"
#include "../core/Edge.h"
#include "../core/Vertex.h"
#include "../graphs/BidirectionalGraph.h"
#include "../graphs/Graph.h"
#include "../iterator/SequenceIterator.h"
#include "../Config.h"
#include "../Exception.h"
#include "GetSubGraph.h"

// STL
#include <cstdlib>
#include <iostream>


te::graph::GetSubGraph::GetSubGraph(te::graph::BidirectionalGraph* graph, te::graph::AbstractGraph* outGraph, int vertexId)
{
  te::graph::Vertex* vertex = graph->getVertex(vertexId);

  te::graph::Vertex* outVertex = new  te::graph::Vertex(vertex);
  
  outGraph->add(outVertex);
  
  std::set<int> predecessors;

  predecessors.insert(vertex->getPredecessors().begin(), vertex->getPredecessors().end());
  
  std::set<int>::iterator it = predecessors.begin();

  std::set<int> vertexIdSet;
  
  vertexIdSet.insert(vertex->getId());
  
  while(it != predecessors.end())
  {
    int edgeId = *(it);

    te::graph::Edge* e = graph->getEdge(edgeId);

    if(e)
    {
      te::graph::Edge* outE = new te::graph::Edge(e);

      outGraph->add(outE);

      te::graph::Vertex* vFrom = graph->getVertex(e->getIdFrom());

      if(vFrom)
      {
        std::set<int>::iterator itSet = vertexIdSet.find(vFrom->getId());

        if(itSet == vertexIdSet.end())
        {
          vertexIdSet.insert(vFrom->getId());

          te::graph::Vertex* outVFrom = new  te::graph::Vertex(vFrom);

          outGraph->add(outVFrom);

          getPredecessor(vFrom, graph, outGraph, vertexIdSet);
        }
      }
    }

    ++it;
  }
}

te::graph::GetSubGraph::~GetSubGraph()
{
}

void te::graph::GetSubGraph::getPredecessor(te::graph::Vertex* v, te::graph::BidirectionalGraph* g, te::graph::AbstractGraph* outGraph, std::set<int>& vertexIdSet)
{
  if(v->getPredecessors().empty() == false)
  {
    std::set<int> predecessors;

    predecessors.insert(v->getPredecessors().begin(), v->getPredecessors().end());

    std::set<int>::iterator it = predecessors.begin();

    while(it != predecessors.end())
    {
      int edgeId = *(it);

      te::graph::Edge* e = g->getEdge(edgeId);

      if(e)
      {
        te::graph::Edge* outE = new te::graph::Edge(e);

        outGraph->add(outE);
  
        te::graph::Vertex* vFrom = g->getVertex(e->getIdFrom());

        if(vFrom)
        {
          std::set<int>::iterator itSet = vertexIdSet.find(vFrom->getId());

          if(itSet == vertexIdSet.end())
          {
            vertexIdSet.insert(vFrom->getId());

            te::graph::Vertex* outVFrom = new  te::graph::Vertex(vFrom);

            outGraph->add(outVFrom);

            getPredecessor(vFrom, g, outGraph, vertexIdSet);
          }
        }
      }

      ++it;
    }
  }
}
