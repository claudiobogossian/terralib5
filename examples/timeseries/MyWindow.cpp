#include "MyWindow.h"
#include "MyLayer.h"
#include "SelectLayer.h"
#include "STExamples.h"
#include "PlotTemporalDistance.h"
#include "RasterExamples.h"
#include "STExamples.h"
#include "LineStyle.h"
#include "PointStyle.h"
#include "PolygonStyle.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/postgis.h>
#include <terralib/qt/widgets.h>
#include <terralib/color.h>
#include <terralib/geometry.h>
#include <terralib/datatype.h>
#include <terralib/memory.h>
#include <terralib/raster.h>
#include <terralib/raster/RasterSummary.h>
#include <terralib/raster/RasterSummaryManager.h>


//Qt
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QImage>
#include <QCloseEvent>
#include <QColorDialog>

//STL
#include <string>

MyWindow::MyWindow(QWidget* parent) : QWidget(parent),
  m_configStyle(0),
  m_temporalDrawingConfig(0),
  m_temporalDrawLines(false),
  m_temporalLoop(false)

{
  setWindowTitle("My Main Window - Display: Root Folder");

  te::da::DataSource* ds = te::da::DataSourceFactory::make("PostGIS");
  std::string dsInfo("host=atlas.dpi.inpe.br&port=5432&dbname=terralib4&user=postgres&password=sitim110&connect_timeout=20&MaxPoolSize=15");
  ds->open(dsInfo);

  te::da::DataSourceCatalog* catalog = ds->getCatalog();
  te::da::DataSourceTransactor* transactor = ds->getTransactor();
  te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();
//--------------------------------------------------------------------------------
// create the root layer
  m_rootFolderLayer = new te::map::FolderLayer("Root Folder", "Root Folder");

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
      f->setDataSource(ds);
      f->setSRID(gp->getSRID());

    }
    else if(id.find("public.rj_") != std::string::npos)
    {
      te::da::DataSetType* dst = loader->getDataSetType(id, true);
      if(catalog->getDataSetType(id) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* f = new MyLayer(id, id, rjFolderLayer);
      f->setDataSource(ds);
      f->setSRID(gp->getSRID());
    }
    else if(id.find("public.mg_") != std::string::npos)
    {
      te::da::DataSetType* dst = loader->getDataSetType(id, true);
      if(catalog->getDataSetType(id) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* f = new MyLayer(id, id, mgFolderLayer);
      f->setDataSource(ds);
      f->setSRID(gp->getSRID());
    }
    else if(id.find("public.goias_") != std::string::npos)
    {
      te::da::DataSetType* dst = loader->getDataSetType(id, true);
      if(catalog->getDataSetType(id) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* f = new MyLayer(id, id, goFolderLayer);
      f->setDataSource(ds);
      f->setSRID(gp->getSRID());
    }
    else if(id.find("public.sp_") != std::string::npos)
    {
      te::da::DataSetType* dst = loader->getDataSetType(id, true);
      if(catalog->getDataSetType(id) == 0)
        catalog->add(dst);
      te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
      MyLayer* f = new MyLayer(id, id, spFolderLayer);
      f->setDataSource(ds);
      f->setSRID(gp->getSRID());
    }
  }
  delete loader;
// Create MovingObjects folder
  te::map::FolderLayer *movingObjectsFolderLayer = new te::map::FolderLayer("MovingObjects", "MovingObjects", m_rootFolderLayer);

// Create MovingObjects layer 40 and 41
  ds = te::da::DataSourceFactory::make("OGR");
  ds->open("connection_string=./data/kml/t_40_41.kml");
  catalog = ds->getCatalog();
  transactor = ds->getTransactor();
  loader = transactor->getCatalogLoader();

//alterar este codigo para pegar todos os layers (por enquanto vou pegar apenas o 40 e o 41)
  std::string name = "40: locations";
  te::da::DataSetType* dst = loader->getDataSetType(name, true);
  dst->setId(2340);  // teste
  if(catalog->getDataSetType(name) == 0)
    catalog->add(dst);
  te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
  MyLayer* layer = new MyLayer(name, name, movingObjectsFolderLayer);
  layer->setTemporal(true);
  layer->setDataSource(ds);
  layer->setSRID(4291);  // teste
  //layer->setSRID(gp->getSRID());

  name = "41: locations";
  dst = loader->getDataSetType(name, true);
  dst->setId(2341);  // teste
  if(catalog->getDataSetType(name) == 0)
    catalog->add(dst);
  gp = dst->getDefaultGeomProperty();
  layer = new MyLayer(name, name, movingObjectsFolderLayer);
  layer->setTemporal(true);
  layer->setDataSource(ds);
  layer->setSRID(4291);  // teste

//NOTA: o tempo inicial e o tempo final sao calculados em TimeSlider (loadMovingObjects)
// este metodo pega todos os layers existentes debaixo da pasta MovingObjects e calcula o tempo inicial e final.
// Talves o correto seja calcular de acordo com a visibilidade dos layers temporais.

  delete loader;
  delete transactor;

  //Create TemporalImages Folder
  te::map::FolderLayer *temporalImagesFolderLayer = new te::map::FolderLayer("TemporalImages", "TemporalImages", m_rootFolderLayer);

// Create TemporalImages layers
  std::vector<MyLayer*> mylayers;
  ds = te::da::DataSourceFactory::make("GDAL");
  std::string path = "URI=";
  path += TE_DATA_EXAMPLE_LOCALE"/data/rasters";
  ds->open(path);
  catalog = ds->getCatalog();
  transactor = ds->getTransactor();
  loader = transactor->getCatalogLoader();
  datasets.clear();
  loader->getDataSets(datasets);
  size = datasets.size();
  for(int i = 0; i < size; ++i)
  {
    std::string& id = (*datasets[i]);
    if(id.find("hidro_") == std::string::npos)
      continue;
    te::da::DataSetType* dst = loader->getDataSetType(id, true);
    if(catalog->getDataSetType(id) == 0)
      catalog->add(dst);
    MyLayer* layer = new MyLayer(id, id, temporalImagesFolderLayer);
    layer->setTemporal(true);
    layer->setDataSource(ds);
    te::da::DataSet* dataSet = transactor->getDataSet(id);
    te::rst::Raster* raster = dataSet->getRaster();
    int srid = raster->getSRID();
    layer->setSRID(srid);
    te::gm::Envelope* env = raster->getExtent();
    layer->setExtent(env);
    mylayers.push_back(layer);
//    delete raster; //se deletar isto, parece que perde o Envelope????
    delete dataSet;
 }
  //Na verdade estas imagens deveriam ser geradas na projecao de visualizacao (por enquanto fica fixa nesta projecao).
  generatePNGs(mylayers);
//NOTA: o tempo inicial e o tempo final sao calculados em TimeSlider (calculateTemporalImageTimes)
// este metodo pega todos os layers existentes debaixo da pasta TemporalImages e calcula o tempo inicial e final.

  delete loader;
  delete transactor;

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

  m_displayBox = new QGroupBox(this);
  QBoxLayout *displayLayout = new QVBoxLayout(m_displayBox);
  //QVBoxLayout *displayLayout = new QVBoxLayout(m_displayBox);
  m_splitter = new QSplitter;
  displayLayout->addWidget(m_splitter);
  MyDisplay* display = new MyDisplay(650, 600, m_rootFolderLayer, m_splitter);
  m_mapDisplayVec.push_back(display);

  display->setMinimumWidth(300);
  display->setMinimumHeight(200);
  display->setLayerTree(m_rootFolderLayer);
  //faca conexao para atualizacao de grid operation
  QObject::connect(display, SIGNAL(selectionChanged(te::map::DataGridOperation*)), this, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
  QObject::connect(this, SIGNAL(selectionChanged(te::map::DataGridOperation*)), display, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));

  m_timeSlider = new TimeSlider(Qt::Horizontal);
  m_timeSlider->setMapDisplay(display);
  displayLayout->addWidget(m_timeSlider);

  m_timeSlider->setContextMenuPolicy(Qt::CustomContextMenu);
  m_timeSliderMenu = new QMenu(this);
  QAction* autoDrawingAction = new QAction("&Automatic", m_timeSliderMenu);
  QAction* manualDrawingAction = new QAction("&Manual", m_timeSliderMenu);
  QAction* configDrawingAction = new QAction("&Config...", m_timeSliderMenu);
  m_timeSliderMenu->addAction(autoDrawingAction);
  m_timeSliderMenu->addAction(manualDrawingAction);
  m_timeSliderMenu->addAction(configDrawingAction);
  connect(autoDrawingAction, SIGNAL(triggered()), this, SLOT(autoDrawingSlot()));
  connect(manualDrawingAction, SIGNAL(triggered()), this, SLOT(manualDrawingSlot()));
  connect(configDrawingAction, SIGNAL(triggered()), this, SLOT(configDrawingSlot()));
  connect(m_timeSlider, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(timeSliderContextMenu(const QPoint&)));
  m_timeSlider->loadMovingObjects();
  display->setTimeSlider(m_timeSlider);

  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(m_layerExplorer);
  horizontalLayout->addWidget(m_displayBox);

  setLayout(horizontalLayout);

  connect(m_layerExplorer, SIGNAL(contextMenuPressed(const QModelIndex&, const QPoint&)), this, SLOT(contextMenuPressed(const QModelIndex&, const QPoint&)));
  connect(m_layerExplorer, SIGNAL(checkBoxWasClicked(const QModelIndex&)), this, SLOT(layerVisibilityChanged(const QModelIndex&)));
  connect(m_layerExplorerModel, SIGNAL(dragDropEnded(const QModelIndex&, const QModelIndex&)), this, SLOT(layerItemMoved(const QModelIndex&, const QModelIndex&)));

  m_treeMenu = new QMenu(this);
  m_openNewMapDisplayAction = new QAction("&Open New MapDisplay...", m_treeMenu);
  m_treeMenu->addAction(m_openNewMapDisplayAction);
  connect(m_openNewMapDisplayAction, SIGNAL(triggered()), this, SLOT(openNewMapDisplaySlot()));

  m_openGridAction = new QAction("Open &Grid...", m_treeMenu);
  m_treeMenu->addAction(m_openGridAction);
  connect(m_openGridAction, SIGNAL(triggered()), this, SLOT(openGridSlot()));

  m_changeColorMenu = m_treeMenu->addMenu("Change Color");
  m_changeDefaultColorAction = new QAction("Change Default Color...", m_changeColorMenu);
  m_changeColorMenu->addAction(m_changeDefaultColorAction);
  connect(m_changeDefaultColorAction, SIGNAL(triggered()), this, SLOT(changeDefaultColorSlot()));
  m_changePointedColorAction = new QAction("Change Pointed Color...", m_changeColorMenu);
  m_changeColorMenu->addAction(m_changePointedColorAction);
  connect(m_changePointedColorAction, SIGNAL(triggered()), this, SLOT(changePointedColorSlot()));
  m_changeQueriedColorAction = new QAction("Change Queried Color...", m_changeColorMenu);
  m_changeColorMenu->addAction(m_changeQueriedColorAction);
  connect(m_changeQueriedColorAction, SIGNAL(triggered()), this, SLOT(changeQueriedColorSlot()));
  m_changePointedAndQueriedColorAction = new QAction("Change Pointed And Queried Color...", m_changeColorMenu);
  m_changeColorMenu->addAction(m_changePointedAndQueriedColorAction);
  connect(m_changePointedAndQueriedColorAction, SIGNAL(triggered()), this, SLOT(changePointedAndQueriedColorSlot()));

  m_changeStyleMenu = m_treeMenu->addMenu("Change Style");
  m_changePointStyleAction = new QAction("Change Point Style...", m_changeStyleMenu);
  m_changeStyleMenu->addAction(m_changePointStyleAction);
  connect(m_changePointStyleAction, SIGNAL(triggered()), this, SLOT(changePointStyleSlot()));
  m_changeLineStyleAction = new QAction("Change Line Style...", m_changeStyleMenu);
  m_changeStyleMenu->addAction(m_changeLineStyleAction);
  connect(m_changeLineStyleAction, SIGNAL(triggered()), this, SLOT(changeLineStyleSlot()));
  m_changePolygonStyleAction = new QAction("Change Polygon Style...", m_changeStyleMenu);
  m_changeStyleMenu->addAction(m_changePolygonStyleAction);
  connect(m_changePolygonStyleAction, SIGNAL(triggered()), this, SLOT(changePolygonStyleSlot()));

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

  m_keepOnMemoryAction = new QAction("&Keep Data On Memory", m_treeMenu);
  m_keepOnMemoryAction->setCheckable(true);
  m_treeMenu->addAction(m_keepOnMemoryAction);
  connect(m_keepOnMemoryAction, SIGNAL(triggered()), this, SLOT(keepOnMemorySlot()));

  m_plotTemporalDistanceAction = new QAction("&Plot Temporal Distance...", m_treeMenu);
  m_treeMenu->addAction(m_plotTemporalDistanceAction);
  connect(m_plotTemporalDistanceAction, SIGNAL(triggered()), this, SLOT(plotTemporalDistanceSlot()));
}

MyWindow::~MyWindow()
{
// delete m_rootFolderLayer; //como deletar isto???????
// acho que o LayerExplorer deveria fazer isso, mas, vou varrer a arvore e fazer aqui...
  std::set<te::da::DataSource*> dataSources;
  std::vector<te::map::AbstractLayer*> layers;
  getLayers(m_rootFolderLayer, layers);
  std::vector<te::map::AbstractLayer*>::iterator lit;
  for(lit = layers.begin(); lit != layers.end(); ++lit)
  {
    MyLayer* layer = (MyLayer*)(*lit);
    dataSources.insert(layer->getDataSource());
    delete layer; // already delete grid, plots, and dataSet

  }

// delete map displays
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
    delete (*it);

  std::set<te::da::DataSource*>::iterator di;
  for(di = dataSources.begin(); di != dataSources.end(); ++di)
    delete *di;

  delete m_splitter;
}

void MyWindow::closeEvent(QCloseEvent *event)
{
  setAttribute(Qt::WA_DeleteOnClose);
  event->accept();
}

void MyWindow::generatePNGs(std::vector<MyLayer*>& layers)
{
  // calcular min e max global para esticar os valores entre 0 e 255
  // criar lut de azul para verde para vermelho.

  double mmax = -(std::numeric_limits<double>::max());
  double mmin = -mmax;
  double ganho;

  MyLayer* layer;
  std::vector<MyLayer*>::iterator it;
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    layer = *it;
    std::string name = TE_DATA_EXAMPLE_LOCALE"/data/png/" + layer->getId();
    te::da::DataSource* ds = layer->getDataSource();
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    te::da::DataSet* dataSet = transactor->getDataSet(layer->getId());
    te::rst::Raster* raster = dataSet->getRaster();
    const te::rst::RasterSummary* rs = te::rst::RasterSummaryManager::getInstance().get(raster, te::rst::SUMMARY_ALL);
    const std::complex<double>* cmin = rs->at(0).m_minVal;
    const std::complex<double>* cmax = rs->at(0).m_maxVal;
    double min = cmin->real();
    double max = cmax->real();
    if(min < mmin)
      mmin = min;
    if(max > mmax)
      mmax = max;
  }

  ganho = 255. / (mmax - mmin);

  //criando lut
  char rlut[256], glut[256], blut[256];
  // do azul para o verde para o vermelho
  int a = 0;
  int t = 256 / 2;
  QColor icor(0, 0, 255); // blue
  QColor fcor(0, 255, 0); // to green
  double rd = (1. + (double)fcor.red() - (double)icor.red()) / (double)t;
  double gd = (1. + (double)fcor.green() - (double)icor.green()) / (double)t;
  double bd = (1. + (double)fcor.blue() - (double)icor.blue()) / (double)t;
  for(int i = 0; i < t; ++i)
  {
    rlut[a] = (int)((double)icor.red() + rd * (double)i);
    glut[a] = (int)((double)icor.green() + gd * (double)i);
    blut[a++] = (int)((double)icor.blue() + bd * (double)i);
  }
  icor = fcor; // green
  fcor = QColor(255, 0, 0); // to red
  rd = (1. + (double)fcor.red() - (double)icor.red()) / (double)t;
  gd = (1. + (double)fcor.green() - (double)icor.green()) / (double)t;
  bd = (1. + (double)fcor.blue() - (double)icor.blue()) / (double)t;
  for(int i = 0; i < t; ++i)
  {
    rlut[a] = (int)((double)icor.red() + rd * (double)i);
    glut[a] = (int)((double)icor.green() + gd * (double)i);
    blut[a++] = (int)((double)icor.blue() + bd * (double)i);
  }
  while(a < 256)
  {
    rlut[a] = fcor.red();
    glut[a] = fcor.green();
    blut[a++] = fcor.blue();
  }

  //create png dir
  QDir dir(TE_DATA_EXAMPLE_LOCALE"/data");
  if(dir.exists("png") == false)
    dir.mkdir("png");

  //generate PNGs
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    layer = *it;

    std::string namePNG = layer->getId();
    size_t p = namePNG.find(".tif");
    namePNG.erase(p, 4);
    namePNG += ".PNG";
    std::string filePNG = TE_DATA_EXAMPLE_LOCALE"/data/png/" + namePNG;

    // o codigo abaixo nao pega os valores do raster - porque nao funciona?????
    //te::da::DataSource* ds = layer->getDataSource();
    //te::da::DataSourceTransactor* transactor = ds->getTransactor();
    //te::da::DataSet* dataSet = transactor->getDataSet(layer->getId());
    //te::rst::Raster* raster = dataSet->getRaster();

    // usando o codigo abaixo para pegar os valores do raster - porque assim funciona?????
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/" + layer->getId();
    te::rst::Raster* raster = te::rst::RasterFactory::open(std::string("MEM"), rinfo);
    int ncols = raster->getNumberOfColumns();
    int nrows = raster->getNumberOfRows();
    te::rst::Band* band0 = raster->getBand(0);
    te::rst::BandProperty* prop = band0->getProperty();
    int hsize = prop->m_blkw;
    int vsize = prop->m_blkh;
    int numblocksx = prop->m_nblocksx;
    int numblocksy = prop->m_nblocksy;  

    int size = ncols * nrows;
    double* buf = new double[size];
    band0->read(0, 0, buf);

    QImage ima(ncols, nrows, QImage::Format_ARGB32);

    int k = 0;
    for(int j = 0; j < nrows; ++j)
    {
      unsigned char* c = ima.scanLine(j);
      for(int i = 0; i < ncols; ++i)
      {
        double v = buf[k];
        v *= ganho;
        c[i<<2] = blut[(int)v]; //blue
        c[(i<<2) + 1] = glut[(int)v]; //green
        c[(i<<2) + 2] = rlut[(int)v]; //red
        if((int)v == 0)
          c[(i<<2) + 3] = 0; //transparent
        else
          c[(i<<2) + 3] = 150; // meio transparente
        ++k;
      }
    }

    bool b = ima.save(filePNG.c_str(), "PNG", 80);
  }
}


