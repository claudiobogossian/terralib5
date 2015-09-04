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
#include "../../common/PlatformUtils.h"
#include "../../common/Translator.h"
#include "../../common/TerraLib.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/UserApplicationSettings.h"
#include "../../common/Logger.h"
#include "../../common/Version.h"
#include "../../dataaccess/serialization/xml/Serializer.h"
#include "../../plugin/PluginManager.h"
#include "../../plugin/PluginInfo.h"
#include "../../plugin/Utils.h"
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

//STL
#include <algorithm>

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

//te::qt::af::ApplicationController* te::qt::af::ApplicationController::sm_instance(0);

te::qt::af::ApplicationController::ApplicationController(/*QObject* parent*/)
  : QObject(/*parent*/),
    m_msgBoxParentWidget(0),
    m_defaultSRID(TE_UNKNOWN_SRS),
    m_selectionColor(QColor(0, 255, 0)),
    m_initialized(false),
//    m_project(0),
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
  
  if(!boost::filesystem::exists(m_userDataDir.toStdString()))
    boost::filesystem::create_directories(m_userDataDir.toStdString());

  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  SplashScreenManager::getInstance().showMessage(tr("Loading TerraLib Modules..."));

// terralib log startup
#if defined(TERRALIB_APACHE_LOG4CXX_ENABLED) && defined(TERRALIB_LOGGER_ENABLED)
  std::string path = m_userDataDir.toStdString();
  path += "/log/terralib.log";

  log4cxx::FileAppender* fileAppender = new log4cxx::FileAppender(log4cxx::LayoutPtr(new log4cxx::SimpleLayout()),
    log4cxx::helpers::Transcoder::decode(path.c_str()), false);

  log4cxx::helpers::Pool p;
  fileAppender->activateOptions(p);

  log4cxx::BasicConfigurator::configure(log4cxx::AppenderPtr(fileAppender));
  log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getAll() /*log4cxx::Level::getDebug()*/);
  log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("te");
#endif
  //LOG4CXX_INFO(logger, "Created FileAppender appender");

  if(m_resetTerralib)
    TerraLib::getInstance().initialize();

  SplashScreenManager::getInstance().showMessage(tr("TerraLib Modules loaded!"));

  SplashScreenManager::getInstance().showMessage(tr("Loading the application configuration file..."));

  te::common::SystemApplicationSettings::getInstance().load(m_appConfigFile);

// general application info
  SplashScreenManager::getInstance().showMessage(tr("Application configuration file loaded!"));

  m_appName = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Name"));
  m_appTitle = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title"));
  //m_appProjectExtension = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.ProjectExtension"));
  m_appIconName = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconName"));
  
  if(!boost::filesystem::exists(m_appIconName.toStdString()))
    m_appIconName = te::common::FindInTerraLibPath(m_appIconName.toStdString()).c_str();

  m_appPluginsPath = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Plugins.<xmlattr>.xlink:href"));

  if (!boost::filesystem::exists(m_appPluginsPath.toStdString()))
    m_appPluginsPath = te::common::FindInTerraLibPath(m_appPluginsPath.toStdString()).c_str();

  m_aboutLogo = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.AboutDialogLogo.<xmlattr>.xlink:href"));
  
  if(!boost::filesystem::exists(m_aboutLogo.toStdString()))
    m_aboutLogo = te::common::FindInTerraLibPath(m_aboutLogo.toStdString()).c_str();
  
  m_tLibLogo = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.TerraLibLogo.<xmlattr>.xlink:href"));
  
  if(!boost::filesystem::exists(m_tLibLogo.toStdString()))
    m_tLibLogo = te::common::FindInTerraLibPath(m_tLibLogo.toStdString()).c_str();

  QString fullAppName = m_appName + "-" + QString(te::common::Version::asString().c_str());
  qApp->setApplicationName(fullAppName);

  m_appOrganization = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Organization"));

  qApp->setOrganizationName(m_appOrganization);

