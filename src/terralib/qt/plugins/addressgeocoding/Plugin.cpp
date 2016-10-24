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
  \file terralib/qt/plugins/addressgeocoding/Plugin.cpp

  \brief Plugin implementation for the Address Geocoding Qt Plugin widget.
*/

// TerraLib
#include "../../../BuildConfig.h"
#include "../../../addressgeocoding/qt/MainWindowDialog.h"
#include "../../../common/Config.h"
#include "../../../core/translator/Translator.h"
#include "../../../core/logger/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Utils.h"
#include "Plugin.h"

// QT
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

te::qt::plugins::addressgeocoding::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : QObject(),
  te::plugin::Plugin(pluginInfo)
{
}

te::qt::plugins::addressgeocoding::Plugin::~Plugin() 
{
}

void te::qt::plugins::addressgeocoding::Plugin::startup()
{
  if(m_initialized)
    return;

  TE_LOG_TRACE(TE_TR("TerraLib Qt Address Geocoding Plugin startup!"));

// add plugin menu
  QMenu* pluginMenu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("Processing");

  m_action = new QAction(pluginMenu);
  m_action->setText("Address Geocoding...");
  m_action->setIcon(QIcon::fromTheme("addressgeocoding-icon"));
  m_action->setObjectName("Processing.Address Geocoding");

  connect(m_action, SIGNAL(triggered(bool)), this, SLOT(onActionActivated(bool)));

// Insert action before plugin manager action
  QAction* pluginsSeparator = te::qt::af::AppCtrlSingleton::getInstance().findAction("ManagePluginsSeparator");
  pluginMenu->insertAction(pluginsSeparator, m_action);

  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_action);

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);

  m_initialized = true;
}

void te::qt::plugins::addressgeocoding::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_TR("TerraLib Qt Address Geocoding Plugin shutdown!"));

  m_initialized = false;

  te::qt::af::AppCtrlSingleton::getInstance().removeListener(this);
}

void te::qt::plugins::addressgeocoding::Plugin::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();
  te::addressgeocoding::MainWindowDialog dlg(parent);

  // get the list of layers from current project
  te::qt::af::evt::GetAvailableLayers e;

  emit triggered(&e);

  dlg.setLayers(e.m_layers);

  if(dlg.exec() != QDialog::Accepted)
    return;

  te::map::AbstractLayerPtr layer = dlg.getLayer();

  if(layer.get() == 0)
    return;

  int reply = QMessageBox::question(0, tr("Address Geocoding Result"), tr("The operation was concluded successfully. Would you like to add the layer to the project?"), QMessageBox::No, QMessageBox::Yes);

  if(reply == QMessageBox::Yes)
  {
    te::qt::af::evt::LayerAdded evt(layer);

    emit triggered(&evt);
  }
}

std::list<te::map::AbstractLayerPtr> te::qt::plugins::addressgeocoding::Plugin::getLayers()
{
  te::qt::af::evt::GetAvailableLayers e;

  emit triggered(&e);

  return e.m_layers;
}


void te::qt::plugins::addressgeocoding::Plugin::unRegisterActions()
{
  delete m_action;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::addressgeocoding::Plugin)