void MyWindow::layerVisibilityChanged(const QModelIndex& mi)
{
  try
  {
    te::qt::widgets::AbstractLayerItem* childItem = static_cast<te::qt::widgets::AbstractLayerItem*>(mi.internalPointer());
    te::map::AbstractLayer* al = childItem->getRefLayer();
    if(al->getType() == "FOLDERLAYER")
    {
      std::vector<te::map::AbstractLayer*> layers;
      getLayers(al, layers);
      if(layers.empty())
      {
        if(al->getVisibility() == 1)
          QMessageBox::information(this, tr("Folder is empty"), tr("EMPTY FOLDER"));
        al->setVisibility(te::map::NOT_VISIBLE);
        m_layerExplorer->update();
        return; // nao ha' layer debaixo do folder
      }
      al = *layers.begin();
    }

    //redesenhar os displays afetados
    std::set<te::map::MapDisplay*> displays;
    std::vector<te::map::MapDisplay*>::iterator it;
    for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
    {
      std::vector<te::map::AbstractLayer*> layers;
      getLayers((*it)->getLayerTree(), layers);
      std::vector<te::map::AbstractLayer*>::iterator lit;
      for(lit = layers.begin(); lit != layers.end(); ++lit)
      {
        if(al == *lit)
        {
          displays.insert(*it);
          break;
        }
      }
    }

    std::set<te::map::MapDisplay*>::iterator sit;
    for(sit = displays.begin(); sit != displays.end(); ++sit)
    {
      ((MyDisplay*)(*sit))->draw();
      ((MyDisplay*)(*sit))->update();
    }

    // acabou de mudar para (0 = not visible, 1 = visible, 2 = meio visible)
    int visible = al->getVisibility();
    if(visible == 0)
    {
      // todos os layers abaixo deste abstract layer ficaram nao visivel
      // verifique se esses layers nao estao sendo usados.
      // Delete grid operation dos layers que nao estao sendo utilizados
      std::vector<te::map::AbstractLayer*> layers;
      getLayers(al, layers);
      std::vector<te::map::AbstractLayer*>::iterator lit;
      for(lit = layers.begin(); lit != layers.end(); ++lit)
      {
        MyLayer* mlayer = (MyLayer*)(*lit);
        // este display deixou de usar este layer (apagou o desenho do layer)
        // se nenhum widget mais estiver usando este layer delete grid operation
        if(isUsed(mlayer) == false)
          deleteGridOperation(mlayer);
      }
    }

    //parar o desenho dos layers temporais e reinicializar o vector de layers temporais no time Slider
    manualDrawingSlot();
    m_timeSlider->removeAllLayers();
    std::vector<te::map::AbstractLayer*> layers;
    getLayers(m_rootFolderLayer, layers);
    std::vector<te::map::AbstractLayer*>::iterator lit;
    for(lit = layers.begin(); lit != layers.end(); ++lit)
    {
      MyLayer* layer = (MyLayer*)(*lit);
      if(layer->isTemporal())
      {
        if(layer->getVisibility() == te::map::VISIBLE)
          m_timeSlider->addLayer(layer);
      }
    }
  }
  catch(std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, tr("Error Open Grid..."), tr(e.what()));
    return;
  }
}

