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
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../../common/StringUtils.h"
#include "../../../qt/af/ApplicationController.h"

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

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_CREATELAYER
#include "CreateLayerAction.h"
#endif

// QT
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::terramobile::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_menu(0)
{
}

te::qt::plugins::terramobile::Plugin::~Plugin()
{
}

void te::qt::plugins::terramobile::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);

  TE_LOG_TRACE(TE_TR("Terra Mobile Plugin startup!"));

// add plugin menu
  m_menu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("terramobile");

  m_menu->setTitle(TE_TR("Mobile"));

// add pop up menu
  m_popupAction = new QAction(m_menu);
  m_popupAction->setText(TE_TR("Mobile"));

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
  connect(m_gpBuider, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEPUBLISHER
  m_gpPublisher = new te::qt::plugins::terramobile::GeoPackagePublisherAction(m_menu);
  connect(m_gpPublisher, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_GATHERINGLAYERCONFIGURER
  m_glConfiguer = new te::qt::plugins::terramobile::GatheringLayerConfigurerAction(m_menu);
  connect(m_glConfiguer, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_CREATELAYER
  m_menu->addSeparator();
  m_createLayer = new te::qt::plugins::terramobile::CreateLayerAction(m_menu);
  connect(m_createLayer, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
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

#ifdef TE_QT_PLUGIN_TERRAMOBILE_HAVE_CREATELAYER
  delete m_createLayer;
#endif

}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::terramobile::Plugin)
