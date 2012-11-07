#include "MainWindow.h"
#include <ui_MainWindow.h>
#include "NewOGRLayer.h"

//! TerraLib include files
#include <terralib/common/TerraLib.h>
#include <terralib/common/OSSettingsDir.h>
#include <terralib/common/SystemApplicationSettings.h>

#include <terralib/qt/af/events/NewToolBar.h>
#include <terralib/qt/af/events/LayerAdded.h>
#include <terralib/qt/af/events/TrackedCoordinate.h>
#include <terralib/qt/af/CoreApplication.h>
#include <terralib/qt/af/connectors/LayerExplorer.h>
#include <terralib/qt/af/connectors/MapDisplay.h>
#include <terralib/qt/af/connectors/TabularViewer.h>
#include <terralib/qt/widgets/layer/AbstractTreeItem.h>
#include <terralib/maptools/Layer.h>

#include <terralib/qt/widgets/layer/LayerExplorer.h>
#include <terralib/maptools/FolderLayer.h>
#include <terralib/qt/widgets/layer/LayerExplorerModel.h>
#include <terralib/qt/widgets/canvas/MultiThreadMapDisplay.h>
#include <terralib/qt/widgets/dataview/TabularViewer.h>
#include <terralib/qt/widgets/tools/Pan.h>
#include <terralib/qt/widgets/tools/ZoomArea.h>
#include <terralib/qt/widgets/tools/Measure.h>

//! Qt include files
#include <QDockWidget>
#include <QActionGroup>
#include <QFileInfo>

void getConfigurations(std::vector< std::pair<std::string, std::string> >& configs)
{
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns", "http://www.terralib.org/schemas/af"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af ../myschemas/terralib/af/af.xsd"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.release", "2011-01-01"));
  configs.push_back(std::pair<std::string, std::string>("Application.Organization", "INPE"));
  configs.push_back(std::pair<std::string, std::string>("Application.Name", "TerraView"));
  configs.push_back(std::pair<std::string, std::string>("Application.Title", "TerraView"));
  configs.push_back(std::pair<std::string, std::string>("Application.IconName", "terralib_logo"));

  std::string user_set_path = te::common::OSSettingsDir::getInstance().getUserSettingsPath() + "/terraView/user_settings.xml";
  std::string app_plg_path = te::common::OSSettingsDir::getInstance().getSystemSettingsPath() + "/terraView/application_plugins.xml";

  QFileInfo info_us(user_set_path.c_str());
  QFileInfo info_plg(app_plg_path.c_str());

  configs.push_back(std::pair<std::string, std::string>("Application.UserSettingsFile.<xmlattr>.xlink:href", info_us.absoluteFilePath().toStdString()));
  configs.push_back(std::pair<std::string, std::string>("Application.PluginsFile.<xmlattr>.xlink:href", info_plg.absoluteFilePath().toStdString()));
  configs.push_back(std::pair<std::string, std::string>("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href", ICON_THEME_PATH));
  configs.push_back(std::pair<std::string, std::string>("Application.IconThemeInfo.DefaultTheme", "terralib"));
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

  te::qt::af::teApp::getInstance().setApplicationName("terraView");
  te::qt::af::teApp::getInstance().setApplicationInfo(configs);

  TerraLib::getInstance().initialize();

  std::string iconThemePath = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href");
  std::string iconTheme = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconThemeInfo.DefaultTheme");
  std::string icon = te::common::SystemApplicationSettings::getInstance().getValue("Application.IconName");
  std::string title = te::common::SystemApplicationSettings::getInstance().getValue("Application.Title");
  std::string iconSize = te::common::SystemApplicationSettings::getInstance().getValue("Application.ToolBarDefaultIconSize");

  if(!iconThemePath.empty())
  {
    QStringList ls;
    ls << iconThemePath.c_str();
    QIcon::setThemeSearchPaths(ls);
  }

  if(!iconTheme.empty())
    QIcon::setThemeName(iconTheme.c_str());

  if(!icon.empty())
    QMainWindow::setWindowIcon(QIcon::fromTheme(icon.c_str()));

  if(!title.empty())
    QMainWindow::setWindowTitle(title.c_str());

  if(!iconSize.empty())
  {
    QString sh = QString("QToolBar { qproperty-iconSize: ") + iconSize.c_str() + "px " + iconSize.c_str() + "px; }";
    qApp->setStyleSheet(sh);
  }

  makeDialog();

  updateIcons();
}

MainWindow::~MainWindow()
{
  delete m_display;
  TerraLib::getInstance().finalize();
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

    default :
    break;
  }
}

void MainWindow::addOGRLayer()
{
  te::map::AbstractLayer* layer = NewOGRLayer::getNewLayer(this);

  if(layer != 0)
    te::qt::af::teApp::getInstance().broadCast(&te::qt::af::LayerAdded(layer));
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
  te::qt::af::teApp::getInstance().addListener(this);
  te::qt::af::teApp::getInstance().addListener(m_explorer);
  te::qt::af::teApp::getInstance().addListener(m_display);
  te::qt::af::teApp::getInstance().addListener(m_viewer);

  //! Docking components
  QDockWidget* doc = new QDockWidget(tr("Layer explorer"), this);
  doc->setWidget(exp);
  QMainWindow::addDockWidget(Qt::LeftDockWidgetArea, doc);
  doc->connect(m_ui->m_show_explorer, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_show_explorer->setChecked(true);

  doc = new QDockWidget(tr("Main display"), this);
  doc->setWidget(map);
  QMainWindow::setCentralWidget(doc);
  doc->connect(m_ui->m_show_display, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_show_display->setChecked(true);

  doc = new QDockWidget(tr("Tabular data viewer"), this);
  doc->setWidget(view);
  QMainWindow::addDockWidget(Qt::BottomDockWidgetArea, doc);
  doc->connect(m_ui->m_show_table, SIGNAL(toggled(bool)), SLOT(setVisible(bool)));
  m_ui->m_show_table->setChecked(false);
  doc->setVisible(false);
}
void MainWindow::updateIcons()
{
  //! Setting icons
  m_ui->m_show_explorer->setIcon(QIcon::fromTheme("layerExplorer"));
  m_ui->m_show_display->setIcon(QIcon::fromTheme("mapDisplay"));
  m_ui->m_show_table->setIcon(QIcon::fromTheme("table"));
  m_ui->m_drawlayers_act->setIcon(QIcon::fromTheme("drawLayers"));
  m_ui->m_actionPan->setIcon(QIcon::fromTheme("pan"));
  m_ui->m_actionZoom_area->setIcon(QIcon::fromTheme("zoom-in"));
  m_ui->m_area_act->setIcon(QIcon::fromTheme("area-measure"));
  m_ui->m_angle_act->setIcon(QIcon::fromTheme("angle-measure"));
  m_ui->m_distance_act->setIcon(QIcon::fromTheme("distance-measure"));
}
