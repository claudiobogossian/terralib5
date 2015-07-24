#include "BaseApplication.h"

#include "ui_BaseApplicationForm.h"

// TerraLib
#include "ApplicationController.h"
#include "connectors/LayerExplorer.h"
#include "connectors/MapDisplay.h"
#include "connectors/StyleExplorer.h"
#include "events/ApplicationEvents.h"
#include "events/LayerEvents.h"
#include "Utils.h"

#include "../widgets/tools/ZoomArea.h"
#include "../widgets/tools/Pan.h"
#include "../widgets/tools/Selection.h"
#include "../widgets/srs/SRSManagerDialog.h"

#include "../../common/TerraLib.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
#include "../../plugin/PluginManager.h"

#include "events/LayerEvents.h"
#include "events/MapEvents.h"

// Qt
#include <QMessageBox>
#include <QToolButton>

te::qt::af::BaseApplication::BaseApplication(QWidget* parent) :
  QMainWindow(parent)
{
  m_ui = 0;
  m_layerExplorer = 0;
  m_display = 0;
  m_styleExplorer = 0;
  m_app = 0;
}


te::qt::af::BaseApplication::~BaseApplication()
{
  delete m_ui;
  delete m_layerExplorer;
  delete m_display;
  delete m_styleExplorer;

  te::qt::af::UpdateUserSettings();

  if (m_app)
  m_app->finalize();
}

void te::qt::af::BaseApplication::init(const QString& cfgFile)
{
  initFramework(cfgFile);

  m_ui = new Ui::BaseApplicationForm;
  m_ui->setupUi(this);

  QActionGroup* grp = new QActionGroup(this);
  m_ui->m_zoomInAction->setActionGroup(grp);
  m_ui->m_panAction->setActionGroup(grp);
  m_ui->m_selectToolAction->setActionGroup(grp);

  try
  {
    makeDialog();
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::warning(this, "Error", e.what());
  }
}

te::qt::widgets::LayerItemView* te::qt::af::BaseApplication::getLayerExplorer()
{
  return m_ui->m_treeView;
}

te::qt::widgets::MapDisplay* te::qt::af::BaseApplication::getMapDisplay()
{
  return m_ui->m_display;
}

void te::qt::af::BaseApplication::onDrawTriggered()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  te::qt::af::evt::DrawButtonClicked drawClicked;
  emit triggered(&drawClicked);

  m_display->draw(getLayerExplorer()->getVisibleLayers());

  QApplication::restoreOverrideCursor();
}

void te::qt::af::BaseApplication::onFitLayersTriggered()
{
  QApplication::setOverrideCursor(Qt::BusyCursor);

  m_display->fit(getLayerExplorer()->getVisibleLayers());

  QApplication::restoreOverrideCursor();
}

void te::qt::af::BaseApplication::onPanTriggered(bool s)
{
  if(s)
    m_display->setCurrentTool(new te::qt::widgets::Pan(m_display->getDisplay(), Qt::CrossCursor));
}

void te::qt::af::BaseApplication::onSelectionTriggered(bool s)
{
  if(!s)
    return;
  //Revisar: Fred
  //te::qt::widgets::Selection* selection = new te::qt::widgets::Selection(m_display->getDisplay(), Qt::ArrowCursor, m_ui->m_layerExplorer->getSelectedSingleLayers());
  //m_display->setCurrentTool(selection);

  //connect(m_ui->m_layerExplorer, SIGNAL(selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)),
  //        selection, SLOT(setLayers(const std::list<te::map::AbstractLayerPtr>&)));
  //connect(selection, SIGNAL(layerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)), SLOT(onLayerSelectionChanged(const te::map::AbstractLayerPtr&)));

  //te::qt::af::evt::SelectionButtonToggled esel;
  //emit triggered(&esel);
}

void te::qt::af::BaseApplication::onMapSRIDTriggered()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if (srsDialog.exec() == QDialog::Rejected)
    return;

  std::pair<int, std::string> srid = srsDialog.getSelectedSRS();

  te::qt::af::evt::MapSRIDChanged mapSRIDChagned(srid);
  m_app->triggered(&mapSRIDChagned);

  m_display->getDisplay()->setSRID(srid.first);
}

void te::qt::af::BaseApplication::onMapSetUnknwonSRIDTriggered()
{
  std::pair<int, std::string> srid = std::make_pair(TE_UNKNOWN_SRS, "");
  te::qt::af::evt::MapSRIDChanged mapSRIDChagned(srid);
  m_app->triggered(&mapSRIDChagned);

  m_display->getDisplay()->setSRID(TE_UNKNOWN_SRS);
}

