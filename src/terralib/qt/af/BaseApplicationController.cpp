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
  \file terralib/qt/af/BaseApplicationController.cpp

  \brief The base API for controllers of TerraLib applications.
*/

// TerraLib
#include "../../common/Exception.h"
#include "../../common/Translator.h"
#include "../../common/TerraLib.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/UserApplicationSettings.h"
#include "../../common/Logger.h"
#include "../../plugin/PluginManager.h"
#include "../../plugin/PluginInfo.h"
#include "../widgets/utils/ScopedCursor.h"
#include "events/NewToolBar.h"
#include "ApplicationController.h"
#include "ApplicationPlugins.h"
#include "BaseApplicationController.h"
#include "Exception.h"
#include "SplashScreenManager.h"
#include "UserPlugins.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QResource>
#include <QtGui/QIcon>

// Boost
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>

const te::qt::af::BaseApplicationController& sm_core = te::qt::af::ApplicationController::getInstance();

te::qt::af::BaseApplicationController::BaseApplicationController(QObject* parent)
  : QObject(parent),
    m_initialized(false)
{
}

te::qt::af::BaseApplicationController::~BaseApplicationController()
{
  finalize();
}

void te::qt::af::BaseApplicationController::setMainConfig(const std::string& configFileName)
{
  m_configFile = configFileName;
}

void te::qt::af::BaseApplicationController::addToolBar(const QString& id, QToolBar* bar)
{
  registerToolBar(id, bar);

// send event: tool bar added
  te::qt::af::NewToolBar evt(bar);

  broadcast(&evt);
}

void te::qt::af::BaseApplicationController::registerToolBar(const QString& id, QToolBar* bar)
{
  QToolBar* b = getToolBar(id);

  if(b != 0)
    throw Exception(TR_QT_AF("There is already a tool bar registered with the same name!"));

  m_toolbars[id] = bar;
}

QToolBar* te::qt::af::BaseApplicationController::getToolBar(const QString& id) const
{
  std::map<QString, QToolBar*>::const_iterator it = m_toolbars.find(id);

  return (it != m_toolbars.end()) ? it->second : 0;
}

void te::qt::af::BaseApplicationController::addListener(QObject* obj)
{
  std::set<QObject*>::const_iterator it = m_applicationItems.find(obj);

  if(it != m_applicationItems.end())
    return;

  m_applicationItems.insert(obj);

  obj->connect(this, SIGNAL(triggered(te::qt::af::Event*)), SLOT(onApplicationTriggered(te::qt::af::Event*)));
}

void te::qt::af::BaseApplicationController::removeListener(QObject* obj)
{
  std::set<QObject*>::iterator it = m_applicationItems.find(obj);

  if(it == m_applicationItems.end())
    return;

  m_applicationItems.erase(it);

  disconnect(SIGNAL(triggered(te::qt::af::Event*)), obj, SLOT(onApplicationTriggered(te::qt::af::Event*)));
}

void  te::qt::af::BaseApplicationController::initialize()
{
  if(m_initialized)
    return;

  initializeMainModules();

  initializePlugins();

  m_initialized = true;
}

