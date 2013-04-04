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
  \file terralib/qt/plugins/rp/Plugin.cpp

  \brief Plugin implementation for the RP Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "Plugin.h"

#ifdef TE_QT_PLUGIN_RP_HAVE_CONTRAST
  #include "ContrastAction.h"
#endif

// QT
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

te::qt::plugins::rp::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_rpMenu(0)
{
}

te::qt::plugins::rp::Plugin::~Plugin() 
{
}

void te::qt::plugins::rp::Plugin::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib RP Qt Plugin
  TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_RP_TEXT_DOMAIN, TE_QT_PLUGIN_RP_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_QT_PLUGIN_RP("TerraLib Qt RP Plugin startup!"));

// add plugin menu
  m_rpMenu = te::qt::af::ApplicationController::getInstance().getMenu("RP");

  m_rpMenu->setTitle(TE_QT_PLUGIN_RP("Raster Processing"));

// register actions
  registerActions();

  m_initialized = true;
}

void te::qt::plugins::rp::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_rpMenu;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_QT_PLUGIN_RP("TerraLib Qt RP Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::rp::Plugin::registerActions()
{
#ifdef TE_QT_PLUGIN_RP_HAVE_CONTRAST
    m_contrast = new te::qt::plugins::rp::ContrastAction(m_rpMenu);
#endif
}

void  te::qt::plugins::rp::Plugin::unRegisterActions()
{
#ifdef TE_QT_PLUGIN_RP_HAVE_CONTRAST
    delete m_contrast;
#endif
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::rp::Plugin)
