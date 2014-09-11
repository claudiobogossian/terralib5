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
  \file terralib/qt/plugins/cellspace/Plugin.cpp

  \brief Plugin implementation for the Cellular Spaces Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "CreateCellLayerAction.h"
#include "Plugin.h"

// QT
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::cellspace::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_cellSpaceMenu(0)
{
}

te::qt::plugins::cellspace::Plugin::~Plugin() 
{
}

void te::qt::plugins::cellspace::Plugin::startup()
{
  if(m_initialized)
    return;

  TE_LOG_TRACE(TE_TR("TerraLib Qt Cellular Spaces Plugin startup!"));

// add plugin menu
  QMenu* pluginMenu = te::qt::af::ApplicationController::getInstance().getMenu("Plugins");
  m_cellSpaceMenu = new QMenu(pluginMenu);

  // Insert action before plugin manager action
  QAction* pluginsSeparator = te::qt::af::ApplicationController::getInstance().findAction("ManagePluginsSeparator");

  pluginMenu->insertMenu(pluginsSeparator, m_cellSpaceMenu);

  m_cellSpaceMenu->setTitle(TE_TR("Cellular Spaces"));
  m_cellSpaceMenu->setIcon(QIcon::fromTheme("cellspace"));

  // register actions
  registerActions();

// add pop up menu
  m_popupAction = new QAction(m_cellSpaceMenu);
  m_popupAction->setText(TE_TR("Cellular Spaces"));

  m_initialized = true;
}

void te::qt::plugins::cellspace::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_cellSpaceMenu;

  // unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_TR("TerraLib Qt Cellular Spaces Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::cellspace::Plugin::registerActions()
{
  m_createCellLayer = new te::qt::plugins::cellspace::CreateCellLayerAction(m_cellSpaceMenu);
}

void te::qt::plugins::cellspace::Plugin::unRegisterActions()
{
  delete m_createCellLayer;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::cellspace::Plugin)
