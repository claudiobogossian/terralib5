/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/plugins/datasource/shp/Plugin.cpp

  \brief Plugin implementation for the Shapefile data source widget.
*/

// TerraLib
#include "../../../../common/Config.h"
#include "../../../../common/Translator.h"
#include "../../../../common/Logger.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../widgets/layer/utils/DataSet2Layer.h"

#include "../../../af/ApplicationController.h"
#include "../../../af/Utils.h"
#include "../../../af/events/LayerEvents.h"

#include "Plugin.h"

// OGR
#include <ogrsf_frmts.h>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Qt 
#include <QAction>
#include <QMenu>
#include <QFileDialog>

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

QString GetFileExtensionName(const std::string& type)
{
  if(type.compare("ESRI Shapefile") == 0)
    return QObject::tr("ESRI Shapefile (*.shp *.SHP)");
  else if(type.compare("MapInfo File") == 0)
    return QObject::tr("MapInfo File (*.mid *.MID)");
  else if(type.compare("DGN") == 0)
    return QObject::tr("DGN File (*.dgn *.DGN)");
  else if(type.compare("CSV") == 0)
    return QObject::tr("CSV File (*.csv *.CSV)");
  else if(type.compare("GML") == 0)
    return QObject::tr("GML File (*.gml *.GML)");
  else if(type.compare("KML") == 0)
    return QObject::tr("KML File (*.kml *.KML)");
  else if(type.compare("GeoJSON") == 0)
    return QObject::tr("GeoJSON File (*.json *.JSON)");
  else if(type.compare("DXF") == 0)
    return QObject::tr("DXF File (*.dxf *.DXF)");
  else if(type.compare("PDF") == 0)
    return QObject::tr("PDF File (*.pdf *.PDF)");

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

te::qt::plugins::shp::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo) :
QObject(),
te::plugin::Plugin(pluginInfo)
{
}

te::qt::plugins::shp::Plugin::~Plugin() 
{
}

void te::qt::plugins::shp::Plugin::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib Shapefile driver support
  TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_DATASOURCE_SHP_TEXT_DOMAIN, TE_QT_PLUGIN_DATASOURCE_SHP_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_QT_PLUGIN_DATASOURCE_SHP("TerraLib Qt Shapefile widget startup!"));

  m_initialized = true;

  //Initializing action
  QAction* act = te::qt::af::ApplicationController::getInstance().findAction("Project.Add Layer.All Sources");
  QMenu* mnu = te::qt::af::ApplicationController::getInstance().findMenu("Project.Add Layer");

  if(act != 0 && mnu != 0)
  {
    QWidget* parent = act->parentWidget();
    m_showWindow = new QAction(QIcon::fromTheme("datasource-shapefile"), tr("Vector File..."), parent);
    m_showWindow->setObjectName("Project.Add Layer.Vector File");
    mnu->insertAction(act, m_showWindow);

    te::qt::af::AddActionToCustomToolbars(m_showWindow);

    connect (m_showWindow, SIGNAL(triggered()), SLOT(showWindow()));
  }
}

void te::qt::plugins::shp::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  TE_LOG_TRACE(TE_QT_PLUGIN_DATASOURCE_SHP("TerraLib Qt Shapefile widget shutdown!"));

  delete m_showWindow;

  m_initialized = false;
}

void te::qt::plugins::shp::Plugin::showWindow()
{
  QString filter = GetSupportedFiles();

  QStringList fileNames = QFileDialog::getOpenFileNames(te::qt::af::ApplicationController::getInstance().getMainWindow(), tr("Open Vector File"), QString(), filter);

  if(fileNames.isEmpty())
    return;

  std::list<te::map::AbstractLayerPtr> layers;

  for(QStringList::iterator it = fileNames.begin(); it != fileNames.end(); ++it)
  {
    te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);

    ds->setAccessDriver("OGR");

    std::map<std::string, std::string> dsinfo;
    dsinfo["URI"] = it->toStdString();

    ds->setConnInfo(dsinfo);

    ds->setDescription("A single vector file");

    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);

    ds->setId(id);

    boost::filesystem::path mpath(dsinfo["URI"]);

    std::string fileBaseName = mpath.stem().string();

    ds->setTitle(fileBaseName);

    ds->setType("OGR");

    te::da::DataSourceInfoManager::getInstance().add(ds);

    GetLayers(ds, layers);
  }

  if(!layers.empty())
  {
    te::qt::af::evt::LayersAdded evt(layers);
    te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  }
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::shp::Plugin)
