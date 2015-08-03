#include "BaseApplication.h"

#include "ui_BaseApplicationForm.h"

// TerraLib
#include "ApplicationController.h"
#include "connectors/ChartDisplayDockWidget.h"
#include "connectors/DataSetTableDockWidget.h"
#include "connectors/LayerExplorer.h"
#include "connectors/MapDisplay.h"
#include "connectors/StyleExplorer.h"
#include "events/ApplicationEvents.h"
#include "events/LayerEvents.h"
#include "Utils.h"

#include "../widgets/layer/info/LayerPropertiesInfoWidget.h"
#include "../widgets/tools/Info.h"
#include "../widgets/tools/Pan.h"
#include "../widgets/tools/Selection.h"
#include "../widgets/tools/ZoomArea.h"
#include "../widgets/tools/ZoomClick.h"
#include "../widgets/srs/SRSManagerDialog.h"

#include "../../common/TerraLib.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
#include "../../maptools/Utils.h"
#include "../../plugin/PluginManager.h"

#include "events/LayerEvents.h"
#include "events/MapEvents.h"

// Qt
#include <QInputDialog>
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
  m_mapCursorSize = QSize(20, 20);
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

  try
  {
    makeDialog();
  }
  catch(te::common::Exception& e)
  {
    QMessageBox::warning(this, "Error", e.what());
  }
}

te::qt::widgets::LayerExplorer* te::qt::af::BaseApplication::getLayerExplorer()
{
  return m_ui->m_layerExplorer;
}

te::qt::widgets::MapDisplay* te::qt::af::BaseApplication::getMapDisplay()
{
  return m_ui->m_display;
}

te::qt::af::DataSetTableDockWidget* te::qt::af::BaseApplication::getLayerDock(const te::map::AbstractLayer* layer, const std::vector<te::qt::af::DataSetTableDockWidget*>& docs)
{
  std::vector<te::qt::af::DataSetTableDockWidget*>::const_iterator it;

  for (it = docs.begin(); it != docs.end(); ++it)
    if ((*it)->getLayer() == layer)
      return *it;

  return 0;
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

void te::qt::af::BaseApplication::onDrawTriggered()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  te::qt::af::evt::DrawButtonClicked drawClicked;
  emit triggered(&drawClicked);

  m_display->draw(m_ui->m_layerExplorer->getTopLayers());

  QApplication::restoreOverrideCursor();
}

void te::qt::af::BaseApplication::onZoomInToggled(bool checked)
{
  if (!checked)
    return;

  QCursor zoomAreaCursor(QIcon::fromTheme("zoom-in").pixmap(m_mapCursorSize));

  te::qt::widgets::ZoomArea* zoomArea = new te::qt::widgets::ZoomArea(m_display->getDisplay(), zoomAreaCursor);
  m_display->setCurrentTool(zoomArea);

  te::qt::af::evt::ZoomInButtonToggled ezoom;
  m_app->triggered(&ezoom);
}

void te::qt::af::BaseApplication::onZoomOutToggled(bool checked)
{
  if (!checked)
    return;

  QCursor zoomOutCursor(QIcon::fromTheme("zoom-out").pixmap(m_mapCursorSize));

  te::qt::widgets::ZoomClick* zoomOut = new te::qt::widgets::ZoomClick(m_display->getDisplay(), zoomOutCursor, 2.0, te::qt::widgets::Zoom::Out);
  m_display->setCurrentTool(zoomOut);

  te::qt::af::evt::ZoomOutButtonToggled ezoom;
  m_app->triggered(&ezoom);
}

void te::qt::af::BaseApplication::onPreviousExtentTriggered()
{
  m_display->previousExtent();
}

void te::qt::af::BaseApplication::onNextExtentTriggered()
{
  m_display->nextExtent();
}

void te::qt::af::BaseApplication::onPanToggled(bool checked)
{
  if (!checked)
    return;

  te::qt::widgets::Pan* pan = new te::qt::widgets::Pan(m_display->getDisplay(), Qt::OpenHandCursor, Qt::ClosedHandCursor);
  m_display->setCurrentTool(pan);

  te::qt::af::evt::PanButtonToggled epan;
  m_app->triggered(&epan);
}

