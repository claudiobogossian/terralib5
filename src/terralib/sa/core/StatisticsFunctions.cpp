/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/core/StatisticsFunctions.cpp

  \brief Functions used in statistics operations.

  \reference Methods adapted from TerraLib4.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../datatype/SimpleData.h"
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/core/Vertex.h"
#include "../../graph/iterator/MemoryIterator.h"
#include "GeneralizedProximityMatrix.h"
#include "StatisticsFunctions.h"
#include "Utils.h"

// STL
#include <cassert>


double te::sa::Sum(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx)
{
  assert(gpm);

  double sum = 0.;

  //get graph
  te::graph::AbstractGraph* graph = gpm->getGraph();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    te::dt::AbstractData* ad = v->getAttributes()[attrIdx];

    if(ad)
    {
      sum += te::sa::GetDataValue(ad);
    }

    v = it->getNextVertex();
  }

  return sum;
}

double te::sa::FirstMoment(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx)
{
  assert(gpm);

  double mean = 0.;
  int count = 0;

  //get graph
  te::graph::AbstractGraph* graph = gpm->getGraph();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    te::dt::AbstractData* ad = v->getAttributes()[attrIdx];

    if(ad)
    {
      double attrValue = te::sa::GetDataValue(ad);

      mean += attrValue;

      ++count;
    }

    v = it->getNextVertex();
  }

  return mean /= count;
}

double te::sa::SecondMoment(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx, double mean)
{
  assert(gpm);

  double ssd = 0.; //sum of squares of desviation
  int count = 0;

  //get graph
  te::graph::AbstractGraph* graph = gpm->getGraph();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    te::dt::AbstractData* ad = v->getAttributes()[attrIdx];

    if(ad)
    {
      double attrValue = te::sa::GetDataValue(ad);

      ssd += pow((attrValue - mean), 2);

      ++count;
    }

    v = it->getNextVertex();
  }

  return ssd /= count;
}
