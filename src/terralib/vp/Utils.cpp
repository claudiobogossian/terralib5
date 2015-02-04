/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/qt/Utils.cpp
   
  \brief Utility functions for Vector Processing.
*/

// TerraLib
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/DataSetTypeCapabilities.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/utils/Utils.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Point.h"
#include "../memory/DataSet.h"
#include "Utils.h"

//STL
#include <vector>
#include <memory>

// Boost
#include <boost/algorithm/string.hpp>

te::gm::Geometry* te::vp::GetGeometryUnion(const std::vector<te::mem::DataSetItem*>& items, size_t geomIdx, te::gm::GeomType outGeoType)
{
  te::gm::Geometry* resultGeometry(0); 

  std::auto_ptr<te::gm::Geometry> seedGeometry = items[0]->getGeometry(geomIdx);

  if(items.size() < 2)
    resultGeometry = seedGeometry.release();

  if(items.size() == 2)
  {
    std::auto_ptr<te::gm::Geometry> teGeom = items[1]->getGeometry(geomIdx);

    if(teGeom->isValid())
      resultGeometry = seedGeometry->Union(teGeom.release());
    else
      resultGeometry = seedGeometry.release();
  }
  if(items.size() > 2)
  {
    te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, seedGeometry->getSRID());

    for(std::size_t i = 1; i < items.size(); ++i)
    {
      std::auto_ptr<te::gm::Geometry> currentGeom = items[i]->getGeometry(geomIdx);

      if(currentGeom->isValid())
        teGeomColl->add(currentGeom.release());
    }

    resultGeometry = seedGeometry->Union(teGeomColl);

  }

  if (resultGeometry->getGeomTypeId() != outGeoType)
  {
    if(resultGeometry->getGeomTypeId() == te::gm::GeometryCollectionType)
    {
      te::gm::GeometryCollection* gc = new te::gm::GeometryCollection(0, outGeoType, resultGeometry->getSRID());
      std::vector<te::gm::Geometry*> geomVec = ((te::gm::GeometryCollection*)resultGeometry)->getGeometries();
      for(std::size_t i = 0; i < geomVec.size(); ++i)
      {
        te::gm::GeometryCollection* gcIn = dynamic_cast<te::gm::GeometryCollection*>(geomVec[i]);
        if(gcIn == 0)
          gc->add(geomVec[i]);
        else
          SplitGeometryCollection(gcIn, gc);
      }
      return gc;
    }
    else
    {
      te::gm::GeometryCollection* gc = new te::gm::GeometryCollection(1, outGeoType, resultGeometry->getSRID());
      gc->setGeometryN(0,resultGeometry);
      return gc;
    }
  }
  else
    return resultGeometry;
}

te::gm::Geometry* te::vp::GetGeometryUnion(const std::vector<te::mem::DataSetItem*>& items, size_t geomIdx)
{
  te::gm::Geometry* resultGeometry(0); 

  std::auto_ptr<te::gm::Geometry> seedGeometry = items[0]->getGeometry(geomIdx);

  if(items.size() < 2)
    resultGeometry = seedGeometry.release();

  if(items.size() == 2)
  {
    std::auto_ptr<te::gm::Geometry> teGeom = items[1]->getGeometry(geomIdx);

    if(teGeom->isValid())
      resultGeometry = seedGeometry->Union(teGeom.release());
    else
      resultGeometry = seedGeometry.release();
  }
  if(items.size() > 2)
  {
    te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, seedGeometry->getSRID());

    for(std::size_t i = 1; i < items.size(); ++i)
    {
      std::auto_ptr<te::gm::Geometry> currentGeom = items[i]->getGeometry(geomIdx);

      if(currentGeom->isValid())
        teGeomColl->add(currentGeom.release());
    }

    resultGeometry = seedGeometry->Union(teGeomColl);

  }
  return resultGeometry;
}

void te::vp::SplitGeometryCollection(te::gm::GeometryCollection* gcIn, te::gm::GeometryCollection* gcOut)
{
  std::vector<te::gm::Geometry*> geomVec = ((te::gm::GeometryCollection*)gcIn)->getGeometries();
  for(std::size_t i = 0; i < geomVec.size(); ++i)
  {
    te::gm::GeometryCollection* gc = dynamic_cast<te::gm::GeometryCollection*>(geomVec[i]);
    if(gc == 0)
      gcOut->add(geomVec[i]);
    else
      SplitGeometryCollection(gc, gcOut);
  }
}