void te::qt::af::BaseApplication::onZoomExtentTriggered()
{
  if (!m_layerExplorer && m_layerExplorer->getExplorer()->getTopLayers().empty())
    return;

  //m_display->fit(m_layerExplorer->getExplorer()->getAllLayers());
  // TODO: Change to get only visible single layers, not selected.
  m_display->fit(m_layerExplorer->getExplorer()->getSelectedAndVisibleSingleLayers());

}

void te::qt::af::BaseApplication::onInfoToggled(bool checked)
{
  if (!checked)
    return;

  QPixmap pxmap = QIcon::fromTheme("pointer-info").pixmap(m_mapCursorSize);
  QCursor infoCursor(pxmap, 0, 0);

  te::qt::widgets::Info* info = new te::qt::widgets::Info(m_display->getDisplay(), infoCursor, m_layerExplorer->getExplorer()->getSelectedSingleLayers());
  m_display->setCurrentTool(info);

  connect(m_layerExplorer->getExplorer(), SIGNAL(selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)), info, SLOT(setLayers(const std::list<te::map::AbstractLayerPtr>&)));
}

void te::qt::af::BaseApplication::onMapRemoveSelectionTriggered()
{
  //std::list<te::map::AbstractLayerPtr> layers = m_layerExplorer->getExplorer()->getAllLayers();
  std::list<te::map::AbstractLayerPtr> layers = m_layerExplorer->getExplorer()->getTopLayers();
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while (it != layers.end())
  {
    te::map::AbstractLayerPtr layer = (*it);

    layer->clearSelected();

    ++it;

    te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
    m_app->triggered(&e);
  }
}

void te::qt::af::BaseApplication::onSelectionToggled(bool checked)
{
  if (!checked)
    return;

  te::qt::widgets::Selection* selection = new te::qt::widgets::Selection(m_display->getDisplay(), Qt::ArrowCursor, m_layerExplorer->getExplorer()->getSelectedSingleLayers());
  m_display->setCurrentTool(selection);

  connect(m_layerExplorer->getExplorer(), SIGNAL(selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)), selection, SLOT(setLayers(const std::list<te::map::AbstractLayerPtr>&)));
  connect(selection, SIGNAL(layerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)), SLOT(onLayerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)));

  te::qt::af::evt::SelectionButtonToggled esel;
  m_app->triggered(&esel);
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

void te::qt::af::BaseApplication::onLayerRemoveTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_layerExplorer->getExplorer()->getSelectedLayerItems();

  if (selectedLayerItems.empty())
  {
    QString msg = tr("Select at least one layer to be removed!");
    QMessageBox::warning(this, tr("Remove Layer"), msg);

    return;
  }

  QString msg;
  QString questionTitle;

  if (selectedLayerItems.size() == 1)
  {
    msg = tr("Do you really want to remove the selected layer?");
    questionTitle = tr("Remove Layer");
  }
  else
  {
    msg = tr("Do you really want to remove the selected layers?");
    questionTitle = tr("Remove Layers");
  }

  int reply = QMessageBox::question(this, questionTitle, msg, QMessageBox::No, QMessageBox::Yes);

  if (reply == QMessageBox::No)
    return;

  std::list<te::qt::widgets::AbstractTreeItem*>::const_iterator it;
  for (it = selectedLayerItems.begin(); it != selectedLayerItems.end(); ++it)
  {
    te::qt::af::evt::LayerRemoved evt((*it)->getLayer());
    m_app->triggered(&evt);
  }
}