void MyWindow::layerItemMoved(const QModelIndex& mi, const QModelIndex& mf)
{
  te::qt::widgets::AbstractLayerItem* item = static_cast<te::qt::widgets::AbstractLayerItem*>(mi.internalPointer());
  te::map::AbstractLayer* al = item->getRefLayer();
  if(al->getVisibility() == 0)
    return;

  te::map::AbstractLayer* parent = (te::map::AbstractLayer*)al->getParent();

  bool modif = false;
  int ini = mi.row();
  int fim = mf.row();
  int inc = 1;
  if(ini > fim)
    inc = -1;
  for(int i = ini; i != fim; i += inc)
  {
    te::map::AbstractLayer* l = (te::map::AbstractLayer*)parent->getChild(i);
    if(l->getVisibility() != 0)
    {
      modif = true;
      break;
    }
  }
  if(modif == false)
    return;

  //redesenhar os displays afetados
  std::set<te::map::MapDisplay*> displays;
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
  {
    std::vector<te::map::AbstractLayer*> layers;
    getLayers((*it)->getLayerTree(), layers);
    std::vector<te::map::AbstractLayer*>::iterator lit, lit2;
    for(lit = layers.begin(); lit != layers.end(); ++lit)
    {
      if(al == *lit)
      {
        displays.insert(*it);
        break;
      }
    }
  }

  std::set<te::map::MapDisplay*>::iterator sit;
  for(sit = displays.begin(); sit != displays.end(); ++sit)
  {
    std::vector<te::map::AbstractLayer*> layers;
    getLayers((*sit)->getLayerTree(), layers);
    MyDisplay* display = (MyDisplay*)*sit;
    display->reorderDrawing(layers);
    //((MyDisplay*)(*sit))->draw();
    ((MyDisplay*)(*sit))->update();
  }
}

void MyWindow::timeSliderContextMenu(const QPoint& pos)
{
  QPoint p = mapToGlobal(m_displayBox->pos()) + m_timeSlider->pos() + pos;
  m_timeSliderMenu->exec(p);
}

