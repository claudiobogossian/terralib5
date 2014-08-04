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
  \file terralib/qt/plugins/layout/Plugin.cpp

  \brief Plugin implementation for the LayoutEditor Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "Plugin.h"

#ifdef TE_QT_PLUGIN_LAYOUT_HAVE_LAYOUTEDITOR
  #include "LayoutEditorAction.h"
#endif

// QT
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::layout::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_layoutMenu(0)
{
}

te::qt::plugins::layout::Plugin::~Plugin() 
{
}

void te::qt::plugins::layout::Plugin::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib LayoutEditor Qt Plugin
  TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_LAYOUT_TEXT_DOMAIN, TE_QT_PLUGIN_LAYOUT_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_TR("TerraLib Qt LAYOUT Plugin startup!"));

// add plugin menu
  m_layoutMenu = te::qt::af::ApplicationController::getInstance().getMenu("LAYOUT");

  m_layoutMenu->setTitle(TE_TR("Layout"));

// register actions
  registerActions();

  m_initialized = true;
}

void te::qt::plugins::layout::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_layoutMenu;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_TR("TerraLib Qt LAYOUT Plugin 2 shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::layout::Plugin::registerActions()
{
#ifdef TE_QT_PLUGIN_LAYOUT_HAVE_LAYOUTEDITOR
    m_layout = new te::qt::plugins::layout::LayoutEditorAction(m_layoutMenu);
#endif
}

void  te::qt::plugins::layout::Plugin::unRegisterActions()
{
#ifdef TE_QT_PLUGIN_LAYOUT_HAVE_LAYOUTEDITOR
    delete m_layout;
#endif
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::layout::Plugin)
