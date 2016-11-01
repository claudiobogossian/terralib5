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
  \file terralib/qt/plugins/datasource/ogr/Utils.cpp
   
  \brief Utility functions for the OGR data source widget plugin.
*/

// TerraLib
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
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

std::list<te::da::DataSetTypePtr> GetDataSetsInfo(const te::da::DataSourceInfoPtr& info)
{
  std::list<te::da::DataSetTypePtr> res;

  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(info->getId(), info->getType(), info->getConnInfo());
  if (!ds->isOpened())
    ds->open();

  std::vector<std::string> dsets = ds->getDataSetNames();

  std::vector<std::string>::iterator it;

  for (it = dsets.begin(); it != dsets.end(); ++it)
    res.push_back(te::da::DataSetTypePtr(ds->getDataSetType(*it).release()));

  return res;
}

void GetLayers(const te::da::DataSourceInfoPtr& info, std::list<te::map::AbstractLayerPtr>& layers)
{
  std::list<te::map::AbstractLayerPtr> res;
  std::list<te::da::DataSetTypePtr> dss = GetDataSetsInfo(info);

  std::transform(dss.begin(), dss.end(), std::back_inserter(layers), te::qt::widgets::DataSet2Layer(info->getId()));
}

bool te::qt::plugins::ogr::IsShapeFile(const QString& path)
{
  QFileInfo fileInfo(path);

  if(!fileInfo.isFile())
    return false;

  if(fileInfo.suffix().toLower() == "shp")
    return true;

  return false;
}

bool te::qt::plugins::ogr::HasShapeFileSpatialIndex(const QString& path)
{
  assert(IsShapeFile(path));

  QFileInfo fileInfo(path);

  // Gets the ShapeFile name
  QString fileName = fileInfo.fileName();

  // Builds the ShapeFile directory
  QDir dir(fileInfo.absolutePath());

  // Looking for .qix file
  QString qixFile = fileName;
  qixFile.replace(".shp", ".qix");
  if(dir.exists(qixFile))
    return true;

  // Looking for .sbn file
  QString sbnFile = fileName;
  sbnFile.replace(".shp", ".sbn");
  if(dir.exists(sbnFile))
    return true;

  return false;
}

void te::qt::plugins::ogr::CreateLayers(QStringList fileNames)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  // The selected shapefiles without spatial index
  std::map<std::string, std::string> shpWithoutSpatialIndex;

  std::list<te::map::AbstractLayerPtr> layers;

  bool hasErrors = false;
  QString errorMsg = QObject::tr("Error occurred trying to create layer(s):\n\n");

  for (QStringList::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
  {
    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);

    ds->setAccessDriver("OGR");

    std::string fpath = it->toUtf8().data();

    ds->setConnInfo("file://" + fpath);

    std::string desc("A single vector file: ");
    desc += fpath;
    ds->setDescription(desc);

    boost::filesystem::path mpath(fpath);

    std::string fileBaseName = mpath.leaf().string();

    ds->setTitle(fileBaseName);

    ds->setType("OGR");

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);

    ds->setId(id);
    if (!te::da::DataSourceInfoManager::getInstance().add(ds))
      ds = te::da::DataSourceInfoManager::getInstance().getByConnInfo(ds->getConnInfoAsString());

    id = ds->getId();

    if (IsShapeFile(*it) && !HasShapeFileSpatialIndex(*it))
    {
      QString datasetName(fileBaseName.c_str());
      datasetName.remove(".shp", Qt::CaseInsensitive);
      shpWithoutSpatialIndex[id] = datasetName.toUtf8().data();
    }

    try
    {
      GetLayers(ds, layers);
    }
    catch (const te::common::Exception& e)
    {
      hasErrors = true;
      errorMsg.append(" - ");
      errorMsg.append(fileBaseName.c_str());
      errorMsg.append("\n");

    }
    catch (std::exception& e)
    {
      hasErrors = true;
      errorMsg.append(" - ");
      errorMsg.append(fileBaseName.c_str());
      errorMsg.append("\n");
    }
  }

  QApplication::restoreOverrideCursor();

  if (hasErrors)
  {
    QMessageBox::warning(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(), QObject::tr("Add Layer"), errorMsg);
  }

  if (!shpWithoutSpatialIndex.empty())
  {
    if (QMessageBox::question(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(),
      QObject::tr("Spatial Index"), QObject::tr("Do you want create spatial index to the selected ESRI ShapeFiles?"),
      QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      std::map<std::string, std::string>::iterator it;
      for (it = shpWithoutSpatialIndex.begin(); it != shpWithoutSpatialIndex.end(); ++it)
      {
        te::da::DataSourcePtr driver;

        try
        {
          driver = te::da::GetDataSource(it->first, true);

          std::string command = "CREATE SPATIAL INDEX ON " + it->second;

          driver->execute(command);

          QMessageBox::information(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(), QObject::tr("Spatial Index"), "Spatial index created with successfully!");

          QApplication::restoreOverrideCursor();
        }
        catch (const te::common::Exception& e)
        {
          QMessageBox::information(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(), QObject::tr("Spatial Index"), "Error creating spatial index.");

          QApplication::restoreOverrideCursor();

        }
        catch (std::exception& e)
        {
          QMessageBox::information(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(), QObject::tr("Spatial Index"), "Error creating spatial index.");

          QApplication::restoreOverrideCursor();
        }
      }
    }
  }

  // If there is only a parent folder layer that is selected, get it as the parent of the layer to be added;
  // otherwise, add the layer as a top level layer
  te::map::AbstractLayerPtr parentLayer(0);

  //  std::list<te::map::AbstractLayerPtr> selectedLayers = te::qt::af::AppCtrlSingleton::getInstance().getProject()->getSelectedLayers();

  //  if(selectedLayers.size() == 1 && selectedLayers.front()->getType() == "FOLDERLAYER")
  //    parentLayer = selectedLayers.front();

  std::list<te::map::AbstractLayerPtr>::iterator it;
  for (it = layers.begin(); it != layers.end(); ++it)
  {
    if ((*it)->getSRID() != TE_UNKNOWN_SRS)
    {
      if (!te::srs::SpatialReferenceSystemManager::getInstance().recognizes((*it)->getSRID()))
      {
        QString msgErr(QObject::tr("Layer %1 has SRID %2 that is not recognized by TerraLib. Setting it to unknown."));
        msgErr = msgErr.arg((*it)->getTitle().c_str());
        msgErr = msgErr.arg((*it)->getSRID());

        QMessageBox::warning(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(),
          QObject::tr("Layer SRS check"), msgErr);
        (*it)->setSRID(TE_UNKNOWN_SRS);
      }
    }

    te::qt::af::evt::LayerAdded evt(*it, parentLayer);
    te::qt::af::AppCtrlSingleton::getInstance().triggered(&evt);
  }
}
