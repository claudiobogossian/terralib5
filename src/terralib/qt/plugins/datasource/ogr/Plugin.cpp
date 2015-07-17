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
  \file terralib/qt/plugins/datasource/ogr/Plugin.cpp

  \brief Plugin implementation for the OGR data source widget.
*/

// TerraLib
#include "../../../../common/Config.h"
#include "../../../../common/Logger.h"
#include "../../../../common/Translator.h"
#include "../../../../srs/Config.h"
#include "../../../../srs/SpatialReferenceSystemManager.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../widgets/layer/utils/DataSet2Layer.h"
#include "../../../widgets/datasource/core/DataSourceTypeManager.h"
#include "../../../widgets/Utils.h"
#include "../../../af/ApplicationController.h"
#include "../../../af/Project.h"
#include "../../../af/Utils.h"
#include "../../../af/events/LayerEvents.h"
#include "OGRType.h"
#include "Plugin.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt 
#include <QFileInfo>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>

std::list<te::da::DataSetTypePtr> GetDataSetsInfo(const te::da::DataSourceInfoPtr& info)
{
  std::list<te::da::DataSetTypePtr> res;

  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(info->getId(), info->getType(), info->getConnInfo());
  if (!ds->isOpened())
    ds->open();

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

QString GetFileExtensionName(const std::string& type)
{
  if(type.compare("ESRI Shapefile") == 0)
    return QObject::tr("ESRI Shapefile (*.shp *.SHP)");
  else if(type.compare("MapInfo File") == 0)
    return QObject::tr("MapInfo File (*.mif *.MIF)");
  else if(type.compare("DGN") == 0)
    return QObject::tr("DGN File (*.dgn *.DGN)");
//  else if(type.compare("CSV") == 0)
//    return QObject::tr("CSV File (*.csv *.CSV)");
  else if(type.compare("GML") == 0)
    return QObject::tr("GML File (*.gml *.GML)");
  else if(type.compare("KML") == 0)
    return QObject::tr("KML File (*.kml *.KML)");
  else if(type.compare("GeoJSON") == 0)
    return QObject::tr("GeoJSON File (*.geojson *.GEOJSON)");
  else if(type.compare("DXF") == 0)
    return QObject::tr("DXF File (*.dxf *.DXF)");

  return "";
}

QString GetSupportedFiles()
{
  QString res;
  OGRSFDriverRegistrar* dMgr = OGRSFDriverRegistrar::GetRegistrar();

  int count = dMgr->GetDriverCount();

  for(int i=0; i<count; i++)
  {
    std::string dName = dMgr->GetDriver(i)->GetName();

    QString drv = GetFileExtensionName(dName);

    if(!drv.isEmpty())
      res += drv + ";;";
  }

  return res;
}

te::qt::plugins::ogr::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) : 
QObject(),
te::plugin::Plugin(pluginInfo),
m_showWindow(0)
{
}

te::qt::plugins::ogr::Plugin::~Plugin() 
{
}

void te::qt::plugins::ogr::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::widgets::DataSourceTypeManager::getInstance().add(new OGRType);

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGR widget startup!"));

  m_initialized = true;
  
  //Initializing action
  QAction* act = te::qt::af::ApplicationController::getInstance().findAction("Project.Add Layer.Tabular File");
  QMenu* mnu = te::qt::af::ApplicationController::getInstance().findMenu("Project.Add Layer");

  if(act != 0 && mnu != 0)
  {
    QWidget* parent = act->parentWidget();
    m_showWindow = new QAction(QIcon::fromTheme("file-vector"), tr("Vector File..."), parent);
    m_showWindow->setObjectName("Project.Add Layer.Vector File");
    mnu->insertAction(act, m_showWindow);
    //mnu->addAction(m_showWindow);

    te::qt::af::AddActionToCustomToolbars(m_showWindow);

    connect (m_showWindow, SIGNAL(triggered()), SLOT(showWindow()));
  }
}

void te::qt::plugins::ogr::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te::da::DataSourceInfoManager::getInstance().removeByType("OGR");
  te::qt::widgets::DataSourceTypeManager::getInstance().remove("OGR");

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGR widget shutdown!"));

  delete m_showWindow;

  m_initialized = false;
}

