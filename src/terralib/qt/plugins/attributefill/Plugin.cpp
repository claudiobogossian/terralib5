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
  \file terralib/qt/plugins/attributefill/Plugin.cpp

  \brief Plugin implementation for the Attribute Fill Qt Plugin widget.
*/

// TerraLib
#include "../../../BuildConfig.h"
#include "../../../common/Config.h"
#include "../../../core/translator/Translator.h"
#include "../../../core/logger/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Utils.h"
#include "RasterToVectorAction.h"
#include "VectorToRasterAction.h"
#include "VectorToVectorAction.h"
#include "Plugin.h"

// QT
#include <QMenu>
#include <QMenuBar>
#include <qaction.h>

te::qt::plugins::attributefill::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo),
    m_attributefillMenu(0),
    m_popupAction(0),
    m_rasterToVector(0),
    m_vectorToRaster(0),
    m_vectorToVector(0)
{
}

te::qt::plugins::attributefill::Plugin::~Plugin() 
{
}

void te::qt::plugins::attributefill::Plugin::startup()
{
  if(m_initialized)
    return;

  TE_LOG_TRACE(TE_TR("TerraLib Qt Attribute Fill Plugin startup!"));

// add plugin menu
  QMenu* pluginMenu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("Processing");
  m_attributefillMenu = new QMenu(pluginMenu);
  m_attributefillMenu->setIcon(QIcon::fromTheme("attributefill-icon"));

  // Insert action before plugin manager action
  QAction* pluginsSeparator = te::qt::af::AppCtrlSingleton::getInstance().findAction("ManagePluginsSeparator");

  pluginMenu->insertMenu(pluginsSeparator, m_attributefillMenu);

  m_attributefillMenu->setTitle(TE_TR("Attribute Fill"));

  // register actions
  registerActions();

// add pop up menu
  m_popupAction = new QAction(m_attributefillMenu);
  m_popupAction->setText(TE_TR("Attribute Fill"));

  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_rasterToVector->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_vectorToRaster->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_vectorToVector->getAction());

  m_initialized = true;

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);
}

void te::qt::plugins::attributefill::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_attributefillMenu;

  // unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_TR("TerraLib Qt Attribute Fill Plugin shutdown!"));

  m_initialized = false;

  te::qt::af::AppCtrlSingleton::getInstance().removeListener(this);
}

void te::qt::plugins::attributefill::Plugin::registerActions()
{
  m_rasterToVector = new te::qt::plugins::attributefill::RasterToVectorAction(m_attributefillMenu);
  m_vectorToRaster = new te::qt::plugins::attributefill::VectorToRasterAction(m_attributefillMenu);
  m_vectorToVector = new te::qt::plugins::attributefill::VectorToVectorAction(m_attributefillMenu);

  connect(m_rasterToVector, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_vectorToRaster, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_vectorToVector, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
}

void te::qt::plugins::attributefill::Plugin::unRegisterActions()
{
  delete m_rasterToVector;
  delete m_vectorToRaster;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::attributefill::Plugin)
