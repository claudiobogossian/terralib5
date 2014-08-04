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
  \file terralib/sa/core/Utils.cpp

  \brief Utilitary function for spatial analysis module.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../datatype/AbstractData.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/GeometryProperty.h"
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/core/GraphMetadata.h"
#include "../../graph/core/Vertex.h"
#include "GeneralizedProximityMatrix.h"
#include "Utils.h"

// STL
#include <cassert>

int te::sa::AssociateGPMVertexAttribute(te::sa::GeneralizedProximityMatrix* gpm, te::da::DataSource* ds, std::string dataSetName, std::string attrLink, std::string attr, int dataType, int srid, int subType)
{
  assert(ds);
  assert(gpm);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //add graph attr
  int attrGraphIdx = te::sa::AddGraphVertexAttribute(graph, attr, dataType, srid, subType);

  //get the number of attributes from graph
  int attrSize = graph->getMetadata()->getVertexPropertySize();

  //dataset iterator
  std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(dataSetName);

  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    te::dt::AbstractData* ad = dataSet->getValue(attr).release();

    int idx = dataSet->getInt32(attrLink);

    te::graph::Vertex* v = graph->getVertex(idx);

    if(v)
    {
      //resize attr vector
      v->setAttributeVecSize(attrSize);

      //add attribute
      v->addAttribute(attrGraphIdx, ad);
    }
  }

  return attrGraphIdx;
}

int te::sa::AddGraphVertexAttribute(te::graph::AbstractGraph* graph, std::string attrName, int dataType, int srid, int subType)
{
  assert(graph);

  //add new attribute
  te::dt::Property* p = 0;
  
  if(dataType == te::dt::GEOMETRY_TYPE)
  {
    p = new te::gm::GeometryProperty(attrName, srid, (te::gm::GeomType)subType);
  }
  else
  {
    p = new te::dt::SimpleProperty(attrName, dataType);
  }

  p->setParent(0);
  p->setId(0);

  graph->addVertexProperty(p);

  // verify what the index of the new property
  int idx = 0;

  for(int i = 0; i < graph->getVertexPropertySize(); ++ i)
  {
    if(graph->getVertexProperty(i)->getName() == attrName)
    {
      idx = i;
      break;
    }
  }

  return idx;
}

int te::sa::AddGraphEdgeAttribute(te::graph::AbstractGraph* graph, std::string attrName, int dataType)
{
  assert(graph);

  //add new attribute
  te::dt::SimpleProperty* p = new te::dt::SimpleProperty(attrName, dataType);
  p->setParent(0);
  p->setId(0);

  graph->addEdgeProperty(p);

  // verify what the index of the new property
  int idx = 0;

  for(int i = 0; i < graph->getEdgePropertySize(); ++ i)
  {
    if(graph->getEdgeProperty(i)->getName() == attrName)
    {
      idx = i;
      break;
    }
  }

  return idx;
}

bool te::sa::GetGraphVertexAttrIndex(te::graph::AbstractGraph* graph, std::string attrName, int& index)
{
  assert(graph);

  for(int i = 0; i < graph->getVertexPropertySize(); ++ i)
  {
    if(graph->getVertexProperty(i)->getName() == attrName)
    {
      index = i;
      return true;
    }
  }

  return false;
}

bool te::sa::GetGraphEdgeAttrIndex(te::graph::AbstractGraph* graph, std::string attrName, int& index)
{
  assert(graph);

  for(int i = 0; i < graph->getEdgePropertySize(); ++ i)
  {
    if(graph->getEdgeProperty(i)->getName() == attrName)
    {
      index = i;
      return true;
    }
  }

  return false;
}

double te::sa::GetDataValue(te::dt::AbstractData* ad)
{
  assert(ad);

  std::string strValue = ad->toString();

  return atof(strValue.c_str());
}
