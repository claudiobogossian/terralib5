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
  \file terralib/qt/plugins/terralib4/Plugin.cpp

  \brief Plugin implementation for the TERRALIB4 Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "TL4ConverterAction.h"
#include "Plugin.h"

// QT
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

te::qt::plugins::terralib4::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_terralib4Menu(0)
{
}

te::qt::plugins::terralib4::Plugin::~Plugin() 
{
}

void te::qt::plugins::terralib4::Plugin::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib TERRALIB4 Qt Plugin
  TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_TERRALIB4_TEXT_DOMAIN, TE_QT_PLUGIN_TERRALIB4_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_QT_PLUGIN_TERRALIB4("TerraLib Qt TERRALIB4 Plugin startup!"));

// add plugin menu
  m_terralib4Menu = te::qt::af::ApplicationController::getInstance().getMenu("Terralib4");

// register actions
  registerActions();

  m_initialized = true;
}

void te::qt::plugins::terralib4::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_terralib4Menu;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_QT_PLUGIN_TERRALIB4("TerraLib Qt TERRALIB4 Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::terralib4::Plugin::registerActions()
{
  m_converter = new te::qt::plugins::terralib4::TL4ConverterAction(m_terralib4Menu);
}

void  te::qt::plugins::terralib4::Plugin::unRegisterActions()
{
  delete m_converter;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::terralib4::Plugin)
