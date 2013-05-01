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
#include "../../common/progress/ProgressManager.h"
#include "../../common/progress/TaskProgress.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/Translator.h"
#include "../../common/UserApplicationSettings.h"
#include "../../maptools/Utils.h"
#include "../../srs/Config.h"
#include "../widgets/canvas/MultiThreadMapDisplay.h"
#include "../widgets/charts/ChartStyleDialog.h"
#include "../widgets/charts/HistogramCreatorDialog.h"
#include "../widgets/charts/ScatterCreatorDialog.h"
#include "../widgets/datasource/core/DataSourceType.h"
#include "../widgets/datasource/core/DataSourceTypeManager.h"
#include "../widgets/datasource/selector/DataSourceSelectorDialog.h"
//#include "../widgets/dataview/TabularViewer.h"
#include "../widgets/exchanger/DataExchangerWizard.h"
#include "../widgets/help/HelpManager.h"
#include "../widgets/layer/explorer/LayerExplorer.h"
#include "../widgets/layer/explorer/LayerTreeView.h"
#include "../widgets/layer/explorer/AbstractLayerTreeItem.h"
#include "../widgets/layer/info/LayerPropertiesInfoWidget.h"
#include "../widgets/layer/selector/AbstractLayerSelector.h"
#include "../widgets/plugin/builder/PluginBuilderWizard.h"
#include "../widgets/plugin/manager/PluginManagerDialog.h"
#include "../widgets/progress/ProgressViewerBar.h"
#include "../widgets/progress/ProgressViewerDialog.h"
#include "../widgets/progress/ProgressViewerWidget.h"
#include "../widgets/query/QueryLayerBuilderWizard.h"
#include "../widgets/se/RasterVisualDockWidget.h"
#include "../widgets/table/DataSetTableView.h"
#include "../widgets/tools/Measure.h"
#include "../widgets/tools/Pan.h"
#include "../widgets/tools/ZoomArea.h"
#include "../widgets/tools/ZoomClick.h"
#include "connectors/LayerExplorer.h"
#include "connectors/MapDisplay.h"
#include "connectors/TabularViewer.h"
#include "events/LayerEvents.h"
#include "events/ProjectEvents.h"
#include "events/ApplicationEvents.h"
#include "events/ToolEvents.h"
#include "settings/SettingsDialog.h"
#include "ApplicationController.h"
#include "ApplicationPlugins.h"
#include "BaseApplication.h"
#include "Exception.h"
#include "Project.h"
#include "ProjectInfoDialog.h"
#include "SplashScreenManager.h"
#include "Utils.h"

// Qt
#include <QtCore/QDir>
#include <QtGui/QActionGroup>
#include <QtGui/QApplication>
#include <QtGui/QCloseEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>

// STL
#include <list>
#include <memory>

// Boost
#include <boost/format.hpp>

te::qt::af::BaseApplication::BaseApplication(QWidget* parent)
  : QMainWindow(parent, 0),
    m_explorer(0),
    m_display(0),
    m_viewer(0),
    m_project(0),
    m_controller(0)
{
  m_controller = new ApplicationController;

  if (objectName().isEmpty())
    setObjectName("BaseApplicationForm");

  resize(640, 480);
  setMinimumSize(QSize(640, 480));

  m_centralwidget = new QWidget(this);
  m_centralwidget->setObjectName("centralwidget");
  setCentralWidget(m_centralwidget);

  // Initilazing menus
  m_menubar = new QMenuBar(this);
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
  m_projectMenu->addAction(m_projectAddLayerMenu->menuAction());
  m_layerMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_layerMenu->menuAction());
  m_layerChartsMenu = new QMenu(m_layerMenu);
  m_layerMenu->addMenu(m_layerChartsMenu);
  m_mapMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_mapMenu->menuAction());
  m_toolsMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_toolsMenu->menuAction());
  m_pluginsMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_pluginsMenu->menuAction());
  m_helpMenu = new QMenu(m_menubar);
  m_menubar->addAction(m_helpMenu->menuAction());
}

te::qt::af::BaseApplication::~BaseApplication()
{
  delete m_explorer;
  delete m_display;
  delete m_viewer;
  delete m_project;
  delete m_progressDockWidget;

  te::qt::af::ApplicationController::getInstance().finalize();

  delete m_controller;
}

void te::qt::af::BaseApplication::init()
{
  throw Exception("Not implemented yet!");
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
    te::qt::af::ApplicationController::getInstance().initializeProjectMenus();
  }
  catch(const std::exception& e)
  {
    QString msgErr(tr("Error loading recent projects: %1"));
    msgErr = msgErr.arg(e.what());
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msgErr);
  }

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

// try to load the last opened project
  QString recentProject = te::qt::af::ApplicationController::getInstance().getMostRecentProject();

  if(recentProject.isEmpty())
    newProject();
  else
    openProject(recentProject);
}

