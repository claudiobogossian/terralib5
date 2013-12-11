/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../../common/UserApplicationSettings.h"
#include "../../maptools/Utils.h"
#include "../../srs/Config.h"
#include "../widgets/canvas/EyeBirdMapDisplayWidget.h"
#include "../widgets/canvas/MultiThreadMapDisplay.h"
#include "../widgets/canvas/ZoomInMapDisplayWidget.h"
#include "../widgets/charts/ChartLayerDialog.h"
#include "../widgets/charts/HistogramDialog.h"
#include "../widgets/charts/ScatterDialog.h"
#include "../widgets/datasource/core/DataSourceType.h"
#include "../widgets/datasource/core/DataSourceTypeManager.h"
#include "../widgets/datasource/selector/DataSourceExplorerDialog.h"
#include "../widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../widgets/exchanger/DataExchangerWizard.h"
#include "../widgets/exchanger/DirectExchangerDialog.h"
#include "../widgets/externalTable/DataPropertiesDialog.h"
#include "../widgets/help/HelpManager.h"
#include "../widgets/layer/explorer/ColorMapItem.h"
#include "../widgets/layer/explorer/ChartItem.h"
#include "../widgets/layer/explorer/GroupingItem.h"
#include "../widgets/layer/explorer/LayerExplorer.h"
#include "../widgets/layer/explorer/LayerTreeView.h"
#include "../widgets/layer/explorer/AbstractTreeItem.h"
#include "../widgets/layer/explorer/FolderLayerItem.h"
#include "../widgets/layer/info/LayerPropertiesInfoWidget.h"
#include "../widgets/layer/selector/AbstractLayerSelector.h"
#include "../widgets/plugin/manager/PluginManagerDialog.h"
#include "../widgets/progress/ProgressViewerBar.h"
#include "../widgets/progress/ProgressViewerDialog.h"
#include "../widgets/progress/ProgressViewerWidget.h"
#include "../widgets/query/QueryLayerBuilderWizard.h"
#include "../widgets/query/QueryDialog.h"
#include "../widgets/se/GroupingDialog.h"
#include "../widgets/se/StyleDockWidget.h"
#include "../widgets/tools/Info.h"
#include "../widgets/tools/Measure.h"
#include "../widgets/tools/Pan.h"
#include "../widgets/tools/Selection.h"
#include "../widgets/tools/ZoomArea.h"
#include "../widgets/tools/ZoomClick.h"
#include "../widgets/srs/SRSManagerDialog.h"
#include "connectors/ChartDisplayDockWidget.h"
#include "connectors/DataSetTableDockWidget.h"
#include "connectors/LayerExplorer.h"
#include "connectors/MapDisplay.h"
#include "connectors/StyleExplorer.h"
#include "events/ApplicationEvents.h"
#include "events/LayerEvents.h"
#include "events/MapEvents.h"
#include "events/ProjectEvents.h"
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
#include <QtGui/QInputDialog>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>

// STL
#include <list>
#include <memory>
#include <utility>

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

void CloseAllTables(std::vector<te::qt::af::DataSetTableDockWidget*>& tables)
{
  std::vector<te::qt::af::DataSetTableDockWidget*>::iterator it;

  for(it=tables.begin(); it!=tables.end(); ++it)
    (*it)->close();

  tables.clear();
}

te::qt::af::BaseApplication::BaseApplication(QWidget* parent)
  : QMainWindow(parent, 0),
    m_mapCursorSize(QSize(20, 20)),
    m_explorer(0),
    m_display(0),
    m_styleExplorer(0),
    m_project(0),
    m_progressDockWidget(0),
    m_zoomInDisplaysDockWidget(0),
    m_eyeBirdDisplaysDockWidget(0),
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

  te::qt::af::ApplicationController::getInstance().setMsgBoxParentWidget(this);
}

te::qt::af::BaseApplication::~BaseApplication()
{
  delete m_explorer;
  delete m_display;
  delete m_styleExplorer;
  delete m_project;
  delete m_progressDockWidget;
  delete m_zoomInDisplaysDockWidget;
  delete m_eyeBirdDisplaysDockWidget;

  while(!m_tableDocks.empty())
    delete *m_tableDocks.begin();

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

  te::qt::af::ApplicationController::getInstance().initializeProjectMenus();

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

  if(recentProject.isEmpty() || !te::qt::af::GetOpenLastProjectFromSettings())
    newProject();
  else
  {
    try 
    {
      openProject(recentProject);
    } 
    catch (const te::common::Exception& ex) 
    {
      QString msgErr(tr("Error loading the project: %1!"));
      
      msgErr = msgErr.arg(ex.what());
      
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msgErr);
      
      newProject();
    }
  }

  // The selection tool come active as default
  onSelectionToggled(true);
  m_mapSelection->setChecked(true);

  m_viewDataTable->setChecked(false);
  m_viewDataTable->setEnabled(false);
}

void  te::qt::af::BaseApplication::resetState()
{
  te::qt::af::RestoreState(this);
}

void te::qt::af::BaseApplication::onApplicationTriggered(te::qt::af::evt::Event* evt)
{
  switch(evt->m_id)
  {
    case te::qt::af::evt::PROJECT_UNSAVED:
    {
      // Add the unsave asterisk
      setWindowTitle(te::qt::af::UnsavedStar(windowTitle(), true));

      m_project->setProjectAsChanged(true);
    }
    break;

    case te::qt::af::evt::TOOLBAR_ADDED:
    {
      te::qt::af::evt::ToolBarAdded* e = static_cast<te::qt::af::evt::ToolBarAdded*>(evt);
      QMainWindow::addToolBar(Qt::TopToolBarArea, e->m_toolbar);
    }
    break;

    case te::qt::af::evt::COORDINATE_TRACKED:
    {
      te::qt::af::evt::CoordinateTracked* e = static_cast<te::qt::af::evt::CoordinateTracked*>(evt);
      QString text = "(" + QString::number(e->m_x, 'f', 5) + " , " + QString::number(e->m_y, 'f', 5) + ")";
      m_coordinateLineEdit->setText(text);
    }
    break;

    case te::qt::af::evt::MAP_SRID_CHANGED:
    {
      te::qt::af::evt::MapSRIDChanged* e = static_cast<te::qt::af::evt::MapSRIDChanged*>(evt);

      std::pair<int, std::string> srid = e->m_srid;
      
      if (srid.first != TE_UNKNOWN_SRS)
      {
        QString sridText(srid.second.c_str());
        sridText += ":" + QString::number(srid.first);
        m_mapSRIDLineEdit->setText(sridText);
      }
      else 
      {
        m_mapSRIDLineEdit->setText("Unknown SRS");
        m_coordinateLineEdit->setText("Coordinates");
      }
    }
    break;

    default:
      break;
  }
}

