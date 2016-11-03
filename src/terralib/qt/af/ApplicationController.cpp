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
  \file terralib/qt/af/ApplicationController.cpp

  \brief The base API for controllers of TerraLib applications.
*/

// Boost
#include <boost/foreach.hpp> // Boost => don't change this include order, otherwise you may have compiling problems! 

// TerraLib
#include "../../BuildConfig.h"
#include "../../common/Exception.h"
#include "../../common/TerraLib.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/UserApplicationSettings.h"
#include "../../common/Version.h"
#include "../../core/logger/Logger.h"
#include "../../core/plugin.h"
#include "../../core/translator/Translator.h"
#include "../../core/utils/Platform.h"
#include "../../dataaccess/serialization/xml/Serializer.h"
#include "../../srs/Config.h"
#include "../widgets/help/AssistantHelpManagerImpl.h"
#include "../widgets/help/HelpManager.h"
#include "../widgets/Utils.h"
#include "../widgets/utils/ScopedCursor.h"
#include "events/ApplicationEvents.h"
#include "ApplicationController.h"
#include "Exception.h"
//#include "Project.h"
#include "SplashScreenManager.h"
#include "Utils.h"
#include "XMLFormatter.h"

// Qt
#include <QApplication>
#if QT_VERSION < 0x050000
#include <QDesktopServices>
#endif
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QMessageBox>
#include <QResource>
#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#endif
#include <QWidget>

// Boost
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>

//STL
#include <algorithm>

te::qt::af::ApplicationController::ApplicationController(/*QObject* parent*/)
  : QObject(/*parent*/),
    m_msgBoxParentWidget(0),
    m_defaultSRID(TE_UNKNOWN_SRS),
    m_selectionColor(QColor(0, 255, 0)),
    m_initialized(false),
    m_resetTerralib(true)
{
}

te::qt::af::ApplicationController::~ApplicationController()
{

}

void te::qt::af::ApplicationController::setConfigFile(const std::string& configFileName)
{
  m_appConfigFile = configFileName;
}

void te::qt::af::ApplicationController::setMsgBoxParentWidget(QWidget* w)
{
  m_msgBoxParentWidget = w;
}

void te::qt::af::ApplicationController::addToolBar(const QString& id, QToolBar* bar)
{
  registerToolBar(id, bar);

// send event: tool bar added
  te::qt::af::evt::ToolBarAdded evt(bar);

  emit triggered(&evt);
}

void te::qt::af::ApplicationController::registerToolBar(const QString& id, QToolBar* bar)
{
  QToolBar* b = getToolBar(id);

  if(b != 0)
    throw Exception(TE_TR("There is already a tool bar registered with the same name!"));

  m_toolbars[id] = bar;
}

QToolBar* te::qt::af::ApplicationController::getToolBar(const QString& id) const
{
  std::map<QString, QToolBar*>::const_iterator it = m_toolbars.find(id);

  return (it != m_toolbars.end()) ? it->second : 0;
}

std::vector<QToolBar*> te::qt::af::ApplicationController::getToolBars() const
{
  std::vector<QToolBar*> res;
  std::map<QString, QToolBar*>::const_iterator it;

  for(it = m_toolbars.begin(); it != m_toolbars.end(); ++it)
    res.push_back(it->second);

  return res;
}

void te::qt::af::ApplicationController::removeToolBar(const QString& id)
{
  std::map<QString, QToolBar*>::iterator it = m_toolbars.find(id);

  if(it != m_toolbars.end())
    m_toolbars.erase(it);
}

void te::qt::af::ApplicationController::registerMenu(QMenu* mnu)
{
  m_menus.push_back(mnu);
}

QMenu* te::qt::af::ApplicationController::findMenu(const QString& id) const
{
  std::vector<QMenu*>::const_iterator it;

  // Searching in menus vector
  for(it = m_menus.begin(); it != m_menus.end(); ++it)
  {
    QMenu* mnu = te::qt::widgets::FindMenu(id, *it);
    
    if(mnu != 0)
      return mnu;
  }

  // Searching in menu bars vector
  std::vector<QMenuBar*>::const_iterator it_bar;
  
  for(it_bar = m_menuBars.begin(); it_bar != m_menuBars.end(); ++it_bar)
  {
    QMenu* mnu = te::qt::widgets::FindMenu(id, *it_bar);

    if(mnu != 0)
      return mnu;
  }

  return 0;
}

