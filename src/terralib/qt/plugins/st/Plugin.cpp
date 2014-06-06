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
  \file terralib/qt/plugins/st/Plugin.cpp

  \brief Plugin implementation for the st Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "Plugin.h"

#ifdef TE_QT_PLUGIN_ST_HAVE_SLIDER
  #include "TimeSliderWidgetAction.h"
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_OBSERVATION
    #include "ObservationAction.h"
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TIMESERIES
    #include "TimeSeriesAction.h"
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TRAJECTORY
    #include "TrajectoryAction.h"
#endif

// QT
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::st::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_stMenu(0)
{
}

te::qt::plugins::st::Plugin::~Plugin() 
{
}

void te::qt::plugins::st::Plugin::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib st Qt Plugin
  //TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_ST_TEXT_DOMAIN, TE_QT_PLUGIN_ST_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_TR("TerraLib Qt ST Plugin startup!"));

// add plugin menu
  m_stMenu = te::qt::af::ApplicationController::getInstance().getMenu("Project.Add Layer.Add Temporal Layer");

  m_stMenu->setTitle(TE_TR("Add Temporal Layer"));

// register actions
  registerActions();

  m_initialized = true;
}

void te::qt::plugins::st::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// unregister actions
  unRegisterActions();

// remove menu
  delete m_stMenu;

  TE_LOG_TRACE(TE_TR("TerraLib Qt ST Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::st::Plugin::registerActions()
{
//#ifdef TE_QT_PLUGIN_ST_HAVE_SLIDER
//    m_sliderAction = new te::qt::plugins::st::TimeSliderWidgetAction(m_stMenu);
//#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_OBSERVATION
    m_observactionAction = new te::qt::plugins::st::ObservationAction(m_stMenu);
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TIMESERIES
    m_timeSeriesAction = new te::qt::plugins::st::TimeSeriesAction(m_stMenu);
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TRAJECTORY
    m_trajectoryAction = new te::qt::plugins::st::TrajectoryAction(m_stMenu);
#endif
}

void  te::qt::plugins::st::Plugin::unRegisterActions()
{
//#ifdef TE_QT_PLUGIN_ST_HAVE_SLIDER
//    delete m_sliderAction;
//#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_OBSERVATION
    delete m_observactionAction;
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TIMESERIES
    delete m_timeSeriesAction;
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TRAJECTORY
    delete m_trajectoryAction;
#endif
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::st::Plugin)
