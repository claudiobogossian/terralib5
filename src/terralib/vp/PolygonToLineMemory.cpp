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
  \file PolygonToLineMemory.h

  \brief Polygon to Line Vector Processing functions.
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

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/LineString.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/NumericStatisticalSummary.h"
#include "../statistics/core/Utils.h"

#include "PolygonToLineMemory.h"
#include "Config.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <map>
#include <math.h>
#include <string>
#include <vector>

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

te::vp::PolygonToLineMemory::PolygonToLineMemory()
{}

te::vp::PolygonToLineMemory::~PolygonToLineMemory()
{}

bool te::vp::PolygonToLineMemory::run() throw(te::common::Exception)
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

        std::auto_ptr<te::gm::MultiLineString> lineResult = polygon2Line(geom.get());
        if(!lineResult->isValid())
          geomState = false;
        
        outDsItem->setGeometry(i, lineResult.release());
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

std::auto_ptr<te::gm::MultiLineString> te::vp::PolygonToLineMemory::polygon2Line(te::gm::Geometry* geom)
{
  std::vector<te::gm::LineString*> lines;
  std::auto_ptr<te::gm::MultiLineString> lineResult(new te::gm::MultiLineString(0, te::gm::MultiLineStringType, geom->getSRID()));

  getLines(geom, lines);

  if(lines.size() > 1)
  {
    for(size_t i = 0; i < lines.size(); ++i)
      lineResult->Union(lines[i]);
  }
  else
  {
    lineResult->add(lines[0]);
  }

  return lineResult;
}

void te::vp::PolygonToLineMemory::getLines(te::gm::Geometry* geom, std::vector<te::gm::LineString*>& lines)
{
  if(geom == 0)
    return;

  switch(geom->getGeomTypeId())
  {
    case te::gm::MultiPolygonType:
      getLines(dynamic_cast<te::gm::GeometryCollection*>(geom), lines);
    break;

    case te::gm::PolygonType:
      getLines(dynamic_cast<te::gm::Polygon*>(geom), lines);
    break;

    case te::gm::LineStringType:
      getLines(dynamic_cast<te::gm::LineString*>(geom), lines);
    break;

    default:
      return;
  }
}

void te::vp::PolygonToLineMemory::getLines(te::gm::GeometryCollection* gc, std::vector<te::gm::LineString*>& lines)
{
  assert(gc);

  for(std::size_t i = 0; i < gc->getNumGeometries(); ++i)
    getLines(gc->getGeometryN(i), lines);
}

void te::vp::PolygonToLineMemory::getLines(te::gm::Polygon* p, std::vector<te::gm::LineString*>& lines)
{
  assert(p);

  std::vector<te::gm::Curve*>& rings = p->getRings();

  for(std::size_t i = 0; i < rings.size(); ++i)
  {
    te::gm::LineString* lsCurve = dynamic_cast<te::gm::LineString*>(rings[i]);
    te::gm::LineString* ls = new te::gm::LineString(*lsCurve);
    
    getLines(ls, lines);
  }
}

void te::vp::PolygonToLineMemory::getLines(te::gm::LineString* l, std::vector<te::gm::LineString*>& lines)
{
  assert(l);
  lines.push_back(l);
}