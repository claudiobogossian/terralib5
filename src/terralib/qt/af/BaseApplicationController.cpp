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
#include "../../serialization/dataaccess/DataSourceInfo.h"
#include "../widgets/help/AssistantHelpManagerImpl.h"
#include "../widgets/help/HelpManager.h"
#include "../widgets/Utils.h"
#include "../widgets/utils/ScopedCursor.h"
#include "events/NewToolBar.h"
#include "ApplicationController.h"
#include "ApplicationPlugins.h"
#include "BaseApplicationController.h"
#include "Exception.h"
#include "SplashScreenManager.h"
#include "UserPlugins.h"
#include "Project.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QResource>
#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QWidget>
#include <QtGui/QMenu>

// Boost
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>


void updateUserSettingsProjects(const QStringList& prj_files, const QStringList& prj_titles, const std::string& userConfigFile)
{
  if(prj_files.empty())
    return;

  boost::property_tree::ptree p = te::common::UserApplicationSettings::getInstance().getAllSettings();

  p.get_child("UserSettings.MostRecentProject.<xmlattr>.xlink:href").put_value(prj_files.at(0).toStdString());
  p.get_child("UserSettings.MostRecentProject.<xmlattr>.title").put_value(prj_titles.at(0).toStdString());

  if(prj_files.size() > 1)
  {
    boost::property_tree::ptree rec_prjs;

    for(int i=1; i<prj_files.size(); i++)
    {
      boost::property_tree::ptree prj;

      prj.add("<xmlattr>.xlink:href", prj_files.at(i).toStdString());
      prj.add("<xmlattr>.title", prj_titles.at(i).toStdString());

      rec_prjs.add_child("Project", prj);
    }

    p.put_child("UserSettings.RecentProjects", rec_prjs);
  }

  boost::property_tree::xml_writer_settings<char> settings('\t', 1);
  boost::property_tree::write_xml(userConfigFile, p, std::locale(), settings);
}

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

void te::qt::af::BaseApplicationController::registerMenu(QMenu* mnu)
{
  m_menus.push_back(mnu);
}

QMenu* te::qt::af::BaseApplicationController::findMenu(const QString& id) const
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

QMenu* te::qt::af::BaseApplicationController::getMenu(const QString& id)
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

void te::qt::af::BaseApplicationController::registerMenuBar(QMenuBar* bar)
{
  m_menuBars.push_back(bar);
}

QMenuBar* te::qt::af::BaseApplicationController::findMenuBar(const QString& id) const
{
  throw std::exception("Not implemented yet.");
}

QMenuBar* te::qt::af::BaseApplicationController::getMenuBar(const QString& id) const
{
  throw std::exception("Not implemented yet.");
}

QAction* te::qt::af::BaseApplicationController::findAction(const QString& id) const
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
  m_appTitle = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title"));
  m_appIconName = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.IconName"));

  qApp->setApplicationName(m_appName);

  m_appOrganization = QString::fromStdString(te::common::SystemApplicationSettings::getInstance().getValue("Application.Organization"));

  qApp->setOrganizationName(m_appOrganization);

// read used config data
  SplashScreenManager::getInstance().showMessage(tr("Reading user settings..."));

  m_appUserSettingsFile = te::common::SystemApplicationSettings::getInstance().getValue("Application.UserSettingsFile.<xmlattr>.xlink:href");

  te::common::UserApplicationSettings::getInstance().load(m_appUserSettingsFile);

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

    std::string iconSize = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.ToolBarIconSize");

    if(iconSize.empty())
      iconSize = te::common::SystemApplicationSettings::getInstance().getValue("Application.ToolBarDefaultIconSize");
    if(!iconSize.empty())
    {
      QString sh = QString("QToolBar { qproperty-iconSize: ") + iconSize.c_str() + "px " + iconSize.c_str() + "px; }";
      qApp->setStyleSheet(sh);
    }

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

      te::serialize::ReadDataSourceInfo(m_appDatasourcesFile);

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

  m_initialized = true;