void te::qt::af::BaseApplication::onApplicationTriggered(te::qt::af::evt::Event* evt)
{
  switch(evt->m_id)
  {
    case te::qt::af::evt::PROJECT_UNSAVED:
      m_project->projectChanged(true);
    break;

    case te::qt::af::evt::TOOLBAR_ADDED:
      {
        te::qt::af::evt::ToolBarAdded* ev = static_cast<te::qt::af::evt::ToolBarAdded*>(evt);
        QMainWindow::addToolBar(Qt::TopToolBarArea, ev->m_toolbar);
      }
    break;



    default:
    break;
  }
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

    std::list<te::da::DataSourceInfoPtr> selectedDatasources = dselector->getSelecteds();

    if(selectedDatasources.empty())
      return;

    dselector.reset(0);

    const std::string& dsTypeId = selectedDatasources.front()->getType();

    const te::qt::widgets::DataSourceType* dsType = te::qt::widgets::DataSourceTypeManager::getInstance().get(dsTypeId);

    std::auto_ptr<QWidget> lselectorw(dsType->getWidget(te::qt::widgets::DataSourceType::WIDGET_LAYER_SELECTOR, this));

    if(lselectorw.get() == 0)
      throw Exception((boost::format(TR_QT_AF("No layer selector widget found for this type of data source: %1%!")) % dsTypeId).str());

    te::qt::widgets::AbstractLayerSelector* lselector = dynamic_cast<te::qt::widgets::AbstractLayerSelector*>(lselectorw.get());

    if(lselector == 0)
      throw Exception(TR_QT_AF("Wrong type of object for layer selection!"));

    lselector->set(selectedDatasources);

    std::list<te::map::AbstractLayerPtr> layers = lselector->getLayers();

    lselectorw.reset(0);

    if(m_project == 0)
      throw Exception(TR_QT_AF("Error: there is no opened project!"));

// TODO: use signal/slot to avoid inserting direct in th explorer and in the project!
    std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin();
    std::list<te::map::AbstractLayerPtr>::const_iterator itend = layers.end();

    while(it != itend)
    {
      m_project->add(*it);

      if((m_explorer != 0) && (m_explorer->getExplorer() != 0))
        m_explorer->getExplorer()->add(*it);

      ++it;
    }
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

void te::qt::af::BaseApplication::onAddQueryLayerTriggered()
{
   try
  {
    std::auto_ptr<te::qt::widgets::QueryLayerBuilderWizard> qlb(new te::qt::widgets::QueryLayerBuilderWizard(this));

    int retval = qlb->exec();

    if(retval == QDialog::Rejected)
      return;

    te::map::AbstractLayerPtr layer = qlb->getQueryLayer();

    if(m_project == 0)
      throw Exception(TR_QT_AF("Error: there is no opened project!"));

    m_project->add(layer);

    if((m_explorer != 0) && (m_explorer->getExplorer() != 0))
      m_explorer->getExplorer()->add(layer);

  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         te::qt::af::ApplicationController::getInstance().getAppTitle(),
                         tr("Unknown error while trying to add a layer from a queried dataset!"));
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

void te::qt::af::BaseApplication::onRecentProjectsTriggered(QAction* proj)
{
  QString projFile = proj->data().toString();

  openProject(projFile);
}

void te::qt::af::BaseApplication::onNewProjectTriggered()
{
  newProject();
}

void te::qt::af::BaseApplication::onOpenProjectTriggered()
{
  QString file = QFileDialog::getOpenFileName(this, tr("Open project file"), qApp->applicationDirPath(), tr("XML File (*.xml *.XML)"));

  if(file.isEmpty())
    return;

  openProject(file);
}

void te::qt::af::BaseApplication::onSaveProjectAsTriggered()
{
  if(m_project == 0)
    return;

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), qApp->applicationDirPath(), tr("XML Files (*.xml *.XML)"));

  if(fileName.isEmpty())
    return;

  std::string fName = fileName.toStdString();

  m_project->setFileName(fName);

  te::qt::af::Save(*m_project, fName);

  m_project->projectChanged(false);

  ApplicationController::getInstance().updateRecentProjects(fileName, m_project->getTitle().c_str());
}

void te::qt::af::BaseApplication::onToolsCustomizeTriggered()
{
  try
  {
    te::qt::af::SettingsDialog dlg(this);
    dlg.exec();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onToolsDataExchangerTriggered()
{
  try
  {
    te::qt::widgets::DataExchangerWizard dlg(this);
    dlg.exec();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onProjectPropertiesTriggered()
{
  if(m_project == 0)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("There's no current project."));
    return;
  }

  ProjectInfoDialog editor(this);
  editor.setProject(m_project);
  editor.exec();
}

void te::qt::af::BaseApplication::onLayerPropertiesTriggered()
{
  std::list<te::qt::widgets::AbstractLayerTreeItem*> layers = m_explorer->getExplorer()->getTreeView()->getSelectedItems();

  if(layers.empty())
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("There's no selected layer."));
    return;
  }

  // Docking
  QDockWidget* doc = new QDockWidget(this, Qt::Dialog);

  te::qt::widgets::LayerPropertiesInfoWidget* info = new te::qt::widgets::LayerPropertiesInfoWidget((*(layers.begin()))->getLayer().get(), doc);

  doc->setWidget(info);
  doc->setWindowTitle(info->windowTitle());

  doc->show();
}

void te::qt::af::BaseApplication::onLayerShowTableTriggered()
{
  std::list<te::qt::widgets::AbstractLayerTreeItem*> layers = m_explorer->getExplorer()->getTreeView()->getSelectedItems();

  if(layers.empty())
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("There's no selected layer."));
    return;
  }

  te::map::AbstractLayer* lay = (*layers.begin())->getLayer().get();

  // Docking
  QDockWidget* doc = new QDockWidget(this, Qt::Widget);
  te::qt::widgets::DataSetTableView* tbl = new te::qt::widgets::DataSetTableView(doc);

  tbl->setDataSet(lay->getData());
  tbl->setLayerSchema(lay->getSchema());

  doc->setWindowTitle(lay->getTitle().c_str());

  doc->setWidget(tbl);

  addDockWidget(Qt::BottomDockWidgetArea, doc);

  if(!m_tableDocks.empty())
  {
    tabifyDockWidget(m_tableDocks[m_tableDocks.size()-1], doc);
    doc->show();
    doc->raise();
  }

  m_tableDocks.push_back(doc);
}

