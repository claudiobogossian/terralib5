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
  \file terralib/qt/af/ApplicationController.cpp

  \brief The base API for controllers of TerraLib applications.
*/

// Boost
#include <boost/foreach.hpp> // Boost => don't change this include order, otherwise you may have compiling problems! 

// TerraLib
#include "../../common/Exception.h"
#include "../../common/PlatformUtils.h"
#include "../../common/Translator.h"
#include "../../common/TerraLib.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/UserApplicationSettings.h"
#include "../../common/Logger.h"
#include "../../dataaccess/serialization/xml/Serializer.h"
#include "../../plugin/PluginManager.h"
#include "../../plugin/PluginInfo.h"
#include "../../srs/Config.h"
#include "../widgets/help/AssistantHelpManagerImpl.h"
#include "../widgets/help/HelpManager.h"
#include "../widgets/Utils.h"
#include "../widgets/utils/ScopedCursor.h"
#include "events/ApplicationEvents.h"
#include "ApplicationController.h"
#include "ApplicationPlugins.h"
#include "Exception.h"
#include "Project.h"
#include "SplashScreenManager.h"
#include "UserPlugins.h"
#include "Utils.h"

// Qt
#include <QDir>
#include <QResource>
#include <QApplication>
#include <QIcon>
#include <QMessageBox>
#include <QWidget>
#include <QMenu>

// Boost
#include <boost/filesystem.hpp>

//te::qt::af::ApplicationController* te::qt::af::ApplicationController::sm_instance(0);

te::qt::af::ApplicationController::ApplicationController(/*QObject* parent*/)
  : QObject(/*parent*/),
    m_msgBoxParentWidget(0),
    m_defaultSRID(TE_UNKNOWN_SRS),
    m_selectionColor(QColor(0, 255, 0)),
    m_initialized(false),
    m_project(0),
    m_resetTerralib(true)
{
  //if(sm_instance)
  //  throw Exception(TE_TR("Can not start another instance of the TerraLib Application Controller!"));

  //sm_instance = this;
}

te::qt::af::ApplicationController::~ApplicationController()
{
  finalize();

//  sm_instance = 0;
}

//te::qt::af::ApplicationController& te::qt::af::ApplicationController::getInstance()
//{
//  return *sm_instance;
//}

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

  broadcast(&evt);
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

void te::qt::af::ApplicationController::addListener(QObject* obj)
{
  std::set<QObject*>::const_iterator it = m_applicationItems.find(obj);

  if(it != m_applicationItems.end())
    return;

  m_applicationItems.insert(obj);

  obj->connect(this, SIGNAL(triggered(te::qt::af::evt::Event*)), SLOT(onApplicationTriggered(te::qt::af::evt::Event*)));
}

void te::qt::af::ApplicationController::removeListener(QObject* obj)
{
  std::set<QObject*>::iterator it = m_applicationItems.find(obj);

  if(it == m_applicationItems.end())
    return;

  m_applicationItems.erase(it);

  disconnect(SIGNAL(triggered(te::qt::af::evt::Event*)), obj, SLOT(onApplicationTriggered(te::qt::af::evt::Event*)));
}