void te::qt::af::BaseApplication::onAddDataSetLayerTriggered()
{
  try
  {
    // Get the parent layer where the dataset layer(s) will be added.
    te::map::AbstractLayerPtr parentLayer(0);

    std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedLayerItems();

    if(selectedLayerItems.size() == 1 && selectedLayerItems.front()->getItemType() == "FOLDER_LAYER_ITEM")
      parentLayer = selectedLayerItems.front()->getLayer();

    // Get the layer(s) to be added
    std::auto_ptr<te::qt::widgets::DataSourceSelectorDialog> dselector(new te::qt::widgets::DataSourceSelectorDialog(this));

    QString dsTypeSett = GetLastDatasourceFromSettings();

    if(!dsTypeSett.isNull() && !dsTypeSett.isEmpty())
      dselector->setDataSourceToUse(dsTypeSett);

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

    std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin();
    std::list<te::map::AbstractLayerPtr>::const_iterator itend = layers.end();

    while(it != itend)
    {
      if((m_explorer != 0) && (m_explorer->getExplorer() != 0))
      {
        te::qt::af::evt::LayerAdded evt(*it, parentLayer);
        te::qt::af::ApplicationController::getInstance().broadcast(&evt);
      }
      ++it;
    }

    SaveLastDatasourceOnSettings(dsTypeId.c_str());

    te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
    ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
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
    if(m_project == 0)
      throw Exception(TR_QT_AF("Error: there is no opened project!"));

    // Get the parent layer where the dataset layer(s) will be added.
    te::map::AbstractLayerPtr parentLayer(0);

    std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedLayerItems();

    if(selectedLayerItems.size() == 1 && selectedLayerItems.front()->getItemType() == "FOLDER_LAYER_ITEM")
      parentLayer = selectedLayerItems.front()->getLayer();

    std::auto_ptr<te::qt::widgets::QueryLayerBuilderWizard> qlb(new te::qt::widgets::QueryLayerBuilderWizard(this));

    std::list<te::map::AbstractLayerPtr> layers = te::qt::af::ApplicationController::getInstance().getProject()->getAllLayers();

    qlb->setLayerList(layers);

    int retval = qlb->exec();

    if(retval == QDialog::Rejected)
      return;

    te::map::AbstractLayerPtr layer = qlb->getQueryLayer();

    if((m_explorer != 0) && (m_explorer->getExplorer() != 0))
    {
      te::qt::af::evt::LayerAdded evt(layer, parentLayer);
      te::qt::af::ApplicationController::getInstance().broadcast(&evt);
    }

    te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
    ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
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

void te::qt::af::BaseApplication::onAddTabularLayerTriggered()
{
  try
  {
    if(m_project == 0)
      throw Exception(TR_QT_AF("Error: there is no opened project!"));

    // Get the parent layer where the tabular layer will be added.
    te::map::AbstractLayerPtr parentLayer(0);

    std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedLayerItems();

    if(selectedLayerItems.size() == 1 && selectedLayerItems.front()->getItemType() == "FOLDER_LAYER_ITEM")
      parentLayer = selectedLayerItems.front()->getLayer();

    te::qt::widgets::DataPropertiesDialog dlg (this);
    int res = dlg.exec();
    if (res == QDialog::Accepted)
    {
      if((m_explorer != 0) && (m_explorer->getExplorer() != 0))
      {
        te::qt::af::evt::LayerAdded evt(dlg.getDataSetAdapterLayer(), parentLayer);
        te::qt::af::ApplicationController::getInstance().broadcast(&evt);
      }

      te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
      ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
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
                         tr("Unknown error while trying to add a layer from a queried dataset!"));
  }
}

void te::qt::af::BaseApplication::onRemoveLayerTriggered()
{
 std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedLayerItems();

  if(selectedLayerItems.empty())
  {
    QString msg = tr("Select at least one layer to be removed!");
    QMessageBox::warning(this, tr("Remove Layer"), msg);

    return;
  }

  QString msg;
  QString questionTitle;

  if(selectedLayerItems.size() == 1)
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

  if(reply == QMessageBox::No)
    return;

  std::list<te::qt::widgets::AbstractTreeItem*>::const_iterator it;
  for(it = selectedLayerItems.begin();  it != selectedLayerItems.end(); ++it)
  {
    te::qt::af::evt::LayerRemoved evt((*it)->getLayer());
    te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  }
}

void te::qt::af::BaseApplication::onLayerRemoveItemTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> selectedItems = m_explorer->getExplorer()->getSelectedItems();

  std::list<te::qt::widgets::AbstractTreeItem*>::const_iterator it;
  for(it = selectedItems.begin();  it != selectedItems.end(); ++it)
  {
    te::qt::af::evt::ItemOfLayerRemoved evt((*it));
    te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  }
}

void te::qt::af::BaseApplication::onRenameLayerTriggered()
{
 std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedLayerItems();

  if(selectedLayerItems.empty() ||
    (selectedLayerItems.size() == 1 &&  !selectedLayerItems.front()->getLayer()) ||
    selectedLayerItems.size() > 1)
  {
    QString msg = tr("Select only one layer to be renamed!");
    QMessageBox::warning(this, tr("Rename Layer"), msg);

    return;
  }

  bool ok;
  QString text = QInputDialog::getText(this, ApplicationController::getInstance().getAppTitle(),
                                       tr("Rename Layer:"), QLineEdit::Normal,
                                       tr("Enter the new layer name"), &ok);

  if (!ok)
    return;

  if(text.isEmpty())
  {
    QMessageBox::warning(this, ApplicationController::getInstance().getAppTitle(), tr("Enter the new name!"));
    return;
  }

  te::qt::widgets::AbstractTreeItem* selectedLayerItem = selectedLayerItems.front();
  te::map::AbstractLayerPtr layer = selectedLayerItem->getLayer();
  layer->setTitle(text.toStdString());

  te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
  ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
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

void te::qt::af::BaseApplication::onRecentProjectsTriggered(QAction* proj)
{
  QString projFile = proj->data().toString();

  openProject(projFile);
}

void te::qt::af::BaseApplication::onNewProjectTriggered()
{
  newProject();

  onSaveProjectAsTriggered();
}

void te::qt::af::BaseApplication::onOpenProjectTriggered()
{
  QString file = QFileDialog::getOpenFileName(this, tr("Open project file"), qApp->applicationDirPath(), tr("XML File (*.xml *.XML)"));

  if(file.isEmpty())
    return;

  try
  {
    openProject(file);
  }
  catch(const te::common::Exception& e)
  {
    QString msg = tr("Fail to open project.");
    msg += " ";
    msg += e.what();
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msg);
  }
}

void te::qt::af::BaseApplication::onSaveProjectTriggered()
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

  m_project->setProjectAsChanged(false);

  setWindowTitle(te::qt::af::UnsavedStar(windowTitle(), m_project->hasChanged()));

  te::qt::af::ApplicationController::getInstance().updateRecentProjects(m_project->getFileName().c_str(), m_project->getTitle().c_str());

  te::qt::af::SaveDataSourcesFile();
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

  m_project->setProjectAsChanged(false);

  setWindowTitle(te::qt::af::UnsavedStar(windowTitle(), m_project->hasChanged()));

  ApplicationController::getInstance().updateRecentProjects(fileName, m_project->getTitle().c_str());

  QString projectTile(tr(" - Project: %1 - %2"));
  std::string name = boost::filesystem::basename(m_project->getFileName()) + boost::filesystem::extension(m_project->getFileName());
  setWindowTitle(te::qt::af::ApplicationController::getInstance().getAppTitle() + projectTile.arg(m_project->getTitle().c_str(), name.c_str()));

  te::qt::af::SaveDataSourcesFile();
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

void te::qt::af::BaseApplication::onToolsDataExchangerDirectTriggered()
{
  try
  {
    te::qt::widgets::DirectExchangerDialog dlg(this);

    std::list<te::map::AbstractLayerPtr> layers = te::qt::af::ApplicationController::getInstance().getProject()->getAllLayers();
    dlg.setLayers(layers);

    dlg.exec();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void  te::qt::af::BaseApplication::onToolsDataExchangerDirectPopUpTriggered()
{
  try
  {
    te::qt::widgets::DirectExchangerDialog dlg(this);

    std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedSingleLayerItems();

    if(selectedLayerItems.empty())
    {
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(),
                           tr("Select a single layer in the layer explorer!"));
      return;
    }

    te::qt::widgets::AbstractTreeItem* selectedLayerItem = *(selectedLayerItems.begin());
    te::map::AbstractLayerPtr selectedLayer = selectedLayerItem->getLayer();

    std::list<te::map::AbstractLayerPtr> layers;
    layers.push_back(selectedLayer);

    dlg.setLayers(layers);

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
  
  if(editor.exec() == QDialog::Accepted)
  {
    QString projectTile(tr(" - Project: %1 - %2"));
    std::string fName = boost::filesystem::basename(m_project->getFileName()) + boost::filesystem::extension(m_project->getFileName());
    setWindowTitle(te::qt::af::ApplicationController::getInstance().getAppTitle() + projectTile.arg(m_project->getTitle().c_str(), fName.c_str()));
  }
}

void te::qt::af::BaseApplication::onAddFolderLayerTriggered()
{
  // Get the parent item where the folder layer will be added.
  te::map::AbstractLayerPtr parentLayer(0);

  std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedLayerItems();

  if(selectedLayerItems.size() == 1 && selectedLayerItems.front()->getItemType() == "FOLDER_LAYER_ITEM")
    parentLayer = selectedLayerItems.front()->getLayer();

  // Get the folder layer to be added
  bool ok;
  QString text = QInputDialog::getText(this, ApplicationController::getInstance().getAppTitle(),
                                      tr("Folder layer name:"), QLineEdit::Normal,
                                      tr("Enter folder layer name"), &ok);

  if (!ok)
    return;

  if(text.isEmpty())
  {
    QMessageBox::warning(this, ApplicationController::getInstance().getAppTitle(), tr("Enter the layer name!"));
    return;
  }

  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  te::map::AbstractLayerPtr folderLayer(new te::map::FolderLayer);
  folderLayer->setTitle(text.toStdString());
  folderLayer->setId(id);

  te::qt::af::evt::LayerAdded evt(folderLayer, parentLayer);
  te::qt::af::ApplicationController::getInstance().broadcast(&evt);

  te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
  ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
}

void te::qt::af::BaseApplication::onLayerPropertiesTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> layers = m_explorer->getExplorer()->getSelectedItems();

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
  doc->setAttribute(Qt::WA_DeleteOnClose, true);

  doc->show();
}

void te::qt::af::BaseApplication::onLayerRemoveSelectionTriggered()
{
  std::list<te::map::AbstractLayerPtr> layers =  m_explorer->getExplorer()->getSelectedSingleLayers();

  if(layers.empty())
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("There's no selected layer."));
    return;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while(it != layers.end())
  {
    te::map::AbstractLayerPtr layer = (*it);
    layer->clearSelected();

    ++it;

    te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
    ApplicationController::getInstance().broadcast(&e);
  }
}

void te::qt::af::BaseApplication::onLayerSRSTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> layers = m_explorer->getExplorer()->getSelectedItems();
  
  if(layers.empty())
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("There's no selected layer."));
    return;
  }
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));
  
  if(srsDialog.exec() == QDialog::Rejected)
    return;
  
  std::pair<int, std::string> srid = srsDialog.getSelectedSRS();
  
  te::map::AbstractLayerPtr lay = (*layers.begin())->getLayer();
  
  lay->setSRID(srid.first);
}

