/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
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
#include "SplashScreenManager.h"
#include "ui_BaseApplicationForm.h"

// Qt
#include <QtCore/QDir>
#include <QtGui/QActionGroup>
#include <QtGui/QCloseEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QMessageBox>

te::qt::af::BaseApplication::BaseApplication(const std::string& mainConfigFile, QWidget* parent)
  : QMainWindow(parent, 0),
    m_ui(new Ui::BaseApplicationForm),
    m_explorer(0),
    m_display(0),
    m_viewer(0)
{
  setMinimumSize(60, 60);
  resize(QSize(512, 512));

  te::qt::af::ApplicationController::getInstance().setMainConfig(mainConfigFile);

  try
  {
    te::qt::af::ApplicationController::getInstance().initializeMainModules();
  }
  catch(const std::exception& e)
  {
    //QMessageBox::critical(this, //errMsg = QString(e.what());
    QMessageBox::warning(this, tr("TerraView warning"), "teste1");
  }

  m_ui->setupUi(this);

  std::string icon = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconName");

  if(!icon.empty())
    setWindowIcon(QIcon::fromTheme(icon.c_str()));

  std::string title = te::common::SystemApplicationSettings::getInstance().getValue("Application.Title");

  if(!title.empty())
    setWindowTitle(title.c_str());

  makeDialog();

  try
  {
    te::qt::af::ApplicationController::getInstance().initializePlugins();
  }
  catch(const std::exception& e)
  {
    //QMessageBox::critical(this, //errMsg = QString(e.what());
      //if(!err_msg.isEmpty())
    QMessageBox::warning(this, tr("TerraView warning"), "teste 2");
  }
}

te::qt::af::BaseApplication::~BaseApplication()
{
  delete m_explorer;
  delete m_display;
  delete m_viewer;

  te::qt::af::ApplicationController::getInstance().finalize();
}

void te::qt::af::BaseApplication::makeDialog()
{
  ////! Putting tools on excluse group
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
  connect(lexplorer, SIGNAL(checkBoxWasClicked(const QModelIndex&)), SLOT(layerVisibilityChanged(const QModelIndex&)));

  m_explorer = new te::qt::af::LayerExplorer(lexplorer);

// map display
  te::qt::widgets::MapDisplay* map = new te::qt::widgets::MultiThreadMapDisplay(QSize(512, 512), this);
  map->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  map->setMouseTracking(true);

  m_display = new te::qt::af::MapDisplay(map);

// tabular viewer
  te::qt::widgets::TabularViewer* view = new te::qt::widgets::TabularViewer(this);

  m_viewer = new te::qt::af::TabularViewer(view);

// initializing connectors

// Connecting framework
  te::qt::af::ApplicationController::getInstance().addListener(this);
  te::qt::af::ApplicationController::getInstance().addListener(m_explorer);
  te::qt::af::ApplicationController::getInstance().addListener(m_display);
  te::qt::af::ApplicationController::getInstance().addListener(m_viewer);

// Docking components
  QDockWidget* doc = new QDockWidget(tr("Layer explorer"), this);
  doc->setWidget(lexplorer);
  QMainWindow::addDockWidget(Qt::LeftDockWidgetArea, doc);
  doc->connect(m_ui->m_viewLayerExplorer, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_viewLayerExplorer->connect(doc, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_ui->m_viewLayerExplorer->setChecked(true);

  doc = new QDockWidget(tr("Main display"), this);
  doc->setWidget(map);
  QMainWindow::setCentralWidget(doc);
  doc->connect(m_ui->m_viewMapDisplay, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_viewMapDisplay->connect(doc, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_ui->m_viewMapDisplay->setChecked(true);

  doc = new QDockWidget(tr("Tabular data viewer"), this);
  doc->setWidget(view);
  QMainWindow::addDockWidget(Qt::BottomDockWidgetArea, doc);
  doc->connect(m_ui->m_viewDataTable, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_viewDataTable->connect(doc, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_ui->m_viewDataTable->setChecked(false);
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
  m_ui->m_fileNewProject->setIcon(QIcon::fromTheme("document-new"));
  m_ui->m_fileOpenProject->setIcon(QIcon::fromTheme("document-open"));
  m_ui->m_fileSaveProject->setIcon(QIcon::fromTheme("document-save"));
  m_ui->m_fileSaveProjectAs->setIcon(QIcon::fromTheme("document-save-as"));
  m_ui->m_filePrint->setIcon(QIcon::fromTheme("document-print"));
  m_ui->m_filePrintPreview->setIcon(QIcon::fromTheme("document-print-preview"));
  m_ui->m_fileExit->setIcon(QIcon::fromTheme("system-log-out"));

  m_ui->m_editUndo->setIcon(QIcon::fromTheme("edit-undo"));
  m_ui->m_editRedo->setIcon(QIcon::fromTheme("edit-redo"));
  m_ui->m_editCut->setIcon(QIcon::fromTheme("edit-cut"));
  m_ui->m_editCopy->setIcon(QIcon::fromTheme("edit-copy"));
  m_ui->m_editPaste->setIcon(QIcon::fromTheme("edit-paste"));
  m_ui->m_editSelectAll->setIcon(QIcon::fromTheme("edit-select-all"));
  m_ui->m_editClear->setIcon(QIcon::fromTheme("edit-clear"));
  m_ui->m_editFind->setIcon(QIcon::fromTheme("edit-find"));
  m_ui->m_editReplace->setIcon(QIcon::fromTheme("edit-find-replace"));

  m_ui->m_viewLayerExplorer->setIcon(QIcon::fromTheme("tree-visible"));
  m_ui->m_viewMapDisplay->setIcon(QIcon::fromTheme("display-visible"));
  m_ui->m_viewDataTable->setIcon(QIcon::fromTheme("grid-visible"));
  m_ui->m_viewStyleExplorer->setIcon(QIcon::fromTheme("grid-visible"));
  m_ui->m_viewGrid->setIcon(QIcon::fromTheme("grid-visible"));
  m_ui->m_viewDataSourceExplorer->setIcon(QIcon::fromTheme("grid-visible"));
  m_ui->m_viewFullScreen->setIcon(QIcon::fromTheme("view-fullscreen"));
  m_ui->m_viewRefresh->setIcon(QIcon::fromTheme("view-refresh"));

  m_ui->m_layerEdit->setIcon(QIcon::fromTheme("layer-edit"));
  m_ui->m_layerRename->setIcon(QIcon::fromTheme("layer-rename"));
  m_ui->m_layerRaise->setIcon(QIcon::fromTheme("layer-raise"));
  m_ui->m_layerLower->setIcon(QIcon::fromTheme("layer-lower"));
  m_ui->m_layerToTop->setIcon(QIcon::fromTheme("layer-to-top"));
  m_ui->m_layerToBottom->setIcon(QIcon::fromTheme("layer-to-bottom"));

  m_ui->m_toolsCustomize->setIcon(QIcon::fromTheme("preferences-system"));

  m_ui->m_helpContents->setIcon(QIcon::fromTheme("help-browser"));
  m_ui->m_helpUpdate->setIcon(QIcon::fromTheme("system-software-update"));
}

void te::qt::af::BaseApplication::closeEvent(QCloseEvent* e)
{
  //AppClose aclose;

  //emit triggered(&aclose);

  e->accept();
}



