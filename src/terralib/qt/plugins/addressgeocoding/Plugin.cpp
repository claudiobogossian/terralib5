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
  \file terralib/qt/plugins/addressgeocoding/Plugin.cpp

  \brief Plugin implementation for the Address Geocoding Qt Plugin widget.
*/

// TerraLib
#include "../../../addressgeocoding/qt/MainWindowDialog.h"
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Project.h"
#include "Plugin.h"

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
#endif

// QT
#include <QAction>
#include <QtCore/QObject>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

te::qt::plugins::addressgeocoding::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::qt::plugins::addressgeocoding::Plugin::~Plugin() 
{
}

void te::qt::plugins::addressgeocoding::Plugin::startup()
{
  if(m_initialized)
    return;

  TE_LOG_TRACE(TE_TR("TerraLib Qt Address Geocoding Plugin startup!"));

// add plugin menu
	QMenu* pluginMenu = te::qt::af::ApplicationController::getInstance().getMenu("Plugins");

	m_action = new QAction(pluginMenu);
	m_action->setText("Address Geocoding...");
  m_action->setIcon(QIcon::fromTheme("addressgeocoding-icon"));

	connect(m_action, SIGNAL(triggered(bool)), this, SLOT(onActionActivated(bool)));

	// Insert action before plugin manager action
  QAction* pluginsSeparator = te::qt::af::ApplicationController::getInstance().findAction("ManagePluginsSeparator");
	pluginMenu->insertAction(pluginsSeparator, m_action);

  // address geocoding log startup
  std::string path = te::qt::af::ApplicationController::getInstance().getUserDataDir().toStdString();
  path += "/log/terralib_addressgeocoding.log";

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
  log4cxx::FileAppender* fileAppender = new log4cxx::FileAppender(log4cxx::LayoutPtr(new log4cxx::SimpleLayout()),
  log4cxx::helpers::Transcoder::decode(path.c_str()), false);

  log4cxx::helpers::Pool p;
  fileAppender->activateOptions(p);

  log4cxx::BasicConfigurator::configure(log4cxx::AppenderPtr(fileAppender));
  log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getDebug());
  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("addressgeocoding");
#endif

  m_initialized = true;
}

void te::qt::plugins::addressgeocoding::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// unregister actions
  unRegisterActions();

#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
  log4cxx::LogManager::shutdown();
#endif

  TE_LOG_TRACE(TE_TR("TerraLib Qt Address Geocoding Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::addressgeocoding::Plugin::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::ApplicationController::getInstance().getMainWindow();
	te::addressgeocoding::MainWindowDialog dlg(parent);

  // get the list of layers from current project
  te::qt::af::Project* prj = te::qt::af::ApplicationController::getInstance().getProject();

  if(prj)
  {
    dlg.setLayers(prj->getSingleLayers(false));
  }

  if(dlg.exec() != QDialog::Accepted)
    return;

  te::map::AbstractLayerPtr layer = dlg.getLayer();

  if(!layer)
    return;

  int reply = QMessageBox::question(0, tr("Address Geocoding Result"), tr("The operation was concluded successfully. Would you like to add the layer to the project?"), QMessageBox::No, QMessageBox::Yes);

  if(prj && reply == QMessageBox::Yes)
  {
    te::qt::af::evt::LayerAdded evt(layer);

    te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  }
}

std::list<te::map::AbstractLayerPtr> te::qt::plugins::addressgeocoding::Plugin::getLayers()
{
  std::list<te::map::AbstractLayerPtr> list;

  te::qt::af::Project* prj = te::qt::af::ApplicationController::getInstance().getProject();

  if(prj)
    list = prj->getAllLayers(false);

  return list;
}


void te::qt::plugins::addressgeocoding::Plugin::unRegisterActions()
{
	delete m_action;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::addressgeocoding::Plugin)
