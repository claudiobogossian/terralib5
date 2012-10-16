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

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools.h>
#include <terralib/se.h>
#include <terralib/qt/widgets/canvas/MapDisplay.h>
#include <terralib/qt/widgets/tools/CoordTracking.h>
#include <terralib/qt/widgets/tools/Measure.h>
#include <terralib/qt/widgets/tools/Pan.h>
#include <terralib/qt/widgets/tools/ZoomArea.h>
#include <terralib/qt/widgets/tools/ZoomWheel.h>

// Qt
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>

// STL
#include <cassert>

std::size_t MainWindow::ms_id = 0;

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f)
  : QMainWindow(parent, f),
    m_tool(0)
{
  // Creates the tool bar
  m_toolBar = addToolBar("Tools");

  // Creates the status bar
  statusBar();

  // Setups the tool actions
  setupActions();

  // Creates the Map Display
  QSize size(512, 512);
  m_display = new te::qt::widgets::MapDisplay(size, this);
  m_display->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  setCentralWidget(m_display);

  // Install zoom wheel tool. It's always active!
  m_display->installEventFilter(new te::qt::widgets::ZoomWheel(m_display, this));

  // Install coordinate tracking tool. It's always active!
  te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_display, this);
  connect(coordTracking, SIGNAL(coordTracked(QPointF&)), SLOT(onCoordTracked(QPointF&)));
  m_display->installEventFilter(coordTracking);

  // Adjusting
  setWindowTitle(tr("TerraLib Qt Tools Example"));
  setMinimumSize(60, 60);
  resize(size);

  // Adding a hard-coded layer
  addLayer(""TE_DATA_EXAMPLE_LOCALE"/data/shp/Muni_SM_Setoresrec_pol.shp");
}

MainWindow::~MainWindow()
{
  te::common::FreeContents(m_layers);
  te::common::FreeContents(m_ds);
}

void MainWindow::setupActions()
{
  // Pan
  m_setPan = new QAction(tr("Pan"), this);
  m_setPan->setCheckable(true);
  connect(m_setPan, SIGNAL(triggered()), SLOT(onPanTriggered()));

  // Zoom Area
  m_setZoomArea = new QAction(tr("Zoom Area"), this);
  m_setZoomArea->setCheckable(true);
  connect(m_setZoomArea, SIGNAL(triggered()), SLOT(onZoomAreaTriggered()));

  // Distance
  m_setDistance = new QAction(tr("Distance"), this);
  m_setDistance->setCheckable(true);
  connect(m_setDistance, SIGNAL(triggered()), SLOT(onDistanceTriggered()));

  // Area
  m_setArea = new QAction(tr("Area"), this);
  m_setArea->setCheckable(true);
  connect(m_setArea, SIGNAL(triggered()), SLOT(onAreaTriggered()));

  // Angle
  m_setAngle = new QAction(tr("Angle"), this);
  m_setAngle->setCheckable(true);
  connect(m_setAngle, SIGNAL(triggered()), SLOT(onAngleTriggered()));

  // Tools group
  QActionGroup* toolsGroup = new QActionGroup(this);
  toolsGroup->addAction(m_setPan);
  toolsGroup->addAction(m_setZoomArea);
  toolsGroup->addAction(m_setDistance);
  toolsGroup->addAction(m_setArea);
  toolsGroup->addAction(m_setAngle);

  // Add action on toolbar
  m_toolBar->addAction(m_setPan);
  m_toolBar->addAction(m_setZoomArea);
  m_toolBar->addAction(m_setDistance);
  m_toolBar->addAction(m_setArea);
  m_toolBar->addAction(m_setAngle);
}

void MainWindow::addLayer(const QString& path)
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

  // Gets DataSet Type
  te::da::DataSetType* dt = cl->getDataSetType(datasets[0]);

  // Default geometry property
  te::gm::GeometryProperty* geomProperty = dt->getDefaultGeomProperty();

  // Creates a Layer
  te::map::Layer* layer = new te::map::Layer(te::common::Convert2String(static_cast<unsigned int>(ms_id++)), datasets[0]);
  layer->setDataSource(ds);
  layer->setDataSetName(datasets[0]);
  layer->setVisibility(te::map::VISIBLE);

  // Creates a hard-coded style
  te::se::PolygonSymbolizer* symbolizer = new te::se::PolygonSymbolizer;
  symbolizer->setFill(te::se::CreateFill("#339966", "1.0"));
  symbolizer->setStroke(te::se::CreateStroke("#000000", "2", "1.0"));

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
  // Storing the data source
  m_ds.push_back(ds);

  // Calculates new extent to MapDisplay
  te::gm::Envelope* extent = cl->getExtent(geomProperty);

  // No more necessary
  delete cl;
  delete transactor;

  // Updates MapDisplay layer list and extent
  m_display->setLayerList(m_layers);
  m_display->setExtent(*extent);
}

void MainWindow::onPanTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Pan(m_display, this);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onZoomAreaTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::ZoomArea(m_display, this);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onDistanceTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Measure(m_display, te::qt::widgets::Measure::Distance, this);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onAreaTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Measure(m_display, te::qt::widgets::Measure::Area, this);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onAngleTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Measure(m_display, te::qt::widgets::Measure::Angle, this);
  m_display->installEventFilter(m_tool);
}

void MainWindow::onCoordTracked(QPointF& coordinate)
{
  QString text = QString::fromLatin1("Coordinates: (") + QString::number(coordinate.x()) + " , " + QString::number(coordinate.y()) + ")";
  QStatusBar* sb = statusBar();
  sb->showMessage(text);
}
