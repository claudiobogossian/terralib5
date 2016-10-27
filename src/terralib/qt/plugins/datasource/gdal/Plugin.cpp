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
#include "../../../../core/translator/Translator.h"
#include "../../../../core/logger/Logger.h"

#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"

#include "../../../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../widgets/datasource/core/DataSourceTypeManager.h"
#include "../../../widgets/layer/utils/DataSet2Layer.h"
#include "../../../widgets/raster/RasterInfoDialog.h"
#include "../../../widgets/raster/RasterInfoWidget.h"
#include "../../../widgets/Utils.h"

#include "../../../af/ApplicationController.h"
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
#include <QToolBar>

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

te::map::AbstractLayerPtr GetLayer(const te::da::DataSourceInfoPtr& info, std::string fileName)
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

void GetLayers(const te::da::DataSourceInfoPtr& info, std::list<te::map::AbstractLayerPtr>& layers)
{
  std::list<te::map::AbstractLayerPtr> res;
  std::list<te::da::DataSetTypePtr> dss = GetDataSetsInfo(info);

  std::transform(dss.begin(), dss.end(), std::back_inserter(layers), te::qt::widgets::DataSet2Layer(info->getId()));
}


te::qt::plugins::gdal::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) :
QObject(),
 te::plugin::Plugin(pluginInfo),
 m_openFile(0), m_openMultipleFiles(0)
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
  m_openMultipleFiles = new QAction(QIcon::fromTheme("file-raster"), tr("Raster File..."), this);
  m_openMultipleFiles->setToolTip(tr("Add new raster file as a layer."));
  m_openMultipleFiles->setObjectName("Project.Add Layer.Raster File");
  m_openFile = new QAction(QIcon::fromTheme("file-raster"), tr("RAW Raster File..."), this);
  m_openFile->setObjectName("Project.Add Layer.RAW Raster File");


  te::qt::af::evt::NewActionsAvailable e;
  e.m_category = "Dataaccess";
  e.m_actions << m_openMultipleFiles;
  e.m_actions << m_openFile;
  

  emit triggered(&e);

  connect (m_openFile, SIGNAL(triggered()), SLOT(openFileDialog()));
  connect (m_openMultipleFiles, SIGNAL(triggered()), SLOT(openMultipleFilesDialog()));

  //register actions into application tool bar
  QToolBar* toolBar = te::qt::af::AppCtrlSingleton::getInstance().getToolBar("File Tool Bar");

  if (toolBar)
    toolBar->addAction(m_openMultipleFiles);
}

void te::qt::plugins::gdal::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te::da::DataSourceInfoManager::getInstance().removeByType("GDAL");
  te::qt::widgets::DataSourceTypeManager::getInstance().remove("GDAL");

  TE_LOG_TRACE(TE_TR("TerraLib Qt GDAL widget shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::gdal::Plugin::openFileDialog()
{
  std::auto_ptr< te::qt::widgets::RasterInfoDialog > diagPtr( 
    new te::qt::widgets::RasterInfoDialog( false,
    te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(), 0 ) );
  diagPtr->exec();
  if( diagPtr->getWidget()->getShortName().empty() )
  {
    return;
  }
  
  QFileInfo info( diagPtr->getWidget()->getFullName().c_str() );
  QStringList fileNames;
  fileNames << diagPtr->getWidget()->getFullName().c_str();

  te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "raster");

  std::list<te::map::AbstractLayerPtr> layers;

  for(QStringList::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
  {
    te::map::AbstractLayerPtr layer;
    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);

    ds->setAccessDriver("GDAL");

    std::string fpath = it->toUtf8().data();
    ds->setConnInfo("file://" + fpath);
    ds->setDescription("A single raster file");

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);

    ds->setId(id);

    boost::filesystem::path mpath(it->toUtf8().data());

    std::string fileBaseName = mpath.stem().string();

    ds->setTitle(fileBaseName);

    ds->setType("GDAL");

    if(!te::da::DataSourceInfoManager::getInstance().add(ds))
      ds = te::da::DataSourceInfoManager::getInstance().getByConnInfo(ds->getConnInfoAsString());

    layer = GetLayer(ds, mpath.leaf().string());
    if (layer.get())
      layers.push_back(layer);
  }

  // If there is a parent folder layer that is selected, get it as the parent of the layer to be added;
  // otherwise, add the layer as a top level layer
  te::map::AbstractLayerPtr parentLayer(0);

  std::list<te::map::AbstractLayerPtr>::iterator it;
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    te::qt::af::evt::LayerAdded evt(*it, parentLayer);
    emit triggered(&evt);
  }
}

void te::qt::plugins::gdal::Plugin::openMultipleFilesDialog()
{
  QStringList fileNames = QFileDialog::getOpenFileNames(
    te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(),
    tr("Open Multiple Raster Files"), te::qt::widgets::GetFilePathFromSettings("raster"), 
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

    std::string fpath = it->toUtf8().data();
    ds->setConnInfo("file://" + fpath);

    ds->setDescription("A single raster file");

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);

    ds->setId(id);

    boost::filesystem::path mpath(it->toUtf8().data());

    std::string fileBaseName = mpath.stem().string();

    ds->setTitle(fileBaseName);

    ds->setType("GDAL");

    if(!te::da::DataSourceInfoManager::getInstance().add(ds))
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
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    te::qt::af::evt::LayerAdded evt(*it, parentLayer);
    emit triggered(&evt);
  }
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::gdal::Plugin)

