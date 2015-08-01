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
#include "terralib_config.h"
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/Utils.h"
#include "AggregationAction.h"
#include "GeometricOpAction.h"
#include "BufferAction.h"
#include "IntersectionAction.h"
#include "LineToPolygonAction.h"
#include "MultipartToSinglepartAction.h"
#include "Plugin.h"
#include "PolygonToLineAction.h"
//#include "SummarizationAction.h"
//#include "TransformationAction.h"

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
//Log4cxx
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/helpers/transcoder.h>
#include <log4cxx/logger.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/logstring.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/rollingfileappender.h>
#endif

// QT
#include <QMenu>
#include <QMenuBar>
#include <QString>

// STL
#include <string>

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

// vp log startup
  std::string path = te::qt::af::AppCtrlSingleton::getInstance().getUserDataDir().toStdString();
  path += "/log/terralib_vp.log";

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
  std::string layout = "%d{ISO8601} [%t] %-5p %c - %m%n";
  log4cxx::LogString lString(layout.begin(), layout.end());

  log4cxx::FileAppender* fileAppender = new log4cxx::RollingFileAppender(log4cxx::LayoutPtr(new log4cxx::PatternLayout(lString)),
    log4cxx::helpers::Transcoder::decode(path.c_str()), true);

  log4cxx::helpers::Pool p;
  fileAppender->activateOptions(p);

  log4cxx::BasicConfigurator::configure(log4cxx::AppenderPtr(fileAppender));
  log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getDebug());

  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("vp");
  logger->setAdditivity(false);
  logger->addAppender(fileAppender);

#endif
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

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
  log4cxx::LogManager::shutdown();
#endif

  TE_LOG_TRACE(TE_TR("TerraLib Qt VP Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::vp::Plugin::registerActions()
{
  m_aggregation = new te::qt::plugins::vp::AggregationAction(m_vpMenu);
  m_buffer = new te::qt::plugins::vp::BufferAction(m_vpMenu);
  m_geometricOp = new te::qt::plugins::vp::GeometricOpAction(m_vpMenu);
  m_intersection = new te::qt::plugins::vp::IntersectionAction(m_vpMenu);
  m_multipart2singlepart = new te::qt::plugins::vp::MultipartToSinglepartAction(m_vpMenu);
  m_lineToPolygon = new te::qt::plugins::vp::LineToPolygonAction(m_vpMenu);
  m_polygonToLine = new te::qt::plugins::vp::PolygonToLineAction(m_vpMenu);
  //m_summarization = new te::qt::plugins::vp::SummarizationAction(m_vpMenu);
  //m_transformation = new te::qt::plugins::vp::TransformationAction(m_vpMenu);

  te::qt::af::AddActionToCustomToolbars(m_aggregation->getAction());
  te::qt::af::AddActionToCustomToolbars(m_buffer->getAction());
  te::qt::af::AddActionToCustomToolbars(m_geometricOp->getAction());
  te::qt::af::AddActionToCustomToolbars(m_intersection->getAction());
  te::qt::af::AddActionToCustomToolbars(m_multipart2singlepart->getAction());
  te::qt::af::AddActionToCustomToolbars(m_lineToPolygon->getAction());
  te::qt::af::AddActionToCustomToolbars(m_polygonToLine->getAction());
}

void  te::qt::plugins::vp::Plugin::unRegisterActions()
{
  delete m_aggregation;
  delete m_buffer;
  delete m_geometricOp;
  delete m_intersection;
  delete m_multipart2singlepart;
  delete m_lineToPolygon;
  delete m_polygonToLine;
  //delete m_summarization;
  //delete m_transformation;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::vp::Plugin)