void MyWindow::autoDrawingSlot()
{
  if(m_temporalDrawingConfig == 0)
    configDrawingSlot();

  manualDrawingSlot();
  //carregar os moving objects: 40 e 41
  m_timeSlider->loadMovingObjects();

  //te::dt::TimeInstant* ini = new te::dt::TimeInstant(te::dt::Date(2000, 1, 1), te::dt::TimeDuration(3, 10, 20));
  //te::dt::TimeInstant* fim = new te::dt::TimeInstant(te::dt::Date(2001, 3, 16), te::dt::TimeDuration(13, 20, 30));
  //m_timeSlider->setInitialDateTime(ini);
  //m_timeSlider->setFinalDateTime(fim);
  m_timeSlider->setMinuteInterval(m_minuteInterval);
  m_timeSlider->setLines(m_temporalDrawLines);
  m_timeSlider->setLoop(m_temporalLoop);
  m_timeSlider->startTimer(m_temporalDrawingInterval);
}

void MyWindow::manualDrawingSlot()
{
  m_timeSlider->killTimer();
  m_timeSlider->backToInit();
}

void MyWindow::configDrawingSlot()
{
  if(m_temporalDrawingConfig == 0)
    m_temporalDrawingConfig = new TemporalDrawingConfig(this);

  if(m_temporalDrawingConfig->exec() == QDialog::Rejected)
    return;
  m_minuteInterval = 15;
  m_dateInterval = 0;

  int index = m_temporalDrawingConfig->m_intervalDateComboBox->currentIndex();
  if(index == 0)
    m_minuteInterval = 15;
  else if(index == 1)
    m_minuteInterval = 30;
  else if(index == 2)
    m_minuteInterval = 60;
  else
    m_minuteInterval = (index - 2) * 24 * 60;

  index = m_temporalDrawingConfig->m_intervalDrawingComboBox->currentIndex();
  m_temporalDrawingInterval = (index + 1) * 100;
  m_temporalDrawLines = m_temporalDrawingConfig->m_drawLinesCheckBox->isChecked();
  m_temporalLoop = m_temporalDrawingConfig->m_loopCheckBox->isChecked();

  autoDrawingSlot();
}

void MyWindow::timeSliderValueChangedSlot(int)
{

}

void MyWindow::contextMenuPressed(const QModelIndex& mi, const QPoint& pos)
{
  //teste m_styleAction->setEnabled(false);
  m_parentModelIndex = mi.parent();
  te::qt::widgets::AbstractLayerItem* childItem = static_cast<te::qt::widgets::AbstractLayerItem*>(mi.internalPointer());
  m_selectedLayer = childItem->getRefLayer();

  //if(m_selectedLayer->getType() == "DATASETLAYER")
  QString title = m_selectedLayer->getTitle().c_str();
  QString title2;
  te::map::AbstractLayer* al = (te::map::AbstractLayer*)m_selectedLayer->getParent();
  if(al)
    title2 = al->getTitle().c_str();

  if(title.toUpper() == "MOVINGOBJECTS")
    m_plotTemporalDistanceAction->setEnabled(true);
  else
    m_plotTemporalDistanceAction->setEnabled(false);

  if(title.toUpper() == "MOVINGOBJECTS" || title2.toUpper() == "MOVINGOBJECTS" || 
     title.toUpper() == "TEMPORALIMAGES" || title2.toUpper() == "TEMPORALIMAGES")
  {
    m_openNewMapDisplayAction->setEnabled(false);
    m_addFolderAction->setEnabled(false);
    m_keepOnMemoryAction->setEnabled(false);
    m_keepOnMemoryAction->setChecked(false);
  }
  else
  {
    m_openNewMapDisplayAction->setEnabled(true);

    if(m_selectedLayer->getType() == "FOLDERLAYER")
    {
      m_keepOnMemoryAction->setEnabled(false);
      m_keepOnMemoryAction->setChecked(false);
      m_addFolderAction->setEnabled(true);
      m_changeColorMenu->setEnabled(false);
      m_changeStyleMenu->setEnabled(false);
    }
    else
    {
      MyLayer* layer = (MyLayer*)m_selectedLayer;
      m_keepOnMemoryAction->setEnabled(true);
      m_keepOnMemoryAction->setChecked(layer->isKeepOnMemory());
      m_addFolderAction->setEnabled(false);
      te::map::DataGridOperation* op = layer->getDataGridOperation();
      if(op)
      {
        m_changeColorMenu->setEnabled(true);
        m_changeStyleMenu->setEnabled(true);
        QColor qcor;
        QIcon icon;
        QPixmap p(20,15);
        te::color::RGBAColor cor;
        cor = op->getDefaultColor();
        qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue(), cor.getAlpha());
        p.fill(qcor);
        icon = QIcon(p);
        m_changeDefaultColorAction->setIcon(icon);

        cor = op->getPointedColor();
        qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue(), cor.getAlpha());
        p.fill(qcor);
        icon = QIcon(p);
        m_changePointedColorAction->setIcon(icon);

        cor = op->getQueriedColor();
        qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue(), cor.getAlpha());
        p.fill(qcor);
        icon = QIcon(p);
        m_changeQueriedColorAction->setIcon(icon);

        cor = op->getPointedAndQueriedColor();
        qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue(), cor.getAlpha());
        p.fill(qcor);
        icon = QIcon(p);
        m_changePointedAndQueriedColorAction->setIcon(icon);
      }
      else
      {
        m_changeColorMenu->setEnabled(false);
        m_changeStyleMenu->setEnabled(false);
      }
    }
  }
  
  m_treeMenu->exec(pos);
}

void MyWindow::setStyleSlot()
{
  //if(m_configStyle == 0)
  //  m_configStyle = new ConfigStyle(m_layerExplorer);
  //m_configStyle->resize(420, 300);
  //QString title = "Style Configuration: ";
  //title += m_selectedLayer->getId().c_str();
  //m_configStyle->setWindowTitle(title);
  //m_configStyle->setLayer(m_selectedLayer);
  //m_configStyle->exec();

  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters/hidro_3_20091231000000.tif";

  te::rst::Raster* memraster = te::rst::RasterFactory::open(std::string("MEM"), rinfo);
  std::size_t n = memraster->getNumberOfBands();
  int type0 = memraster->getBandDataType(0);
  int ncols = memraster->getNumberOfColumns();
  int nrows = memraster->getNumberOfRows();
  QImage ima(ncols, nrows, QImage::Format_ARGB32);
  double resx = memraster->getResolutionX();
  double resy = memraster->getResolutionY();
  int srid = memraster->getSRID();
  te::gm::Envelope* env = memraster->getExtent();

  te::rst::Band* band0 = memraster->getBand(0);
  te::rst::BandProperty* prop = band0->getProperty();
  int hsize = prop->m_blkw;
  int vsize = prop->m_blkh;
  int numblocksx = prop->m_nblocksx;
  int numblocksy = prop->m_nblocksy;  

  int size = ncols * nrows;
  double* buf = new double[size];
  band0->read(0, 0, buf);

  const te::rst::RasterSummary* rs = te::rst::RasterSummaryManager::getInstance().get(memraster, te::rst::SUMMARY_ALL);

  const std::complex<double>* cmin = rs->at(0).m_minVal;
  const std::complex<double>* cmax = rs->at(0).m_maxVal;
  double min = cmin->real();
  double max = cmax->real();

  double g = 255 / max;

  //criando lut
  char rlut[256], glut[256], blut[256];
  // do vermelho para o amarelo para o azul para o cyan
  int a = 0;
  int t = 256 / 3;
  QColor icor(255, 0, 0);
  QColor fcor(255, 255, 0);
  double rd = (1. + (double)fcor.red() - (double)icor.red()) / (double)t;
  double gd = (1. + (double)fcor.green() - (double)icor.green()) / (double)t;
  double bd = (1. + (double)fcor.blue() - (double)icor.blue()) / (double)t;
  for(int i = 0; i < t; ++i)
  {
    rlut[a] = (int)((double)icor.red() + rd * (double)i);
    glut[a] = (int)((double)icor.green() + gd * (double)i);
    blut[a++] = (int)((double)icor.blue() + bd * (double)i);
  }
  icor = fcor;
  fcor = QColor(0, 0, 255);
  rd = (1. + (double)fcor.red() - (double)icor.red()) / (double)t;
  gd = (1. + (double)fcor.green() - (double)icor.green()) / (double)t;
  bd = (1. + (double)fcor.blue() - (double)icor.blue()) / (double)t;
  for(int i = 0; i < t; ++i)
  {
    rlut[a] = (int)((double)icor.red() + rd * (double)i);
    glut[a] = (int)((double)icor.green() + gd * (double)i);
    blut[a++] = (int)((double)icor.blue() + bd * (double)i);
  }
  icor = fcor;
  fcor = QColor(0, 255, 255);
  rd = (1. + (double)fcor.red() - (double)icor.red()) / (double)t;
  gd = (1. + (double)fcor.green() - (double)icor.green()) / (double)t;
  bd = (1. + (double)fcor.blue() - (double)icor.blue()) / (double)t;
  for(int i = 0; i < t; ++i)
  {
    rlut[a] = (int)((double)icor.red() + rd * (double)i);
    glut[a] = (int)((double)icor.green() + gd * (double)i);
    blut[a++] = (int)((double)icor.blue() + bd * (double)i);
  }
  while(a < 256)
  {
    rlut[a] = fcor.red();
    glut[a] = fcor.green();
    blut[a++] = fcor.blue();
  }

  int k = 0;
  for(int j = 0; j < nrows; ++j)
  {
    unsigned char* c = ima.scanLine(j);
    for(int i = 0; i < ncols; ++i)
    {
      c[i<<2] = blut[(int)(buf[k] * g)]; //blue
      c[(i<<2) + 1] = glut[(int)(buf[k] * g)]; //green
      c[(i<<2) + 2] = rlut[(int)(buf[k] * g)]; //red
      if(buf[k] == 0)
        c[(i<<2) + 3] = 0; //transparent
      else
        c[(i<<2) + 3] = 200; // meio transparente
      ++k;
    }
  }

  bool b = ima.save("C:/lixo/imagemKarine.PNG", "PNG", 80);

  //........usando Canvas ................................................................
  char *bbuf = new char[size];
  FILE* fp = fopen("C:/lixo/imagemKarine.PNG", "rb");
  size_t bsize = fread(bbuf, sizeof(char), size, fp);

  te::qt::widgets::Canvas canvas(ncols * 5, nrows * 5);
  canvas.drawImage(0, 0, ncols * 5, nrows * 5, bbuf, bsize, te::map::PNG);
  canvas.save("C:/lixo/imagemKerineCopiaEmZoom.PNG", te::map::PNG, 80);

  fclose(fp);
  delete []bbuf;
}

