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
#include "ProjectInfoDialog.h"

// TerraLib
#include <terralib/common/Exception.h>
#include <terralib/common/PlatformUtils.h>
#include <terralib/common/Translator.h>
#include <terralib/dataaccess/datasource/DataSourceInfoManager.h>
#include <terralib/maptools/Utils.h>
#include <terralib/qt/af/ApplicationController.h>
#include <terralib/qt/af/connectors/ChartDisplayDockWidget.h>
#include <terralib/qt/af/connectors/DataSetTableDockWidget.h>
#include <terralib/qt/af/connectors/InterfaceController.h>
#include <terralib/qt/af/connectors/LayerExplorer.h>
#include <terralib/qt/af/connectors/MapDisplay.h>
#include <terralib/qt/af/connectors/StyleExplorer.h>
#include <terralib/qt/af/events/ApplicationEvents.h>
#include <terralib/qt/af/events/LayerEvents.h>
#include <terralib/qt/af/events/MapEvents.h>
#include <terralib/qt/af/events/ProjectEvents.h>
#include <terralib/qt/af/events/ToolEvents.h>
#include <terralib/qt/af/settings/SettingsDialog.h>
#include <terralib/qt/af/Utils.h>
#include <terralib/qt/widgets/canvas/EyeBirdMapDisplayWidget.h>
#include <terralib/qt/widgets/canvas/MultiThreadMapDisplay.h>
#include <terralib/qt/widgets/canvas/ZoomInMapDisplayWidget.h>
#include <terralib/qt/widgets/charts/ChartLayerDialog.h>
#include <terralib/qt/widgets/charts/HistogramDialog.h>
#include <terralib/qt/widgets/charts/ScatterDialog.h>
#include <terralib/qt/widgets/datasource/core/DataSourceType.h>
#include <terralib/qt/widgets/datasource/core/DataSourceTypeManager.h>
#include <terralib/qt/widgets/datasource/connector/AbstractDataSourceConnector.h>
#include <terralib/qt/widgets/datasource/selector/DataSourceExplorerDialog.h>
#include <terralib/qt/widgets/datasource/selector/DataSourceSelectorDialog.h>
#include <terralib/qt/widgets/exchanger/DataExchangerWizard.h>
#include <terralib/qt/widgets/exchanger/DirectExchangerDialog.h>
#include <terralib/qt/widgets/externalTable/DataPropertiesDialog.h>
#include <terralib/qt/widgets/externalTable/TableLinkDialog.h>
#include <terralib/qt/widgets/help/HelpManager.h>
//#include <terralib/qt/widgets/layer/explorer/ColorMapItem.h>
//#include <terralib/qt/widgets/layer/explorer/ChartItem.h>
//#include <terralib/qt/widgets/layer/explorer/GroupingItem.h>
//#include <terralib/qt/widgets/layer/explorer/LayerExplorer.h>
#include <terralib/qt/widgets/layer/explorer/FolderItem.h>
#include <terralib/qt/widgets/layer/explorer/LayerItemView.h>
#include <terralib/qt/widgets/layer/explorer/LayerItem.h>
//#include <terralib/qt/widgets/layer/explorer/AbstractTreeItem.h>
//#include <terralib/qt/widgets/layer/explorer/FolderLayerItem.h>
#include <terralib/qt/widgets/layer/info/LayerPropertiesInfoWidget.h>
#include <terralib/qt/widgets/layer/selector/AbstractLayerSelector.h>
#include <terralib/qt/widgets/layer/utils/CompositionModeMenuWidget.h>
#include <terralib/qt/widgets/plugin/manager/PluginManagerDialog.h>
#include <terralib/qt/widgets/progress/ProgressViewerBar.h>
#include <terralib/qt/widgets/progress/ProgressViewerDialog.h>
#include <terralib/qt/widgets/progress/ProgressViewerWidget.h>
#include <terralib/qt/widgets/query/QueryDataSourceDialog.h>
#include <terralib/qt/widgets/query/QueryDialog.h>
#include <terralib/qt/widgets/query/QueryLayerBuilderWizard.h>
#include <terralib/qt/widgets/raster/MultiResolutionDialog.h>
#include <terralib/qt/widgets/se/GroupingDialog.h>
#include <terralib/qt/widgets/se/StyleDockWidget.h>
#include <terralib/qt/widgets/tools/Info.h>
#include <terralib/qt/widgets/tools/Measure.h>
#include <terralib/qt/widgets/tools/Pan.h>
#include <terralib/qt/widgets/tools/Selection.h>
#include <terralib/qt/widgets/tools/ZoomArea.h>
#include <terralib/qt/widgets/tools/ZoomClick.h>
#include <terralib/qt/widgets/srs/SRSManagerDialog.h>
#include <terralib/qt/widgets/vector/FixGeometryDialog.h>

// STL
#include <memory>

// Qt
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


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

QModelIndex GetParent(QTreeView* view)
{
  QModelIndex res;

  QModelIndexList idxs = view->selectionModel()->selectedIndexes();

  if(idxs.size() == 1)
  {
    QModelIndex idx = idxs.at(0);
    te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer());

    if(item->getType() == "FOLDER")
      res = idx;
  }

  return res;
}

std::list<te::map::AbstractLayerPtr> GetSelectedLayersOnly(te::qt::widgets::LayerItemView* view)
{
  std::list<te::map::AbstractLayerPtr> res;
  std::list<te::qt::widgets::TreeItem*> items = view->getSelectedItems();

  for(std::list<te::qt::widgets::TreeItem*>::iterator it = items.begin(); it != items.end(); ++it)
    if((*it)->getType() == "LAYER")
      res.push_back(((te::qt::widgets::LayerItem*)(*it))->getLayer());

  return res;
}

void GetValidLayers(QAbstractItemModel* model, const QModelIndex& parent, std::vector<te::map::AbstractLayerPtr>& layers)
{
  int cs = model->rowCount(parent);

  for(int i = 0; i < cs; i++)
  {
    QModelIndex idx = model->index(i, 0, parent);

    if(idx.isValid())
    {
      te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer());

      if(item->getType() == "LAYER")
      {
        te::map::AbstractLayerPtr l = ((te::qt::widgets::LayerItem*)item)->getLayer();

        if(l->isValid())
          layers.push_back(l);
      }
      else if(item->getType() == "FOLDER")
        GetValidLayers(model, idx, layers);
    }
  }
}

void GetValidLayers(QAbstractItemModel* model, const QModelIndex& parent, std::list<te::map::AbstractLayerPtr>& layers)
{
  int cs = model->rowCount(parent);

  for(int i = 0; i < cs; i++)
  {
    QModelIndex idx = model->index(i, 0, parent);

    if(idx.isValid())
    {
      te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer());

      if(item->getType() == "LAYER")
      {
        te::map::AbstractLayerPtr l = ((te::qt::widgets::LayerItem*)item)->getLayer();

        if(l->isValid())
          layers.push_back(l);
      }
      else if(item->getType() == "FOLDER")
        GetValidLayers(model, idx, layers);
    }
  }
}

