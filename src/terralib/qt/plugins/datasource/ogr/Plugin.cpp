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
#include "../../../../core/logger/Logger.h"
#include "../../../../core/translator/Translator.h"
#include "../../../../srs/Config.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../widgets/datasource/core/DataSourceTypeManager.h"
#include "../../../widgets/layer/explorer/LayerItemView.h"
#include "../../../widgets/Utils.h"
#include "../../../af/ApplicationController.h"
#include "../../../af/BaseApplication.h"
#include "../../../af/Utils.h"
#include "../../../af/events/ApplicationEvents.h"
#include "GeoFileDragAndDropHandler.h"
#include "OGRType.h"
#include "Plugin.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>

// Qt 
#include <QFileInfo>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QToolBar>

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
    //std::string dName = dMgr->GetDriver(i)->GetName();
    std::string dName = dMgr->GetDriver(i)->GetDescription();

    QString drv = GetFileExtensionName(dName);

    if(!drv.isEmpty())
      res += drv + ";;";
  }

  return res;
}

te::qt::plugins::ogr::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) : 
QObject(),
te::plugin::Plugin(pluginInfo),
m_showWindow(0),
m_handler(0)
{
  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);
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
  
//  //Initializing action
//  QAction* act = te::qt::af::AppCtrlSingleton::getInstance().findAction("Project.Add Layer.Tabular File");
//  QMenu* mnu = te::qt::af::AppCtrlSingleton::getInstance().findMenu("Project.Add Layer");

//  if(act != 0 && mnu != 0)
  {
//    QWidget* parent = act->parentWidget();
    m_showWindow = new QAction(QIcon::fromTheme("file-vector"), tr("Vector File..."), this);
    m_showWindow->setToolTip(tr("Add new vector file as a layer."));
    m_showWindow->setObjectName("Project.Add Layer.Vector File");

    te::qt::af::evt::NewActionsAvailable e;
    e.m_category = "Dataaccess";
    e.m_actions << m_showWindow;

    emit triggered(&e);

    connect (m_showWindow, SIGNAL(triggered()), SLOT(showWindow()));

    //register actions into application tool bar
    QToolBar* toolBar = te::qt::af::AppCtrlSingleton::getInstance().getToolBar("File Tool Bar");

    if (toolBar)
      toolBar->addAction(m_showWindow);

    //install handler
    m_handler = new GeoFileDragAndDropHandler();

    te::qt::af::BaseApplication* ba = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
    ba->getLayerExplorer()->installEventFilter(m_handler);
  }
}

void te::qt::plugins::ogr::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  //remove event handler
  te::qt::af::BaseApplication* ba = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  ba->getLayerExplorer()->removeEventFilter(m_handler);

  te::da::DataSourceInfoManager::getInstance().removeByType("OGR");
  te::qt::widgets::DataSourceTypeManager::getInstance().remove("OGR");

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGR widget shutdown!"));

//  delete m_showWindow;

  m_initialized = false;
}

void te::qt::plugins::ogr::Plugin::showWindow()
{
  QStringList fileNames = QFileDialog::getOpenFileNames(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(), tr("Open Vector File"), te::qt::widgets::GetFilePathFromSettings("vector"), tr("Esri Shapefile (*.shp *.SHP);; Mapinfo File (*.mif *.MIF);; GeoJSON (*.geojson *.GeoJSON);; GML (*.gml *.GML);; KML (*.kml *.KML);; All Files (*.*)"));

  if(fileNames.isEmpty())
    return;

  QFileInfo info(fileNames.value(0));

  te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "vector");

  te::qt::plugins::ogr::CreateLayers(fileNames);
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::ogr::Plugin)
