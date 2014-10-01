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
  \file terralib/qt/plugins/slider/Plugin.cpp

  \brief Plugin implementation for the slider Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "Plugin.h"

#ifdef TE_QT_PLUGIN_SLIDER_HAVE_SLIDER
  #include "TimeSliderWidgetAction.h"
#endif

// QT
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::slider::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_sliderMenu(0)
{
}

te::qt::plugins::slider::Plugin::~Plugin() 
{
}

void te::qt::plugins::slider::Plugin::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib slider Qt Plugin
  //TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_SLIDER_TEXT_DOMAIN, TE_QT_PLUGIN_SLIDER_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_TR("TerraLib Qt SLIDER Plugin startup!"));

// add plugin menu
  m_sliderMenu = te::qt::af::ApplicationController::getInstance().getMenu("SLIDER");

  m_sliderMenu->setTitle(TE_TR("Slider"));

// register actions
  registerActions();

  m_initialized = true;
}

void te::qt::plugins::slider::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// unregister actions
  unRegisterActions();

// remove menu
  delete m_sliderMenu;

  TE_LOG_TRACE(TE_TR("TerraLib Qt SLIDER Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::slider::Plugin::registerActions()
{
#ifdef TE_QT_PLUGIN_SLIDER_HAVE_SLIDER
    m_sliderAction = new te::qt::plugins::slider::TimeSliderWidgetAction(m_sliderMenu);
#endif
}

void  te::qt::plugins::slider::Plugin::unRegisterActions()
{
#ifdef TE_QT_PLUGIN_SLIDER_HAVE_SLIDER
    delete m_sliderAction;
#endif
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::slider::Plugin)