void te::qt::af::BaseApplication::onLayerShowTableTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> layers = m_explorer->getExplorer()->getSelectedItems();

  if(layers.empty())
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("There's no selected layer."));
    return;
  }

  te::map::AbstractLayerPtr lay = (*layers.begin())->getLayer();

  if (lay->getSchema()->hasRaster())
    return;


  te::qt::af::DataSetTableDockWidget* doc = GetLayerDock(lay.get(), m_tableDocks);

  if(doc == 0)
  {
    doc = new te::qt::af::DataSetTableDockWidget(this);
    doc->setLayer(lay.get());
    addDockWidget(Qt::BottomDockWidgetArea, doc);

    connect (doc, SIGNAL(closed(te::qt::af::DataSetTableDockWidget*)), SLOT(onLayerTableClose(te::qt::af::DataSetTableDockWidget*)));

    if(!m_tableDocks.empty())
      tabifyDockWidget(m_tableDocks[m_tableDocks.size()-1], doc);
  
    m_tableDocks.push_back(doc);

    ApplicationController::getInstance().addListener(doc);
  }

  doc->show();
  doc->raise();

  m_viewDataTable->setChecked(true);

  m_viewDataTable->setEnabled(true);
}

void te::qt::af::BaseApplication::onLayerHistogramTriggered()
{
  try
  {
    std::list<te::map::AbstractLayerPtr> selectedLayers = m_explorer->getExplorer()->getSelectedSingleLayers();

    if(selectedLayers.empty())
    {
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(),
                           tr("Select a layer in the layer explorer!"));
      return;
    }

    // The histogram will be created based on the first selected layer
    te::map::AbstractLayerPtr selectedLayer = *(selectedLayers.begin());

    const te::map::LayerSchema* schema = selectedLayer->getSchema().release();

    te::da::DataSet* dataset = selectedLayer->getData().release();
    te::da::DataSetType* dataType = (te::da::DataSetType*) schema;

    te::qt::widgets::HistogramDialog dlg(dataset, dataType, this);

    int res = dlg.exec();
    if (res == QDialog::Accepted)
    {
      ChartDisplayDockWidget* doc = new ChartDisplayDockWidget(dlg.getDisplayWidget(), this);
      doc->setSelectionColor(ApplicationController::getInstance().getSelectionColor());
      doc->setWindowTitle("Histogram");
      doc->setWindowIcon(QIcon::fromTheme("chart-bar"));
      doc->setLayer(selectedLayer.get());

      ApplicationController::getInstance().addListener(doc);
      addDockWidget(Qt::RightDockWidgetArea, doc, Qt::Horizontal);
      doc->show();
    }
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
    std::list<te::map::AbstractLayerPtr> selectedLayers = m_explorer->getExplorer()->getSelectedSingleLayers();

    if(selectedLayers.empty())
    {
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(),
                           tr("Select a layer in the layer explorer!"));
      return;
    }

    // The scatter will be created based on the first selected layer
    te::map::AbstractLayerPtr selectedLayer = *(selectedLayers.begin());

    const te::map::LayerSchema* schema = selectedLayer->getSchema().release();

    te::da::DataSet* dataset = selectedLayer->getData().release();
    te::da::DataSetType* dataType = (te::da::DataSetType*) schema;

    te::qt::widgets::ScatterDialog dlg(dataset, dataType, this);
    int res = dlg.exec();
    if (res == QDialog::Accepted)
    {
      ChartDisplayDockWidget* doc = new ChartDisplayDockWidget(dlg.getDisplayWidget(), this);

      doc->setSelectionColor(ApplicationController::getInstance().getSelectionColor());
      doc->setWindowTitle("Scatter");
      doc->setWindowIcon(QIcon::fromTheme("chart-scatter"));
      ApplicationController::getInstance().addListener(doc);
      doc->setLayer(selectedLayer.get());

      addDockWidget(Qt::RightDockWidgetArea, doc, Qt::Horizontal);
      doc->show();
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onLayerChartTriggered()
{
 try
  {
    std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedSingleLayerItems();

    if(selectedLayerItems.empty())
    {
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(),
                           tr("Select a single layer in the layer explorer!"));
      return;
    }

    // The chart will be accomplished only on the first single layer selected
    te::qt::widgets::AbstractTreeItem* selectedLayerItem = *(selectedLayerItems.begin());
    te::map::AbstractLayerPtr selectedLayer = selectedLayerItem->getLayer();

    te::qt::widgets::ChartLayerDialog dlg(this);
    dlg.setLayer(selectedLayer);

    // If the selected layer has a chart associated to it, set the chart layer
    // dialog for initializing with this chart.
    te::map::Chart* chart = selectedLayer->getChart();

    if(chart)
      dlg.setChart(chart);

    // Check if the selected layer item has a chart item; in positive case, remove it from the layer item.
    te::qt::widgets::ChartItem* chartItem = selectedLayerItem->findChild<te::qt::widgets::ChartItem*>();

    if(chartItem)
      m_explorer->getExplorer()->remove(chartItem);

    // Collapse the selected layer item to allow the new chart item to be generated
    // in the next time the selected layer item is expanded.
    m_explorer->getExplorer()->collapse(selectedLayerItem);
    
    if(dlg.exec() == QDialog::Accepted)
    {
      // Expand the selected layer item and the chart item
      m_explorer->getExplorer()->expand(selectedLayerItem);

      chartItem = selectedLayerItem->findChild<te::qt::widgets::ChartItem*>();
      if(chartItem)
        m_explorer->getExplorer()->expand(chartItem);

      m_display->getDisplay()->refresh();

      // Send out an event informing that the project is not saved
      te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
      ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onLayerGroupingTriggered()
{
  try
  {
    std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedSingleLayerItems();

    if(selectedLayerItems.empty())
    {
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(),
                           tr("Select a single layer in the layer explorer!"));
      return;
    }

    // The object grouping will be accomplished only on the first layer selected
    te::qt::widgets::AbstractTreeItem* selectedLayerItem = *(selectedLayerItems.begin());
    te::map::AbstractLayerPtr selectedLayer = selectedLayerItem->getLayer();

    te::qt::widgets::GroupingDialog dlg(this);
    dlg.setLayer(selectedLayer);

    // Check if the selected layer item has a grouping item; in positive case, remove it from the layer item.
    te::qt::widgets::GroupingItem* groupingItem = selectedLayerItem->findChild<te::qt::widgets::GroupingItem*>();

    if(groupingItem)
      m_explorer->getExplorer()->remove(groupingItem);

    // Check if the selected layer item has a color map item; in positive case, remove it from the layer item.
    te::qt::widgets::ColorMapItem* cmi = selectedLayerItem->findChild<te::qt::widgets::ColorMapItem*>();

    if(cmi)
      m_explorer->getExplorer()->remove(cmi);

    // Collapse the selected layer item to allow the new grouping item to be generated
    // in the next time the selected layer item is expanded.
    m_explorer->getExplorer()->collapse(selectedLayerItem);

    if(dlg.exec() == QDialog::Accepted)
    {
      // Expand the selected layer item and the grouping item
      m_explorer->getExplorer()->expand(selectedLayerItem);

      groupingItem = selectedLayerItem->findChild<te::qt::widgets::GroupingItem*>();
      if(groupingItem)
        m_explorer->getExplorer()->expand(groupingItem);

      cmi = selectedLayerItem->findChild<te::qt::widgets::ColorMapItem*>();
      if(cmi)
        m_explorer->getExplorer()->expand(cmi);

      m_display->getDisplay()->refresh();

      // Send out an event informing that the project is not saved
      te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
      ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onMapSRIDTriggered()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if(srsDialog.exec() == QDialog::Rejected)
    return;

  std::pair<int, std::string> srid = srsDialog.getSelectedSRS();

  te::qt::af::evt::MapSRIDChanged mapSRIDChagned(srid);
  ApplicationController::getInstance().broadcast(&mapSRIDChagned);

  m_display->getDisplay()->setSRID(srid.first);
}

void te::qt::af::BaseApplication::onMapSetUnknwonSRIDTriggered()
{
  std::pair<int, std::string> srid = std::make_pair(TE_UNKNOWN_SRS, "");
  te::qt::af::evt::MapSRIDChanged mapSRIDChagned(srid);
  ApplicationController::getInstance().broadcast(&mapSRIDChagned);
  
  m_display->getDisplay()->setSRID(TE_UNKNOWN_SRS);
}

void te::qt::af::BaseApplication::onDrawTriggered()
{
  te::qt::af::evt::DrawButtonClicked drawClicked;
  ApplicationController::getInstance().broadcast(&drawClicked);

  m_display->draw(ApplicationController::getInstance().getProject()->getVisibleSingleLayers());
}

void te::qt::af::BaseApplication::onLayerFitOnMapDisplayTriggered()
{
  try
  {
    std::list<te::map::AbstractLayerPtr> selectedLayers = m_explorer->getExplorer()->getSelectedSingleLayers();

    if(selectedLayers.empty())
    {
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(),
                           tr("Select a layer in the layer explorer!"));
      return;
    }

    // The layer fitting will be accomplished only on the first layer selected
    te::map::AbstractLayerPtr selectedLayer = *(selectedLayers.begin());

    te::qt::widgets::MapDisplay* display = m_display->getDisplay();

    te::gm::Envelope env = selectedLayer->getExtent();

    if( (display->getSRID() == TE_UNKNOWN_SRS && selectedLayer->getSRID() == TE_UNKNOWN_SRS) || (display->getSRID() == selectedLayer->getSRID()))
    {
      display->setExtent(env, true);
      return;
    }

    if(display->getSRID() == TE_UNKNOWN_SRS || selectedLayer->getSRID() == TE_UNKNOWN_SRS)
    {
      QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(),
                           TR_QT_AF("The spatial reference system of the map display and the layer are not compatible!"));
      return;
    }

    if(display->getSRID() != selectedLayer->getSRID())
      env.transform(selectedLayer->getSRID(), display->getSRID());

    display->setExtent(env, true);
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onLayerFitSelectedOnMapDisplayTriggered()
{
  std::list<te::map::AbstractLayerPtr> layers = m_explorer->getExplorer()->getSelectedSingleLayers();
  if(layers.empty())
  {
    QString msg = tr("Select at least a layer to accomplish this operation!");
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msg);
    return;
  }

  te::gm::Envelope finalEnv = te::map::GetSelectedExtent(layers, m_display->getDisplay()->getSRID(), true);

  if(!finalEnv.isValid())
  {
    QString msg = tr("Select object(s) in the selected layer(s) to accomplish this operation!");
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msg);
    return;
  }

  m_display->getDisplay()->setExtent(finalEnv, true);
}

void te::qt::af::BaseApplication::onLayerPanToSelectedOnMapDisplayTriggered()
{
  std::list<te::map::AbstractLayerPtr> layers = m_explorer->getExplorer()->getSelectedSingleLayers();
  if(layers.empty())
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), tr("There's no selected layer."));
    return;
  }

  te::map::MapDisplay* display = m_display->getDisplay();

  te::gm::Envelope selectedExtent = te::map::GetSelectedExtent(layers, display->getSRID(), true);

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

