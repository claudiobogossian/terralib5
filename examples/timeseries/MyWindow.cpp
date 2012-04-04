#include "MyWindow.h"
#include "MyLayer.h"
#include "SelectLayer.h"
#include "STExamples.h"
#include "PlotTemporalDistance.h"
#include "RasterExamples.h"
#include "STExamples.h"

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

//STL
#include <string>

MyWindow::MyWindow(QWidget* parent) : QWidget(parent),
  m_configStyle(0),
  m_temporalDrawingConfig(0),
  m_temporalDrawLines(false),
  m_temporalLoop(false)

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
  delete loader;
// Create MovingObjects folder
  te::map::FolderLayer *movingObjectsFolderLayer = new te::map::FolderLayer("MovingObjects", "MovingObjects", m_rootFolderLayer);

// Create MovingObjects layer 40 and 41
  te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
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

  m_displayBox = new QGroupBox;
  QVBoxLayout *displayLayout = new QVBoxLayout(m_displayBox);
  m_splitter = new QSplitter;
  MyDisplay* display = new MyDisplay(650, 600, m_splitter);
  m_mapDisplayVec.push_back(display);
  display->setMinimumWidth(300);
  display->setMinimumHeight(200);
  display->setLayerTree(m_rootFolderLayer);

  m_timeSlider = new TimeSlider(Qt::Horizontal);
  m_timeSlider->setMapDisplay(display);
  displayLayout->addWidget(m_splitter);
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
  connect(m_timeSlider, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(timeSliderContextMenuSlot(const QPoint&)));
  m_timeSlider->loadMovingObjects();
  display->setTimeSlider(m_timeSlider);

  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(m_layerExplorer);
  horizontalLayout->addWidget(m_displayBox);

  setLayout(horizontalLayout);

  connect(m_layerExplorer, SIGNAL(contextMenuPressed(const QModelIndex&, const QPoint&)), this, SLOT(contextMenuPressedSlot(const QModelIndex&, const QPoint&)));
  connect(m_layerExplorer, SIGNAL(checkBoxWasClicked(const QModelIndex&)), this, SLOT(layerVisibilityChangedSlot(const QModelIndex&)));
  connect(m_layerExplorerModel, SIGNAL(dragDropEnded(const QModelIndex&, const QModelIndex&)), this, SLOT(layerItemMovedSlot(const QModelIndex&, const QModelIndex&)));

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

  m_plotTemporalDistanceAction = new QAction("&Plot Temporal Distance...", m_treeMenu);
  m_treeMenu->addAction(m_plotTemporalDistanceAction);
  connect(m_plotTemporalDistanceAction, SIGNAL(triggered()), this, SLOT(plotTemporalDistanceSlot()));
}

MyWindow::~MyWindow()
{
// delete m_rootFolderLayer; //como deletar isto???????
// acho que o LayerExplorer deveria fazer isso, mas, vou varrer a arvore e fazer aqui...
  std::vector<te::map::AbstractLayer*> layers;
  getLayers(m_rootFolderLayer, layers);  // so pega o tipo LAYER
  std::vector<te::map::AbstractLayer*>::iterator lit;
  for(lit = layers.begin(); lit != layers.end(); ++lit)
  {
    MyLayer* layer = (MyLayer*)(*lit);
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op)
    {
      delete op->getDataSet()->getTransactor();
      delete op->getDataSet();
      delete op;
    }
    layer->setDataGridOperation(0);
  }

// delete map displays
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
    delete (*it);

// delete grids and plots
  std::map<std::string, MyGrid*>::iterator git;
  for(git = m_gridMap.begin(); git != m_gridMap.end(); ++git)
  {
    // delete todos os plots do grid
    std::map<std::string, std::vector<QwtPlot*> >::iterator pit = m_plotMap.find(git->first);
    if(pit != m_plotMap.end())
    {
      std::vector<QwtPlot*>& plots = pit->second;
      std::vector<QwtPlot*>::iterator vit;
      for(vit = plots.begin(); vit != plots.end(); ++vit)
        delete *vit;
    }

    // delete grid
    delete git->second;
  }

  delete m_ds;
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