void MyWindow::openNewMapDisplaySlot()
{
  MyDisplay *md = new MyDisplay(300, 250, m_rootFolderLayer);
  QString wtitle = "Display: ";
  wtitle += m_selectedLayer->getTitle().c_str();
  md->setWindowTitle(wtitle);
  md->setMinimumWidth(300);
  md->setMinimumHeight(250);
  md->setLayerTree(m_selectedLayer);
  m_mapDisplayVec.push_back(md);

  //faca conexao para atualizacao de grid operation
  QObject::connect(md, SIGNAL(selectionChanged(te::map::DataGridOperation*)), this, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
  QObject::connect(this, SIGNAL(selectionChanged(te::map::DataGridOperation*)), md, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));

  //faca conexao para remover o display do m_mapDisplayVec quando ele for closed
  QObject::connect(md, SIGNAL(closed(MyDisplay*)), this, SLOT(removeDisplaySlot(MyDisplay*)));

  md->show();
  ((te::qt::widgets::MapDisplay*)md)->draw();
}

void MyWindow::removeDisplaySlot(MyDisplay* d)
{
  // quais sao os layers que estavam sendo visualizados
  std::vector<te::map::AbstractLayer*>layers;
  getLayers(d->getLayerTree(), layers);
  std::vector<te::map::AbstractLayer*>::iterator lit;
  for(lit = layers.begin(); lit != layers.end(); ++lit)
  {
    MyLayer* layer = (MyLayer*)(*lit);
    //layer esta' sendo usado? grid operation pode ser deletado?
    if(isUsed(layer) == false)
      deleteGridOperation(layer);
  }

  // remova do mapa
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
  {
    MyDisplay* display = (MyDisplay*)*it;
    if(d == display)
    {
      m_mapDisplayVec.erase(it);
      break;
    }
  }
}

void MyWindow::openGridSlot()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  if((m_selectedLayer->getType() == "LAYER") == false)
  {
    QMessageBox::information(this, tr("Open Grid..."), tr("ERROR: It is not a layer!"));
    QApplication::restoreOverrideCursor();
    return;
  }

  try
  {
    MyLayer* layer = (MyLayer*)m_selectedLayer;
    MyGrid* grid = layer->getGrid();

    if(grid == 0)
    {
      layer->createGrid();
      QString wtitle = "Grid: ";
      wtitle += m_selectedLayer->getTitle().c_str();
      grid = layer->getGrid();
      grid->setWindowTitle(wtitle);
      connect(grid, SIGNAL(plotHistogram(MyGrid*)), this, SLOT(plotHistogramSlot(MyGrid*)));
      connect(grid, SIGNAL(plotScatter(MyGrid*)), this, SLOT(plotScatterSlot(MyGrid*)));
      connect(grid, SIGNAL(plotTimeSeries(MyGrid*)), this, SLOT(plotTimeSeriesSlot(MyGrid*)));

      //faca conexao para atualizacao de grid operation
      QObject::connect(grid, SIGNAL(selectionChanged(te::map::DataGridOperation*)), this, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
      QObject::connect(this, SIGNAL(selectionChanged(te::map::DataGridOperation*)), grid, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));

      //faca conexao para remover este grid quando ele for closed
      connect(grid, SIGNAL(closed(MyGrid*)), this, SLOT(removeGridSlot(MyGrid*)));
    }
    else
      grid->raise();  

    QApplication::restoreOverrideCursor();
  }
  catch(std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, tr("Error Open Grid..."), tr(e.what()));
    return;
  }
}

void MyWindow::removeGridSlot(MyGrid* g)
{
  MyLayer* layer = g->getLayer();
  layer->deleteGrid(false);

  if(isUsed(layer) == false)
    deleteGridOperation(layer);
}

void MyWindow::deleteGridOperation(te::map::AbstractLayer* l)
{
  MyLayer* layer = (MyLayer*)l;
  te::map::DataGridOperation* op = layer->getDataGridOperation();
  if(op && layer->isKeepOnMemory() == false)
  {
    delete op->getDataSet()->getTransactor();
    delete op->getDataSet();
    delete op;
    layer->setDataGridOperation(0);

    std::vector<te::map::MapDisplay*>::iterator it;
    for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
    {
      std::vector<te::map::AbstractLayer*> layers;
      getLayers((*it)->getLayerTree(), layers);
      std::vector<te::map::AbstractLayer*>::iterator lit;
      for(lit = layers.begin(); lit != layers.end(); ++lit)
      {
        if(layer == *lit)
        {
          ((MyDisplay*)(*it))->removeDrawOnlyChanged(layer);
          break;
        }
      }
    }
  }
}

bool MyWindow::isUsed(te::map::AbstractLayer* l)
{
  MyLayer* layer = (MyLayer*)l;
  if(layer->getVisibility() == 0 && layer->getGrid() == 0)
  {
    std::set<QwtPlot*> plots = layer->getPlots();
    if(plots.empty())
      return false;
    else
    {
      std::set<QwtPlot*>::iterator it;
      for(it = plots.begin(); it != plots.end(); ++it)
      {
        te::qt::qwt::Plot* p = (te::qt::qwt::Plot*)(*it);
        if(p->getType() != "TIMESERIES")
          return true;
      }
      return false;
    }
  }
  return true;
}

