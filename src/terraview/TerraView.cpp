/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A Free and Open Source GIS Application.

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
  \file terraview/TerraView.cpp

  \brief The main class of TerraView.
*/

// TerraView
#include "AboutDialog.h"
#include "TerraView.h"
#include "Project.h"

// TerraLib
#include <terralib/common/PlatformUtils.h>
#include <terralib/qt/af/ApplicationController.h>
#include <terralib/qt/af/connectors/DataSetTableDockWidget.h>
#include <terralib/qt/af/connectors/LayerExplorer.h>
#include <terralib/qt/af/connectors/MapDisplay.h>
#include <terralib/qt/af/connectors/StyleExplorer.h>
#include <terralib/qt/af/events/ApplicationEvents.h>
#include <terralib/qt/af/Utils.h>
#include <terralib/qt/widgets/canvas/MapDisplay.h>
#include <terralib/qt/widgets/help/HelpManager.h>
#include <terralib/qt/widgets/layer/explorer/AbstractTreeItem.h>
#include <terralib/qt/widgets/layer/explorer/LayerExplorer.h>
#include <terralib/qt/widgets/se/StyleDockWidget.h>

// STL
#include <memory>

// Qt
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>


te::qt::af::DataSetTableDockWidget* GetLayerDock(const te::map::AbstractLayer* layer, const std::vector<te::qt::af::DataSetTableDockWidget*>& docs)
{
  std::vector<te::qt::af::DataSetTableDockWidget*>::const_iterator it;

  for(it=docs.begin(); it!=docs.end(); ++it)
    if((*it)->getLayer() == layer)
      return *it;

  return 0;
}

void GetProjectsFromSettings(QStringList& prjTitles, QStringList& prjPaths)
{
  QSettings s(QSettings::IniFormat,
                          QSettings::UserScope,
                          qApp->organizationName(),
                          qApp->applicationName());

  s.beginGroup("projects");

  s.beginGroup("most_recent");
  QString path = s.value("path").toString();
  QString title = s.value("title").toString();

  s.endGroup();

  if(path.isEmpty() || title.isEmpty())
    return;

  prjPaths.append(path);
  prjTitles.append(title);

  int size = s.beginReadArray("recents");

  for(int i=0; i<size; i++)
  {
    s.setArrayIndex(i);

    path = s.value("project/path").toString();
    title = s.value("project/title").toString();

    prjPaths.append(path);
    prjTitles.append(title);
  }

  s.endArray();
  s.endGroup();
}

void WriteProjectsToSettings(const QStringList& prjTitles, const QStringList& prjPaths)
{
  QSettings s(QSettings::IniFormat,
                          QSettings::UserScope,
                          qApp->organizationName(),
                          qApp->applicationName());

  s.beginGroup("projects");
  s.beginGroup("most_recent");
  s.setValue("path", *prjPaths.begin());
  s.setValue("title", *prjTitles.begin());
  s.endGroup();

  s.beginWriteArray("recents");
  for(int i=1; i<prjTitles.size(); i++)
  {
    s.setArrayIndex(i-1);

    s.setValue("project/path", prjPaths.at(i));
    s.setValue("project/title", prjTitles.at(i));
  }
  s.endArray();
  s.endGroup();
}

void AddRecentProjectToSettings(const QString& prjTitle, const QString& prjPath)
{
  QStringList prjPaths,
      prjTitles;

  GetProjectsFromSettings(prjTitles, prjPaths);

  if(!prjPaths.contains(prjPath))
  {
    prjTitles.prepend(prjTitle);
    prjPaths.prepend(prjPath);
  }
  else
  {
    prjTitles.move(prjTitles.indexOf(prjTitle), 0);
    prjPaths.move(prjPaths.indexOf(prjPath), 0);
  }

  WriteProjectsToSettings(prjTitles, prjPaths);
}

TerraView::TerraView(QWidget* parent)
  : te::qt::af::BaseApplication(parent),
    m_helpManager(0)
{
  //m_dsMenu = menuBar()->addMenu(tr("Datasources"));

  //QAction* act = new QAction(QIcon::fromTheme("view-data-table"), tr("Show table..."), this);
  //connect(act, SIGNAL(triggered()), SLOT(onShowTableTriggered()));

  //getLayerExplorer()->add(act, "", "DATASET_LAYER_ITEM", te::qt::widgets::LayerTreeView::UNIQUE_ITEM_SELECTED);

  //QMenu* hmenu = menuBar()->addMenu(tr("&Help"));

  //act = hmenu->addAction(tr("&View Help..."));
  //act->setObjectName("Help.View Help");
  //act->setIcon(QIcon::fromTheme("help-browser"));

  //connect(act, SIGNAL(triggered()), this, SLOT(onHelpTriggered()));

  //act = hmenu->addAction(tr("&About..."));

  //act->setObjectName("Help.About");
  //act->setIcon(QIcon::fromTheme("help-about-browser"));

  //connect(act, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

  //act = new QAction(QIcon::fromTheme("document-save"), tr("Save Project..."), this);

  //QMenu* mnu = 0;// BaseApplication::getMenuFile();
  //QToolBar* bar = 0;// BaseApplication::getToolbar("m_fileToolbar");

  //if(bar != 0)
  //  bar->insertAction(bar->actions().at(0), act);

  //mnu->insertAction(mnu->actions().at(0), act);

  //connect(act, SIGNAL(triggered()), SLOT(onSaveProjectTriggered()));

  //QAction* loadProjectAction = new QAction(QIcon::fromTheme("document-open"), tr("Open Project..."), this);

  //if(bar != 0)
  //  bar->insertAction(act, loadProjectAction);

  //mnu->insertAction(act, loadProjectAction);

  //connect(loadProjectAction, SIGNAL(triggered()), SLOT(onOpenProjectTriggered()));

  m_project = new ProjectMetadata;

  m_project->m_author = "INPE";
  m_project->m_title = tr("Default project");
  m_project->m_changed = false;
}

