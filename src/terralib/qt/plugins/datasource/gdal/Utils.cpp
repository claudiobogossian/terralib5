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
\file terralib/qt/plugins/datasource/gdal/Utils.cpp

\brief Utility functions for the GDAL data source widget plugin.
*/

// TerraLib
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../srs/SpatialReferenceSystemManager.h"
#include "../../../widgets/layer/utils/DataSet2Layer.h"
#include "../../../af/ApplicationController.h"
#include "../../../af/events/LayerEvents.h"
#include "Utils.h"

// Qt
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

// STL
#include <cassert>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

std::list<te::da::DataSetTypePtr> te::qt::plugins::gdal::GetDataSetsInfo(const te::da::DataSourceInfoPtr& info)
{
  std::list<te::da::DataSetTypePtr> res;

  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(info->getId(), info->getType(), info->getConnInfo());

  std::vector<std::string> dsets = ds->getDataSetNames();

  std::vector<std::string>::iterator it;

  for (it = dsets.begin(); it != dsets.end(); ++it)
    res.push_back(te::da::DataSetTypePtr(ds->getDataSetType(*it).release()));

  return res;
}

te::map::AbstractLayerPtr te::qt::plugins::gdal::GetLayer(const te::da::DataSourceInfoPtr& info, std::string fileName)
{
  te::map::AbstractLayerPtr res;
  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(info->getId(), info->getType(), info->getConnInfo());

  te::da::DataSetTypePtr dss = ds->getDataSetType(fileName);

  if (dss.get())
  {
    te::qt::widgets::DataSet2Layer layer(info->getId());
    res = layer(dss);
  }

  return res;
}

void te::qt::plugins::gdal::GetLayers(const te::da::DataSourceInfoPtr& info, std::list<te::map::AbstractLayerPtr>& layers)
{
  std::list<te::map::AbstractLayerPtr> res;
  std::list<te::da::DataSetTypePtr> dss = GetDataSetsInfo(info);

  std::transform(dss.begin(), dss.end(), std::back_inserter(layers), te::qt::widgets::DataSet2Layer(info->getId()));
}

void te::qt::plugins::gdal::CreateLayers(QStringList fileNames)
{
  std::list<te::map::AbstractLayerPtr> layers;

  for (QStringList::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
  {
    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);

    ds->setAccessDriver("GDAL");

    std::map<std::string, std::string> dsinfo;
    dsinfo["URI"] = it->toUtf8().data();

    ds->setConnInfo(dsinfo);

    ds->setDescription("A single raster file");

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);

    ds->setId(id);

    boost::filesystem::path mpath(it->toUtf8().data());

    std::string fileBaseName = mpath.stem().string();

    ds->setTitle(fileBaseName);

    ds->setType("GDAL");

    if (!te::da::DataSourceInfoManager::getInstance().add(ds))
      ds = te::da::DataSourceInfoManager::getInstance().getByConnInfo(ds->getConnInfoAsString());

    GetLayers(ds, layers);
  }

  // If there is a parent folder layer that is selected, get it as the parent of the layer to be added;
  // otherwise, add the layer as a top level layer
  te::map::AbstractLayerPtr parentLayer(0);

  //   std::list<te::map::AbstractLayerPtr> selectedLayers = te::qt::af::AppCtrlSingleton::getInstance().getProject()->getSelectedLayers();
  // 
  //   if(selectedLayers.size() == 1 && selectedLayers.front()->getType() == "FOLDERLAYER")
  //     parentLayer = selectedLayers.front();

  std::list<te::map::AbstractLayerPtr>::iterator it;
  for (it = layers.begin(); it != layers.end(); ++it)
  {
    te::qt::af::evt::LayerAdded evt(*it, parentLayer);
    te::qt::af::AppCtrlSingleton::getInstance().triggered(&evt);
  }
}

