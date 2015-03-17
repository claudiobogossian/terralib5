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
  \file MultipartToSinglepart.h

  \brief Aggregation Vector Processing functions.
*/

//Terralib

#include "../common/progress/TaskProgress.h"
#include "../common/Logger.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/ObjectId.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "MultipartToSinglepart.h"
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

te::vp::MultipartToSinglepart::MultipartToSinglepart()
{
}

te::vp::MultipartToSinglepart::~MultipartToSinglepart()
{
}

bool te::vp::MultipartToSinglepart::run()
{
  te::dt::Property* prop = te::da::GetFirstSpatialProperty(m_inDsType.get());
  te::gm::GeometryProperty* geomProp = dynamic_cast<te::gm::GeometryProperty*>(prop);

  if(!te::vp::IsMultiType(geomProp->getGeometryType()))
  {
    throw te::common::Exception(TE_TR("This layer has not a multipart geometry!"));
  }

  if(m_oidSet)
    return runSelected();
  else
    return runAll();
}

bool te::vp::MultipartToSinglepart::runAll()
{
  std::auto_ptr<te::da::DataSet> inDs;

  inDs = m_inDsrc->getDataSet(m_inDsName);

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(inDs.get());

  std::auto_ptr<te::da::DataSetType> outDst = getOutDst();

  std::auto_ptr<te::mem::DataSet> outDs(new te::mem::DataSet(outDst.get()));

  inDs->moveBeforeFirst();

  std::size_t pkCount = 0;

  while(inDs->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> geom = inDs->getGeometry(geomPos);

    std::vector<te::gm::Geometry*> geoms;
    te::vp::Multi2Single(geom.release(), geoms);

    if(geoms.size() == 1)
    {
      te::mem::DataSetItem* item = new te::mem::DataSetItem(outDs.get());

      for(std::size_t i = 0; i < inDs->getNumProperties(); ++i)
      {
        if(i == 0)
        {
          item->setInt32(0, pkCount);
          ++pkCount;
          item->setValue(1, inDs->getValue(i).release());
        }
        else if(i != geomPos)
          item->setValue((i+1), inDs->getValue(i).release());
        else
          item->setGeometry((i+1), geoms[0]);
      }

      outDs->add(item);
    }
    else
    {
      for(std::size_t g = 0; g < geoms.size(); ++g)
      {
        te::mem::DataSetItem* item = new te::mem::DataSetItem(outDs.get());

        for(std::size_t i = 0; i < inDs->getNumProperties(); ++i)
        {
          if(i == 0)
          {
            item->setInt32(0, pkCount);
            ++pkCount;
            item->setValue(1, inDs->getValue(i).release());
          }
          else if(i != geomPos)
            item->setValue((i+1), inDs->getValue(i).release());
          else
            item->setGeometry((i+1), geoms[g]);
        }

        outDs->add(item);
      }
    }
  }

  te::vp::Save(m_outDsrc.get(), outDs.get(), outDst.get());

  return true;
}

bool te::vp::MultipartToSinglepart::runSelected()
{
  std::auto_ptr<te::da::DataSet> inDs;

  inDs = m_inDsrc->getDataSet(m_inDsName);

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(inDs.get());

  std::auto_ptr<te::da::DataSetType> outDst = getOutDst(true);

  std::auto_ptr<te::mem::DataSet> outDs(new te::mem::DataSet(outDst.get()));

  inDs->moveBeforeFirst();

  std::size_t pkCount = 0;

  while(inDs->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> geom = inDs->getGeometry(geomPos);

    te::da::ObjectId* geomOid = te::da::GenerateOID(inDs.get(), m_oidSet->getPropertyNames());

    std::vector<te::gm::Geometry*> geoms;
    std::vector<te::gm::Geometry*> multiGeoms;

    if(m_oidSet->contains(geomOid))
    {
      te::vp::Multi2Single(geom.release(), geoms);

      for(std::size_t i = 0; i < geoms.size(); ++i)
      {
        multiGeoms.push_back(single2multi(geoms[i]));
      }
    }
    else
    {
      geoms.push_back(geom.release());
    }

    if(geoms.size() == 1)
    {
      te::mem::DataSetItem* item = new te::mem::DataSetItem(outDs.get());

      for(std::size_t i = 0; i < inDs->getNumProperties(); ++i)
      {
        if(i == 0)
        {
          item->setInt32(0, pkCount);
          ++pkCount;
          item->setValue(1, inDs->getValue(i).release());
        }
        else if(i != geomPos)
          item->setValue((i+1), inDs->getValue(i).release());
        else
          item->setGeometry((i+1), geoms[0]);
      }

      outDs->add(item);
    }
    else
    {
      for(std::size_t g = 0; g < multiGeoms.size(); ++g)
      {
        te::mem::DataSetItem* item = new te::mem::DataSetItem(outDs.get());

        for(std::size_t i = 0; i < inDs->getNumProperties(); ++i)
        {
          if(i == 0)
          {
            item->setInt32(0, pkCount);
            ++pkCount;
            item->setValue(1, inDs->getValue(i).release());
          }
          else if(i != geomPos)
            item->setValue((i+1), inDs->getValue(i).release());
          else
            item->setGeometry((i+1), multiGeoms[g]);
        }

        outDs->add(item);
      }
    }
  }

  te::vp::Save(m_outDsrc.get(), outDs.get(), outDst.get());
  return true;
}

