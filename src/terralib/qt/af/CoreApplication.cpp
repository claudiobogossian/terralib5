/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/CoreApplication.cpp

  \brief The application is a singleton.
*/

// don't change this include order, otherwise
// some boost versions can punish you!
// see: (https://svn.boost.org/trac/boost/ticket/6455)!

// TerraLib
#include "CoreApplication.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/TerraLib.h"
#include "../../common/Translator.h"
#include "../../common/UserApplicationSettings.h"
#include "../../plugin/PluginManager.h"
#include "../../serialization/qt/widgets/DataSource.h"
#include "../widgets/plugin/builder/PluginBuilderWizard.h"
#include "../widgets/plugin/manager/PluginManagerDialog.h"
#include "../widgets/utils/ScopedCursor.h"
#include "events/AppClose.h"
#include "events/Event.h"
#include "ApplicationPlugins.h"
#include "Exception.h"
#include "SplashScreenManager.h"
#include "UserPlugins.h"
#include "UserRecentProjects.h"

// STL
#include <algorithm>

// Boost
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
//#include <boost/thread.hpp>

// Qt
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QCloseEvent>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QSplashScreen>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>

te::qt::af::CoreApplication* te::qt::af::CoreApplication::sm_instance(0);

te::qt::af::CoreApplication::CoreApplication()
  : QMainWindow(0),
    m_viewToolBarsMenu(0)
{
  if(sm_instance)
    throw Exception(TR_QT_AF("Can not start another instance of the TerraLib Application Framework!"));

  sm_instance = this;

  initialize();
}

te::qt::af::CoreApplication::~CoreApplication()
{
  finalize();

  sm_instance = 0;
}

te::qt::af::CoreApplication* te::qt::af::CoreApplication::getInstance()
{
  return sm_instance;
}

QMenu* te::qt::af::CoreApplication::addMenu(const QString& name, const QString& title)
{
  QMenu* menu = getMenu(name);

  if(menu != 0)
  {
    QString errmsg(tr("There is already a menu registered with the name %1."));

    errmsg = errmsg.arg(name);

    throw Exception(errmsg.toStdString());
  }

  menu = menuBar()->addMenu(title);
  menu->setObjectName(name);

  return menu;
}

QMenu* te::qt::af::CoreApplication::getMenu(const QString& name) const
{
  QStringList l = name.split(".", QString::SkipEmptyParts);

  if(l.empty())
    return 0;

  QStringList::const_iterator it = l.begin();

  QMenuBar* mb = menuBar();

  QMenu* menu = mb->findChild<QMenu*>(*it);

  ++it;

  for(; it != l.end(); ++it)
  {
    menu = menu->findChild<QMenu*>(*it);

    if(menu == 0)
      return 0;
  }

  return menu;

  //std::map<QString, QMenu*>::const_iterator it = m_menus.find(name);

  //return (it != m_menus.end()) ? it->second : 0;
}

QToolBar* te::qt::af::CoreApplication::addToolBar(const QString& name, const QString& title)
{
  QToolBar* toolbar = getToolBar(name);

  if(toolbar != 0)
  {
    QString errmsg(tr("There is already a toolbar registered with the name %1."));

    errmsg = errmsg.arg(name);

    throw Exception(errmsg.toStdString());
  }

  toolbar = QMainWindow::addToolBar(title);

  if(m_viewToolBarsMenu != 0)
    m_viewToolBarsMenu->addAction(toolbar->toggleViewAction());

  m_toolbars[name] = toolbar;

  return toolbar;
}

QToolBar* te::qt::af::CoreApplication::getToolBar(const QString& name) const
{
  std::map<QString, QToolBar*>::const_iterator it = m_toolbars.find(name);

  return (it != m_toolbars.end()) ? it->second : 0;
}

void te::qt::af::CoreApplication::addItem(const QString& id, QObject* appItem)
{
  std::map<QString, QObjectList>::const_iterator it = m_appItems.find(id);

  if(it != m_appItems.end())
  {
    QObjectList::const_iterator appIt = std::find(it->second.begin(), it->second.end(), appItem);

    if(appIt != it->second.end())
      throw Exception((boost::format(TR_QT_AF("Can not add application item under the same id %1%")) % id.toStdString()).str());
  }

  connect(this, SIGNAL(triggered(Event*)), appItem, SLOT(onApplicationTriggered(Event*)), Qt::UniqueConnection);

  m_appItems[id].push_back(appItem);

  emit appItemAdded(id, appItem);
}

