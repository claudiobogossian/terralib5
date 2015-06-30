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
  \file terralib/qt/plugins/datasource/gdal/Plugin.cpp

  \brief Plugin implementation for the GDAL data source widget.
*/

// TerraLib
#include "../../../../common/Config.h"
#include "../../../../common/Translator.h"
#include "../../../../common/Logger.h"

#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"

#include "../../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../widgets/datasource/core/DataSourceTypeManager.h"
#include "../../../widgets/layer/utils/DataSet2Layer.h"
#include "../../../widgets/Utils.h"

#include "../../../af/ApplicationController.h"
//#include "../../../af/Project.h"
#include "../../../af/Utils.h"
#include "../../../af/events/LayerEvents.h"
#include "../../../af/events/ApplicationEvents.h"

#include "GDALType.h"
#include "Plugin.h"
#include "../../../../cellspace/CellSpaceOperations.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt 
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>

std::list<te::da::DataSetTypePtr> GetDataSetsInfo(const te::da::DataSourceInfoPtr& info)
{
  std::list<te::da::DataSetTypePtr> res;

  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(info->getId(), info->getType(), info->getConnInfo());

  std::vector<std::string> dsets = ds->getDataSetNames();

  std::vector<std::string>::iterator it;

  for(it = dsets.begin(); it != dsets.end(); ++it)
    res.push_back(te::da::DataSetTypePtr(ds->getDataSetType(*it).release()));

  return res;
}

void GetLayers(const te::da::DataSourceInfoPtr& info, std::list<te::map::AbstractLayerPtr>& layers)
{
  std::list<te::map::AbstractLayerPtr> res;
  std::list<te::da::DataSetTypePtr> dss = GetDataSetsInfo(info);

  std::transform(dss.begin(), dss.end(), std::back_inserter(layers), te::qt::widgets::DataSet2Layer(info->getId()));
}


te::qt::plugins::gdal::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) :
QObject(),
 te::plugin::Plugin(pluginInfo),
 m_openFile(0)
{
  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);
}

te::qt::plugins::gdal::Plugin::~Plugin() 
{
}

void te::qt::plugins::gdal::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::widgets::DataSourceTypeManager::getInstance().add(new GDALType);

  TE_LOG_TRACE(TE_TR("TerraLib Qt GDAL widget startup!"));

  m_initialized = true;

  //Initializing action
  m_openFile = new QAction(QIcon::fromTheme("file-raster"), tr("Raster File..."), this);
  m_openFile->setObjectName("Project.Add Layer.Raster File");

  te::qt::af::evt::NewActionsAvailable e;
  e.m_category = "Datasource";
  e.m_actions << m_openFile;

  emit triggered(&e);
//  QAction* act = te::qt::af::AppCtrlSingleton::getInstance().findAction("Project.Add Layer.Tabular File");
//  QMenu* mnu = te::qt::af::AppCtrlSingleton::getInstance().findMenu("Project.Add Layer");

//  if(act != 0 && mnu != 0)
//  {
//    QWidget* parent = act->parentWidget();
//    m_openFile = new QAction(QIcon::fromTheme("file-raster"), tr("Raster File..."), parent);
//    m_openFile->setObjectName("Project.Add Layer.Raster File");
//    mnu->insertAction(act, m_openFile);
//    //mnu->addAction(m_openFile);

//    te::qt::af::AddActionToCustomToolbars(m_openFile);

    connect (m_openFile, SIGNAL(triggered()), SLOT(openFileDialog()));
//  }
}

void te::qt::plugins::gdal::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te::da::DataSourceInfoManager::getInstance().removeByType("GDAL");
  te::qt::widgets::DataSourceTypeManager::getInstance().remove("GDAL");

  TE_LOG_TRACE(TE_TR("TerraLib Qt GDAL widget shutdown!"));

  m_initialized = false;

//  delete m_openFile;
}

void te::qt::plugins::gdal::Plugin::openFileDialog()
{
//  te::qt::af::Project* proj = te::qt::af::AppCtrlSingleton::getInstance().getProject();

//  if(proj == 0)
//  {
//    QMessageBox::warning(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(), tr("Raster File"), tr("Error: there is no opened project!"));
//    return;
//  }

  QStringList fileNames = QFileDialog::getOpenFileNames(
    te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(),
    tr("Open Raster File"), te::qt::widgets::GetFilePathFromSettings("raster"), 
    te::qt::widgets::GetDiskRasterFileSelFilter());

  if(fileNames.isEmpty())
    return;

  QFileInfo info(fileNames.value(0));

  te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "raster");

  std::list<te::map::AbstractLayerPtr> layers;

  for(QStringList::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
  {
    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);

    ds->setAccessDriver("GDAL");

    std::map<std::string, std::string> dsinfo;
    dsinfo["URI"] = it->toLatin1().data();

    ds->setConnInfo(dsinfo);

    ds->setDescription("A single raster file");

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);

    ds->setId(id);

    boost::filesystem::path mpath(it->toStdString());

    std::string fileBaseName = mpath.stem().string();

    ds->setTitle(fileBaseName);

    ds->setType("GDAL");

    te::da::DataSourceInfoManager::getInstance().add(ds);

    GetLayers(ds, layers);
  }

  // If there is a parent folder layer that is selected, get it as the parent of the layer to be added;
  // otherwise, add the layer as a top level layer
  te::map::AbstractLayerPtr parentLayer(0);

//  std::list<te::map::AbstractLayerPtr> selectedLayers = te::qt::af::AppCtrlSingleton::getInstance().getProject()->getSelectedLayers();

//  if(selectedLayers.size() == 1 && selectedLayers.front()->getType() == "FOLDERLAYER")
//    parentLayer = selectedLayers.front();

  std::list<te::map::AbstractLayerPtr>::iterator it;
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    te::qt::af::evt::LayerAdded evt(*it, parentLayer);
    emit triggered(&evt);
  }
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::gdal::Plugin)

