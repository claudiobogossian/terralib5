#include "MyWindow.h"
#include "MyLayer.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/postgis.h>
#include <terralib/qt/widgets.h>
#include <terralib/color.h>
#include <terralib/geometry.h>
#include <terralib/datatype.h>


//Qt
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>

//STL
#include <string>

MyWindow::MyWindow(QWidget* parent) : QWidget(parent),
  m_configStyle(0)
{
  setWindowTitle("My Window");

  m_ds = te::da::DataSourceFactory::make("PostGIS");
  std::string dsInfo("host=atlas.dpi.inpe.br&port=5432&dbname=terralib4&user=postgres&password=sitim110&connect_timeout=20&MaxPoolSize=15");
  m_ds->open(dsInfo);

  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  te::da::DataSourceTransactor* transactor = m_ds->getTransactor();
  te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();
//--------------------------------------------------------------------------------
// create the root layer
  m_rootFolderLayer = new te::map::FolderLayer("0", "Layers");

// create the folders
  te::map::FolderLayer *brFolderLayer = new te::map::FolderLayer("Brasil", "Brasil", m_rootFolderLayer);
  te::map::FolderLayer *goFolderLayer = new te::map::FolderLayer("Goias", "Goias", brFolderLayer);
  te::map::FolderLayer *mgFolderLayer = new te::map::FolderLayer("Minas Gerais", "Minas Gerais", brFolderLayer);
  te::map::FolderLayer *spFolderLayer = new te::map::FolderLayer("Sao Paulo", "Sao Paulo", brFolderLayer);
  te::map::FolderLayer *rjFolderLayer = new te::map::FolderLayer("Rio de Janeiro", "Rio de Janeiro", brFolderLayer);

// create the layers
  std::vector<std::string*> datasets;
  loader->getDataSets(datasets);
  int size = datasets.size();
  for(int i = 0; i < size; ++i)
  {
    std::string& id = (*datasets[i]);
    if(id.find("public.br_focos") != std::string::npos ||
      id.find("public.br_munic") != std::string::npos)
    {
      te::da::DataSetType* dst = loader->getDataSetType(id, true);
      if(catalog->getDataSetType(id) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* f = new MyLayer(id, id, brFolderLayer);
      f->setDataSource(m_ds);
      f->setSRID(gp->getSRID());

    }
    else if(id.find("public.rj_") != std::string::npos)
    {
      te::da::DataSetType* dst = loader->getDataSetType(id, true);
      if(catalog->getDataSetType(id) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* f = new MyLayer(id, id, rjFolderLayer);
      f->setDataSource(m_ds);
      f->setSRID(gp->getSRID());
    }
    else if(id.find("public.mg_") != std::string::npos)
    {
      te::da::DataSetType* dst = loader->getDataSetType(id, true);
      if(catalog->getDataSetType(id) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* f = new MyLayer(id, id, mgFolderLayer);
      f->setDataSource(m_ds);
      f->setSRID(gp->getSRID());
    }
    else if(id.find("public.goias_") != std::string::npos)
    {
      te::da::DataSetType* dst = loader->getDataSetType(id, true);
      if(catalog->getDataSetType(id) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* f = new MyLayer(id, id, goFolderLayer);
      f->setDataSource(m_ds);
      f->setSRID(gp->getSRID());
    }
    else if(id.find("public.sp_") != std::string::npos)
    {
      te::da::DataSetType* dst = loader->getDataSetType(id, true);
      if(catalog->getDataSetType(id) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* f = new MyLayer(id, id, spFolderLayer);
      f->setDataSource(m_ds);
      f->setSRID(gp->getSRID());
    }
  }


  //layer->setSRID(gp->getSRID());
//--------------------------------------------------------------------------------
//  loader->loadCatalog(true);
//  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
//  size_t size = catalog->getNumberOfDataSets();
//
//// create the layers
//  m_rootFolderLayer = new te::map::FolderLayer("0", "Layers");
//  te::map::FolderLayer *brFolderLayer = new te::map::FolderLayer("Brasil", "Brasil", m_rootFolderLayer);
//  te::map::FolderLayer *goFolderLayer = new te::map::FolderLayer("Goias", "Goias", brFolderLayer);
//  te::map::FolderLayer *mgFolderLayer = new te::map::FolderLayer("Minas Gerais", "Minas Gerais", brFolderLayer);
//  te::map::FolderLayer *spFolderLayer = new te::map::FolderLayer("Sao Paulo", "Sao Paulo", brFolderLayer);
//  for(size_t i=0; i < size; ++i)
//  {
//    te::da::DataSetType* dst = catalog->getDataSetType(i);
//    if(dst->hasGeom() == false)
//      continue;
//
//    size_t gpos = dst->getDefaultGeomPropertyPos();
//    if(gpos == 0xffffffff)
//      continue;
//
//    std::string id = dst->getName();
//
//    if(id.find("public.br_") != std::string::npos)
//    {
//      if(id.find("public.br_uf") == std::string::npos)
//      {
//        te::map::FeatureLayer* f = new te::map::FeatureLayer(id, id, brFolderLayer);
//        f->setDataSource(m_ds);
//      }
//    }
//    else if(id.find("public.sp_") != std::string::npos)
//    {
//      te::map::FeatureLayer* f = new te::map::FeatureLayer(id, id, spFolderLayer);
//      f->setDataSource(m_ds);
//    }
//    else if(id.find("public.goias_") != std::string::npos)
//    {
//      te::map::FeatureLayer* f = new te::map::FeatureLayer(id, id, goFolderLayer);
//      f->setDataSource(m_ds);
//    }
//    else if(id.find("public.mg_") != std::string::npos)
//    {
//      te::map::FeatureLayer* f = new te::map::FeatureLayer(id, id, mgFolderLayer);
//      f->setDataSource(m_ds);
//    }
//  }
  delete loader;
//  delete transactor;

// create the explorer model and set the layer tree
  m_layerExplorerModel = new te::qt::widgets::LayerExplorerModel(m_rootFolderLayer, 0);
  
// create the explorer view and set its model
  m_layerExplorer = new te::qt::widgets::LayerExplorer;
  m_layerExplorer->setModel(m_layerExplorerModel);

  QObject::connect(m_layerExplorerModel, SIGNAL(dragDropEnded(const QModelIndex&, const QModelIndex&)),
                   m_layerExplorer, SLOT(updateCurrentIndex(const QModelIndex&, const QModelIndex&)));

  m_layerExplorer->setDragEnabled(true);
  m_layerExplorer->setAcceptDrops(true);
  m_layerExplorer->setDropIndicatorShown(true);

  m_layerExplorer->setMinimumWidth(100);
  m_layerExplorer->setMaximumWidth(250);

  m_splitter = new QSplitter;
  MyDisplay* display = new MyDisplay(650, 600, m_splitter);
  m_mapDisplayVec.push_back(display);
  display->setMinimumWidth(300);
  display->setMinimumHeight(200);
  display->setLayerTree(brFolderLayer);

  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(m_layerExplorer);
  horizontalLayout->addWidget(m_splitter);

  setLayout(horizontalLayout);

  connect(m_layerExplorer, SIGNAL(contextMenuPressed(const QModelIndex&, const QPoint&)), this, SLOT(contextMenuPressed(const QModelIndex&, const QPoint&)));
  connect(m_layerExplorer, SIGNAL(checkBoxWasClicked(const QModelIndex&)), this, SLOT(layerVisibilityChanged(const QModelIndex&)));
  connect(m_layerExplorerModel, SIGNAL(dragDropEnded(const QModelIndex&, const QModelIndex&)), this, SLOT(layerItemMoved(const QModelIndex&, const QModelIndex&)));

  m_treeMenu = new QMenu(this);
  m_styleAction = new QAction("&Set Style...", m_treeMenu);
  m_treeMenu->addAction(m_styleAction);
  connect(m_styleAction, SIGNAL(triggered()), this, SLOT(setStyleSlot()));

  m_openNewMapDisplayAction = new QAction("&Open New MapDisplay...", m_treeMenu);
  m_treeMenu->addAction(m_openNewMapDisplayAction);
  connect(m_openNewMapDisplayAction, SIGNAL(triggered()), this, SLOT(openNewMapDisplaySlot()));

  m_openGridAction = new QAction("Open &Grid...", m_treeMenu);
  m_treeMenu->addAction(m_openGridAction);
  connect(m_openGridAction, SIGNAL(triggered()), this, SLOT(openGridSlot()));

  m_renameAction = new QAction("Re&name...", m_treeMenu);
  m_treeMenu->addAction(m_renameAction);
  connect(m_renameAction, SIGNAL(triggered()), this, SLOT(renameSlot()));

  m_addFolderAction = new QAction("&Add Folder...", m_treeMenu);
  m_treeMenu->addAction(m_addFolderAction);
  connect(m_addFolderAction, SIGNAL(triggered()), this, SLOT(addFolderSlot()));

  m_addLayerAction = new QAction("Add &Layer...", m_treeMenu);
  m_treeMenu->addAction(m_addLayerAction);
  connect(m_addLayerAction, SIGNAL(triggered()), this, SLOT(addLayerSlot()));

  m_removeAction = new QAction("&Remove...", m_treeMenu);
  m_treeMenu->addAction(m_removeAction);
  connect(m_removeAction, SIGNAL(triggered()), this, SLOT(removeSlot()));
}

MyWindow::~MyWindow()
{
//  delete m_rootFolderLayer; //como deletar isto???????
// acho que o LayerExplorer deveria fazer isso, mas, vou varrer a arvore e fazer aqui...
  std::vector<te::map::AbstractLayer*> layers;
  getLayers(m_rootFolderLayer, layers);
  std::vector<te::map::AbstractLayer*>::iterator lit;
  for(lit = layers.begin(); lit != layers.end(); ++lit)
  {
    MyLayer* layer = (MyLayer*)(*lit);
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op)
    {
      delete op->getDataSet()->getTransactor();
      delete op->getDataSet();
    }
    layer->setDataGridOperation(0);
  }
////////////////////////////////////////////////////////////////////////////////////////////
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
    delete (*it);

  delete m_ds;
  delete m_splitter;
}

void MyWindow::layerVisibilityChanged(const QModelIndex& /*mi*/)
{
  try
  {
    std::vector<te::map::MapDisplay*>::iterator it;
    for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
    {
      ((MyDisplay*)(*it))->draw();
      ((MyDisplay*)(*it))->update();
    }
  }
  catch(std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, tr("Error Open Grid..."), tr(e.what()));
    return;
  }
}

void MyWindow::layerItemMoved(const QModelIndex&, const QModelIndex&)
{
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
  {
    ((MyDisplay*)(*it))->draw();
    ((MyDisplay*)(*it))->update();
  }
}

void MyWindow::contextMenuPressed(const QModelIndex& mi, const QPoint& pos)
{
  m_styleAction->setEnabled(false);

  m_parentModelIndex = mi.parent();
  te::qt::widgets::AbstractLayerItem* childItem = static_cast<te::qt::widgets::AbstractLayerItem*>(mi.internalPointer());
  m_selectedLayer = childItem->getRefLayer();
  
  m_treeMenu->exec(pos);
}

void MyWindow::setStyleSlot()
{
  if(m_configStyle == 0)
    m_configStyle = new ConfigStyle(m_layerExplorer);
  m_configStyle->resize(420, 300);
  QString title = "Style Configuration: ";
  title += m_selectedLayer->getId().c_str();
  m_configStyle->setWindowTitle(title);
  m_configStyle->setLayer(m_selectedLayer);
  m_configStyle->exec();
}

void MyWindow::openNewMapDisplaySlot()
{
  MyDisplay *md = new MyDisplay(300, 250);
  md->setMinimumWidth(300);
  md->setMinimumHeight(250);
  md->setLayerTree(m_selectedLayer);
  m_mapDisplayVec.push_back(md);
  md->show();
  ((te::qt::widgets::MapDisplay*)md)->draw();
}

void MyWindow::openGridSlot()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  std::string name = m_selectedLayer->getId();
  if((m_selectedLayer->getType() == "LAYER") == false)
  {
    QMessageBox::information(this, tr("Open Grid..."), tr("ERROR: It is not a layer!"));
    QApplication::restoreOverrideCursor();
    return;
  }

  try
  {
    MyLayer* layer = (MyLayer*)m_selectedLayer;
    m_ds = layer->getDataSource();
    if(m_gridMap.find(name) == m_gridMap.end())
    {
      MyGrid* grid = new MyGrid(&m_mapDisplayVec);
      m_gridMap[name] = grid;

      te::da::DataSourceTransactor* t = m_ds->getTransactor();
      assert(t);

      te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();
      assert(loader);

      te::da::DataSetType* dsType = loader->getDataSetType(name, true);
      dsType->setCatalog(m_ds->getCatalog());
      assert(dsType);
      delete loader;

      grid->setWindowTitle(name.c_str());

      // If the data set type has geometry, get the position of the geometry column
      size_t geometryColumn;

      if(dsType->hasGeom() == true)
        geometryColumn = dsType->getDefaultGeomPropertyPos();

      te::da::DataSet* dataSet;
      te::map::DataGridOperation* operation = layer->getDataGridOperation();
      if(operation == 0)
      {
        dataSet = t->getDataSet(name);
        assert(dataSet);
        if(dsType->getPrimaryKey())
        {
          operation = new te::map::DataGridOperation();
          operation->init(dsType, dataSet);
          layer->setDataGridOperation(operation);
        }
      }
      else
        dataSet = operation->getDataSet();
      te::qt::widgets::DataGridModel* gridModel = new te::qt::widgets::DataGridModel(dsType, dataSet, operation);

      grid->setVisible(true);
      grid->setModel(gridModel);
      grid->hideColumn(geometryColumn);
      grid->viewport()->update();
      grid->show();  
  //    delete t;
    }
    else
    {
      MyGrid* grid = m_gridMap.find(name)->second;
      grid->show();  
    }
    QApplication::restoreOverrideCursor();
  }
  catch(std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, tr("Error Open Grid..."), tr(e.what()));
    return;
  }
}

void MyWindow::renameSlot()
{
  QString title = m_selectedLayer->getTitle().c_str();
  bool ok;
  QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
    tr("Rename"), QLineEdit::Normal, title, &ok);
  if (ok && !text.isEmpty())
    m_selectedLayer->setTitle(text.toStdString());
}