void  te::qt::af::ApplicationController::initialize()
{
  if(m_initialized)
    return;

  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  SplashScreenManager::getInstance().showMessage(tr("Loading TerraLib Modules..."));

  if(m_resetTerralib)
    TerraLib::getInstance().initialize();

  SplashScreenManager::getInstance().showMessage(tr("TerraLib Modules loaded!"));

  SplashScreenManager::getInstance().showMessage(tr("Loading the application configuration file..."));

  te::common::SystemApplicationSettings::getInstance().load(m_appConfigFile);

// general application info
  SplashScreenManager::getInstance().showMessage(tr("Application configuration file loaded!"));

  m_appName = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Name"));
  m_appTitle = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title"));
  m_appIconName = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconName"));
  m_aboutLogo = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.AboutDialogLogo.<xmlattr>.xlink:href"));
  m_tLibLogo = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.TerraLibLogo.<xmlattr>.xlink:href"));

  qApp->setApplicationName(m_appName);

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

    QString iconTheme = QString::fromStdString(te::common::UserApplicationSettings::getInstance().getValue("UserSettings.SelectedIconTheme"));

    if(iconTheme.isEmpty())
      QIcon::setThemeName(m_appDefaultIconTheme);
    else
      QIcon::setThemeName(iconTheme);

    std::string iconSize = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.ToolBarIconSize");

    if(iconSize.empty())
      iconSize = te::common::SystemApplicationSettings::getInstance().getValue("Application.ToolBarDefaultIconSize");
    if(!iconSize.empty())
    {
      QString sh = QString("QToolBar { qproperty-iconSize: ") + iconSize.c_str() + "px " + iconSize.c_str() + "px; }";
      qApp->setStyleSheet(sh);
    }

    // Default SRID
    QString srid = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.DefaultSRID").c_str();
    if(srid.isEmpty())
      srid = te::common::SystemApplicationSettings::getInstance().getValue("Application.DefaultSRID").c_str();

    if(!srid.isEmpty())
      m_defaultSRID = srid.toInt();

    // Selection Color
    QString selectionColor = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.SelectionColor").c_str();
    if(selectionColor.isEmpty())
      selectionColor = te::common::SystemApplicationSettings::getInstance().getValue("Application.DefaultSelectionColor").c_str();

    if(!selectionColor.isEmpty())
      m_selectionColor = QColor(selectionColor);

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
    m_appDatasourcesFile = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.DataSourcesFile");

    if(!m_appDatasourcesFile.empty())
    {
      SplashScreenManager::getInstance().showMessage(tr("Loading user registered data sources..."));

      te::serialize::xml::ReadDataSourceInfo(m_appDatasourcesFile);

      SplashScreenManager::getInstance().showMessage(tr("Known data sources loaded!"));
    }
  }
  catch(const std::exception& e)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Error loading the registered data sources: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
  }

  QSettings s(QSettings::IniFormat, QSettings::UserScope, m_appOrganization, m_appName);

  QFileInfo info(s.fileName());

//  if(!info.exists())
//    CreateDefaultSettings();

  m_initialized = true;
}

void te::qt::af::ApplicationController::initializePlugins()
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

    SplashScreenManager::getInstance().close();

    QString msgErr(tr("Some plugins couldn't be loaded. %1\n\n"
                      "Please, refer to plugin manager to fix the problem."));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);

    //const std::vector<te::plugin::PluginInfo*>& bps = te::plugin::PluginManager::getInstance().getBrokenPlugins();

    //for(int kk = 0; kk < bps.size(); ++kk)
    //{
    //  QMessageBox::warning(sm_instance,
    //                     tr(SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
    //                     tr(bps[kk]->m_name.c_str()));
    //}
  }
}

