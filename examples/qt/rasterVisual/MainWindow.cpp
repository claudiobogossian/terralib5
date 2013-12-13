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
#include "ReadPixelTool.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools.h>
#include <terralib/raster.h>
#include <terralib/se.h>
#include <terralib/qt/widgets/canvas/MapDisplay.h>
#include <terralib/qt/widgets/tools/CoordTracking.h>
#include <terralib/qt/widgets/tools/Measure.h>
#include <terralib/qt/widgets/tools/Pan.h>
#include <terralib/qt/widgets/tools/ZoomArea.h>
#include <terralib/qt/widgets/tools/ZoomClick.h>
#include <terralib/qt/widgets/tools/ZoomKeyboard.h>
#include <terralib/qt/widgets/tools/ZoomWheel.h>
#include <terralib/qt/widgets/se/RasterSymbolizerDialog.h>
#include <terralib/qt/widgets/se/RasterSymbolizerWidget.h>
#include <terralib/qt/widgets/utils/FileChooser.h>

// Qt
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMenu>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QDockWidget>
#include <QtGui/QLayout> 

// STL
#include <cassert>

#include <boost/lexical_cast.hpp>

std::size_t MainWindow::ms_id = 0;

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f)
  : QMainWindow(parent, f),
    m_tool(0),
    m_startTools(false),
    m_rvW(0)
{
  // Creates the tool bar
  m_toolBar = addToolBar("Tools");

  // Creates the menu
  m_menu = new QMenu(this);

  // Creates the status bar
  statusBar();

  // Setup dock window
  setupDockWindow();

  // Add File Chooser Interface
  te::qt::widgets::FileChooser* fc = new te::qt::widgets::FileChooser(this);
  fc->setFilterPattern("*.tif");
  statusBar()->addPermanentWidget(fc);

  connect(fc, SIGNAL(resourceSelected(QString)), this, SLOT(onFileSelected(QString)));


  // Setups the tool actions
  setupActions();

  checkToolsStatus();

  // Creates the Map Display
  QSize size(512, 512);
  m_display = new te::qt::widgets::MapDisplay(size, this);
  m_display->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  setCentralWidget(m_display);

  //progress
  m_dlgViewer = new te::qt::widgets::ProgressViewerDialog(this);
  m_dlgViewerId = te::common::ProgressManager::getInstance().addViewer(m_dlgViewer);

  // Adjusting
  setWindowTitle(tr("TerraLib Qt Raster Visual Example"));
  setMinimumSize(400, 400);
  resize(size);
}

MainWindow::~MainWindow()
{
  te::common::ProgressManager::getInstance().removeViewer(m_dlgViewerId);
  delete m_dlgViewer;

  te::common::FreeContents(m_layers);
  te::common::FreeContents(m_ds);
}

void MainWindow::setupActions()
{
  // List of created actions

  // Pan
  QAction* setPan = new QAction(QIcon::fromTheme("pan"), tr("Pan"), this);
  setPan->setCheckable(true);
  connect(setPan, SIGNAL(triggered()), SLOT(onPanTriggered()));
  m_actions << setPan;

  // Zoom In
  QAction* setZoomIn = new QAction(QIcon::fromTheme("zoom-in"), tr("Zoom In"), this);
  setZoomIn->setCheckable(true);
  connect(setZoomIn, SIGNAL(triggered()), SLOT(onZoomInTriggered()));
  m_actions << setZoomIn;

  // Zoom In
  QAction* setZoomOut = new QAction(QIcon::fromTheme("zoom-out"), tr("Zoom Out"), this);
  setZoomOut->setCheckable(true);
  connect(setZoomOut, SIGNAL(triggered()), SLOT(onZoomOutTriggered()));
  m_actions << setZoomOut;

  // Zoom Area
  QAction* setZoomArea = new QAction(QIcon::fromTheme("zoom-area"), tr("Zoom Area"), this);
  setZoomArea->setCheckable(true);
  connect(setZoomArea, SIGNAL(triggered()), SLOT(onZoomAreaTriggered()));
  m_actions << setZoomArea;

  // Zoom Area
  QAction* setReadPixel = new QAction(QIcon::fromTheme("readPixel"), tr("Read Pixel"), this);
  setReadPixel->setCheckable(true);
  connect(setReadPixel, SIGNAL(triggered()), SLOT(onReadPixelTriggered()));
  m_actions << setReadPixel;

  // Raster Style
  QAction* rasterStyle = new QAction(QIcon::fromTheme("raster-visual"), tr("Raster Visual"), this);
  rasterStyle->setCheckable(false);
  connect(rasterStyle, SIGNAL(triggered()), SLOT(onRasterStyleTriggered()));
  m_actions << rasterStyle;

  // Tools group
  QActionGroup* toolsGroup = new QActionGroup(this);
  
  // Add actions on group and toolbar
  QList<QAction*>::iterator it;
  for(it = m_actions.begin(); it != m_actions.end(); ++it)
  {
    toolsGroup->addAction(*it);
    m_toolBar->addAction(*it);
    m_menu->addAction(*it);
  }
}