void MyWindow::removeSlot()
{
  size_t ind = m_selectedLayer->getIndex();
  // este metodo ja remove da interface e da memoria - TreeItem
  m_layerExplorerModel->removeRows(ind, 1, m_parentModelIndex); 

  // delete todos os DataSets e Transactors abaixo de m_selectedLayer
  std::vector<te::map::AbstractLayer*> layers;
  getLayers(m_selectedLayer, layers);
  std::vector<te::map::AbstractLayer*>::iterator lit;
  for(lit = layers.begin(); lit != layers.end(); ++lit)
  {
    MyLayer* layer = (MyLayer*)(*lit);
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op)
    {
      delete op->getDataSet()->getTransactor();
      delete op->getDataSet();
    }
    layer->setDataGridOperation(0);

    //delete o grid relacionado ao layer
    std::string name = layer->getId();
    if(m_gridMap.find(name) != m_gridMap.end())
    {
      m_gridMap.find(name)->second->close();
      delete m_gridMap.find(name)->second;
    }

    //delete o mapDisplay relacionado ao layer
    std::vector<te::map::MapDisplay*>::iterator it;
    for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
    {
      MyDisplay* md = (MyDisplay*)(*it);
      if(md->getLayerTree() == layer)
      {
        md->disconnect();
        md->close();
        delete md;
        m_mapDisplayVec.erase(it);
        it = m_mapDisplayVec.begin();
      }
    }
  }
  m_selectedLayer = 0;
}