void te::qt::af::BaseApplication::onLayerRenameTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_layerExplorer->getExplorer()->getSelectedLayerItems();

  if (selectedLayerItems.empty() ||
    (selectedLayerItems.size() == 1 && !selectedLayerItems.front()->getLayer()) ||
    selectedLayerItems.size() > 1)
  {
    QString msg = tr("Select only one layer to be renamed!");
    QMessageBox::warning(this, tr("Rename Layer"), msg);

    return;
  }

  te::qt::widgets::AbstractTreeItem* selectedLayerItem = selectedLayerItems.front();
  te::map::AbstractLayerPtr layer = selectedLayerItem->getLayer();

  bool ok;
  QString text = QInputDialog::getText(this, m_app->getAppTitle(),
    tr("Rename Layer:"), QLineEdit::Normal,
    layer->getTitle().c_str(), &ok);

  if (!ok)
    return;

  if (text.isEmpty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("Enter the new name!"));
    return;
  }

  layer->setTitle(text.toStdString());

  //te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
  //m_app->triggered(&projectUnsavedEvent);
}

void te::qt::af::BaseApplication::onLayerRemoveItemTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> selectedItems = m_layerExplorer->getExplorer()->getSelectedItems();

  std::list<te::qt::widgets::AbstractTreeItem*>::const_iterator it;
  for (it = selectedItems.begin(); it != selectedItems.end(); ++it)
  {
    te::qt::af::evt::ItemOfLayerRemoved evt((*it));
    m_app->triggered(&evt);
  }
}

void te::qt::af::BaseApplication::onLayerPropertiesTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> layers = m_layerExplorer->getExplorer()->getSelectedItems();

  if (layers.empty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("There's no selected layer."));
    return;
  }

  // Docking
  QDockWidget* doc = new QDockWidget(this, Qt::Dialog);

  te::qt::widgets::LayerPropertiesInfoWidget* info = new te::qt::widgets::LayerPropertiesInfoWidget((*(layers.begin()))->getLayer().get(), doc);

  doc->setWidget(info);
  doc->setWindowTitle(info->windowTitle());
  doc->setAttribute(Qt::WA_DeleteOnClose, true);

  doc->show();
}

void te::qt::af::BaseApplication::onLayerRemoveSelectionTriggered()
{
  std::list<te::map::AbstractLayerPtr> layers = m_layerExplorer->getExplorer()->getSelectedSingleLayers();

  if (layers.empty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("There's no selected layer."));
    return;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while (it != layers.end())
  {
    te::map::AbstractLayerPtr layer = (*it);

    if (!layer->isValid())
    {
      ++it;
      continue;
    }

    layer->clearSelected();

    ++it;

    te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
    m_app->triggered(&e);
  }
}

void te::qt::af::BaseApplication::onLayerSRSTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> layers = m_layerExplorer->getExplorer()->getSelectedItems();

  if (layers.empty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("There's no selected layer."));
    return;
  }
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if (srsDialog.exec() == QDialog::Rejected)
    return;

  std::pair<int, std::string> srid = srsDialog.getSelectedSRS();

  te::map::AbstractLayerPtr lay = (*layers.begin())->getLayer();

  lay->setSRID(srid.first);
}

void te::qt::af::BaseApplication::onLayerFitOnMapDisplayTriggered()
{
  try
  {
    std::list<te::map::AbstractLayerPtr> selectedLayers = m_layerExplorer->getExplorer()->getSelectedSingleLayers();

    if (selectedLayers.empty())
    {
      QMessageBox::warning(this, m_app->getAppTitle(),
        tr("Select a layer in the layer explorer!"));
      return;
    }
    else
    {
      std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin();

      while (it != selectedLayers.end())
      {
        if (!it->get()->isValid())
        {
          QMessageBox::warning(this, m_app->getAppTitle(),
            tr("There are invalid layers selected!"));

          return;
        }

        ++it;
      }
    }

    // The layer fitting will be accomplished only on the first layer selected
    te::map::AbstractLayerPtr selectedLayer = *(selectedLayers.begin());

    te::qt::widgets::MapDisplay* display = m_display->getDisplay();

    te::gm::Envelope env = selectedLayer->getExtent();

    if ((display->getSRID() == TE_UNKNOWN_SRS && selectedLayer->getSRID() == TE_UNKNOWN_SRS) || (display->getSRID() == selectedLayer->getSRID()))
    {
      display->setExtent(env, true);
      return;
    }

    if (display->getSRID() == TE_UNKNOWN_SRS && selectedLayer->getSRID() != TE_UNKNOWN_SRS)
    {
      display->setSRID(selectedLayer->getSRID());
      display->setExtent(env, true);
      return;
    }

    if (display->getSRID() == TE_UNKNOWN_SRS || selectedLayer->getSRID() == TE_UNKNOWN_SRS)
    {
      QMessageBox::warning(this, m_app->getAppTitle(),
        tr("The spatial reference system of the map display and the layer are not compatible!"));
      return;
    }

    if (display->getSRID() != selectedLayer->getSRID())
      env.transform(selectedLayer->getSRID(), display->getSRID());

    display->setExtent(env, true);
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
  }
}

