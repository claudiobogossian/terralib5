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
#include <terralib/qt/af/events/ApplicationEvents.h>
#include <terralib/qt/widgets/help/HelpManager.h>
#include <terralib/qt/widgets/layer/explorer/AbstractTreeItem.h>
#include <terralib/qt/widgets/layer/explorer/LayerExplorer.h>

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
  m_dsMenu = menuBar()->addMenu(tr("Datasources"));

  QAction* act = new QAction(QIcon::fromTheme("view-data-table"), tr("Show table..."), this);
  connect(act, SIGNAL(triggered()), SLOT(onShowTableTriggered()));

  getLayerExplorer()->add(act, "", "DATASET_LAYER_ITEM", te::qt::widgets::LayerTreeView::UNIQUE_ITEM_SELECTED);

  QMenu* hmenu = menuBar()->addMenu(tr("&Help"));

  act = hmenu->addAction(tr("&View Help..."));
  act->setObjectName("Help.View Help");
  act->setIcon(QIcon::fromTheme("help-browser"));

  connect(act, SIGNAL(triggered()), this, SLOT(onHelpTriggered()));

  act = hmenu->addAction(tr("&About..."));

  act->setObjectName("Help.About");
  act->setIcon(QIcon::fromTheme("help-about-browser"));

  connect(act, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

  act = new QAction(QIcon::fromTheme("document-save"), tr("Save Project..."), this);

  QMenu* mnu = BaseApplication::getMenuFile();
  QToolBar* bar = BaseApplication::getToolbar("m_fileToolbar");

  if(bar != 0)
    bar->insertAction(bar->actions().at(0), act);

  mnu->insertAction(mnu->actions().at(0), act);

  connect(act, SIGNAL(triggered()), SLOT(onSaveProjectTriggered()));

  QAction* loadProjectAction = new QAction(QIcon::fromTheme("document-open"), tr("Open Project..."), this);

  if(bar != 0)
    bar->insertAction(act, loadProjectAction);

  mnu->insertAction(act, loadProjectAction);

  connect(loadProjectAction, SIGNAL(triggered()), SLOT(onOpenProjectTriggered()));

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
//  te::qt::af::BaseApplication::makeDialog();

//  QMenu* hmenu = te::qt::af::ApplicationController::getInstance().getMenu("Help");

//  QAction* helpAbout = hmenu->addAction(tr("&About..."));

//  helpAbout->setObjectName("Help.About");
//  helpAbout->setIcon(QIcon::fromTheme("help-about-browser"));

//  connect(helpAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
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
        m_dsMenu->addActions(evt->m_actions);
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


