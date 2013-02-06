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
  \file terralib/qt/af/BaseApplication.cpp

  \brief A main window to be used as the basis for TerraLib applications.
*/

// TerraLib
#include "../../common/progress/TaskProgress.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/UserApplicationSettings.h"
#include "../../maptools/FolderLayer.h"
#include "../../maptools/Layer.h"
#include "../../maptools/RasterLayer.h"
#include "../../srs/Config.h"
#include "../widgets/canvas/MultiThreadMapDisplay.h"
#include "../widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../widgets/dataview/TabularViewer.h"
#include "../widgets/layer/AbstractTreeItem.h"
#include "../widgets/layer/LayerExplorer.h"
#include "../widgets/layer/LayerExplorerModel.h"
#include "../widgets/plugin/manager/PluginManagerDialog.h"
#include "../widgets/plugin/builder/PluginBuilderWizard.h"
#include "../widgets/progress/ProgressViewerDialog.h"
#include "../widgets/progress/ProgressViewerWidget.h"
#include "../widgets/progress/ProgressViewerBar.h"
#include "../widgets/se/RasterVisualDockWidget.h"
#include "../widgets/tools/Measure.h"
#include "../widgets/tools/Pan.h"
#include "../widgets/tools/ZoomArea.h"
#include "connectors/LayerExplorer.h"
#include "connectors/MapDisplay.h"
#include "connectors/TabularViewer.h"
#include "events/NewToolBar.h"
#include "events/LayerAdded.h"
#include "events/LayerSelected.h"
#include "events/TrackedCoordinate.h"
#include "ApplicationController.h"
#include "ApplicationPlugins.h"
#include "BaseApplication.h"
#include "BaseApplicationController.h"
#include "Project.h"
#include "SplashScreenManager.h"

// Qt
#include <QtCore/QDir>
#include <QtGui/QActionGroup>
#include <QtGui/QCloseEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>

te::qt::af::BaseApplication::BaseApplication(QWidget* parent)
  : QMainWindow(parent, 0),
    m_explorer(0),
    m_display(0),
    m_viewer(0),
    m_project(0)
{
  if (objectName().isEmpty())
    setObjectName("BaseApplicationForm");

  resize(640, 480);
  setMinimumSize(QSize(640, 480));

  m_centralwidget = new QWidget(this);
  m_centralwidget->setObjectName("centralwidget");
  setCentralWidget(m_centralwidget);
}

te::qt::af::BaseApplication::~BaseApplication()
{
  delete m_explorer;
  delete m_display;
  delete m_viewer;
  delete m_project;

  te::qt::af::ApplicationController::getInstance().finalize();
}

void te::qt::af::BaseApplication::init()
{

}

