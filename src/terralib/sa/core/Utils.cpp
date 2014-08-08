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
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/AbstractData.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/MultiPolygon.h"
#include "../../geometry/Point.h"
#include "../../geometry/Polygon.h"
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/core/GraphMetadata.h"
#include "../../graph/core/Vertex.h"
#include "../../maptools/AbstractLayer.h"
#include "../../maptools/DataSetLayer.h"
#include "../../raster/Grid.h"
#include "../../raster/RasterProperty.h"
#include "../../se/Utils.h"
#include "GeneralizedProximityMatrix.h"
#include "Utils.h"

// STL
#include <cassert>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

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

double te::sa::CalculateDistance(te::gm::Geometry* geom, te::gm::Coord2D& coord)
{
  double distance = std::numeric_limits<double>::max();

  std::auto_ptr<te::gm::Point> point(new te::gm::Point(coord.x, coord.y));

  point->setSRID(geom->getSRID());

  if(geom->getGeomTypeId() == te::gm::PointType)
  {
    te::gm::Point* p = ((te::gm::Point*)geom);

    distance = p->distance(point.get());
  }
  else if(geom->getGeomTypeId() == te::gm::PolygonType)
  {
    te::gm::Point* p = ((te::gm::Polygon*)geom)->getCentroid();

    distance = p->distance(point.get());

    delete p;
  }
  else if(geom->getGeomTypeId() == te::gm::MultiPolygonType)
  {
    te::gm::Polygon* poly = (te::gm::Polygon*)((te::gm::MultiPolygon*)geom)->getGeometryN(0);

    te::gm::Point* p = poly->getCentroid();

    distance = p->distance(point.get());

    delete p;
  }

  return distance;
}

te::gm::Coord2D te::sa::GetCentroidCoord(te::gm::Geometry* geom)
{
  assert(geom);

  te::gm::Coord2D coord;  

  if(geom->getGeomTypeId() == te::gm::PointType)
  {
    te::gm::Point* p = ((te::gm::Point*)geom);

    coord.x = p->getX();
    coord.y = p->getY();
  }
  else if(geom->getGeomTypeId() == te::gm::PolygonType)
  {
    te::gm::Point* p = ((te::gm::Polygon*)geom)->getCentroid();

    coord.x = p->getX();
    coord.y = p->getY();

    delete p;
  }
  else if(geom->getGeomTypeId() == te::gm::MultiPolygonType)
  {
    te::gm::Polygon* poly = (te::gm::Polygon*)((te::gm::MultiPolygon*)geom)->getGeometryN(0);

    te::gm::Point* p = poly->getCentroid();

    coord.x = p->getX();
    coord.y = p->getY();

    delete p;
  }

  return coord;
}

double te::sa::GetArea(te::gm::Geometry* geom)
{
  assert(geom);

  if(geom->getGeomTypeId() == te::gm::PolygonType)
  {
    return ((te::gm::Polygon*)geom)->getArea();
  }
  else if(geom->getGeomTypeId() == te::gm::MultiPolygonType)
  {
    te::gm::Polygon* poly = (te::gm::Polygon*)((te::gm::MultiPolygon*)geom)->getGeometryN(0);

    return poly->getArea();
  }

  return 0.;
}