void te::qt::af::BaseApplication::onLayerHistogramTriggered()
{
  try
  {
    std::list<te::qt::widgets::AbstractLayerTreeItem*> layers = m_explorer->getExplorer()->getTreeView()->getSelectedItems();

    if(layers.empty())
    {
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("There's no selected layer."));
      return;
    }
    
    te::da::DataSet* dataset = (*(layers.begin()))->getLayer()->getData();
    te::qt::widgets::HistogramCreatorDialog dlg(dataset, this);
    dlg.exec();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onLayerScatterTriggered()
{
  try
  {
    std::list<te::qt::widgets::AbstractLayerTreeItem*> layers = m_explorer->getExplorer()->getTreeView()->getSelectedItems();

    if(layers.empty())
    {
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("There's no selected layer."));
      return;
    }
    
    te::da::DataSet* dataset = (*(layers.begin()))->getLayer()->getData();
    te::qt::widgets::ScatterCreatorDialog dlg(dataset, this);
    dlg.exec();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onDrawTriggered()
{
  if(m_project == 0)
    return;

  m_display->draw(m_project->getLayers());
}

void te::qt::af::BaseApplication::onZoomInToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::ZoomClick* zoomIn = new te::qt::widgets::ZoomClick(m_display->getDisplay(), 2.0);
  m_display->setCurrentTool(zoomIn);
}

void te::qt::af::BaseApplication::onZoomOutToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::ZoomClick* zoomOut = new te::qt::widgets::ZoomClick(m_display->getDisplay(), 2.0, te::qt::widgets::Zoom::Out);
  m_display->setCurrentTool(zoomOut);
}

void te::qt::af::BaseApplication::onZoomAreaToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::ZoomArea* zoomArea = new te::qt::widgets::ZoomArea(m_display->getDisplay(), Qt::BlankCursor);
  m_display->setCurrentTool(zoomArea);
}

void te::qt::af::BaseApplication::onPanToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::Pan* pan = new te::qt::widgets::Pan(m_display->getDisplay(), Qt::OpenHandCursor, Qt::ClosedHandCursor);
  m_display->setCurrentTool(pan);
}

void te::qt::af::BaseApplication::onZoomExtentTriggered()
{
  if(!m_project && m_project->getLayers().empty())
    return;

  te::qt::widgets::MapDisplay* display = m_display->getDisplay();
  te::gm::Envelope e = te::map::GetExtent(m_project->getLayers(), display->getSRID(), true);
  display->setExtent(e, true);
}

void te::qt::af::BaseApplication::onMeasureDistanceToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::Measure* distance = new te::qt::widgets::Measure(m_display->getDisplay(), te::qt::widgets::Measure::Distance);
  m_display->setCurrentTool(distance);
}

void te::qt::af::BaseApplication::onMeasureAreaToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::Measure* area = new te::qt::widgets::Measure(m_display->getDisplay(), te::qt::widgets::Measure::Area);
  m_display->setCurrentTool(area);
}

void te::qt::af::BaseApplication::onMeasureAngleToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::Measure* angle = new te::qt::widgets::Measure(m_display->getDisplay(), te::qt::widgets::Measure::Angle);
  m_display->setCurrentTool(angle);
}

void te::qt::af::BaseApplication::onStopDrawTriggered()
{
  te::common::ProgressManager::getInstance().cancelTasks(te::common::TaskProgress::DRAW);
}

void te::qt::af::BaseApplication::showProgressDockWidget()
{
  m_progressDockWidget->setVisible(true);
}

void te::qt::af::BaseApplication::openProject(const QString& projectFileName)
{
  try
  {
    checkProjectSave();

    Project* nproject = te::qt::af::ReadProject(projectFileName.toStdString());

    delete m_project;

    m_project = nproject;

    ApplicationController::getInstance().updateRecentProjects(projectFileName, m_project->getTitle().c_str());

    QString projectTile(tr(" - Project: %1"));

    setWindowTitle(te::qt::af::ApplicationController::getInstance().getAppTitle() + projectTile.arg(m_project->getTitle().c_str()));

    te::qt::af::ApplicationController::getInstance().set(m_project);

    te::qt::af::evt::ProjectAdded evt(m_project);

    ApplicationController::getInstance().broadcast(&evt);
  }
  catch(const std::exception& e)
  {
    QString msg(tr("Fail to open project: %1"));

    msg = msg.arg(e.what());

    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msg);
  }
}

void te::qt::af::BaseApplication::checkProjectSave()
{
  if(m_project != 0 && m_project->hasChanged())
  {
    QString msg("The current project has unsaved changes. Do you want to save?");
    int btn = QMessageBox::question(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msg, QMessageBox::No, QMessageBox::Yes);

    if(btn == QMessageBox::Yes)
    {
      std::string fName = m_project->getFileName();

      if(fName.empty())
      {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), qApp->applicationDirPath(), tr("XML Files (*.xml *.XML)"));

        if(!fileName.isEmpty())
        {
          fName = fileName.toStdString();
          m_project->setFileName(fName);
        }
        else
        {
          QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("Project not saved."));
          return;
        }
      }

      te::qt::af::Save(*m_project, m_project->getFileName());
      m_project->projectChanged(false);
    }
  }
}

void te::qt::af::BaseApplication::newProject()
{
  delete m_project;

  m_project = new Project;

  QString author;
  int maxSaved;

  GetProjectInformationsFromSettings(author, maxSaved);

  m_project->setTitle("New Project");

  m_project->setAuthor(author.toStdString());

  QString projectTile(tr(" - Project: %1"));

  setWindowTitle(te::qt::af::ApplicationController::getInstance().getAppTitle() + projectTile.arg(m_project->getTitle().c_str()));

  te::qt::af::ApplicationController::getInstance().set(m_project);

  te::qt::af::evt::ProjectAdded evt(m_project);

  ApplicationController::getInstance().broadcast(&evt);
}