TerraView::~TerraView()
{
  delete m_project;
}

void TerraView::init()
{
  BaseApplication::init(te::common::FindInTerraLibPath(TERRALIB_APPLICATION_CONFIG_FILE).c_str());

  QStringList prjTitles,
      prjPaths;

  initToolbars();

  GetProjectsFromSettings(prjTitles, prjPaths);

  if(!prjPaths.empty())
    openProject(*prjPaths.begin());
  else
    setWindowTitle(windowTitle() + " - " + m_project->m_title);
}

//void TerraView::init(const std::string& configFile)
//{
//  te::qt::af::BaseApplication::init(configFile.c_str());

//  m_app->setMsgBoxParentWidget(this);

//  QPixmap pix(m_app->getAppIconName());
////  pix = pix.scaled(16, 16, Qt::KeepAspectRatio/*, Qt::SmoothTransformation*/);
//  QIcon icon(m_app->getAppIconName());

//  setWindowIcon(icon);
//}

void TerraView::makeDialog()
{
  te::qt::af::BaseApplication::makeDialog();

  addMenusActions();

  m_viewLayerExplorer->setChecked(true);
  m_display->getDisplay()->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  m_viewStyleExplorer->setChecked(false);
  m_styleExplorer->getExplorer()->setVisible(false);
}

