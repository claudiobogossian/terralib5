#include "MainWindow.h"
#include <ui_MainWindow.h>
#include "NewOGRLayer.h"
#include "NewGDALLayer.h"

//! TerraLib include files
#include <terralib/common/SystemApplicationSettings.h>
#include <terralib/common/UserApplicationSettings.h>
#include <terralib/common/Exception.h>

#include <terralib/qt/af/events/NewToolBar.h>
#include <terralib/qt/af/events/LayerAdded.h>
#include <terralib/qt/af/events/LayerSelected.h>
#include <terralib/qt/af/events/TrackedCoordinate.h>
#include <terralib/qt/af/CoreApplication.h>
#include <terralib/qt/af/ApplicationPlugins.h>
#include <terralib/qt/af/SplashScreenManager.h>
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
#include <terralib/qt/widgets/plugin/manager/PluginManagerDialog.h>

#include <terralib/qt/widgets/datasource/selector/DataSourceSelectorDialog.h>

#include <terralib/common/progress/TaskProgress.h>
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/qt/widgets/progress/ProgressViewerDialog.h>
#include <terralib/qt/widgets/progress/ProgressViewerWidget.h>
#include <terralib/qt/widgets/progress/ProgressViewerBar.h>

#include <terralib/qt/widgets/se/RasterVisualDockWidget.h>

//! Qt include files
#include <QDockWidget>
#include <QActionGroup>
#include <QDir>
#include <QMessageBox>

void getConfigurations(std::vector< std::pair<std::string, std::string> >& configs)
{
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns", "http://www.terralib.org/schemas/af"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af " + std::string(TERRALIB_SCHEMA_LOCATION) + "/terralib/af/af.xsd"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.release", "2011-01-01"));
  configs.push_back(std::pair<std::string, std::string>("Application.Organization", "INPE"));
  configs.push_back(std::pair<std::string, std::string>("Application.Name", "TerraView"));
  configs.push_back(std::pair<std::string, std::string>("Application.Title", "TerraView"));
  configs.push_back(std::pair<std::string, std::string>("Application.IconName", "terralib_logo"));

  QString user_set_path = QDir::toNativeSeparators(qApp->applicationDirPath() + "/user_settings.xml");
  QString app_plg_path = QDir::toNativeSeparators(qApp->applicationDirPath() + "/application_plugins.xml");

  configs.push_back(std::pair<std::string, std::string>("Application.UserSettingsFile.<xmlattr>.xlink:href", user_set_path.toStdString()));
  configs.push_back(std::pair<std::string, std::string>("Application.PluginsFile.<xmlattr>.xlink:href", app_plg_path.toStdString()));
  configs.push_back(std::pair<std::string, std::string>("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href", QDir::toNativeSeparators(ICON_THEME_PATH).toStdString()));
  configs.push_back(std::pair<std::string, std::string>("Application.IconThemeInfo.DefaultTheme", ICON_THEME));
  configs.push_back(std::pair<std::string, std::string>("Application.ToolBarDefaultIconSize", "24"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Cellular Space"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Data Access"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Data Management"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Examples"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Language Bindings"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Location Base Services"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Plugin Management"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Spatial Analysis"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Spatial Operations"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Web Services"));
  configs.push_back(std::pair<std::string, std::string>("Application.AboutDialogLogo.<xmlattr>.xlink:href",  ""));
  configs.push_back(std::pair<std::string, std::string>("Application.TerraLibLogo.<xmlattr>.xlink:href",  QDir::toNativeSeparators(TERRALIB_LOGO).toStdString()));
}

void getPluginsConfigurations(std::vector< std::pair<std::string, std::string> >& configs)
{
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.xmlns", "http://www.terralib.org/schemas/af"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af " + std::string(TERRALIB_SCHEMA_LOCATION) + "/terralib/af/af.xsd"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.release", "2011-01-01"));

  //! ado
#ifdef _MSC_VER
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Name", "te.da.ado"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Path.<xmlattr>.xlink:href", std::string(TERRALIB_BIN_DIR) + "/plugin_ado_info.xml"));
#endif
  //! gdal
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Name", "te.da.gdal"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Path.<xmlattr>.xlink:href", std::string(TERRALIB_BIN_DIR) + "/plugin_gdal_info.xml"));
  //! ogr
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Name", "te.da.ogr"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Path.<xmlattr>.xlink:href", std::string(TERRALIB_BIN_DIR) + "/plugin_ogr_info.xml"));
  //! pgis
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Name", "te.da.pgis"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Path.<xmlattr>.xlink:href", std::string(TERRALIB_BIN_DIR) + "/plugin_pgis_info.xml"));
}