QMenu* te::qt::af::ApplicationController::getMenu(const QString& id)
{
  QMenu* mnu = findMenu(id);

  if(mnu == 0)
  {
    if(!m_menuBars.empty())
      mnu = te::qt::widgets::GetMenu(id, m_menuBars[0]);
    else
    {
      mnu = new QMenu(id);
      m_menus.push_back(mnu);
    }
  }

  return mnu;
}

void te::qt::af::ApplicationController::registerMenuBar(QMenuBar* bar)
{
  m_menuBars.push_back(bar);
}

QMenuBar* te::qt::af::ApplicationController::findMenuBar(const QString& id) const
{
  throw Exception("Not implemented yet.");
}

QMenuBar* te::qt::af::ApplicationController::getMenuBar(const QString& id) const
{
  return m_menuBars[0];
}

QAction* te::qt::af::ApplicationController::findAction(const QString& id) const
{
  for(size_t i=0; i<m_menus.size(); i++)
  {
    QAction* act = te::qt::widgets::FindAction(id, m_menus[i]);
    
    if (act != 0)
      return act;
  }

  for(size_t i=0; i<m_menuBars.size(); i++)
  {
    QAction* act = te::qt::widgets::FindAction(id, m_menuBars[i]);

    if (act != 0)
      return act;
  }

  return 0;
}

QActionGroup* te::qt::af::ApplicationController::findActionGroup(const QString& id) const
{
  for(size_t i=0; i<m_menus.size(); i++)
  {
    QActionGroup* actGroup = te::qt::widgets::FindActionGroup(id, m_menus[i]);
    
    if (actGroup != 0)
      return actGroup;
  }

  for(size_t i=0; i<m_menuBars.size(); i++)
  {
    QActionGroup* actGroup = te::qt::widgets::FindActionGroup(id, m_menuBars[i]);

    if (actGroup != 0)
      return actGroup;
  }

  return 0;
}

void te::qt::af::ApplicationController::addListener(QObject* obj, const ListenerType& type)
{
  if(type == SENDER || type == BOTH)
    connect(obj, SIGNAL(triggered(te::qt::af::evt::Event*)),
            this, SIGNAL(triggered(te::qt::af::evt::Event*)));

  if(type == RECEIVER || type == BOTH)
    obj->connect(this, SIGNAL(triggered(te::qt::af::evt::Event*)), SLOT(onApplicationTriggered(te::qt::af::evt::Event*)));
}

void te::qt::af::ApplicationController::removeListener(QObject* obj)
{
  disconnect(obj);
}

