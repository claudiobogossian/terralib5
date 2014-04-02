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
  \file terralib/qt/plugins/vp/Plugin.cpp

  \brief Plugin implementation for the VP Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "AggregationAction.h"
#include "BasicGeographicOpAction.h"
#include "BufferAction.h"
#include "IntersectionAction.h"
#include "Plugin.h"
//#include "PolygonToLineAction.h"
//#include "SummarizationAction.h"
//#include "TransformationAction.h"

// QT
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

te::qt::plugins::vp::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo), m_vpMenu(0)
{
}

te::qt::plugins::vp::Plugin::~Plugin() 
{
}

void te::qt::plugins::vp::Plugin::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib VP Qt Plugin
  TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_VP_TEXT_DOMAIN, TE_QT_PLUGIN_VP_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_QT_PLUGIN_VP("TerraLib Qt VP Plugin startup!"));

// add plugin menu
  m_vpMenu = te::qt::af::ApplicationController::getInstance().getMenu("VP");

  m_vpMenu->setTitle(TE_QT_PLUGIN_VP("Vector Processing"));

// register actions
  registerActions();

  m_initialized = true;
}

void te::qt::plugins::vp::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_vpMenu;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_QT_PLUGIN_VP("TerraLib Qt VP Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::vp::Plugin::registerActions()
{
  m_aggregation = new te::qt::plugins::vp::AggregationAction(m_vpMenu);
  m_basicGeographicOp = new te::qt::plugins::vp::BasicGeographicOpAction(m_vpMenu);
  m_buffer = new te::qt::plugins::vp::BufferAction(m_vpMenu);
  m_intersection = new te::qt::plugins::vp::IntersectionAction(m_vpMenu);
  //m_polygonToLine = new te::qt::plugins::vp::PolygonToLineAction(m_vpMenu);
  //m_summarization = new te::qt::plugins::vp::SummarizationAction(m_vpMenu);
  //m_transformation = new te::qt::plugins::vp::TransformationAction(m_vpMenu);
}

void  te::qt::plugins::vp::Plugin::unRegisterActions()
{
  delete m_aggregation;
  delete m_basicGeographicOp;
  delete m_buffer;
  delete m_intersection;
  //delete m_polygonToLine;
  //delete m_summarization;
  //delete m_transformation;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::vp::Plugin)
