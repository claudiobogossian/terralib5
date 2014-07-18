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
  \file terralib/qt/plugins/sa/Plugin.cpp

  \brief Plugin implementation for the SA Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "Plugin.h"

#ifdef TE_QT_PLUGIN_SA_HAVE_PROXIMITYMATRIXCREATOR
  #include "ProximityMatrixCreatorAction.h"
#endif


// QT
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::sa::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_saMenu(0)
{
}

te::qt::plugins::sa::Plugin::~Plugin() 
{
}

void te::qt::plugins::sa::Plugin::startup()
{
  if(m_initialized)
    return;

  TE_LOG_TRACE(TE_TR("TerraLib Qt SA Plugin startup!"));

// add plugin menu
  m_saMenu = te::qt::af::ApplicationController::getInstance().getMenu("SA");

  m_saMenu->setTitle(TE_TR("Spatial Analisys"));

// add pop up menu
  m_popupAction = new QAction(m_saMenu);
  m_popupAction->setText(TE_TR("Spatial Analisys"));

// register actions
  registerActions();

  m_initialized = true;
}

void te::qt::plugins::sa::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_saMenu;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_TR("TerraLib Qt SA Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::sa::Plugin::registerActions()
{

#ifdef TE_QT_PLUGIN_SA_HAVE_PROXIMITYMATRIXCREATOR
  m_proxMatrixCreator = new te::qt::plugins::sa::ProximityMatrixCreatorAction(m_saMenu);
#endif

}

void  te::qt::plugins::sa::Plugin::unRegisterActions()
{

#ifdef TE_QT_PLUGIN_SA_HAVE_PROXIMITYMATRIXCREATOR
    delete m_proxMatrixCreator;
#endif

}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::sa::Plugin)