QObjectList te::qt::af::CoreApplication::getAppItemList(const QString& id) const
{
  std::map<QString, QObjectList>::const_iterator it = m_appItems.find(id);

  return (it != m_appItems.end()) ? it->second : QObjectList();
}

void te::qt::af::CoreApplication::fileNewProject()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::fileOpenProject()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::fileOpenProject(QAction* /*action*/)
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::fileOpenProject(const QString& /*fileName*/)
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::fileSaveProject()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::fileSaveAsProject()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::fileExit()
{
  close();
}

void te::qt::af::CoreApplication::editUndo()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::editRedo()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::editCut()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::editCopy()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::editPaste()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::editSelectAll()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::editClear()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::editFind()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::editReplace()
{
  QMessageBox::warning(this,
                       tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                       tr("This option is not implemented yet! We will provide it soon!"));
}

void te::qt::af::CoreApplication::viewFullScreen(bool checked)
{
  if(checked)
  {
    menuBar()->hide();
    
    showFullScreen();

    if(statusBar())
      statusBar()->hide();

    if(!m_viewFullScreenToolBar->isVisible())
      m_viewFullScreenToolBar->show();
  }
  else
  {
    menuBar()->show();

    showNormal();

    if(statusBar())
      statusBar()->show();

    m_viewFullScreenToolBar->hide();
  }
}

void te::qt::af::CoreApplication::viewRefresh()
{
  emit(refresh());
}

void te::qt::af::CoreApplication::pluginManagement()
{
  std::auto_ptr<te::qt::widgets::PluginManagerDialog> pm(new te::qt::widgets::PluginManagerDialog(this));

  pm->setModal(true);

  pm->exec();
}

void te::qt::af::CoreApplication::pluginBuilder()
{
  std::auto_ptr<te::qt::widgets::PluginBuilderWizard> pbw(new te::qt::widgets::PluginBuilderWizard(this));

  pbw->setModal(true);

  pbw->exec();
}