void MyWindow::layerVisibilityChangedSlot(const QModelIndex& mi)
{
  try
  {
    te::qt::widgets::AbstractLayerItem* childItem = static_cast<te::qt::widgets::AbstractLayerItem*>(mi.internalPointer());
    te::map::AbstractLayer* al = childItem->getRefLayer();

    std::vector<te::map::AbstractLayer*> changedLayers;
    getLayers(al, changedLayers);  // so pega o tipo LAYER

    // veja quais displays estao usando estes changedLayers e atualize o mapa de gridoperation
    std::set<te::map::MapDisplay*> displaySet;
    std::vector<te::map::MapDisplay*>::iterator it;
    for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
    {
      MyDisplay* display = (MyDisplay*)*it;
      te::map::AbstractLayer* ald = display->getLayerTree();
      std::vector<te::map::AbstractLayer*> layers;
      getLayers(ald, layers);  // so pega o tipo LAYER
      std::vector<te::map::AbstractLayer*>::iterator lit, cit;
      for(lit = layers.begin(); lit != layers.end(); ++lit)
      {
        MyLayer* layer = (MyLayer*)*lit;
        if(layer->isTemporal())
          continue;

        for(cit = changedLayers.begin(); cit != changedLayers.end(); ++cit)
        {
          if(*lit == *cit)
          {
            // atualiza o display porque ele usa o layer (ou desenha ou apaga o layer)
            displaySet.insert(display);

            // mudou para
            int visible = al->getVisibility(); // NOT_VISIBLE, VISIBLE, PARTIALLY_VISIBLE

            te::map::DataGridOperation* op = layer->getDataGridOperation();
            if(op == 0)
            {
              createGridOperation(layer);
              op = layer->getDataGridOperation();

              // para novo grid operation, o canvas deve ser totalamente repintado.
              layer->setRenderer(0);
            }
            if(visible == 1) // ficou visible
              insertGridOperation(op, display);
            else if(visible == 0)
              removeGridOperation(op, display);
            break;
          }
        }
      }
    }
    std::set<te::map::MapDisplay*>::iterator sit;
    for(sit = displaySet.begin(); sit != displaySet.end(); ++sit)
    {
      ((MyDisplay*)(*sit))->draw();
      ((MyDisplay*)(*sit))->update();
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

void MyWindow::insertGridOperation(te::map::DataGridOperation* op, QWidget* w)
{
  std::map<te::map::DataGridOperation*, std::set<QWidget*> >::iterator it = m_operationMap.find(op);
  if(it == m_operationMap.end())
  {
    std::set<QWidget*> s;
    s.insert(w);
    m_operationMap[op] = s;
  }
  else
  {
    if(it->second.find(w) == it->second.end())
      it->second.insert(w);
  }
}

void MyWindow::removeGridOperation(te::map::DataGridOperation* op, QWidget* w)
{
  std::map<te::map::DataGridOperation*, std::set<QWidget*> >::iterator it = m_operationMap.find(op);
  if(it != m_operationMap.end())
  {
    std::set<QWidget*>& widgets = it->second;
    std::set<QWidget*>::iterator sit = widgets.find(w);
    if(sit != widgets.end())
      widgets.erase(sit);

    if(widgets.empty())
    {
      // remova do mapa
      m_operationMap.erase(it);

      // nenhum widget usa este gridoperation, entao,
      // ache o layer que usa este gridoperation
      std::vector<te::map::AbstractLayer*> layers;
      getLayers(m_rootFolderLayer, layers); // so pega o tipo LAYER
      std::vector<te::map::AbstractLayer*>::iterator lit;
      for(lit = layers.begin(); lit != layers.end(); ++lit)
      {
        MyLayer* layer = (MyLayer*)(*lit);
        te::map::DataGridOperation* layerop = layer->getDataGridOperation();
        if(layerop == op)
        {
          // libere memoria (remova gridoperation do layer)
          delete op->getDataSet()->getTransactor();
          delete op->getDataSet();
          delete op;
          layer->setDataGridOperation(0);

          // delete os canvas que usavam este layer
          std::vector<te::map::MapDisplay*>::iterator cit;
          for(cit = m_mapDisplayVec.begin(); cit != m_mapDisplayVec.end(); ++cit)
            ((MyDisplay*)(*cit))->deleteCanvas(layer);

          break;
        }
      }
    }
  }
}

void MyWindow::createGridOperation(MyLayer* layer)
{
  if(layer->getDataGridOperation() == 0)
  {
    std::string name = layer->getId();
    te::da::DataSource* ds = layer->getDataSource();
    te::da::DataSourceTransactor* t = ds->getTransactor();
    assert(t);

    te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();
    assert(loader);

    te::da::DataSetType* dsType = loader->getDataSetType(name, true);
    dsType->setCatalog(ds->getCatalog());
    assert(dsType);
    delete loader;

    te::da::DataSet* dataSet = t->getDataSet(name);
    assert(dataSet);
    if(dsType->getPrimaryKey())
    {
      te::map::DataGridOperation* operation = new te::map::DataGridOperation();
      operation->init(dsType, dataSet);
      layer->setDataGridOperation(operation);
    }
  }
}

void MyWindow::insertPlot(std::string gridName, QwtPlot* p)
{
  // esqueca grid operation
  std::map<std::string, std::vector<QwtPlot*> >::iterator it = m_plotMap.find(gridName);
  if(it != m_plotMap.end())
  {
    std::vector<QwtPlot*>& plots = it->second;
    std::vector<QwtPlot*>::iterator vit;
    for(vit = plots.begin(); vit != plots.end(); ++vit)
    {
      if(*vit == p)
        break;
    }
    if(vit == plots.end())
      plots.push_back(p);
  }
  else
  {
    std::vector<QwtPlot*> plots;
    plots.push_back(p);
    m_plotMap[gridName] = plots;
  }
}

void MyWindow::removePlot(std::string gridName, QwtPlot* p)
{
  // esqueca grid operation
  std::map<std::string, std::vector<QwtPlot*> >::iterator it = m_plotMap.find(gridName);
  if(it != m_plotMap.end())
  {
    std::vector<QwtPlot*>& plots = it->second;
    std::vector<QwtPlot*>::iterator vit;
    for(vit = plots.begin(); vit != plots.end(); ++vit)
    {
      if(*vit == p)
      {
        plots.erase(vit);
        break;
      }
    }

    if(plots.empty())
      m_plotMap.erase(it);
  }
}

QwtPlot* MyWindow::getPlot(std::string gridName, std::string plotTitle)
{
  std::map<std::string, std::vector<QwtPlot*> >::iterator it;
  for(it = m_plotMap.begin(); it != m_plotMap.end(); ++it)
  {
    std::vector<QwtPlot*>& plots = it->second;
    std::vector<QwtPlot*>::iterator vit;
    for(vit = plots.begin(); vit != plots.end(); ++vit)
    {
      if(((te::qt::qwt::Plot*)(*vit))->windowTitle().toStdString() == plotTitle)
        return *vit;
    }
  }
  return 0;
}

std::vector<QwtPlot*> MyWindow::getPlots(std::string gridName)
{
  std::map<std::string, std::vector<QwtPlot*> >::iterator it = m_plotMap.find(gridName);
  if(it != m_plotMap.end())
    return it->second;

  std::vector<QwtPlot*> plots;
  return plots;
}

void MyWindow::layerItemMovedSlot(const QModelIndex&, const QModelIndex&)
{
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
  {
    ((MyDisplay*)(*it))->draw();
    ((MyDisplay*)(*it))->update();
  }
}

void MyWindow::timeSliderContextMenuSlot(const QPoint& pos)
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

void MyWindow::contextMenuPressedSlot(const QModelIndex& mi, const QPoint& pos)
{
  //teste m_styleAction->setEnabled(false);
  m_styleAction->setEnabled(true);

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

  if(title.toUpper() == "MOVINGOBJECTS" || title2.toUpper() == "MOVINGOBJECTS")
  {
    m_openNewMapDisplayAction->setEnabled(false);
    m_addFolderAction->setEnabled(false);
  }
  else
  {
    m_openNewMapDisplayAction->setEnabled(true);
    m_addFolderAction->setEnabled(true);
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
/////////////////////////////////////////////////////
  // resolver o problema de layer temporais......
  // ainda nao faz nada com temporais.....
////////////////////////////////////////////////////
  MyDisplay *md = new MyDisplay(300, 250);
  md->setMinimumWidth(300);
  md->setMinimumHeight(250);
  md->setLayerTree(m_selectedLayer);

  std::vector<te::map::AbstractLayer*> layers;
  getLayers(m_selectedLayer, layers); // so pega o tipo LAYER
  std::vector<te::map::AbstractLayer*>::iterator lit;
  for(lit = layers.begin(); lit != layers.end(); ++lit)
  {
    te::map::AbstractLayer* item = *lit;
    if(item->getVisibility() == te::map::VISIBLE)
    {
      MyLayer* layer = (MyLayer*)item;
      createGridOperation(layer);
      te::map::DataGridOperation* op = layer->getDataGridOperation();
      //add this display to operation map
      insertGridOperation(op, md);
    }
  }

//faca conexao deste com os map display existentes
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
  {
    MyDisplay* display = (MyDisplay*)*it;
    QObject::connect(md, SIGNAL(selectionChanged(te::map::DataGridOperation*)), display, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    QObject::connect(display, SIGNAL(selectionChanged(te::map::DataGridOperation*)), md, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
  }

//faca conexao deste com os grids existentes
  std::map<std::string, MyGrid*>::iterator git;
  for(git = m_gridMap.begin(); git != m_gridMap.end(); ++git)
  {
    MyGrid* grid = git->second;
    QObject::connect(md, SIGNAL(selectionChanged(te::map::DataGridOperation*)), grid, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    QObject::connect(grid, SIGNAL(selectionChanged(te::map::DataGridOperation*)), md, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
  }

//faca conexao deste com os plots existentes
  std::map<std::string, std::vector<QwtPlot*> >::iterator pit; 
  for(pit = m_plotMap.begin(); pit != m_plotMap.end(); ++pit)
  {
    std::vector<QwtPlot*>& plots = pit->second;
    std::vector<QwtPlot*>::iterator vit;
    for(vit = plots.begin(); vit != plots.end(); ++vit)
    {
      QObject::connect(md, SIGNAL(selectionChanged(te::map::DataGridOperation*)), *vit, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
      QObject::connect(*vit, SIGNAL(selectionChanged(te::map::DataGridOperation*)), md, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    }
  }

//faca conexao para remover o display do m_mapDisplayVec quando ele for closed
  QObject::connect(md, SIGNAL(closed(MyDisplay*)), this, SLOT(removeDisplaySlot(MyDisplay*)));

  m_mapDisplayVec.push_back(md);
  md->show();
  ((te::qt::widgets::MapDisplay*)md)->draw();
}

void MyWindow::removeDisplaySlot(MyDisplay* d)
{
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

  te::map::AbstractLayer* al = d->getLayerTree();
  if(al && al->getVisibility() != te::map::NOT_VISIBLE)
  {
    std::vector<te::map::AbstractLayer*> layers;
    getLayers(al, layers); // so pega o tipo LAYER
    std::vector<te::map::AbstractLayer*>::iterator lit;
    for(lit = layers.begin(); lit != layers.end(); ++lit)
    {
      MyLayer* layer = (MyLayer*)*lit;
      if(layer->isTemporal())
        continue;
      if(layer->getVisibility() == te::map::VISIBLE)
      {
        // Neste caso estou supondo que layer é folha (nao tem filhos)
        removeGridOperation(layer->getDataGridOperation(), d);

        // verifique se ha outro display usando este Layer.
        if(isDrawnOnTheDisplay(layer) == false)
        {
          // nenhum display usa este layer
          // set layer para nao visivel --- como ?????????????????
          // Bom! Talves este codigo nao seja necessario porque sempre vai existir
          // um display que e' da janela principal
          //??????????????????????????????????????????????????????????
          //??????????????????????????????????????????????????????????
          //??????????????????????????????????????????????????????????
          //??????????????????????????????????????????????????????????
          //??????????????????????????????????????????????????????????
          //??????????????????????????????????????????????????????????
        }
      }
    }
  }
}

bool MyWindow::isDrawnOnTheDisplay(te::map::Layer* layer)
{
  if(layer->getVisibility() == false || ((MyLayer*)(layer))->isTemporal())
    return false;

  // verifica se tem algum display visualizando o layer
  std::vector<te::map::MapDisplay*>::iterator it;
  for(it = m_mapDisplayVec.begin(); it != m_mapDisplayVec.end(); ++it)
  {
    MyDisplay* display = (MyDisplay*)*it;
    te::map::AbstractLayer* al = display->getLayerTree();
    if(al && al->getVisibility() != te::map::NOT_VISIBLE)
    {
      std::vector<te::map::AbstractLayer*> layers;
      getLayers(al, layers); // so pega o tipo LAYER
      std::vector<te::map::AbstractLayer*>::iterator lit;
      for(lit = layers.begin(); lit != layers.end(); ++lit)
      {
        te::map::AbstractLayer* item = *lit;
        if(item == layer)
          return true;
      }
    }
  }
  return false;
}

void MyWindow::openGridSlot()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  MyLayer* layer = (MyLayer*)m_selectedLayer;
  std::string name = m_selectedLayer->getId();
  if((layer->getType() == "LAYER") == false)
  {
    QMessageBox::information(this, tr("Open Grid..."), tr("ERROR: It is not a layer!"));
    QApplication::restoreOverrideCursor();
    return;
  }
  if(layer->isTemporal())
  {
    QMessageBox::information(this, tr("Open Grid..."), tr("ERROR: The layer is temporal!"));
    QApplication::restoreOverrideCursor();
    return;
  }

  try
  {
    m_ds = layer->getDataSource();
    if(m_gridMap.find(name) == m_gridMap.end())
    {
      // abra um novo grid
      MyGrid* grid = new MyGrid(&m_mapDisplayVec, this);
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
        // ninguem está usando o layer (este widget é o primeiro a usar)
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
      {
        // ja tem algum widget usando este layer
        dataSet = operation->getDataSet();

        // verifique se tem algum plot usando este layer
        std::vector<QwtPlot*> plots = getPlots(name);
        if(plots.empty() == false)
        {
          // isto significa que um outro grid deste layer ja foi aberto e, por sua vez,
          // este antigo grid, criou plots que ficaram abertos, entao,
          // faca conexao entre este grid e os plots
          std::vector<QwtPlot*>::iterator it;
          for(it = plots.begin(); it != plots.end(); ++it)
          {
            QwtPlot* plot = *it;
            QObject::connect(grid, SIGNAL(selectionChanged(te::map::DataGridOperation*)), plot, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
            QObject::connect(plot, SIGNAL(selectionChanged(te::map::DataGridOperation*)), grid, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
          }
        }
      }
      te::qt::widgets::DataGridModel* gridModel = new te::qt::widgets::DataGridModel(dsType, dataSet, operation);

      //faca conexao para remover o grid do m_gridMap quando ele for closed
      QObject::connect(grid, SIGNAL(closed(MyGrid*)), this, SLOT(removeGridSlot(MyGrid*)));

      //add this grid to operation multimap
      insertGridOperation(operation, grid);

      grid->setVisible(true);
      grid->setModel(gridModel);
      grid->hideColumn(geometryColumn);
      grid->viewport()->update();
      grid->show();  
  //    delete t;
    }
    else
    {
      // grid ja aberto, antao, de show apenas
      MyGrid* grid = m_gridMap.find(name)->second;
      if(grid->isMinimized())
        grid->showNormal();
      else
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

void MyWindow::removeGridSlot(MyGrid* g)
{
  std::map<std::string, MyGrid*>::iterator it;
  for(it = m_gridMap.begin(); it != m_gridMap.end(); ++it)
  {
    if(g == it->second)
    {
      m_gridMap.erase(it);
      break;
    }
  }

  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)g->model();
  te::map::DataGridOperation* op = model->getDataGridOperation();
  removeGridOperation(op, g);
}

void MyWindow::removePlotSlot(QwtPlot* plot)
{
  te::map::DataGridOperation* op = ((te::qt::qwt::Plot*)(plot))->getDataGridOperation();
  std::string layerName = op->getDataSetType()->getName();
  removePlot(layerName, plot);
  removeGridOperation(op, plot);
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
///////////////////////////////////////////////////////////////////////////
  // este metodo ainda deve ser ajustado para redesenhar os displays afetados
  // deve tambem acertar o gridoperation e remover todas as interfaces que
  // dependem deste layer (histogram, scaterr, grid, etc
///////////////////////////////////////////////////////////////////////////////


  size_t ind = m_selectedLayer->getIndex();
  // este metodo ja remove da interface e da memoria - TreeItem
  m_layerExplorerModel->removeRows(ind, 1, m_parentModelIndex); 

  // delete todos os DataSets e Transactors abaixo de m_selectedLayer
  std::vector<te::map::AbstractLayer*> layers;
  getLayers(m_selectedLayer, layers);  // so pega o tipo LAYER
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
  SelectLayer* sel = new SelectLayer(m_ds, this);
  if(sel->exec() == QDialog::Rejected)
    return;

  //std::vector<te::st::MovingObject*> output;
  //std::string XMLFileName = TE_DATA_EXAMPLE_LOCALE"\\data\\kml\\t_40_41_metadata.xml";
  //MovingObjectsFromKMLAndMetadata(output, XMLFileName);
  //PlotMovingObjectDistance(output[0], output[1]);

  std::string dstype = sel->m_dataSourceTypeComboBox->currentText().toStdString();
  std::string conInfo = sel->m_connectionStringLineEdit->text().toStdString();
  std::string lname = sel->m_layerNameComboBox->currentText().toStdString();

  delete sel;

  m_ds = te::da::DataSourceFactory::make(dstype);

  std::string dsInfo;
  if(dstype == "GDAL")
    dsInfo = "URI=" + conInfo;
  else
  dsInfo = "connection_string=" + conInfo;
  m_ds->open(dsInfo);

  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  te::da::DataSourceTransactor* transactor = m_ds->getTransactor();
  te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();

  te::da::DataSetType* dst = loader->getDataSetType(lname, true);
  if(catalog->getDataSetType(lname) == 0)
    catalog->add(dst);
  MyLayer* layer = new MyLayer(lname, lname, m_selectedLayer);

  QString folderName = m_selectedLayer->getTitle().c_str();
  if(folderName.toUpper() == "MOVINGOBJECTS" || folderName.toUpper() == "TEMPORALIMAGES")
    layer->setTemporal(true);

  layer->setDataSource(m_ds);

  if(dstype == "GDAL")
  {
    te::da::DataSourceTransactor* transactor = m_ds->getTransactor();
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
  te::qt::widgets::AbstractLayerItem* item = static_cast<te::qt::widgets::AbstractLayerItem*>(itemIndex.internalPointer());
  
  size_t ind = m_layerExplorerModel->rowCount(itemIndex);
  te::qt::widgets::LayerItem* layerItem = new te::qt::widgets::LayerItem(layer);
  layerItem->setParent(item);
  std::vector<te::qt::widgets::AbstractLayerItem*> layerItems;
  layerItems.push_back(layerItem);

  m_layerExplorerModel->setItemsToBeInserted(layerItems);
  m_layerExplorerModel->insertRows(ind, 1, itemIndex);






  //try
  //{
  //  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  //  te::da::DataSourceTransactor* transactor = m_ds->getTransactor();
  //  te::da::DataSourceCatalogLoader* loader = transactor->getCatalogLoader();
  //  std::vector<std::string*> names;
  //  loader->getDataSets(names);
  //  int size = names.size();
  //  QStringList items;
  //  for(int i = 0; i < size; ++i)
  //  {
  //    std::string& id = (*names[i]);
  //    QString s = id.c_str();
  //    items.insert(i, s);
  //  }
  //  bool ok;
  //  QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),
  //                                        tr("DataSet:"), items, 0, false, &ok);
  //  if (ok && !item.isEmpty())
  //  {
  //    std::string name = item.toStdString();

  //    te::da::DataSetType* dst = loader->getDataSetType(name, true);
  //    if(catalog->getDataSetType(name) == 0)
  //      catalog->add(dst);
  //    te::gm::GeometryProperty* gp = dst->getDefaultGeomProperty();
  //    MyLayer* layer = new MyLayer(name, name, m_selectedLayer);
  //    layer->setDataSource(m_ds);
  //    layer->setSRID(gp->getSRID());

  //    QModelIndex itemIndex = m_layerExplorer->currentIndex();
  //    te::qt::widgets::AbstractLayerItem* item = static_cast<te::qt::widgets::AbstractLayerItem*>(itemIndex.internalPointer());
  //  
  //    size_t ind = m_layerExplorerModel->rowCount(itemIndex);
  //    te::qt::widgets::LayerItem* layerItem = new te::qt::widgets::LayerItem(layer);
  //    layerItem->setParent(item);
  //    std::vector<te::qt::widgets::AbstractLayerItem*> layerItems;
  //    layerItems.push_back(layerItem);

  //    m_layerExplorerModel->setItemsToBeInserted(layerItems);
  //    m_layerExplorerModel->insertRows(ind, 1, itemIndex);
  //  }
  //  delete loader;
  //  delete transactor;
  //}
  //catch(std::exception& e)
  //{
  //  QApplication::restoreOverrideCursor();
  //  QMessageBox::information(this, tr("Error Add Layer..."), tr(e.what()));
  //  return;
  //}
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
