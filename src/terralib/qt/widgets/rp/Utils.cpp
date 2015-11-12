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
#include "../layer/utils/DataSet2Layer.h"
#include "Utils.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


te::map::AbstractLayerPtr te::qt::widgets::createLayer(const std::string& driverName, 
                                                       const std::map<std::string, std::string>& connInfo)
{
  te::map::AbstractLayerPtr layer;

  static boost::uuids::basic_random_generator<boost::mt19937> gen;

  boost::uuids::uuid valU = gen();
  std::string id = boost::uuids::to_string(valU);

  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make(driverName));
  ds->setConnectionInfo(connInfo);
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

/*
  te::map::RasterLayer* rLayer = new te::map::RasterLayer(layerId, layerName);

  rLayer->setRasterInfo(connInfo);
  rLayer->setVisibility(te::map::VISIBLE);
  rLayer->setRendererType("RASTER_LAYER_RENDERER");

  layer.reset(rLayer);

  std::auto_ptr<te::rst::Raster> rst(rLayer->getRaster());

  te::se::Style* style = layer->getStyle();
  if(style == 0)
  {
    // Try create an appropriate style. i.e. a CoverageStyle
    style = te::se::CreateCoverageStyle(rst->getNumberOfBands());

    if(style != 0)
      layer->setStyle(style);
  }
*/