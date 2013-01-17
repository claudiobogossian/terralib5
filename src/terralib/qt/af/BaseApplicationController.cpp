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

// Boost
#include <boost/foreach.hpp> // Boost => don't change this include order, otherwise you may have compiling problems! 

// TerraLib
#include "../../common/Exception.h"
#include "../../common/Translator.h"
#include "../../common/TerraLib.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/UserApplicationSettings.h"
#include "../../common/Logger.h"
#include "../../plugin/PluginManager.h"
#include "../../plugin/PluginInfo.h"
#include "../widgets/help/AssistantHelpManagerImpl.h"
#include "../widgets/help/HelpManager.h"
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
#include <QtGui/QMessageBox>
#include <QtGui/QWidget>

// Boost
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

const te::qt::af::BaseApplicationController& sm_core = te::qt::af::ApplicationController::getInstance();

te::qt::af::BaseApplicationController::BaseApplicationController(QObject* parent)
  : QObject(parent),
    m_msgBoxParentWidget(0),
    m_initialized(false)
{
}

te::qt::af::BaseApplicationController::~BaseApplicationController()
{
  finalize();
}

void te::qt::af::BaseApplicationController::setConfigFile(const std::string& configFileName)
{
  m_appConfigFile = configFileName;
}

void te::qt::af::BaseApplicationController::setMsgBoxParentWidget(QWidget* w)
{
  m_msgBoxParentWidget = w;
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

  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  SplashScreenManager::getInstance().showMessage(tr("Loading TerraLib Modules..."));

  TerraLib::getInstance().initialize();

  SplashScreenManager::getInstance().showMessage(tr("TerraLib Modules loaded!"));

  SplashScreenManager::getInstance().showMessage(tr("Loading the application configuration file..."));

  if(m_appConfigFile.empty())
    m_appConfigFile = TERRALIB_APPLICATION_CONFIG_FILE;

  te::common::SystemApplicationSettings::getInstance().load(m_appConfigFile);

// general application info
  SplashScreenManager::getInstance().showMessage(tr("Application configuration file loaded!"));

  m_appName = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Name"));

  qApp->setApplicationName(m_appName);

  m_appOrganization = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Organization"));

  qApp->setOrganizationName(m_appOrganization);

// read used config data
  SplashScreenManager::getInstance().showMessage(tr("Reading user settings..."));

  std::string userSettingsFile = te::common::SystemApplicationSettings::getInstance().getValue("Application.UserSettingsFile.<xmlattr>.xlink:href");

  te::common::UserApplicationSettings::getInstance().load(userSettingsFile);

  SplashScreenManager::getInstance().showMessage(tr("User settings read!"));

// read application resources
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

// load help system
  try
  {
    m_appHelpFile = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.HelpFile.<xmlattr>.xlink:href"));

    if(!m_appHelpFile.isEmpty())
    {
      SplashScreenManager::getInstance().showMessage(tr("Loading application help system..."));

      te::qt::widgets::AssistantHelpManagerImpl* helpImpl = new te::qt::widgets::AssistantHelpManagerImpl(m_appHelpFile, this);

      te::qt::widgets::HelpManager::getInstance().setImpl(helpImpl);

      SplashScreenManager::getInstance().showMessage(tr("Application help system loaded!"));
    }
  }
  catch(const std::exception& e)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Error loading application help system: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
  }

// load icon theme
  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Loading application icon theme..."));

    m_appIconThemeDir = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href"));

    if(!m_appIconThemeDir.isEmpty())
    {
      QStringList ithemes = QIcon::themeSearchPaths();

      ithemes.push_back(m_appIconThemeDir);

      QIcon::setThemeSearchPaths(ithemes);
    }

    m_appDefaultIconTheme = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.DefaultTheme"));

    QString iconTheme = QString::fromStdString(te::common::UserApplicationSettings::getInstance().getValue("UserSettings.SelectedIconTheme"));

    if(iconTheme.isEmpty())
      QIcon::setThemeName(m_appDefaultIconTheme);
    else
      QIcon::setThemeName(iconTheme);

    SplashScreenManager::getInstance().showMessage(tr("Application icon theme loaded!"));
  }
  catch(const std::exception& e)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Error loading application icon theme: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
  }
}

void te::qt::af::BaseApplicationController::initializePlugins()
{
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  bool loadPlugins = true;

  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Reading application plugins list..."));

    std::string appPlugins = te::common::SystemApplicationSettings::getInstance().getValue("Application.PluginsFile.<xmlattr>.xlink:href");

    ApplicationPlugins::getInstance().load(appPlugins);

    SplashScreenManager::getInstance().showMessage(tr("Plugins list read!"));
  }
  catch(const std::exception& e)
  {
    loadPlugins = false;

    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Error reading application's plugin list: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
  }

  if(!loadPlugins)
    return;

  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Loading plugins..."));

    UserPlugins::getInstance().load();

    SplashScreenManager::getInstance().showMessage(tr("Plugins loaded successfully!"));
  }
  catch(const std::exception& e)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Some plugins couldn't be loaded: %1.\n "
                      "Please, refer to plugin manager to fix the problem."));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
  }
}

void te::qt::af::BaseApplicationController::finalize()
{
  if(!m_initialized)
    return;

  //savePluginsFiles();

  te::plugin::PluginManager::getInstance().shutdownAll();

  te::plugin::PluginManager::getInstance().unloadAll();

  TerraLib::getInstance().finalize();

  m_appConfigFile.clear();

  m_initialized = false;
}

void  te::qt::af::BaseApplicationController::broadcast(te::qt::af::Event* evt)
{
  // Need to check event send to prevent loops
  // -----------------------------------------

  emit triggered(evt);
}

const QString& te::qt::af::BaseApplicationController::getAppTitle() const
{
  return m_appTitle;
}

const QString& te::qt::af::BaseApplicationController::getAppIconName() const
{
  return m_appIconName;
}