void MainWindow::setupDockWindow()
{
  if(m_rvW == 0)
  {
    //create dock
    QDockWidget* dw = new QDockWidget("Raster Visual", this);

    dw->layout()->setAlignment(Qt::AlignTop);

    m_rvW = new te::qt::widgets::RasterSymbolizerWidget(dw);

    //m_rvW->setVerticalLayout();

    dw->setWidget(m_rvW);

    m_rvW->show();

    connect(m_rvW, SIGNAL(symbolizerChanged()), this, SLOT(onSymbolizerUpdated()));

    addDockWidget(Qt::RightDockWidgetArea, dw);
  }

  checkToolsStatus();
}


void MainWindow::addRasterLayer(const QString& path)
{
 // * Under revision *
 // std::string datasetName = path.toStdString();
 // 
 // // set input raster name
 // std::map<std::string, std::string> rinfo;
 // rinfo["URI"] = datasetName;
 //   
 //  // open input raster
 // te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
 // ds->open(rinfo);

 // te::da::DataSourceTransactor* tr = ds->getTransactor();
 // te::da::DataSourceCatalogLoader* cl = tr->getCatalogLoader();

 // cl->loadCatalog();

 // // Gets the number of data set types that belongs to the data source
 // boost::ptr_vector<std::string> datasets;
 // tr->getCatalogLoader()->getDataSets(datasets);
 // assert(!datasets.empty());

 // // Gets the first dataset
 // std::string dataSetName(datasets[0]);
 // te::da::DataSet* dataSet = tr->getDataSet(dataSetName);
 // te::rst::Raster* raster = dataSet->getRaster();

 // te::gm::Envelope* extent = raster->getExtent();

 //// Creates a Layer
 // te::map::RasterLayer* rasterLayer = new te::map::RasterLayer(boost::lexical_cast<std::string>(ms_id++), datasetName);
 // rasterLayer->setDataSource(ds);
 // rasterLayer->setDataSetName(dataSetName);
 // rasterLayer->setVisibility(te::map::VISIBLE);

 // // Creates a Layer Renderer
 // te::map::RasterLayerRenderer* r = new te::map::RasterLayerRenderer();
 // rasterLayer->setRenderer(r);

 // // Adding layer to layer list
 // m_layers.push_back(rasterLayer);

 // // Storing the data source
 // m_ds.push_back(ds);

 // // No more necessary
 // delete cl;
 // delete tr;

 // // Updates MapDisplay layer list and extent
 // m_display->setLayerList(m_layers);
 // m_display->setSRID(raster->getSRID());
 // m_display->setExtent(*extent);


 // if(m_rvW)
 // {
 //  //get raster properties
 //   te::map::RasterLayer* layer = dynamic_cast<te::map::RasterLayer*>(*m_layers.begin());

 //   if(layer->getRasterSymbolizer())
 //   {
 //     te::rst::RasterProperty* prop = 0;
 // 
 //     prop = (te::rst::RasterProperty*)layer->getRasterProperty()->clone();

 //     te::se::RasterSymbolizer* rs = (te::se::RasterSymbolizer*)layer->getRasterSymbolizer()->clone();

 //     disconnect(m_rvW, SIGNAL(symbolizerChanged()), this, SLOT(onSymbolizerUpdated()));

 //     m_rvW->setBandProperty(prop->getBandProperties());

 //     m_rvW->setRasterSymbolizer(rs);

 //     connect(m_rvW, SIGNAL(symbolizerChanged()), this, SLOT(onSymbolizerUpdated()));

 //     delete rs;

 //     delete prop;
 //   }
 // }
}

