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
  \file AddRasterAttribute.cpp

  \brief This class defines a function used to add to a graph the raster pixel value as attribute

*/

// Terralib
#include "../../common/Translator.h"
#include "../../common/progress/TaskProgress.h"
#include "../../datatype/SimpleProperty.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/Enums.h"
#include "../../geometry/Coord2D.h"
#include "../../geometry/Point.h"
#include "../../raster/Grid.h"
#include "../../raster/Raster.h"
#include "../core/AbstractGraph.h"
#include "../core/Edge.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../graphs/Graph.h"
#include "../iterator/SequenceIterator.h"
#include "../Exception.h"
#include "AddRasterAttribute.h"


te::graph::AddRasterAttribute::AddRasterAttribute(te::graph::AbstractGraph* graph, std::string spatialAttributeName, std::string attributeName, te::rst::Raster* raster, int band) 
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
  task.setMessage("Add Raster Attribute Operation");

  int spatialPropertyId = -1;

  for(int i = 0; i < graph->getMetadata()->getVertexPropertySize(); ++i)
  {
    if(graph->getMetadata()->getVertexProperty(i)->getName() == spatialAttributeName)
    {
      spatialPropertyId = i;
      break;
    }
  }

  while(it->isVertexIteratorAfterEnd() == false)
  {
    if(vertex)
    {
      te::gm::Point* p = dynamic_cast<te::gm::Point*>(vertex->getAttributes()[spatialPropertyId]);

      if(p)
      {
        te::gm::Coord2D coord =  raster->getGrid()->geoToGrid(p->getX(), p->getY());

        double pixelValue;

        raster->getValue((int)coord.x, (int)coord.y, pixelValue, band);

        vertex->addAttribute(pIdx, new te::dt::SimpleData<int, te::dt::INT32_TYPE>((int)pixelValue));

        //set the vertex as durty
        graph->update(vertex);
      }
    }

    vertex = it->getNextVertex();

    task.pulse();
  }

  delete it;

  //clear graph cache
  graph->flush();
}

te::graph::AddRasterAttribute::~AddRasterAttribute()
{
}