void te::qt::af::BaseApplication::onQueryLayerTriggered()
{
  te::qt::widgets::QueryDialog* dlg = new te::qt::widgets::QueryDialog(this);
  dlg->setAttribute(Qt::WA_DeleteOnClose, true);

  if(m_project)
    dlg->setList(m_project->getTopLayers());

  std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_explorer->getExplorer()->getSelectedSingleLayerItems();

  if(!selectedLayerItems.empty())
  {
    te::qt::widgets::AbstractTreeItem* selectedLayerItem = *(selectedLayerItems.begin());
    te::map::AbstractLayerPtr selectedLayer = selectedLayerItem->getLayer();

    dlg->setCurrentLayer(selectedLayer);
  }

  connect(dlg, SIGNAL(highlightLayerObjects(const te::map::AbstractLayerPtr&, te::da::DataSet*, const QColor&)),
               SLOT(onHighlightLayerObjects(const te::map::AbstractLayerPtr&, te::da::DataSet*, const QColor&)));

  connect(dlg, SIGNAL(layerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)),
                 SLOT(onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)));

  dlg->show();
}

void te::qt::af::BaseApplication::onZoomInToggled(bool checked)
{
  if(!checked)
    return;

  QCursor zoomAreaCursor(QIcon::fromTheme("zoom-in").pixmap(m_mapCursorSize));

  te::qt::widgets::ZoomArea* zoomArea = new te::qt::widgets::ZoomArea(m_display->getDisplay(), zoomAreaCursor);
  m_display->setCurrentTool(zoomArea);

  te::qt::af::evt::ZoomInButtonToggled ezoom;
  ApplicationController::getInstance().broadcast(&ezoom);
}

void te::qt::af::BaseApplication::onZoomOutToggled(bool checked)
{
  if(!checked)
    return;

  QCursor zoomOutCursor(QIcon::fromTheme("zoom-out").pixmap(m_mapCursorSize));

  te::qt::widgets::ZoomClick* zoomOut = new te::qt::widgets::ZoomClick(m_display->getDisplay(), zoomOutCursor, 2.0, te::qt::widgets::Zoom::Out);
  m_display->setCurrentTool(zoomOut);

  te::qt::af::evt::ZoomOutButtonToggled ezoom;
  ApplicationController::getInstance().broadcast(&ezoom);
}

void te::qt::af::BaseApplication::onPreviousExtentTriggered()
{
  m_display->previousExtent();
}

void te::qt::af::BaseApplication::onNextExtentTriggered()
{
  m_display->nextExtent();
}

void te::qt::af::BaseApplication::onPanToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::Pan* pan = new te::qt::widgets::Pan(m_display->getDisplay(), Qt::OpenHandCursor, Qt::ClosedHandCursor);
  m_display->setCurrentTool(pan);

  te::qt::af::evt::PanButtonToggled epan;
  ApplicationController::getInstance().broadcast(&epan);
}

void te::qt::af::BaseApplication::onZoomExtentTriggered()
{
  if(!m_project && m_project->getTopLayers().empty())
    return;

  //m_display->fit(m_explorer->getExplorer()->getAllLayers());
  m_display->fit(te::qt::af::ApplicationController::getInstance().getProject()->getAllLayers());

}

void te::qt::af::BaseApplication::onInfoToggled(bool checked)
{
  if(!checked)
    return;

  QPixmap pxmap = QIcon::fromTheme("pointer-info").pixmap(m_mapCursorSize);
  QCursor infoCursor(pxmap, 0, 0);

  te::qt::widgets::Info* info = new te::qt::widgets::Info(m_display->getDisplay(), infoCursor, m_explorer->getExplorer()->getSelectedSingleLayers());
  m_display->setCurrentTool(info);

  connect(m_explorer->getExplorer(), SIGNAL(selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)), info, SLOT(setLayers(const std::list<te::map::AbstractLayerPtr>&)));
}

void te::qt::af::BaseApplication::onMapRemoveSelectionTriggered()
{
  //std::list<te::map::AbstractLayerPtr> layers = m_explorer->getExplorer()->getAllLayers();
  std::list<te::map::AbstractLayerPtr> layers = te::qt::af::ApplicationController::getInstance().getProject()->getAllLayers();
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();
  
  while(it != layers.end())
  {
    te::map::AbstractLayerPtr layer = (*it);
    layer->clearSelected();

    ++it;

    te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
    ApplicationController::getInstance().broadcast(&e);
  }
}

void te::qt::af::BaseApplication::onSelectionToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::Selection* selection = new te::qt::widgets::Selection(m_display->getDisplay(), Qt::ArrowCursor, m_explorer->getExplorer()->getSelectedSingleLayers());
  m_display->setCurrentTool(selection);

  connect(m_explorer->getExplorer(), SIGNAL(selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)), selection, SLOT(setLayers(const std::list<te::map::AbstractLayerPtr>&)));
  connect(selection, SIGNAL(layerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)), SLOT(onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)));

  te::qt::af::evt::SelectionButtonToggled esel;
  ApplicationController::getInstance().broadcast(&esel);
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

