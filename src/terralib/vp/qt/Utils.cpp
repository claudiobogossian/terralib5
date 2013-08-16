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
#include "../../dataaccess/dataset/DataSetPersistence.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/DataSetTypePersistence.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryCollection.h"
#include "../../geometry/GeometryProperty.h"
#include "../../memory/DataSet.h"
#include "Utils.h"

// Qt
#include <QtGui/QTreeView>

//STL
#include <vector>

// Boost
#include <boost/algorithm/string.hpp>

std::list<te::map::AbstractLayerPtr> te::vp::GetFilteredLayers(std::string text, std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr> filteredList;

  for(std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin(); it != layers.end(); ++it)
  {
    std::string layName = (*it)->getTitle().substr(0, text.size());
    
    if(boost::to_upper_copy(layName) == boost::to_upper_copy(text))
      filteredList.push_back(*it);
  }

  return filteredList;
}

te::gm::Geometry* te::vp::GetUnionGeometry(const std::vector<te::mem::DataSetItem*>& items, size_t geomIdx)
{
  te::gm::Geometry* resultGeometry = 0; 

  te::gm::Geometry* seedGeometry = items[0]->getGeometry(geomIdx);
  
  if(items.size() < 2)
  {
      resultGeometry = seedGeometry;
  }
  if(items.size() == 2)
  {
    te::gm::Geometry* teGeom = items[1]->getGeometry(geomIdx);

    if(items[1]->getGeometry(geomIdx)->isValid())
      resultGeometry = seedGeometry->Union(teGeom);
    else
      resultGeometry = seedGeometry;
  }
  if(items.size() > 2)
  {
    te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, seedGeometry->getSRID());

    for(std::size_t i = 1; i < items.size(); ++i)
    {
      if(items[i]->getGeometry(geomIdx)->isValid())
        teGeomColl->add(items[i]->getGeometry(geomIdx));
    }

    resultGeometry = seedGeometry->Union(teGeomColl);
  }

  return resultGeometry;
}

std::string te::vp::GetSimpleTableName(std::string fullName)
{
  std::string key (".");
  std::size_t found = fullName.rfind(key);
  
  if(found >= std::string::npos)
    return fullName;

  std::string tableName = fullName.substr(found + 1);

  return tableName;
}

te::gm::GeometryProperty* te::vp::SetOutputGeometryType(const te::gm::GeometryProperty* firstGeom, const te::gm::GeometryProperty* secondGeom)
{
  te::gm::GeometryProperty* fiGeomProp = (te::gm::GeometryProperty*)firstGeom->clone();

  if( (firstGeom->getGeometryType() == te::gm::PolygonType && secondGeom->getGeometryType() == te::gm::PolygonType) ||
      (firstGeom->getGeometryType() == te::gm::MultiPolygonType && secondGeom->getGeometryType() == te::gm::MultiPolygonType) ||
      (firstGeom->getGeometryType() == te::gm::PolygonType && secondGeom->getGeometryType() == te::gm::MultiPolygonType) ||
      (firstGeom->getGeometryType() == te::gm::PolygonType && secondGeom->getGeometryType() == te::gm::PolygonType))
  {
    fiGeomProp->setName("geom");
    fiGeomProp->setGeometryType(te::gm::PolygonType);
  }
  else if((firstGeom->getGeometryType() == te::gm::LineStringType && secondGeom->getGeometryType() == te::gm::LineStringType) ||
          (firstGeom->getGeometryType() == te::gm::LineStringType && secondGeom->getGeometryType() == te::gm::PolygonType) ||
          (firstGeom->getGeometryType() == te::gm::LineStringType && secondGeom->getGeometryType() == te::gm::MultiPolygonType) ||
          (firstGeom->getGeometryType() == te::gm::PolygonType && secondGeom->getGeometryType() == te::gm::LineStringType) ||
          (firstGeom->getGeometryType() == te::gm::MultiPolygonType && secondGeom->getGeometryType() == te::gm::LineStringType))
  {
    fiGeomProp->setName("geom");
    fiGeomProp->setGeometryType(te::gm::LineStringType);
  }
  else
  {
    fiGeomProp->setName("geom");
    fiGeomProp->setGeometryType(te::gm::PointType);
  }

  return fiGeomProp;
}

void te::vp::Persistence( te::da::DataSetType* dataSetType,
                          te::mem::DataSet* dataSet,
                          const te::da::DataSourceInfoPtr& dsInfo,
                          std::map<std::string, std::string> options)
{
  std::pair<te::da::DataSetType*, te::mem::DataSet*> pair;
  pair.first = dataSetType;
  pair.second = dataSet;

  te::da::DataSourcePtr dataSource = te::da::DataSourceManager::getInstance().get(dsInfo->getId(), dsInfo->getType(), dsInfo->getConnInfo());
  std::auto_ptr<te::da::DataSourceTransactor> t(dataSource->getTransactor());
  pair.second->moveFirst();
  te::da::Create(t.get(), pair.first, pair.second, options);
}