void MyWindow::plotHistogramSlot(MyGrid* grid)
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  MyLayer* layer = grid->getLayer();
  te::map::DataGridOperation* operation = layer->getDataGridOperation();
  te::qt::widgets::HeaderView* header = (te::qt::widgets::HeaderView*)grid->getHorizontalHeaderView();
  int visCol = header->getContextVisualColumnClicked();
  int col = operation->getLogicalColumn(visCol);

  // verifique se um plot com esse parametro ja esta aberto
  std::set<QwtPlot*> plots = layer->getPlots();
  std::set<QwtPlot*>::iterator it;
  for(it = plots.begin(); it != plots.end(); ++it)
  {
    te::qt::qwt::Plot* plot = (te::qt::qwt::Plot*)(*it);
    if(plot->getType() == "HISTOGRAM")
    {
      if(plot->getXCol() == col) 
      {
        // histograma ja aberto (apenas update)
        plot->update();
        plot->raise();
        break;
      }
    }
  }
    
  if(it == plots.end()) // criar novo plot (histograma)
  {
    te::qt::qwt::HistogramDisplay* h = new te::qt::qwt::HistogramDisplay(col, operation);
    if(h->getNumberOfBars() == 0) // histogram nao pode ser criado
    {
      delete h;
      return;
    }

    te::da::DataSetType* dsType = operation->getDataSetType();
    te::dt::Property *prop = dsType->getProperty(col);
    std::string propName = prop->getName();
    std::string wtitle = "Histogram: " + propName;

    h->setWindowTitle(wtitle.c_str());
    h->show();

    //faca conexao para atualizacao de grid operation
    QObject::connect(h, SIGNAL(selectionChanged(te::map::DataGridOperation*)), this, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    QObject::connect(this, SIGNAL(selectionChanged(te::map::DataGridOperation*)), h, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));

    //faca conexao para remover este HistogramDisplay* quando ele for closed
    QObject::connect(h, SIGNAL(closed(QwtPlot*)), this, SLOT(removePlotSlot(QwtPlot*)));

    layer->insertPlot(h);
  }
  QApplication::restoreOverrideCursor();
}

void MyWindow::plotScatterSlot(MyGrid* grid)
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  MyLayer* layer = grid->getLayer();
  te::map::DataGridOperation* operation = layer->getDataGridOperation();
  te::da::DataSetType* dsType = operation->getDataSetType();

  std::vector<int> selectedColumns = operation->getSelectedColumns();
  if(selectedColumns.size() < 2)
  {
    QMessageBox::information(this, tr("Scatter Plot"), tr("Select 2 columns in the grid to be plotted!"));
    QApplication::restoreOverrideCursor();
    return;
  }

  int logicalColumnX = operation->getLogicalColumn(selectedColumns[0]);
  std::string colNameX = dsType->getProperty(logicalColumnX)->getName();

  int logicalColumnY = operation->getLogicalColumn(selectedColumns[1]);
  std::string colNameY = dsType->getProperty(logicalColumnY)->getName();

  std::string name = dsType->getName();
  std::string scatname = colNameX + " vs " + colNameY;

  // verifique se um plot com esses parametros ja esta aberto
  std::set<QwtPlot*> plots = layer->getPlots();
  std::set<QwtPlot*>::iterator it;
  for(it = plots.begin(); it != plots.end(); ++it)
  {
    te::qt::qwt::Plot* plot = (te::qt::qwt::Plot*)(*it);
    if(plot->getType() == "SCATTER")
    {
      if(plot->getXCol() == logicalColumnX && plot->getYCol() == logicalColumnY) 
      {
        // scatter ja aberto (apenas update)
        plot->update();
        plot->raise();
        break;
      }
    }
  }
    
  if(it == plots.end()) // criar novo plot (scatter)
  {
    te::qt::qwt::ScatterDisplay* esc = new te::qt::qwt::ScatterDisplay(logicalColumnX, logicalColumnY, operation);
    if(esc->getLegend() == 0) // scatter nao pode ser criado
    {
      delete esc;
      return;
    }
    std::string wtitle = "Scatter: " + scatname;
    esc->setWindowTitle(wtitle.c_str());
    esc->show();

    //faca conexao para atualizacao de grid operation
    QObject::connect(esc, SIGNAL(selectionChanged(te::map::DataGridOperation*)), this, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    QObject::connect(this, SIGNAL(selectionChanged(te::map::DataGridOperation*)), esc, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));

    //faca conexao para remover este ScatterDisplay* quando ele for closed
    QObject::connect(esc, SIGNAL(closed(QwtPlot*)), this, SLOT(removePlotSlot(QwtPlot*)));

    layer->insertPlot(esc);
  }
  QApplication::restoreOverrideCursor();
}

void MyWindow::plotTimeSeriesSlot(MyGrid* grid)
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  MyLayer* layer = grid->getLayer();
  te::map::DataGridOperation* operation = layer->getDataGridOperation();
  te::da::DataSet* dataSet = operation->getDataSet();
  te::da::DataSetType* dsType = operation->getDataSetType();

  std::vector<int> selectedColumns = operation->getSelectedColumns();
  if(selectedColumns.size() < 2)
  {
    QMessageBox::information(this, tr("Time Series Plot"), tr("Select 2 columns or more to be plotted!"));
    QApplication::restoreOverrideCursor();
    return;
  }

  int i, xTimeType, xType, logicalColumnX = -1;
  std::string colNameX;
  std::vector<std::pair<std::string, int> >logicalColumns;

  // verifique se existe uma coluna do tipo date time
  for(i = 0; i < (int)selectedColumns.size(); ++i)
  {
    int col = operation->getLogicalColumn(selectedColumns[i]);
    std::string colName = dsType->getProperty(col)->getName();

    te::dt::Property *prop = dsType->getProperty(col);
    int type = prop->getType();
    if(logicalColumnX == -1 && type == te::dt::DATETIME_TYPE)
    {
      logicalColumnX = col;
      xType = type;
      colNameX = colName;
      dataSet->moveBeforeFirst();
      while(dataSet->moveNext())
      {
        if(dataSet->isNull(logicalColumnX))
          continue;
        te::dt::DateTime* t = dataSet->getDateTime(logicalColumnX);
        xTimeType = t->getDateTimeType();
        break;
      }
      continue;
    }
    logicalColumns.push_back(std::pair<std::string, int>(colName, col));
  }
  if(logicalColumnX == -1)
  {
    QMessageBox::information(this, tr("Time Series Plot Error"), tr("One of the columns must be of type date time!"));
    QApplication::restoreOverrideCursor();
    return;
  }
  if(selectedColumns.size() == 0)
  {
    QMessageBox::information(this, tr("Time Series Plot Error"), tr("Select columns to be ploted!"));
    QApplication::restoreOverrideCursor();
    return;
  }
  else
  {
    while(++i < (int)selectedColumns.size())
    {
      int a = operation->getLogicalColumn(selectedColumns[i]);
      std::string s = dsType->getProperty(a)->getName();
      logicalColumns.push_back(std::pair<std::string, int>(s, a));
      ++i;
    }
  }

  // verifique se um plot com esses parametros ja esta aberto
  std::set<QwtPlot*> plots = layer->getPlots();
  std::set<QwtPlot*>::iterator it;
  for(it = plots.begin(); it != plots.end(); ++it)
  {
    te::qt::qwt::Plot* plot = (te::qt::qwt::Plot*)(*it);
    if(plot->getType() == "TIMESERIES")
    {
      if(plot->getXCol() != logicalColumnX)
        continue;
      te::qt::qwt::TimeSeriesDisplay* t = (te::qt::qwt::TimeSeriesDisplay*)plot;
      std::set<int> cols = t->getColumns();
      std::vector<std::pair<std::string, int> >::iterator vit;
      if(cols.size() == logicalColumns.size())
      {
        for(vit = logicalColumns.begin(); vit != logicalColumns.end(); ++vit)
        {
          if(cols.find(vit->second) == cols.end())
            break;
        }
        if(vit == logicalColumns.end())
        {
          // scatter ja aberto (apenas update)
          t->update();
          t->raise();
          break;
        }
      }
    }
  }

  if(it == plots.end())
  {
    //criar nova serie temporal
    std::vector<std::pair<std::string, int> >::iterator cit;
    te::qt::qwt::TimeSeries ts(xTimeType);
    for(cit = logicalColumns.begin(); cit != logicalColumns.end(); ++cit)
    {
      int logicalColumnY = cit->second;
      std::string colNameY = cit->first;
      std::string name = colNameX + " vs " + colNameY;

      std::vector<std::pair<te::dt::DateTime*, double> > values;
      double vy;
      dataSet->moveBeforeFirst();
      while(dataSet->moveNext())
      {
        if(dataSet->isNull(logicalColumnX) || dataSet->isNull(logicalColumnY))
          continue;

        te::dt::DateTime* t = dataSet->getDateTime(logicalColumnX);
        QString s = dataSet->getAsString(logicalColumnY).c_str();
        vy = s.toDouble();
        values.push_back(std::pair<te::dt::DateTime*, double> (t, vy));
      }
      srand(time(NULL)/vy);
      ts.insertCurve(values, name, QColor(rand()%256, rand()%256, rand()%256), true);
      std::vector<std::pair<te::dt::DateTime*, double> >::iterator it;
      for(it = values.begin(); it != values.end(); ++it)
        delete it->first;
    }

    //exibir as series temporais
    te::qt::qwt::TimeSeriesDisplay* tsd = new te::qt::qwt::TimeSeriesDisplay(ts, logicalColumns, logicalColumnX);
    tsd->setWindowTitle("Time Series");
    tsd->show();

    //faca conexao para remover este TimeSeriesDisplay* quando ele for closed
    QObject::connect(tsd, SIGNAL(closed(QwtPlot*)), this, SLOT(removePlotSlot(QwtPlot*)));

    layer->insertPlot(tsd);
  }
  QApplication::restoreOverrideCursor();
}

