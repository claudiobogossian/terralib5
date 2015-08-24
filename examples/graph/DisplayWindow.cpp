/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file DisplayWindow.cpp

  \brief A simple main window to show example of TerraLib Graph.
*/

// Example
#include "DisplayWindow.h"
#include "GraphExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/graph/core/AbstractGraph.h>
#include <terralib/graph/core/GraphMetadata.h>
#include <terralib/graph/maptools/Layer.h>
#include <terralib/graph/maptools/LayerRenderer.h>
#include <terralib/maptools.h>
#include <terralib/raster.h>
#include <terralib/se.h>
#include <terralib/qt/widgets/canvas/MapDisplay.h>
#include <terralib/qt/widgets/tools/CoordTracking.h>
#include <terralib/qt/widgets/tools/Measure.h>
#include <terralib/qt/widgets/tools/Pan.h>
#include <terralib/qt/widgets/tools/ZoomArea.h>
#include <terralib/qt/widgets/tools/ZoomWheel.h>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QAction>
#include <QActionGroup>
#include <QStatusBar>
#include <QToolBar>

// STL
#include <cassert>

std::size_t DisplayWindow::ms_id = 0;

DisplayWindow::DisplayWindow(QWidget* parent, Qt::WindowFlags f)
  : QMainWindow(parent, f),
    m_tool(0),
    m_count(0),
    m_pngPrefix("")
{
  // Creates the tool bar
  m_toolBar = addToolBar("Tools");

  // Creates the status bar
  statusBar();

  // Setups the tool actions
  setupActions();

  // Creates the Map Display
  QSize size(600, 600);
  m_display = new te::qt::widgets::MapDisplay(size, this);
  m_display->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  setCentralWidget(m_display);

  // Install zoom wheel tool. It's always active!
  m_display->installEventFilter(new te::qt::widgets::ZoomWheel(m_display));

  // Install coordinate tracking tool. It's always active!
  te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_display);
  connect(coordTracking, SIGNAL(coordTracked(QPointF&)), SLOT(onCoordTracked(QPointF&)));
  m_display->installEventFilter(coordTracking);

  // Adjusting
  setWindowTitle(tr("TerraLib Graph Example"));
  setMinimumSize(60, 60);
  resize(size);
}

DisplayWindow::~DisplayWindow()
{
}

void DisplayWindow::setupActions()
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
  m_png = new QAction(tr("PNG"), this);
  m_png->setCheckable(false);
  connect(m_png, SIGNAL(triggered()), SLOT(onPNGTriggered()));

  // Tools group
  QActionGroup* toolsGroup = new QActionGroup(this);
  toolsGroup->addAction(m_setPan);
  toolsGroup->addAction(m_setZoomArea);

  // Add action on toolbar
  m_toolBar->addAction(m_setPan);
  m_toolBar->addAction(m_setZoomArea);
  m_toolBar->addAction(m_png);
}

void DisplayWindow::addGraph(te::graph::AbstractGraph* graph, te::gm::Envelope extent, te::se::Style* s)
{
  //creates a layer
  te::graph::Layer* layer = new te::graph::Layer(te::common::Convert2String(0), graph->getMetadata()->getName());

  layer->setGraph(graph);
  layer->setVisibility(te::map::VISIBLE);
  layer->setStyle(s);

  // creates a layer renderer
  te::graph::LayerRenderer* r = new te::graph::LayerRenderer();
  layer->setRenderer(r);

  // Adding layer to layer list
  m_layers.push_back(layer);

  // Updates MapDisplay layer list and extent
  m_display->setLayerList(m_layers);
  m_display->setSRID(layer->getSRID());
  m_display->setExtent(extent);
}