// load help system
  try
  {
    std::string help_file = te::common::FindInTerraLibPath(te::common::SystemApplicationSettings::getInstance().getValue("Application.HelpFile.<xmlattr>.xlink:href"));

    m_appHelpFile = QString::fromStdString(help_file);

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

    std::string icon_dir = te::common::FindInTerraLibPath(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href"));
    
    m_appIconThemeDir = QString::fromStdString(icon_dir);

    if(!m_appIconThemeDir.isEmpty())
    {
      QStringList ithemes = QIcon::themeSearchPaths();

      ithemes.push_back(m_appIconThemeDir);

      QIcon::setThemeSearchPaths(ithemes);
    }

    m_appDefaultIconTheme = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.DefaultTheme"));
    
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
    m_appDatasourcesFile = user_settings.value("data_sources/data_file", "").toString().toStdString();

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
        int reply = QMessageBox::question(0, tr("Data Sources XML"), tr("A file containing data sources already configured was found. Would you like to load it."), QMessageBox::No, QMessageBox::Yes);

        if (reply == QMessageBox::Yes)
        {
          std::string dataSourcesFile = fileName.toString().toStdString();

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

  std::vector<std::string> plgFiles;

  try
  {
    SplashScreenManager::getInstance().showMessage(tr("Reading application plugins list..."));

    std::vector<std::string> default_plg = GetDefaultPluginsNames(this);
    plgFiles = GetPluginsFiles();

    //SplashScreenManager::getInstance().showMessage(tr("Plugins list read!"));

    SplashScreenManager::getInstance().showMessage(tr("Checking enabled plugins..."));
    
    QSettings user_settings(QSettings::IniFormat,
                            QSettings::UserScope,
                            QApplication::instance()->organizationName(),
                            QApplication::instance()->applicationName());

    user_settings.beginGroup("plugins");

    std::set<std::string> user_enabled_plugins;

    int nitems = user_settings.beginReadArray("enabled");

    for(int i = 0; i != nitems; ++i)
    {
      user_settings.setArrayIndex(i);

      QString name = user_settings.value("name").toString();

      user_enabled_plugins.insert(name.toStdString());
    }

    user_settings.endArray();

    // get the unloaded plugins
    std::set<std::string> user_unloaded_plugins;
    int n_itemsUnloaded = user_settings.beginReadArray("unloaded");

    for (int i = 0; i != n_itemsUnloaded; ++i)
    {
      user_settings.setArrayIndex(i);

      QString name = user_settings.value("name").toString();

      user_unloaded_plugins.insert(name.toStdString());
    }

    user_settings.endArray();

    // get the broken plugins
    std::set<std::string> user_broken_plugins;
    int n_itemsBroken = user_settings.beginReadArray("broken");

    for (int i = 0; i != n_itemsBroken; ++i)
    {
      user_settings.setArrayIndex(i);

      QString name = user_settings.value("name").toString();

      user_broken_plugins.insert(name.toStdString());
    }

    user_settings.endArray();

    user_settings.endGroup();

    //SplashScreenManager::getInstance().showMessage(tr("Enabled plugin list read!"));

    SplashScreenManager::getInstance().showMessage(tr("Loading plugins..."));

// retrieve information for each plugin
    boost::ptr_vector<te::plugin::PluginInfo> plugins;
    boost::ptr_vector<te::plugin::PluginInfo> unloadedPlugins;
    boost::ptr_vector<te::plugin::PluginInfo> brokenPlugins;

    for(std::size_t i = 0; i != plgFiles.size(); ++i)
    {
      te::plugin::PluginInfo* pinfo = te::plugin::GetInstalledPlugin(plgFiles[i]);
      
      if (user_enabled_plugins.empty())                               // if there is no list of enabled plugins
      {
        if (default_plg.size() > 0)
        {
          if (std::find(default_plg.begin(), default_plg.end(), pinfo->m_name) != default_plg.end())
            plugins.push_back(pinfo);                                 // try to load all default plugins.
        }
        else
        {
          plugins.push_back(pinfo);                                   // try to load all plugins.
        }
      }
      else if (user_enabled_plugins.count(pinfo->m_name) != 0)        // else, if a list is available,
      {
        plugins.push_back(pinfo);                                     // load all enabled plugins using .ini file as reference.
      }
      else if (user_unloaded_plugins.count(pinfo->m_name) != 0)       // else, if a list is available,
      {
        unloadedPlugins.push_back(pinfo);                             // load only unloaded plugins
      }
      else if (user_broken_plugins.count(pinfo->m_name) != 0)         // else, if a list is available,
      {
        brokenPlugins.push_back(pinfo);                               // load only broken plugins
      }
    }
    
// load and start each plugin
    te::plugin::PluginManager::getInstance().load(plugins);

    if (user_unloaded_plugins.size() > 0)
      te::plugin::PluginManager::getInstance().setUnloadedPlugins(unloadedPlugins);

    if (user_broken_plugins.size() > 0)
      te::plugin::PluginManager::getInstance().setBrokenPlugins(brokenPlugins);

    SplashScreenManager::getInstance().showMessage(tr("Plugins loaded successfully!"));
  }
  catch(const std::exception& e)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Error reading application's plugin list: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
  }
}

//void te::qt::af::ApplicationController::initializeProjectMenus()
//{
//  SplashScreenManager::getInstance().showMessage("Loading recent projects...");
//
//  try
//  {
//    QSettings user_settings(QSettings::IniFormat,
//                            QSettings::UserScope,
//                            QApplication::instance()->organizationName(),
//                            QApplication::instance()->applicationName());
//
//    QVariant projPath = user_settings.value("projects/most_recent/path", "");
//    QVariant projTitle = user_settings.value("projects/most_recent/title", "");
//
//    QMenu* mnu = getMenu("File.Recent Projects");
//
//    if(!projPath.toString().isEmpty())
//    {
//      QAction* act = mnu->addAction(projPath.toString());
//      act->setData(projPath);
//
//      mnu->addSeparator();
//
//      m_recentProjs.append(projPath.toString());
//      m_recentProjsTitles.append(projTitle.toString());
//    }
//    
//    user_settings.beginGroup("projects");
//    
//    int nrc = user_settings.beginReadArray("recents");
//    
//    for(int i = 0; i != nrc; ++i)
//    {
//      user_settings.setArrayIndex(i);
//      QString npath = user_settings.value("projects/path").toString();
//      QString ntitle = user_settings.value("projects/title").toString();
//      
//      
//      QAction* act = mnu->addAction(npath);
//      act->setData(npath);
//      m_recentProjs.append(npath);
//      m_recentProjsTitles.append(ntitle);
//    }
//
//    mnu->setEnabled(true);
//
//    SplashScreenManager::getInstance().showMessage("Recent projects loaded!");
//  }
//  catch(const std::exception& e)
//  {
//    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);
//
//    QString msgErr(tr("Error loading the registered projects: %1"));
//
//    msgErr = msgErr.arg(e.what());
//
//    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
//  }
//}
//
//void te::qt::af::ApplicationController::updateRecentProjects(const QString& prjFile, const QString& prjTitle)
//{
//  int pos = m_recentProjs.indexOf(prjFile);
//
//  QString author;
//  int maxSaved;
//
//  GetProjectInformationsFromSettings(author, maxSaved);
//
//  if(pos != 0)
//  {
//    if(pos < 0)
//    {
//      if(m_recentProjs.size() > maxSaved) // TODO: Size of the list must be configurable.
//      {
//        m_recentProjs.removeLast();
//        m_recentProjsTitles.removeLast();
//      }
//
//      m_recentProjs.prepend(prjFile);
//      m_recentProjsTitles.prepend(prjTitle);
//    }
//    else
//    {
//      m_recentProjs.move(pos, 0);
//      m_recentProjsTitles.move(pos, 0);
//    }
//
//    if(m_recentProjs.isEmpty())
//      return;
//
//    QMenu* mnu = getMenu("File.Recent Projects");
//
//    mnu->clear();
//
//    mnu->setEnabled(true);
//
//    QString recPrj = m_recentProjs.at(0);
//    QAction* act = mnu->addAction(recPrj);
//    act->setData(recPrj);
//
//    mnu->addSeparator();
//
//    if(m_recentProjs.size() > 1)
//      for(int i=1; i<m_recentProjs.size(); i++)
//      {
//        recPrj = m_recentProjs.at(i);
//        act = mnu->addAction(recPrj);
//        act->setData(recPrj);
//      }
//  }
//
//  QAction* act = findAction("File.Save Project As");
//
//  if(act != 0)
//    act->setEnabled(true);
//}

//void te::qt::af::ApplicationController::set(te::qt::af::Project* prj)
//{
//  m_project = prj;
//}

//te::qt::af::Project* te::qt::af::ApplicationController::getProject()
//{
//  return m_project;
//}

void te::qt::af::ApplicationController::finalize()
{
  if(!m_initialized)
    return;

//  UpdateUserSettings(m_recentProjs, m_recentProjsTitles, m_appUserSettingsFile);

//  SaveDataSourcesFile();

  te::plugin::PluginManager::getInstance().shutdownAll();

  te::plugin::PluginManager::getInstance().unloadAll();

  te::plugin::PluginManager::getInstance().clear();

//  delete m_project;

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

  //m_appProjectExtension.clear();

  m_tLibLogo.clear();
          
  //m_recentProjs.clear();

  //m_recentProjsTitles.clear();

  m_appUserSettingsFile.clear();
  
  m_appPluginsPath.clear();
  
  m_appToolBarDefaultIconSize.clear();
  
  m_defaultSRID = 0;
  
  m_selectionColor = QColor();

//  m_project = 0;

  m_initialized = false;
}

QSettings& te::qt::af::ApplicationController::getSettings()
{
  return m_appSettings;
}

//void  te::qt::af::ApplicationController::broadcast(te::qt::af::evt::Event* evt)
//{
//  // Need to check event send to prevent loops
//  // -----------------------------------------

//  emit triggered(evt);
//}

const QString& te::qt::af::ApplicationController::getAppName() const
{
  return m_appName;
}

const QString& te::qt::af::ApplicationController::getAppTitle() const
{
  return m_appTitle;
}

//const QString& te::qt::af::ApplicationController::getAppProjectExtension() const
//{
//  return m_appProjectExtension;
//}

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

//QString te::qt::af::ApplicationController::getMostRecentProject() const
//{
//  return m_recentProjs.isEmpty() ? QString("") : m_recentProjs.front();
//}

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