void te::qt::af::BaseApplication::onLayerFitSelectedOnMapDisplayTriggered()
{
  std::list<te::map::AbstractLayerPtr> selectedLayers = m_layerExplorer->getExplorer()->getSelectedSingleLayers();
  if (selectedLayers.empty())
  {
    QString msg = tr("Select at least a layer to accomplish this operation!");
    QMessageBox::warning(this, m_app->getAppTitle(), msg);
    return;
  }
  else
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin();

    while (it != selectedLayers.end())
    {
      if (!it->get()->isValid())
      {
        QMessageBox::warning(this, m_app->getAppTitle(),
          tr("There are invalid layers selected!"));
        return;
      }

      ++it;
    }
  }

  te::gm::Envelope finalEnv = te::map::GetSelectedExtent(selectedLayers, m_display->getDisplay()->getSRID(), false);

  if (!finalEnv.isValid())
  {
    QString msg = tr("Select object(s) in the selected layer(s) to accomplish this operation!");
    QMessageBox::warning(this, m_app->getAppTitle(), msg);
    return;
  }

  m_display->getDisplay()->setExtent(finalEnv, true);
}

void te::qt::af::BaseApplication::onLayerPanToSelectedOnMapDisplayTriggered()
{
  std::list<te::map::AbstractLayerPtr> selectedLayers = m_layerExplorer->getExplorer()->getSelectedSingleLayers();
  if (selectedLayers.empty())
  {
    QString msg = tr("Select at least a layer to accomplish this operation!");
    QMessageBox::warning(this, m_app->getAppTitle(), msg);
    return;
  }
  else
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin();

    while (it != selectedLayers.end())
    {
      if (!it->get()->isValid())
      {
        QMessageBox::warning(this, m_app->getAppTitle(),
          tr("There are invalid layers selected!"));
        return;
      }

      ++it;
    }
  }

  te::map::MapDisplay* display = m_display->getDisplay();

  te::gm::Envelope selectedExtent = te::map::GetSelectedExtent(selectedLayers, display->getSRID(), true);

  te::gm::Coord2D centerOfSelectedExtent = selectedExtent.getCenter();

  te::gm::Envelope displayExtent = display->getExtent();

  double halfWidth = displayExtent.getWidth() * 0.5;
  double halfHeight = displayExtent.getHeight() * 0.5;

  te::gm::Envelope newExtent;

  newExtent.m_llx = centerOfSelectedExtent.x - halfWidth;
  newExtent.m_lly = centerOfSelectedExtent.y - halfHeight;

  newExtent.m_urx = centerOfSelectedExtent.x + halfWidth;
  newExtent.m_ury = centerOfSelectedExtent.y + halfHeight;

  display->setExtent(newExtent);
}


void te::qt::af::BaseApplication::onFullScreenToggled(bool checked)
{
  checked ? showFullScreen() : showMaximized();
}

void te::qt::af::BaseApplication::onLayerExplorerVisibilityChanged(bool visible)
{
  m_viewLayerExplorer->blockSignals(true);
  m_viewLayerExplorer->setChecked(visible);
  m_viewLayerExplorer->blockSignals(false);
}

void te::qt::af::BaseApplication::onDisplayDataTableChanged(bool visible)
{
  if (m_tables.empty())
    return;

  for (std::size_t i = 0; i < m_tables.size(); ++i)
  {
    if (visible)
      m_tables[i]->show();
    else
      m_tables[i]->hide();
  }

  m_viewDataTable->setChecked(visible);
}