void DisplayWindow::addRasterLayer(std::string path, std::string name)
{
 // std::string datasetName = path;
 //   // set input raster name
 // std::map<std::string, std::string> rinfo;
 // rinfo["URI"] = datasetName;

 //  // open input raster
 // std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
 // ds->setConnectionInfo(rinfo);
 // ds->open();

 // std::auto_ptr<te::da::DataSetType> dt = ds->getDataSetType(name);
 // te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(dt->getProperties()[0]->clone());

 // te::da::DataSet* dataSet = tr->getDataSet(name);
 // te::rst::Raster* raster = dataSet->getRaster();
 // raster->getGrid()->setSRID(29193); //UTM SAD 69 zona 23
 // //raster->getGrid()->setSRID(4326); //lat long wgs84

 // te::gm::Envelope* extent = raster->getExtent();

 //// Creates a Layer
 // te::map::RasterLayer* layer = new te::map::RasterLayer(te::common::Convert2String(ms_id++), datasetName);
 // layer->setDataSource(ds);
 // layer->setDataSetName(name);
 // layer->setVisibility(te::map::VISIBLE);
 // layer->getRaster()->getGrid()->setSRID(29193); //UTM SAD 69 zona 23
 // layer->setSRID(29193);
 // //layer->getRaster()->getGrid()->setSRID(4326); //lat long wgs84
 // //layer->setSRID(4326);

 // // Creates a Layer Renderer
 // te::map::RasterLayerRenderer* r = new te::map::RasterLayerRenderer();
 // layer->setRenderer(r);

 // // Adding layer to layer list
 // m_layers.push_back(layer);


 // // Updates MapDisplay layer list and extent
 // m_display->setLayerList(m_layers);
 // m_display->setSRID(raster->getSRID());
 // m_display->setExtent(*extent);
}

void DisplayWindow::addVectorialLayer(std::string path, int srid)
{
  // Creates and connects data source
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = path;

  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  ds->setConnectionInfo(connInfo);
  ds->open();

  // let's include the new datasource in the managers
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  te::da::DataSourceInfoPtr dsInfo(new te::da::DataSourceInfo);
  dsInfo->setConnInfo(connInfo);
  dsInfo->setTitle("DS");
  dsInfo->setAccessDriver("OGR");
  dsInfo->setType("OGR");
  dsInfo->setDescription("");
  dsInfo->setId(id);
 
  te::da::DataSourceInfoManager::getInstance().add(dsInfo);


  std::vector<std::string> datasets = ds->getDataSetNames();

  // Gets DataSet Type
  std::string dsName = datasets[0];
  std::auto_ptr<te::da::DataSetType> dt = ds->getDataSetType(dsName);

  // Default geometry property
  std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(dsName);

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet.get());

  //te::gm::GeometryProperty* geomProperty = te::da::GetFirstGeomProperty(dt.get());

  std::auto_ptr<te::gm::Envelope> extent = dataSet->getExtent(geomPos);

  // Creates a Layer
  te::map::DataSetLayer* layer = new te::map::DataSetLayer(boost::lexical_cast<std::string>(ms_id++), dsName);
  layer->setDataSetName(dsName);
  layer->setVisibility(te::map::VISIBLE);
  layer->setSRID(srid);
  layer->setExtent(*extent.get());
  layer->setDataSourceId(id);

  // Creates a hard-coded style
  te::se::PolygonSymbolizer* symbolizer = new te::se::PolygonSymbolizer;
  symbolizer->setFill(te::se::CreateFill("#FFE4E1", "1.0"));
  symbolizer->setStroke(te::se::CreateStroke("#000000", "2", "1.0"));

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  layer->setStyle(style);
  layer->setRendererType("ABSTRACT_LAYER_RENDERER");

  // Adding layer to layer list
  m_layers.push_back(layer);

  // Updates MapDisplay layer list and extent
  m_display->setLayerList(m_layers);
  m_display->setSRID(srid);
  m_display->setExtent(*extent.get());
}

void DisplayWindow::setPNGPrefix(std::string prefix)
{
  m_pngPrefix = prefix;
}

void DisplayWindow::onPanTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::Pan(m_display, Qt::OpenHandCursor, Qt::ClosedHandCursor);
  m_display->installEventFilter(m_tool);
}

void DisplayWindow::onZoomAreaTriggered()
{
  delete m_tool;
  m_tool = new te::qt::widgets::ZoomArea(m_display, Qt::BlankCursor);
  m_display->installEventFilter(m_tool);
}

void DisplayWindow::onPNGTriggered()
{
  QString fileName = m_pngPrefix.c_str();
  QString count;
  count.setNum(m_count);
  fileName.append(count);
  fileName.append(".png");

  m_display->getDisplayPixmap()->save(fileName);

  ++m_count;
}

void DisplayWindow::onCoordTracked(QPointF& coordinate)
{
  QString text = QString::fromLatin1("Coordinates: (") + QString::number(coordinate.x()) + " , " + QString::number(coordinate.y()) + ")";
  QStatusBar* sb = statusBar();
  sb->showMessage(text);
}
