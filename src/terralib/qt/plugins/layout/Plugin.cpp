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
  \file terralib/qt/plugins/layout/Plugin.cpp

  \brief Plugin implementation for the LayoutEditor Qt Plugin widget.
*/

// TerraLib
#include "../../../BuildConfig.h"
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/ApplicationEvents.h"
#include "Plugin.h"
#include "../../af/Utils.h"

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
#include <log4cxx/simplelayout.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/rollingfileappender.h>
#endif

#ifdef TE_QT_PLUGIN_LAYOUT_HAVE_LAYOUTEDITOR
  #include "LayoutEditorAction.h"
#endif

// QT
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::layout::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : QObject(),
  te::plugin::Plugin(pluginInfo), m_layoutMenu(0)
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

  TE_LOG_TRACE(TE_TR("TerraLib Qt Map Layout Plugin startup!"));
  
  // add plugin menu
  QMenu* pluginMenu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("Plugins");

  //if(!pluginMenu)
  //  return;


  // register actions
  //registerActions();
  //QMenu* mnu = te::qt::af::AppCtrlSingleton::getInstance().findMenu("Tools");
  //QAction* act = te::qt::af::AppCtrlSingleton::getInstance().findAction("Tools.Customize");

  //if(act)
  //{
//  evt.m_actions << m_showWindow;
  //mnu->insertAction(act, m_showWindow);
  //mnu->addSeparator();

//  connect(m_showWindow, SIGNAL(triggered()), SLOT(showWindow()));

  //if(!pluginsSeparator)
  //  return;

  m_layoutMenu = new QMenu(pluginMenu);
  m_layoutMenu->setIcon(QIcon::fromTheme("map-layout-icon"));
  m_layoutMenu->setTitle(tr("Map Layout"));

  // register actions
  registerActions();

  // layout log startup
  std::string path = te::qt::af::AppCtrlSingleton::getInstance().getUserDataDir().toStdString();
  path += "/log/terralib_map_layout.log";

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
  std::string layout = "%d{ISO8601} [%t] %-5p %c - %m%n";
  log4cxx::LogString lString(layout.begin(), layout.end());

  log4cxx::FileAppender* fileAppender = new log4cxx::RollingFileAppender(log4cxx::LayoutPtr(new log4cxx::PatternLayout(lString)),
    log4cxx::helpers::Transcoder::decode(path.c_str()), true);

  log4cxx::helpers::Pool p;
  fileAppender->activateOptions(p);

  log4cxx::BasicConfigurator::configure(log4cxx::AppenderPtr(fileAppender));
  log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getDebug());

  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("maplayout");
  logger->setAdditivity(false);
  logger->addAppender(fileAppender);
#endif
  m_initialized = true;

  te::qt::af::evt::NewActionsAvailable evt;
  evt.m_category = "Layout";
  evt.m_plgName = "Layout";
  QAction* act = m_layoutMenu->menuAction();
  act->setText(tr("Map Layout"));

  evt.m_actions.push_back(act);

  te::qt::af::AppCtrlSingleton::getInstance().trigger(&evt);

  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_layout->getAction());
}

void te::qt::plugins::layout::Plugin::shutdown()
{
  if(!m_initialized)
    return;
  
  // unregister actions
  unRegisterActions();

  // remove menu
  if(m_layoutMenu)
  {
    delete m_layoutMenu;
    m_layoutMenu = 0;
  }

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
  log4cxx::LogManager::shutdown();
#endif

  TE_LOG_TRACE(TE_TR("TerraLib Qt Map Layout Plugin shutdown!"));

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
    if(m_layout)
    {
      delete m_layout;
      m_layout = 0;
    }
#endif
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::layout::Plugin)
