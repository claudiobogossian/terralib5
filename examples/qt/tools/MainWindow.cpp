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
  \file \file MainWindow.cpp

  \brief A simple main window to show example of TerraLib Qt Tools.
*/

// Example
#include "MainWindow.h"
#include "SelectionTool.h"

// TerraLib
#include "../../Config.h"
#include <terralib/common.h>
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/common/progress/TaskProgress.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools.h>
#include <terralib/se.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterProperty.h>
#include <terralib/qt/widgets/canvas/MultiThreadMapDisplay.h>
#include <terralib/qt/widgets/progress/ProgressViewerBar.h>
#include <terralib/qt/widgets/progress/ProgressViewerWidget.h>
#include <terralib/qt/widgets/tools/CoordTracking.h>
#include <terralib/qt/widgets/tools/Measure.h>
#include <terralib/qt/widgets/tools/Pan.h>
#include <terralib/qt/widgets/tools/ZoomArea.h>
#include <terralib/qt/widgets/tools/ZoomClick.h>
#include <terralib/qt/widgets/tools/ZoomKeyboard.h>
#include <terralib/qt/widgets/tools/ZoomWheel.h>

// Qt
#include <QAction>
#include <QActionGroup>
#include <QContextMenuEvent>
#include <QDockWidget>
#include <QFileDialog>
#include <QMenu>
#include <QStatusBar>
#include <QToolBar>

// Boost
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>
#include <memory>

std::size_t MainWindow::ms_id = 0;

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f)
  : QMainWindow(parent, f),
    m_tool(0)
{
  // Creates the tool bar
  m_toolBar = addToolBar("Tools");

  // Creates the menu
  m_menu = new QMenu(this);

  // Setups the tool actions
  setupActions();

  // Creates the Map Display
  QSize size(800, 600);
  m_display = new te::qt::widgets::MultiThreadMapDisplay(size, this);
  m_display->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  setCentralWidget(m_display);

  // Install zoom wheel tool. It's always active!
  m_display->installEventFilter(new te::qt::widgets::ZoomWheel(m_display, 2.0, m_display));

  // Install zoom keyboard tool. It's always active!
  m_display->installEventFilter(new te::qt::widgets::ZoomKeyboard(m_display, 2.0, m_display));

  // Install coordinate tracking tool. It's always active!
  te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_display, m_display);
  connect(coordTracking, SIGNAL(coordTracked(QPointF&)), SLOT(onCoordTracked(QPointF&)));
  m_display->installEventFilter(coordTracking);

  // Creates the status progress bar
  te::qt::widgets::ProgressViewerBar* pb = new te::qt::widgets::ProgressViewerBar(this);
  te::common::ProgressManager::getInstance().addViewer(pb);
  statusBar()->addPermanentWidget(pb);

  // Creates the multi task progress bar
  te::qt::widgets::ProgressViewerWidget* pbw = new te::qt::widgets::ProgressViewerWidget(this);
  te::common::ProgressManager::getInstance().addViewer(pbw);

  QDockWidget* docWidget = new QDockWidget(this);
  docWidget->setWidget(pbw);
  docWidget->setFixedWidth(250);
  docWidget->setAllowedAreas(Qt::RightDockWidgetArea);
  docWidget->setWindowTitle(tr("Threads Progress"));
  addDockWidget(Qt::RightDockWidgetArea, docWidget);

  // Adjusting
  setWindowTitle(tr("TerraLib Qt Tools Example"));
  setMinimumSize(60, 60);
  resize(size);
}

MainWindow::~MainWindow()
{
  m_layers.clear();
}