void MyWindow::addFolderSlot()
{
  try
  {
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
      tr("Folder Name"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
      std::string name = text.toStdString();
      te::map::FolderLayer *folder = new te::map::FolderLayer(name, name, m_selectedLayer);

      QModelIndex itemIndex = m_layerExplorer->currentIndex();
      te::qt::widgets::AbstractLayerItem* item = static_cast<te::qt::widgets::AbstractLayerItem*>(itemIndex.internalPointer());
    
      size_t ind = m_layerExplorerModel->rowCount(itemIndex);
      te::qt::widgets::LayerItem* folderItem = new te::qt::widgets::LayerItem(folder);
      folderItem->setParent(item);
      std::vector<te::qt::widgets::AbstractLayerItem*> folderItems;
      folderItems.push_back(folderItem);

      m_layerExplorerModel->setItemsToBeInserted(folderItems);
      m_layerExplorerModel->insertRows(ind, 1, itemIndex);
    }
  }
  catch(std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, tr("Error Add Layer..."), tr(e.what()));
    return;
  }
}

void MyWindow::addLayerSlot()
{
  try
  {
    te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
    te::da::DataSourceTransactor* transactor = m_ds->getTransactor();
    te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();
    std::vector<std::string*> names;
    loader->getDataSets(names);
    int size = names.size();
    QStringList items;
    for(int i = 0; i < size; ++i)
    {
      std::string& id = (*names[i]);
      QString s = id.c_str();
      items.insert(i, s);
    }
    bool ok;
    QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),
                                          tr("DataSet:"), items, 0, false, &ok);
    if (ok && !item.isEmpty())
    {
      std::string name = item.toStdString();

      te::da::DataSetType* dst = loader->getDataSetType(name, true);
      if(catalog->getDataSetType(name) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* layer = new MyLayer(name, name, m_selectedLayer);
      layer->setDataSource(m_ds);
      layer->setSRID(gp->getSRID());

      QModelIndex itemIndex = m_layerExplorer->currentIndex();
      te::qt::widgets::AbstractLayerItem* item = static_cast<te::qt::widgets::AbstractLayerItem*>(itemIndex.internalPointer());
    
      size_t ind = m_layerExplorerModel->rowCount(itemIndex);
      te::qt::widgets::LayerItem* layerItem = new te::qt::widgets::LayerItem(layer);
      layerItem->setParent(item);
      std::vector<te::qt::widgets::AbstractLayerItem*> layerItems;
      layerItems.push_back(layerItem);

      m_layerExplorerModel->setItemsToBeInserted(layerItems);
      m_layerExplorerModel->insertRows(ind, 1, itemIndex);
    }
    delete loader;
    delete transactor;
  }
  catch(std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, tr("Error Add Layer..."), tr(e.what()));
    return;
  }
}

void MyWindow::getLayers(te::map::AbstractLayer* al, std::vector<te::map::AbstractLayer*>& layers)
{
  if(al->getType() == "LAYER")
    layers.push_back(al);

  te::map::AbstractLayer::iterator it;
  for(it = al->begin(); it != al->end(); ++it)
  {
    te::map::AbstractLayer* t = dynamic_cast<te::map::AbstractLayer*>(*it);
    getLayers(t, layers);
  }
}