void te::vp::MultipartToSinglepart::setInput(te::da::DataSourcePtr inDsrc,
                                             std::string inDsName,
                                             std::auto_ptr<te::da::DataSetType> inDsType,
                                             const te::da::ObjectIdSet* oidSet)
{
  m_inDsrc = inDsrc;
  m_inDsName = inDsName;
  m_inDsType = inDsType;
  m_oidSet = oidSet;
}

void te::vp::MultipartToSinglepart::setOutput(te::da::DataSourcePtr outDsrc, std::string outDsName)
{
  m_outDsrc = outDsrc;
  m_outDsName = outDsName;
}

std::auto_ptr<te::da::DataSetType> te::vp::MultipartToSinglepart::getOutDst(bool onlySelected)
{
  std::auto_ptr<te::da::DataSetType> outDst(new te::da::DataSetType(m_outDsName));

  te::dt::Property* newId = new te::dt::SimpleProperty("newid", te::dt::INT32_TYPE, true);

  outDst->add(newId);

  for(std::size_t i = 0; i < m_inDsType->size(); ++i)
  {
    te::dt::Property* oldP = m_inDsType->getProperty(i);
    te::dt::Property* newP = oldP->clone();

    if(!onlySelected)
    {
      if(newP->getType() == te::dt::GEOMETRY_TYPE)
      {
        te::gm::GeometryProperty* geomP = dynamic_cast<te::gm::GeometryProperty*>(newP);

        geomP->setGeometryType(te::vp::GetSimpleType(geomP->getGeometryType()));
      }
    }

    outDst->add(newP);
  }

  return outDst;
}

te::gm::Geometry* te::vp::MultipartToSinglepart::single2multi(te::gm::Geometry* geom)
{
  switch(geom->getGeomTypeId())
  {
    case te::gm::LineStringType:
    {
      te::gm::MultiLineString* g = new te::gm::MultiLineString(0, te::gm::MultiLineStringType);
      g->add(geom);
      return g;
    }
    case te::gm::LineStringMType:
    {
      te::gm::MultiLineString* g = new te::gm::MultiLineString(0, te::gm::MultiLineStringMType);
      g->add(geom);
      return g;
    }
    case te::gm::LineStringZType:
    {
      te::gm::MultiLineString* g = new te::gm::MultiLineString(0, te::gm::MultiLineStringZType);
      g->add(geom);
      return g;
    }
    case te::gm::LineStringZMType:
    {
      te::gm::MultiLineString* g = new te::gm::MultiLineString(0, te::gm::MultiLineStringZMType);
      g->add(geom);
      return g;
    }
    case te::gm::PointType:
    {
      te::gm::MultiPoint* g = new te::gm::MultiPoint(0, te::gm::MultiPointType);
      g->add(geom);
      return g;
    }
    case te::gm::PointMType:
    {
      te::gm::MultiPoint* g = new te::gm::MultiPoint(0, te::gm::MultiPointMType);
      g->add(geom);
      return g;
    }
    case te::gm::PointZType:
    {
      te::gm::MultiPoint* g = new te::gm::MultiPoint(0, te::gm::MultiPointZType);
      g->add(geom);
      return g;
    }
    case te::gm::PointZMType:
    {
      te::gm::MultiPoint* g = new te::gm::MultiPoint(0, te::gm::MultiPointZMType);
      g->add(geom);
      return g;
    }
    case te::gm::PolygonType:
    {
      te::gm::MultiPolygon* g = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType);
      g->add(geom);
      return g;
    }
    case te::gm::PolygonMType:
    {
      te::gm::MultiPolygon* g = new te::gm::MultiPolygon(0, te::gm::MultiPolygonMType);
      g->add(geom);
      return g;
    }
    case te::gm::PolygonZType:
    {
      te::gm::MultiPolygon* g = new te::gm::MultiPolygon(0, te::gm::MultiPolygonZType);
      g->add(geom);
      return g;
    }
    case te::gm::PolygonZMType:
    {
      te::gm::MultiPolygon* g = new te::gm::MultiPolygon(0, te::gm::MultiPolygonZMType);
      g->add(geom);
      return g;
    }
    default:
      return 0;
  }
}