void MainWindow::setupActions()
{
  // Add Vector Data
  QAction* addVectorData = new QAction(QIcon::fromTheme("list-add"), tr("Add Vector Data..."), this);
  connect(addVectorData, SIGNAL(triggered()), SLOT(onAddVectorDataTriggered()));
  m_toolBar->addAction(addVectorData);

  // Add Raster Data
  QAction* addRasterData = new QAction(QIcon::fromTheme("list-add"), tr("Add Raster Data..."), this);
  connect(addRasterData, SIGNAL(triggered()), SLOT(onAddRasterDataTriggered()));
  m_toolBar->addAction(addRasterData);

  m_toolBar->addSeparator();

  // List of created actions
  QList<QAction*> actions;

  // Pan
  QAction* setPan = new QAction(QIcon::fromTheme("pan"), tr("Pan"), this);
  setPan->setCheckable(true);
  connect(setPan, SIGNAL(triggered()), SLOT(onPanTriggered()));
  actions << setPan;

  // Zoom In
  QAction* setZoomIn = new QAction(QIcon::fromTheme("zoom-in"), tr("Zoom In"), this);
  setZoomIn->setCheckable(true);
  connect(setZoomIn, SIGNAL(triggered()), SLOT(onZoomInTriggered()));
  actions << setZoomIn;

  // Zoom In
  QAction* setZoomOut = new QAction(QIcon::fromTheme("zoom-out"), tr("Zoom Out"), this);
  setZoomOut->setCheckable(true);
  connect(setZoomOut, SIGNAL(triggered()), SLOT(onZoomOutTriggered()));
  actions << setZoomOut;

  // Zoom Area
  QAction* setZoomArea = new QAction(QIcon::fromTheme("zoom-area"), tr("Zoom Area"), this);
  setZoomArea->setCheckable(true);
  connect(setZoomArea, SIGNAL(triggered()), SLOT(onZoomAreaTriggered()));
  actions << setZoomArea;

  // Distance
  QAction* setDistance = new QAction(QIcon::fromTheme("distance-measure"), tr("Measure Distance"), this);
  setDistance->setCheckable(true);
  connect(setDistance, SIGNAL(triggered()), SLOT(onDistanceTriggered()));
  actions << setDistance;

  // Area
  QAction* setArea = new QAction(QIcon::fromTheme("area-measure"), tr("Measure Area"), this);
  setArea->setCheckable(true);
  connect(setArea, SIGNAL(triggered()), SLOT(onAreaTriggered()));
  actions << setArea;

  // Angle
  QAction* setAngle = new QAction(QIcon::fromTheme("angle-measure"), tr("Measure Angle"), this);
  setAngle->setCheckable(true);
  connect(setAngle, SIGNAL(triggered()), SLOT(onAngleTriggered()));
  actions << setAngle;

  // Selection
  QAction* setSelection = new QAction(QIcon::fromTheme("pointer"), tr("Selection"), this);
  setSelection->setCheckable(true);
  connect(setSelection, SIGNAL(triggered()), SLOT(onSelectionTriggered()));
  actions << setSelection;

  // Tools group
  QActionGroup* toolsGroup = new QActionGroup(this);
  
  // Add actions on group and toolbar
  QList<QAction*>::iterator it;
  for(it = actions.begin(); it != actions.end(); ++it)
  {
    toolsGroup->addAction(*it);
    m_toolBar->addAction(*it);
    m_menu->addAction(*it);
  }

  m_toolBar->addSeparator();

  // Stop All
  QAction* stopAll = new QAction(QIcon::fromTheme("process-stop"), tr("Stop All"), this);
  connect(stopAll, SIGNAL(triggered()), SLOT(onStopAllTriggered()));
  m_toolBar->addAction(stopAll);
}