TerraView::TerraView(QWidget* parent)
  : te::qt::af::BaseApplication(parent),
    m_mapCursorSize(QSize(20, 20)),
    m_helpManager(0),
    m_iController(0),
    m_queryDlg(0),
    m_compModeMenu(0)
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
  if (m_iController)
    m_iController->removeInteface(m_queryDlg);

  delete m_iController;
  delete m_compModeMenu;
  delete m_queryDlg;
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

  addPopUpMenu();

  //composition mode
  m_compModeMenu = new te::qt::widgets::CompositionModeMenuWidget();
  m_layerCompositionMode->setMenu(m_compModeMenu->getMenu());

  //interface controller
  m_iController = new te::qt::af::InterfaceController(this);
  m_app->addListener(m_iController, te::qt::af::RECEIVER);

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
//  connect(m_layerExplorer->getExplorer(), SIGNAL(visibilityChanged(bool)), this, SLOT(onLayerExplorerVisibilityChanged(bool)));

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

void TerraView::addPopUpMenu()
{
  te::qt::widgets::LayerItemView* treeView = m_layerExplorer->getExplorer();
  treeView->setAnimated(true);

  //// Actions to be added to the context menu when there is no item selected
  treeView->addNoLayerAction(m_projectAddLayerMenu->menuAction());

  QAction* noItemSelectedSep = new QAction(this);
  noItemSelectedSep->setSeparator(true);
  treeView->addNoLayerAction(noItemSelectedSep);

  treeView->addNoLayerAction(m_projectAddFolderLayer);

  //// Actions to be added to the context menu when there is a unique item selected

  //// Actions for the folder layer item
  treeView->addFolderLayerAction(m_projectAddLayerMenu->menuAction());

  QAction* folderSep1 = new QAction(this);
  folderSep1->setSeparator(true);
  treeView->addFolderLayerAction(folderSep1);

  treeView->addFolderLayerAction(m_projectAddFolderLayer);

  QAction* folderSep2 = new QAction(this);
  folderSep2->setSeparator(true);
  treeView->addFolderLayerAction(folderSep2);

  treeView->addFolderLayerAction(m_projectRemoveLayer);
  treeView->addFolderLayerAction(m_projectRenameLayer);

  //// Actions for the single layer item that is not a raster layer
  treeView->addVectorLayerAction(m_layerObjectGrouping);
  treeView->addVectorLayerAction(m_toolsDataExchangerDirectPopUp);
  treeView->addVectorLayerAction(m_layerChartsHistogram);
  treeView->addVectorLayerAction(m_layerChart);
  treeView->addVectorLayerAction(m_queryLayer);
  treeView->addVectorLayerAction(m_layerChartsScatter);
  treeView->addVectorLayerAction(m_layerLinkTable);

  QAction* actionChartSep = new QAction(this);
  actionChartSep->setSeparator(true);
  treeView->addVectorLayerAction(actionChartSep);

  treeView->addVectorLayerAction(m_layerShowTable);
  treeView->addVectorLayerAction(m_viewStyleExplorer);

  QAction* actionStyleSep = new QAction(this);
  actionStyleSep->setSeparator(true);
  treeView->addVectorLayerAction(actionStyleSep);

  treeView->addVectorLayerAction(m_layerRemoveObjectSelection);
  treeView->addVectorLayerAction(m_projectRemoveLayer);
  treeView->addVectorLayerAction(m_projectRenameLayer);

  QAction* actionRemoveSep = new QAction(this);
  actionRemoveSep->setSeparator(true);
  treeView->addVectorLayerAction(actionRemoveSep);

  treeView->addVectorLayerAction(m_layerFitOnMapDisplay);
  treeView->addVectorLayerAction(m_layerFitSelectedOnMapDisplay);
  treeView->addVectorLayerAction(m_layerPanToSelectedOnMapDisplay);

  QAction* actionFitSep = new QAction(this);
  actionFitSep->setSeparator(true);
  treeView->addVectorLayerAction(actionFitSep);

  treeView->addVectorLayerAction(m_layerSRS);

  QAction* actionSRSSep = new QAction(this);
  actionSRSSep->setSeparator(true);
  treeView->addVectorLayerAction(actionSRSSep);

  treeView->addVectorLayerAction(m_layerCompositionMode);
  treeView->addVectorLayerAction(m_layerProperties);

  //// Actions for the items of a layer item such as the chart item and the grouping item
  //treeView->add(m_layerRemoveItem, "", "ITEM_OF_LAYER");

  //// Actions for the raster layer item
  treeView->addRasterLayerAction(m_layerObjectGrouping);
  treeView->addRasterLayerAction(m_layerChartsHistogram);
  treeView->addRasterLayerAction(m_layerChartsScatter);

  QAction* rasterSep1 = new QAction(this);
  rasterSep1->setSeparator(true);
  treeView->addRasterLayerAction(rasterSep1);

  treeView->addRasterLayerAction(m_viewStyleExplorer);

  QAction* rasterSep2 = new QAction(this);
  rasterSep2->setSeparator(true);
  treeView->addRasterLayerAction(rasterSep2);

  treeView->addRasterLayerAction(m_projectRemoveLayer);
  treeView->addRasterLayerAction(m_projectRenameLayer);

  QAction* rasterSep3 = new QAction(this);
  rasterSep3->setSeparator(true);
  treeView->addRasterLayerAction(rasterSep3);

  treeView->addRasterLayerAction(m_layerFitOnMapDisplay);

  QAction* rasterSep4 = new QAction(this);
  rasterSep4->setSeparator(true);
  treeView->addRasterLayerAction(rasterSep4);

  treeView->addRasterLayerAction(m_layerSRS);

  QAction* rasterSep5 = new QAction(this);
  rasterSep5->setSeparator(true);
  treeView->addRasterLayerAction(rasterSep5);

  treeView->addRasterLayerAction(m_layerCompositionMode);
  treeView->addRasterLayerAction(m_layerProperties);

  //// Actions for invalid layers
  //treeView->add(m_projectChangeLayerDataSource, "", "INVALID_LAYER_ITEM");
  //treeView->add(m_projectUpdateLayerDataSource, "", "INVALID_LAYER_ITEM");
  //treeView->add(m_projectRemoveLayer, "", "INVALID_LAYER_ITEM");

  //// Actions to be added to the context menu when there are multiple items selected
  //treeView->addMultipleSelectionAction(m_layerFitSelectedOnMapDisplay);
  //treeView->addMultipleSelectionAction(m_layerPanToSelectedOnMapDisplay);

  //treeView->add(m_layerFitSelectedOnMapDisplay, "", "QUERY_LAYER_ITEM", te::qt::widgets::LayerTreeView::MULTIPLE_ITEMS_SELECTED);
  //treeView->add(m_layerPanToSelectedOnMapDisplay, "", "QUERY_LAYER_ITEM", te::qt::widgets::LayerTreeView::MULTIPLE_ITEMS_SELECTED);

  QAction* multipleSelectedSep = new QAction(this);
  multipleSelectedSep->setSeparator(true);
  treeView->addMultipleSelectionAction(multipleSelectedSep);

  treeView->addMultipleSelectionAction(m_projectRemoveLayer);
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
    case te::qt::af::evt::PROJECT_UNSAVED:
      projectChanged();
      break;

    default:
      BaseApplication::onApplicationTriggered(e);
  }
}