void te::qt::af::BaseApplication::onLayerTableClose(te::qt::af::DataSetTableDockWidget* wid)
{
  std::vector<DataSetTableDockWidget*>::iterator it;

  for(it=m_tableDocks.begin(); it!=m_tableDocks.end(); ++it)
    if(*it == wid)
      break;

  if(it != m_tableDocks.end())
    m_tableDocks.erase(it);

  if(m_tableDocks.empty())
  {
    m_viewDataTable->setChecked(false);
    m_viewDataTable->setEnabled(false);
  }

  ApplicationController::getInstance().removeListener(wid);
}

void te::qt::af::BaseApplication::onFullScreenToggled(bool checked)
{
  checked ? showFullScreen() : showMaximized();
}

void te::qt::af::BaseApplication::onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr& layer)
{
  assert(layer.get());

  te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
  ApplicationController::getInstance().broadcast(&e);
}

void te::qt::af::BaseApplication::onHighlightLayerObjects(const te::map::AbstractLayerPtr& layer, te::da::DataSet* dataset, const QColor& color)
{
  assert(layer.get());
  assert(dataset);

  te::qt::af::evt::HighlightLayerObjects e(layer, dataset, color);
  ApplicationController::getInstance().broadcast(&e);
}

void te::qt::af::BaseApplication::onLayerExplorerVisibilityChanged(bool visible)
{
  m_viewLayerExplorer->blockSignals(true);
  m_viewLayerExplorer->setChecked(visible);
  m_viewLayerExplorer->blockSignals(false);
}

void te::qt::af::BaseApplication::onDisplayVisibilityChanged(bool visible)
{
  m_viewMapDisplay->blockSignals(true);
  m_viewMapDisplay->setChecked(visible);
  m_viewMapDisplay->blockSignals(false);
}

void te::qt::af::BaseApplication::onDisplayDataTableChanged(bool visible)
{
  if(m_tableDocks.empty())
    return;

  for(std::size_t i = 0; i < m_tableDocks.size(); ++i)
  {
    if(visible)
      m_tableDocks[i]->show();
    else
      m_tableDocks[i]->hide();
  }

  m_viewDataTable->setChecked(visible);
}

void te::qt::af::BaseApplication::onStyleExplorerVisibilityChanged(bool visible)
{
  m_viewStyleExplorer->blockSignals(true);
  m_viewStyleExplorer->setChecked(visible);
  m_viewStyleExplorer->blockSignals(false);
}

void te::qt::af::BaseApplication::onDataSourceExplorerTriggered()
{
  try
  {
    std::auto_ptr<te::qt::widgets::DataSourceExplorerDialog> dExplorer(new te::qt::widgets::DataSourceExplorerDialog(this));

    QString dsTypeSett = GetLastDatasourceFromSettings();

    if(!dsTypeSett.isNull() && !dsTypeSett.isEmpty())
      dExplorer->setDataSourceToUse(dsTypeSett);


    int retval = dExplorer->exec();

    if(retval == QDialog::Rejected)
      return;

    std::list<te::da::DataSourceInfoPtr> selectedDatasources = dExplorer->getSelecteds();

    if(selectedDatasources.empty())
      return;

    dExplorer.reset(0);

    const std::string& dsTypeId = selectedDatasources.front()->getType();

    SaveLastDatasourceOnSettings(dsTypeId.c_str());
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), e.what());
  }
  catch(...)
  {
    QMessageBox::warning(this,
                         te::qt::af::ApplicationController::getInstance().getAppTitle(),
                         tr("DataSetExplorer Error!"));
  }
}

void te::qt::af::BaseApplication::openProject(const QString& projectFileName)
{
  try
  {
    checkProjectSave();

    if(!boost::filesystem::exists(projectFileName.toStdString()))
    {
      QMessageBox::critical(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), (boost::format(TR_QT_AF("This project could not be found: %1%.")) % projectFileName.toStdString()).str().c_str());
      return;
    }

    CloseAllTables(m_tableDocks);

    Project* nproject = te::qt::af::ReadProject(projectFileName.toStdString());

    delete m_project;

    m_project = nproject;

    ApplicationController::getInstance().updateRecentProjects(projectFileName, m_project->getTitle().c_str());

    QString projectTile(tr(" - Project: %1 - %2"));
    std::string fName = boost::filesystem::basename(m_project->getFileName()) + boost::filesystem::extension(m_project->getFileName());
    setWindowTitle(te::qt::af::ApplicationController::getInstance().getAppTitle() + projectTile.arg(m_project->getTitle().c_str(), fName.c_str()));

    te::qt::af::ApplicationController::getInstance().set(m_project);

    te::qt::af::evt::ProjectAdded evt(m_project);

    ApplicationController::getInstance().broadcast(&evt);
  }
  catch(const te::common::Exception& e)
  {
    throw e;
  }
  catch(...)
  {
    QString msg(tr("Fail to open project: %1"));
    
    msg = msg.arg(projectFileName);
    
    throw Exception(TR_QT_AF(msg.toStdString()));
  }
}

void te::qt::af::BaseApplication::checkProjectSave()
{
  if(m_project != 0 && m_project->hasChanged())
  {
    QString msg("The current project has unsaved changes. Do you want to save them?");
    int btn = QMessageBox::question(this, te::qt::af::ApplicationController::getInstance().getAppTitle(), msg, QMessageBox::No, QMessageBox::Yes);

    if(btn == QMessageBox::Yes)
      onSaveProjectTriggered();
  }
}

void te::qt::af::BaseApplication::newProject()
{
  CloseAllTables(m_tableDocks);

  checkProjectSave();

  delete m_project;

  m_project = new Project;

  QString author;
  int maxSaved;

  GetProjectInformationsFromSettings(author, maxSaved);

  m_project->setTitle("New Project");

  m_project->setAuthor(author.toStdString());

  QString projectTile(tr(" - Project: %1 - %2"));
  std::string fName = boost::filesystem::basename(m_project->getFileName()) + boost::filesystem::extension(m_project->getFileName());
  setWindowTitle(te::qt::af::ApplicationController::getInstance().getAppTitle() + projectTile.arg(m_project->getTitle().c_str(), fName.c_str()));

  te::qt::af::ApplicationController::getInstance().set(m_project);

  m_project->setProjectAsChanged(false);

  te::qt::af::evt::ProjectAdded evt(m_project);

  ApplicationController::getInstance().broadcast(&evt);
}