void te::qt::af::BaseApplication::init(const std::string& configFile)
{
  te::qt::af::ApplicationController::getInstance().setConfigFile(configFile);

  try
  {
    te::qt::af::ApplicationController::getInstance().initialize();
  }
  catch(const std::exception& e)
  {
    QString msgErr(tr("Could not initialize the application: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::critical(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msgErr);

    throw;
  }

  setWindowIcon(QIcon::fromTheme(te::qt::af::ApplicationController::getInstance().getAppIconName()));

  setWindowTitle(te::qt::af::ApplicationController::getInstance().getAppTitle());

  makeDialog();

  try
  {
    te::qt::af::ApplicationController::getInstance().initializePlugins();
  }
  catch(const std::exception& e)
  {
    QString msgErr(tr("Error loading plugins: %1"));

    msgErr = msgErr.arg(e.what());

    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msgErr);
  }
}

void te::qt::af::BaseApplication::onApplicationTriggered(te::qt::af::Event* evt)
{
  //switch(evt->getId())
  //{
  //  case te::qt::af::evt::NEW_TOOLBAR :
  //  break;

  //  case te::qt::af::evt::TRACK_COORDINATE:
  //  {
  //    te::qt::af::TrackedCoordinate* e = static_cast<te::qt::af::TrackedCoordinate*>(evt);
  //    QString text = tr("Coordinates: ") + "(" + QString::number(e->m_pos.x()) + " , " + QString::number(e->m_pos.y()) + ")";
  //    QStatusBar* sb = statusBar();
  //    sb->showMessage(text);
  //  }
  //  break;

  //  //PROVISORIO
  //  case te::qt::af::evt::LAYER_SELECTED:
  //  {
  //    te::qt::af::LayerSelected* e = static_cast<te::qt::af::LayerSelected*>(evt);
  //    
  //    if(e->m_layer->getType() == "RASTERLAYER")
  //    {
  //      m_rasterVisualDock->setRasterLayer(dynamic_cast<te::map::RasterLayer*>(e->m_layer));
  //    }
  //  }
  //  break;

  //  default :
  //  break;
  //}
}

void te::qt::af::BaseApplication::onAddDataSetLayerTriggered()
{
  try
  {
    std::auto_ptr<te::qt::widgets::DataSourceSelectorDialog> dselector(new te::qt::widgets::DataSourceSelectorDialog(this));

    int retval = dselector->exec();

    if(retval == QDialog::Rejected)
      return;
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         te::qt::af::ApplicationController::getInstance().getAppTitle(),
                         tr("Unknown error while trying to add a layer from a dataset!"));
  }
}