void MainWindow::contextMenuEvent(QContextMenuEvent* e)
{
  m_menu->popup(e->globalPos());
}

void MainWindow::updateDisplay()
{
  m_display->refresh();
}

void MainWindow::startTools()
{
  if(m_startTools == false)
  {
    // Install zoom wheel tool. It's always active!
    m_display->installEventFilter(new te::qt::widgets::ZoomWheel(m_display, 2.0, m_display));

    // Install zoom keyboard tool. It's always active!
    m_display->installEventFilter(new te::qt::widgets::ZoomKeyboard(m_display, 2.0, m_display));

    // Install coordinate tracking tool. It's always active!
    te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_display, m_display);
    connect(coordTracking, SIGNAL(coordTracked(QPointF&)), SLOT(onCoordTracked(QPointF&)));
    m_display->installEventFilter(coordTracking);

    m_startTools = true;
  }
}

void MainWindow::checkToolsStatus()
{
  bool status = false;
  
  if(m_layers.empty() == false)
  {
    status = true;
  }

  QList<QAction*>::iterator it;
  for(it = m_actions.begin(); it != m_actions.end(); ++it)
  {
    (*it)->setEnabled(status);
  }

  if(m_rvW)
  {
    m_rvW->setEnabled(status);
  }
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
  m_tool = new te::qt::widgets::ZoomClick(m_display, Qt::BlankCursor, 2.0, te::qt::widgets::Zoom::In);
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

void MainWindow::onCoordTracked(QPointF& coordinate)
{
  QString text = QString::fromLatin1("Coordinates: (") + QString::number(coordinate.x()) + " , " + QString::number(coordinate.y()) + ")";
  QStatusBar* sb = statusBar();
  sb->showMessage(text);
}

void MainWindow::onReadPixelTriggered()
{
  // * Under revision *
  /*delete m_tool;
  m_tool = new ReadPixelTool(m_display, dynamic_cast<te::map::RasterLayer*>(*m_layers.begin()));
  m_display->installEventFilter(m_tool);*/
}

void MainWindow::onRasterStyleTriggered()
{
  // * Under revision *
  // Creates the rastersymbolizer dialog
  //te::qt::widgets::RasterSymbolizerDialog dlg;

  //te::map::RasterLayer* layer = dynamic_cast<te::map::RasterLayer*>(*m_layers.begin());

  //te::rst::RasterProperty* prop = 0;
  //
  //prop = (te::rst::RasterProperty*)layer->getRasterProperty()->clone();

  //dlg.setRasterProperty(layer->getRaster(), prop);

  //dlg.setRasterSymbolizer(layer->getRasterSymbolizer());

  //if(dlg.exec() == QDialog::Accepted)
  //{
  //  te::se::RasterSymbolizer* rs = static_cast<te::se::RasterSymbolizer*>(dlg.getRasterSymbolizer());

  //  //add symbolizer to a layer style
  //  te::se::Rule* rule = new te::se::Rule();
  //  rule->push_back(rs);

  //  te::se::Style* s = new te::se::CoverageStyle();
  //  s->push_back(rule);

  //  layer->setStyle(s);

  //  updateDisplay();
  //}

  //delete prop;
}

void MainWindow::onFileSelected(QString s)
{
  te::common::FreeContents(m_layers);
  m_layers.clear();

  te::common::FreeContents(m_ds);
  m_ds.clear();

  m_display->setExtent(te::gm::Envelope());

  addRasterLayer(s);

  startTools();

  checkToolsStatus();
}

void MainWindow::onSymbolizerUpdated()
{
  //add symbolizer to a layer style
  te::se::Rule* rule = new te::se::Rule();
  rule->push_back(m_rvW->getRasterSymbolizer());

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(rule);

  // * Under revision *
  //te::map::RasterLayer* layer = dynamic_cast<te::map::RasterLayer*>(*m_layers.begin());
  //layer->setStyle(s);

  updateDisplay();
}