void getUserConfigurations(std::vector< std::pair<std::string, std::string> >& configs)
{
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.xmlns", "http://www.terralib.org/schemas/af"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af " + std::string(TERRALIB_SCHEMA_LOCATION) + "/terralib/af/af.xsd"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.release", "2011-01-01"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.SelectedIconTheme", "terralib"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.LastSearchedFolder.<xmlattr>.xlink:href", qApp->applicationDirPath().toStdString()));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.ToolBarIconSize", "24"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.SpecificPlugins", ""));

#ifdef _MSC_VER
  configs.push_back(std::pair<std::string, std::string>("UserSettings.EnabledPlugins.Plugin", "te.da.ado"));
#endif
  configs.push_back(std::pair<std::string, std::string>("UserSettings.EnabledPlugins.Plugin", "te.da.gdal"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.EnabledPlugins.Plugin", "te.da.ogr"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.EnabledPlugins.Plugin", "te.da.pgis"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.DataSourcesFile", ""));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.MostRecentProject.<xmlattr>.xlink:href", ""));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.RecentProjects", ""));
}

MainWindow::MainWindow(QWidget* parent) :
QMainWindow(parent, 0),
  m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);

  setMinimumSize(60, 60);
  resize(QSize(512, 512));

  std::vector< std::pair<std::string, std::string> > configs;
  getConfigurations(configs);
  te::qt::af::Application::getInstance().setApplicationInfo(configs);

  configs.clear();
  getPluginsConfigurations(configs);
  te::qt::af::Application::getInstance().setApplicationPlgInfo(configs);

  configs.clear();
  getUserConfigurations(configs);
  te::qt::af::Application::getInstance().setUserInfo(configs);

  QString err_msg;

  try
  {
    te::qt::af::Application::getInstance().initialize();
  }
  catch(te::common::Exception& e)
  {
    err_msg = QString(e.what());
  }

  std::string icon = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconName");
  std::string title = te::common::SystemApplicationSettings::getInstance().getValue("Application.Title");

  if(!icon.empty())
    QMainWindow::setWindowIcon(QIcon::fromTheme(icon.c_str()));

  if(!title.empty())
    QMainWindow::setWindowTitle(title.c_str());

  makeDialog();

  if(!err_msg.isEmpty())
    QMessageBox::warning(this, tr("TerraView warning"), err_msg);
}

MainWindow::~MainWindow()
{
  delete m_display;
  delete m_viewer;

  te::qt::af::Application::getInstance().finalize();
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
    {
      te::qt::af::LayerAdded evt(layer);
      te::qt::af::Application::getInstance().broadcast(&evt);
    }
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
    {
      te::qt::af::LayerAdded evt(layer);
      te::qt::af::Application::getInstance().broadcast(&evt);
    }
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

void MainWindow::openPluginsManager()
{
  te::qt::widgets::PluginManagerDialog dlg(this);

  te::qt::af::ApplicationPlugins::getInstance().connect(&dlg, SIGNAL(pluginsAdded(const std::map<std::string, std::string>&)), 
    SLOT(addPlugins(const std::map<std::string, std::string>&)));
  te::qt::af::ApplicationPlugins::getInstance().connect(&dlg, SIGNAL(pluginsRemoved(const std::vector<std::string>&)), SLOT(removePlugins(const std::vector<std::string>&)));

  dlg.exec();
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

void MainWindow::openDSrcManager()
{
  te::qt::widgets::DataSourceSelectorDialog dlg(this);

  dlg.exec();
}


void MainWindow::makeDialog()
{
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
  te::qt::af::Application::getInstance().addListener(this);
  te::qt::af::Application::getInstance().addListener(m_explorer);
  te::qt::af::Application::getInstance().addListener(m_display);
  te::qt::af::Application::getInstance().addListener(m_viewer);

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
}