void te::qt::af::BaseApplication::onPluginsManagerTriggered()
{
  try
  {
    te::qt::widgets::PluginManagerDialog dlg(this);
    dlg.exec();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onPluginsBuilderTriggered()
{
  try
  {
    te::qt::widgets::PluginBuilderWizard dlg(this);
    dlg.exec();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::makeDialog()
{
  initActions();

  initMenus();

  initToolbars();

  initSlotsConnections();

// placing tools on an exclusive group
  //QActionGroup* vis_tools_group = new QActionGroup(this);
  //vis_tools_group->setExclusive(true);
  //m_ui->m_actionPan->setActionGroup(vis_tools_group);
  //m_ui->m_actionZoom_area->setActionGroup(vis_tools_group);
  //m_ui->m_area_act->setActionGroup(vis_tools_group);
  //m_ui->m_angle_act->setActionGroup(vis_tools_group);
  //m_ui->m_distance_act->setActionGroup(vis_tools_group);

// initializing basic components

// layer explorer
  te::qt::widgets::LayerExplorer* lexplorer = new te::qt::widgets::LayerExplorer(this);
  lexplorer->setModel(new te::qt::widgets::LayerExplorerModel(new te::map::FolderLayer("MainLayer", tr("My Layers").toStdString()), lexplorer));
  //connect(lexplorer, SIGNAL(checkBoxWasClicked(const QModelIndex&)), SLOT(layerVisibilityChanged(const QModelIndex&)));

  m_explorer = new te::qt::af::LayerExplorer(lexplorer);

// map display
  te::qt::widgets::MapDisplay* map = new te::qt::widgets::MultiThreadMapDisplay(QSize(512, 512), this);
  map->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  map->setMouseTracking(true);

  m_display = new te::qt::af::MapDisplay(map);

// tabular viewer
  te::qt::widgets::TabularViewer* view = new te::qt::widgets::TabularViewer(this);

  m_viewer = new te::qt::af::TabularViewer(view);

// Connecting framework
  te::qt::af::ApplicationController::getInstance().addListener(this);
  te::qt::af::ApplicationController::getInstance().addListener(m_explorer);
  te::qt::af::ApplicationController::getInstance().addListener(m_display);
  te::qt::af::ApplicationController::getInstance().addListener(m_viewer);


// initializing connector widgets

  QDockWidget* doc = new QDockWidget(tr("Layer explorer"), this);
  doc->setWidget(lexplorer);
  QMainWindow::addDockWidget(Qt::LeftDockWidgetArea, doc);
  doc->connect(m_viewLayerExplorer, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_viewLayerExplorer->connect(doc, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_viewLayerExplorer->setChecked(true);

  doc = new QDockWidget(tr("Main display"), this);
  doc->setWidget(map);
  QMainWindow::setCentralWidget(doc);
  doc->connect(m_viewMapDisplay, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_viewMapDisplay->connect(doc, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_viewMapDisplay->setChecked(true);

  doc = new QDockWidget(tr("Tabular data viewer"), this);
  doc->setWidget(view);
  QMainWindow::addDockWidget(Qt::BottomDockWidgetArea, doc);
  doc->connect(m_viewDataTable, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_viewDataTable->connect(doc, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_viewDataTable->setChecked(false);
  doc->setVisible(false);

//// Raster Visual Dock widget
//  m_rasterVisualDock = new te::qt::widgets::RasterVisualDockWidget(tr("Raster Enhancement"), this);
//  connect(m_rasterVisualDock, SIGNAL(symbolizerChanged()), this, SLOT(drawLayers()));
//  QMainWindow::addDockWidget(Qt::RightDockWidgetArea, m_rasterVisualDock);
//  m_rasterVisualDock->connect(m_ui->m_viewRasterVisual, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
//  m_ui->m_viewRasterVisual->connect(m_rasterVisualDock, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
//  m_ui->m_viewRasterVisual->setChecked(false);
//  m_rasterVisualDock->setVisible(false);

// Progress support
  //te::qt::widgets::ProgressViewerBar* pvb = new te::qt::widgets::ProgressViewerBar(this);
  //te::common::ProgressManager::getInstance().addViewer(pvb);

  //te::qt::widgets::ProgressViewerWidget* pvw = new te::qt::widgets::ProgressViewerWidget(this);
  //te::common::ProgressManager::getInstance().addViewer(pvw);

  //statusBar()->addPermanentWidget(pvb);

  //connect(pvb, SIGNAL(clicked()), this, SLOT(showProgressDock()));

  //m_progressDock = new QDockWidget(this);
  //m_progressDock->setWidget(pvw);
  //m_progressDock->setMinimumWidth(250);
  //m_progressDock->setAllowedAreas(Qt::RightDockWidgetArea);
  //m_progressDock->setWindowTitle(tr("Tasks Progress"));
  //addDockWidget(Qt::RightDockWidgetArea, m_progressDock);

  //m_progressDock->setVisible(false);

// setting icons
//  m_ui->m_fileNewProject->setIcon(QIcon::fromTheme("document-new"));
//  m_ui->m_fileOpenProject->setIcon(QIcon::fromTheme("document-open"));
//  m_ui->m_fileSaveProject->setIcon(QIcon::fromTheme("document-save"));
//  m_ui->m_fileSaveProjectAs->setIcon(QIcon::fromTheme("document-save-as"));
//  m_ui->m_filePrint->setIcon(QIcon::fromTheme("document-print"));
//  m_ui->m_filePrintPreview->setIcon(QIcon::fromTheme("document-print-preview"));
//  m_ui->m_fileExit->setIcon(QIcon::fromTheme("system-log-out"));
//
//  m_ui->m_editUndo->setIcon(QIcon::fromTheme("edit-undo"));
//  m_ui->m_editRedo->setIcon(QIcon::fromTheme("edit-redo"));
//  m_ui->m_editCut->setIcon(QIcon::fromTheme("edit-cut"));
//  m_ui->m_editCopy->setIcon(QIcon::fromTheme("edit-copy"));
//  m_ui->m_editPaste->setIcon(QIcon::fromTheme("edit-paste"));
//  m_ui->m_editSelectAll->setIcon(QIcon::fromTheme("edit-select-all"));
//  m_ui->m_editClear->setIcon(QIcon::fromTheme("edit-clear"));
//  m_ui->m_editFind->setIcon(QIcon::fromTheme("edit-find"));
//  m_ui->m_editReplace->setIcon(QIcon::fromTheme("edit-find-replace"));
//
//  m_ui->m_viewLayerExplorer->setIcon(QIcon::fromTheme("tree-visible"));
//  m_ui->m_viewMapDisplay->setIcon(QIcon::fromTheme("display-visible"));
//  m_ui->m_viewDataTable->setIcon(QIcon::fromTheme("grid-visible"));
//  m_ui->m_viewStyleExplorer->setIcon(QIcon::fromTheme("grid-visible"));
//  m_ui->m_viewGrid->setIcon(QIcon::fromTheme("grid-visible"));
//  m_ui->m_viewDataSourceExplorer->setIcon(QIcon::fromTheme("grid-visible"));
//  m_ui->m_viewFullScreen->setIcon(QIcon::fromTheme("view-fullscreen"));
//  m_ui->m_viewRefresh->setIcon(QIcon::fromTheme("view-refresh"));
//
//  m_ui->m_layerEdit->setIcon(QIcon::fromTheme("layer-edit"));
//  m_ui->m_layerRename->setIcon(QIcon::fromTheme("layer-rename"));
//  m_ui->m_layerRaise->setIcon(QIcon::fromTheme("layer-raise"));
//  m_ui->m_layerLower->setIcon(QIcon::fromTheme("layer-lower"));
//  m_ui->m_layerToTop->setIcon(QIcon::fromTheme("layer-to-top"));
//  m_ui->m_layerToBottom->setIcon(QIcon::fromTheme("layer-to-bottom"));
//
//  m_ui->m_toolsCustomize->setIcon(QIcon::fromTheme("preferences-system"));
//
//  m_ui->m_helpContents->setIcon(QIcon::fromTheme("help-browser"));
//  m_ui->m_helpUpdate->setIcon(QIcon::fromTheme("system-software-update"));
//
//// connect signals/slots
//  connect(m_ui->m_projectAddLayerDataset, SIGNAL(triggered()), SLOT(onAddDataSetLayer()));
}

void te::qt::af::BaseApplication::closeEvent(QCloseEvent* e)
{
  //AppClose aclose;

  //emit triggered(&aclose);

  e->accept();
}

void te::qt::af::BaseApplication::initAction(QAction*& act, const QString& icon, const QString& name, const QString& text, const QString& tooltip, 
  const bool& iconVisibleInMenu, const bool& isCheckable, const bool& enabled)
{
  act = new QAction(this);

  if(!icon.isEmpty())
    act->setIcon(QIcon::fromTheme(icon));

  act->setObjectName(name);
  act->setText(text);
  act->setIconVisibleInMenu(iconVisibleInMenu);
  act->setCheckable(isCheckable);
  act->setEnabled(enabled);

#ifndef QT_NO_TOOLTIP
  act->setToolTip(tooltip);
#endif
}

void te::qt::af::BaseApplication::initActions()
{
  //! Menu -View- actions
  initAction(m_viewLayerExplorer, "tree-visible", "Layer Explorer", tr("&Layer Explorer"), tr("Show or hide the layer explorer"), true, true, true);
  initAction(m_viewMapDisplay, "display-visible", "Map Display", tr("&Map Display"), tr("Show or hide the map display"), true, true, true);
  initAction(m_viewDataTable, "grid-visible", "Data Table", tr("&Data Table"), tr("Show or hide the data table"), true, true, false);
  initAction(m_viewStyleExplorer, "grid-visible", "Style Explorer", tr("&Style Explorer"), tr("Show or hide the style explorer"), true, true, false);
  initAction(m_viewFullScreen, "grid-visible", "Full Screen", tr("F&ull Screen..."), tr(""), true, false, false);
  initAction(m_viewRefresh, "view-refresh", "Refresh", tr("&Refresh..."), tr(""), true, false, false);
  initAction(m_viewToolBars, "", "Toolbars", tr("&Toolbars..."), tr(""), true, false, false);
  initAction(m_viewGrid, "grid-visible", "Grid", tr("&Grid"), tr("Show or hide the geographic grid"), true, true, false);
  initAction(m_viewDataSourceExplorer, "grid-visible", "Data Source Explorer", tr("&Data Source Explorer"), tr("Show or hide the data source explorer"), true, true, false);

  //! Menu -Tools- actions
  initAction(m_toolsCustomize, "preferences-system", "Customize", tr("&Customize..."), tr("Customize the system preferences"), true, false, false);
  initAction(m_toolsDataSourceManagement, "", "Data Source Management", tr("&Data Source Management..."), tr("Manage the registered data sources"), true, false, false);

  //! Menu -Edit- actions
  initAction(m_editUndo, "edit-undo", "Undo", tr("&Undo"), tr("Undo the last operation"), true, false, false);
  initAction(m_editRedo, "edit-redo", "Redo", tr("&Redo"), tr("Redo the last operation"), true, false, false);
  initAction(m_editCut, "edit-cut", "Cut", tr("Cu&t"), tr(""), true, true, false);
  initAction(m_editCopy, "edit-copy", "Copy", tr("&Copy"), tr(""), true, true, false);
  initAction(m_editPaste, "edit-paste", "&Paste", tr("&Paste"), tr(""), true, true, false);
  initAction(m_editSelectAll, "edit-select-all", "Select All", tr("Select &All"), tr(""), true, true, false);
  initAction(m_editClear, "edit-clear", "Clear", tr("C&lear"), tr(""), true, true, false);
  initAction(m_editFind, "edit-find", "Find", tr("&Find..."), tr(""), true, true, false);
  initAction(m_editReplace, "edit-find-replace", "Replace", tr("R&eplace..."), tr(""), true, true, false);

  //! Menu -Plugins- actions
  initAction(m_pluginsManager, "", "Management", tr("&Management..."), tr("Manage the application plugins"), true, false, true);
  initAction(m_pluginsBuilder, "", "Build a new Plugin", tr("&Build a new Plugin..."), tr("Create a new plugin"), true, false, true);

  //! Menu -Help- actions
  initAction(m_helpContents, "help-browser", "View Help", tr("&View help..."), tr("Shows help dialog"), true, false, false);
  initAction(m_helpUpdate, "system-software-update", "Update", tr("&Update..."), tr(""), true, false, false);
  initAction(m_helpAbout, "", "About", tr("&About..."), tr(""), true, false, false);

  //! Menu -Project- actions
  initAction(m_projectRemoveLayer, "", "Remove Layer", tr("&Remove Layer"), tr("Remove layer from the project"), true, false, false);
  initAction(m_projectProperties, "", "Properties", tr("&Properties..."), tr("Show the project properties"), true, false, false);
  initAction(m_projectAddLayerDataset, "", "Dataset", tr("&Dataset..."), tr("Add a new layer from a dataset"), true, false, true);
  initAction(m_projectAddLayerImage, "", "Image", tr("&Image"), tr("Add a new layer from a satellite image"), true, false, false);
  initAction(m_projectAddLayerGraph, "", "Graph", tr("&Graph"), tr("Add a new layer from a graph"), true, false, false);

  //! Menu -Layer- actions
  initAction(m_layerEdit, "layer-edit", "Edit", tr("&Edit"), tr(""), true, false, false);
  initAction(m_layerRename, "layer-rename", "Rename", tr("R&ename"), tr(""), true, false, false);
  initAction(m_layerExport, "", "Export", tr("E&xport..."), tr(""), true, false, false);
  initAction(m_layerProperties, "", "Properties", tr("&Properties"), tr(""), true, false, false);
  initAction(m_layerRaise, "layer-raise", "Raise", tr("&Raise"), tr(""), true, false, false);
  initAction(m_layerLower, "layer-lower", "Lower", tr("&Lower"), tr(""), true, false, false);
  initAction(m_layerToTop, "layer-to-top", "To Top", tr("To &Top"), tr(""), true, false, false);
  initAction(m_layerToBottom, "layer-to-bottom", "To Bottom", tr("To &Bottom"), tr(""), true, false, false);

  //! Menu -File- actions
  initAction(m_fileNewProject, "document-new", "New Project", tr("&New Project"), tr(""), true, false, false);
  initAction(m_fileSaveProject, "document-save", "Save Project", tr("&Save Project..."), tr(""), true, false, false);
  initAction(m_fileSaveProjectAs, "document-save-as", "Save Project As", tr("Save Project &As..."), tr(""), true, false, false);
  initAction(m_fileOpenProject, "document-open", "Open Project", tr("&Open Project..."), tr(""), true, false, false);
  initAction(m_fileExit, "system-log-out", "Exit", tr("E&xit"), tr(""), true, false, true);
  initAction(m_filePrint, "document-print", "Print", tr("&Print..."), tr(""), true, false, false);
  initAction(m_filePrintPreview, "document-print-preview", "Print Preview", tr("Print Pre&view..."), tr(""), true, false, false);
}

void te::qt::af::BaseApplication::initMenus()
{
  //! Making menus
  m_menubar = new QMenuBar(this);
  m_menubar->setObjectName(QString::fromUtf8("menubar"));
  m_menubar->setGeometry(QRect(0, 0, 640, 21));

    //! File menu
  m_fileMenu = new QMenu(m_menubar);
  m_fileMenu->setObjectName("File");
  m_fileMenu->setTitle(tr("&File"));

  m_recentProjectsMenu = new QMenu(m_fileMenu);
  m_recentProjectsMenu->setObjectName("Recent Projects");
  m_recentProjectsMenu->setTitle(tr("&Recent Projects"));

  m_menubar->addAction(m_fileMenu->menuAction());

  m_fileMenu->addAction(m_fileNewProject);
  m_fileMenu->addAction(m_fileOpenProject);
  m_fileMenu->addAction(m_fileSaveProject);
  m_fileMenu->addAction(m_fileSaveProject);
  m_fileMenu->addAction(m_fileSaveProjectAs);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_recentProjectsMenu->menuAction());
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_filePrint);
  m_fileMenu->addAction(m_filePrintPreview);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_fileExit);

    //! Edit menu
  m_editMenu = new QMenu(m_menubar);
  m_editMenu->setObjectName("Edit");
  m_editMenu->setTitle(tr("&Edit"));

  m_menubar->addAction(m_editMenu->menuAction());

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

    //! View menu
  m_viewMenu = new QMenu(m_menubar);
  m_viewMenu->setObjectName("View");
  m_viewMenu->setTitle(tr("&View"));

  m_menubar->addAction(m_viewMenu->menuAction());

  m_viewMenu->addAction(m_viewToolBars);
  m_viewMenu->addSeparator();
  m_viewMenu->addAction(m_viewLayerExplorer);
  m_viewMenu->addAction(m_viewMapDisplay);
  m_viewMenu->addAction(m_viewDataTable);
  m_viewMenu->addAction(m_viewStyleExplorer);
  m_viewMenu->addAction(m_viewGrid);
  m_viewMenu->addAction(m_viewDataSourceExplorer);
  m_viewMenu->addSeparator();
  m_viewMenu->addAction(m_viewFullScreen);
  m_viewMenu->addSeparator();
  m_viewMenu->addAction(m_viewRefresh);

    //! Project menu
  m_projectMenu = new QMenu(m_menubar);
  m_projectMenu->setObjectName("Project");
  m_projectMenu->setTitle(tr("&Project"));

  m_projectAddLayerMenu = new QMenu(m_projectMenu);
  m_projectAddLayerMenu->setObjectName("Add Layer");
  m_projectAddLayerMenu->setTitle(tr("&Add Layer"));

  m_menubar->addAction(m_projectMenu->menuAction());

  m_projectMenu->addAction(m_projectAddLayerMenu->menuAction());
  m_projectMenu->addAction(m_projectRemoveLayer);
  m_projectMenu->addSeparator();
  m_projectMenu->addAction(m_projectProperties);
  m_projectAddLayerMenu->addAction(m_projectAddLayerDataset);
  m_projectAddLayerMenu->addAction(m_projectAddLayerImage);
  m_projectAddLayerMenu->addAction(m_projectAddLayerGraph);

    //! Layer menu
  m_layerMenu = new QMenu(m_menubar);
  m_layerMenu->setObjectName("Layer");
  m_layerMenu->setTitle(tr("&Layer"));

  m_menubar->addAction(m_layerMenu->menuAction());

  m_layerMenu->addAction(m_layerEdit);
  m_layerMenu->addAction(m_layerRename);
  m_layerMenu->addAction(m_layerExport);
  m_layerMenu->addAction(m_layerProperties);
  m_layerMenu->addSeparator();
  m_layerMenu->addAction(m_layerRaise);
  m_layerMenu->addAction(m_layerLower);
  m_layerMenu->addAction(m_layerToTop);
  m_layerMenu->addAction(m_layerToBottom);

    //! Tools menu
  m_toolsMenu = new QMenu(m_menubar);
  m_toolsMenu->setObjectName("Tools");
  m_toolsMenu->setTitle(tr("&Tools"));

  m_menubar->addAction(m_toolsMenu->menuAction());

  m_toolsMenu->addAction(m_toolsDataSourceManagement);
  m_toolsMenu->addSeparator();
  m_toolsMenu->addAction(m_toolsCustomize);

    //! Plugins menu
  m_pluginsMenu = new QMenu(m_menubar);
  m_pluginsMenu->setObjectName("Plugins");
  m_pluginsMenu->setTitle(tr("Pl&ugins"));

  m_menubar->addAction(m_pluginsMenu->menuAction());

  m_pluginsMenu->addAction(m_pluginsManager);
  m_pluginsMenu->addSeparator();
  m_pluginsMenu->addAction(m_pluginsBuilder);

    //! Help menu
  m_helpMenu = new QMenu(m_menubar);
  m_helpMenu->setObjectName("Help");
  m_helpMenu->setTitle(tr("&Help"));

  m_menubar->addAction(m_helpMenu->menuAction());

  m_helpMenu->addAction(m_helpContents);
  m_helpMenu->addAction(m_helpUpdate);

  //! Sets the toolbar
  this->setMenuBar(m_menubar);
}

void te::qt::af::BaseApplication::initToolbars()
{
  m_statusbar = new QStatusBar(this);
  m_fileToolBar = new QToolBar(this);
  m_editToolBar = new QToolBar(this);

  m_statusbar->setObjectName("Status bar");
  setStatusBar(m_statusbar);

  m_fileToolBar->setObjectName("File tool bar");
  addToolBar(Qt::TopToolBarArea, m_fileToolBar);
  m_fileToolBar->setWindowTitle(tr("File tool bar"));

  m_editToolBar->setObjectName("Edit tool bar");
  addToolBar(Qt::TopToolBarArea, m_editToolBar);
  m_fileToolBar->setWindowTitle(tr("Edit tool bar"));

  m_fileToolBar->addAction(m_fileNewProject);
  m_fileToolBar->addAction(m_fileOpenProject);
  m_fileToolBar->addAction(m_fileSaveProject);

  m_editToolBar->addAction(m_editUndo);
  m_editToolBar->addAction(m_editRedo);
  m_editToolBar->addSeparator();
  m_editToolBar->addAction(m_editCut);
  m_editToolBar->addAction(m_editCopy);
  m_editToolBar->addAction(m_editPaste);
}

void te::qt::af::BaseApplication::initSlotsConnections()
{
  connect(m_fileExit, SIGNAL(triggered()), SLOT(close()));
  connect(m_projectAddLayerDataset, SIGNAL(triggered()), SLOT(onAddDataSetLayerTriggered()));
  connect(m_pluginsManager, SIGNAL(triggered()), SLOT(onPluginsManagerTriggered()));
  connect(m_pluginsBuilder, SIGNAL(triggered()), SLOT(onPluginsBuilderTriggered()));
}
