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
  \file terralib/qt/plugins/vp/Plugin.cpp

  \brief Plugin implementation for the VP Qt Plugin widget.
*/

// TerraLib
#include "../../../BuildConfig.h"
#include "../../../common/Config.h"
#include "../../../core/translator/Translator.h"
#include "../../../core/logger/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/Utils.h"
#include "AggregationAction.h"
#include "BufferAction.h"
#include "DifferenceAction.h"
#include "GeometricOpAction.h"
#include "IntersectionAction.h"
#include "LineToPolygonAction.h"
#include "MergeAction.h"
#include "MultipartToSinglepartAction.h"
#include "Plugin.h"
#include "PolygonToLineAction.h"
#include "UnionAction.h"

// QT
#include <QMenu>
#include <QMenuBar>
#include <QString>

// STL
#include <string>

te::qt::plugins::vp::Plugin::Plugin(const te::core::PluginInfo& pluginInfo)
  : te::core::CppPlugin(pluginInfo),
    m_vpMenu(0),
    m_aggregation(0),
    m_buffer(0),
    m_difference(0),
    m_geometricOp(0),
    m_intersection(0),
    m_lineToPolygon(0),
    m_merge(0),
    m_multipart2singlepart(0),
    m_polygonToLine(0),
    m_union(0)
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
  //TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_VP_TEXT_DOMAIN, TE_QT_PLUGIN_VP_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_TR("TerraLib Qt VP Plugin startup!"));

// add plugin menu
  QMenu* pluginMenu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("Processing");
  m_vpMenu = new QMenu(pluginMenu);
  m_vpMenu->setIcon(QIcon::fromTheme("vp-vectorprocessing-icon"));

  // Insert action before plugin manager action
  QAction* pluginsSeparator = te::qt::af::AppCtrlSingleton::getInstance().findAction("ManagePluginsSeparator");

  pluginMenu->insertMenu(pluginsSeparator, m_vpMenu);

  m_vpMenu->setTitle(TE_TR("Vector Processing"));

// register actions
  registerActions();

  m_initialized = true;

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);
}

void te::qt::plugins::vp::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_vpMenu;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_TR("TerraLib Qt VP Plugin shutdown!"));

  m_initialized = false;

  te::qt::af::AppCtrlSingleton::getInstance().removeListener(this);
}

void te::qt::plugins::vp::Plugin::registerActions()
{
  m_aggregation = new te::qt::plugins::vp::AggregationAction(m_vpMenu);
  m_buffer = new te::qt::plugins::vp::BufferAction(m_vpMenu);
  m_difference = new te::qt::plugins::vp::DifferenceAction(m_vpMenu);
  m_geometricOp = new te::qt::plugins::vp::GeometricOpAction(m_vpMenu);
  m_intersection = new te::qt::plugins::vp::IntersectionAction(m_vpMenu);
  m_merge = new te::qt::plugins::vp::MergeAction(m_vpMenu);
  m_lineToPolygon = new te::qt::plugins::vp::LineToPolygonAction(m_vpMenu);
  m_multipart2singlepart = new te::qt::plugins::vp::MultipartToSinglepartAction(m_vpMenu);
  m_polygonToLine = new te::qt::plugins::vp::PolygonToLineAction(m_vpMenu);
  m_union = new te::qt::plugins::vp::UnionAction(m_vpMenu);

  connect(m_aggregation, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_buffer, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_difference, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_geometricOp, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_intersection, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_merge, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_lineToPolygon, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_multipart2singlepart, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_polygonToLine, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_union, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));

  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_aggregation->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_buffer->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_difference->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_geometricOp->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_intersection->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_merge->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_multipart2singlepart->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_polygonToLine->getAction());
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_union->getAction());
}

void  te::qt::plugins::vp::Plugin::unRegisterActions()
{
  delete m_aggregation;
  delete m_buffer;
  delete m_difference;
  delete m_geometricOp;
  delete m_intersection;
  delete m_merge;
  delete m_lineToPolygon;
  delete m_multipart2singlepart;
  delete m_polygonToLine;
  delete m_union;
  
}

TERRALIB_PLUGIN_CALL_BACK_IMPL(te::qt::plugins::vp::Plugin)