void MyWindow::removePlotSlot(QwtPlot* p)
{
  std::vector<te::map::AbstractLayer*> layers;
  getLayers(m_rootFolderLayer, layers);
  std::vector<te::map::AbstractLayer*>::iterator lit;
  for(lit = layers.begin(); lit != layers.end(); ++lit)
  {
    MyLayer* layer = (MyLayer*)(*lit);
    std::set<QwtPlot*> plots = layer->getPlots();
    if(plots.find(p) != plots.end())
    {
      layer->removePlot(p);
      if(isUsed(layer) == false)
        deleteGridOperation(layer);
      break;
    }
  }
}

void MyWindow::keepOnMemorySlot()
{
  MyLayer* layer = (MyLayer*)m_selectedLayer;
  bool b = m_keepOnMemoryAction->isChecked();
  layer->setKeepOnMemory(b);
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
//  m_layerExplorerModel->removeRowForced(ind, m_parentModelIndex); 

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
    layer->deleteGrid();

    //delete o mapDisplay relacionado ao layer
    std::vector<te::map::MapDisplay*>::iterator it;
    for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
    {
      MyDisplay* md = (MyDisplay*)(*it);
      if(md->parent() == 0 && md->getLayerTree() == layer)
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
  QString folderName;
  te::map::FolderLayer* folderLayer;
  te::da::DataSource* ds = 0;
  if(m_selectedLayer->getType() == "LAYER")
  {
    ds = ((te::map::Layer*)m_selectedLayer)->getDataSource();
    folderLayer = (te::map::FolderLayer*)m_selectedLayer->getParent();
    folderName = folderLayer->getId().c_str();
  }
  else
  {
    std::vector<te::map::AbstractLayer*> layers;
    getLayers(m_rootFolderLayer, layers);
    if(layers.begin() != layers.end())
      ds = ((te::map::Layer*)(*layers.begin()))->getDataSource();
    folderLayer = (te::map::FolderLayer*)m_selectedLayer;
    folderName = folderLayer->getId().c_str();

  }

  SelectLayer* sel = new SelectLayer(ds, this);
  if(sel->exec() == QDialog::Rejected)
    return;

  //std::vector<te::st::MovingObject*> output;
  //std::string XMLFileName = TE_DATA_EXAMPLE_LOCALE"\\data\\kml\\t_40_41_metadata.xml";
  //MovingObjectsFromKMLAndMetadata(output, XMLFileName);
  //PlotMovingObjectDistance(output[0], output[1]);

  std::string dstype = sel->m_dataSourceTypeComboBox->currentText().toStdString();
  std::string conInfo = sel->m_connectionStringLineEdit->text().toStdString();
  std::string lname = sel->m_layerNameComboBox->currentText().toStdString();
  std::string tname = sel->m_titleNameLineEdit->text().toStdString();

  delete sel;

  ds = te::da::DataSourceFactory::make(dstype);

  std::string dsInfo;
  //if(dstype == "GDAL")
  //  dsInfo = "URI=" + conInfo;
  //else
  //dsInfo = "connection_string=" + conInfo;
  if(dstype == "GDAL")
  {
    int p = conInfo.find("host=");
    if(p != std::string::npos)
      conInfo.replace(p, 5, "URI=");
  }
  dsInfo = conInfo;
  ds->open(dsInfo);

  te::da::DataSourceCatalog* catalog = ds->getCatalog();
  te::da::DataSourceTransactor* transactor = ds->getTransactor();
  te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();

  te::da::DataSetType* dst = loader->getDataSetType(lname, true);
  if(catalog->getDataSetType(lname) == 0)
    catalog->add(dst);
  MyLayer* layer = new MyLayer(lname, tname, folderLayer);

  if(folderName.toUpper() == "MOVINGOBJECTS" || folderName.toUpper() == "TEMPORALIMAGES")
    layer->setTemporal(true);

  layer->setDataSource(ds);

  if(dstype == "GDAL")
  {
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    te::da::DataSet* dataSet = transactor->getDataSet(lname);
    te::rst::Raster* raster = dataSet->getRaster();
    int srid = raster->getSRID();
    layer->setSRID(srid);
    te::gm::Envelope* env = raster->getExtent();
    layer->setExtent(env);

    delete raster;
    delete dataSet;
    delete transactor;
  }
  else
  {
    te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
    layer->setSRID(gp->getSRID());
    layer->setExtent(gp->getExtent());
  }

  QModelIndex itemIndex = m_layerExplorer->currentIndex();
  QModelIndex parentIndex = itemIndex.parent();
  te::qt::widgets::AbstractLayerItem* item = static_cast<te::qt::widgets::AbstractLayerItem*>(itemIndex.internalPointer());

  if(item->getRefLayer()->getType() == "LAYER")
  {
    // insert abaixo do layer current
    size_t ind = itemIndex.row() + 1;
    te::qt::widgets::LayerItem* layerItem = new te::qt::widgets::LayerItem(layer);
    layerItem->setParent(item->parent());
    std::vector<te::qt::widgets::AbstractLayerItem*> layerItems;
    layerItems.push_back(layerItem);
    m_layerExplorerModel->setItemsToBeInserted(layerItems);
    m_layerExplorerModel->insertRows(ind, 1, parentIndex);
  }
  else
  {
    // insert no fim do folder
    size_t ind = m_layerExplorerModel->rowCount(itemIndex);
    te::qt::widgets::LayerItem* layerItem = new te::qt::widgets::LayerItem(layer);
    layerItem->setParent(item);
    std::vector<te::qt::widgets::AbstractLayerItem*> layerItems;
    layerItems.push_back(layerItem);

    m_layerExplorerModel->setItemsToBeInserted(layerItems);
    m_layerExplorerModel->insertRows(ind, 1, itemIndex);

    if(item->getRefLayer()->getVisibility() == 1)
    {
      std::map<te::map::AbstractLayer*, te::map::Visibility> mapv;
      std::vector<te::map::AbstractLayer*> layers;
      getLayers(item->getRefLayer(), layers);
      std::vector<te::map::AbstractLayer*>::iterator it;
      for(it = layers.begin(); it != layers.end(); ++it)
      {
        te::map::Visibility v = (*it)->getVisibility();
        mapv[*it] = v;
      }

      item->getRefLayer()->setVisibility(te::map::PARTIALLY_VISIBLE); // este comando deixa todos os filhos do tipos layer no estado
                                                                      // PARTIALLY_VISIBLE, entao, o codigo abaixo acerta a visbilidade
                                                                      // de seus filhos (LAYERS) na arvore
      std::map<te::map::AbstractLayer*, te::map::Visibility>::iterator mt;
      for(mt = mapv.begin(); mt != mapv.end(); ++mt)
        mt->first->setVisibility(mt->second);

      m_layerExplorer->update();
    }
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

void MyWindow::plotTemporalDistanceSlot()
{
  std::vector<te::map::AbstractLayer*> layers = m_timeSlider->getLayers(); 
  PlotTemporalDistance w(layers, this);
  if(w.exec() == QDialog::Rejected)
    return;

  std::string mo1 = w.m_layer1ComboBox->currentText().toStdString();
  std::string mo2 = w.m_layer2ComboBox->currentText().toStdString();

  std::vector<te::st::MovingObject*> output;
  std::string XMLFileName = TE_DATA_EXAMPLE_LOCALE"\\data\\kml\\t_40_41_metadata.xml";
  MovingObjectsFromKMLAndMetadata(output, XMLFileName);

  te::st::MovingObject *ob1, *ob2;
  std::vector<te::st::MovingObject*>::iterator it;
  for(it = output.begin(); it != output.end(); ++it)
  {
    if((*it)->getId() == mo1)
      ob1 = *it;
    if((*it)->getId() == mo2)
      ob2 = *it;
  }

   PlotMovingObjectDistance(ob1, ob2);
}

void MyWindow::selectionChangedSlot(te::map::DataGridOperation* op)
{
  Q_EMIT selectionChanged(op);
}

void MyWindow::changeDefaultColorSlot()
{
  if(m_selectedLayer->getType() != "LAYER")
    return;

  MyLayer* layer = (MyLayer*)m_selectedLayer;
  te::map::DataGridOperation* op = layer->getDataGridOperation();
  if(op == 0)
    return;

  te::color::RGBAColor cor = op->getDefaultColor();
  QColor color, oldColor(cor.getRed(), cor.getGreen(), cor.getBlue(), cor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Default Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
  {
    cor.setColor(color.red(), color.green(), color.blue(), color.alpha());
    op->setDefaultColor(cor);
    //selectionChangedSlot(op);
    updateDisplays(layer);
  }
}

void MyWindow::changePointedColorSlot()
{
  if(m_selectedLayer->getType() != "LAYER")
    return;

  MyLayer* layer = (MyLayer*)m_selectedLayer;
  te::map::DataGridOperation* op = layer->getDataGridOperation();
  if(op == 0)
    return;

  te::color::RGBAColor cor = op->getPointedColor();
  QColor color, oldColor(cor.getRed(), cor.getGreen(), cor.getBlue(), cor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Pointed Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
  {
    cor.setColor(color.red(), color.green(), color.blue(), color.alpha());
    op->setPointedColor(cor);
    //selectionChangedSlot(op);
    updateDisplays(layer);
  }
}

void MyWindow::changeQueriedColorSlot()
{
  if(m_selectedLayer->getType() != "LAYER")
    return;

  MyLayer* layer = (MyLayer*)m_selectedLayer;
  te::map::DataGridOperation* op = layer->getDataGridOperation();
  if(op == 0)
    return;

  te::color::RGBAColor cor = op->getQueriedColor();
  QColor color, oldColor(cor.getRed(), cor.getGreen(), cor.getBlue(), cor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Queried Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
  {
    cor.setColor(color.red(), color.green(), color.blue(), color.alpha());
    op->setQueriedColor(cor);
    //selectionChangedSlot(op);
    updateDisplays(layer);
  }
}

void MyWindow::changePointedAndQueriedColorSlot()
{
  if(m_selectedLayer->getType() != "LAYER")
    return;

  MyLayer* layer = (MyLayer*)m_selectedLayer;
  te::map::DataGridOperation* op = layer->getDataGridOperation();
  if(op == 0)
    return;

  te::color::RGBAColor cor = op->getPointedAndQueriedColor();
  QColor color, oldColor(cor.getRed(), cor.getGreen(), cor.getBlue(), cor.getAlpha());

  color = QColorDialog::getColor(oldColor, this, "Select Pointed And Queried Color", QColorDialog::ShowAlphaChannel);
  if (color.isValid()) 
  {
    cor.setColor(color.red(), color.green(), color.blue(), 255);
    op->setPointedAndQueriedColor(cor);
    //selectionChangedSlot(op);
    updateDisplays(layer);
  }
}

void MyWindow::changePointStyleSlot()
{
  if(m_selectedLayer->getType() != "LAYER")
    return;

  MyLayer* layer = (MyLayer*)m_selectedLayer;
  te::map::DataGridOperation* op = layer->getDataGridOperation();
  if(op == 0)
    return;

  PointStyle w(op, this);
  if(w.exec() == QDialog::Rejected)
    return;

  QString width = w.m_widthComboBox->currentText();
  int pointWidth = width.toInt();
  op->setPointWidth(pointWidth);
  op->setPointColor(w.m_color);
  op->setPointIcon(w.m_pattern);
  op->setPointIconSize(w.m_patternSize);
  op->setPointIconImageType(w.m_imageType);

  updateDisplays(layer);
}

void MyWindow::changeLineStyleSlot()
{
  if(m_selectedLayer->getType() != "LAYER")
    return;

  MyLayer* layer = (MyLayer*)m_selectedLayer;
  te::map::DataGridOperation* op = layer->getDataGridOperation();
  if(op == 0)
    return;

  LineStyle w(op, this);
  if(w.exec() == QDialog::Rejected)
    return;

  op->setLineColor(w.m_color);

  QString width = w.m_widthComboBox->currentText();
  int lineWidth = width.toInt();
  op->setLineWidth(lineWidth);

  op->setLinePatternIcon(w.m_pattern);
  op->setLinePatternIconSize(w.m_patternSize);
  op->setLinePatternIconImageType(w.m_imageType);

  updateDisplays(layer);
}

void MyWindow::changePolygonStyleSlot()
{
  if(m_selectedLayer->getType() != "LAYER")
    return;

  MyLayer* layer = (MyLayer*)m_selectedLayer;
  te::map::DataGridOperation* op = layer->getDataGridOperation();
  if(op == 0)
    return;

  PolygonStyle w(op, this);
  if(w.exec() == QDialog::Rejected)
    return;

  //polygon contour
  op->setPolygonContourColor(w.m_polygonContourColor);

  QString width = w.m_polygonContourWidthComboBox->currentText();
  int contourWidth = width.toInt();
  op->setPolygonContourWidth(contourWidth);
  op->setPolygonContourPatternIcon(w.m_contourPattern);
  op->setPolygonContourPatternIconSize(w.m_contourSize);
  op->setPolygonContourPatternIconImageType(w.m_contourImageType);

  //polygon fill
  op->setPolygonFillColor(w.m_polygonFillColor);
  width = w.m_polygonFillPatternWidthComboBox->currentText();
  int patternWidth = width.toInt();
  op->setPolygonPatternWidth(patternWidth);
  op->setPolygonPatternIcon(w.m_fillPattern);
  op->setPolygonPatternIconSize(w.m_fillSize);
  op->setPolygonPatternIconImageType(w.m_fillImageType);

  //int findex = w.m_polygonFillMarkComboBox->currentIndex();
  //te::map::PtMarkerType markerType = (te::map::PtMarkerType)findex;
  //op->setPolygonMarkerType(markerType);
  //op->setPolygonFillMarkerColor(w.m_polygonFillMarkColor);


  updateDisplays(layer);
}

void MyWindow::updateDisplays(MyLayer* layer)
{
  //redesenhar os displays afetados
  std::set<te::map::MapDisplay*> displays;
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
  {
    std::vector<te::map::AbstractLayer*> layers;
    getLayers((*it)->getLayerTree(), layers);
    std::vector<te::map::AbstractLayer*>::iterator lit;
    for(lit = layers.begin(); lit != layers.end(); ++lit)
    {
      if(layer == *lit)
      {
        displays.insert(*it);
        break;
      }
    }
  }

  std::set<te::map::MapDisplay*>::iterator sit;
  for(sit = displays.begin(); sit != displays.end(); ++sit)
  {
    MyDisplay* display = (MyDisplay*)*sit;
    display->removeDrawOnlyChanged(layer);
    display->draw(layer);

    std::vector<te::map::AbstractLayer*> layers;
    getLayers(display->getLayerTree(), layers);
    display->reorderDrawing(layers);
    ((MyDisplay*)(*sit))->update();
  }
}