std::string te::vp::GetSimpleTableName(std::string fullName)
{
  std::size_t found = fullName.rfind(".");
  
  if(found >= std::string::npos)
    return fullName;

  return fullName.substr(found + 1);
}

te::gm::GeomType te::vp::GeomOpResultType(te::gm::GeomType firstGeom, te::gm::GeomType secondGeom)
{
  if( (firstGeom == te::gm::PolygonType && secondGeom == te::gm::PolygonType) ||
      (firstGeom == te::gm::MultiPolygonType &&  secondGeom == te::gm::MultiPolygonType) ||
      (firstGeom == te::gm::PolygonType && secondGeom == te::gm::MultiPolygonType) ||
      (firstGeom == te::gm::MultiPolygonType && secondGeom == te::gm::PolygonType))
    return te::gm::MultiPolygonType;
  
  if((firstGeom == te::gm::PolygonType && secondGeom == te::gm::LineStringType)||
          (firstGeom == te::gm::PolygonType && secondGeom == te::gm::MultiLineStringType)||
          (firstGeom == te::gm::MultiPolygonType && secondGeom == te::gm::LineStringType)||
          (firstGeom == te::gm::MultiPolygonType && secondGeom == te::gm::MultiLineStringType)||

          (firstGeom == te::gm::LineStringType && secondGeom == te::gm::PolygonType)||
          (firstGeom == te::gm::LineStringType && secondGeom == te::gm::MultiPolygonType)||
          (firstGeom == te::gm::MultiLineStringType && secondGeom == te::gm::PolygonType)||
          (firstGeom == te::gm::MultiLineStringType && secondGeom == te::gm::MultiPolygonType)||

          (firstGeom == te::gm::LineStringType && secondGeom == te::gm::LineStringType) ||
          (firstGeom == te::gm::MultiLineStringType && secondGeom == te::gm::MultiLineStringType) ||
          (firstGeom == te::gm::LineStringType && secondGeom == te::gm::MultiLineStringType) ||
          (firstGeom == te::gm::MultiLineStringType && secondGeom == te::gm::LineStringType))
    return te::gm::MultiLineStringType;

  return te::gm::MultiPointType;
}

te::gm::GeomType te::vp::GeomOpResultType(te::gm::GeomType firstGeom)
{
  if (firstGeom == te::gm::PolygonType)
    return te::gm::MultiPolygonType;
  
  if (firstGeom == te::gm::LineStringType)
    return te::gm::MultiLineStringType;
  
  if (firstGeom == te::gm::PointType)
    return te::gm::MultiPointType;
  
  return firstGeom;
}

void te::vp::Save(te::da::DataSource* source, te::da::DataSet* result, te::da::DataSetType* outDsType)
{
  // do any adaptation necessary to persist the output dataset
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outDsType, source->getCapabilities());
  te::da::DataSetType* dsTypeResult = converter->getResult();

  std::auto_ptr<te::da::DataSourceTransactor> t = source->getTransactor();

  std::map<std::string, std::string> options;

  try
  {
    if(source->getType() == "OGR")
    {
      // create the dataset
      source->createDataSet(dsTypeResult, options);
  
      // copy from memory to output datasource
      result->moveBeforeFirst();
      std::string name = dsTypeResult->getName();
      source->add(dsTypeResult->getName(),result, options);
  
      // create the primary key if it is possible
      if (source->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
      {
        std::string pk_name = dsTypeResult->getName() + "_pkey";
        te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
        pk->add(dsTypeResult->getProperty(0));
        source->addPrimaryKey(outDsType->getName(), pk);
      }
    }
    else
    {
      t->begin();

      // create the dataset
      t->createDataSet(dsTypeResult, options);
  
      // copy from memory to output datasource
      result->moveBeforeFirst();
      std::string name = dsTypeResult->getName();
      t->add(dsTypeResult->getName(),result, options);
  
      // create the primary key if it is possible
      if (source->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
      {
        std::string pk_name = dsTypeResult->getName() + "_pkey";
        te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
        pk->add(dsTypeResult->getProperty(0));
        t->addPrimaryKey(outDsType->getName(), pk);
      }

      t->commit();
    }

  }
  catch(std::exception& e)
  {
    if(source->getType() != "OGR")
    {
      t->rollBack();
    }
    else
    {
      //TODO: Deletar possivel shp gerado
    }
    throw e;
  }
  catch(te::common::Exception& e)
  {
    if(source->getType() != "OGR")
    {
      t->rollBack();
    }
    else
    {
      //TODO: Deletar possivel shp gerado
    }
    throw e;
  }
}