void TerraView::initActions()
{
  te::qt::af::BaseApplication::initActions();

  // Menu -View- actions
  initAction(m_viewLayerExplorer, "view-layer-explorer", "View.Layer Explorer", tr("&Layer Explorer"), tr("Show or hide the layer explorer"), true, true, true, m_menubar);
  initAction(m_viewDataTable, "view-data-table", "View.Data Table", tr("&Data Table"), tr("Show or hide the data table"), true, true, true, m_menubar);
  initAction(m_viewStyleExplorer, "style", "View.Style Explorer", tr("&Style Explorer"), tr("Show or hide the style explorer"), true, true, true, m_menubar);
  initAction(m_viewFullScreen, "view-fullscreen", "View.Full Screen", tr("F&ull Screen"), tr(""), true, true, true, m_menubar);

  // Menu -Tools- actions
  initAction(m_toolsCustomize, "preferences-system", "Tools.Customize", tr("&Customize..."), tr("Customize the system preferences"), true, false, true, m_menubar);
  initAction(m_toolsDataExchanger, "datasource-exchanger", "Tools.Exchanger.All to All", tr("&Advanced..."), tr("Exchange data sets between data sources"), true, false, true, m_menubar);
  initAction(m_toolsDataExchangerDirect, "data-exchange-direct-icon", "Tools.Exchanger.Direct", tr("&Layer..."), tr("Exchange data sets from layers"), true, false, true, m_menubar);
  initAction(m_toolsDataExchangerDirectPopUp, "data-exchange-direct-icon", "Tools.Exchanger.Direct", tr("&Exchange..."), tr("Exchange data sets from layers"), true, false, true, m_menubar);
  initAction(m_toolsDataSourceExplorer, "datasource-explorer", "Tools.Data Source Explorer", tr("&Data Source Explorer..."), tr("Show or hide the data source explorer"), true, false, true, m_menubar);
  initAction(m_toolsQueryDataSource, "datasource-query", "Tools.Query Data Source", tr("&Query Data Source..."), tr("Allows you to query data in a data source"), true, false, true, m_menubar);
  initAction(m_toolsRasterMultiResolution, "raster-multiresolution-icon", "Tools.Raster Multi Resolution", tr("&Raster Multi Resolution..."), tr("Creates multi resolution over a raster..."), true, false, true, m_menubar);

  // Menu -Plugins- actions
  initAction(m_pluginsManager, "plugin", "Plugins.Management", tr("&Manage Plugins..."), tr("Manage the application plugins"), true, false, true, m_menubar);

  // Menu -Help- actions
  initAction(m_helpContents, "help-browser", "Help.View Help", tr("&View Help..."), tr("Shows help dialog"), true, false, true, m_menubar);
  initAction(m_helpAbout, "help-about-browser", "Help.About", tr("&About..."), tr(""), true, false, false, m_menubar);

  // Menu -Project- actions
  initAction(m_projectAddLayerDataset, "datasource", "Project.Add Layer.All Sources", tr("&From Data Source..."), tr("Add a new layer from all available data sources"), true, false, true, m_menubar);
  initAction(m_projectAddFolderLayer, "folderlayer-new", "Project.New Folder Layer", tr("Add &Folder Layer..."), tr("Add a new folder layer"), true, false, true, m_menubar);
  initAction(m_projectAddLayerQueryDataSet, "view-filter", "Project.Add Layer.Query Dataset", tr("&Query Dataset..."), tr("Add a new layer from a queried dataset"), true, false, true, m_menubar);
  initAction(m_projectAddLayerTabularDataSet, "view-data-table", "Project.Add Layer.Tabular File", tr("&Tabular File..."), tr("Add a new layer from a Tabular file"), true, false, true, m_menubar);
  initAction(m_projectRemoveLayer, "layer-remove", "Project.Remove Layer", tr("&Remove Layer(s)"), tr("Remove layer(s) from the project"), true, false, true, this);
  initAction(m_projectRenameLayer, "layer-rename", "Project.Rename Layer", tr("Rename Layer..."), tr("Rename layer"), true, false, true, this);
  initAction(m_projectChangeLayerDataSource, "", "Project.Change Layer Data Source", tr("&Change Layer Data Source"), tr("Chanage layer Data Source"), true, false, true, this);
  initAction(m_projectUpdateLayerDataSource, "", "Project.Update Layer Data Source", tr("&Update Layer Data Source"), tr("Update layer Data Source"), true, false, true, this);
  initAction(m_projectProperties, "document-info", "Project.Properties", tr("&Properties..."), tr("Show the project properties"), true, false, true, m_menubar);

  // Menu -Layer- actions
  initAction(m_layerRemoveObjectSelection, "pointer-remove-selection", "Layer.Remove Selection", tr("&Remove Selection"), tr(""), true, false, true, m_menubar);
  initAction(m_layerRemoveItem, "item-remove", "Layer.Remove Item", tr("&Remove Item"), tr(""), true, false, true, m_menubar);
  initAction(m_layerObjectGrouping, "grouping", "Layer.ObjectGrouping", tr("&Edit Legend..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerProperties, "layer-info", "Layer.Properties", tr("&Properties..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerSRS, "layer-srs", "Layer.SRS", tr("&Inform SRS..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerShowTable, "view-data-table", "Layer.Show Table", tr("S&how Table"), tr(""), true, false, true, m_menubar);
  initAction(m_layerChartsHistogram, "chart-bar", "Layer.Charts.Histogram", tr("&Histogram..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerChartsScatter, "chart-scatter", "Layer.Charts.Scatter", tr("&Scatter..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerChart, "chart-pie", "Layer.Charts.Chart", tr("&Pie/Bar Chart..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerFitOnMapDisplay, "layer-fit", "Layer.Fit Layer on the Map Display", tr("Fit Layer"), tr("Fit the current layer on the Map Display"), true, false, true, m_menubar);
  initAction(m_layerFitSelectedOnMapDisplay, "zoom-selected-extent", "Layer.Fit Selected Features on the Map Display", tr("Fit Selected Features"), tr("Fit the selected features on the Map Display"), true, false, true, m_menubar);
  initAction(m_layerPanToSelectedOnMapDisplay, "pan-selected", "Layer.Pan to Selected Features on Map Display", tr("Pan to Selected Features"), tr("Pan to the selected features on the Map Display"), true, false, true, m_menubar);
  initAction(m_queryLayer, "view-filter", "Layer.Query", tr("Query..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerLinkTable, "layer-link", "Layer.Link", tr("&Link..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerCompositionMode, "layer-compose", "Layer.Composition Mode", tr("&Composition Mode..."), tr("Set the composition mode to renderer the selected layer"), true, false, true, m_menubar);

  // Menu -File- actions
  initAction(m_fileNewProject, "document-new", "File.New Project", tr("&New Project..."), tr(""), true, false, true, m_menubar);
  initAction(m_fileSaveProject, "document-save", "File.Save Project", tr("&Save Project"), tr(""), true, false, true, m_menubar);
  initAction(m_fileSaveProjectAs, "document-save-as", "File.Save Project As", tr("Save Project &As..."), tr(""), true, false, false, m_menubar);
  initAction(m_fileOpenProject, "document-open", "File.Open Project", tr("&Open Project..."), tr(""), true, false, true, m_menubar);
  initAction(m_fileRestartSystem, "", "File.Restart System", tr("&Restart System..."), tr("Restart the system."), true, false, true, m_menubar);
  initAction(m_fileExit, "system-log-out", "File.Exit", tr("E&xit"), tr(""), true, false, true, m_menubar);
  initAction(m_filePrintPreview, "document-print-preview", "File.Print Preview", tr("Print Pre&view..."), tr(""), true, false, false, m_menubar);
  initAction(m_filePrint, "document-print", "File.Print", tr("&Print..."), tr(""), true, false, false, m_menubar);

  // Menu -Map- actions
  initAction(m_mapDraw, "map-draw", "Map.Draw", tr("&Draw"), tr("Draw the visible layers"), true, false, true, m_menubar);
  initAction(m_mapZoomIn, "zoom-in", "Map.Zoom In", tr("Zoom &In"), tr(""), true, true, true, m_menubar);
  initAction(m_mapZoomOut, "zoom-out", "Map.Zoom Out", tr("Zoom &Out"), tr(""), true, true, true, m_menubar);
  initAction(m_mapPan, "pan", "Map.Pan", tr("&Pan"), tr(""), true, true, true, m_menubar);
  initAction(m_mapZoomExtent, "zoom-extent", "Map.Zoom Extent", tr("Zoom &Extent"), tr(""), true, false, true, m_menubar);
  initAction(m_mapPreviousExtent, "edit-undo", "Map.Previous Extent", tr("&Previous Extent"), tr(""), true, false, false, m_menubar);
  initAction(m_mapNextExtent, "edit-redo", "Map.Next Extent", tr("&Next Extent"), tr(""), true, false, false, m_menubar);
  initAction(m_mapInfo, "pointer-info", "Map.Info", tr("&Info"), tr(""), true, true, true, m_menubar);
  initAction(m_mapRemoveSelection, "pointer-remove-selection", "Map.Remove Selection", tr("&Remove Selection"), tr(""), true, false, true, m_menubar);
  initAction(m_mapSelection, "pointer-selection", "Map.Selection", tr("&Selection"), tr(""), true, true, true, m_menubar);
  initAction(m_mapMeasureDistance, "distance-measure", "Map.Measure Distance", tr("Measure &Distance"), tr(""), true, true, true, m_menubar);
  initAction(m_mapMeasureArea, "area-measure", "Map.Measure Area", tr("Measure &Area"), tr(""), true, true, true, m_menubar);
  initAction(m_mapMeasureAngle, "angle-measure", "Map.Measure Angle", tr("Measure &Angle"), tr(""), true, true, true, m_menubar);
}

void TerraView::initSlotsConnections()
{
  te::qt::af::BaseApplication::initSlotsConnections();

  connect(m_fileExit, SIGNAL(triggered()), SLOT(close()));
  connect(m_fileRestartSystem, SIGNAL(triggered()), SLOT(onRestartSystemTriggered()));
  connect(m_projectAddLayerDataset, SIGNAL(triggered()), SLOT(onAddDataSetLayerTriggered()));
  connect(m_projectAddLayerQueryDataSet, SIGNAL(triggered()), SLOT(onAddQueryLayerTriggered()));
  connect(m_projectAddLayerTabularDataSet, SIGNAL(triggered()), SLOT(onAddTabularLayerTriggered()));
  connect(m_projectRemoveLayer, SIGNAL(triggered()), SLOT(onRemoveLayerTriggered()));
  connect(m_projectRenameLayer, SIGNAL(triggered()), SLOT(onRenameLayerTriggered()));
  connect(m_projectProperties, SIGNAL(triggered()), SLOT(onProjectPropertiesTriggered()));
  connect(m_projectChangeLayerDataSource, SIGNAL(triggered()), SLOT(onChangeLayerDataSourceTriggered()));
  connect(m_projectUpdateLayerDataSource, SIGNAL(triggered()), SLOT(onUpdateLayerDataSourceTriggered()));
  connect(m_pluginsManager, SIGNAL(triggered()), SLOT(onPluginsManagerTriggered()));
  connect(m_recentProjectsMenu, SIGNAL(triggered(QAction*)), SLOT(onRecentProjectsTriggered(QAction*)));
  connect(m_fileNewProject, SIGNAL(triggered()), SLOT(onNewProjectTriggered()));
  connect(m_fileOpenProject, SIGNAL(triggered()), SLOT(onOpenProjectTriggered()));
  connect(m_fileSaveProject, SIGNAL(triggered()), SLOT(onSaveProjectTriggered()));
  connect(m_fileSaveProjectAs, SIGNAL(triggered()), SLOT(onSaveProjectAsTriggered()));
  connect(m_toolsCustomize, SIGNAL(triggered()), SLOT(onToolsCustomizeTriggered()));
  connect(m_toolsDataExchanger, SIGNAL(triggered()), SLOT(onToolsDataExchangerTriggered()));
  connect(m_toolsDataExchangerDirect, SIGNAL(triggered()), SLOT(onToolsDataExchangerDirectTriggered()));
  connect(m_toolsDataExchangerDirectPopUp, SIGNAL(triggered()), SLOT(onToolsDataExchangerDirectPopUpTriggered()));
  connect(m_toolsQueryDataSource, SIGNAL(triggered()), SLOT(onToolsQueryDataSourceTriggered()));
  connect(m_toolsRasterMultiResolution, SIGNAL(triggered()), SLOT(onToolsRasterMultiResolutionTriggered()));
  connect(m_helpContents, SIGNAL(triggered()), SLOT(onHelpTriggered()));
  connect(m_layerChartsHistogram, SIGNAL(triggered()), SLOT(onLayerHistogramTriggered()));
  connect(m_layerLinkTable, SIGNAL(triggered()), SLOT(onLinkTriggered()));
  connect(m_layerChartsScatter, SIGNAL(triggered()), SLOT(onLayerScatterTriggered()));
  connect(m_layerChart, SIGNAL(triggered()), SLOT(onLayerChartTriggered()));
  connect(m_projectAddFolderLayer, SIGNAL(triggered()), SLOT(onAddFolderLayerTriggered()));
  connect(m_layerProperties, SIGNAL(triggered()), SLOT(onLayerPropertiesTriggered()));
  connect(m_layerRemoveObjectSelection, SIGNAL(triggered()), SLOT(onLayerRemoveSelectionTriggered()));
  connect(m_layerRemoveItem, SIGNAL(triggered()), SLOT(onLayerRemoveItemTriggered()));
  connect(m_layerSRS, SIGNAL(triggered()), SLOT(onLayerSRSTriggered()));
  connect(m_layerObjectGrouping, SIGNAL(triggered()), SLOT(onLayerGroupingTriggered()));
  connect(m_mapDraw, SIGNAL(triggered()), SLOT(onDrawTriggered()));
  connect(m_layerFitOnMapDisplay, SIGNAL(triggered()), SLOT(onLayerFitOnMapDisplayTriggered()));
  connect(m_layerFitSelectedOnMapDisplay, SIGNAL(triggered()), SLOT(onLayerFitSelectedOnMapDisplayTriggered()));
  connect(m_layerPanToSelectedOnMapDisplay, SIGNAL(triggered()), SLOT(onLayerPanToSelectedOnMapDisplayTriggered()));
  connect(m_layerCompositionMode, SIGNAL(hovered()), SLOT(onLayerCompositionModeTriggered()));
  connect(m_queryLayer, SIGNAL(triggered()), SLOT(onQueryLayerTriggered()));
  connect(m_mapZoomIn, SIGNAL(toggled(bool)), SLOT(onZoomInToggled(bool)));
  connect(m_mapZoomOut, SIGNAL(toggled(bool)), SLOT(onZoomOutToggled(bool)));
  connect(m_mapPreviousExtent, SIGNAL(triggered()), SLOT(onPreviousExtentTriggered()));
  connect(m_mapNextExtent, SIGNAL(triggered()), SLOT(onNextExtentTriggered()));
  connect(m_mapPan, SIGNAL(toggled(bool)), SLOT(onPanToggled(bool)));
  connect(m_mapZoomExtent, SIGNAL(triggered()), SLOT(onZoomExtentTriggered()));
  connect(m_mapInfo, SIGNAL(toggled(bool)), SLOT(onInfoToggled(bool)));
  connect(m_mapRemoveSelection, SIGNAL(triggered()), SLOT(onMapRemoveSelectionTriggered()));
  connect(m_mapSelection, SIGNAL(toggled(bool)), SLOT(onSelectionToggled(bool)));
  connect(m_mapMeasureDistance, SIGNAL(toggled(bool)), SLOT(onMeasureDistanceToggled(bool)));
  connect(m_mapMeasureArea, SIGNAL(toggled(bool)), SLOT(onMeasureAreaToggled(bool)));
  connect(m_mapMeasureAngle, SIGNAL(toggled(bool)), SLOT(onMeasureAngleToggled(bool)));
  connect(m_layerShowTable, SIGNAL(triggered()), SLOT(onLayerShowTableTriggered()));
  connect(m_viewFullScreen, SIGNAL(toggled(bool)), SLOT(onFullScreenToggled(bool)));
  connect(m_toolsDataSourceExplorer, SIGNAL(triggered()), SLOT(onDataSourceExplorerTriggered()));

  connect(m_viewLayerExplorer, SIGNAL(toggled(bool)), m_layerExplorer->getExplorer(), SLOT(setVisible(bool)));
  connect(m_layerExplorer->getExplorer(), SIGNAL(visibilityChanged(bool)), this, SLOT(onLayerExplorerVisibilityChanged(bool)));

  connect(m_display, SIGNAL(hasPreviousExtent(bool)), m_mapPreviousExtent, SLOT(setEnabled(bool)));
  connect(m_display, SIGNAL(hasNextExtent(bool)), m_mapNextExtent, SLOT(setEnabled(bool)));

  connect(m_viewStyleExplorer, SIGNAL(toggled(bool)), m_styleExplorer->getExplorer(), SLOT(setVisible(bool)));
  connect(m_styleExplorer->getExplorer(), SIGNAL(visibilityChanged(bool)), this, SLOT(onStyleExplorerVisibilityChanged(bool)));
  connect(m_styleExplorer->getExplorer(), SIGNAL(repaintMapDisplay()), this, SLOT(onDrawTriggered()));

  connect(m_viewDataTable, SIGNAL(toggled(bool)), this, SLOT(onDisplayDataTableChanged(bool)));
}

void TerraView::addMenusActions()
{
  // File menu
  m_fileMenu->setObjectName("File");
  m_fileMenu->setTitle(tr("&File"));

  m_recentProjectsMenu->setObjectName("File.Recent Projects");
  m_recentProjectsMenu->setTitle(tr("Recent &Projects"));

  m_fileMenu->addAction(m_fileNewProject);
  m_fileMenu->addAction(m_fileOpenProject);
  m_fileMenu->addAction(m_fileSaveProject);
  m_fileMenu->addAction(m_fileSaveProjectAs);
  m_fileMenu->addSeparator();
  m_fileMenu->addMenu(m_recentProjectsMenu);
  m_recentProjectsMenu->setEnabled(false);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_filePrintPreview);
  m_fileMenu->addAction(m_filePrint);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_fileRestartSystem);

#if TE_PLATFORM != TE_PLATFORMCODE_APPLE
  m_fileMenu->addSeparator();
#endif

  m_fileMenu->addAction(m_fileExit);

  // View menu
  m_viewMenu->setObjectName("View");
  m_viewMenu->setTitle(tr("&View"));

  m_viewToolBarsMenu->setObjectName("View.Toolbars");
  m_viewToolBarsMenu->setTitle(tr("&Toolbars"));

  m_viewMenu->addAction(m_viewDataTable);
  m_viewMenu->addAction(m_viewLayerExplorer);
  m_viewMenu->addAction(m_viewStyleExplorer);
  m_viewMenu->addSeparator();
  m_viewMenu->addAction(m_viewFullScreen);
  m_viewMenu->addSeparator();
  m_viewMenu->addMenu(m_viewToolBarsMenu);

  // Project menu
  m_projectMenu->setObjectName("Project");
  m_projectMenu->setTitle(tr("&Project"));

  m_projectAddLayerMenu->setObjectName("Project.Add Layer");
  m_projectAddLayerMenu->setTitle(tr("&Add Layer"));
  m_projectAddLayerMenu->setIcon(QIcon::fromTheme("layer-add"));
  m_projectAddLayerMenu->addAction(m_projectAddLayerTabularDataSet);
  m_projectAddLayerMenu->addAction(m_projectAddLayerDataset);
  m_projectAddLayerMenu->addSeparator();
  m_projectAddLayerMenu->addAction(m_projectAddLayerQueryDataSet);
  m_projectAddLayerMenu->addSeparator();
  m_projectMenu->addAction(m_projectAddFolderLayer);
  m_projectMenu->addSeparator();
  m_projectMenu->addAction(m_projectRemoveLayer);
  m_projectMenu->addAction(m_projectRenameLayer);
  m_projectMenu->addSeparator();
  m_projectMenu->addAction(m_projectProperties);

  m_layerMenu->setObjectName("Layer");
  m_layerMenu->setTitle(tr("&Layer"));

  m_layerMenu->addAction(m_layerObjectGrouping);
  m_layerMenu->addAction(m_layerChartsHistogram);
  m_layerMenu->addAction(m_layerChart);
  m_layerMenu->addAction(m_queryLayer);
  m_layerMenu->addAction(m_layerChartsScatter);
  m_layerMenu->addSeparator();
  m_layerMenu->addAction(m_layerFitOnMapDisplay);
  m_layerMenu->addAction(m_layerFitSelectedOnMapDisplay);
  m_layerMenu->addAction(m_layerPanToSelectedOnMapDisplay);
  m_layerMenu->addSeparator();
  m_layerMenu->addAction(m_layerShowTable);
  m_layerMenu->addAction(m_viewStyleExplorer);
  m_layerMenu->addSeparator();
  m_layerMenu->addAction(m_layerRemoveObjectSelection);
  m_layerMenu->addSeparator();
  m_layerMenu->addAction(m_layerSRS);
  m_layerMenu->addSeparator();
  m_layerMenu->addAction(m_layerProperties);

  // Map Menu
  m_mapMenu->setObjectName("Map");
  m_mapMenu->setTitle(tr("&Map"));

  m_mapMenu->addAction(m_mapDraw);
  m_mapMenu->addAction(m_mapStopDrawing);
  m_mapMenu->addSeparator();
  m_mapMenu->addAction(m_mapInfo);
  m_mapMenu->addAction(m_mapRemoveSelection);
  m_mapMenu->addAction(m_mapSelection);
  m_mapMenu->addSeparator();
  m_mapMenu->addAction(m_mapPan);
  m_mapMenu->addAction(m_mapZoomExtent);
  m_mapMenu->addAction(m_mapZoomIn);
  m_mapMenu->addAction(m_mapZoomOut);
  m_mapMenu->addSeparator();
  m_mapMenu->addAction(m_mapNextExtent);
  m_mapMenu->addAction(m_mapPreviousExtent);
  m_mapMenu->addSeparator();
  m_mapMenu->addAction(m_mapMeasureAngle);
  m_mapMenu->addAction(m_mapMeasureArea);
  m_mapMenu->addAction(m_mapMeasureDistance);
  m_mapMenu->addSeparator();
  m_mapMenu->addAction(m_mapSRID);
  m_mapMenu->addAction(m_mapUnknownSRID);

  // Group the map tools
  QActionGroup* mapToolsGroup = new QActionGroup(m_mapMenu);
  mapToolsGroup->setObjectName("Map.ToolsGroup");
  mapToolsGroup->addAction(m_mapZoomIn);
  mapToolsGroup->addAction(m_mapZoomOut);
  mapToolsGroup->addAction(m_mapPan);
  mapToolsGroup->addAction(m_mapMeasureDistance);
  mapToolsGroup->addAction(m_mapMeasureArea);
  mapToolsGroup->addAction(m_mapMeasureAngle);
  mapToolsGroup->addAction(m_mapInfo);
  mapToolsGroup->addAction(m_mapSelection);
  mapToolsGroup->addAction(m_mapRemoveSelection);

  // Tools menu
  m_toolsMenu->setObjectName("Tools");
  m_toolsMenu->setTitle(tr("&Tools"));

  m_toolsExchangerMenu->setObjectName("Tools.Exchanger");
  m_toolsExchangerMenu->setTitle(tr("&Data Exchanger"));
  m_toolsExchangerMenu->setIcon(QIcon::fromTheme("datasource-exchanger"));
  m_toolsExchangerMenu->addAction(m_toolsDataExchangerDirect);
  m_toolsExchangerMenu->addAction(m_toolsDataExchanger);

  m_toolsMenu->addAction(m_toolsDataSourceExplorer);
  m_toolsMenu->addAction(m_toolsQueryDataSource);
  m_toolsMenu->addSeparator();
  m_toolsMenu->addAction(m_toolsRasterMultiResolution);
  m_toolsMenu->addSeparator();
  m_toolsMenu->addAction(m_toolsCustomize);

  // Plugins menu
  m_pluginsMenu->setObjectName("Plugins");
  m_pluginsMenu->setTitle(tr("Pl&ugins"));

  m_pluginsMenu->addSeparator()->setObjectName("ManagePluginsSeparator");
  m_pluginsMenu->addAction(m_pluginsManager);

  // Help menu
  m_helpMenu->setObjectName("Help");
  m_helpMenu->setTitle(tr("&Help"));

  m_helpMenu->addAction(m_helpContents);
}

void TerraView::initMenus()
{
  te::qt::af::BaseApplication::initMenus();

  m_fileMenu = new QMenu(m_menubar);
  m_recentProjectsMenu = new QMenu(m_fileMenu);
  m_menubar->addAction(m_fileMenu->menuAction());
  m_viewMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_viewMenu->menuAction());
  m_viewToolBarsMenu = new QMenu(m_viewMenu);
  m_viewMenu->addMenu(m_viewToolBarsMenu);
  m_projectMenu = new QMenu(m_menubar);
  m_projectAddLayerMenu = new QMenu(m_projectMenu);
  m_menubar->addAction(m_projectMenu->menuAction());
  m_projectMenu->addMenu(m_projectAddLayerMenu);
  m_layerMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_layerMenu->menuAction());
  m_mapMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_mapMenu->menuAction());
  m_toolsMenu = new QMenu(m_menubar);
  m_toolsExchangerMenu = new QMenu(m_toolsMenu);
  m_toolsMenu->addAction(m_toolsExchangerMenu->menuAction());
  m_menubar->addAction(m_toolsMenu->menuAction());
  m_pluginsMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_pluginsMenu->menuAction());
  m_helpMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_helpMenu->menuAction());
}