void te::qt::af::BaseApplication::makeDialog()
{
  initActions();

  initMenus();

  initToolbars();

  initSlotsConnections();

//  QMenu* mnu = ApplicationController::getInstance().findMenu(QString("File.Recent Projects"));
//
//  if(mnu != 0)
//  {
////    QMenu* m = new QMenu("Teste", mnu);
//    mnu->addMenu("Teste");
//  }
// placing tools on an exclusive group
  //QActionGroup* vis_tools_group = new QActionGroup(this);
  //vis_tools_group->setExclusive(true);
  //m_ui->m_actionPan->setActionGroup(vis_tools_group);
  //m_ui->m_actionZoom_area->setActionGroup(vis_tools_group);
  //m_ui->m_area_act->setActionGroup(vis_tools_group);
  //m_ui->m_angle_act->setActionGroup(vis_tools_group);
  //m_ui->m_distance_act->setActionGroup(vis_tools_group);

// initializing well known widgets

// 1. Layer Explorer
  te::qt::widgets::LayerExplorer* lexplorer = new te::qt::widgets::LayerExplorer(this);

  lexplorer->getTreeView()->add(m_projectAddLayerDataset, "", "", te::qt::widgets::LayerTreeView::NO_LAYER_SELECTED);
  lexplorer->getTreeView()->add(m_layerLower, "", "", te::qt::widgets::LayerTreeView::SINGLE_LAYER_SELECTED);
  lexplorer->getTreeView()->add(m_layerProperties, "", "", te::qt::widgets::LayerTreeView::ALL_SELECTION_TYPES);
  lexplorer->getTreeView()->add(m_projectRemoveLayer, "", "", te::qt::widgets::LayerTreeView::ALL_SELECTION_TYPES);

  QMainWindow::addDockWidget(Qt::LeftDockWidgetArea, lexplorer);

  //lexplorer->getTreeView()->setSelectionMode(QAbstractItemView::MultiSelection);

  connect(m_viewLayerExplorer, SIGNAL(toggled(bool)), lexplorer, SLOT(setVisible(bool)));
  m_viewLayerExplorer->setChecked(true);

  m_explorer = new te::qt::af::LayerExplorer(lexplorer, this);

// 2. Map Display
  te::qt::widgets::MapDisplay* map = new te::qt::widgets::MultiThreadMapDisplay(QSize(512, 512), this);
  map->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  map->setMouseTracking(true);

  m_display = new te::qt::af::MapDisplay(map);

// 3. Data Table
//  te::qt::widgets::TabularViewer* view = new te::qt::widgets::TabularViewer(this);

 // m_viewer = new te::qt::af::TabularViewer(view);

// registering framework listeners
  te::qt::af::ApplicationController::getInstance().addListener(this);
  te::qt::af::ApplicationController::getInstance().addListener(m_explorer);
  te::qt::af::ApplicationController::getInstance().addListener(m_display);
  //te::qt::af::ApplicationController::getInstance().addListener(m_viewer);


// initializing connector widgets
  QDockWidget* doc = new QDockWidget(tr("Map Display"), this);
  doc->setWidget(map);
  QMainWindow::setCentralWidget(doc);
  doc->connect(m_viewMapDisplay, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_viewMapDisplay->setChecked(true);

/*  doc = new QDockWidget(tr("Data Table"), this);
  doc->setWidget(view);
  QMainWindow::addDockWidget(Qt::BottomDockWidgetArea, doc);
  doc->connect(m_viewDataTable, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_viewDataTable->setChecked(false);
  doc->setVisible(false);
*/

//// Raster Visual Dock widget
//  m_rasterVisualDock = new te::qt::widgets::RasterVisualDockWidget(tr("Raster Enhancement"), this);
//  connect(m_rasterVisualDock, SIGNAL(symbolizerChanged()), this, SLOT(drawLayers()));
//  QMainWindow::addDockWidget(Qt::RightDockWidgetArea, m_rasterVisualDock);
//  m_rasterVisualDock->connect(m_ui->m_viewRasterVisual, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
//  m_ui->m_viewRasterVisual->setChecked(false);
//  m_rasterVisualDock->setVisible(false);

// Progress support
  te::qt::widgets::ProgressViewerBar* pvb = new te::qt::widgets::ProgressViewerBar(this);
  pvb->setFixedWidth(250);
  te::common::ProgressManager::getInstance().addViewer(pvb);

  te::qt::widgets::ProgressViewerWidget* pvw = new te::qt::widgets::ProgressViewerWidget(this);
  te::common::ProgressManager::getInstance().addViewer(pvw);

  m_statusbar->addPermanentWidget(pvb);

  connect(pvb, SIGNAL(clicked()), this, SLOT(showProgressDockWidget()));

  m_progressDockWidget = new QDockWidget(this);
  m_progressDockWidget->setWidget(pvw);
  m_progressDockWidget->setMinimumWidth(250);
  m_progressDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
  m_progressDockWidget->setWindowTitle(tr("Tasks Progress"));
  addDockWidget(Qt::RightDockWidgetArea, m_progressDockWidget);
  m_progressDockWidget->setVisible(false);
}

void te::qt::af::BaseApplication::closeEvent(QCloseEvent* e)
{
  checkProjectSave();

  //AppClose aclose;

  //emit triggered(&aclose);

  e->accept();
}

void te::qt::af::BaseApplication::initAction(QAction*& act, const QString& icon, const QString& name,
                                             const QString& text, const QString& tooltip,
                                             bool iconVisibleInMenu, bool isCheckable,
                                             bool enabled, QMenuBar* menu)
{
  act = new QAction (menu);

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
// Menu -View- actions
  initAction(m_viewLayerExplorer, "view-layer-explorer", "View.Layer Explorer", tr("&Layer Explorer"), tr("Show or hide the layer explorer"), true, true, true, m_menubar);
  initAction(m_viewMapDisplay, "view-map-display", "View.Map Display", tr("&Map Display"), tr("Show or hide the map display"), true, true, true, m_menubar);
  initAction(m_viewDataTable, "view-data-table", "View.Data Table", tr("&Data Table"), tr("Show or hide the data table"), true, true, false, m_menubar);
  initAction(m_viewStyleExplorer, "grid-visible", "View.Style Explorer", tr("&Style Explorer"), tr("Show or hide the style explorer"), true, true, false, m_menubar);
  initAction(m_viewFullScreen, "view-fullscreen", "View.Full Screen", tr("F&ull Screen"), tr(""), true, false, false, m_menubar);
  initAction(m_viewRefresh, "view-refresh", "View.Refresh", tr("&Refresh"), tr(""), true, false, false, m_menubar);
  //initAction(m_viewToolBars, "", "Toolbars", tr("&Toolbars"), tr(""), true, false, false);
  initAction(m_viewGrid, "view-grid", "View.Grid", tr("&Grid"), tr("Show or hide the geographic grid"), true, true, false, m_menubar);
  initAction(m_viewDataSourceExplorer, "view-datasource-explorer", "View.Data Source Explorer", tr("&Data Source Explorer"), tr("Show or hide the data source explorer"), 
    true, true, false, m_menubar);

// Menu -Tools- actions
  initAction(m_toolsCustomize, "preferences-system", "Tools.Customize", tr("&Customize..."), tr("Customize the system preferences"), true, false, true, m_menubar);
  initAction(m_toolsDataExchanger, "", "Tools.Data Exchanger", tr("&Data Exchanger..."), tr("Exchange data sets between data sources"), true, false, true, m_menubar);
  initAction(m_toolsDataSourceManagement, "", "Tools.Data Source Management", tr("&Data Source Management..."), tr("Manage the registered data sources"), true, false, 
    false, m_menubar);

// Menu -Edit- actions
  //initAction(m_editUndo, "edit-undo", "Undo", tr("&Undo"), tr("Undo the last operation"), true, false, false);
  //initAction(m_editRedo, "edit-redo", "Redo", tr("&Redo"), tr("Redo the last operation"), true, false, false);
  //initAction(m_editCut, "edit-cut", "Cut", tr("Cu&t"), tr(""), true, true, false);
  //initAction(m_editCopy, "edit-copy", "Copy", tr("&Copy"), tr(""), true, true, false);
  //initAction(m_editPaste, "edit-paste", "&Paste", tr("&Paste"), tr(""), true, true, false);
  //initAction(m_editSelectAll, "edit-select-all", "Select All", tr("Select &All"), tr(""), true, true, false);
  //initAction(m_editClear, "edit-clear", "Clear", tr("C&lear"), tr(""), true, true, false);
  //initAction(m_editFind, "edit-find", "Find", tr("&Find..."), tr(""), true, true, false);
  //initAction(m_editReplace, "edit-find-replace", "Replace", tr("R&eplace..."), tr(""), true, true, false);

// Menu -Plugins- actions
  initAction(m_pluginsManager, "", "Plugins.Management", tr("&Manage Plugins..."), tr("Manage the application plugins"), true, false, true, m_menubar);
  initAction(m_pluginsBuilder, "", "Plugins.Build a New Plugin", tr("&Build a New Plugin..."), tr("Create a new plugin"), true, false, true, m_menubar);

// Menu -Help- actions
  initAction(m_helpContents, "help-browser", "Help.View Help", tr("&View Help..."), tr("Shows help dialog"), true, false, true, m_menubar);
  initAction(m_helpUpdate, "system-software-update", "Help.Update", tr("&Update..."), tr(""), true, false, false, m_menubar);
  initAction(m_helpAbout, "", "Help.About", tr("&About..."), tr(""), true, false, false, m_menubar);

// Menu -Project- actions
  initAction(m_projectRemoveLayer, "layer-remove", "Project.Remove Layer", tr("&Remove Layer"), tr("Remove layer from the project"), true, false, false, m_menubar);
  initAction(m_projectProperties, "", "Project.Properties", tr("&Properties..."), tr("Show the project properties"), true, false, true, m_menubar);
  initAction(m_projectAddLayerDataset, "", "Project.Add Layer.Dataset", tr("&Dataset..."), tr("Add a new layer from a dataset"), true, false, true, m_menubar);
  initAction(m_projectAddLayerQueryDataSet, "", "Project.Add Layer.Query Dataset", tr("&Query Dataset..."), tr("Add a new layer from a queried dataset"), true, false, true, m_menubar);
  //initAction(m_projectAddLayerGraph, "", "Graph", tr("&Graph"), tr("Add a new layer from a graph"), true, false, false);

// Menu -Layer- actions
  initAction(m_layerEdit, "layer-edit", "Layer.Edit", tr("&Edit"), tr(""), true, false, false, m_menubar);
  initAction(m_layerRename, "layer-rename", "Layer.Rename", tr("R&ename"), tr(""), true, false, false, m_menubar);
  initAction(m_layerExport, "document-export", "Layer.Export", tr("E&xport..."), tr(""), true, false, false, m_menubar);
  initAction(m_layerProperties, "", "Layer.Properties", tr("&Properties..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerShowTable, "", "Layer.Show Table", tr("S&how Table"), tr(""), true, false, true, m_menubar);
  initAction(m_layerRaise, "layer-raise", "Layer.Raise", tr("&Raise"), tr(""), true, false, false, m_menubar);
  initAction(m_layerLower, "layer-lower", "Layer.Lower", tr("&Lower"), tr(""), true, false, false, m_menubar);
  initAction(m_layerToTop, "layer-to-top", "Layer.To Top", tr("To &Top"), tr(""), true, false, false, m_menubar);
  initAction(m_layerToBottom, "layer-to-bottom", "Layer.To Bottom", tr("To &Bottom"), tr(""), true, false, false, m_menubar);
  initAction(m_layerChartsHistogram, "chart-bar", "Layer.Charts.Histogram", tr("&Histogram"), tr(""), true, false, true, m_menubar);
  initAction(m_layerChartsScatter, "chart-scatter", "Layer.Charts.Scatter", tr("&Scatter"), tr(""), true, false, true, m_menubar);

// Menu -File- actions
  initAction(m_fileNewProject, "document-new", "File.New Project", tr("&New Project"), tr(""), true, false, true, m_menubar);
  initAction(m_fileSaveProject, "document-save", "File.Save Project", tr("&Save Project"), tr(""), true, false, false, m_menubar);
  initAction(m_fileSaveProjectAs, "document-save-as", "File.Save Project As", tr("Save Project &As..."), tr(""), true, false, false, m_menubar);
  initAction(m_fileOpenProject, "document-open", "File.Open Project", tr("&Open Project..."), tr(""), true, false, true, m_menubar);
  initAction(m_fileExit, "system-log-out", "File.Exit", tr("E&xit"), tr(""), true, false, true, m_menubar);
  initAction(m_filePrint, "document-print", "File.Print", tr("&Print..."), tr(""), true, false, false, m_menubar);
  initAction(m_filePrintPreview, "document-print-preview", "File.Print Preview", tr("Print Pre&view..."), tr(""), true, false, false, m_menubar);

// Menu -Map- actions
  initAction(m_mapDraw, "map-draw", "Map.Draw", tr("&Draw"), tr("Draw the visible layers"), true, false, true, m_menubar);
  initAction(m_mapZoomIn, "zoom-in", "Map.Zoom In", tr("Zoom &In"), tr(""), true, true, true, m_menubar);
  initAction(m_mapZoomOut, "zoom-out", "Map.Zoom Out", tr("Zoom &Out"), tr(""), true, true, true, m_menubar);
  initAction(m_mapZoomArea, "zoom-area", "Map.Zoom Area", tr("Zoom &Area"), tr(""), true, true, true, m_menubar);
  initAction(m_mapPan, "pan", "Map.Pan", tr("&Pan"), tr(""), true, true, true, m_menubar);
  initAction(m_mapZoomExtent, "zoom-extent", "Map.Zoom Extent", tr("Zoom &Extent"), tr(""), true, false, true, m_menubar);
  initAction(m_mapPreviousExtent, "edit-undo", "Map.Previous Extent", tr("&Previous Extent"), tr(""), true, false, false, m_menubar);
  initAction(m_mapNextExtent, "edit-redo", "Map.Next Extent", tr("&Next Extent"), tr(""), true, false, false, m_menubar);
  initAction(m_mapMeasureDistance, "distance-measure", "Map.Measure Distance", tr("Measure &Distance"), tr(""), true, true, false, m_menubar);
  initAction(m_mapMeasureArea, "area-measure", "Map.Measure Area", tr("Measure &Area"), tr(""), true, true, false, m_menubar);
  initAction(m_mapMeasureAngle, "angle-measure", "Map.Measure Angle", tr("Measure &Angle"), tr(""), true, true, false, m_menubar);
  initAction(m_mapStopDraw, "process-stop", "Map.Stop Draw", tr("&Stop Draw"), tr("Stop all draw tasks"), true, false, true, m_menubar);

// Group the map tools
  QActionGroup* mapToolsGroup = new QActionGroup(this);
  mapToolsGroup->addAction(m_mapZoomIn);
  mapToolsGroup->addAction(m_mapZoomOut);
  mapToolsGroup->addAction(m_mapZoomArea);
  mapToolsGroup->addAction(m_mapPan);
  mapToolsGroup->addAction(m_mapMeasureDistance);
  mapToolsGroup->addAction(m_mapMeasureArea);
  mapToolsGroup->addAction(m_mapMeasureAngle);
}

void te::qt::af::BaseApplication::initMenus()
{
// Making menus
  m_menubar->setObjectName(QString::fromUtf8("menubar"));
  m_menubar->setGeometry(QRect(0, 0, 640, 21));

// File menu
  m_fileMenu->setObjectName("File");
  m_fileMenu->setTitle(tr("&File"));

  m_recentProjectsMenu->setObjectName("File.Recent Projects");
  m_recentProjectsMenu->setTitle(tr("&Recent Projects"));

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

// Edit menu
  //m_editMenu = new QMenu(m_menubar);
  //m_editMenu->setObjectName("Edit");
  //m_editMenu->setTitle(tr("&Edit"));

  //m_menubar->addAction(m_editMenu->menuAction());

  //m_editMenu->addAction(m_editUndo);
  //m_editMenu->addAction(m_editRedo);
  //m_editMenu->addSeparator();
  //m_editMenu->addAction(m_editCut);
  //m_editMenu->addAction(m_editCopy);
  //m_editMenu->addAction(m_editPaste);
  //m_editMenu->addSeparator();
  //m_editMenu->addAction(m_editSelectAll);
  //m_editMenu->addAction(m_editClear);
  //m_editMenu->addSeparator();
  //m_editMenu->addAction(m_editFind);
  //m_editMenu->addAction(m_editReplace);

// View menu
  m_viewMenu->setObjectName("View");
  m_viewMenu->setTitle(tr("&View"));

  m_viewToolBarsMenu->setObjectName("View.Toolbars");
  m_viewToolBarsMenu->setTitle(tr("&Toolbars"));
  m_viewMenu->addMenu(m_viewToolBarsMenu);

  //m_viewMenu->addAction(m_viewToolBars);
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

// Project menu
  m_projectMenu->setObjectName("Project");
  m_projectMenu->setTitle(tr("&Project"));

  m_projectAddLayerMenu->setObjectName("Project.Add Layer");
  m_projectAddLayerMenu->setTitle(tr("&Add Layer"));

  m_projectMenu->addAction(m_projectRemoveLayer);
  m_projectMenu->addSeparator();
  m_projectMenu->addAction(m_projectProperties);
  m_projectAddLayerMenu->addAction(m_projectAddLayerDataset);
  m_projectAddLayerMenu->addAction(m_projectAddLayerQueryDataSet);

  m_layerMenu->setObjectName("Layer");
  m_layerMenu->setTitle(tr("&Layer"));

  m_layerMenu->addAction(m_layerEdit);
  m_layerMenu->addAction(m_layerRename);
  m_layerMenu->addAction(m_layerExport);
  m_layerMenu->addAction(m_layerProperties);
  m_layerMenu->addAction(m_layerShowTable);
  m_layerMenu->addSeparator();

  m_layerChartsMenu->setObjectName("Layer.Charts");
  m_layerChartsMenu->setTitle(tr("&Charts"));

  m_layerMenu->addMenu(m_layerChartsMenu);
  m_layerChartsMenu->addAction(m_layerChartsHistogram);
  m_layerChartsMenu->addAction(m_layerChartsScatter);
  m_layerMenu->addSeparator();

  m_layerMenu->addAction(m_layerRaise);
  m_layerMenu->addAction(m_layerLower);
  m_layerMenu->addAction(m_layerToTop);
  m_layerMenu->addAction(m_layerToBottom);

// Map Menu
  m_mapMenu->setObjectName("Map");
  m_mapMenu->setTitle(tr("&Map"));

  m_mapMenu->addAction(m_mapDraw);
  m_mapMenu->addSeparator();
  m_mapMenu->addAction(m_mapZoomIn);
  m_mapMenu->addAction(m_mapZoomOut);
  m_mapMenu->addAction(m_mapZoomArea);
  m_mapMenu->addAction(m_mapPan);
  m_mapMenu->addAction(m_mapZoomExtent);
  m_mapMenu->addAction(m_mapPreviousExtent);
  m_mapMenu->addAction(m_mapNextExtent);
  m_mapMenu->addSeparator();
  m_mapMenu->addAction(m_mapMeasureDistance);
  m_mapMenu->addAction(m_mapMeasureArea);
  m_mapMenu->addAction(m_mapMeasureAngle);
  m_mapMenu->addSeparator();
  m_mapMenu->addAction(m_mapStopDraw);

// Tools menu
  m_toolsMenu->setObjectName("Tools");
  m_toolsMenu->setTitle(tr("&Tools"));

//  m_toolsMenu->addAction(m_toolbarsManagement);
  m_toolsMenu->addSeparator();
  m_toolsMenu->addAction(m_toolsDataExchanger);
  m_toolsMenu->addAction(m_toolsDataSourceManagement);
  m_toolsMenu->addSeparator();
  m_toolsMenu->addAction(m_toolsCustomize);  

// Plugins menu
  m_pluginsMenu->setObjectName("Plugins");
  m_pluginsMenu->setTitle(tr("Pl&ugins"));

  m_pluginsMenu->addAction(m_pluginsManager);
  m_pluginsMenu->addSeparator();
  m_pluginsMenu->addAction(m_pluginsBuilder);

// Help menu
  m_helpMenu->setObjectName("Help");
  m_helpMenu->setTitle(tr("&Help"));

  m_helpMenu->addAction(m_helpContents);
  m_helpMenu->addAction(m_helpUpdate);

// Sets the toolbar
  setMenuBar(m_menubar);

  //! Register menu bar 
  ApplicationController::getInstance().registerMenuBar(m_menubar);
}

void te::qt::af::BaseApplication::initToolbars()
{
 // Status Bar
  m_statusbar = new QStatusBar(this);
  m_statusbar->setObjectName("StatusBar");
  setStatusBar(m_statusbar);

  std::vector<QToolBar*> bars = ReadToolBarsFromSettings(this);
  std::vector<QToolBar*>::iterator it;

  for(it=bars.begin(); it!=bars.end(); ++it)
  {
    QToolBar* bar = *it;
    addToolBar(Qt::TopToolBarArea, bar);
    m_viewToolBarsMenu->addAction(bar->toggleViewAction());
    ApplicationController::getInstance().registerToolBar(bar->objectName(), bar);
  }
// File Tool Bar
  //m_fileToolBar = new QToolBar(this);
  //m_fileToolBar->setObjectName("File Tool Bar");
  //addToolBar(Qt::TopToolBarArea, m_fileToolBar);
  //m_fileToolBar->setWindowTitle(tr("File Tool Bar"));
  //m_fileToolBar->addAction(m_fileNewProject);
  //m_fileToolBar->addAction(m_fileOpenProject);
  //m_fileToolBar->addAction(m_fileSaveProject);

  //m_viewToolBarsMenu->addAction(m_fileToolBar->toggleViewAction());

// Edit Tool Bar
  //m_editToolBar = new QToolBar(this);
  //m_editToolBar->setObjectName("EditToolBar");
  //addToolBar(Qt::TopToolBarArea, m_editToolBar);
  //m_editToolBar->setWindowTitle(tr("Edit Tool Bar"));
  //m_editToolBar->addAction(m_editUndo);
  //m_editToolBar->addAction(m_editRedo);
  //m_editToolBar->addSeparator();
  //m_editToolBar->addAction(m_editCut);
  //m_editToolBar->addAction(m_editCopy);
  //m_editToolBar->addAction(m_editPaste);

  //m_viewToolBarsMenu->addAction(m_editToolBar->toggleViewAction());

// Map Display Tool Bar
  //m_mapToolBar = new QToolBar(this);
  //m_mapToolBar->setObjectName("Map Tool Bar");
  //addToolBar(Qt::TopToolBarArea, m_mapToolBar);
  //m_mapToolBar->setWindowTitle(tr("Map Tool Bar"));
  //m_mapToolBar->addAction(m_mapDraw);
  //m_mapToolBar->addAction(m_mapZoomIn);
  //m_mapToolBar->addAction(m_mapZoomOut);
  //m_mapToolBar->addAction(m_mapZoomArea);
  //m_mapToolBar->addAction(m_mapPan);
  //m_mapToolBar->addAction(m_mapZoomExtent);
  //m_mapToolBar->addAction(m_mapPreviousExtent);
  //m_mapToolBar->addAction(m_mapNextExtent);

  QToolButton* stopDrawToolButton = new QToolButton(m_statusbar);
  stopDrawToolButton->setDefaultAction(m_mapStopDraw);
  m_statusbar->addPermanentWidget(stopDrawToolButton);

//  m_viewToolBarsMenu->addAction(m_mapToolBar->toggleViewAction());

// registering the toolbars
  //ApplicationController::getInstance().registerToolBar(m_fileToolBar->objectName(), m_fileToolBar);
  ////ApplicationController::getInstance().registerToolBar("EditToolBar", m_editToolBar);
  //ApplicationController::getInstance().registerToolBar(m_mapToolBar->objectName(), m_mapToolBar);
}

void te::qt::af::BaseApplication::initSlotsConnections()
{
  connect(m_fileExit, SIGNAL(triggered()), SLOT(close()));
  connect(m_projectAddLayerDataset, SIGNAL(triggered()), SLOT(onAddDataSetLayerTriggered()));
  connect(m_projectAddLayerQueryDataSet, SIGNAL(triggered()), SLOT(onAddQueryLayerTriggered()));
  connect(m_pluginsManager, SIGNAL(triggered()), SLOT(onPluginsManagerTriggered()));
  connect(m_pluginsBuilder, SIGNAL(triggered()), SLOT(onPluginsBuilderTriggered()));
  connect(m_recentProjectsMenu, SIGNAL(triggered(QAction*)), SLOT(onRecentProjectsTriggered(QAction*)));
  connect(m_fileNewProject, SIGNAL(triggered()), SLOT(onNewProjectTriggered()));
  connect(m_fileOpenProject, SIGNAL(triggered()), SLOT(onOpenProjectTriggered()));
  connect(m_fileSaveProjectAs, SIGNAL(triggered()), SLOT(onSaveProjectAsTriggered()));
  connect(m_toolsCustomize, SIGNAL(triggered()), SLOT(onToolsCustomizeTriggered()));
  connect(m_toolsDataExchanger, SIGNAL(triggered()), SLOT(onToolsDataExchangerTriggered()));
  connect(m_helpContents, SIGNAL(triggered()), SLOT(onHelpTriggered()));
  connect(m_projectProperties, SIGNAL(triggered()), SLOT(onProjectPropertiesTriggered()));
  connect(m_layerChartsHistogram, SIGNAL(triggered()), SLOT(onLayerHistogramTriggered()));
  connect(m_layerChartsScatter, SIGNAL(triggered()), SLOT(onLayerScatterTriggered()));
  connect(m_layerProperties, SIGNAL(triggered()), SLOT(onLayerPropertiesTriggered()));
  connect(m_mapDraw, SIGNAL(triggered()), SLOT(onDrawTriggered()));
  connect(m_mapZoomIn, SIGNAL(toggled(bool)), SLOT(onZoomInToggled(bool)));
  connect(m_mapZoomOut, SIGNAL(toggled(bool)), SLOT(onZoomOutToggled(bool)));
  connect(m_mapZoomArea, SIGNAL(toggled(bool)), SLOT(onZoomAreaToggled(bool)));
  connect(m_mapPan, SIGNAL(toggled(bool)), SLOT(onPanToggled(bool)));
  connect(m_mapZoomExtent, SIGNAL(triggered()), SLOT(onZoomExtentTriggered()));
  connect(m_mapMeasureDistance, SIGNAL(toggled(bool)), SLOT(onMeasureDistanceToggled(bool)));
  connect(m_mapMeasureArea, SIGNAL(toggled(bool)), SLOT(onMeasureAreaToggled(bool)));
  connect(m_mapMeasureAngle, SIGNAL(toggled(bool)), SLOT(onMeasureAngleToggled(bool)));
  connect(m_mapStopDraw, SIGNAL(triggered()), SLOT(onStopDrawTriggered()));
  connect(m_layerShowTable, SIGNAL(triggered()), SLOT(onLayerShowTableTriggered()));
}