void te::qt::af::BaseApplication::onStyleExplorerVisibilityChanged(bool visible)
{
  m_viewStyleExplorer->blockSignals(true);
  m_viewStyleExplorer->setChecked(visible);
  m_viewStyleExplorer->blockSignals(false);
}

void te::qt::af::BaseApplication::onLayerShowTableTriggered()
{
  std::list<te::qt::widgets::AbstractTreeItem*> layers = m_layerExplorer->getExplorer()->getSelectedSingleLayerItems();

  if (layers.empty())
  {
    QMessageBox::warning(this, m_app->getAppTitle(), tr("There's no selected layer."));
    return;
  }
  else
  {
    std::list<te::qt::widgets::AbstractTreeItem*>::iterator it = layers.begin();

    while (it != layers.end())
    {
      if (!(*it)->getLayer()->isValid())
      {
        QMessageBox::warning(this, m_app->getAppTitle(),
          tr("There are invalid layers selected!"));
        return;
      }

      ++it;
    }
  }


  te::map::AbstractLayerPtr lay = (*layers.begin())->getLayer();

  if (lay->getSchema()->hasRaster())
    return;

  te::qt::af::DataSetTableDockWidget* doc = getLayerDock(lay.get(), m_tables);

  if (doc == 0)
  {
    doc = new te::qt::af::DataSetTableDockWidget(this);
    doc->setLayer(lay.get());
    doc->setHighlightColor(m_app->getSelectionColor());
    addDockWidget(Qt::BottomDockWidgetArea, doc);

    connect(doc, SIGNAL(closed(te::qt::af::DataSetTableDockWidget*)), SLOT(onLayerTableClose(te::qt::af::DataSetTableDockWidget*)));
    connect(doc, SIGNAL(createChartDisplay(te::qt::widgets::ChartDisplayWidget*, te::map::AbstractLayer*)), SLOT(onChartDisplayCreated(te::qt::widgets::ChartDisplayWidget*, te::map::AbstractLayer*)));

    if (!m_tables.empty())
      tabifyDockWidget(m_tables[m_tables.size() - 1], doc);

    m_tables.push_back(doc);

    m_app->addListener(doc);
  }

  //  doc->get

  doc->show();
  doc->raise();

  m_viewDataTable->setChecked(true);

  m_viewDataTable->setEnabled(true);
}

void te::qt::af::BaseApplication::onLayerTableClose(te::qt::af::DataSetTableDockWidget* wid)
{
  std::vector<te::qt::af::DataSetTableDockWidget*>::iterator it;

  for (it = m_tables.begin(); it != m_tables.end(); ++it)
    if (*it == wid)
      break;

  if (it != m_tables.end())
  {
    m_app->removeListener(*it);
    m_tables.erase(it);
  }
}

