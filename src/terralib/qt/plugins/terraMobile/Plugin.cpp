/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file src/terraMobilePlugin/Plugin.cpp

  \brief Plugin implementation for the RP Qt Plugin widget.
*/

// TerraLib
#include <terralib/common/Translator.h>
#include <terralib/common/Logger.h>
#include <terralib/qt/af/ApplicationController.h>

#include "Plugin.h"

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEBUILDER
  #include "GeoPackageBuilderAction.h"
#endif

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEPUBLISHER
  #include "GeoPackagePublisherAction.h"
#endif

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GATHERINGLAYERCONFIGURER
#include "GatheringLayerConfigurerAction.h"
#endif

// QT
#include <QMenu>
#include <QMenuBar>

//#include "d:\Workspace\BOEING\gdal\include\ogrsf_frmts.h"
//#include "d:\Workspace\BOEING\gdal\include\ogr_p.h"
//#include "d:\Workspace\BOEING\gdal\include\cpl_conv.h"
//#include "d:\Workspace\BOEING\gdal\include\cpl_string.h"
//#include "d:\Workspace\BOEING\gdal\include\ogr_api.h"
//#include "d:\Workspace\BOEING\gdal\include\gdal.h"
//#include "d:\Workspace\BOEING\gdal\include\gdal_alg.h"

te::qt::plugins::terramobile::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_menu(0)
{
}

te::qt::plugins::terramobile::Plugin::~Plugin()
{
}

void te::qt::plugins::terramobile::Plugin::startup()
{
  
  //std::string version = GDAL_RELEASE_NAME;

  //OGRRegisterAll();

  //OGRSFDriverRegistrar* poR = OGRSFDriverRegistrar::GetRegistrar();

  //for (int iDriver = 0; iDriver < poR->GetDriverCount(); iDriver++)
  //{
  //  GDALDriver *poDriver = poR->GetDriver(iDriver);

  //  if (CSLTestBoolean(CSLFetchNameValueDef(poDriver->GetMetadata(), GDAL_DCAP_CREATE, "FALSE")))
  //    printf("     -f \"%s\"\n", poDriver->GetDescription());
  //}


  //return;


  if(m_initialized)
    return;

  TE_LOG_TRACE(TE_TR("Terra Mobile Plugin startup!"));

// add plugin menu
  m_menu = te::qt::af::ApplicationController::getInstance().getMenu("terramobile");

  m_menu->setTitle(TE_TR("TerraMobile"));

// add pop up menu
  m_popupAction = new QAction(m_menu);
  m_popupAction->setText(TE_TR("TerraMobile"));

// register actions
  registerActions();

  m_initialized = true;
}

void te::qt::plugins::terramobile::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_menu;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_TR("Terra Mobile Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::terramobile::Plugin::registerActions()
{

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEBUILDER
  m_gpBuider = new te::qt::plugins::terramobile::GeoPackageBuilderAction(m_menu);
#endif

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEPUBLISHER
  m_gpPublisher = new te::qt::plugins::terramobile::GeoPackagePublisherAction(m_menu);
#endif

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GATHERINGLAYERCONFIGURER
  m_glConfiguer = new te::qt::plugins::terramobile::GatheringLayerConfigurerAction(m_menu);
#endif

}

void  te::qt::plugins::terramobile::Plugin::unRegisterActions()
{

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEBUILDER
  delete m_gpBuider;
#endif

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEPUBLISHER
  delete m_gpPublisher;
#endif

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GATHERINGLAYERCONFIGURER
  delete m_glConfiguer;
#endif

}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::terramobile::Plugin)