void TerraView::initToolbars()
{
  std::vector<QToolBar*> bars = ReadToolBarsFromSettings(m_app, this);
  std::vector<QToolBar*>::iterator it;

  for (it = bars.begin(); it != bars.end(); ++it)
  {
    QToolBar* bar = *it;
    addToolBar(Qt::TopToolBarArea, bar);
    m_viewToolBarsMenu->addAction(bar->toggleViewAction());
    m_app->registerToolBar(bar->objectName(), bar);
  }
}

void TerraView::showAboutDialog()
{
  AboutDialog dialog(this);

  dialog.exec();
}

void TerraView::onHelpTriggered()
{
  te::qt::widgets::HelpManager::getInstance().showHelp("terraview/index.html", "dpi.inpe.br.terraview");
}

void TerraView::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch(e->m_id)
  {
    case te::qt::af::evt::NEW_ACTIONS_AVAILABLE:
    {
      te::qt::af::evt::NewActionsAvailable* evt = static_cast<te::qt::af::evt::NewActionsAvailable*>(e);

      if(evt->m_category == "Datasource")
      {
        //m_dsMenu->addActions(evt->m_actions);
      }
    }
      break;

    case te::qt::af::evt::LAYER_ADDED:
    case te::qt::af::evt::LAYER_REMOVED:
    case te::qt::af::evt::LAYER_VISIBILITY_CHANGED:
      projectChanged();
      break;

    default:
      BaseApplication::onApplicationTriggered(e);
  }
}