void te::qt::af::BaseApplication::makeDialog()
{
  initActions();

  initMenus();

  initToolbars();

  initSlotsConnections();

  // Initializing well known widgets

  // 1. Layer Explorer
  QAction* act = new QAction(this);
  act->setSeparator(true);

  te::qt::widgets::LayerExplorer* lexplorer = new te::qt::widgets::LayerExplorer(this);
  lexplorer->setObjectName("LayerExplorerDockWidget");
  te::qt::widgets::LayerTreeView* treeView = lexplorer->getTreeView();
  treeView->setAnimated(true);

  // **********************************************************************************************
  // Add the actions to the context menu of the tree view according to the number of selected items
  // **********************************************************************************************

  /****************** Actions to be added to the context menu when there is no item selected ******************/

  treeView->add(m_projectAddLayerMenu->menuAction(), "", "", te::qt::widgets::LayerTreeView::NO_ITEM_SELECTED);

  QAction* noItemSelectedSep = new QAction(this);
  noItemSelectedSep->setSeparator(true);
  treeView->add(noItemSelectedSep, "", "", te::qt::widgets::LayerTreeView::NO_ITEM_SELECTED);

  treeView->add(m_projectAddFolderLayer, "", "", te::qt::widgets::LayerTreeView::NO_ITEM_SELECTED);

  /****************** Actions to be added to the context menu when there is a unique item selected ******************/

  // Actions for the folder layer item
  treeView->add(m_projectAddLayerMenu->menuAction(), "", "FOLDER_LAYER_ITEM");

  QAction* folderSep1 = new QAction(this);
  folderSep1->setSeparator(true);
  treeView->add(folderSep1, "", "FOLDER_LAYER_ITEM");

  treeView->add(m_projectAddFolderLayer, "", "FOLDER_LAYER_ITEM");

  QAction* folderSep2 = new QAction(this);
  folderSep2->setSeparator(true);
  treeView->add(folderSep2, "", "FOLDER_LAYER_ITEM");

  treeView->add(m_projectRemoveLayer, "", "FOLDER_LAYER_ITEM");
  treeView->add(m_projectRenameLayer, "", "FOLDER_LAYER_ITEM");

  // Actions for the single layer item that is not a raster layer
  treeView->add(m_layerObjectGrouping);
  treeView->add(m_toolsDataExchangerDirectPopUp);
  treeView->add(m_layerChartsHistogram);
  treeView->add(m_layerChart);
  treeView->add(m_queryLayer);
  treeView->add(m_layerChartsScatter);

  QAction* actionChartSep = new QAction(this);
  actionChartSep->setSeparator(true);
  treeView->add(actionChartSep);

  treeView->add(m_layerShowTable);
  treeView->add(m_viewStyleExplorer);

  QAction* actionStyleSep = new QAction(this);
  actionStyleSep->setSeparator(true);
  treeView->add(actionStyleSep);

  treeView->add(m_layerRemoveObjectSelection);
  treeView->add(m_projectRemoveLayer);
  treeView->add(m_projectRenameLayer);

  QAction* actionRemoveSep = new QAction(this);
  actionRemoveSep->setSeparator(true);
  treeView->add(actionRemoveSep);

  treeView->add(m_layerFitOnMapDisplay);
  treeView->add(m_layerFitSelectedOnMapDisplay);
  treeView->add(m_layerPanToSelectedOnMapDisplay);

  QAction* actionFitSep = new QAction(this);
  actionFitSep->setSeparator(true);
  treeView->add(actionFitSep);

  treeView->add(m_layerSRS);

  QAction* actionSRSSep = new QAction(this);
  actionSRSSep->setSeparator(true);
  treeView->add(actionSRSSep);

  treeView->add(m_layerProperties);

  // Actions for the items of a layer item such as the chart item and the grouping item
  treeView->add(m_layerRemoveItem, "", "ITEM_OF_LAYER");

  // Actions for the raster layer item
  treeView->add(m_layerObjectGrouping, "", "RASTER_LAYER_ITEM");
  treeView->add(m_layerChartsHistogram, "", "RASTER_LAYER_ITEM");

  QAction* rasterSep1 = new QAction(this);
  rasterSep1->setSeparator(true);
  treeView->add(rasterSep1, "", "RASTER_LAYER_ITEM");

  treeView->add(m_viewStyleExplorer, "", "RASTER_LAYER_ITEM");

  QAction* rasterSep2 = new QAction(this);
  rasterSep2->setSeparator(true);
  treeView->add(rasterSep2, "", "RASTER_LAYER_ITEM");

  treeView->add(m_projectRemoveLayer, "", "RASTER_LAYER_ITEM");
  treeView->add(m_projectRenameLayer, "", "RASTER_LAYER_ITEM");

  QAction* rasterSep3 = new QAction(this);
  rasterSep3->setSeparator(true);
  treeView->add(rasterSep3, "", "RASTER_LAYER_ITEM");

  treeView->add(m_layerFitOnMapDisplay, "", "RASTER_LAYER_ITEM");

  QAction* rasterSep4 = new QAction(this);
  rasterSep4->setSeparator(true);
  treeView->add(rasterSep4, "", "RASTER_LAYER_ITEM");

  treeView->add(m_layerSRS, "", "RASTER_LAYER_ITEM");

  QAction* rasterSep5 = new QAction(this);
  rasterSep5->setSeparator(true);
  treeView->add(rasterSep5, "", "RASTER_LAYER_ITEM");

  treeView->add(m_layerProperties, "", "RASTER_LAYER_ITEM");

  /********* Actions to be added to the context menu when there are multiple items selected ***********/

  treeView->add(m_layerFitSelectedOnMapDisplay, "", "DATASET_LAYER_ITEM", te::qt::widgets::LayerTreeView::MULTIPLE_ITEMS_SELECTED);
  treeView->add(m_layerPanToSelectedOnMapDisplay, "", "DATASET_LAYER_ITEM", te::qt::widgets::LayerTreeView::MULTIPLE_ITEMS_SELECTED);

  treeView->add(m_layerFitSelectedOnMapDisplay, "", "QUERY_LAYER_ITEM", te::qt::widgets::LayerTreeView::MULTIPLE_ITEMS_SELECTED);
  treeView->add(m_layerPanToSelectedOnMapDisplay, "", "QUERY_LAYER_ITEM", te::qt::widgets::LayerTreeView::MULTIPLE_ITEMS_SELECTED);

  QAction* multipleSelectedSep = new QAction(this);
  multipleSelectedSep->setSeparator(true);
  treeView->add(multipleSelectedSep, "", "", te::qt::widgets::LayerTreeView::MULTIPLE_ITEMS_SELECTED);

  treeView->add(m_projectRemoveLayer, "", "", te::qt::widgets::LayerTreeView::MULTIPLE_ITEMS_SELECTED);

  // ********** End the actions to be added to the context menu ********

  QMainWindow::addDockWidget(Qt::LeftDockWidgetArea, lexplorer);

  connect(m_viewLayerExplorer, SIGNAL(toggled(bool)), lexplorer, SLOT(setVisible(bool)));
  m_viewLayerExplorer->setChecked(true);
  connect(lexplorer, SIGNAL(visibilityChanged(bool)), this, SLOT(onLayerExplorerVisibilityChanged(bool)));

  m_explorer = new te::qt::af::LayerExplorer(lexplorer, this);

// 2. Map Display
  te::qt::widgets::MapDisplay* map = new te::qt::widgets::MultiThreadMapDisplay(QSize(512, 512), this);
  map->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  m_display = new te::qt::af::MapDisplay(map);

  connect(m_display, SIGNAL(hasPreviousExtent(bool)), m_mapPreviousExtent, SLOT(setEnabled(bool)));
  connect(m_display, SIGNAL(hasNextExtent(bool)), m_mapNextExtent, SLOT(setEnabled(bool)));

// 3. Symbolizer Explorer

  te::qt::widgets::StyleDockWidget* stylelDock = new te::qt::widgets::StyleDockWidget(tr("Style Explorer"), this);
  stylelDock->setObjectName("StyleDockWidget");
  QMainWindow::addDockWidget(Qt::RightDockWidgetArea, stylelDock);
  stylelDock->connect(m_viewStyleExplorer, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_viewStyleExplorer->setChecked(false);
  stylelDock->setVisible(false);
  connect(stylelDock, SIGNAL(visibilityChanged(bool)), this, SLOT(onStyleExplorerVisibilityChanged(bool)));
  connect(stylelDock, SIGNAL(repaintMapDisplay()), this, SLOT(onDrawTriggered()));

  m_styleExplorer = new te::qt::af::StyleExplorer(stylelDock, this);

  initStatusBar();

// 3. Data Table
//  te::qt::widgets::TabularViewer* view = new te::qt::widgets::TabularViewer(this);

 // m_viewer = new te::qt::af::TabularViewer(view);

// registering framework listeners
  te::qt::af::ApplicationController::getInstance().addListener(this);
  te::qt::af::ApplicationController::getInstance().addListener(m_explorer);
  te::qt::af::ApplicationController::getInstance().addListener(m_display);
  te::qt::af::ApplicationController::getInstance().addListener(m_styleExplorer);
  //te::qt::af::ApplicationController::getInstance().addListener(m_viewer);

// initializing connector widgets
  QDockWidget* doc = new QDockWidget(tr("Map Display"), this);
  doc->setWidget(map);
  QMainWindow::setCentralWidget(doc);
  doc->connect(m_viewMapDisplay, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_viewMapDisplay->setChecked(true);
  connect(doc, SIGNAL(visibilityChanged(bool)), this, SLOT(onDisplayVisibilityChanged(bool)));

  // View Data Table
  connect(m_viewDataTable, SIGNAL(toggled(bool)), this, SLOT(onDisplayDataTableChanged(bool)));

/*  doc = new QDockWidget(tr("Data Table"), this);
  doc->setWidget(view);
  QMainWindow::addDockWidget(Qt::BottomDockWidgetArea, doc);
  doc->connect(m_viewDataTable, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_viewDataTable->setChecked(false);
  doc->setVisible(false);
*/

// Progress support
  te::qt::widgets::ProgressViewerBar* pvb = new te::qt::widgets::ProgressViewerBar(this);
  pvb->setFixedWidth(220);
  te::common::ProgressManager::getInstance().addViewer(pvb);

  te::qt::widgets::ProgressViewerWidget* pvw = new te::qt::widgets::ProgressViewerWidget(this);
  te::common::ProgressManager::getInstance().addViewer(pvw);

  m_statusbar->addPermanentWidget(pvb);

  connect(pvb, SIGNAL(clicked()), this, SLOT(showProgressDockWidget()));

  m_progressDockWidget = new QDockWidget(this);
  m_progressDockWidget->setObjectName("ProgressDockWidget");
  m_progressDockWidget->setWidget(pvw);
  m_progressDockWidget->setMinimumHeight(300);
  m_progressDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
  m_progressDockWidget->setWindowTitle(tr("Tasks Progress"));
  addDockWidget(Qt::RightDockWidgetArea, m_progressDockWidget);
  m_progressDockWidget->setVisible(false);
}

void te::qt::af::BaseApplication::closeEvent(QCloseEvent* e)
{
  checkProjectSave();

  te::qt::af::SaveState(this);

  e->accept();
}

void te::qt::af::BaseApplication::initAction(QAction*& act, const QString& icon, const QString& name,
                                             const QString& text, const QString& tooltip,
                                             bool iconVisibleInMenu, bool isCheckable,
                                             bool enabled, QObject* parent)
{
  act = new QAction (parent);

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
  initAction(m_viewDataTable, "view-data-table", "View.Data Table", tr("&Data Table"), tr("Show or hide the data table"), true, true, true, m_menubar);
  initAction(m_viewStyleExplorer, "raster-visual", "View.Style Explorer", tr("&Style Explorer"), tr("Show or hide the style explorer"), true, true, true, m_menubar);
  initAction(m_viewFullScreen, "view-fullscreen", "View.Full Screen", tr("F&ull Screen"), tr(""), true, true, true, m_menubar);
  //initAction(m_viewRefresh, "view-refresh", "View.Refresh", tr("&Refresh"), tr(""), true, false, false, m_menubar); TODO
  //initAction(m_viewToolBars, "", "Toolbars", tr("&Toolbars"), tr(""), true, false, false);
  //initAction(m_viewGrid, "view-grid", "View.Grid", tr("&Grid"), tr("Show or hide the geographic grid"), true, true, false, m_menubar); TODO

// Menu -Tools- actions
  initAction(m_toolsCustomize, "preferences-system", "Tools.Customize", tr("&Customize..."), tr("Customize the system preferences"), true, false, true, m_menubar);
  initAction(m_toolsDataExchanger, "datasource-exchanger", "Tools.Exchanger.All to All", tr("&Advanced..."), tr("Exchange data sets between data sources"), true, false, true, m_menubar);
  initAction(m_toolsDataExchangerDirect, "data-exchange-direct-icon", "Tools.Exchanger.Direct", tr("&Layer..."), tr("Exchange data sets from layers"), true, false, true, m_menubar);
  initAction(m_toolsDataExchangerDirectPopUp, "data-exchange-direct-icon", "Tools.Exchanger.Direct", tr("&Exchange..."), tr("Exchange data sets from layers"), true, false, true, m_menubar);
  initAction(m_toolsDataSourceExplorer, "datasource-explorer", "Tools.Data Source Explorer", tr("&Data Source Explorer..."), tr("Show or hide the data source explorer"), 
    true, false, true, m_menubar);

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
  initAction(m_pluginsManager, "plugin", "Plugins.Management", tr("&Manage Plugins..."), tr("Manage the application plugins"), true, false, true, m_menubar);

// Menu -Help- actions
  initAction(m_helpContents, "help-browser", "Help.View Help", tr("&View Help..."), tr("Shows help dialog"), true, false, true, m_menubar);
  //initAction(m_helpUpdate, "system-software-update", "Help.Update", tr("&Update..."), tr(""), true, false, false, m_menubar); TODO
  initAction(m_helpAbout, "help-about-browser", "Help.About", tr("&About..."), tr(""), true, false, false, m_menubar);

// Menu -Project- actions
  initAction(m_projectAddLayerDataset, "datasource", "Project.Add Layer.All Sources", tr("&From Data Source..."), tr("Add a new layer from all available data sources"), true, false, true, m_menubar);
  initAction(m_projectAddFolderLayer, "folderlayer-new", "Project.New Folder Layer", tr("Add &Folder Layer..."), tr("Add a new folder layer"), true, false, true, m_menubar);
  initAction(m_projectAddLayerQueryDataSet, "view-filter", "Project.Add Layer.Query Dataset", tr("&Query Dataset..."), tr("Add a new layer from a queried dataset"), true, false, true, m_menubar);
  initAction(m_projectAddLayerTabularDataSet, "view-data-table", "Project.Add Layer.Tabular File", tr("&Tabular File..."), tr("Add a new layer from a Tabular file"), true, false, true, m_menubar);
  initAction(m_projectRemoveLayer, "layer-remove", "Project.Remove Layer", tr("&Remove Layer(s)"), tr("Remove layer(s) from the project"), true, false, true, this);
  initAction(m_projectRenameLayer, "layer-rename", "Project.Rename Layer", tr("Rename Layer..."), tr("Rename layer"), true, false, true, this);
  initAction(m_projectProperties, "document-info", "Project.Properties", tr("&Properties..."), tr("Show the project properties"), true, false, true, m_menubar);
  //initAction(m_projectAddLayerGraph, "", "Graph", tr("&Graph"), tr("Add a new layer from a graph"), true, false, false);

// Menu -Layer- actions
  initAction(m_layerRemoveObjectSelection, "pointer-remove-selection", "Layer.Remove Selection", tr("&Remove Selection"), tr(""), true, false, true, m_menubar);
  initAction(m_layerRemoveItem, "item-remove", "Layer.Remove Item", tr("&Remove Item"), tr(""), true, false, true, m_menubar);
  initAction(m_layerObjectGrouping, "grouping", "Layer.ObjectGrouping", tr("&Classification..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerProperties, "layer-info", "Layer.Properties", tr("&Properties..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerSRS, "layer-srs", "Layer.SRS", tr("&Inform SRS..."), tr(""), true, false, true, m_menubar);  
  initAction(m_layerShowTable, "view-data-table", "Layer.Show Table", tr("S&how Table"), tr(""), true, false, true, m_menubar);
  initAction(m_layerChartsHistogram, "chart-bar", "Layer.Charts.Histogram", tr("&Histogram..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerChartsScatter, "chart-scatter", "Layer.Charts.Scatter", tr("&Scatter..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerChart, "chart-pie", "Layer.Charts.Chart", tr("&Pie/Bar Chart..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerFitOnMapDisplay, "layer-fit", "Layer.Fit Layer on the Map Display", tr("Fit Layer"), tr("Fit the current layer on the Map Display"), true, false, true, m_menubar);
  initAction(m_layerFitSelectedOnMapDisplay, "zoom-selected-extent", "Layer.Fit Selected Objects on the Map Display", tr("Fit Selected Objects"), tr("Fit the selected objects on the Map Display"), true, false, true, m_menubar);
  initAction(m_layerPanToSelectedOnMapDisplay, "pan-selected", "Layer.Pan to Selected Objects on Map Display", tr("Pan to Selected Objects"), tr("Pan to the selected objects on the Map Display"), true, false, true, m_menubar);
  initAction(m_queryLayer, "view-filter", "Layer.Query", tr("Query..."), tr(""), true, false, true, m_menubar);

// Menu -File- actions
  initAction(m_fileNewProject, "document-new", "File.New Project", tr("&New Project..."), tr(""), true, false, true, m_menubar);
  initAction(m_fileSaveProject, "document-save", "File.Save Project", tr("&Save Project"), tr(""), true, false, true, m_menubar);
  initAction(m_fileSaveProjectAs, "document-save-as", "File.Save Project As", tr("Save Project &As..."), tr(""), true, false, false, m_menubar);
  initAction(m_fileOpenProject, "document-open", "File.Open Project", tr("&Open Project..."), tr(""), true, false, true, m_menubar);
  initAction(m_fileExit, "system-log-out", "File.Exit", tr("E&xit"), tr(""), true, false, true, m_menubar);
  initAction(m_filePrintPreview, "document-print-preview", "File.Print Preview", tr("Print Pre&view..."), tr(""), true, false, false, m_menubar);
  initAction(m_filePrint, "document-print", "File.Print", tr("&Print..."), tr(""), true, false, false, m_menubar);

// Menu -Map- actions
  initAction(m_mapSRID, "srs", "Map.SRID", tr("&SRS..."), tr("Config the Map SRS"), true, false, true, m_menubar);
  initAction(m_mapUnknownSRID, "srs-unknown", "Map.UnknownSRID", tr("&Set Unknown SRS"), tr("Set the Map SRS to unknown"), true, false, true, m_menubar);
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
  initAction(m_mapStopDrawing, "map-draw-cancel", "Map.Stop Drawing", tr("&Stop Drawing"), tr("Stop all drawing tasks"), true, false, true, m_menubar);

// Group the map tools
  QActionGroup* mapToolsGroup = new QActionGroup(this);
  mapToolsGroup->addAction(m_mapZoomIn);
  mapToolsGroup->addAction(m_mapZoomOut);
  mapToolsGroup->addAction(m_mapPan);
  mapToolsGroup->addAction(m_mapMeasureDistance);
  mapToolsGroup->addAction(m_mapMeasureArea);
  mapToolsGroup->addAction(m_mapMeasureAngle);
  mapToolsGroup->addAction(m_mapInfo);
  mapToolsGroup->addAction(m_mapSelection);
  mapToolsGroup->addAction(m_mapRemoveSelection);
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
  m_fileMenu->addAction(m_fileSaveProjectAs);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_recentProjectsMenu->menuAction());
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_filePrintPreview);
  m_fileMenu->addAction(m_filePrint);
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

  //m_viewMenu->addAction(m_viewToolBars);
  m_viewMenu->addAction(m_viewDataTable);
  m_viewMenu->addAction(m_viewLayerExplorer);
  m_viewMenu->addAction(m_viewMapDisplay);
  m_viewMenu->addAction(m_viewStyleExplorer);
  //m_viewMenu->addAction(m_viewGrid); TODO
  m_viewMenu->addSeparator();
  m_viewMenu->addAction(m_viewFullScreen);
  m_viewMenu->addSeparator();
  m_viewMenu->addMenu(m_viewToolBarsMenu);
  //m_viewMenu->addAction(m_viewRefresh); TODO

// Project menu
  m_projectMenu->setObjectName("Project");
  m_projectMenu->setTitle(tr("&Project"));

  m_projectMenu->addAction(m_projectAddLayerMenu->menuAction());
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

  // TODO
  //m_layerMenu->addAction(m_layerRaise);
  //m_layerMenu->addAction(m_layerLower);
  //m_layerMenu->addAction(m_layerToTop);
  //m_layerMenu->addAction(m_layerToBottom);

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

  ApplicationController::getInstance().registerMenu(m_mapMenu);

// Tools menu
  m_toolsMenu->setObjectName("Tools");
  m_toolsMenu->setTitle(tr("&Tools"));

//  m_toolsMenu->addAction(m_toolbarsManagement);
//  m_toolsMenu->addSeparator();
  m_toolsExchangerMenu->setObjectName("Tools.Exchanger");
  m_toolsExchangerMenu->setTitle(tr("&Data Exchanger"));
  m_toolsExchangerMenu->setIcon(QIcon::fromTheme("datasource-exchanger"));
  m_toolsExchangerMenu->addAction(m_toolsDataExchangerDirect);
  m_toolsExchangerMenu->addAction(m_toolsDataExchanger);

  m_toolsMenu->addAction(m_toolsDataSourceExplorer);
  m_toolsMenu->addSeparator();
  m_toolsMenu->addAction(m_toolsCustomize);  

// Plugins menu
  m_pluginsMenu->setObjectName("Plugins");
  m_pluginsMenu->setTitle(tr("Pl&ugins"));

  m_pluginsMenu->addAction(m_pluginsManager);

// Help menu
  m_helpMenu->setObjectName("Help");
  m_helpMenu->setTitle(tr("&Help"));

  m_helpMenu->addAction(m_helpContents);
  //m_helpMenu->addAction(m_helpUpdate); TODO

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
  //m_mapToolBar->addAction(m_mapPan);
  //m_mapToolBar->addAction(m_mapZoomExtent);
  //m_mapToolBar->addAction(m_mapPreviousExtent);
  //m_mapToolBar->addAction(m_mapNextExtent);

//  m_viewToolBarsMenu->addAction(m_mapToolBar->toggleViewAction());

// registering the toolbars
  //ApplicationController::getInstance().registerToolBar(m_fileToolBar->objectName(), m_fileToolBar);
  ////ApplicationController::getInstance().registerToolBar("EditToolBar", m_editToolBar);
  //ApplicationController::getInstance().registerToolBar(m_mapToolBar->objectName(), m_mapToolBar);
}

void te::qt::af::BaseApplication::initStatusBar()
{
  // Map SRID reset action
  QToolButton* mapUnknownSRIDToolButton = new QToolButton(m_statusbar);
  mapUnknownSRIDToolButton->setDefaultAction(m_mapUnknownSRID);
  m_statusbar->addPermanentWidget(mapUnknownSRIDToolButton);
  
  // Map SRID action
  QToolButton* mapSRIDToolButton = new QToolButton(m_statusbar);
  mapSRIDToolButton->setDefaultAction(m_mapSRID);
  m_statusbar->addPermanentWidget(mapSRIDToolButton);

  // Map SRID information
  m_mapSRIDLineEdit = new QLineEdit(m_statusbar);
  m_mapSRIDLineEdit->setFixedWidth(120);
  m_mapSRIDLineEdit->setAlignment(Qt::AlignHCenter);
  m_mapSRIDLineEdit->setEnabled(false);

  int srid = m_display->getDisplay()->getSRID();
  srid != TE_UNKNOWN_SRS ? m_mapSRIDLineEdit->setText("EPSG:" + QString::number(srid)) : m_mapSRIDLineEdit->setText(tr("Unknown SRS"));
  m_statusbar->addPermanentWidget(m_mapSRIDLineEdit);

  // Coordinate Line Edit
  m_coordinateLineEdit = new QLineEdit(m_statusbar);
  m_coordinateLineEdit->setFixedWidth(220);
  m_coordinateLineEdit->setAlignment(Qt::AlignHCenter);
  m_coordinateLineEdit->setReadOnly(true);
  m_coordinateLineEdit->setFocusPolicy(Qt::NoFocus);
  m_coordinateLineEdit->setText(tr("Coordinates"));
  m_statusbar->addPermanentWidget(m_coordinateLineEdit);

  // Stop draw action
  QToolButton* stopDrawToolButton = new QToolButton(m_statusbar);
  stopDrawToolButton->setDefaultAction(m_mapStopDrawing);
  m_statusbar->addPermanentWidget(stopDrawToolButton);
}

void te::qt::af::BaseApplication::initSlotsConnections()
{
  connect(m_fileExit, SIGNAL(triggered()), SLOT(close()));
  connect(m_projectAddLayerDataset, SIGNAL(triggered()), SLOT(onAddDataSetLayerTriggered()));
  connect(m_projectAddLayerQueryDataSet, SIGNAL(triggered()), SLOT(onAddQueryLayerTriggered()));
  connect(m_projectAddLayerTabularDataSet, SIGNAL(triggered()), SLOT(onAddTabularLayerTriggered()));
  connect(m_projectRemoveLayer, SIGNAL(triggered()), SLOT(onRemoveLayerTriggered()));
  connect(m_projectRenameLayer, SIGNAL(triggered()), SLOT(onRenameLayerTriggered()));
  connect(m_projectProperties, SIGNAL(triggered()), SLOT(onProjectPropertiesTriggered()));
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
  connect(m_helpContents, SIGNAL(triggered()), SLOT(onHelpTriggered()));
  connect(m_layerChartsHistogram, SIGNAL(triggered()), SLOT(onLayerHistogramTriggered()));
  connect(m_layerChartsScatter, SIGNAL(triggered()), SLOT(onLayerScatterTriggered()));
  connect(m_layerChart, SIGNAL(triggered()), SLOT(onLayerChartTriggered()));
  connect(m_projectAddFolderLayer, SIGNAL(triggered()), SLOT(onAddFolderLayerTriggered()));
  connect(m_layerProperties, SIGNAL(triggered()), SLOT(onLayerPropertiesTriggered()));
  connect(m_layerRemoveObjectSelection, SIGNAL(triggered()), SLOT(onLayerRemoveSelectionTriggered()));
  connect(m_layerRemoveItem, SIGNAL(triggered()), SLOT(onLayerRemoveItemTriggered()));
  connect(m_layerSRS, SIGNAL(triggered()), SLOT(onLayerSRSTriggered()));
  connect(m_layerObjectGrouping, SIGNAL(triggered()), SLOT(onLayerGroupingTriggered()));
  connect(m_mapSRID, SIGNAL(triggered()), SLOT(onMapSRIDTriggered()));
  connect(m_mapUnknownSRID, SIGNAL(triggered()), SLOT(onMapSetUnknwonSRIDTriggered()));
  connect(m_mapDraw, SIGNAL(triggered()), SLOT(onDrawTriggered()));
  connect(m_layerFitOnMapDisplay, SIGNAL(triggered()), SLOT(onLayerFitOnMapDisplayTriggered()));
  connect(m_layerFitSelectedOnMapDisplay, SIGNAL(triggered()), SLOT(onLayerFitSelectedOnMapDisplayTriggered()));
  connect(m_layerPanToSelectedOnMapDisplay, SIGNAL(triggered()), SLOT(onLayerPanToSelectedOnMapDisplayTriggered()));
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
  connect(m_mapStopDrawing, SIGNAL(triggered()), SLOT(onStopDrawTriggered()));
  connect(m_layerShowTable, SIGNAL(triggered()), SLOT(onLayerShowTableTriggered()));
  connect(m_viewFullScreen, SIGNAL(toggled(bool)), SLOT(onFullScreenToggled(bool)));
  connect(m_toolsDataSourceExplorer, SIGNAL(triggered()), SLOT(onDataSourceExplorerTriggered()));
}
