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
  \file AddDeepAttribute.cpp

  \brief This class defines a function used to add to a graph the deep information attribute

*/

// Terralib
#include "../../common/Translator.h"
#include "../../common/progress/TaskProgress.h"
#include "../../datatype/SimpleProperty.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/Enums.h"
#include "../core/Edge.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../graphs/Graph.h"
#include "../graphs/BidirectionalGraph.h"
#include "../iterator/SequenceIterator.h"
#include "../Exception.h"
#include "AddDeepAttribute.h"


te::graph::AddDeepAttribute::AddDeepAttribute(te::graph::BidirectionalGraph* graph, std::string attributeName) 
{
  //clear graph cache
  graph->flush();

  //add new attribute
  te::dt::SimpleProperty* p = new te::dt::SimpleProperty(attributeName, te::dt::INT32_TYPE);
  p->setParent(0);
  p->setId(0);

  graph->addVertexProperty(p);

  // verify what the index of the new property
  int pIdx = 0;

  for(int i = 0; i < graph->getVertexPropertySize(); ++ i)
  {
    if(graph->getVertexProperty(i)->getName() == attributeName)
    {
      pIdx = i;
      break;
    }
  }

  //iterator for all vertex objects
  te::graph::SequenceIterator* it = new te::graph::SequenceIterator(graph);

  te::graph::Vertex* vertex = it->getFirstVertex();

  te::common::TaskProgress task;

  task.setTotalSteps(it->getVertexInteratorCount());
  task.setMessage("Add Deep Attribute Operation");

  while(it->isVertexIteratorAfterEnd() == false)
  {
    if(vertex)
    {
      std::set<int> vertexIdSet;

      vertexIdSet.insert(vertex->getId());

      int deepValue = 0;

      //calculate deep attribute
      calculateDeepValue(vertex, graph, deepValue, vertexIdSet);

      vertex->addAttribute(pIdx, new te::dt::SimpleData<int, te::dt::INT32_TYPE>(deepValue));

      //set the vertex as durty
      graph->update(vertex);
    }

    vertex = it->getNextVertex();

    task.pulse();
  }

  delete it;

  //clear graph cache
  graph->flush();
}

te::graph::AddDeepAttribute::~AddDeepAttribute()  
{
}

void te::graph::AddDeepAttribute::calculateDeepValue(te::graph::Vertex* v, te::graph::BidirectionalGraph* g, int& deepValue, std::set<int>& vertexIdSet)
{
  if(v->getPredecessors().empty() == false)
  {
    deepValue += v->getPredecessors().size();

    std::set<int> predecessors;

    predecessors.insert(v->getPredecessors().begin(), v->getPredecessors().end());

    std::set<int>::iterator it = predecessors.begin();

    while(it != predecessors.end())
    {
      te::graph::Edge* e = g->getEdge(*it);

      if(e)
      {
        te::graph::Vertex* vFrom = g->getVertex(e->getIdFrom());

        if(vFrom)
        {
          std::set<int>::iterator itSet = vertexIdSet.find(vFrom->getId());

          if(itSet == vertexIdSet.end())
          {
            vertexIdSet.insert(vFrom->getId());

            calculateDeepValue(vFrom, g, deepValue, vertexIdSet);
          }
        }
      }

      ++it;
    }
  }
}
