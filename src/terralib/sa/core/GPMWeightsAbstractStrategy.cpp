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
  \file GPMWeightsAbstractStrategy.cpp

  \brief This class defines a an Abstract class to calculates a weight for a GPM.
*/

// Terralib Includes
#include "../../datatype/AbstractData.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/SimpleProperty.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/GraphMetadata.h"
#include "GeneralizedProximityMatrix.h"
#include "GPMWeightsAbstractStrategy.h"

// STL Includes
#include <cassert>

te::sa::GPMWeightsAbstractStrategy::GPMWeightsAbstractStrategy() : m_gpm(0)
{
}

te::sa::GPMWeightsAbstractStrategy::~GPMWeightsAbstractStrategy()
{
}

te::sa::GPMWeightsStrategyType te::sa::GPMWeightsAbstractStrategy::getWeightsType()
{
  return m_type;
}

void te::sa::GPMWeightsAbstractStrategy::createWeightAttribute(GeneralizedProximityMatrix* gpm)
{
  assert(gpm);

  //add edge property
  te::dt::SimpleProperty* p = new te::dt::SimpleProperty(TE_SA_WEIGHT_ATTR_NAME, te::dt::DOUBLE_TYPE);
  p->setParent(0);
  p->setId(0);

  gpm->getGraph()->addEdgeProperty(p);
}

bool te::sa::GPMWeightsAbstractStrategy::getDistanceAttributeIndex(te::graph::GraphMetadata* gm, int& index)
{
  for(int i = 0; i < gm->getEdgePropertySize(); ++i)
  {
    if(gm->getEdgeProperty(i)->getName() == TE_SA_DISTANCE_ATTR_NAME)
    {
      index = i;
      return true;
    }
  }

  return false;
}

double te::sa::GPMWeightsAbstractStrategy::getDistanceAttributeValue(te::graph::Edge* e, const int index)
{
  te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = dynamic_cast< te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* >(e->getAttributes()[index]);

  if(sd)
    return sd->getValue();

  return 0.;
}
