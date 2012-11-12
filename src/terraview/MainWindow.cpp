#include "MainWindow.h"
#include <ui_MainWindow.h>
#include "NewOGRLayer.h"
#include "NewGDALLayer.h"

//! TerraLib include files
#include <terralib/qt/af/events/NewToolBar.h>
#include <terralib/qt/af/events/LayerAdded.h>
#include <terralib/qt/af/events/LayerSelected.h>
#include <terralib/qt/af/events/TrackedCoordinate.h>
#include <terralib/qt/af/CoreApplication.h>
#include <terralib/qt/af/connectors/LayerExplorer.h>
#include <terralib/qt/af/connectors/MapDisplay.h>
#include <terralib/qt/af/connectors/TabularViewer.h>
#include <terralib/qt/widgets/layer/AbstractTreeItem.h>
#include <terralib/maptools/Layer.h>
#include <terralib/maptools/RasterLayer.h>
#include <terralib/srs/Config.h>

#include <terralib/qt/widgets/layer/LayerExplorer.h>
#include <terralib/maptools/FolderLayer.h>
#include <terralib/qt/widgets/layer/LayerExplorerModel.h>
#include <terralib/qt/widgets/canvas/MultiThreadMapDisplay.h>
#include <terralib/qt/widgets/dataview/TabularViewer.h>
#include <terralib/qt/widgets/tools/Pan.h>
#include <terralib/qt/widgets/tools/ZoomArea.h>
#include <terralib/qt/widgets/tools/Measure.h>

#include <terralib/common/progress/TaskProgress.h>
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/qt/widgets/progress/ProgressViewerDialog.h>
#include <terralib/qt/widgets/progress/ProgressViewerWidget.h>
#include <terralib/qt/widgets/progress/ProgressViewerBar.h>

#include <terralib/qt/widgets/se/RasterVisualDockWidget.h>


//! Qt include files
#include <QDockWidget>
#include <QActionGroup>

MainWindow::MainWindow(QWidget* parent) :
QMainWindow(parent, 0),
  m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  makeDialog();

  QMainWindow::setWindowIcon(QIcon::fromTheme("terralib_logo"));

  setMinimumSize(60, 60);
  resize(QSize(512, 512));
}

MainWindow::~MainWindow()
{
  delete m_display;
}

void MainWindow::onApplicationTriggered(te::qt::af::Event* evt)
{
  switch(evt->getId())
  {
    case te::qt::af::evt::NEW_TOOLBAR :
    break;

    case te::qt::af::evt::TRACK_COORDINATE:
    {
      te::qt::af::TrackedCoordinate* e = static_cast<te::qt::af::TrackedCoordinate*>(evt);
      QString text = tr("Coordinates: ") + "(" + QString::number(e->m_pos.x()) + " , " + QString::number(e->m_pos.y()) + ")";
      QStatusBar* sb = statusBar();
      sb->showMessage(text);
    }
    break;

    //PROVISORIO
    case te::qt::af::evt::LAYER_SELECTED:
    {
      te::qt::af::LayerSelected* e = static_cast<te::qt::af::LayerSelected*>(evt);
      
      if(e->m_layer->getType() == "RASTERLAYER")
      {
        m_rasterVisualDock->setRasterLayer(dynamic_cast<te::map::RasterLayer*>(e->m_layer));
      }
    }
    break;

    default :
    break;
  }
}

void MainWindow::addOGRLayer()
{
  NewOGRLayer w(this);

  if(w.exec() == QDialog::Accepted)
  {
    te::map::AbstractLayer* layer = w.getNewLayer();

    if(m_display->getDisplayComponent()->getSRID() == -1 ||
       m_display->getDisplayComponent()->getSRID() == TE_UNKNOWN_SRS)
    {  
      m_display->getDisplayComponent()->setSRID(layer->getSRID());
      m_display->getDisplayComponent()->setExtent(*layer->getExtent());
    }

    if(layer != 0)
      te::qt::af::teApp::getInstance().broadCast(&te::qt::af::LayerAdded(layer));
  }
}