//// load recent projects
//  UserRecentProjects::getInstance().load();
//
//  splashScreen.showMessage("Recent projects loaded!", Qt::AlignBottom | Qt::AlignHCenter, Qt::white);
//  QCoreApplication::processEvents();
//
//// open most recent opened project
//  std::string mostRecentProjectFile = UserApplicationSettings::getInstance().getValue("UserSettings.MostRecentProject.<xmlattr>.xlink:href");
//  sm_instance->fileOpenProject(mostRecentProjectFile.c_str());
//
//  splashScreen.showMessage("Most recent project loaded!", Qt::AlignBottom | Qt::AlignHCenter, Qt::white);
//  QCoreApplication::processEvents();

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

    SplashScreenManager::getInstance().close();

    QString msgErr(tr("Some plugins couldn't be loaded: %1.\n "
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

void te::qt::af::BaseApplicationController::initializeProjectMenus()
{
  SplashScreenManager::getInstance().showMessage("Loading recent projects...");

  try
  {
    boost::property_tree::ptree p = te::common::UserApplicationSettings::getInstance().getAllSettings().get_child("UserSettings");
    std::string proj_path, proj_title;

    bool hasProjects = p.count("MostRecentProject") > 0;

    if(hasProjects)
    {
      proj_path = p.get<std::string>("MostRecentProject.<xmlattr>.xlink:href");
      proj_title = p.get<std::string>("MostRecentProject.<xmlattr>.title");
    }

    QMenu* mnu = getMenu("File.Recent Projects");

    if(!proj_path.empty())
    {
      QString pp = proj_path.c_str();
      QAction* act = mnu->addAction(pp);
      act->setData(pp);

      mnu->addSeparator();

      m_recent_projs.append(pp);
      m_recent_projs_titles.append(proj_title.c_str());
    }

    hasProjects = p.count("RecentProjects") > 0;

    if(hasProjects)
    {
      BOOST_FOREACH(boost::property_tree::ptree::value_type& v, p.get_child("RecentProjects"))
      {
        QString pp = v.second.get<std::string>("<xmlattr>.xlink:href").c_str();
        QString pt = v.second.get<std::string>("<xmlattr>.title").c_str();
        QAction* act = mnu->addAction(pp);
        act->setData(pp);
        m_recent_projs.append(pp);
        m_recent_projs_titles.append(pt);
      }
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

void te::qt::af::BaseApplicationController::updateRecentProjects(const QString& prj_file, const QString& prj_title)
{
  int pos = m_recent_projs.indexOf(prj_file);

  if(pos != 0)
  {
    if(pos < 0)
    {
      if(m_recent_projs.size() >= 10) // TODO: Size of the list must be configurable.
      {
        m_recent_projs.removeLast();
        m_recent_projs_titles.removeLast();
      }

      m_recent_projs.prepend(prj_file);
      m_recent_projs_titles.prepend(prj_title);
    }
    else
    {
      m_recent_projs.move(pos, 0);
      m_recent_projs_titles.move(pos, 0);
    }

    QMenu* mnu = getMenu("File.Recent Projects");

    mnu->clear();

    QString rec_prj = m_recent_projs.at(0);
    QAction* act = mnu->addAction(rec_prj);
    act->setData(rec_prj);

    mnu->addSeparator();

    if(m_recent_projs.size() > 1)
      for(int i=1; i<m_recent_projs.size(); i++)
      {
        rec_prj = m_recent_projs.at(i);
        act = mnu->addAction(rec_prj);
        act->setData(rec_prj);
      }
  }

  QAction* act = findAction("File.Save Project As");

  if(act != 0)
    act->setEnabled(true);
}

void te::qt::af::BaseApplicationController::finalize()
{
  if(!m_initialized)
    return;

  updateUserSettingsProjects(m_recent_projs, m_recent_projs_titles, m_appUserSettingsFile);
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