void te::qt::af::BaseApplicationController::initializeMainModules()
{
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  SplashScreenManager::getInstance().showMessage(tr("Loading TerraLib Modules..."));

  TerraLib::getInstance().initialize();

  SplashScreenManager::getInstance().showMessage(tr("TerraLib Modules loaded!"));

  SplashScreenManager::getInstance().showMessage(tr("Loading the application configuration file..."));

  if(m_configFile.empty())
    m_configFile = TERRALIB_APPLICATION_CONFIG_FILE;

  te::common::SystemApplicationSettings::getInstance().load(m_configFile);

  SplashScreenManager::getInstance().showMessage(tr("Application configuration file loaded!"));

  bool loadPlugins = true;

  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Reading application plugins list..."));

    std::string appPlugins = te::common::SystemApplicationSettings::getInstance().getValue("Application.PluginsFile.<xmlattr>.xlink:href");

    ApplicationPlugins::getInstance().load(appPlugins);

    SplashScreenManager::getInstance().showMessage(tr("User settings read!"));
  }
  catch(...)
  {
    loadPlugins = false;
  }

  bool loadUserSettings = true;

  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Reading user settings..."));

    std::string userSettingsFile = te::common::SystemApplicationSettings::getInstance().getValue("Application.UserSettingsFile.<xmlattr>.xlink:href");

    te::common::UserApplicationSettings::getInstance().load(userSettingsFile);

    SplashScreenManager::getInstance().showMessage(tr("User settings read!"));
  }
  catch(...)
  {
    loadUserSettings = false;
  }

  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Loading application resources..."));

    //boost::property_tree::ptree& p = te::common::SystemApplicationSettings::getInstance().getAllSettings();

    //BOOST_FOREACH(boost::property_tree::ptree::value_type& v, p.get_child("Application.Resources"))
    //{
    //  const std::string& resourceFile = v.second.get<std::string>("<xmlattr>.xlink:href");

    //  QResource::registerResource(resourceFile.c_str());
    //}

    SplashScreenManager::getInstance().showMessage(tr("Application resources loaded!"));
  }
  catch(...)
  {
  }

//// load help implementation
//  if(m_helpImpl)
//    delete m_helpImpl;

  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Loading application help system..."));

    //std::string help_file = te::common::SystemApplicationSettings::getInstance().getValue("Application.HelpFile.<xmlattr>.xlink:href");

    //m_helpImpl = new te::qt::widgets::AssistantHelpManagerImpl(help_file.c_str(), this);
    //te::qt::widgets::HelpManager::getInstance().setImpl(m_helpImpl);
    //SplashScreenManager::getInstance().showMessage(tr("Help System Configured!"));

    SplashScreenManager::getInstance().showMessage(tr("Application help system loaded!"));
  }
  catch(...)
  {
  }

  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Loading application icon theme..."));

    std::string ithemedir = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href");

    if(!ithemedir.empty())
    {
      QStringList ithemes = QIcon::themeSearchPaths();
      ithemes.push_back(ithemedir.c_str());
      QIcon::setThemeSearchPaths(ithemes);
    }

    std::string iconTheme = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.SelectedIconTheme");

    if(iconTheme.empty())
      iconTheme = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.DefaultTheme");

    if(!iconTheme.empty())
      QIcon::setThemeName(iconTheme.c_str());

    SplashScreenManager::getInstance().showMessage(tr("Application icon theme loaded!"));
  }
  catch(...)
  {
    // TODO:????
  }
}

void te::qt::af::BaseApplicationController::initializePlugins()
{
  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Loading plugins..."));

    UserPlugins::getInstance().load();

    SplashScreenManager::getInstance().showMessage(tr("Plugins loaded successfully!"));
  }
  catch(const std::exception& e)
  {
    //TR_QT_AF("Some problems occurried during application framework startup: \n");
    //errMsg += e.what();
    //te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    //QMessageBox::warning(this,
    //                     tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
    //                     tr("Some plugins couldn't be loaded! Please, refer to plugin manager to fix the problem."));
  }

  std::string appName = te::common::SystemApplicationSettings::getInstance().getValue("Application.Name");

  qApp->setApplicationName(appName.c_str());

  std::string appOrg = te::common::SystemApplicationSettings::getInstance().getValue("Application.Organization");
  qApp->setOrganizationName(appOrg.c_str());

}

void te::qt::af::BaseApplicationController::finalize()
{
  if(!m_initialized)
    return;

  //savePluginsFiles();

  te::plugin::PluginManager::getInstance().shutdownAll();

  te::plugin::PluginManager::getInstance().unloadAll();

  TerraLib::getInstance().finalize();

  m_configFile.clear();

  m_initialized = false;
}

void  te::qt::af::BaseApplicationController::broadcast(te::qt::af::Event* evt)
{
  // Need to check event send to prevent loops
  // -----------------------------------------

  emit triggered(evt);
}

