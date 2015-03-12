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
  \file LineToPolygonMemory.h

  \brief Line to Polygon Vector Processing functions.
*/

//Terralib

#include "../common/progress/TaskProgress.h"
#include "../common/Logger.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/LinearRing.h"
#include "../geometry/Polygon.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/NumericStatisticalSummary.h"
#include "../statistics/core/Utils.h"

#include "LineToPolygonMemory.h"
#include "Config.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <vector>

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

te::vp::LineToPolygonMemory::LineToPolygonMemory()
{}

te::vp::LineToPolygonMemory::~LineToPolygonMemory()
{}

bool te::vp::LineToPolygonMemory::run()
{
  std::auto_ptr<te::da::DataSetType> outDsType = buildOutDataSetType();

  std::auto_ptr<te::da::DataSetType> inDsType = m_inDsrc->getDataSetType(m_inDsetName);
  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(inDsType.get());
  std::string geomName = geomProp->getName();

  std::auto_ptr<te::da::DataSet> inDset;

  if(m_oidSet == 0)
    inDset = m_inDsrc->getDataSet(m_inDsetName);
  else
    inDset = m_inDsrc->getDataSet(m_inDsetName, m_oidSet);

  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(outDsType.get()));

  te::common::TaskProgress task("Processing...");
  task.setTotalSteps(inDset->size());
  task.useTimer(true);

  inDset->moveBeforeFirst();
  while(inDset->moveNext())
  {
    te::mem::DataSetItem* outDsItem = new te::mem::DataSetItem(outDSet.get());
    bool geomState = true;

    for(size_t i = 0; i < outDsItem->getNumProperties(); ++i)
    {
      if(outDsItem->getPropertyDataType(i) != te::dt::GEOMETRY_TYPE)
      {
        outDsItem->setValue(i, inDset->getValue(outDsItem->getPropertyName(i)).get()->clone());
      }
      else
      {
        std::auto_ptr<te::gm::Geometry> geom = inDset->getGeometry(geomName);
        if(!geom->isValid())
        {
          geomState = false;
          continue;
        }

        std::auto_ptr<te::gm::MultiPolygon> polygonResult = line2Polygon(geom.get());
        if(!polygonResult->isValid() || polygonResult->isEmpty())
          geomState = false;

        outDsItem->setGeometry(i, polygonResult.release());
      }
    }
    if(!geomState)
      continue;

    outDSet->add(outDsItem);

    if (task.isActive() == false)
      throw te::vp::Exception(TE_TR("Operation canceled!"));
  
    task.pulse();
  }

  te::vp::Save(m_outDsrc.get(), outDSet.get(), outDsType.get());

  return true;
}

std::auto_ptr<te::gm::MultiPolygon> te::vp::LineToPolygonMemory::line2Polygon(te::gm::Geometry* geom)
{
  std::vector<te::gm::Polygon*> polygons;
  std::auto_ptr<te::gm::MultiPolygon> polygonResult(new te::gm::MultiPolygon(0, te::gm::MultiPolygonType, geom->getSRID()));

  getPolygons(geom, polygons);

  if(polygons.size() > 1)
  {
    for(std::size_t i = 0; i < polygons.size(); ++i)
      polygonResult->add(polygons[i]);
  }
  else
  {
    polygonResult->add(polygons[0]);
  }

  return polygonResult;
}

void te::vp::LineToPolygonMemory::getPolygons(te::gm::Geometry* geom, std::vector<te::gm::Polygon*>& polygons)
{
  if(geom == 0)
    return;

  switch(geom->getGeomTypeId())
  {
    case te::gm::MultiLineStringType:
      getPolygons(dynamic_cast<te::gm::GeometryCollection*>(geom), polygons);
    break;

    case te::gm::LineStringType:
      getPolygons(dynamic_cast<te::gm::LineString*>(geom), polygons);
    break;

    default:
      break;
  }
}

void te::vp::LineToPolygonMemory::getPolygons(te::gm::GeometryCollection* gc, std::vector<te::gm::Polygon*>& polygons)
{
  assert(gc);

  for(std::size_t i = 0; i < gc->getNumGeometries(); ++i)
    getPolygons(gc->getGeometryN(i), polygons);
}

void te::vp::LineToPolygonMemory::getPolygons(te::gm::LineString* l, std::vector<te::gm::Polygon*>& polygons)
{
  assert(l);

  if(l->isClosed())
  {
    te::gm::Polygon* p = new te::gm::Polygon(0, te::gm::PolygonType, l->getSRID());

    te::gm::LinearRing* ring = new te::gm::LinearRing(l->getNPoints(), te::gm::LineStringType);

    for(std::size_t i = 0; i < l->getNPoints(); ++i)
      ring->setPoint(i, l->getX(i), l->getY(i));

    p->add(ring);

    getPolygons(p, polygons);
  }
}

void te::vp::LineToPolygonMemory::getPolygons(te::gm::Polygon* p, std::vector<te::gm::Polygon*>& polygons)
{
  assert(p);
  polygons.push_back(p);
}