void te::qt::af::CoreApplication::initialize()
{
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  SplashScreenManager::getInstance().showMessage(tr("Loading TerraLib Modules..."));

  TerraLib::getInstance().initialize();
  SplashScreenManager::getInstance().showMessage(tr("TerraLib modules initialized!"));

  te::common::SystemApplicationSettings::getInstance().load();
  SplashScreenManager::getInstance().showMessage(tr("System application settings loaded!"));

  te::common::UserApplicationSettings::getInstance().load();
  SplashScreenManager::getInstance().showMessage(tr("User application settings loaded!"));

  ApplicationPlugins::getInstance().load();
  SplashScreenManager::getInstance().showMessage(tr("Application plugin list loaded!"));

  UserPlugins::getInstance();

// add icon theme path
  QStringList ithemes = QIcon::themeSearchPaths();

  std::string ithemedir = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href");

  ithemes.push_back(ithemedir.c_str());

  QIcon::setThemeSearchPaths(ithemes);

// set default icon theme
  std::string defaultTheme = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.DefaultTheme");

  QIcon::setThemeName(defaultTheme.c_str());

// see if user settings has another icon theme as default
  defaultTheme = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.SelectedIconTheme");

  QIcon::setThemeName(defaultTheme.c_str());

  SplashScreenManager::getInstance().showMessage(tr("Icon Theme set!"));

// global application settings
  std::string app_org = te::common::SystemApplicationSettings::getInstance().getValue("Application.Organization");
  QCoreApplication::setOrganizationName(app_org.c_str());

  std::string app_name = te::common::SystemApplicationSettings::getInstance().getValue("Application.Name");
  QCoreApplication::setApplicationName(QString::fromStdString(app_name));

  std::string appTitle = te::common::SystemApplicationSettings::getInstance().getValue("Application.Title");
  setWindowTitle(QString::fromStdString(appTitle));

  std::string appIcon = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconName");
  setWindowIcon(QIcon::fromTheme(appIcon.c_str()));

// config tool bar
  std::string toolBarSize = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.ToolBarIconSize");
  setIconSize(QSize(boost::lexical_cast<int>(toolBarSize), boost::lexical_cast<int>(toolBarSize)));

  //boost::this_thread::sleep(boost::posix_time::seconds(5));

// config dock-widgets area
  //setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
  //setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
  //setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
  //setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

// create statusbar
  createStatusBar();
  SplashScreenManager::getInstance().showMessage(tr("Status bar created!"));

// create actions, menus and toolbars -> tip: keep this order
  createActions();
  SplashScreenManager::getInstance().showMessage(tr("Application actions created!"));

  createMenus();
  SplashScreenManager::getInstance().showMessage(tr("Application menus created!"));

  createToolBars();
  SplashScreenManager::getInstance().showMessage(tr("Application toolbars created!"));

// connect signals and slots

// File
  connect(m_fileNewProject, SIGNAL(triggered()), this, SLOT(fileNewProject()));
  connect(m_fileOpenProject, SIGNAL(triggered()), this, SLOT(fileOpenProject()));
  connect(m_fileSaveProject, SIGNAL(triggered()), this, SLOT(fileSaveProject()));
  connect(m_fileSaveAsProject, SIGNAL(triggered()), this, SLOT(fileSaveAsProject()));
  connect(m_fileRecentProjects, SIGNAL(triggered(QAction*)), this, SLOT(fileOpenProject(QAction*)));
  connect(m_fileExit, SIGNAL(triggered()), this, SLOT(fileExit()));

// Edit
  connect(m_editUndo, SIGNAL(triggered()), this, SLOT(editUndo()));
  connect(m_editRedo, SIGNAL(triggered()), this, SLOT(editRedo()));
  connect(m_editCut, SIGNAL(triggered()), this, SLOT(editCut()));
  connect(m_editCopy, SIGNAL(triggered()), this, SLOT(editCopy()));
  connect(m_editPaste, SIGNAL(triggered()), this, SLOT(editPaste()));
  connect(m_editSelectAll, SIGNAL(triggered()), this, SLOT(editSelectAll()));
  connect(m_editClear, SIGNAL(triggered()), this, SLOT(editClear()));
  connect(m_editFind, SIGNAL(triggered()), this, SLOT(editFind()));
  connect(m_editReplace, SIGNAL(triggered()), this, SLOT(editReplace()));

// View
  connect(m_viewFullScreen, SIGNAL(toggled(bool)), this, SLOT(viewFullScreen(bool)));
  connect(m_viewRefresh, SIGNAL(triggered()), this, SLOT(viewRefresh()));

// Settings
  connect(m_iconThemes, SIGNAL(triggered(QAction*)), this, SLOT(setIconTheme(QAction*)));

// Plugins
  connect(m_pluginManager, SIGNAL(triggered()), this, SLOT(pluginManagement()));
  connect(m_pluginBuilder, SIGNAL(triggered()), this, SLOT(pluginBuilder()));

  SplashScreenManager::getInstance().showMessage(tr("Application signals/slots connected!"));

// load plugins
  try
  {
    UserPlugins::getInstance().load();
  }
  catch(const std::exception& /*e*/)
  {
    te::qt::widgets::ScopedCursor acursor(Qt::ArrowCursor);

    QMessageBox::warning(this,
                         tr(te::common::SystemApplicationSettings::getInstance().getValue("Application.Title").c_str()),
                         tr("Some plugins couldn't be loaded! Please, refer to plugin manager to fix the problem."));
  }

  SplashScreenManager::getInstance().showMessage(tr("Plugins loaded!"));

// load registered data sources
  std::string datasourcesFile = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.DataSourcesFile");
  te::serialize::ReadDataSource(datasourcesFile);
  SplashScreenManager::getInstance().showMessage(tr("Known data sources loaded!"));

// load recent projects
  UserRecentProjects::getInstance().load();
  SplashScreenManager::getInstance().showMessage(tr("Recent projects loaded!"));


// open most recent opened project
  std::string mostRecentProjectFile = te::common::UserApplicationSettings::getInstance().getValue("UserSettings.MostRecentProject.<xmlattr>.xlink:href");
  fileOpenProject(mostRecentProjectFile.c_str());
  SplashScreenManager::getInstance().showMessage(tr("Most recent project loaded!"));

// show maximized
  sm_instance->showMaximized();

  SplashScreenManager::getInstance().showMessage(tr("Application core ready!"));
}

void te::qt::af::CoreApplication::finalize()
{
// WARNING:
// 1. don't change this order
// 2. every plugin must make its cleanup before the application go away

  te::plugin::PluginManager::getInstance().shutdownAll();

  te::plugin::PluginManager::getInstance().unloadAll();

  TerraLib::getInstance().finalize();

  sm_instance = 0;
}