void te::qt::af::ApplicationController::initializeProjectMenus()
{
  SplashScreenManager::getInstance().showMessage("Loading recent projects...");

  try
  {
    boost::property_tree::ptree p = te::common::UserApplicationSettings::getInstance().getAllSettings().get_child("UserSettings");
    std::string projPath, projTitle;

    projPath = p.get<std::string>("MostRecentProject.<xmlattr>.xlink:href");
    projTitle = p.get<std::string>("MostRecentProject.<xmlattr>.title");

    QMenu* mnu = getMenu("File.Recent Projects");

    if(!projPath.empty())
    {
      QString pp = projPath.c_str();
      QAction* act = mnu->addAction(pp);
      act->setData(pp);

      mnu->addSeparator();

      m_recentProjs.append(pp);
      m_recentProjsTitles.append(projTitle.c_str());
    }

    bool hasProjects = p.count("RecentProjects") > 0;

    if(hasProjects)
    {
      BOOST_FOREACH(boost::property_tree::ptree::value_type& v, p.get_child("RecentProjects"))
      {
        QString pp = v.second.get<std::string>("<xmlattr>.xlink:href").c_str();
        QString pt = v.second.get<std::string>("<xmlattr>.title").c_str();
        QAction* act = mnu->addAction(pp);
        act->setData(pp);
        m_recentProjs.append(pp);
        m_recentProjsTitles.append(pt);
      }

      mnu->setEnabled(true);
    }

    SplashScreenManager::getInstance().showMessage("Recent projects loaded!");
  }
  catch(const std::exception& e)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QString msgErr(tr("Error loading the registered projects: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(m_msgBoxParentWidget, m_appTitle, msgErr);
  }
}

void te::qt::af::ApplicationController::updateRecentProjects(const QString& prjFile, const QString& prjTitle)
{
  int pos = m_recentProjs.indexOf(prjFile);

  QString author;
  int maxSaved;

  GetProjectInformationsFromSettings(author, maxSaved);

  if(pos != 0)
  {
    if(pos < 0)
    {
      if(m_recentProjs.size() >= maxSaved) // TODO: Size of the list must be configurable.
      {
        m_recentProjs.removeLast();
        m_recentProjsTitles.removeLast();
      }

      m_recentProjs.prepend(prjFile);
      m_recentProjsTitles.prepend(prjTitle);
    }
    else
    {
      m_recentProjs.move(pos, 0);
      m_recentProjsTitles.move(pos, 0);
    }

    if(m_recentProjs.isEmpty())
      return;

    QMenu* mnu = getMenu("File.Recent Projects");

    mnu->clear();

    mnu->setEnabled(true);

    QString recPrj = m_recentProjs.at(0);
    QAction* act = mnu->addAction(recPrj);
    act->setData(recPrj);

    mnu->addSeparator();

    if(m_recentProjs.size() > 1)
      for(int i=1; i<m_recentProjs.size(); i++)
      {
        recPrj = m_recentProjs.at(i);
        act = mnu->addAction(recPrj);
        act->setData(recPrj);
      }
  }

  QAction* act = findAction("File.Save Project As");

  if(act != 0)
    act->setEnabled(true);
}

void te::qt::af::ApplicationController::set(te::qt::af::Project* prj)
{
  m_project = prj;
}

te::qt::af::Project* te::qt::af::ApplicationController::getProject()
{
  return m_project;
}

void te::qt::af::ApplicationController::finalize()
{
  if(!m_initialized)
    return;

  te::common::SystemApplicationSettings::getInstance().update();

  UpdateApplicationPlugins();

  UpdateUserSettings(m_recentProjs, m_recentProjsTitles, m_appUserSettingsFile);

  SaveDataSourcesFile();

  te::plugin::PluginManager::getInstance().shutdownAll();

  te::plugin::PluginManager::getInstance().unloadAll();

  te::plugin::PluginManager::getInstance().clear();

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
          
  m_recentProjs.clear();

  m_recentProjsTitles.clear();

  m_appUserSettingsFile.clear();
  
  m_appPluginsFile.clear();
  
  m_appToolBarDefaultIconSize.clear();
  
  m_defaultSRID = 0;
  
  m_selectionColor = QColor();

  m_project = 0;
  
  m_initialized = false;
}

QSettings& te::qt::af::ApplicationController::getSettings()
{
  return m_appSettings;
}

void  te::qt::af::ApplicationController::broadcast(te::qt::af::evt::Event* evt)
{
  // Need to check event send to prevent loops
  // -----------------------------------------

  emit triggered(evt);
}

const QString& te::qt::af::ApplicationController::getAppTitle() const
{
  return m_appTitle;
}

const QString& te::qt::af::ApplicationController::getAppIconName() const
{
  return m_appIconName;
}

const QString& te::qt::af::ApplicationController::getAboutLogo() const
{
  return m_aboutLogo;
}

const QString& te::qt::af::ApplicationController::getTlibLogo() const
{
  return m_tLibLogo;
}

QString te::qt::af::ApplicationController::getMostRecentProject() const
{
  return m_recentProjs.isEmpty() ? QString("") : m_recentProjs.front();
}

int te::qt::af::ApplicationController::getDefaultSRID() const
{
  return m_defaultSRID;
}

QColor te::qt::af::ApplicationController::getSelectionColor() const
{
  return m_selectionColor;
}

QWidget* te::qt::af::ApplicationController::getMainWindow() const
{
  return m_msgBoxParentWidget;
}

void te::qt::af::ApplicationController::setResetTerraLibFlag(const bool& status)
{
  m_resetTerralib = status;
}