void TerraView::onShowTableTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> layers = getLayerExplorer()->getSelectedSingleLayerItems();

  if(layers.empty())
  {
    QMessageBox::warning(this, qApp->applicationName(), tr("There's no selected layer."));
    return;
  }
  else
    {
      std::list<te::qt::widgets::AbstractTreeItem*>::iterator it = layers.begin();

      while(it != layers.end())
      {
        if(!(*it)->getLayer()->isValid())
        {
          QMessageBox::warning(this, qApp->applicationName(), tr("There are invalid layers selected!"));
          return;
        }

        ++it;
      }
    }


  te::map::AbstractLayerPtr lay = (*layers.begin())->getLayer();

  if (lay->getSchema()->hasRaster())
    return;

  te::qt::af::DataSetTableDockWidget* doc = GetLayerDock(lay.get(), m_tables);

  if(doc == 0)
  {
    doc = new te::qt::af::DataSetTableDockWidget(this);
    doc->setLayer(lay.get());
    addDockWidget(Qt::BottomDockWidgetArea, doc);

    connect (doc, SIGNAL(closed(te::qt::af::DataSetTableDockWidget*)), SLOT(onLayerTableClose(te::qt::af::DataSetTableDockWidget*)));
//    connect (doc, SIGNAL(createChartDisplay(te::qt::widgets::ChartDisplayWidget*, te::map::AbstractLayer*)), SLOT(onChartDisplayCreated(te::qt::widgets::ChartDisplayWidget*, te::map::AbstractLayer*)));

    if(!m_tables.empty())
      tabifyDockWidget(m_tables[m_tables.size()-1], doc);

    m_tables.push_back(doc);

    m_app->addListener(doc);
  }

  doc->show();
  doc->raise();
}

