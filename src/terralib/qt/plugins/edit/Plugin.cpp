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
  \file terralib/qt/plugins/edit/Plugin.cpp

  \brief Plugin implementation for the TerraLib Edit Qt Plugin.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "ApplicationListener.h"
#include "Plugin.h"
#include "ToolBar.h"

// QT
#include <QAction>
#include <QtCore/QObject>
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::edit::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo),
    m_toolbar(0),
    m_menu(0),
    m_action(0)
{
}

te::qt::plugins::edit::Plugin::~Plugin() 
{
}

void te::qt::plugins::edit::Plugin::startup()
{
  if(m_initialized)
    return;

  // Register the application framework listener
  te::qt::af::ApplicationController::getInstance().addListener(&ApplicationListener::getInstance());

  // Create the main toolbar
  m_toolbar = new ToolBar;

  // Add plugin toolbar
  //te::qt::af::ApplicationController::getInstance().addToolBar("EditToolBar", m_toolbar->get());

  // Get plugins menu
  QMenu* pluginsMenu = te::qt::af::ApplicationController::getInstance().getMenu("Plugins");

  // Create the main menu
  m_menu = new QMenu(pluginsMenu);
  m_menu->setTitle(TE_TR("Edit"));
  m_menu->setIcon(QIcon::fromTheme("layer-edit"));

  // Insert menu before plugins last action
  QAction* lastAction = te::qt::af::ApplicationController::getInstance().findAction("ManagePluginsSeparator");
  pluginsMenu->insertMenu(lastAction, m_menu);

  m_action = new QAction(m_menu);
  //m_action->setObjectName("Layer.Edit");
  m_action->setText("Enable Edit Buttons");
  m_menu->addAction(m_action);

  connect(m_action, SIGNAL(triggered(bool)), this, SLOT(onActionActivated(bool)));

  TE_LOG_TRACE(TE_TR("TerraLib Edit Qt Plugin startup!"));

  m_initialized = true;
}

void te::qt::plugins::edit::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  delete m_toolbar;

  delete m_menu;

  TE_LOG_TRACE(TE_TR("TerraLib Edit Qt Plugin shutdown!"));

  m_initialized = false;
}
void te::qt::plugins::edit::Plugin::onActionActivated(bool)
{
  // Add plugin toolbar
  te::qt::af::ApplicationController::getInstance().addToolBar("EditToolBar", m_toolbar->get());
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::edit::Plugin)
