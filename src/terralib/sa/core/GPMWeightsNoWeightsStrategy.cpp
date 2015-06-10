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
  \file GPMWeightsNoWeightsStrategy.cpp

  \brief This class defines a class to calculates a weight for a GPM using No Weights strategy.
*/

// Terralib Includes
#include "../../datatype/SimpleData.h"
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/GraphMetadata.h"
#include "../../graph/core/Vertex.h"
#include "../../graph/iterator/MemoryIterator.h"
#include "GeneralizedProximityMatrix.h"
#include "GPMWeightsNoWeightsStrategy.h"

te::sa::GPMWeightsNoWeightsStrategy::GPMWeightsNoWeightsStrategy(bool normalize) : GPMWeightsAbstractStrategy(),
  m_normalize(normalize)
{
  m_type = NoWeightStrategy;
}

te::sa::GPMWeightsNoWeightsStrategy::~GPMWeightsNoWeightsStrategy()
{
}

void te::sa::GPMWeightsNoWeightsStrategy::calculate(GeneralizedProximityMatrix* gpm)
{
  //create weight property
  int weightIdx = createWeightAttribute(gpm);

  int nEdgeAttrs = gpm->getGraph()->getMetadata()->getEdgePropertySize();

  //iterate over all vertex
  te::graph::AbstractGraph* g = gpm->getGraph();

  te::graph::MemoryIterator* it = new te::graph::MemoryIterator(g);

  te::graph::Vertex* v = it->getFirstVertex();

  while(v)
  {
    std::set<int> neighbours = v->getSuccessors();
    std::set<int>::iterator itNeighbours = neighbours.begin();

    //calculate weight
    double weight = 1.;
    
    if(m_normalize)
      weight = 1./ neighbours.size();

    while(itNeighbours != neighbours.end())
    {
      te::graph::Edge* e = g->getEdge(*itNeighbours);

      if(e)
      {
        e->setAttributeVecSize(nEdgeAttrs);

        e->addAttribute(weightIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(weight));
      }

      ++itNeighbours;
    }

    v = it->getNextVertex();
  }
}