void TerraView::onLayerTableClose(te::qt::af::DataSetTableDockWidget* wid)
{
  std::vector<te::qt::af::DataSetTableDockWidget*>::iterator it;

  for(it=m_tables.begin(); it!=m_tables.end(); ++it)
    if(*it == wid)
      break;

  if(it != m_tables.end())
  {
    m_app->removeListener(*it);
    m_tables.erase(it);
  }
}

void TerraView::onSaveProjectTriggered()
{
  QString projFile = m_project->m_fileName;

  if(projFile.isEmpty())
  {
    QFileDialog d(this);

    QString filter = tr("TerraView project(*.") + m_app->getAppProjectExtension() + ")";

    QString fileName = d.getSaveFileName(this, tr("Save project"), qApp->applicationDirPath(), filter);

    if(fileName.isEmpty())
      return;

    QFileInfo info(fileName);

    if(info.suffix().isEmpty())
      fileName.append("." + m_app->getAppProjectExtension());

    m_project->m_fileName = fileName;
    m_project->m_title = info.baseName();
  }

  SaveProject(*m_project, getLayerExplorer()->getTopLayers());

  m_project->m_changed = false;

  setWindowTitle(m_app->getAppName() + " - " + m_project->m_title);

  AddRecentProjectToSettings(m_project->m_title, m_project->m_fileName);
}