void te::qt::af::CoreApplication::createActions()
{
// File
  m_fileNewProject = new QAction(QIcon::fromTheme("document-new"), tr("&New Project"), this);
  m_fileNewProject->setShortcuts(QKeySequence::New);

  m_fileOpenProject = new QAction(QIcon::fromTheme("document-open"), tr("&Open Project..."), this);
  m_fileOpenProject->setShortcuts(QKeySequence::Open);

  m_fileSaveProject = new QAction(QIcon::fromTheme("document-save"), tr("&Save Project"), this);
  m_fileSaveProject->setShortcuts(QKeySequence::Save);

  m_fileSaveAsProject = new QAction(QIcon::fromTheme("document-save-as"), tr("Save Project &As..."), this);
  m_fileSaveAsProject->setObjectName("save_project_as");
  m_fileSaveAsProject->setShortcuts(QKeySequence::SaveAs);

  m_fileImportProject = new QAction(QIcon::fromTheme("document-import"), tr("&Import Project..."), this);
  m_fileImportProject->setEnabled(false);

  m_fileExportProject = new QAction(QIcon::fromTheme("document-export"), tr("&Export Project..."), this);
  m_fileExportProject->setEnabled(false);

  m_filePrint = new QAction(QIcon::fromTheme("document-print"), tr("&Print..."), this);
  m_filePrint->setShortcuts(QKeySequence::Print);
  m_filePrint->setEnabled(false);

  m_filePrintPreview = new QAction(QIcon::fromTheme("document-print-preview"), tr("Print Pre&view..."), this);
  m_filePrintPreview->setEnabled(false);

  m_fileRecentProjects = new QActionGroup(this);
  m_fileRecentProjects->setObjectName("file.recent_projects");
  m_fileRecentProjects->setExclusive(true);

  m_fileExit = new QAction(QIcon::fromTheme("system-log-out"), tr("E&xit"), this);
  m_fileExit->setShortcuts(QKeySequence::Quit);

// Edit
  m_editUndo = new QAction(QIcon::fromTheme("edit-undo"), tr("&Undo"), this);
  m_editUndo->setShortcut(QKeySequence::Undo);
  m_editUndo->setEnabled(false);

  m_editRedo = new QAction(QIcon::fromTheme("edit-redo"), tr("&Redo"), this);
  m_editRedo->setShortcut(QKeySequence::Redo);
  m_editRedo->setEnabled(false);

  m_editCut = new QAction(QIcon::fromTheme("edit-cut"), tr("Cu&t"), this);
  m_editCut->setShortcut(QKeySequence::Cut);
  m_editCut->setEnabled(false);

  m_editCopy = new QAction(QIcon::fromTheme("edit-copy"), tr("&Copy"), this);
  m_editCopy->setShortcut(QKeySequence::Copy);
  m_editCopy->setEnabled(false);

  m_editPaste = new QAction(QIcon::fromTheme("edit-paste"), tr("&Paste"), this);
  m_editPaste->setShortcut(QKeySequence::Paste);
  m_editPaste->setEnabled(false);

  m_editSelectAll = new QAction(QIcon::fromTheme("edit-select-all"), tr("Select &All"), this);
  m_editSelectAll->setShortcut(QKeySequence::SelectAll);
  m_editSelectAll->setEnabled(false);

  m_editClear = new QAction(QIcon::fromTheme("edit-clear"), tr("C&lear"), this);
  m_editClear->setEnabled(false);

  m_editFind = new QAction(QIcon::fromTheme("edit-find"), tr("&Find..."), this);
  m_editFind->setShortcut(QKeySequence::Find);
  m_editFind->setEnabled(false);

  m_editReplace = new QAction(QIcon::fromTheme("edit-find-replace"), tr("R&eplace..."), this);
  m_editReplace->setShortcut(QKeySequence::Replace);
  m_editReplace->setEnabled(false);

// View
  //usar o metodo toggleViewAction do layer explorer para colocar este cara nomenu
  //m_viewLayerExplorer = new QAction(QIcon::fromTheme("view-layer-explorer"), tr("&Layer Explorer"), this);
  //m_viewLayerExplorer->setCheckable(true);
  //m_viewLayerExplorer->setChecked(false);

  //m_viewMapDisplay = new QAction(QIcon::fromTheme("view-map-display"), tr("&Map Display"), this);
  //m_viewMapDisplay->setCheckable(true);
  //m_viewMapDisplay->setChecked(false);

  //m_viewZoomIn = new QAction(QIcon::fromTheme("zoom-in"), tr("Zoom &In"), this);
  //m_viewZoomIn->setShortcut(QKeySequence::ZoomIn);
  //m_viewZoomIn->setEnabled(false);

  //m_viewZoomOut = new QAction(QIcon::fromTheme("zoom-out"), tr("Zoom &Out"), this);
  //m_viewZoomOut->setShortcut(QKeySequence::ZoomOut);
  //m_viewZoomOut->setEnabled(false);

  //m_viewZoomSelect = new QAction(QIcon::fromTheme("zoom-selection"), tr("Zoom &Selection"), this);
  //m_viewZoomSelect->setEnabled(false);

  //m_viewZoomFit = new QAction(QIcon::fromTheme("zoom-best-fit"), tr("Zoom &Fit"), this);
  //m_viewZoomFit->setEnabled(false);

  m_viewFullScreen = new QAction(QIcon::fromTheme("view-fullscreen"), tr("F&ull Screen"), this);
  m_viewFullScreen->setShortcut(QKeySequence("Ctrl+F10"));
  m_viewFullScreen->setCheckable(true);

  m_viewRefresh = new QAction(QIcon::fromTheme("view-refresh"), tr("&Refresh"), this);
  m_viewRefresh->setShortcut(QKeySequence::Refresh);
  m_viewRefresh->setEnabled(false);

// Settings
  createIconThemesActions();

  m_settingsPreferences = new QAction(QIcon::fromTheme("preferences-system"), tr("&Preferences..."), this);
  m_settingsPreferences->setShortcut(QKeySequence::Preferences);

// Plugins
  m_pluginManager = new QAction(QIcon::fromTheme("plugin"), tr("&Manage Plugins..."), this);
  m_pluginBuilder = new QAction(tr("&Build a New Plugin..."), this);

// Help
  m_helpContents = new QAction(QIcon::fromTheme("help-browser"), tr("&View Help"), this);
  m_helpContents->setShortcut(QKeySequence::HelpContents);

  m_helpUpdate = new QAction(QIcon::fromTheme("system-software-update"), tr("&Update..."), this);
}

