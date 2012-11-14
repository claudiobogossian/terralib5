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
#include <terralib/common.h>
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools.h>
#include <terralib/se.h>
#include <terralib/raster/Raster.h>
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
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDockWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QMenu>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>

// STL
#include <cassert>

#include <boost/lexical_cast.hpp>

te::se::Symbolizer* BuildSymbolizer(const te::gm::GeomType& geomType)
{
  switch(geomType)
  {
    case te::gm::PolygonType:
    {
      QColor color(rand() % 255, rand() % 255, rand() % 255);
      te::se::PolygonSymbolizer* symbolizer = new te::se::PolygonSymbolizer;
      symbolizer->setFill(te::se::CreateFill(color.name().toStdString(), "1.0"));
      symbolizer->setStroke(te::se::CreateStroke("#000000", "1", "1.0"));
      return symbolizer;
    }

    case te::gm::LineStringType:
    {
      QColor color(rand() % 255, rand() % 255, rand() % 255);
      te::se::LineSymbolizer* symbolizer = new te::se::LineSymbolizer;
      symbolizer->setStroke(te::se::CreateStroke(color.name().toStdString(), "1", "1.0"));
      return symbolizer;
    }

    case te::gm::PointType:
    {
      QColor color(rand() % 255, rand() % 255, rand() % 255);
      te::se::Fill* markFill = te::se::CreateFill(color.name().toStdString(), "1.0");
      te::se::Stroke* markStroke = te::se::CreateStroke("#000000", "1");
      te::se::Mark* mark = te::se::CreateMark("circle", markStroke, markFill);
      te::se::Graphic* graphic = te::se::CreateGraphic(mark, "12", "", "");
      te::se::PointSymbolizer* symbolizer = te::se::CreatePointSymbolizer(graphic);
      return symbolizer;
    }
  }

  return 0;
}

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
  te::common::FreeContents(m_layers);
  te::common::FreeContents(m_ds);
}

void MainWindow::setupActions()
{
  // Add Vector Layer
  QAction* addVectorLayer = new QAction(QIcon::fromTheme("list-add"), tr("Add Vector Layer..."), this);
  connect(addVectorLayer, SIGNAL(triggered()), SLOT(onAddVectorLayerTriggered()));
  m_toolBar->addAction(addVectorLayer);

  // Add Raster Layer
  /*QAction* addRasterLayer = new QAction(QIcon::fromTheme("list-add"), tr("Add Raster Layer..."), this);
  connect(addRasterLayer, SIGNAL(triggered()), SLOT(onAddRasterLayerTriggered()));
  m_toolBar->addAction(addRasterLayer);*/

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
}

void MainWindow::addVectorLayer(const QString& path)
{
  // Creates and connects data source
  std::map<std::string, std::string> connInfo;
  connInfo["path"] = path.toStdString();
  te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
  ds->open(connInfo);

  // Transactor and catalog loader
  te::da::DataSourceTransactor* transactor = ds->getTransactor();
  te::da::DataSourceCatalogLoader* cl = transactor->getCatalogLoader();
  cl->loadCatalog();

  // Get the number of data set types that belongs to the data source
  boost::ptr_vector<std::string> datasets;
  transactor->getCatalogLoader()->getDataSets(datasets);
  assert(!datasets.empty());

  // MapDisplay extent
  te::gm::Envelope env;

  for(std::size_t i = 0; i < datasets.size(); ++i)
  {
    // Gets DataSet Type
    te::da::DataSetType* dt = cl->getDataSetType(datasets[i]);

    // Updates MapDisplay extent
    te::gm::Envelope* e = cl->getExtent(dt->getDefaultGeomProperty());
    env.Union(*e);
    delete e;

    // Creates a Layer
    te::map::Layer* layer = new te::map::Layer(te::common::Convert2String(static_cast<unsigned int>(ms_id++)), datasets[i]);
    layer->setDataSource(ds);
    layer->setDataSetName(datasets[i]);
    layer->setVisibility(te::map::VISIBLE);

    // Creates a hard-coded style
    te::se::Symbolizer* symbolizer = BuildSymbolizer(dt->getDefaultGeomProperty()->getGeometryType());

    te::se::Rule* rule = new te::se::Rule;
    rule->push_back(symbolizer);

    te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
    style->push_back(rule);

    layer->setStyle(style);

    // Creates a Layer Renderer
    te::map::LayerRenderer* r = new te::map::LayerRenderer();
    layer->setRenderer(r);

    // Adding layer to layer list
    m_layers.push_back(layer);
  }

  // Storing the data source
  m_ds.push_back(ds);

  // No more necessary
  delete cl;
  delete transactor;

  // Updates MapDisplay layer list and extent
  m_display->setLayerList(m_layers);
  m_display->setExtent(env);
}