void TerraView::onOpenProjectTriggered()
{
  checkAndSaveProject();

  QFileDialog d(this);

  QString filter = tr("TerraView project(*.") + m_app->getAppProjectExtension() + ")";

  QString fileName = d.getOpenFileName(this, tr("Open project"), qApp->applicationDirPath(), filter);

  if(fileName.isEmpty())
    return;

  openProject(fileName);
}

void TerraView::onNewProjectTriggered()
{
  checkAndSaveProject();

  m_project->m_fileName = "";
  m_project->m_title = tr("Default Project");
  m_project->m_changed = false;

  setWindowTitle(m_app->getAppName() + " - " + m_project->m_title);

  std::list<te::map::AbstractLayerPtr> ls;

  getLayerExplorer()->set(ls);

  getMapDisplay()->setLayerList(ls);
}

void TerraView::projectChanged()
{
  if(m_project->m_changed)
    return;

  m_project->m_changed = true;

  setWindowTitle(windowTitle() + " *");
}

void TerraView::checkAndSaveProject()
{
  if(m_project->m_changed)
  {
    QString msg = tr("The current project has unsaved changes. Do you want to save them?");
    QMessageBox::StandardButton btn = QMessageBox::question(this, windowTitle(), msg, QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);

    if(btn == QMessageBox::Save)
      onSaveProjectTriggered();
  }
}

void TerraView::openProject(const QString& prjFileName)
{
  std::list<te::map::AbstractLayerPtr> lst;

  LoadProject(prjFileName, *m_project, lst);

  getLayerExplorer()->set(lst);

  m_project->m_changed = false;

  setWindowTitle(m_app->getAppName() + " - " + m_project->m_title);

  AddRecentProjectToSettings(m_project->m_title, m_project->m_fileName);
}

void TerraView::closeEvent(QCloseEvent* event)
{
  checkAndSaveProject();

  QMainWindow::close();
}

void TerraView::startProject(const QString& projectFileName)
{
  openProject(projectFileName);
}