void MainWindow::addGDALLayer()
{
  NewGDALLayer w(this);

  if(w.exec() == QDialog::Accepted)
  {
    te::map::AbstractLayer* layer = w.getNewLayer();

    if(m_display->getDisplayComponent()->getSRID() == -1 ||
       m_display->getDisplayComponent()->getSRID() == TE_UNKNOWN_SRS)
    {  
      m_display->getDisplayComponent()->setSRID(layer->getSRID());
      m_display->getDisplayComponent()->setExtent(*layer->getExtent());
    }

    if(layer != 0)
      te::qt::af::teApp::getInstance().broadCast(&te::qt::af::LayerAdded(layer));
  }
}

void MainWindow::layerVisibilityChanged(const QModelIndex& idx)
{
  te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(idx.internalPointer());
  te::map::AbstractLayer* lay = item->getRefLayer();

  if(lay->getVisibility()==te::map::VISIBLE) 
    m_layers.insert(std::pair<int, te::map::AbstractLayer*>(idx.row(), lay));
  else
    m_layers.erase(idx.row());
}

void MainWindow::drawLayers()
{
  if(!m_layers.empty())
    m_display->drawLayers(m_layers);
}

void MainWindow::setPanTool(bool status)
{
  if(status)
    m_display->setCurrentTool(new te::qt::widgets::Pan(m_display->getDisplayComponent(), Qt::OpenHandCursor, Qt::ClosedHandCursor));
}

void MainWindow::setZoomAreaTool(bool status)
{
  if(status)
    m_display->setCurrentTool(new te::qt::widgets::ZoomArea(m_display->getDisplayComponent(), Qt::BlankCursor));
}

void MainWindow::setAngleTool(bool status)
{
  if(status)
    m_display->setCurrentTool(new te::qt::widgets::Measure(m_display->getDisplayComponent(), te::qt::widgets::Measure::Angle, this));
}

void MainWindow::setAreaMeasureTool(bool status)
{
  if(status)
    m_display->setCurrentTool(new te::qt::widgets::Measure(m_display->getDisplayComponent(), te::qt::widgets::Measure::Area, this));
}

void MainWindow::setDistanceTool(bool status)
{
  if(status)
    m_display->setCurrentTool(new te::qt::widgets::Measure(m_display->getDisplayComponent(), te::qt::widgets::Measure::Distance, this));
}

void MainWindow::showProgressDock()
{
  if(m_progressDock->isVisible())
  {
    m_progressDock->setVisible(false);
  }
  else
  {
    m_progressDock->setVisible(true);
  }
}