void te::qt::af::BaseApplication::onChartDisplayCreated(te::qt::widgets::ChartDisplayWidget* chartDisplay, te::map::AbstractLayer* layer)
{
  try
  {
    te::qt::af::ChartDisplayDockWidget* doc = new te::qt::af::ChartDisplayDockWidget(chartDisplay, this);
    doc->setSelectionColor(m_app->getSelectionColor());
    doc->setWindowTitle("Histogram");
    doc->setWindowIcon(QIcon::fromTheme("chart-bar"));
    doc->setLayer(layer);
    doc->setAppController(m_app);

    m_app->addListener(doc);
    addDockWidget(Qt::RightDockWidgetArea, doc, Qt::Horizontal);
    doc->show();
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, m_app->getAppTitle(), e.what());
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
  m_layerExplorer = new LayerExplorer(m_ui->m_layerExplorer);
  m_display = new MapDisplay(m_ui->m_display, m_app);
  m_styleExplorer = new StyleExplorer(m_ui->m_styleExplorer);

  QMainWindow::setCentralWidget(m_ui->m_display);

  initMenus();

  initActions();

  initSlotsConnections();

  initStatusBar();

  m_viewLayerExplorer->setChecked(true);
  m_display->getDisplay()->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  m_viewStyleExplorer->setChecked(false);
  m_styleExplorer->getExplorer()->setVisible(false);

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
  initAction(m_viewLayerExplorer, "view-layer-explorer", "View.Layer Explorer", tr("&Layer Explorer"), tr("Show or hide the layer explorer"), true, true, true, m_menubar);
  initAction(m_viewDataTable, "view-data-table", "View.Data Table", tr("&Data Table"), tr("Show or hide the data table"), true, true, true, m_menubar);
  initAction(m_viewStyleExplorer, "style", "View.Style Explorer", tr("&Style Explorer"), tr("Show or hide the style explorer"), true, true, true, m_menubar);
  initAction(m_viewFullScreen, "view-fullscreen", "View.Full Screen", tr("F&ull Screen"), tr(""), true, true, true, m_menubar);

  initAction(m_layerShowTable, "view-data-table", "Layer.Show Table", tr("S&how Table"), tr(""), true, false, true, m_menubar);
  initAction(m_layerRemove, "layer-remove", "Project.Remove Layer", tr("&Remove Layer(s)"), tr("Remove layer(s) from the project"), true, false, true, this);
  initAction(m_layerRename, "layer-rename", "Project.Rename Layer", tr("Rename Layer..."), tr("Rename layer"), true, false, true, this);
  initAction(m_layerRemoveObjectSelection, "pointer-remove-selection", "Layer.Remove Selection", tr("&Remove Selection"), tr(""), true, false, true, m_menubar);
  initAction(m_layerRemoveItem, "item-remove", "Layer.Remove Item", tr("&Remove Item"), tr(""), true, false, true, m_menubar);
  initAction(m_layerProperties, "layer-info", "Layer.Properties", tr("&Properties..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerSRS, "layer-srs", "Layer.SRS", tr("&Inform SRS..."), tr(""), true, false, true, m_menubar);
  initAction(m_layerFitOnMapDisplay, "layer-fit", "Layer.Fit Layer on the Map Display", tr("Fit Layer"), tr("Fit the current layer on the Map Display"), true, false, true, m_menubar);
  initAction(m_layerFitSelectedOnMapDisplay, "zoom-selected-extent", "Layer.Fit Selected Features on the Map Display", tr("Fit Selected Features"), tr("Fit the selected features on the Map Display"), true, false, true, m_menubar);
  initAction(m_layerPanToSelectedOnMapDisplay, "pan-selected", "Layer.Pan to Selected Features on Map Display", tr("Pan to Selected Features"), tr("Pan to the selected features on the Map Display"), true, false, true, m_menubar);

  initAction(m_mapDraw, "map-draw", "Map.Draw", tr("&Draw"), tr("Draw the visible layers"), true, false, true, m_menubar);
  initAction(m_mapZoomIn, "zoom-in", "Map.Zoom In", tr("Zoom &In"), tr(""), true, true, true, m_menubar);
  initAction(m_mapZoomOut, "zoom-out", "Map.Zoom Out", tr("Zoom &Out"), tr(""), true, true, true, m_menubar);
  initAction(m_mapPan, "pan", "Map.Pan", tr("&Pan"), tr(""), true, true, true, m_menubar);
  initAction(m_mapZoomExtent, "zoom-extent", "Map.Zoom Extent", tr("Zoom &Extent"), tr(""), true, false, true, m_menubar);
  initAction(m_mapPreviousExtent, "edit-undo", "Map.Previous Extent", tr("&Previous Extent"), tr(""), true, false, false, m_menubar);
  initAction(m_mapNextExtent, "edit-redo", "Map.Next Extent", tr("&Next Extent"), tr(""), true, false, false, m_menubar);
  initAction(m_mapInfo, "pointer-info", "Map.Info", tr("&Info"), tr(""), true, true, true, m_menubar);
  initAction(m_mapRemoveSelection, "pointer-remove-selection", "Map.Remove Selection", tr("&Remove Selection"), tr(""), true, false, true, m_menubar);
  initAction(m_mapSelection, "pointer-selection", "Map.Selection", tr("&Selection"), tr(""), true, true, true, m_menubar);

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
  connect(m_mapDraw, SIGNAL(triggered()), SLOT(onDrawTriggered()));
  connect(m_mapZoomIn, SIGNAL(toggled(bool)), SLOT(onZoomInToggled(bool)));
  connect(m_mapZoomOut, SIGNAL(toggled(bool)), SLOT(onZoomOutToggled(bool)));
  connect(m_mapPreviousExtent, SIGNAL(triggered()), SLOT(onPreviousExtentTriggered()));
  connect(m_mapNextExtent, SIGNAL(triggered()), SLOT(onNextExtentTriggered()));
  connect(m_mapPan, SIGNAL(toggled(bool)), SLOT(onPanToggled(bool)));
  connect(m_mapZoomExtent, SIGNAL(triggered()), SLOT(onZoomExtentTriggered()));
  connect(m_mapInfo, SIGNAL(toggled(bool)), SLOT(onInfoToggled(bool)));
  connect(m_mapRemoveSelection, SIGNAL(triggered()), SLOT(onMapRemoveSelectionTriggered()));
  connect(m_mapSelection, SIGNAL(toggled(bool)), SLOT(onSelectionToggled(bool)));

  connect(m_layerRemove, SIGNAL(triggered()), SLOT(onLayerRemoveTriggered()));
  connect(m_layerRename, SIGNAL(triggered()), SLOT(onLayerRenameTriggered()));
  connect(m_layerProperties, SIGNAL(triggered()), SLOT(onLayerPropertiesTriggered()));
  connect(m_layerRemoveObjectSelection, SIGNAL(triggered()), SLOT(onLayerRemoveSelectionTriggered()));
  connect(m_layerRemoveItem, SIGNAL(triggered()), SLOT(onLayerRemoveItemTriggered()));
  connect(m_layerSRS, SIGNAL(triggered()), SLOT(onLayerSRSTriggered()));
  connect(m_layerFitOnMapDisplay, SIGNAL(triggered()), SLOT(onLayerFitOnMapDisplayTriggered()));
  connect(m_layerFitSelectedOnMapDisplay, SIGNAL(triggered()), SLOT(onLayerFitSelectedOnMapDisplayTriggered()));
  connect(m_layerPanToSelectedOnMapDisplay, SIGNAL(triggered()), SLOT(onLayerPanToSelectedOnMapDisplayTriggered()));
  connect(m_layerShowTable, SIGNAL(triggered()), SLOT(onLayerShowTableTriggered()));

  connect(m_mapSRID, SIGNAL(triggered()), SLOT(onMapSRIDTriggered()));
  connect(m_mapUnknownSRID, SIGNAL(triggered()), SLOT(onMapSetUnknwonSRIDTriggered()));
  connect(m_mapStopDrawing, SIGNAL(triggered()), SLOT(onStopDrawTriggered()));

  connect(m_viewFullScreen, SIGNAL(toggled(bool)), SLOT(onFullScreenToggled(bool)));

  connect(m_viewLayerExplorer, SIGNAL(toggled(bool)), m_layerExplorer->getExplorer(), SLOT(setVisible(bool)));
  connect(m_layerExplorer->getExplorer(), SIGNAL(visibilityChanged(bool)), this, SLOT(onLayerExplorerVisibilityChanged(bool)));

  connect(m_display, SIGNAL(hasPreviousExtent(bool)), m_mapPreviousExtent, SLOT(setEnabled(bool)));
  connect(m_display, SIGNAL(hasNextExtent(bool)), m_mapNextExtent, SLOT(setEnabled(bool)));

  connect(m_viewStyleExplorer, SIGNAL(toggled(bool)), m_styleExplorer->getExplorer(), SLOT(setVisible(bool)));
  connect(m_styleExplorer->getExplorer(), SIGNAL(visibilityChanged(bool)), this, SLOT(onStyleExplorerVisibilityChanged(bool)));
  connect(m_styleExplorer->getExplorer(), SIGNAL(repaintMapDisplay()), this, SLOT(onDrawTriggered()));

  connect(m_viewDataTable, SIGNAL(toggled(bool)), this, SLOT(onDisplayDataTableChanged(bool)));
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
