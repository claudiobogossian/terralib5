/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/rp/Utils.h

  \brief Utility functions for the qt widgets rp Module.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../maptools/Utils.h"
#include "../../../raster/Grid.h"
#include "../layer/utils/DataSet2Layer.h"
#include "Utils.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::map::AbstractLayerPtr te::qt::widgets::createLayer(const std::string& driverName,
                                                       const te::core::URI& connInfo)
{
  te::map::AbstractLayerPtr layer;

  static boost::uuids::basic_random_generator<boost::mt19937> gen;

  boost::uuids::uuid valU = gen();
  std::string id = boost::uuids::to_string(valU);

  std::unique_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make(driverName, connInfo));
  ds->open();

  std::vector<std::string> dsNames = ds->getDataSetNames();
  assert(!dsNames.empty());

  //add ds info
  te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
  dsInfoPtr->setConnInfo(connInfo);
  dsInfoPtr->setId(id);
  dsInfoPtr->setTitle(dsNames[0]);
  dsInfoPtr->setAccessDriver(driverName);
  dsInfoPtr->setType(driverName);

  te::da::DataSourcePtr dsPtr(ds.release());

  if (!te::da::DataSourceInfoManager::getInstance().add(dsInfoPtr))
  {
    //Datasource already exist - use its id
    dsInfoPtr = te::da::DataSourceInfoManager::getInstance().getByConnInfo(dsInfoPtr->getConnInfoAsString());
    dsPtr->setId(dsInfoPtr->getId());
  }
  else
  {
    dsPtr->setId(id);
    te::da::DataSourceManager::getInstance().insert(dsPtr);
  }

  //create layer
  te::da::DataSetTypePtr dsType(dsPtr->getDataSetType(dsNames[0]).release());

  te::qt::widgets::DataSet2Layer ds2l(dsPtr->getId());

  layer = ds2l(dsType);

  return layer;
}

te::map::AbstractLayerPtr te::qt::widgets::createLayer(const std::string& driverName,
                                                       const std::string& connInfo)
{
  const te::core::URI uri(connInfo);
  return createLayer(driverName, uri);
}

te::map::AbstractLayerPtr te::qt::widgets::createLayer(const std::string& driverName,
                                                       const std::map<std::string, std::string>& connInfo)
{
  te::map::AbstractLayerPtr layer;

  static boost::uuids::basic_random_generator<boost::mt19937> gen;

  boost::uuids::uuid valU = gen();
  std::string id = boost::uuids::to_string(valU);

  //Adjusting the URI based on the connection info parameters
  std::string connStr("File://");

  std::map<std::string, std::string>::const_iterator it = connInfo.begin();
  std::map<std::string, std::string>::const_iterator itend = connInfo.end();
  
  while (it != itend)
  {
    if (it->first == "URI" || it->first == "SOURCE")
    {
      connStr += it->second + "?";
      ++it;
      continue;
    }

    connStr += it->first + "=" + it->second + "&";
    ++it;
  }

  std::unique_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make(driverName, connStr));
  ds->open();

  std::vector<std::string> dsNames = ds->getDataSetNames();
  assert(!dsNames.empty());

  //add ds info
  te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
  dsInfoPtr->setConnInfo(connStr);
  dsInfoPtr->setId(id);
  dsInfoPtr->setTitle(dsNames[0]);
  dsInfoPtr->setAccessDriver(driverName);
  dsInfoPtr->setType(driverName);

  te::da::DataSourcePtr dsPtr(ds.release());

  if (!te::da::DataSourceInfoManager::getInstance().add(dsInfoPtr))
  {
    //Datasource already exist - use its id
    dsInfoPtr = te::da::DataSourceInfoManager::getInstance().getByConnInfo(dsInfoPtr->getConnInfoAsString());
    dsPtr->setId(dsInfoPtr->getId());
  }
  else
  {
    dsPtr->setId(id);
    te::da::DataSourceManager::getInstance().insert(dsPtr);
  }

  //create layer
  te::da::DataSetTypePtr dsType(dsPtr->getDataSetType(dsNames[0]).release());

  te::qt::widgets::DataSet2Layer ds2l(dsPtr->getId());

  layer = ds2l(dsType);

  return layer;
}

te::rst::Raster* te::qt::widgets::getRasterVisibleArea(te::map::AbstractLayerPtr layer, te::gm::Envelope boxDisplay, int displaySRID)
{
  te::rst::Raster* raster = 0;

  //get box info
  te::gm::Envelope reprojectedBBOX(boxDisplay);
  reprojectedBBOX.transform(displaySRID, layer->getSRID());
  te::gm::Envelope ibbox = reprojectedBBOX.intersection(layer->getExtent());

  //get raster
  std::auto_ptr<te::da::DataSet> ds(layer->getData());
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  te::gm::Coord2D startGrid = inputRst->getGrid()->geoToGrid(ibbox.getLowerLeftX(), ibbox.getLowerLeftY());
  te::gm::Coord2D endGrid = inputRst->getGrid()->geoToGrid(ibbox.getUpperRightX(), ibbox.getUpperRightY());

  int w = (int)endGrid.x - (int)startGrid.x;
  int h = (int)startGrid.y - (int)endGrid.y;

  raster = te::map::GetExtentRaster(inputRst.get(), w, h, reprojectedBBOX, displaySRID, ibbox, layer->getSRID());

  return raster;
}