void te::qt::plugins::ogr::Plugin::showWindow()
{
//  QString filter = GetSupportedFiles();
//  QStringList fileNames = QFileDialog::getOpenFileNames(te::qt::af::ApplicationController::getInstance().getMainWindow(), tr("Open Vector File"), te::qt::af::GetFilePathFromSettings("vector"), filter);
  
  te::qt::af::Project* proj = te::qt::af::ApplicationController::getInstance().getProject();

  if(proj == 0)
  {
    QMessageBox::warning(te::qt::af::ApplicationController::getInstance().getMainWindow(), tr("Vector File"), tr("Error: there is no opened project!"));
    return;
  }

  QStringList fileNames = QFileDialog::getOpenFileNames(te::qt::af::ApplicationController::getInstance().getMainWindow(), tr("Open Vector File"), te::qt::widgets::GetFilePathFromSettings("vector"), tr("Esri Shapefile (*.shp *.SHP);; Mapinfo File (*.mif *.MIF);; GeoJSON (*.geojson *.GeoJSON);; GML (*.gml *.GML);; KML (*.kml *.KML);; All Files (*.*)"));

  if(fileNames.isEmpty())
    return;

  QApplication::setOverrideCursor(Qt::WaitCursor);

  QFileInfo info(fileNames.value(0));

  te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "vector");

  // The selected shapefiles without spatial index
  std::map<std::string, std::string> shpWithoutSpatialIndex;

  std::list<te::map::AbstractLayerPtr> layers;

  bool hasErrors = false;
  QString errorMsg = tr("Error occurred trying to create layer(s):\n\n");

  for(QStringList::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
  {
    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);

    ds->setAccessDriver("OGR");
    
    std::string fpath = it->toStdString();
    std::map<std::string, std::string> dsinfo;
    dsinfo["URI"] = fpath;

    ds->setConnInfo(dsinfo);
    
    std::string desc("A single vector file: ");
    desc += fpath;
    ds->setDescription(desc);
    
    boost::filesystem::path mpath(dsinfo["URI"]);

    std::string fileBaseName = mpath.leaf().string();

    ds->setTitle(fileBaseName);

    ds->setType("OGR");
    
    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);
    
    ds->setId(id);
    te::da::DataSourceInfoManager::getInstance().add(ds);

    if(IsShapeFile(*it) && !HasShapeFileSpatialIndex(*it))
    {
      QString datasetName(fileBaseName.c_str());
      datasetName.remove(".shp", Qt::CaseInsensitive);
      shpWithoutSpatialIndex[id] = datasetName.toStdString();
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
    QMessageBox::warning(te::qt::af::ApplicationController::getInstance().getMainWindow(), tr("Add Layer"), errorMsg);
  }

  if(!shpWithoutSpatialIndex.empty())
  {
    if(QMessageBox::question(te::qt::af::ApplicationController::getInstance().getMainWindow(),
                              tr("Spatial Index"), tr("Do you want create spatial index to the selected ESRI ShapeFiles?"),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      std::map<std::string, std::string>::iterator it;
      for(it = shpWithoutSpatialIndex.begin(); it != shpWithoutSpatialIndex.end(); ++it)
      {
        te::da::DataSourcePtr driver = te::da::GetDataSource(it->first, true);

        std::string command = "CREATE SPATIAL INDEX ON " + it->second;

        driver->execute(command);
      }
      
      QApplication::restoreOverrideCursor();

      QMessageBox::information(te::qt::af::ApplicationController::getInstance().getMainWindow(), tr("Spatial Index"), "Spatial index created with successfully!");
    }
  }

  // If there is only a parent folder layer that is selected, get it as the parent of the layer to be added;
  // otherwise, add the layer as a top level layer
  te::map::AbstractLayerPtr parentLayer(0);

  std::list<te::map::AbstractLayerPtr> selectedLayers = te::qt::af::ApplicationController::getInstance().getProject()->getSelectedLayers();

  if(selectedLayers.size() == 1 && selectedLayers.front()->getType() == "FOLDERLAYER")
    parentLayer = selectedLayers.front();

  std::list<te::map::AbstractLayerPtr>::iterator it;
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    if ((*it)->getSRID() != TE_UNKNOWN_SRS)
    {
      if (!te::srs::SpatialReferenceSystemManager::getInstance().recognizes((*it)->getSRID()))
      {
        QString msgErr(tr("Layer %1 has SRID %2 that is not recognized by TerraLib. Setting it to unknown."));
        msgErr = msgErr.arg((*it)->getTitle().c_str());
        msgErr = msgErr.arg((*it)->getSRID());
        
        QMessageBox::warning(te::qt::af::ApplicationController::getInstance().getMainWindow(),
                              tr("Layer SRS check"), msgErr);
        (*it)->setSRID(TE_UNKNOWN_SRS);
      }
    }
    
    te::qt::af::evt::LayerAdded evt(*it, parentLayer);
    te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  }
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::ogr::Plugin)