void  te::qt::af::ApplicationController::initialize()
{
  if(m_initialized)
    return;
  
// find user data directory
#if QT_VERSION >= 0x050000
  m_userDataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
  m_userDataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
  
  if(!te::core::FileSystem::exists(m_userDataDir.toUtf8().data()))
    te::core::FileSystem::createDirectories(m_userDataDir.toUtf8().data());

  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  SplashScreenManager::getInstance().showMessage(tr("Loading TerraLib Modules..."));

// terralib log startup
#ifdef TERRALIB_LOGGER_ENABLED
  std::string logfile(m_userDataDir.toUtf8().data());
  logfile += "/log/TerraLib.log";
  TE_INIT_DEFAULT_LOGGER(logfile);
#endif

  if(m_resetTerralib)
    TerraLib::getInstance().initialize();

  SplashScreenManager::getInstance().showMessage(tr("TerraLib Modules loaded!"));

  SplashScreenManager::getInstance().showMessage(tr("Loading the application configuration file..."));

  te::common::SystemApplicationSettings::getInstance().load(m_appConfigFile);

// general application info
  SplashScreenManager::getInstance().showMessage(tr("Application configuration file loaded!"));

  m_appName = QString::fromUtf8(te::common::SystemApplicationSettings::getInstance().getValue("Application.Name").c_str());
  m_appTitle = QString::fromUtf8(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str());
  //m_appProjectExtension = QString::fromUtf8(te::common::SystemApplicationSettings::getInstance().getValue("Application.ProjectExtension"));
  m_appIconName = QString::fromUtf8(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconName").c_str());
  
  if(!te::core::FileSystem::exists(m_appIconName.toUtf8().data()))
    m_appIconName = te::core::FindInTerraLibPath(m_appIconName.toUtf8().data()).c_str();

  m_appPluginsPath = QString::fromUtf8(te::common::SystemApplicationSettings::getInstance().getValue("Application.Plugins.<xmlattr>.xlink:href").c_str());

  if (!te::core::FileSystem::exists(m_appPluginsPath.toUtf8().data()))
    m_appPluginsPath = te::core::FindInTerraLibPath(m_appPluginsPath.toUtf8().data()).c_str();

  m_aboutLogo = QString::fromUtf8(te::common::SystemApplicationSettings::getInstance().getValue("Application.AboutDialogLogo.<xmlattr>.xlink:href").c_str());
  
  if(!te::core::FileSystem::exists(m_aboutLogo.toUtf8().data()))
    m_aboutLogo = te::core::FindInTerraLibPath(m_aboutLogo.toUtf8().data()).c_str();
  
  m_tLibLogo = QString::fromUtf8(te::common::SystemApplicationSettings::getInstance().getValue("Application.TerraLibLogo.<xmlattr>.xlink:href").c_str());
  
  if(!te::core::FileSystem::exists(m_tLibLogo.toUtf8().data()))
    m_tLibLogo = te::core::FindInTerraLibPath(m_tLibLogo.toUtf8().data()).c_str();

  QString fullAppName = m_appName + "-" + QString(te::common::Version::asString().c_str());
  qApp->setApplicationName(fullAppName);

  m_appOrganization = QString::fromUtf8(te::common::SystemApplicationSettings::getInstance().getValue("Application.Organization").c_str());

  qApp->setOrganizationName(m_appOrganization);

// load help system
  try
  {
    std::string help_file = te::core::FindInTerraLibPath(te::common::SystemApplicationSettings::getInstance().getValue("Application.HelpFile.<xmlattr>.xlink:href"));

    m_appHelpFile = QString::fromUtf8(help_file.c_str());

    QFileInfo info(m_appHelpFile);

    if(!m_appHelpFile.isEmpty() && info.exists())
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
  
// hold user settings
  QSettings user_settings(QSettings::IniFormat,
                          QSettings::UserScope,
                          QApplication::instance()->organizationName(),
                          QApplication::instance()->applicationName());

// load icon theme
  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Loading application icon theme..."));

    std::string icon_dir = te::core::FindInTerraLibPath(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href"));
    
    m_appIconThemeDir = QString::fromUtf8(icon_dir.c_str());

    if(!m_appIconThemeDir.isEmpty())
    {
      QStringList ithemes = QIcon::themeSearchPaths();

      ithemes.push_back(m_appIconThemeDir);

      QIcon::setThemeSearchPaths(ithemes);
    }

    m_appDefaultIconTheme = QString::fromUtf8(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.DefaultTheme").c_str());
    
    QVariant iconTheme = user_settings.value("icon_theme/selected_theme", m_appDefaultIconTheme);

    QIcon::setThemeName(iconTheme.toString());

    QVariant iconSize = user_settings.value("toolbars/icon_size", te::common::SystemApplicationSettings::getInstance().getValue("Application.ToolBarDefaultIconSize").c_str());

    {
      QString sh = QString("QToolBar { qproperty-iconSize: ") + iconSize.toString() + "px " + iconSize.toString() + "px; }";
      qApp->setStyleSheet(sh);
    }

// Default SRID
    QVariant srid = user_settings.value("srs/default_srid", te::common::SystemApplicationSettings::getInstance().getValue("Application.DefaultSRID").c_str());
    
    m_defaultSRID = srid.toInt();

// Selection Color
    QVariant selectionColor = user_settings.value("color/selection_color", te::common::SystemApplicationSettings::getInstance().getValue("Application.DefaultSelectionColor").c_str());

    m_selectionColor = QColor(selectionColor.toString());

    SplashScreenManager::getInstance().showMessage(tr("Application icon theme loaded!"));
  }
  catch(const std::exception& e)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Error loading application icon theme: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
  }

// load registered data sources
  try
  {
    m_appDatasourcesFile = user_settings.value("data_sources/data_file", "").toString().toUtf8().data();

    if(!m_appDatasourcesFile.empty())
    {
      SplashScreenManager::getInstance().showMessage(tr("Loading user registered data sources..."));

      te::serialize::xml::ReadDataSourceInfo(m_appDatasourcesFile);

      te::qt::af::XMLFormatter::formatDataSourceInfos(false);

      SplashScreenManager::getInstance().showMessage(tr("Known data sources loaded!"));
    }
    else
    {
      const QString& udir = getUserDataDir();

      QVariant fileName = udir + "/" + QString(TERRALIB_APPLICATION_DATASOURCE_FILE_NAME);

      QFileInfo infoDataSourceFile(fileName.toString());
      
      if (infoDataSourceFile.exists())
      {
        SplashScreenManager::getInstance().hide();

        int reply = QMessageBox::question(0, tr("Data Sources XML"), tr("A file containing data sources already configured was found. Would you like to load it."), QMessageBox::No, QMessageBox::Yes);

        SplashScreenManager::getInstance().show();

        if (reply == QMessageBox::Yes)
        {
          std::string dataSourcesFile = fileName.toString().toUtf8().data();

          te::serialize::xml::ReadDataSourceInfo(dataSourcesFile);

          te::qt::af::XMLFormatter::formatDataSourceInfos(false);

          SplashScreenManager::getInstance().showMessage(tr("Known data sources loaded!"));
        }
      }
    }
  }
  catch(const std::exception& e)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Error loading the registered data sources: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
  }

  m_initialized = true;
}

void te::qt::af::ApplicationController::initializePlugins()
{
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  QSettings user_settings(QSettings::IniFormat, QSettings::UserScope,
                          QApplication::instance()->organizationName(),
                          QApplication::instance()->applicationName());

  SplashScreenManager::getInstance().showMessage(
      tr("Initializing plugin system..."));

  te::core::plugin::InitializePluginSystem();

  user_settings.beginGroup("plugins");

  std::set<std::string> user_loaded_plugins;

  int nLoaded = user_settings.beginReadArray("loaded");

  for(int i = 0; i != nLoaded; ++i)
  {
    user_settings.setArrayIndex(i);

    QString name = user_settings.value("name").toString();

    user_loaded_plugins.insert(name.toUtf8().data());
  }

  user_settings.endArray();

  // get the unloaded plugins
  std::set<std::string> user_unloaded_plugins;
  int nUnloaded = user_settings.beginReadArray("unloaded");

  for(int i = 0; i != nUnloaded; ++i)
  {
    user_settings.setArrayIndex(i);

    QString name = user_settings.value("name").toString();

    user_unloaded_plugins.insert(name.toUtf8().data());
  }

  user_settings.endArray();

  user_settings.endGroup();

  if(user_loaded_plugins.size() > 0 || user_unloaded_plugins.size() > 0)
  {
    SplashScreenManager::getInstance().showMessage(tr("Loading plugins..."));

    std::vector<te::core::PluginInfo> v_pInfo = te::core::DefaultPluginFinder();
    v_pInfo = te::core::plugin::TopologicalSort(v_pInfo);
    std::vector<std::string> failToLoad;

    for(const te::core::PluginInfo& pinfo : v_pInfo)
    {
      try
      {
        te::core::PluginManager::instance().insert(pinfo);
        if(user_loaded_plugins.find(pinfo.name) != user_loaded_plugins.end())
          te::core::PluginManager::instance().load(pinfo.name);
      }
      catch(...)
      {
        failToLoad.push_back(pinfo.name);
      }
    }    
    if(failToLoad.size() > 0)
    {
      te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

      boost::format err_msg(
          tr("Could not load the following plugins:\n\n%1%")
              .toUtf8()
              .data());

      QMessageBox msg(
          QMessageBox::Warning, m_appTitle,
          (err_msg % boost::algorithm::join(failToLoad, "\n")).str().c_str());
      msg.setWindowFlags(Qt::WindowStaysOnTopHint);
      msg.exec();
    }
    else
    {
      SplashScreenManager::getInstance().showMessage(
          tr("Plugins loaded successfully!"));
    }
  }
  else
  {
    try
    {
      SplashScreenManager::getInstance().showMessage(tr("Loading plugins..."));

      te::core::plugin::LoadAll();

      SplashScreenManager::getInstance().showMessage(
          tr("Plugins loaded successfully!"));
    }
    catch(const boost::exception& e)
    {
      te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

      if(const std::string* d = boost::get_error_info<te::ErrorDescription>(e))
      {
        QMessageBox msg(
            QMessageBox::Warning, m_appTitle, d->c_str());
        msg.setWindowFlags(Qt::WindowStaysOnTopHint);
        msg.exec();
      }
      else
      {
        QMessageBox msg(
            QMessageBox::Warning, m_appTitle, "An unknown error has occurred");
        msg.setWindowFlags(Qt::WindowStaysOnTopHint);
        msg.exec();
      }
    }
    catch(const std::exception& e)
    {
      te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

      QString msgErr(tr("Error reading application's plugin list: %1"));

      msgErr = msgErr.arg(e.what());
      QMessageBox msg(
          QMessageBox::Warning, m_appTitle,
           msgErr);
      msg.setWindowFlags(Qt::WindowStaysOnTopHint);
      msg.exec();
    }
  }
}

void te::qt::af::ApplicationController::finalize()
{
  if(!m_initialized)
    return;

  te::core::PluginManager::instance().clear();
  te::core::plugin::FinalizePluginSystem();


  if(m_resetTerralib)
    TerraLib::getInstance().finalize();

  m_appConfigFile.clear();

  m_applicationItems.clear();

  m_menuBars.clear();

  m_menus.clear();

  m_toolbars.clear();

  m_aboutLogo.clear();

  m_appDatasourcesFile.clear();

  m_appDefaultIconTheme.clear();

  m_appHelpFile.clear();

  m_appIconName.clear();

  m_appIconThemeDir.clear();

  m_appName.clear();

  m_msgBoxParentWidget = 0;

  m_appOrganization.clear(); 

  m_appTitle.clear();

  m_tLibLogo.clear();
          
  m_appUserSettingsFile.clear();
  
  m_appPluginsPath.clear();
  
  m_appToolBarDefaultIconSize.clear();
  
  m_defaultSRID = 0;
  
  m_selectionColor = QColor();

  m_initialized = false;
}

QSettings& te::qt::af::ApplicationController::getSettings()
{
  return m_appSettings;
}

const QString& te::qt::af::ApplicationController::getAppName() const
{
  return m_appName;
}

const QString& te::qt::af::ApplicationController::getAppTitle() const
{
  return m_appTitle;
}

const QString& te::qt::af::ApplicationController::getAppIconName() const
{
  return m_appIconName;
}

const QString& te::qt::af::ApplicationController::getAppPluginsPath() const
{
  return m_appPluginsPath;
}

const QString& te::qt::af::ApplicationController::getAboutLogo() const
{
  return m_aboutLogo;
}

const QString& te::qt::af::ApplicationController::getTlibLogo() const
{
  return m_tLibLogo;
}

int te::qt::af::ApplicationController::getDefaultSRID() const
{
  return m_defaultSRID;
}

QColor te::qt::af::ApplicationController::getSelectionColor() const
{
  return m_selectionColor;
}

void te::qt::af::ApplicationController::setSelectionColor(const QColor& c)
{
  m_selectionColor = c;
}

QWidget* te::qt::af::ApplicationController::getMainWindow() const
{
  return m_msgBoxParentWidget;
}

void te::qt::af::ApplicationController::setResetTerraLibFlag(const bool& status)
{
  m_resetTerralib = status;
}

const QString& te::qt::af::ApplicationController::getUserDataDir() const
{
  return m_userDataDir;
}

void te::qt::af::ApplicationController::trigger(te::qt::af::evt::Event* e)
{
  emit triggered(e);
}