void MainWindow::addRasterLayer(const QString& path)
{
  // Creates and connects data source
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = path.toStdString();
  te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
  ds->open(connInfo);

  // Transactor and catalog loader
  te::da::DataSourceTransactor* transactor = ds->getTransactor();
  te::da::DataSourceCatalogLoader* cl = transactor->getCatalogLoader();
  cl->loadCatalog();

  // Get the number of data set types that belongs to the data source
  boost::ptr_vector<std::string> datasets;
  transactor->getCatalogLoader()->getDataSets(datasets);
  assert(!datasets.empty());

  // Gets DataSet, raster and extent
  te::da::DataSet* dataSet = transactor->getDataSet(datasets[0]);
  te::rst::Raster* raster = dataSet->getRaster();
  te::gm::Envelope env(*raster->getExtent());

 // Creates a Raster Layer
  te::map::RasterLayer* rasterLayer = new te::map::RasterLayer(boost::lexical_cast<std::string>(ms_id++), datasets[0]);
  rasterLayer->setDataSource(ds);
  rasterLayer->setDataSetName(datasets[0]);
  rasterLayer->setVisibility(te::map::VISIBLE);

  // Creates a Raster Layer Renderer
  te::map::RasterLayerRenderer* r = new te::map::RasterLayerRenderer();
  rasterLayer->setRenderer(r);

  // Adding layer to layer list
  m_layers.push_back(rasterLayer);

  // Storing the data source
  m_ds.push_back(ds);

   m_display->setSRID(raster->getSRID());

  // No more necessary
  delete dataSet;
  delete raster;
  delete cl;
  delete transactor;

  // Updates MapDisplay layer list and extent
  m_display->setLayerList(m_layers);
  m_display->setExtent(env);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* e)
{
  m_menu->popup(e->globalPos());
}

void MainWindow::onAddVectorLayerTriggered()
{
  QString path = QFileDialog::getOpenFileName(this, tr("Select a vector file..."), ""TE_DATA_EXAMPLE_LOCALE"/data/shp/");
  if(!path.isNull())
    addVectorLayer(path);
}

void MainWindow::onAddRasterLayerTriggered()
{
  QString path = QFileDialog::getOpenFileName(this, tr("Select a raster file..."), ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/");
  if(!path.isNull())
    addRasterLayer(path);
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
  m_tool = new te::qt::widgets::ZoomClick(m_display, 2.0, te::qt::widgets::Zoom::In);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onZoomOutTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::ZoomClick(m_display, 2.0, te::qt::widgets::Zoom::Out);
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
  m_tool = new te::qt::widgets::Measure(m_display, te::qt::widgets::Measure::Distance);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onAreaTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Measure(m_display, te::qt::widgets::Measure::Area);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onAngleTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Measure(m_display, te::qt::widgets::Measure::Angle);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onSelectionTriggered()
{
  delete m_tool;
  m_tool = new SelectionTool(m_display, dynamic_cast<te::map::Layer*>(*m_layers.begin()));
  m_display->installEventFilter(m_tool);
}

void MainWindow::onCoordTracked(QPointF& coordinate)
{
  QString text = QString::fromLatin1("Coordinates: (") + QString::number(coordinate.x()) + " , " + QString::number(coordinate.y()) + ")";
  QStatusBar* sb = statusBar();
  sb->showMessage(text);
}