void te::qt::af::CoreApplication::createMenus()
{
// File
  m_fileMenu = addMenu("file", tr("&File"));

// File -> project
  m_fileMenu->addAction(m_fileNewProject);
  m_fileMenu->addAction(m_fileOpenProject);
  m_fileMenu->addAction(m_fileSaveProject);
  m_fileMenu->addAction(m_fileSaveAsProject);

// File -> print
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_filePrint);
  m_fileMenu->addAction(m_filePrintPreview);

// File -> import/export
  m_fileMenu->addSeparator();

  m_fileMenu->addAction(m_fileImportProject);
  m_fileMenu->addAction(m_fileExportProject);

// File -> Recent Projects
  m_fileMenu->addSeparator();

  m_fileMenuRecentProjects = m_fileMenu->addMenu(tr("&Recent Projects"));
  m_fileMenuRecentProjects->setObjectName("recent_projects");

  m_fileMenu->addSeparator();
  
// File -> exit
  m_fileMenu->addAction(m_fileExit);

// Edit
  m_editMenu = addMenu("edit", tr("&Edit"));

  m_editMenu->addAction(m_editUndo);
  m_editMenu->addAction(m_editRedo);

  m_editMenu->addSeparator();

  m_editMenu->addAction(m_editCut);
  m_editMenu->addAction(m_editCopy);
  m_editMenu->addAction(m_editPaste);

  m_editMenu->addSeparator();

  m_editMenu->addAction(m_editSelectAll);
  m_editMenu->addAction(m_editClear);

  m_editMenu->addSeparator();

  m_editMenu->addAction(m_editFind);
  m_editMenu->addAction(m_editReplace);

// View
  m_viewMenu = addMenu("view", tr("&View"));

  m_viewToolBarsMenu = new QMenu(tr("&Toolbars"));
  m_viewToolBarsMenu->setObjectName("toolbars");

  m_viewMenu->addMenu(m_viewToolBarsMenu);

  m_viewMenu->addSeparator();

  //m_viewMenu->addAction(m_viewLayerExplorer);
  //m_viewMenu->addAction(m_viewMapDisplay);

  m_viewMenu->addSeparator();

  //m_viewMenu->addAction(m_viewZoomIn);
  //m_viewMenu->addAction(m_viewZoomOut);
  //m_viewMenu->addAction(m_viewZoomSelect);
  //m_viewMenu->addAction(m_viewZoomFit);

  m_viewMenu->addSeparator();

  m_viewMenu->addAction(m_viewFullScreen);

  m_viewMenu->addSeparator();

  m_viewMenu->addAction(m_viewRefresh);

