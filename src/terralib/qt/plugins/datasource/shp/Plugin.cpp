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

#include "../../../af/ApplicationController.h"

#include "Plugin.h"

// Qt 
#include <QAction>
#include <QMenu>

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
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::shp::Plugin)
