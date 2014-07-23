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
  \file GPMConstructorAbstractStrategy.cpp

  \brief This class defines a an Abstract class for a GPM constructor.
*/

// Terralib Includes
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/MultiPolygon.h"
#include "../../geometry/Point.h"
#include "../../geometry/Polygon.h"
#include "../../graph/core/Vertex.h"
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/graphs/DirectedGraph.h"
#include "GeneralizedProximityMatrix.h"
#include "GPMConstructorAbstractStrategy.h"

// STL Includes
#include <cassert>

te::sa::GPMConstructorAbstractStrategy::GPMConstructorAbstractStrategy() : m_ds(0), m_gpm(0)
{
  m_edgeId = 0;
}

te::sa::GPMConstructorAbstractStrategy::~GPMConstructorAbstractStrategy()
{
}

te::sa::GPMConstructorStrategyType te::sa::GPMConstructorAbstractStrategy::getConstructorType()
{
  return m_type;
}

void te::sa::GPMConstructorAbstractStrategy::construct(te::da::DataSource* ds, GeneralizedProximityMatrix* gpm)
{
  assert(ds);
  m_ds = ds;

  assert(gpm);
  m_gpm = gpm;

  //build vertex
  createVertexObjects();

  // build edges using specific strategy
  constructStrategy();
}

void te::sa::GPMConstructorAbstractStrategy::createVertexObjects()
{
  //get input data information
  std::auto_ptr<te::da::DataSet> dataSet = m_ds->getDataSet(m_gpm->getDataSetName());

  std::auto_ptr<te::da::DataSetType> dataSetType = m_ds->getDataSetType(m_gpm->getDataSetName());

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dataSetType.get());

  int srid = gp->getSRID();

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet.get());

  //create graph vertex attrs
  te::gm::GeometryProperty* gProp = new te::gm::GeometryProperty(TE_SA_GEOMETRY_ATTR_NAME);
  gProp->setId(0);
  gProp->setGeometryType(te::gm::PointType);
  gProp->setSRID(srid);

  m_gpm->getGraph()->addVertexProperty(gProp);

  //create graph vertex
  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(m_gpm->getAttributeName());

    te::graph::Vertex* v = new te::graph::Vertex(id);
    
    v->setAttributeVecSize(1);

    std::auto_ptr<te::gm::Geometry> g = dataSet->getGeometry(geomPos);

    te::dt::AbstractData* ad = 0;

    if(g->getGeomTypeId() == te::gm::PointType)
    {
      g->setSRID(srid);

      ad = g->clone();
    }
    else if(g->getGeomTypeId() == te::gm::PolygonType)
    {
      te::gm::Point* p = ((te::gm::Polygon*)g.get())->getCentroid();
      p->setSRID(srid);

      ad = p;
    }
    else if(g->getGeomTypeId() == te::gm::MultiPolygonType)
    {
      te::gm::Polygon* poly = (te::gm::Polygon*)((te::gm::MultiPolygon*)g.get())->getGeometryN(0);

      te::gm::Point* p = poly->getCentroid();
      p->setSRID(srid);

      ad = p;
    }

    v->addAttribute(0, ad);

    m_gpm->getGraph()->add(v);
  }
}

void te::sa::GPMConstructorAbstractStrategy::createDistanceAttribute(GeneralizedProximityMatrix* gpm)
{
  assert(gpm);

  te::dt::SimpleProperty* p = new te::dt::SimpleProperty(TE_SA_DISTANCE_ATTR_NAME, te::dt::DOUBLE_TYPE);
  p->setParent(0);
  p->setId(0);

  gpm->getGraph()->addEdgeProperty(p);
}

int te::sa::GPMConstructorAbstractStrategy::getEdgeId()
{
  int id = m_edgeId;

  m_edgeId++;

  return id;
}