void MainWindow::makeDialog()
{
  //! Setting icons
  m_ui->m_show_explorer->setIcon(QIcon::fromTheme("tree-visible"));
  m_ui->m_show_display->setIcon(QIcon::fromTheme("display-visible"));
  m_ui->m_show_table->setIcon(QIcon::fromTheme("grid-visible"));
  m_ui->m_drawlayers_act->setIcon(QIcon::fromTheme("draw-layer"));
  m_ui->m_actionPan->setIcon(QIcon::fromTheme("pan"));
  m_ui->m_actionZoom_area->setIcon(QIcon::fromTheme("zoom-in"));
  m_ui->m_actionPointing->setIcon(QIcon::fromTheme("pointer"));
  m_ui->m_area_act->setIcon(QIcon::fromTheme("area-measure"));
  m_ui->m_angle_act->setIcon(QIcon::fromTheme("angle-measure"));
  m_ui->m_distance_act->setIcon(QIcon::fromTheme("distance-measure"));
  m_ui->m_actionRasterVisual->setIcon(QIcon::fromTheme("raster-visual"));

  //! Putting tools on excluse group
  QActionGroup* vis_tools_group = new QActionGroup(this);
  vis_tools_group->setExclusive(true);
  m_ui->m_actionPan->setActionGroup(vis_tools_group);
  m_ui->m_actionZoom_area->setActionGroup(vis_tools_group);
  m_ui->m_area_act->setActionGroup(vis_tools_group);
  m_ui->m_angle_act->setActionGroup(vis_tools_group);
  m_ui->m_distance_act->setActionGroup(vis_tools_group);

  //! Initializing components
  te::qt::widgets::LayerExplorer* exp = new te::qt::widgets::LayerExplorer(this);
  exp->setModel(new te::qt::widgets::LayerExplorerModel(new te::map::FolderLayer("MainLayer", tr("My Layers").toStdString()), exp));
  connect(exp, SIGNAL(checkBoxWasClicked(const QModelIndex&)), SLOT(layerVisibilityChanged(const QModelIndex&)));

  te::qt::widgets::MapDisplay* map = new te::qt::widgets::MultiThreadMapDisplay(QSize(512, 512), this);
  map->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  map->setMouseTracking(true);
  
  te::qt::widgets::TabularViewer* view = new te::qt::widgets::TabularViewer(this);

  //! Initializing connectors
  m_explorer = new te::qt::af::LayerExplorer(exp);
  m_display = new te::qt::af::MapDisplay(map);
  m_viewer = new te::qt::af::TabularViewer(view);

  //! Connecting framework
  te::qt::af::teApp::getInstance().addListener(this);
  te::qt::af::teApp::getInstance().addListener(m_explorer);
  te::qt::af::teApp::getInstance().addListener(m_display);
  te::qt::af::teApp::getInstance().addListener(m_viewer);

  //! Docking components
  QDockWidget* doc = new QDockWidget(tr("Layer explorer"), this);
  doc->setWidget(exp);
  QMainWindow::addDockWidget(Qt::LeftDockWidgetArea, doc);
  doc->connect(m_ui->m_show_explorer, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_show_explorer->connect(doc, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_ui->m_show_explorer->setChecked(true);

  doc = new QDockWidget(tr("Main display"), this);
  doc->setWidget(map);
  QMainWindow::setCentralWidget(doc);
  doc->connect(m_ui->m_show_display, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_show_display->connect(doc, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_ui->m_show_display->setChecked(true);

  doc = new QDockWidget(tr("Tabular data viewer"), this);
  doc->setWidget(view);
  QMainWindow::addDockWidget(Qt::BottomDockWidgetArea, doc);
  doc->connect(m_ui->m_show_table, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_show_table->connect(doc, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_ui->m_show_table->setChecked(false);
  doc->setVisible(false);

  //! Raster Visual Dock widget
  m_rasterVisualDock = new te::qt::widgets::RasterVisualDockWidget(tr("Raster Enhancement"), this);
  connect(m_rasterVisualDock, SIGNAL(symbolizerChanged()), this, SLOT(drawLayers()));
  QMainWindow::addDockWidget(Qt::RightDockWidgetArea, m_rasterVisualDock);
  m_rasterVisualDock->connect(m_ui->m_actionRasterVisual, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_actionRasterVisual->connect(m_rasterVisualDock, SIGNAL(visibilityChanged(bool)), SLOT(setChecked(bool)));
  m_ui->m_actionRasterVisual->setChecked(false);
  m_rasterVisualDock->setVisible(false);

  //! Progress support
  te::qt::widgets::ProgressViewerBar* pvb = new te::qt::widgets::ProgressViewerBar(this);
  te::common::ProgressManager::getInstance().addViewer(pvb);

  te::qt::widgets::ProgressViewerWidget* pvw = new te::qt::widgets::ProgressViewerWidget(this);
  te::common::ProgressManager::getInstance().addViewer(pvw);

  statusBar()->addPermanentWidget(pvb);

  connect(pvb, SIGNAL(clicked()), this, SLOT(showProgressDock()));

  m_progressDock = new QDockWidget(this);
  m_progressDock->setWidget(pvw);
  m_progressDock->setMinimumWidth(250);
  m_progressDock->setAllowedAreas(Qt::RightDockWidgetArea);
  m_progressDock->setWindowTitle(tr("Tasks Progress"));
  addDockWidget(Qt::RightDockWidgetArea, m_progressDock);

  m_progressDock->setVisible(false);
}