void TerraView::onShowTableTriggered()
{
  //Revisar: Fred
  //  std::list<te::qt::widgets::AbstractTreeItem*> layers = getLayerExplorer()->getSelectedSingleLayerItems();
//
//  if(layers.empty())
//  {
//    QMessageBox::warning(this, qApp->applicationName(), tr("There's no selected layer."));
//    return;
//  }
//  else
//    {
//      std::list<te::qt::widgets::AbstractTreeItem*>::iterator it = layers.begin();
//
//      while(it != layers.end())
//      {
//        if(!(*it)->getLayer()->isValid())
//        {
//          QMessageBox::warning(this, qApp->applicationName(), tr("There are invalid layers selected!"));
//          return;
//        }
//
//        ++it;
//      }
//    }
//
//
//  te::map::AbstractLayerPtr lay = (*layers.begin())->getLayer();
//
//  if (lay->getSchema()->hasRaster())
//    return;
//
//  te::qt::af::DataSetTableDockWidget* doc = GetLayerDock(lay.get(), m_tables);
//
//  if(doc == 0)
//  {
//    doc = new te::qt::af::DataSetTableDockWidget(this);
//    doc->setLayer(lay.get());
//    addDockWidget(Qt::BottomDockWidgetArea, doc);
//
//    connect (doc, SIGNAL(closed(te::qt::af::DataSetTableDockWidget*)), SLOT(onLayerTableClose(te::qt::af::DataSetTableDockWidget*)));
////    connect (doc, SIGNAL(createChartDisplay(te::qt::widgets::ChartDisplayWidget*, te::map::AbstractLayer*)), SLOT(onChartDisplayCreated(te::qt::widgets::ChartDisplayWidget*, te::map::AbstractLayer*)));
//
//    if(!m_tables.empty())
//      tabifyDockWidget(m_tables[m_tables.size()-1], doc);
//
//    m_tables.push_back(doc);
//
//    m_app->addListener(doc);
//  }
//
//  doc->show();
//  doc->raise();
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

void TerraView::onOpenProjectTriggered()
{
  checkAndSaveProject();

  QString file = QFileDialog::getOpenFileName(this, tr("Open project file"), qApp->applicationDirPath(), te::qt::af::GetExtensionFilter());

  if (file.isEmpty())
    return;

  try
  {
    openProject(file);
  }
  catch (const te::common::Exception& e)
  {
    QString msg = tr("Fail to open project.");
    msg += " ";
    msg += e.what();
    QMessageBox::warning(this, m_app->getAppTitle(), msg);
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

  SaveProject(*m_project, getLayerExplorer()->getAllLayers());

  m_project->m_changed = false;

  setWindowTitle(m_app->getAppName() + " - " + m_project->m_title);

  AddRecentProjectToSettings(m_project->m_title, m_project->m_fileName);

  m_app->updateRecentProjects(m_project->m_fileName, m_project->m_title);

  te::qt::af::SaveDataSourcesFile();
}

void TerraView::onAddDataSetLayerTriggered()
{
  try
  {
    if (m_project == 0)
      throw te::common::Exception(TE_TR("Error: there is no opened project!"));

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Get the parent layer where the dataset layer(s) will be added.
    QModelIndex pF = GetParent(getLayerExplorer());

    // Get the layer(s) to be added
    std::auto_ptr<te::qt::widgets::DataSourceSelectorDialog> dselector(new te::qt::widgets::DataSourceSelectorDialog(this));

    QString dsTypeSett = te::qt::af::GetLastDatasourceFromSettings();

    if (!dsTypeSett.isNull() && !dsTypeSett.isEmpty())
      dselector->setDataSourceToUse(dsTypeSett);

    QApplication::restoreOverrideCursor();

    int retval = dselector->exec();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (retval == QDialog::Rejected)
    {
      QApplication::restoreOverrideCursor();
      return;
    }

    std::list<te::da::DataSourceInfoPtr> selectedDatasources = dselector->getSelecteds();

    if (selectedDatasources.empty())
    {
      QApplication::restoreOverrideCursor();
      return;
    }

    dselector.reset(0);

    const std::string& dsTypeId = selectedDatasources.front()->getType();

    const te::qt::widgets::DataSourceType* dsType = te::qt::widgets::DataSourceTypeManager::getInstance().get(dsTypeId);

    std::auto_ptr<QWidget> lselectorw(dsType->getWidget(te::qt::widgets::DataSourceType::WIDGET_LAYER_SELECTOR, this));

    if (lselectorw.get() == 0)
    {
      QApplication::restoreOverrideCursor();
      throw te::common::Exception((boost::format(TE_TR("No layer selector widget found for this type of data source: %1%!")) % dsTypeId).str());
    }

    te::qt::widgets::AbstractLayerSelector* lselector = dynamic_cast<te::qt::widgets::AbstractLayerSelector*>(lselectorw.get());

    if (lselector == 0)
    {
      QApplication::restoreOverrideCursor();
      throw te::common::Exception(TE_TR("Wrong type of object for layer selection!"));
    }

    lselector->set(selectedDatasources);

    QApplication::restoreOverrideCursor();

    std::list<te::map::AbstractLayerPtr> layers = lselector->getLayers();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    lselectorw.reset(0);

    getLayerExplorer()->addLayers(layers, pF);

    te::qt::af::SaveLastDatasourceOnSettings(dsTypeId.c_str());

    projectChanged();
  }
  catch (const std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
  catch (...)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this,
      m_app->getAppTitle(),
      tr("Unknown error while trying to add a layer from a dataset!"));
  }

  QApplication::restoreOverrideCursor();
}

void TerraView::onAddQueryLayerTriggered()
{
  try
  {
    if (m_project == 0)
      throw te::common::Exception(TE_TR("Error: there is no opened project!"));

    // Get the parent layer where the dataset layer(s) will be added.
    QModelIndex par = GetParent(getLayerExplorer());

    std::auto_ptr<te::qt::widgets::QueryLayerBuilderWizard> qlb(new te::qt::widgets::QueryLayerBuilderWizard(this));

    std::list<te::map::AbstractLayerPtr> layers;
    GetValidLayers(getLayerExplorer()->model(), QModelIndex(), layers);

    qlb->setLayerList(layers);

    int retval = qlb->exec();

    if (retval == QDialog::Rejected)
      return;

    te::map::AbstractLayerPtr layer = qlb->getQueryLayer();

    if ((m_layerExplorer != 0) && (m_layerExplorer->getExplorer() != 0))
    {
      std::list<te::map::AbstractLayerPtr> ls;
      ls.push_back(layer);

      getLayerExplorer()->addLayers(ls, par);

      projectChanged();
    }
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
  catch (...)
  {
    QMessageBox::warning(this,
      m_app->getAppTitle(),
      tr("Unknown error while trying to add a layer from a queried dataset!"));
  }
}

void TerraView::onAddTabularLayerTriggered()
{
  try
  {
    if (m_project == 0)
      throw te::common::Exception(TE_TR("Error: there is no opened project!"));

    // Get the parent layer where the tabular layer will be added.
    QModelIndex par = GetParent(getLayerExplorer());

    te::qt::widgets::DataPropertiesDialog dlg(this);
    int res = dlg.exec();

    if (res == QDialog::Accepted)
    {
      if((m_layerExplorer != 0) && (m_layerExplorer->getExplorer() != 0))
      {
        std::list<te::map::AbstractLayerPtr> ls;
        ls.push_back(dlg.getDataSetAdapterLayer());

        getLayerExplorer()->addLayers(ls, par);

        projectChanged();
      }
    }
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
  catch (...)
  {
    QMessageBox::warning(this,
      m_app->getAppTitle(),
      tr("Unknown error while trying to add a layer from a queried dataset!"));
  }
}

void TerraView::onChartDisplayCreated(te::qt::widgets::ChartDisplayWidget* chartDisplay, te::map::AbstractLayer* layer)
{
  try
  {
    te::qt::af::ChartDisplayDockWidget* doc = new te::qt::af::ChartDisplayDockWidget(chartDisplay, this);
    doc->setSelectionColor(m_app->getSelectionColor());
    doc->setWindowTitle("Histogram");
    doc->setWindowIcon(QIcon::fromTheme("chart-bar"));
    doc->setLayer(layer);

    m_app->addListener(doc);
    addDockWidget(Qt::RightDockWidgetArea, doc, Qt::Horizontal);
    doc->show();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onRemoveLayerTriggered()
{
  QModelIndexList ls = getLayerExplorer()->selectionModel()->selectedIndexes();

  if (ls.isEmpty())
  {
    QString msg = tr("Select at least one layer to be removed!");
    QMessageBox::warning(this, tr("Remove Layer"), msg);

    return;
  }

  QString msg;
  QString questionTitle;

  if(ls.size() == 1)
  {
    msg = tr("Do you really want to remove the selected layer?");
    questionTitle = tr("Remove Layer");
  }
  else
  {
    msg = tr("Do you really want to remove the selected layers?");
    questionTitle = tr("Remove Layers");
  }

  int reply = QMessageBox::question(this, questionTitle, msg, QMessageBox::No, QMessageBox::Yes);

  if (reply == QMessageBox::No)
    return;

  std::list<te::map::AbstractLayerPtr> lays = GetSelectedLayersOnly(getLayerExplorer());

  getLayerExplorer()->removeSelectedItems();

  bool needRefresh = false;
  for(std::list<te::map::AbstractLayerPtr>::iterator it = lays.begin(); it != lays.end(); ++it)
  {
    if((*it)->getVisibility() == te::map::VISIBLE)
      needRefresh = true;

    te::qt::af::evt::LayerRemoved e(*it);
    emit triggered(&e);
  }

  if(needRefresh)
  {
    lays.clear();
    GetValidLayers(getLayerExplorer()->model(), QModelIndex(), lays);
    getMapDisplay()->setLayerList(lays);
    getMapDisplay()->refresh();
  }

  projectChanged();
}

void TerraView::onChangeLayerDataSourceTriggered()
{
  try
  {
    std::list<te::qt::widgets::TreeItem*> selectedLayerItems = getLayerExplorer()->getSelectedItems();

    // Get Data Source
    std::auto_ptr<te::qt::widgets::DataSourceSelectorDialog> dselector(new te::qt::widgets::DataSourceSelectorDialog(this));

    QString dsTypeSett = te::qt::af::GetLastDatasourceFromSettings();

    if (!dsTypeSett.isNull() && !dsTypeSett.isEmpty())
      dselector->setDataSourceToUse(dsTypeSett);

    int retval = dselector->exec();

    if (retval == QDialog::Rejected)
      return;

    std::list<te::da::DataSourceInfoPtr> selectedDatasources = dselector->getSelecteds();

    if (selectedDatasources.empty())
      return;

    dselector.reset(0);

    const std::string& dsId = selectedDatasources.front()->getId();

    te::map::AbstractLayerPtr layer = ((te::qt::widgets::LayerItem*) selectedLayerItems.front())->getLayer();

    te::map::DataSetLayer* dsl = (te::map::DataSetLayer*)layer.get();

    if (dsl)
      dsl->setDataSourceId(dsId);

    projectChanged();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
  catch (...)
  {
    QMessageBox::warning(this,
      m_app->getAppTitle(),
      tr("Unknown error while trying to change a layer data source!"));
  }
}

void TerraView::onUpdateLayerDataSourceTriggered()
{
  try
  {
    std::list<te::qt::widgets::TreeItem*> selectedLayerItems = getLayerExplorer()->getSelectedItems();

    if (selectedLayerItems.empty())
      return;

    te::map::AbstractLayerPtr layer = ((te::qt::widgets::LayerItem*)selectedLayerItems.front())->getLayer();

    te::map::DataSetLayer* dsl = (te::map::DataSetLayer*)layer.get();

    if (!dsl)
      return;

    std::list<te::da::DataSourceInfoPtr> selecteds;

    te::da::DataSourceInfoPtr ds = te::da::DataSourceInfoManager::getInstance().get(dsl->getDataSourceId());

    selecteds.push_back(ds);

    const std::string& dsTypeId = selecteds.front()->getType();

    const te::qt::widgets::DataSourceType* dsType = te::qt::widgets::DataSourceTypeManager::getInstance().get(dsTypeId);

    std::auto_ptr<QWidget> connectorw(dsType->getWidget(te::qt::widgets::DataSourceType::WIDGET_DATASOURCE_CONNECTOR, this));

    if (connectorw.get() == 0)
    {
      throw te::common::Exception((boost::format(TE_TR("No layer selector widget found for this type of data source: %1%!")) % dsTypeId).str());
    }

    te::qt::widgets::AbstractDataSourceConnector* connector = dynamic_cast<te::qt::widgets::AbstractDataSourceConnector*>(connectorw.get());

    if (connector == 0)
    {
      throw te::common::Exception(TE_TR("Wrong type of object for layer selection!"));
    }

    connector->update(selecteds);
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
  catch (...)
  {
    QMessageBox::warning(this,
      m_app->getAppTitle(),
      tr("Unknown error while trying to update a layer data source!"));
  }
}

void TerraView::onLayerRemoveItemTriggered()
{
  getLayerExplorer()->removeSelectedItems();

  projectChanged();
}

void TerraView::onRenameLayerTriggered()
{
  std::list<te::qt::widgets::TreeItem*> selectedLayerItems = getLayerExplorer()->getSelectedItems();

  bool renameAllowed = !selectedLayerItems.empty() && selectedLayerItems.size() == 1 && (selectedLayerItems.front()->getType() == "LAYER" || selectedLayerItems.front()->getType() == "FOLDER");

  if(!renameAllowed)
  {
    QString msg = tr("Select only one layer to be renamed!");
    QMessageBox::warning(this, tr("Rename Layer"), msg);

    return;
  }

  te::qt::widgets::TreeItem* item = selectedLayerItems.front();

  bool ok;
  QString text = QInputDialog::getText(this, m_app->getAppTitle(),
    tr("Rename Layer:"), QLineEdit::Normal,
    item->getAsString().c_str(), &ok);

  if (!ok)
    return;

  if (text.isEmpty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("Enter the new name!"));
    return;
  }

  if(item->getType() == "LAYER")
    ((te::qt::widgets::LayerItem*)item)->getLayer()->setTitle(text.toStdString());
  else if(item->getType() == "FOLDER")
    ((te::qt::widgets::FolderItem*)item)->setTitle(text.toStdString());

  projectChanged();
}

void TerraView::onPluginsManagerTriggered()
{
  try
  {
    te::qt::widgets::PluginManagerDialog dlg(this);
    dlg.exec();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onRecentProjectsTriggered(QAction* proj)
{
  QString projFile = proj->data().toString();

  openProject(projFile);
}

//void TerraView::onNewProjectTriggered()
//{
////  newProject();
//
//  //onSaveProjectAsTriggered();
//}

void TerraView::onSaveProjectAsTriggered()
{
  if (m_project == 0)
    return;

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), qApp->applicationDirPath(), te::qt::af::GetExtensionFilter());

  if (fileName.isEmpty())
    return;

  std::string fName = fileName.toStdString();

  m_project->m_fileName = fName.c_str();

  if (!boost::filesystem::exists(fName) && m_project->m_title == "New Project")
  {
    m_project->m_title = (boost::filesystem::basename(fName)).c_str();
  }

//  te::qt::af::Save(*m_project, fName);

  m_app->updateRecentProjects(fileName, m_project->m_title);

  // Set the project title and its status as "no change"
  //std::string projectTitle = boost::filesystem::basename(m_project->getFileName());
  //m_project->setTitle(projectTitle);

  m_project->m_changed = false;

  // Set the window title
 // setWindowTitle(te::qt::af::GetWindowTitle(*m_project));

  te::qt::af::SaveDataSourcesFile();
}

void TerraView::onRestartSystemTriggered()
{
  QMessageBox msgBox(this);

  msgBox.setText(tr("The system will be restarted."));
  msgBox.setInformativeText(tr("Do you want to continue?"));
  msgBox.setWindowTitle(tr("Restart system"));

  msgBox.addButton(QMessageBox::No);
  msgBox.addButton(QMessageBox::Yes);

  msgBox.setDefaultButton(QMessageBox::Yes);

  if (msgBox.exec() == QMessageBox::Yes)
    qApp->exit(1000);
}

void TerraView::onToolsCustomizeTriggered()
{
  try
  {
    te::qt::af::SettingsDialog dlg(this);
    dlg.exec();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onToolsDataExchangerTriggered()
{
  try
  {
    te::qt::widgets::DataExchangerWizard dlg(this);
    dlg.exec();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onToolsDataExchangerDirectTriggered()
{
  try
  {
    te::qt::widgets::DirectExchangerDialog dlg(this);

    std::list<te::map::AbstractLayerPtr> layers;
    GetValidLayers(getLayerExplorer()->model(), QModelIndex(), layers);

    dlg.setLayers(layers);

    QString dsTypeSett = te::qt::af::GetLastDatasourceFromSettings();

    if (!dsTypeSett.isNull() && !dsTypeSett.isEmpty())
      dlg.setLastDataSource(dsTypeSett.toStdString());

    dlg.exec();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onToolsDataExchangerDirectPopUpTriggered()
{
  try
  {
    te::qt::widgets::DirectExchangerDialog dlg(this);

    std::list<te::map::AbstractLayerPtr> selectedLayerItems = GetSelectedLayersOnly(getLayerExplorer());

    if (selectedLayerItems.empty())
    {
      QMessageBox::warning(this, m_app->getAppTitle(),
        tr("Select a single layer in the layer explorer!"));
      return;
    }

    std::list<te::map::AbstractLayerPtr> layers;
    layers.push_back(selectedLayerItems.front());

    dlg.setLayers(layers);

    dlg.exec();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onToolsQueryDataSourceTriggered()
{
  try
  {
    te::qt::widgets::QueryDataSourceDialog dlg(this);

    connect(&dlg, SIGNAL(createNewLayer(te::map::AbstractLayerPtr)), this, SLOT(onCreateNewLayer(te::map::AbstractLayerPtr)));

    std::list<te::map::AbstractLayerPtr> layers;
    GetValidLayers(getLayerExplorer()->model(), QModelIndex(), layers);

    dlg.setLayerList(layers);
    dlg.setAppMapDisplay(m_display->getDisplay());

    dlg.exec();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onToolsRasterMultiResolutionTriggered()
{
  try
  {
    te::qt::widgets::MultiResolutionDialog dlg(this);

    std::list<te::map::AbstractLayerPtr> layers;
    GetValidLayers(getLayerExplorer()->model(), QModelIndex(), layers);

    dlg.setLayerList(layers);

    dlg.exec();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onToolsFixGeometryTriggered()
{
  try
  {
    te::qt::widgets::FixGeometryDialog dlg(this);

    std::list<te::map::AbstractLayerPtr> layers = getLayerExplorer()->getAllLayers();
    dlg.setLayerList(layers);

    dlg.exec();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onProjectPropertiesTriggered()
{
  if (m_project == 0)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("There's no current project!"));
    return;
  }

  ProjectInfoDialog editor(this);
  editor.setProject(m_project);

  if(editor.exec() == QDialog::Accepted)
    projectChanged();
}

void TerraView::onAddFolderLayerTriggered()
{
  // Get the parent item where the folder layer will be added.
  QModelIndex idx = GetParent(getLayerExplorer());

  // Get the folder layer to be added
  bool ok;
  QString text = QInputDialog::getText(this, m_app->getAppTitle(),
    tr("Folder layer name:"), QLineEdit::Normal,
    tr("Enter folder layer name"), &ok);

  if (!ok)
    return;

  if (text.isEmpty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("Enter the layer name!"));
    return;
  }

  getLayerExplorer()->addFolder(text.toStdString(), idx);

  projectChanged();
}

void TerraView::onLayerPropertiesTriggered()
{
  std::list<te::map::AbstractLayerPtr> layers = GetSelectedLayersOnly(getLayerExplorer());

  if (layers.empty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("There's no selected layer."));
    return;
  }

  // Docking
  QDockWidget* doc = new QDockWidget(this, Qt::Dialog);

  te::map::AbstractLayerPtr l = *layers.begin();
  te::qt::widgets::LayerPropertiesInfoWidget* info = new te::qt::widgets::LayerPropertiesInfoWidget(l.get(), doc);

  doc->setWidget(info);
  doc->setWindowTitle(info->windowTitle());
  doc->setAttribute(Qt::WA_DeleteOnClose, true);

  doc->show();
}

void TerraView::onLayerRemoveSelectionTriggered()
{
  std::list<te::map::AbstractLayerPtr> layers = GetSelectedLayersOnly(getLayerExplorer());

  if (layers.empty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("There's no selected layer."));
    return;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while (it != layers.end())
  {
    te::map::AbstractLayerPtr layer = (*it);

    if (!layer->isValid())
    {
      ++it;
      continue;
    }

    layer->clearSelected();

    ++it;

    te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
    emit triggered(&e);
  }
}

void TerraView::onLayerSRSTriggered()
{
  std::list<te::map::AbstractLayerPtr> layers = GetSelectedLayersOnly(getLayerExplorer());

  if (layers.empty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("There's no selected layer."));
    return;
  }
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if (srsDialog.exec() == QDialog::Rejected)
    return;

  std::pair<int, std::string> srid = srsDialog.getSelectedSRS();

  te::map::AbstractLayerPtr lay = *layers.begin();

  lay->setSRID(srid.first);
}

void TerraView::onLayerShowTableTriggered()
{
  std::list<te::map::AbstractLayerPtr> layers = GetSelectedLayersOnly(getLayerExplorer());

  if (layers.empty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("There's no selected layer."));
    return;
  }
  else
  {
    for(std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin(); it != layers.end(); ++it)
    {
      if (!(*it)->isValid())
      {
        QMessageBox::warning(this, m_app->getAppTitle(), tr("There are invalid layers selected!"));
        return;
      }
    }
  }


  te::map::AbstractLayerPtr lay = *layers.begin();

  if (lay->getSchema()->hasRaster())
    return;

  te::qt::af::DataSetTableDockWidget* doc = GetLayerDock(lay.get(), m_tables);

  if (doc == 0)
  {
    doc = new te::qt::af::DataSetTableDockWidget(this);
    doc->setLayer(lay.get());
    addDockWidget(Qt::BottomDockWidgetArea, doc);

    connect(doc, SIGNAL(closed(te::qt::af::DataSetTableDockWidget*)), SLOT(onLayerTableClose(te::qt::af::DataSetTableDockWidget*)));
    connect(doc, SIGNAL(createChartDisplay(te::qt::widgets::ChartDisplayWidget*, te::map::AbstractLayer*)), SLOT(onChartDisplayCreated(te::qt::widgets::ChartDisplayWidget*, te::map::AbstractLayer*)));

    if (!m_tables.empty())
      tabifyDockWidget(m_tables[m_tables.size() - 1], doc);

    m_tables.push_back(doc);

    m_app->addListener(doc);
  }

  doc->show();
  doc->raise();

  m_viewDataTable->setChecked(true);

  m_viewDataTable->setEnabled(true);
}

void TerraView::onLayerHistogramTriggered()
{
  try
  {
    std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());

    if (selectedLayers.empty())
    {
      QMessageBox::warning(this, m_app->getAppTitle(),
        tr("Select a layer in the layer explorer!"));
      return;
    }
    else
    {
      for(std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
      {
        if (!it->get()->isValid())
        {
          QMessageBox::warning(this, m_app->getAppTitle(),
            tr("There are invalid layers selected!"));
          return;
        }
      }
    }

    // The histogram will be created based on the first selected layer
    te::map::AbstractLayerPtr selectedLayer = *(selectedLayers.begin());

    const te::map::LayerSchema* schema = selectedLayer->getSchema().release();

    te::da::DataSet* dataset = selectedLayer->getData().release();
    te::da::DataSetType* dataType = (te::da::DataSetType*) schema;

    te::qt::widgets::HistogramDialog dlg(dataset, dataType, this);

    dlg.setWindowTitle(dlg.windowTitle() + " (" + tr("Layer") + ":" + selectedLayer->getTitle().c_str() + ")");

    int res = dlg.exec();
    if (res == QDialog::Accepted)
    {
      te::qt::af::ChartDisplayDockWidget* doc = new te::qt::af::ChartDisplayDockWidget(dlg.getDisplayWidget(), this);
      doc->setSelectionColor(m_app->getSelectionColor());
      doc->setWindowTitle(tr("Histogram"));
      doc->setWindowIcon(QIcon::fromTheme("chart-bar"));
      doc->setLayer(selectedLayer.get());

      m_app->addListener(doc);
      addDockWidget(Qt::RightDockWidgetArea, doc, Qt::Horizontal);
      doc->show();
    }
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onLinkTriggered()
{
  try
  {
    // Get the parent layer where the dataset layer(s) will be added.
    QModelIndex par = GetParent(getLayerExplorer());

    std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());

    if (selectedLayers.empty())
    {
      QMessageBox::warning(this, m_app->getAppTitle(), tr("Select a layer in the layer explorer!"));
      return;
    }
    else
    {
      for(std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
      {
        if (!it->get()->isValid())
        {
          QMessageBox::warning(this, m_app->getAppTitle(), tr("There are invalid layers selected!"));
          return;
        }
      }
    }

    te::map::AbstractLayerPtr selectedLayer = *(selectedLayers.begin());

    std::auto_ptr<te::qt::widgets::TableLinkDialog> elb(new te::qt::widgets::TableLinkDialog(this));
    elb->setInputLayer(selectedLayer);

    int retval = elb->exec();

    if (retval == QDialog::Rejected)
      return;

    te::map::AbstractLayerPtr layer = elb->getQueryLayer();
    std::list<te::map::AbstractLayerPtr> layers;

    layers.push_back(layer);

    getLayerExplorer()->addLayers(layers, par);

    projectChanged();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
  catch (...)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("Unknown error while trying to add a layer from a queried dataset!"));
  }
}

void TerraView::onLayerScatterTriggered()
{
  try
  {
    std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());

    if (selectedLayers.empty())
    {
      QMessageBox::warning(this, m_app->getAppTitle(), tr("Select a layer in the layer explorer!"));
      return;
    }
    else
    {
      for(std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
      {
        if (!it->get()->isValid())
        {
          QMessageBox::warning(this, m_app->getAppTitle(), tr("There are invalid layers selected!"));
          return;
        }
      }
    }

    // The scatter will be created based on the first selected layer
    te::map::AbstractLayerPtr selectedLayer = *(selectedLayers.begin());

    const te::map::LayerSchema* schema = selectedLayer->getSchema().release();

    te::da::DataSet* dataset = selectedLayer->getData().release();
    te::da::DataSetType* dataType = (te::da::DataSetType*) schema;

    te::qt::widgets::ScatterDialog dlg(dataset, dataType, this);

    dlg.setWindowTitle(dlg.windowTitle() + " (" + tr("Layer") + ":" + selectedLayer->getTitle().c_str() + ")");

    int res = dlg.exec();
    if (res == QDialog::Accepted)
    {
      te::qt::af::ChartDisplayDockWidget* doc = new te::qt::af::ChartDisplayDockWidget(dlg.getDisplayWidget(), this);

      doc->setSelectionColor(m_app->getSelectionColor());
      doc->setWindowTitle(tr("Scatter"));
      doc->setWindowIcon(QIcon::fromTheme("chart-scatter"));
      m_app->addListener(doc);
      doc->setLayer(selectedLayer.get());

      addDockWidget(Qt::RightDockWidgetArea, doc, Qt::Horizontal);
      doc->show();
    }
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onLayerChartTriggered()
{
  try
  {
    std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());

    if (selectedLayers.empty())
    {
      QMessageBox::warning(this, m_app->getAppTitle(),
        tr("Select a single layer in the layer explorer!"));
      return;
    }
    else
    {
      for(std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
      {
        if (!(*it)->isValid())
        {
          QMessageBox::warning(this, m_app->getAppTitle(),
            tr("There are invalid layers selected!"));
          return;
        }
      }
    }

    // The chart will be accomplished only on the first single layer selected
    te::map::AbstractLayerPtr selectedLayer = *selectedLayers.begin();

    te::qt::widgets::ChartLayerDialog dlg(this);

    dlg.setWindowTitle(dlg.windowTitle() + " (" + tr("Layer") + ":" + selectedLayer->getTitle().c_str() + ")");

    dlg.setLayer(selectedLayer);

    // If the selected layer has a chart associated to it, set the chart layer
    // dialog for initializing with this chart.
    te::map::Chart* chart = selectedLayer->getChart();

    if (chart)
      dlg.setChart(chart);

    if (dlg.exec() == QDialog::Accepted)
    {
      getLayerExplorer()->updateChart(*getLayerExplorer()->selectionModel()->selectedIndexes().begin());

      m_display->getDisplay()->refresh();

      projectChanged();
    }
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onLayerGroupingTriggered()
{
  try
  {
    std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());

    if (selectedLayers.empty())
    {
      QMessageBox::warning(this, m_app->getAppTitle(), tr("Select a single layer in the layer explorer!"));
      return;
    }
    else
    {
      for(std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
      {
        if (!(*it)->isValid())
        {
          QMessageBox::warning(this, m_app->getAppTitle(), tr("There are invalid layers selected!"));

          return;
        }
      }
    }

    // The object grouping will be accomplished only on the first layer selected
    te::map::AbstractLayerPtr selectedLayer = *selectedLayers.begin();

    // Get all layer with grouping to dispose to import
    std::vector<te::map::AbstractLayerPtr> allLayers;
    GetValidLayers(getLayerExplorer()->model(), QModelIndex(), allLayers);

    te::qt::widgets::GroupingDialog dlg(this);
    dlg.setLayers(selectedLayer, allLayers);


    if (dlg.exec() == QDialog::Accepted)
    {
      getLayerExplorer()->updateGrouping(*getLayerExplorer()->selectionModel()->selectedIndexes().begin());

      m_display->getDisplay()->refresh();

      projectChanged();
    }
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onLayerFitOnMapDisplayTriggered()
{
  try
  {
    std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());

    if (selectedLayers.empty())
    {
      QMessageBox::warning(this, m_app->getAppTitle(),
        tr("Select a layer in the layer explorer!"));
      return;
    }
    else
    {
      std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin();

      while (it != selectedLayers.end())
      {
        if (!it->get()->isValid())
        {
          QMessageBox::warning(this, m_app->getAppTitle(),
            tr("There are invalid layers selected!"));

          return;
        }

        ++it;
      }
    }

    // The layer fitting will be accomplished only on the first layer selected
    te::map::AbstractLayerPtr selectedLayer = *(selectedLayers.begin());

    te::qt::widgets::MapDisplay* display = m_display->getDisplay();

    te::gm::Envelope env = selectedLayer->getExtent();

    if ((display->getSRID() == TE_UNKNOWN_SRS && selectedLayer->getSRID() == TE_UNKNOWN_SRS) || (display->getSRID() == selectedLayer->getSRID()))
    {
      display->setExtent(env, true);
      return;
    }

    if (display->getSRID() == TE_UNKNOWN_SRS && selectedLayer->getSRID() != TE_UNKNOWN_SRS)
    {
      display->setSRID(selectedLayer->getSRID());
      display->setExtent(env, true);
      return;
    }

    if (display->getSRID() == TE_UNKNOWN_SRS || selectedLayer->getSRID() == TE_UNKNOWN_SRS)
    {
      QMessageBox::warning(this, m_app->getAppTitle(),
        TE_TR("The spatial reference system of the map display and the layer are not compatible!"));
      return;
    }

    if (display->getSRID() != selectedLayer->getSRID())
      env.transform(selectedLayer->getSRID(), display->getSRID());

    display->setExtent(env, true);
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void TerraView::onLayerFitSelectedOnMapDisplayTriggered()
{
  std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());
  if (selectedLayers.empty())
  {
    QString msg = tr("Select at least a layer to accomplish this operation!");
    QMessageBox::warning(this, m_app->getAppTitle(), msg);
    return;
  }
  else
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin();

    while (it != selectedLayers.end())
    {
      if (!it->get()->isValid())
      {
        QMessageBox::warning(this, m_app->getAppTitle(),
          tr("There are invalid layers selected!"));
        return;
      }

      ++it;
    }
  }

  te::gm::Envelope finalEnv = te::map::GetSelectedExtent(selectedLayers, m_display->getDisplay()->getSRID(), false);

  if (!finalEnv.isValid())
  {
    QString msg = tr("Select object(s) in the selected layer(s) to accomplish this operation!");
    QMessageBox::warning(this, m_app->getAppTitle(), msg);
    return;
  }

  m_display->getDisplay()->setExtent(finalEnv, true);
}

void TerraView::onLayerPanToSelectedOnMapDisplayTriggered()
{
  std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());
  if (selectedLayers.empty())
  {
    QString msg = tr("Select at least a layer to accomplish this operation!");
    QMessageBox::warning(this, m_app->getAppTitle(), msg);
    return;
  }
  else
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin();

    while (it != selectedLayers.end())
    {
      if (!it->get()->isValid())
      {
        QMessageBox::warning(this, m_app->getAppTitle(),
          tr("There are invalid layers selected!"));
        return;
      }

      ++it;
    }
  }

  te::map::MapDisplay* display = m_display->getDisplay();

  te::gm::Envelope selectedExtent = te::map::GetSelectedExtent(selectedLayers, display->getSRID(), true);

  te::gm::Coord2D centerOfSelectedExtent = selectedExtent.getCenter();

  te::gm::Envelope displayExtent = display->getExtent();

  double halfWidth = displayExtent.getWidth() * 0.5;
  double halfHeight = displayExtent.getHeight() * 0.5;

  te::gm::Envelope newExtent;

  newExtent.m_llx = centerOfSelectedExtent.x - halfWidth;
  newExtent.m_lly = centerOfSelectedExtent.y - halfHeight;

  newExtent.m_urx = centerOfSelectedExtent.x + halfWidth;
  newExtent.m_ury = centerOfSelectedExtent.y + halfHeight;

  display->setExtent(newExtent);
}

void TerraView::onLayerCompositionModeTriggered()
{
  std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());

  if (!selectedLayers.empty())
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin();

    m_compModeMenu->setLayer(*it);
  }
}

void TerraView::onQueryLayerTriggered()
{
  std::list<te::map::AbstractLayerPtr> selectedLayers = GetSelectedLayersOnly(getLayerExplorer());

  if (selectedLayers.empty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(),
      tr("Select a layer in the layer explorer!"));
    return;
  }
  else
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin();

    while (it != selectedLayers.end())
    {
      if (!it->get()->isValid())
      {
        QMessageBox::warning(this, m_app->getAppTitle(),
          tr("There are invalid layers selected!"));
        return;
      }

      ++it;
    }
  }

  if (!m_queryDlg)
  {
    m_queryDlg = new te::qt::widgets::QueryDialog(this);

    connect(m_queryDlg, SIGNAL(highlightLayerObjects(const te::map::AbstractLayerPtr&, te::da::DataSet*, const QColor&)),
      SLOT(onHighlightLayerObjects(const te::map::AbstractLayerPtr&, te::da::DataSet*, const QColor&)));

    connect(m_queryDlg, SIGNAL(layerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)),
      SLOT(onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)));

    if (m_iController)
      m_iController->addInterface(m_queryDlg);
  }

  std::list<te::map::AbstractLayerPtr> allLayersList;
  GetValidLayers(getLayerExplorer()->model(), QModelIndex(), allLayersList);

  m_queryDlg->setLayerList(allLayersList);

  selectedLayers = GetSelectedLayersOnly(getLayerExplorer());

  if(!selectedLayers.empty())
    m_queryDlg->setCurrentLayer(*(selectedLayers.begin()));

  m_queryDlg->show();
}

void TerraView::onZoomInToggled(bool checked)
{
  if (!checked)
    return;

  QCursor zoomAreaCursor(QIcon::fromTheme("zoom-in").pixmap(m_mapCursorSize));

  te::qt::widgets::ZoomArea* zoomArea = new te::qt::widgets::ZoomArea(m_display->getDisplay(), zoomAreaCursor);
  m_display->setCurrentTool(zoomArea);

  te::qt::af::evt::ZoomInButtonToggled ezoom;
  m_app->triggered(&ezoom);
}

void TerraView::onZoomOutToggled(bool checked)
{
  if (!checked)
    return;

  QCursor zoomOutCursor(QIcon::fromTheme("zoom-out").pixmap(m_mapCursorSize));

  te::qt::widgets::ZoomClick* zoomOut = new te::qt::widgets::ZoomClick(m_display->getDisplay(), zoomOutCursor, 2.0, te::qt::widgets::Zoom::Out);
  m_display->setCurrentTool(zoomOut);

  te::qt::af::evt::ZoomOutButtonToggled ezoom;
  m_app->triggered(&ezoom);
}

void TerraView::onPreviousExtentTriggered()
{
  m_display->previousExtent();
}

void TerraView::onNextExtentTriggered()
{
  m_display->nextExtent();
}

void TerraView::onPanToggled(bool checked)
{
  if (!checked)
    return;

  te::qt::widgets::Pan* pan = new te::qt::widgets::Pan(m_display->getDisplay(), Qt::OpenHandCursor, Qt::ClosedHandCursor);
  m_display->setCurrentTool(pan);

  te::qt::af::evt::PanButtonToggled epan;
  m_app->triggered(&epan);
}

void TerraView::onZoomExtentTriggered()
{
  std::list<te::map::AbstractLayerPtr> ls = getLayerExplorer()->getVisibleLayers();

  if(ls.empty())
    return;

  m_display->fit(ls);
}

void TerraView::onInfoToggled(bool checked)
{
  if(!checked)
    return;

  QPixmap pxmap = QIcon::fromTheme("pointer-info").pixmap(m_mapCursorSize);
  QCursor infoCursor(pxmap, 0, 0);

  te::qt::widgets::Info* info = new te::qt::widgets::Info(m_display->getDisplay(), infoCursor, GetSelectedLayersOnly(getLayerExplorer()));
  m_display->setCurrentTool(info);

  connect(getLayerExplorer(), SIGNAL(selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)), info, SLOT(setLayers(const std::list<te::map::AbstractLayerPtr>&)));
}

void TerraView::onMapRemoveSelectionTriggered()
{
  std::list<te::map::AbstractLayerPtr> layers = getLayerExplorer()->getAllLayers();
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while (it != layers.end())
  {
    te::map::AbstractLayerPtr layer = (*it);

    layer->clearSelected();

    ++it;

    te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
    emit triggered(&e);
  }
}

void TerraView::onSelectionToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::Selection* selection = new te::qt::widgets::Selection(m_display->getDisplay(), Qt::ArrowCursor, GetSelectedLayersOnly(getLayerExplorer()));
  m_display->setCurrentTool(selection);

  connect(getLayerExplorer(), SIGNAL(selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)), selection, SLOT(setLayers(const std::list<te::map::AbstractLayerPtr>&)));
  connect(selection, SIGNAL(layerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)), SLOT(onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)));

  te::qt::af::evt::SelectionButtonToggled esel;
  emit triggered(&esel);
}

void TerraView::onMeasureDistanceToggled(bool checked)
{
  if (!checked)
    return;

  QCursor measureDistanceCursor(QIcon::fromTheme("distance-measure-cursor").pixmap(m_mapCursorSize), 0, 0);

  te::qt::widgets::Measure* distance = new te::qt::widgets::Measure(m_display->getDisplay(), te::qt::widgets::Measure::Distance, measureDistanceCursor);
  m_display->setCurrentTool(distance);
}

void TerraView::onMeasureAreaToggled(bool checked)
{
  if (!checked)
    return;

  QCursor measureAreaCursor(QIcon::fromTheme("area-measure-cursor").pixmap(m_mapCursorSize), 0, 0);

  te::qt::widgets::Measure* area = new te::qt::widgets::Measure(m_display->getDisplay(), te::qt::widgets::Measure::Area, measureAreaCursor);
  m_display->setCurrentTool(area);
}

void TerraView::onMeasureAngleToggled(bool checked)
{
  if (!checked)
    return;

  QCursor measureAngleCursor(QIcon::fromTheme("angle-measure-cursor").pixmap(m_mapCursorSize), 0, 0);

  te::qt::widgets::Measure* angle = new te::qt::widgets::Measure(m_display->getDisplay(), te::qt::widgets::Measure::Angle, measureAngleCursor);
  m_display->setCurrentTool(angle);
}

void TerraView::showProgressDockWidget()
{
  m_progressDockWidget->setVisible(true);
}

void TerraView::onFullScreenToggled(bool checked)
{
  checked ? showFullScreen() : showMaximized();
}

void TerraView::onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr& layer)
{
  assert(layer.get());

  te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
  m_app->triggered(&e);
}

void TerraView::onHighlightLayerObjects(const te::map::AbstractLayerPtr& layer, te::da::DataSet* dataset, const QColor& color)
{
  assert(layer.get());
  assert(dataset);

  te::qt::af::evt::HighlightLayerObjects e(layer, dataset, color);
  m_app->triggered(&e);
}

void TerraView::onLayerExplorerVisibilityChanged(bool visible)
{
  m_viewLayerExplorer->blockSignals(true);
  m_viewLayerExplorer->setChecked(visible);
  m_viewLayerExplorer->blockSignals(false);
}

void TerraView::onDisplayDataTableChanged(bool visible)
{
  if (m_tables.empty())
    return;

  for (std::size_t i = 0; i < m_tables.size(); ++i)
  {
    if (visible)
      m_tables[i]->show();
    else
      m_tables[i]->hide();
  }

  m_viewDataTable->setChecked(visible);
}

void TerraView::onStyleExplorerVisibilityChanged(bool visible)
{
  m_viewStyleExplorer->blockSignals(true);
  m_viewStyleExplorer->setChecked(visible);
  m_viewStyleExplorer->blockSignals(false);
}

void TerraView::onDataSourceExplorerTriggered()
{
  try
  {
    std::auto_ptr<te::qt::widgets::DataSourceExplorerDialog> dExplorer(new te::qt::widgets::DataSourceExplorerDialog(this));

    QString dsTypeSett = te::qt::af::GetLastDatasourceFromSettings();

    if (!dsTypeSett.isNull() && !dsTypeSett.isEmpty())
      dExplorer->setDataSourceToUse(dsTypeSett);


    int retval = dExplorer->exec();

    if (retval == QDialog::Rejected)
      return;

    std::list<te::da::DataSourceInfoPtr> selectedDatasources = dExplorer->getSelecteds();

    if (selectedDatasources.empty())
      return;

    dExplorer.reset(0);

    const std::string& dsTypeId = selectedDatasources.front()->getType();

    te::qt::af::SaveLastDatasourceOnSettings(dsTypeId.c_str());
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
  catch (...)
  {
    QMessageBox::warning(this,
      m_app->getAppTitle(),
      tr("DataSetExplorer Error!"));
  }
}

void TerraView::onCreateNewLayer(te::map::AbstractLayerPtr layer)
{
  te::qt::af::evt::LayerAdded evt(layer);
  emit triggered(&evt);
}

void TerraView::onNewProjectTriggered()
{
  checkAndSaveProject();

  m_project->m_fileName = "";
  m_project->m_title = tr("Default Project");
  m_project->m_changed = false;

  setWindowTitle(m_app->getAppName() + " - " + m_project->m_title);

  std::list<te::map::AbstractLayerPtr> ls;

  getLayerExplorer()->setLayers(ls);

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

  getLayerExplorer()->setLayers(lst);

  m_project->m_changed = false;

  setWindowTitle(m_app->getAppName() + " - " + m_project->m_title);

  AddRecentProjectToSettings(m_project->m_title, m_project->m_fileName);
}

void TerraView::closeEvent(QCloseEvent* event)
{
  checkAndSaveProject();

  while(!m_tables.empty())
    delete *m_tables.begin();

  BaseApplication::closeEvent(event);
}

void TerraView::startProject(const QString& projectFileName)
{
  openProject(projectFileName);
}