void te::qt::af::BaseApplication::onStopDrawTriggered()
{
  te::common::ProgressManager::getInstance().cancelTasks(te::common::TaskProgress::DRAW);
}

void te::qt::af::BaseApplication::onZoomInTriggered(bool s)
{
  if(s)
    m_display->setCurrentTool(new te::qt::widgets::ZoomArea(m_display->getDisplay(), Qt::CrossCursor, this));
}

void te::qt::af::BaseApplication::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch (e->m_id)
  {
    case te::qt::af::evt::MAP_SRID_CHANGED:
    {
      te::qt::af::evt::MapSRIDChanged* mEvt = static_cast<te::qt::af::evt::MapSRIDChanged*>(e);

      std::pair<int, std::string> srid = mEvt->m_srid;

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

    case te::qt::af::evt::LAYER_SELECTED_OBJECTS_CHANGED:
    {
      te::qt::af::evt::LayerSelectedObjectsChanged* lEvt = static_cast<te::qt::af::evt::LayerSelectedObjectsChanged*>(e);
      if (lEvt->m_layer == 0 || lEvt->m_layer->getSelected() == 0)
        return;

      m_selected->setText(tr("Selected rows: ") + QString::number(lEvt->m_layer->getSelected()->size()));
    }
    break;

  default:
    break;
  }
}

void te::qt::af::BaseApplication::onLayerSelectionChanged(const te::map::AbstractLayerPtr& layer)
{
  te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
  emit triggered(&e);
}

void te::qt::af::BaseApplication::makeDialog()
{
  //start main components
  m_layerExplorer = new LayerExplorer(m_ui->m_treeView);
  m_display = new MapDisplay(m_ui->m_display);
  m_styleExplorer = new StyleExplorer(m_ui->m_styleExplorer);

  initMenus();

  initActions();

  initSlotsConnections();

  initStatusBar();

  //connect components
  m_app->addListener(m_layerExplorer);
  m_app->addListener(m_display);
  m_app->addListener(m_styleExplorer);
  m_app->addListener(this);

  //set app info
  setWindowTitle(m_app->getAppTitle());
  setWindowIcon(QIcon(m_app->getAppIconName()));
  setWindowIconText(m_app->getAppTitle());

  //register menubar and load plugins
  m_app->registerMenuBar(m_menubar);
  m_app->initializePlugins();
}

void te::qt::af::BaseApplication::initFramework(const QString& cfgFile)
{
  m_app = &AppCtrlSingleton::getInstance();
  m_app->setConfigFile(cfgFile.toStdString());
  m_app->initialize();
}

void te::qt::af::BaseApplication::initStatusBar()
{
  // Status Bar
  m_statusbar = new QStatusBar(this);
  m_statusbar->setObjectName("StatusBar");
  setStatusBar(m_statusbar);

  // Selected status
  m_selected = new QLabel(m_statusbar);
  m_selected->setText(tr("Selected rows: 0"));
  m_statusbar->addPermanentWidget(m_selected);

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

void te::qt::af::BaseApplication::initActions()
{
  initAction(m_mapSRID, "srs", "Map.SRID", tr("&SRS..."), tr("Config the Map SRS"), true, false, true, m_menubar);
  initAction(m_mapUnknownSRID, "srs-unknown", "Map.UnknownSRID", tr("&Set Unknown SRS"), tr("Set the Map SRS to unknown"), true, false, true, m_menubar);
  initAction(m_mapStopDrawing, "map-draw-cancel", "Map.Stop Drawing", tr("&Stop Drawing"), tr("Stop all drawing tasks"), true, false, true, m_menubar);
}

void te::qt::af::BaseApplication::initMenus()
{
  m_menubar = new QMenuBar(this);
  m_menubar->setObjectName("menubar");
  m_menubar->setGeometry(QRect(0, 0, 640, 21));

  setMenuBar(m_menubar);
}

void te::qt::af::BaseApplication::initSlotsConnections()
{
  connect(m_mapSRID, SIGNAL(triggered()), SLOT(onMapSRIDTriggered()));
  connect(m_mapUnknownSRID, SIGNAL(triggered()), SLOT(onMapSetUnknwonSRIDTriggered()));
  connect(m_mapStopDrawing, SIGNAL(triggered()), SLOT(onStopDrawTriggered()));
}

void te::qt::af::BaseApplication::initAction(QAction*& act, const QString& icon, const QString& name,
  const QString& text, const QString& tooltip,
  bool iconVisibleInMenu, bool isCheckable,
  bool enabled, QObject* parent)
{
  act = new QAction(parent);

  if (!icon.isEmpty())
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
