#include "BaseApplication.h"

#include "ui_BaseApplicationForm.h"

// TerraLib
#include "ApplicationController.h"
#include "connectors/LayerExplorer.h"
#include "connectors/MapDisplay.h"
#include "events/ApplicationEvents.h"
#include "events/LayerEvents.h"

#include "../widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../widgets/datasource/core/DataSourceType.h"
#include "../widgets/datasource/core/DataSourceTypeManager.h"
#include "../widgets/layer/explorer/AbstractTreeItem.h"
#include "../widgets/tools/ZoomArea.h"
#include "../widgets/tools/Pan.h"
#include "../widgets/tools/Selection.h"
#include "../widgets/layer/selector/AbstractLayerSelector.h"

#include "../../common/TerraLib.h"
#include "../../plugin/PluginManager.h"

// Qt
#include <QMessageBox>

te::qt::af::BaseApplication::BaseApplication(QWidget* parent) :
  QMainWindow(parent)
{
  m_ui = new Ui::BaseApplicationForm;
  m_ui->setupUi(this);

  QActionGroup* grp = new QActionGroup(this);
  m_ui->m_zoomInAction->setActionGroup(grp);
  m_ui->m_panAction->setActionGroup(grp);
  m_ui->m_selectToolAction->setActionGroup(grp);
}


te::qt::af::BaseApplication::~BaseApplication()
{
  delete m_ui;
  delete m_layerExplorer;
  delete m_display;

  m_app->finalize();
}

void te::qt::af::BaseApplication::init(const QString& cfgFile)
{
  try
  {
    initFramework();

    m_app->setConfigFile(cfgFile.toStdString());
    m_app->initialize();

    setWindowTitle(m_app->getAppTitle());
    setWindowIcon(QIcon(m_app->getAppIconName()));

    setWindowIconText(m_app->getAppTitle());

    m_app->initializePlugins();
  }
  catch(te::common::Exception& e)
  {
    //    QMessageBox::warning(this, "Error", e.what());
  }
}

te::qt::widgets::LayerExplorer*te::qt::af::BaseApplication::getLayerExplorer()
{
  return m_ui->m_layerExplorer;
}

void te::qt::af::BaseApplication::onAddLayerTriggered()
{
  try
  {
    //     if(m_project == 0)
    //       throw Exception(TE_TR("Error: there is no opened project!"));

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Get the parent layer where the dataset layer(s) will be added.
    te::map::AbstractLayerPtr parentLayer(0);

    std::list<te::qt::widgets::AbstractTreeItem*> selectedLayerItems = m_ui->m_layerExplorer->getSelectedLayerItems();

    if(selectedLayerItems.size() == 1 && selectedLayerItems.front()->getItemType() == "FOLDER_LAYER_ITEM")
      parentLayer = selectedLayerItems.front()->getLayer();

    // Get the layer(s) to be added
    std::auto_ptr<te::qt::widgets::DataSourceSelectorDialog> dselector(new te::qt::widgets::DataSourceSelectorDialog(this));

    //     QString dsTypeSett = GetLastDatasourceFromSettings();

    //     if(!dsTypeSett.isNull() && !dsTypeSett.isEmpty())
    //       dselector->setDataSourceToUse(dsTypeSett);

    QApplication::restoreOverrideCursor();

    int retval = dselector->exec();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if(retval == QDialog::Rejected)
    {
      QApplication::restoreOverrideCursor();
      return;
    }

    std::list<te::da::DataSourceInfoPtr> selectedDatasources = dselector->getSelecteds();

    if(selectedDatasources.empty())
    {
      QApplication::restoreOverrideCursor();
      return;
    }

    dselector.reset(0);

    const std::string& dsTypeId = selectedDatasources.front()->getType();

    const te::qt::widgets::DataSourceType* dsType = te::qt::widgets::DataSourceTypeManager::getInstance().get(dsTypeId);

    std::auto_ptr<QWidget> lselectorw(dsType->getWidget(te::qt::widgets::DataSourceType::WIDGET_LAYER_SELECTOR, this));

    if(lselectorw.get() == 0)
    {
      QApplication::restoreOverrideCursor();
      //       throw Exception((boost::format(TE_TR("No layer selector widget found for this type of data source: %1%!")) % dsTypeId).str());
    }

    te::qt::widgets::AbstractLayerSelector* lselector = dynamic_cast<te::qt::widgets::AbstractLayerSelector*>(lselectorw.get());

    if(lselector == 0)
    {
      QApplication::restoreOverrideCursor();
      //       throw Exception(TE_TR("Wrong type of object for layer selection!"));
    }

    lselector->set(selectedDatasources);

    QApplication::restoreOverrideCursor();

    std::list<te::map::AbstractLayerPtr> layers = lselector->getLayers();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    lselectorw.reset(0);

    for(std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin(); it != layers.end(); ++it)
    {
      te::qt::af::evt::LayerAdded evt(*it, parentLayer);
      emit triggered(&evt);
    }

    QApplication::restoreOverrideCursor();
  }
  catch(...)
  {

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

void te::qt::af::BaseApplication::onFitLayersTriggered()
{
  QApplication::setOverrideCursor(Qt::BusyCursor);

  m_display->fit(m_ui->m_layerExplorer->getTopLayers());

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

  te::qt::widgets::Selection* selection = new te::qt::widgets::Selection(m_display->getDisplay(), Qt::ArrowCursor, m_ui->m_layerExplorer->getSelectedSingleLayers());
  m_display->setCurrentTool(selection);

  connect(m_ui->m_layerExplorer, SIGNAL(selectedLayersChanged(const std::list<te::map::AbstractLayerPtr>&)),
          selection, SLOT(setLayers(const std::list<te::map::AbstractLayerPtr>&)));
  connect(selection, SIGNAL(layerSelectedObjectsChanged(const te::map::AbstractLayerPtr&)), SLOT(onLayerSelectionChanged(const te::map::AbstractLayerPtr&)));

  te::qt::af::evt::SelectionButtonToggled esel;
  emit triggered(&esel);
}

void te::qt::af::BaseApplication::onZoomInTriggered(bool s)
{
  if(s)
    m_display->setCurrentTool(new te::qt::widgets::ZoomArea(m_display->getDisplay(), Qt::CrossCursor, this));
}

void te::qt::af::BaseApplication::onApplicationTriggered(te::qt::af::evt::Event* e)
{

}

void te::qt::af::BaseApplication::onLayerSelectionChanged(const te::map::AbstractLayerPtr& layer)
{
  te::qt::af::evt::LayerSelectedObjectsChanged e(layer);
  emit triggered(&e);
}

void te::qt::af::BaseApplication::initFramework()
{
  m_app = &AppCtrlSingleton::getInstance();

  m_layerExplorer = new LayerExplorer(m_ui->m_layerExplorer);
  m_display = new MapDisplay(m_ui->m_display);

  m_app->addListener(m_layerExplorer);
  m_app->addListener(m_display);
  m_app->addListener(this);
}

QMenu* te::qt::af::BaseApplication::getMenuFile()
{
  return m_ui->m_menuFile;
}

QToolBar*te::qt::af::BaseApplication::getToolbar(const QString& barName)
{
  if(barName == "m_fileToolbar")
    return m_ui->m_fileToolbar;

  return 0;
}
