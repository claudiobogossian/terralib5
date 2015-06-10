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
  \file GPMWeightsSquaredInverseDistanceStrategy.h

  \brief This class defines a class to calculates a weight for a GPM using Squared Inverse Distance strategy.
*/

// Terralib Includes
#include "../../datatype/SimpleData.h"
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/GraphMetadata.h"
#include "../../graph/core/Vertex.h"
#include "../../graph/iterator/MemoryIterator.h"
#include "GeneralizedProximityMatrix.h"
#include "GPMWeightsSquaredInverseDistanceStrategy.h"

te::sa::GPMWeightsSquaredInverseDistanceStrategy::GPMWeightsSquaredInverseDistanceStrategy(bool normalize, double a, double factor) : GPMWeightsAbstractStrategy(),
  m_normalize(normalize),
  m_a(a),
  m_factor(factor)
{
  m_type = SquaredInverseDistanceStrategy;
}

te::sa::GPMWeightsSquaredInverseDistanceStrategy::~GPMWeightsSquaredInverseDistanceStrategy()
{
}

void te::sa::GPMWeightsSquaredInverseDistanceStrategy::calculate(GeneralizedProximityMatrix* gpm)
{
  //create weight property
  int weightIdx = createWeightAttribute(gpm);

  int nEdgeAttrs = gpm->getGraph()->getMetadata()->getEdgePropertySize();

  //check if the gpm has the distance attribute
  int distanceIdx;
  bool hasDistance = getDistanceAttributeIndex(gpm->getGraph()->getMetadata(), distanceIdx);

  //iterate over all vertex
  te::graph::AbstractGraph* g = gpm->getGraph();
  te::graph::MemoryIterator* it = new te::graph::MemoryIterator(g);
  te::graph::Vertex* v = it->getFirstVertex();

  while(v)
  {
    std::set<int> neighbours = v->getSuccessors();
    std::set<int>::iterator itNeighbours = neighbours.begin();

    //calculate weight
    double tot = 0.;
    std::vector<double> weightVec;

    while(itNeighbours != neighbours.end())
    {
      double weight = 0.;

      te::graph::Edge* e = g->getEdge(*itNeighbours);

      if(e)
      {
        //get distance attribute
        if(hasDistance)
        {
          double distance = getDistanceAttributeValue(e, distanceIdx);

          if(distance != 0.)
            weight += m_a * (1. / (distance * distance));
        }

        //extra information calculate in TerraLib 4
        //if (attr.WasNetworkObjectsDistanceComputed())
        //if ((d_net = attr.NetworkObjectsDistance()) != 0.0) 
        //w += (params_.b_)*1/(d_net*d_net);

        //if (attr.WasNetworkMinimumPathComputed())
        //if ((d_conn = attr.NetworkMinimumPath()) != 0.0) 
        //w += (params_.c_)*1/(d_conn*d_conn);
      }

      if(weight == 0.)
        weight = 1.;

      weightVec.push_back(weight * m_factor);

      tot += weight;

      ++itNeighbours;
    }

    itNeighbours = neighbours.begin();
    std::vector<double>::iterator itWeights = weightVec.begin();

    while(itNeighbours != neighbours.end())
    {
      te::graph::Edge* e = g->getEdge(*itNeighbours);

      if(e)
      {
        double weight = *itWeights;

        if(m_normalize && tot != 0.)
        {
          weight = weight / tot;
        }

        e->setAttributeVecSize(nEdgeAttrs);

        e->addAttribute(weightIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(weight));
      }

      ++itWeights;
      ++itNeighbours;
    }

    v = it->getNextVertex();
  }
}