// Settings
  m_settingsMenu = addMenu("settings", tr("&Settings"));

  createIconThemesMenu();

  m_settingsMenu->addSeparator();

  m_settingsMenu->addAction(m_settingsPreferences);

// Plugins
  m_pluginsMenu = addMenu("plugins", tr("&Plugins"));

  createPluginCategoriesMenu();

  m_pluginsMenu->addSeparator();

  m_pluginsMenu->addAction(m_pluginManager);

  m_pluginsMenu->addAction(m_pluginBuilder);

// Help
  m_helpMenu = addMenu("help", tr("&Help"));
  
  m_helpMenu->addAction(m_helpContents);
  m_helpMenu->addAction(m_helpUpdate);
}

void te::qt::af::CoreApplication::createToolBars()
{
// File
  m_fileToolBar = addToolBar("file", tr("File"));
  m_fileToolBar->addAction(m_fileNewProject);
  m_fileToolBar->addAction(m_fileOpenProject);
  m_fileToolBar->addAction(m_fileSaveProject);
  //m_fileToolBar->addAction(m_fileSaveAsProject);

// View
  m_viewToolBar = addToolBar("view", tr("View"));
  //m_viewToolBar->addAction(m_viewZoomIn);
  //m_viewToolBar->addAction(m_viewZoomOut);
  //m_viewToolBar->addAction(m_viewZoomSelect);
  //m_viewToolBar->addAction(m_viewZoomFit);
  m_viewToolBar->hide();

  m_viewFullScreenToolBar = addToolBar("view_full_screen", tr("View Full Screen"));
  m_viewFullScreenToolBar->addAction(m_viewFullScreen);
  m_viewFullScreenToolBar->hide();

// Plugins
  //createPluginsToolBar();
}

void te::qt::af::CoreApplication::createStatusBar()
{
  statusBar()->showMessage(tr("TerraLib Application Framework is ready!"));
}

void te::qt::af::CoreApplication::createIconThemesActions()
{
  m_iconThemes = new QActionGroup(this);

  m_iconThemes->setExclusive(true);

// look for icon themes in system folders
  QStringList folders = QIcon::themeSearchPaths();

  for(QStringList::const_iterator itdir = folders.begin(); itdir != folders.end(); ++itdir)
  {
    if(!boost::filesystem::is_directory(itdir->toStdString()))
      continue;

    boost::filesystem::path path(itdir->toStdString());

    for(boost::filesystem::directory_iterator it(path), itEnd; it != itEnd; ++it)
    {
// check just in direct sub-dirs, don't go recursively for ever!
      if(!boost::filesystem::is_directory(it->status()))
        continue;

      std::string ithemeName = it->path().stem().string();

      QAction* a = new QAction(tr(ithemeName.c_str()), m_iconThemes);
    
      a->setCheckable(true);

      if(ithemeName == QIcon::themeName().toStdString())
        a->setChecked(true);
    }
  }
}

void te::qt::af::CoreApplication::createIconThemesMenu()
{
  assert(m_iconThemes);

  m_iconThemesMenu = m_settingsMenu->addMenu(tr("&Icon Themes"));
  m_iconThemesMenu->setObjectName("icon_themes");

  const QObjectList& actions = m_iconThemes->children();

  for(QObjectList::const_iterator it = actions.begin(); it != actions.end(); ++it)
    m_iconThemesMenu->addAction((QAction*)(*it));
}

void te::qt::af::CoreApplication::createPluginCategoriesMenu()
{
  assert(m_pluginsMenu);

// add common plugin categories
  te::common::SystemApplicationSettings::LockRead l(te::common::SystemApplicationSettings::getInstance());

  boost::property_tree::ptree& ptree = te::common::SystemApplicationSettings::getInstance().getAllSettings();

  BOOST_FOREACH(boost::property_tree::ptree::value_type& v, ptree.get_child("Application.DefaultPluginsCategory"))
  {
    if(v.second.data().empty())
      continue;

    m_pluginsMenu->addMenu(tr(v.second.data().c_str()));
  }
}

void te::qt::af::CoreApplication::closeEvent(QCloseEvent* e)
{
  AppClose aclose;

  emit triggered(&aclose);

  e->accept();
}