void MainWindow::addDataSetLayer(const QString& path, const std::string& driver)
{
  // Creates and connects data source
  std::map<std::string, std::string> connInfo;
  driver == "OGR" ? connInfo["path"] = path.toStdString() : connInfo["URI"] = path.toStdString();

  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().open(boost::lexical_cast<std::string>(ms_id++), driver, connInfo);

  // Transactor and catalog loader
  std::auto_ptr<te::da::DataSourceTransactor> transactor(ds->getTransactor());

  // Get the number of data set types that belongs to the data source
  std::vector<std::string> datasets = transactor->getDataSetNames();
  assert(!datasets.empty());

  // MapDisplay extent
  te::gm::Envelope env;

  int srid = 0;

  for(std::size_t i = 0; i < datasets.size(); ++i)
  {
    // Gets DataSet Type
    //std::auto_ptr<te::da::DataSetType> dt(cl->getDataSetType(datasets[i]));
    std::auto_ptr<te::da::DataSetType> dt = transactor->getDataSetType(datasets[i]);
    te::dt::Property* p = dt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);
    std::string propname = p->getName();

    // Creates a DataSetLayer
    te::map::DataSetLayer* layer = new te::map::DataSetLayer(te::common::Convert2String(static_cast<unsigned int>(ms_id++)), datasets[i]);
    layer->setDataSourceId(ds->getId());
    layer->setDataSetName(datasets[i]);
    layer->setRendererType("DATASET_LAYER_RENDERER");
    layer->setVisibility(te::map::VISIBLE);

    // Gets the layer extent
    te::gm::Envelope* e = 0;
    if(driver == "OGR")
      e = (transactor->getExtent(datasets[i], propname)).get(); // getExtent(dt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE)); 
    else
    {
      std::auto_ptr<te::rst::Raster> raster(te::map::GetRaster(layer));
      e = raster->getExtent();
      layer->setSRID(raster->getSRID());
      srid = raster->getSRID();
    }
    
    // Updates MapDisplay Extent
    env.Union(*e);

    // Sets the layer extent
    layer->setExtent(*e);

    //delete e;

    // Adding layer to layer list
    m_layers.push_back(layer);
  }

  // Updates MapDisplay layer list, srid and extent
  m_display->setLayerList(m_layers);
  m_display->setSRID(srid, false);
  m_display->setExtent(env, true);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* e)
{
  m_menu->popup(e->globalPos());
}

void MainWindow::onAddVectorDataTriggered()
{
  QString path = QFileDialog::getOpenFileName(this, tr("Select a vector file..."), ""TERRALIB_DATA_DIR"/shp/", tr("ShapeFile *.shp"));
  if(!path.isNull())
    addDataSetLayer(path, "OGR");
}

void MainWindow::onAddRasterDataTriggered()
{
  QString path = QFileDialog::getOpenFileName(this, tr("Select a raster file..."), ""TERRALIB_DATA_DIR"/rasters/", tr("TIFF *.tif"));
  if(!path.isNull())
    addDataSetLayer(path, "GDAL");
}

void MainWindow::onPanTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Pan(m_display, Qt::OpenHandCursor, Qt::ClosedHandCursor);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onZoomInTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::ZoomClick( m_display, Qt::BlankCursor, 2.0, te::qt::widgets::Zoom::In);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onZoomOutTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::ZoomClick(m_display, Qt::BlankCursor, 2.0, te::qt::widgets::Zoom::Out);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onZoomAreaTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::ZoomArea(m_display, Qt::BlankCursor);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onDistanceTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Measure(m_display, te::qt::widgets::Measure::Distance, Qt::BlankCursor);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onAreaTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Measure(m_display, te::qt::widgets::Measure::Area, Qt::BlankCursor);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onAngleTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Measure(m_display, te::qt::widgets::Measure::Angle, Qt::BlankCursor);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onSelectionTriggered()
{
  delete m_tool;
  m_tool = new SelectionTool(m_display, dynamic_cast<te::map::DataSetLayer*>(m_layers.begin()->get()));
  m_display->installEventFilter(m_tool);
}

void MainWindow::onStopAllTriggered()
{
  te::common::ProgressManager::getInstance().cancelTasks(te::common::TaskProgress::DRAW);
}

void MainWindow::onCoordTracked(QPointF& coordinate)
{
  QString text = QString::fromLatin1("Coordinates: (") + QString::number(coordinate.x()) + " , " + QString::number(coordinate.y()) + ")";
  QStatusBar* sb = statusBar();
  sb->showMessage(text);
}
