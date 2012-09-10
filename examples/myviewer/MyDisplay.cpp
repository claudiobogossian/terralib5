#include "MyDisplay.h"
#include "MyLayer.h"
#include "MyLayerRenderer.h"
#include "PrintInFile.h"
#include <terralib/common/PlatformUtils.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/raster.h>
#include <terralib/geometry.h>

#include "STExamples.h"

//QT
#include <QPaintEvent>
#include <QMessageBox>
#include <QString>
#include <QMatrix>
#include <QRectF>
#include <QPrintDialog>
#include <QPrinter>
#include <QPaintEngine>

MyDisplay::MyDisplay(int w, int h, te::map::AbstractLayer* root, QWidget* parent, Qt::WindowFlags f) :
  te::qt::widgets::MapDisplay(w, h, parent, f),
  m_rootFolderLayer(root),
  m_timeSlider(0)
{
  setAcceptDrops(true);

  m_tooltipDisplayPixmap = new QPixmap(w, h);
  clearTooltipPixmap();

  m_temporalVectorialDisplayPixmap = new QPixmap(w, h);
  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));

  m_temporalImageDisplayPixmap = new QPixmap(w, h);
  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));

  m_mouseOperationMenu = m_menu->addMenu("Mouse Operation");

  m_mouseZoomInAction = new QAction("Zoom In", m_mouseOperationMenu);
  m_mouseZoomInAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseZoomInAction);
  connect(m_mouseZoomInAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToZoomInSlot()));

  m_mouseZoomOutAction = new QAction("Zoom Out", m_mouseOperationMenu);
  m_mouseZoomOutAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseZoomOutAction);
  connect(m_mouseZoomOutAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToZoomOutSlot()));

  m_mousePanAction = new QAction("Pan", m_mouseOperationMenu);
  m_mousePanAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mousePanAction);
  connect(m_mousePanAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToPanSlot()));

  m_mouseSelectionAction = new QAction("Object Selection", m_mouseOperationMenu);
  m_mouseSelectionAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseSelectionAction);
  connect(m_mouseSelectionAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToObjectSelectionSlot()));

  m_mouseAddSelectionAction = new QAction("Object Add Selection", m_mouseOperationMenu);
  m_mouseAddSelectionAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseAddSelectionAction);
  connect(m_mouseAddSelectionAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToAddObjectSelectionSlot()));

  m_mouseToggleSelectionAction = new QAction("Object Toggle selection", m_mouseOperationMenu);
  m_mouseToggleSelectionAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseToggleSelectionAction);
  connect(m_mouseToggleSelectionAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToToggleObjectSelectionSlot()));

  m_mouseTooltipAction = new QAction("Tooltip", m_mouseOperationMenu);
  m_mouseTooltipAction->setCheckable(true);
  m_mouseOperationMenu->addAction(m_mouseTooltipAction);
  connect(m_mouseTooltipAction, SIGNAL(triggered()), this, SLOT(setMouseOperationToTooltipSlot()));

  QAction* drawAllPointedsAction = new QAction("Fit All &Pointeds", m_menu);
  m_menu->addAction(drawAllPointedsAction);
  connect(drawAllPointedsAction, SIGNAL(triggered()), this, SLOT(drawAllPointedsSlot()));

  QAction* drawAllQueriedsAction = new QAction("Fit All &Querieds", m_menu);
  m_menu->addAction(drawAllQueriedsAction);
  connect(drawAllQueriedsAction, SIGNAL(triggered()), this, SLOT(drawAllQueriedsSlot()));

  QAction* drawAllPointedsAndQueriedsAction = new QAction("Fit &All Pointeds And Querieds", m_menu);
  m_menu->addAction(drawAllPointedsAndQueriedsAction);
  connect(drawAllPointedsAndQueriedsAction, SIGNAL(triggered()), this, SLOT(drawAllPointedsAndQueriedsSlot()));

  QAction* showRootFolderAction = new QAction("Show Root Folder", m_menu);
  m_menu->addAction(showRootFolderAction);
  connect(showRootFolderAction, SIGNAL(triggered()), this, SLOT(showRootFolderSlot()));

  QAction* printAction = new QAction("Print...", m_menu);
  m_menu->addAction(printAction);
  connect(printAction, SIGNAL(triggered()), this, SLOT(printSlot()));

  QAction* printFileAction = new QAction("Print In File...", m_menu);
  m_menu->addAction(printFileAction);
  connect(printFileAction, SIGNAL(triggered()), this, SLOT(printFileSlot()));

  m_mouseHandler = new MouseHandler(this);

  connect(m_mouseHandler, SIGNAL(execZoomArea(QRect)), this, SLOT(execZoomAreaSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execZoomIn(QPoint)), this, SLOT(execZoomInSlot(QPoint)));
  connect(m_mouseHandler, SIGNAL(execZoomOut(QPoint)), this, SLOT(execZoomOutSlot(QPoint)));
  connect(m_mouseHandler, SIGNAL(execPan(QPoint, QPoint)), this, SLOT(execPanSlot(QPoint, QPoint)));
  connect(m_mouseHandler, SIGNAL(execSelection(QRect)), this, SLOT(mouseSelectionSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execAddSelection(QRect)), this, SLOT(mouseAddSelectionSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execToggleSelection(QRect)), this, SLOT(mouseToggleSelectionSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execTooltip(QPoint)), this, SLOT(mouseTooltipSlot(QPoint)));

  m_mouseHandler->setCursor();

  sizeHint();
}

MyDisplay::~MyDisplay()
{
  delete m_temporalVectorialDisplayPixmap;
  delete m_temporalImageDisplayPixmap;
  disconnect();
}

void MyDisplay::closeEvent(QCloseEvent *event)
{
  setAttribute(Qt::WA_DeleteOnClose);
  Q_EMIT closed(this);
  event->accept();
}

void MyDisplay::dragEnterEvent(QDragEnterEvent* e)
{
  const QMimeData* mime = e->mimeData();
  QString s = mime->data("application/layer-explorer").constData();
  if(s.isEmpty())
    return;

  unsigned long long v = s.toULongLong();
  te::map::AbstractLayer* al = (te::map::AbstractLayer*)v;
  te::map::AbstractLayer* laux = al;
  while((te::map::AbstractLayer*)laux->getParent())
    laux = (te::map::AbstractLayer*)laux->getParent();

  if(laux == m_rootFolderLayer)
    e->accept();
  return;
}

void MyDisplay::dropEvent(QDropEvent* e)
{
  const QMimeData* mime = e->mimeData();
  QString s = mime->data("application/layer-explorer").constData();
  unsigned long long v = s.toULongLong();
  te::map::AbstractLayer* al = (te::map::AbstractLayer*)v;
  changeTree(al);
}

void MyDisplay::changeTree(te::map::AbstractLayer* al)
{
  // limpe todos os canvas antes usados 
  std::map<te::map::AbstractLayer*, te::qt::widgets::Canvas*>::iterator it;
  for(it = m_layerCanvasMap.begin(); it != m_layerCanvasMap.end(); ++it)
  {
    te::map::Canvas* c = getCanvas(it->first);
    delete c;
  }
  m_layerCanvasMap.clear();

  // calcule novo SRID e extent
  m_srid = -1;
  m_layerTree = al;
  std::list<te::map::AbstractLayer*> layerList;
  std::list<te::map::AbstractLayer*>::iterator lit;
  if(m_layerTree)
    mountLayerList(m_layerTree, layerList);
  te::gm::Envelope extent;

  for(lit = layerList.begin(); lit != layerList.end(); ++lit)
  {
    te::gm::Envelope env = getLayerExtent(*lit);

    int srid = (*lit)->getSRID();
    if(m_srid == -1)
      m_srid = srid;
    if(srid != m_srid)
      env.transform(srid, m_srid);

    extent.Union(env);
  }
  setExtent(extent);

  QString wtitle = "Display: ";
  if(m_layerTree)
    wtitle += m_layerTree->getTitle().c_str();
  setWindowTitle(wtitle);

  QWidget* w = (QWidget*)parent();
  while(w)
  {
    if(w->parent())
      w = (QWidget*)w->parent();
    else
    {
      QString mainTitle = "My Main Window - " + wtitle;
      w->setWindowTitle(mainTitle);
      break;
    }
  }

  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));
  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));

  draw();
  if(extent.isValid() == false)
  {
    m_displayPixmap->fill();
    update();
  }
}

void MyDisplay::showRootFolderSlot()
{
  if(m_layerTree == m_rootFolderLayer)
    return;

  changeTree(m_rootFolderLayer);
}

void MyDisplay::contextMenuEvent(QContextMenuEvent* c)
{
  m_mouseZoomInAction->setChecked(false);
  m_mouseZoomOutAction->setChecked(false);
  m_mousePanAction->setChecked(false);
  m_mouseSelectionAction->setChecked(false);
  m_mouseAddSelectionAction->setChecked(false);
  m_mouseToggleSelectionAction->setChecked(false);
  m_mouseTooltipAction->setChecked(false);

  MouseHandler::MouseMode mode = m_mouseHandler->getMode();
  if(mode == MouseHandler::ZoomInMode)
    m_mouseZoomInAction->setChecked(true);
  else if(mode == MouseHandler::ZoomOutMode)
    m_mouseZoomOutAction->setChecked(true);
  else if(mode == MouseHandler::PanMode)
    m_mousePanAction->setChecked(true);
  else if(mode == MouseHandler::SelectionMode)
    m_mouseSelectionAction->setChecked(true);
  else if(mode == MouseHandler::AddSelectionMode)
    m_mouseAddSelectionAction->setChecked(true);
  else if(mode == MouseHandler::ToggleSelectionMode)
    m_mouseToggleSelectionAction->setChecked(true);
  else if(mode == MouseHandler::TooltipMode)
    m_mouseTooltipAction->setChecked(true);

  m_menu->exec(c->globalPos());
}

void MyDisplay::setMouseOperationToObjectSelectionSlot()
{
  m_mouseHandler->setMode(MouseHandler::SelectionMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToAddObjectSelectionSlot()
{
  m_mouseHandler->setMode(MouseHandler::AddSelectionMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToToggleObjectSelectionSlot()
{
  m_mouseHandler->setMode(MouseHandler::ToggleSelectionMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToTooltipSlot()
{
  m_mouseZoomInAction->setChecked(false);
  m_mouseZoomOutAction->setChecked(false);
  m_mousePanAction->setChecked(false);
  m_mouseSelectionAction->setChecked(false);
  m_mouseAddSelectionAction->setChecked(false);
  m_mouseToggleSelectionAction->setChecked(false);
  m_mouseTooltipAction->setChecked(true);

  m_mouseHandler->setMode(MouseHandler::TooltipMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToZoomInSlot()
{
  m_mouseHandler->setMode(MouseHandler::ZoomInMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToZoomOutSlot()
{
  m_mouseHandler->setMode(MouseHandler::ZoomOutMode);
  m_mouseHandler->setCursor();
}

void MyDisplay::setMouseOperationToPanSlot()
{
  m_mouseHandler->setMode(MouseHandler::PanMode);
  m_mouseHandler->setCursor();
}


void MyDisplay::selectionChangedSlot(te::map::DataGridOperation* op)
{
  //std::list<te::map::AbstractLayer*> layerList;
  //mountLayerList(m_layerTree, layerList);
  //std::list<te::map::AbstractLayer*>::iterator it;
  //for(it = layerList.begin(); it != layerList.end(); ++it)
  //{
  //  MyLayer* layer = (MyLayer*)*it;
  //  if(layer->getDataGridOperation() == op)
  //  {
  //    //neste ponto o style do layer deve ser setado para depois desenhar...
  //    //.....vou fazer um renderer que usa DataGridOperation em vez do style
  //    //te::map::Layer* layer = (te::map::Layer*)al;

      setWaitCursor();
//      te::qt::widgets::Canvas* canvas = getCanvas(layer);
//      canvas->clear();

      draw();

      unsetWaitCursor();
  //    break;
  //  }
  //}
}

void MyDisplay::removeDrawOnlyChanged(te::map::AbstractLayer* al)
{
  m_drawOnlyChanged.erase(al);
}

void MyDisplay::addDrawOnlyChanged(te::map::AbstractLayer* al)
{
  m_drawOnlyChanged.insert(al);
}

// se o layer e' do tipo temporal
// existe apenas um canvas associado ao pai do layer.
te::qt::widgets::Canvas* MyDisplay::getCanvas(te::map::AbstractLayer* layer)
{
  MyLayer* l = (MyLayer*)layer;
  if(l->isTemporal())
  {
    te::map::AbstractLayer* parent = (te::map::AbstractLayer*)l->getParent();
    return te::qt::widgets::MapDisplay::getCanvas(parent);
  }
  return te::qt::widgets::MapDisplay::getCanvas(l);
}

// se o layer e' do tipo temporal
// existe apenas um canvas associado ao pai do layer.
void MyDisplay::setCanvas(te::map::AbstractLayer* layer)
{
  MyLayer* l = (MyLayer*)layer;
  if(l->isTemporal())
  {
    te::map::AbstractLayer* parent = (te::map::AbstractLayer*)l->getParent();    
    te::qt::widgets::MapDisplay::setCanvas(parent);
  }
  else
    te::qt::widgets::MapDisplay::setCanvas(l);
}


void MyDisplay::draw()
{
  te::qt::widgets::MapDisplay::draw();
}

void MyDisplay::draw(te::map::AbstractLayer* al)
{
  try
  {
    setWaitCursor();

    te::qt::widgets::Canvas* canvas = getCanvas(al);
//
//
//if(((MyLayer*)(al))->getId().find("crimes") != std::string::npos)
//{
//  std::vector<te::gm::Geometry*> ggs;
//  canvas->clear(ggs);
//}
//
//
//
//
//

    MyLayer* layer =  (MyLayer*)al;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0 && layer->isTemporal() == false)
    {
      std::string name = layer->getId();
      te::da::DataSource* ds = layer->getDataSource();
      te::da::DataSourceTransactor* t = ds->getTransactor();
      te::da::DataSet* dataSet = t->getDataSet(name);
      te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();
      te::da::DataSetType* dsType = loader->getDataSetType(name, true);
      if(dsType->getCatalog() == 0)
        dsType->setCatalog(ds->getCatalog());

      assert(dataSet);

      if(dsType->getPrimaryKey())
      {
        op = new te::map::DataGridOperation();
        op->init(dsType, dataSet);
        layer->setDataGridOperation(op);
      }
      delete loader;
  //?    delete t;
    }

    bool b = false;
    if(m_drawOnlyChanged.find(layer) != m_drawOnlyChanged.end())
      b = true;
    MyLayerRenderer* renderer = new MyLayerRenderer(b);
    layer->setRenderer(renderer);

    if(layer->isTemporal() == false)
      al->draw(canvas, *m_extent, m_srid);

    addDrawOnlyChanged(layer);
    unsetWaitCursor();
  }
  catch(std::exception& e)
  {
    unsetWaitCursor();
    QMessageBox::information(this, tr("Error Drawing..."), tr(e.what()));
    return;
  }
}

void MyDisplay::reorderDrawing(std::vector<te::map::AbstractLayer*> layers)
{
  try
  {
    setWaitCursor();

    m_displayPixmap->fill();
    QPainter painter(m_displayPixmap);

    std::vector<te::map::AbstractLayer*>::reverse_iterator it;
    for(it = layers.rbegin(); it != layers.rend(); ++it)
    {
      MyLayer* layer =  (MyLayer*)(*it);
      te::qt::widgets::Canvas* c = getCanvas(layer);
      //if(c == 0 || layer->getVisibility() == 0)
      //  continue;
      if(layer->getVisibility() == te::map::VISIBLE)
      {
        if(c == 0)
        {
          setCanvas(layer);
          c = getCanvas(layer);
          draw(layer);
        }
        painter.drawPixmap(0, 0, *(c->getPixmap()));
      }
    }
    unsetWaitCursor();
  }
  catch(std::exception& e)
  {
    unsetWaitCursor();
    QMessageBox::information(this, tr("Error Drawing..."), tr(e.what()));
    return;
  }
}

void MyDisplay::clearTemporalPixmaps(std::vector<te::map::AbstractLayer*> layers)
{
  std::set<te::map::AbstractLayer*> als;
  te::map::AbstractLayer* al;

  std::vector<te::map::AbstractLayer*>::iterator it;
  for(it = layers.begin(); it != layers.end(); ++it)
  {
    al = (te::map::AbstractLayer*)(*it)->getParent();
    if(als.find(al) == als.end())
    {
      als.insert(al);
      clearTemporalCanvas(*it);
    }
  }

  int w = m_displayPixmap->width();
  int h = m_displayPixmap->height();

  if(w != m_temporalVectorialDisplayPixmap->width() || h != m_temporalVectorialDisplayPixmap->height())
  {
    delete m_temporalVectorialDisplayPixmap;
    m_temporalVectorialDisplayPixmap = new QPixmap(w, h);
  }
  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));

  if(w != m_temporalImageDisplayPixmap->width() || h != m_temporalImageDisplayPixmap->height())
  {
    delete m_temporalImageDisplayPixmap;
    m_temporalImageDisplayPixmap = new QPixmap(w, h);
  }
  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));
}

void MyDisplay::clearTemporalCanvas(te::map::AbstractLayer* al)
{
  te::qt::widgets::Canvas* canvas = getCanvas(al);
  if(canvas)
    canvas->clear();
}

void MyDisplay::drawTemporalData(te::map::AbstractLayer* layer, std::vector<te::gm::Geometry*>& geoms, bool drawLines)
{
  MyLayer* l = (MyLayer*)layer;
  te::qt::widgets::Canvas* canvas = getCanvas(l);
  if(canvas == 0)
  {
    setCanvas(l);
    canvas = getCanvas(l);
  }
  te::map::AbstractLayer* parent = (te::map::AbstractLayer*)l->getParent();
  std::string pid = parent->getId();

  if(pid == "MovingObjects") // vectorial data
  {
    if(drawLines)
    {
      canvas->setLinePattern(0, 0, 0);
      canvas->setLineWidth(1);
      if(layer->getId() == "40: locations")
        canvas->setLineColor(te::color::RGBAColor(220, 220, 0, 255));
      else
        canvas->setLineColor(te::color::RGBAColor(0, 220, 220, 255));
    }
    else
    {
      canvas->setPointPattern(0, 0, 0);
      canvas->setPointWidth(1);
      if(layer->getId() == "40: locations")
      {
        //canvas->setPointMarker(te::map::MarkerPixel);
        //canvas->setPointMarkerColor(te::color::RGBAColor(220 ,0, 0, 255));
        canvas->setPointColor(te::color::RGBAColor(220 ,0, 0, 255));
      }
      else
      {
        //canvas->setPointMarker(te::map::MarkerPixel);
        //canvas->setPointMarkerColor(te::color::RGBAColor(0, 220, 0, 255));
        canvas->setPointColor(te::color::RGBAColor(0 , 220, 0, 255));
      }
    }

    std::vector<te::gm::Geometry*>::iterator it = geoms.begin();
    while(it != geoms.end())
    {
      // Verifies the SRID. Case differents, converts coordinates...
      if((*it)->getSRID() > 0)
        (*it)->transform(m_srid);
      canvas->draw(*it);
      ++it;
    }
    QPainter painter(m_temporalVectorialDisplayPixmap);
    painter.drawPixmap(0, 0, *(canvas->getPixmap()));
    update();
  }
  else // raster data
  {
    m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));
    canvas->clear();
    MyLayer* l = (MyLayer*)layer;
    te::da::DataSource* ds = l->getDataSource();
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    std::string name = l->getId();
    te::da::DataSet* dataSet = transactor->getDataSet(name);
    te::rst::Raster* raster = dataSet->getRaster();
    int ncols = raster->getNumberOfColumns();
    int nrows = raster->getNumberOfRows();
    const te::gm::Envelope* env = l->getExtent();
    QRectF recw(env->getLowerLeftX(), env->getLowerLeftY(), env->getWidth(), env->getHeight());
    QMatrix m = canvas->getMatrix();
    QRectF recd = m.mapRect(recw);

    int size = ncols * nrows;
    char *buf = new char[size];
    std::string namePNG = name;
    size_t p = namePNG.find(".tif");
    if(p != std::string::npos)
    {
      namePNG.erase(p, 4);
      namePNG += ".PNG";
    }
    std::string file = TE_DATA_EXAMPLE_LOCALE"/data/png/";
    file += namePNG;

    FILE* fp = fopen(file.c_str(), "rb");
    size_t bsize = fread(buf, sizeof(char), size, fp);
    canvas->drawImage(recd.x(), recd.y(), recd.width(), recd.height(), buf, bsize, te::map::PNG);

    fclose(fp);
    delete []buf;

    delete dataSet;
    QPainter painter(m_temporalImageDisplayPixmap);
    painter.drawPixmap(0, 0, *(canvas->getPixmap()));
    update();
  }

}

void MyDisplay::paintEvent(QPaintEvent* e)
{
  te::qt::widgets::MapDisplay::paintEvent(e);

  QRect rec, wrec;
  QMatrix m;
  QPointF p1, p2;
  te::qt::widgets::Canvas *c = 0;

  if(m_resize == false)
  {
    if(m_repaint)
    {
      QPainter painter(this);
      QRect rec = e->rect();
      painter.drawPixmap(rec, *m_temporalVectorialDisplayPixmap, rec);
      painter.drawPixmap(rec, *m_temporalImageDisplayPixmap, rec);
      painter.drawPixmap(0, 0, *m_tooltipDisplayPixmap);
    }
  }
  else // fazendo resize do display com os pixmaps temporais
  {
    if(m_layerCanvasMap.begin() != m_layerCanvasMap.end())
      c = m_layerCanvasMap.begin()->second;

    if(c == 0)
      return;

    QPainter painter(this);
    painter.drawPixmap(m_resizeRec, *m_temporalVectorialDisplayPixmap, m_resizeWRec);
    painter.drawPixmap(m_resizeRec, *m_temporalImageDisplayPixmap, m_resizeWRec);
    painter.drawPixmap(0, 0, *m_tooltipDisplayPixmap);
  }
}

void MyDisplay::setExtent(const te::gm::Envelope& e)
{
  //m_useChanged = false;
  m_drawOnlyChanged.clear();
  te::qt::widgets::MapDisplay::setExtent(e);
  //if(e.isValid())
  //  m_useChanged = true;
  clearTooltipPixmap();
  if(m_timeSlider)
    clearTemporalPixmaps(m_timeSlider->getLayers());
}

void MyDisplay::setSRID(const int& srid)
{
  //m_useChanged = false;
  m_drawOnlyChanged.clear();
  te::map::MapDisplay::setSRID(srid);
  //m_useChanged = true;
}

void MyDisplay::changeObjectStatus(QRect rec, const std::string& mode)
{
  QRectF recWorld, recDev(rec);
  std::list<te::map::AbstractLayer*> layerList;
  mountLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  setWaitCursor();
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;

    te::qt::widgets::Canvas* canvas = getCanvas(layer);
    recWorld = canvas->getMatrix().inverted().mapRect(recDev);
    if(recWorld.isValid() == false)
      continue;

    te::gm::Envelope mouseEnv(recWorld.bottomLeft().x(), recWorld.topRight().y(), recWorld.topRight().x(), recWorld.bottomLeft().y());
    if(layer->getExtent())
    {
      te::gm::Envelope layerEnv(*layer->getExtent());
      layerEnv.transform(layer->getSRID(), m_srid);
      if(layerEnv.intersects(mouseEnv) == false)
        continue;
    }

    te::gm::Polygon* mousePolyRec = new te::gm::Polygon(0, te::gm::PolygonType);
    te::gm::LinearRing* line = new te::gm::LinearRing(5, te::gm::LineStringType);
    line->setPoint(0, recWorld.bottomLeft().x(), recWorld.topRight().y()); // lower left
    line->setPoint(1, recWorld.topLeft().x(), recWorld.bottomLeft().y()); // upper left
    line->setPoint(2, recWorld.topRight().x(), recWorld.bottomRight().y()); // upper rigth
    line->setPoint(3, recWorld.bottomRight().x(), recWorld.topRight().y()); // lower rigth
    line->setPoint(4, recWorld.bottomLeft().x(), recWorld.topRight().y()); // closing
    mousePolyRec->push_back(line);
   
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
    std::string pkName = pkProps[0]->getName();
    int pkPos = dsType->getPropertyPosition(pkName);
    std::string pkv;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
    int gtype = dsType->getDefaultGeomProperty()->getGeometryType();

    std::vector<int> visRows;
    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      pkv = dataSet->getAsString(pkPos);

      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      g->transform(m_srid);
      if(rec.width() == 1 && rec.height() == 1)
      {
        te::gm::Point p(recWorld.x(), recWorld.y());
        if(g->contains(&p))
        {
          visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
          delete g;
          break;
        }
      }
      else if(gtype == te::gm::PointType || gtype == te::gm::MultiPointType)
      {
        QPointF pg(((te::gm::Point*)g)->getX(), ((te::gm::Point*)g)->getY());
        if(recWorld.contains(pg))
          visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
      }
      else if(gtype == te::gm::LineStringType || gtype == te::gm::MultiLineStringType)
      { 
        if(g->intersects(mousePolyRec))
          visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
      }
      else if(gtype == te::gm::PolygonType || gtype == te::gm::MultiPolygonType)
      {
        if(g->intersects(mousePolyRec))
          visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
      }

      delete g;
    }
    delete mousePolyRec;

    if(visRows.empty())
      continue;

    if(mode == "POINTING")
      op->setRowsAsPointed(visRows);
    else if(mode == "ADDPOINTING")
      op->addRowsToPointed(visRows);
    else if(mode == "TOGGLEPOINTING")
      op->toggleRowsPointingStatus(visRows);
    Q_EMIT selectionChanged(op);
  }
  unsetWaitCursor();
}

void MyDisplay::execZoomAreaSlot(const QRect& rec)
{
  try
  {
    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
    if(canvas == NULL)
      return;

    QPointF pll(rec.left(), rec.bottom());
    QPointF pur(rec.right(), rec.top());

    pll = canvas->getMatrix().inverted().map(pll);
    pur = canvas->getMatrix().inverted().map(pur);
    te::gm::Envelope envelope(pll.x(), pll.y(), pur.x(), pur.y());

    setExtent(envelope);

    te::gm::Envelope env = getAllExtent();
    if(m_extent && env.contains(*m_extent))
      m_envelope = *m_extent;
    draw();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::execZoomInSlot(const QPoint& p)
{
  try
  {
    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
    if(canvas == NULL)
      return;

    QRect r(0, 0, canvas->getWidth(), canvas->getHeight());
    r.setWidth(r.width() / 2);
    r.setHeight(r.height() / 2);
    r.moveCenter(p);
    QPointF pll(r.left(), r.bottom());
    QPointF pur(r.right(), r.top());

    pll = canvas->getMatrix().inverted().map(pll);
    pur = canvas->getMatrix().inverted().map(pur);
    te::gm::Envelope envelope(pll.x(), pll.y(), pur.x(), pur.y());

    setExtent(envelope);

    te::gm::Envelope env = getAllExtent();
    if(m_extent && env.contains(*m_extent))
      m_envelope = *m_extent;
    draw();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::execZoomOutSlot(const QPoint& p)
{
  try
  {
    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
    if(canvas == NULL)
      return;

    QRect rec(0, 0, canvas->getWidth(), canvas->getHeight());
    rec.setWidth(rec.width() * 2);
    rec.moveCenter(p);
    QPointF pll(rec.left(), rec.bottom());
    QPointF pur(rec.right(), rec.top());

    pll = canvas->getMatrix().inverted().map(pll);
    pur = canvas->getMatrix().inverted().map(pur);
    te::gm::Envelope envelope(pll.x(), pll.y(), pur.x(), pur.y());
    setExtent(envelope);

    te::gm::Envelope env = getAllExtent();
    if(m_extent && env.contains(*m_extent))
      m_envelope = *m_extent;
    draw();
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::execPanSlot(const QPoint& from, const QPoint& to)
{
  try
  {
    te::qt::widgets::Canvas *canvas = m_layerCanvasMap.begin()->second;
    if(canvas == NULL)
      return;
  
    QPoint p = to - from;
    QRect rec(0, 0, canvas->getWidth(), canvas->getHeight());
    QPoint c = rec.center();
    c -= p;
    rec.moveCenter(c);

    QPointF pll(rec.left(), rec.bottom());
    QPointF pur(rec.right(), rec.top());

    pll = canvas->getMatrix().inverted().map(pll);
    pur = canvas->getMatrix().inverted().map(pur);
    te::gm::Envelope envelope(pll.x(), pll.y(), pur.x(), pur.y());

    setExtent(envelope);
    draw();

    te::gm::Envelope env = getAllExtent();
    if(m_extent && env.contains(*m_extent))
      m_envelope = *m_extent;
  }
  catch(te::common::Exception& e)
  {
    throw(te::common::Exception(e.what()));
  }
}

void MyDisplay::mouseSelectionSlot(QRect rec)
{
  changeObjectStatus(rec, "POINTING");
}

void MyDisplay::mouseAddSelectionSlot(QRect rec)
{
  changeObjectStatus(rec, "ADDPOINTING");
}

void MyDisplay::mouseToggleSelectionSlot(QRect rec)
{
  changeObjectStatus(rec, "TOGGLEPOINTING");
}

void MyDisplay::mouseTooltipSlot(QPoint p)
{
  std::list<te::map::AbstractLayer*> layerList;
  mountLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;

    std::vector<int> tooltipColumns = layer->getTooltipColumns();
    if(tooltipColumns.empty())
      continue;
    
    te::qt::widgets::Canvas* canvas = getCanvas(layer);
    if(canvas == 0)
      continue;

    QRectF mouseRectF(0., 0., 5., 5.);
    mouseRectF.moveCenter(p);     
    mouseRectF = canvas->getMatrix().inverted().mapRect(mouseRectF);
    te::gm::Polygon* poly = new te::gm::Polygon(0, te::gm::PolygonType);
    te::gm::LinearRing* line = new te::gm::LinearRing(5, te::gm::LineStringType);

    line->setPoint(0, mouseRectF.bottomLeft().x(), mouseRectF.topLeft().y()); // lower left
    line->setPoint(1, mouseRectF.topLeft().x(), mouseRectF.bottomLeft().y()); // upper left
    line->setPoint(2, mouseRectF.topRight().x(), mouseRectF.bottomRight().y()); // upper rigth
    line->setPoint(3, mouseRectF.bottomRight().x(), mouseRectF.topRight().y()); // lower rigth
    line->setPoint(4, mouseRectF.bottomLeft().x(), mouseRectF.topLeft().y()); // closing
    poly->push_back(line);
   
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();
    te::gm::GeomType gtype = dsType->getDefaultGeomProperty()->getGeometryType();

    QString val;
    std::vector<QString> values;
    std::vector<QString>::reverse_iterator its;
    std::vector<int>::iterator it;
    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      g->transform(m_srid);
      if(gtype == te::gm::PolygonType || gtype == te::gm::MultiPolygonType)
      {
        QPointF pw = canvas->getMatrix().inverted().map(p);
        te::gm::Point mouseWPos(pw.x(), pw.y());
        if(g->contains(&mouseWPos))
        {
          delete g;
          for(it = tooltipColumns.begin(); it != tooltipColumns.end(); ++it)
          {
            val = dsType->getProperty(*it)->getName().c_str();
            val += ": ";
            if(dataSet->isNull(*it) == false)
              val += dataSet->getAsString(*it).c_str();
            values.push_back(val);
          }
          break;
        }
      }
      else if(gtype == te::gm::PointType || gtype == te::gm::MultiPointType)
      {
        QRect mouseRect(0, 0, 5, 5);
        mouseRect.moveCenter(p);     
        QPointF gwp(static_cast<const te::gm::Point*>(g)->getX(), static_cast<const te::gm::Point*>(g)->getY());
        QPoint gdp(canvas->getMatrix().map(gwp).toPoint());

        if(mouseRect.contains(gdp))
        {
          delete g;
          for(it = tooltipColumns.begin(); it != tooltipColumns.end(); ++it)
          {
            val = dsType->getProperty(*it)->getName().c_str();
            val += ": ";
            if(dataSet->isNull(*it) == false)
              val += dataSet->getAsString(*it).c_str();
            values.push_back(val);
          }
          break;
        }
      }
      else if(gtype == te::gm::LineStringType || gtype == te::gm::MultiLineStringType)
      {
        if(g->crosses(poly))
        {
          delete g;
          for(it = tooltipColumns.begin(); it != tooltipColumns.end(); ++it)
          {
            val = dsType->getProperty(*it)->getName().c_str();
            val += ": ";
            if(dataSet->isNull(*it) == false)
              val += dataSet->getAsString(*it).c_str();
            values.push_back(val);
          }
          break;
        }
      }
      delete g;
    }
    delete poly;

    int w = m_displayPixmap->width();
    int h = m_displayPixmap->height();
    if(m_tooltipDisplayPixmap->width() != w || m_tooltipDisplayPixmap->height() != h)
    {
      delete m_tooltipDisplayPixmap;
      m_tooltipDisplayPixmap = new QPixmap(w, h);
      clearTooltipPixmap();
      m_tooltipRect = QRect();
    }

    QPainter painter(m_tooltipDisplayPixmap);
    if(m_tooltipRect.isValid())
    {
      painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
      painter.setPen(Qt::black);
      painter.setBrush(Qt::black);
      painter.drawRect(m_tooltipRect);
      painter.setCompositionMode(QPainter::CompositionMode_Source);
      m_tooltipRect = QRect();
    }

    if(values.empty() == false)
    {
      QPoint mp(p);
      for(its = values.rbegin(); its != values.rend(); ++its)
      {
        QRect trect;
        QRect rect(0, 0, 1000, 40);
        QPoint pp(mp.x(), mp.y() + 7);
        rect.moveCenter(pp);
        painter.drawText(rect, Qt::AlignHCenter, *its, &trect);
        trect.setLeft(trect.left() - 2);
        trect.setWidth(trect.width() + 3);
        m_tooltipRect = m_tooltipRect.united(trect);
        mp.setY(mp.y() - trect.height());
      }
    }
    repaint();
  }
}

void MyDisplay::drawAllPointedsSlot()
{
  te::qt::widgets::Canvas* canvas;
  QRectF recF;
  int status;
  std::list<te::map::AbstractLayer*> layerList;
  mountLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  setWaitCursor();
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;
    canvas = getCanvas(layer);
   
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
    std::string pkName = pkProps[0]->getName();
    int pkPos = dsType->getPropertyPosition(pkName);
    std::string pkv;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();

    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      pkv = dataSet->getAsString(pkPos);
      status = op->getDataSetSelectionStatus(pkv);
      if(status == te::map::DataGridOperation::POINTED || status == te::map::DataGridOperation::POINTED_AND_QUERIED)
      {
        g->transform(m_srid);
        const te::gm::Envelope* env = g->getMBR();
        QRectF r(env->m_llx, env->m_lly, env->getWidth(), env->getHeight());
        recF |= r;
      }
      delete g;
    }
  }

  if(recF.isValid())
  {
    recF = canvas->getMatrix().mapRect(recF);
    QRect rec(recF.x(), recF.y(), recF.width(), recF.height());
    execZoomAreaSlot(rec);
  }
  unsetWaitCursor();
}

void MyDisplay::drawAllQueriedsSlot()
{
  te::qt::widgets::Canvas* canvas;
  QRectF recF;
  int status;
  std::list<te::map::AbstractLayer*> layerList;
  mountLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  setWaitCursor();
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;
    canvas = getCanvas(layer);
   
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
    std::string pkName = pkProps[0]->getName();
    int pkPos = dsType->getPropertyPosition(pkName);
    std::string pkv;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();

    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      pkv = dataSet->getAsString(pkPos);
      status = op->getDataSetSelectionStatus(pkv);
      if(status == te::map::DataGridOperation::QUERIED || status == te::map::DataGridOperation::POINTED_AND_QUERIED)
      {
        g->transform(m_srid);
        const te::gm::Envelope* env = g->getMBR();
        QRectF r(env->m_llx, env->m_lly, env->getWidth(), env->getHeight());
        recF |= r;
      }
      delete g;
    }
  }

  if(recF.isValid())
  {
    recF = canvas->getMatrix().mapRect(recF);
    QRect rec(recF.x(), recF.y(), recF.width(), recF.height());
    execZoomAreaSlot(rec);
  }
  unsetWaitCursor();
}

void MyDisplay::drawAllPointedsAndQueriedsSlot()
{
  te::qt::widgets::Canvas* canvas;
  QRectF recF;
  int status;
  std::list<te::map::AbstractLayer*> layerList;
  mountLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  if(layerList.size() == 0)
    return;

  setWaitCursor();
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    MyLayer* layer = (MyLayer*)*it;
    te::map::DataGridOperation* op = layer->getDataGridOperation();
    if(op == 0)
      continue;
    canvas = getCanvas(layer);
   
    te::da::DataSet* dataSet = op->getDataSet();
    te::da::DataSetType* dsType = op->getDataSetType();
    te::da::PrimaryKey *pk = dsType->getPrimaryKey();
    const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
    std::string pkName = pkProps[0]->getName();
    int pkPos = dsType->getPropertyPosition(pkName);
    std::string pkv;
    std::size_t gPos = dsType->getDefaultGeomPropertyPos();

    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      te::gm::Geometry* g = dataSet->getGeometry(gPos);
      if(g == 0)
        continue;

      pkv = dataSet->getAsString(pkPos);
      status = op->getDataSetSelectionStatus(pkv);
      if(status == te::map::DataGridOperation::POINTED_AND_QUERIED)
      {
        g->transform(m_srid);
        const te::gm::Envelope* env = g->getMBR();
        QRectF r(env->m_llx, env->m_lly, env->getWidth(), env->getHeight());
        recF |= r;
      }
      delete g;
    }
  }

  if(recF.isValid())
  {
    recF = canvas->getMatrix().mapRect(recF);
    QRect rec(recF.x(), recF.y(), recF.width(), recF.height());
    execZoomAreaSlot(rec);
  }
  unsetWaitCursor();
}

void MyDisplay::setTimeSlider(TimeSlider* t)
{
  m_timeSlider = t;
}

void MyDisplay::printSlot()
{
  QPrintDialog w(this);
  
  if(w.exec() ==  QDialog::Rejected)
    return;

  QPrinter* printer = w.printer();

  int fx = printer->physicalDpiX();
  int fy = printer->physicalDpiY();
  printer->setResolution(fx);  // precisa fazer isto para que a impressora tenha a resolucao configurada
                               // na interface. Caso contrario a resolucao sempre sera´ 96 dpi.
  print(printer);
}

void MyDisplay::printFileSlot()
{
  PrintInFile w(this);
  if(w.exec() ==  QDialog::Rejected)
    return;

  QPrinter printer(QPrinter::HighResolution);
  printer.setOutputFileName(w.m_fileLineEdit->text());

  if(w.m_fileFormatComboBox->currentText() == "PDF")
    printer.setOutputFormat(QPrinter::PdfFormat);
  else
    printer.setOutputFormat(QPrinter::PostScriptFormat);

  double width = w.m_paperWidthLineEdit->text().toDouble();
  double height = w.m_paperHeightLineEdit->text().toDouble();
  QSizeF paperSize(width, height);     
  printer.setPaperSize(paperSize, QPrinter::Millimeter);
  double left = w.m_leftMarginSpinBox->text().toDouble();
  double top = w.m_rightMarginSpinBox->text().toDouble();
  double right = w.m_topMarginSpinBox->text().toDouble();
  double bottom = w.m_bottomMarginSpinBox->text().toDouble();
  printer.setPageMargins(left, top, right, bottom, QPrinter::Millimeter);

  int resolution = w.m_resolutionComboBox->currentText().toInt();
  printer.setResolution(resolution);
  int r = printer.resolution();

  if(w.m_paperOrientationComboBox->currentText() == "Portrait")
    printer.setOrientation(QPrinter::Portrait);
  else
    printer.setOrientation(QPrinter::Landscape);

  QString file = w.m_fileLineEdit->text();
  printer.setOutputFileName(file);

  print(&printer);
}

void MyDisplay::print(QPrinter* printer)
{
  setWaitCursor();
  QSizeF paperSizeMM = printer->paperSize(QPrinter::Millimeter);

  int resolution = printer->resolution();
  QPainter painter(printer); // so pode inicializar painter depois de configurar a resolucao da impressora

  QPrinter::Orientation orientation = printer->orientation();
  int width, height;

  int pwidth = (double)printer->widthMM() * (double)resolution / 25.4;
  int pheight = (double)printer->heightMM() * (double)resolution / 25.4;

  double left, top, right, bottom;
  printer->getPageMargins (&left, &top, &right, &bottom, QPrinter::Millimeter);
  left = (double)left * (double)resolution / 25.4;
  top = (double)top * (double)resolution / 25.4;
  right = (double)right * (double)resolution / 25.4;
  bottom = (double)bottom * (double)resolution / 25.4;

  pwidth -= (left + right);
  pheight -= (top + bottom);

  int mwidth = m_displayPixmap->width();
  int mheight = m_displayPixmap->height();

  double a = (double)mwidth / (double)mheight;
  double b = (double)pwidth / (double)pheight;
  
  if(a < b)
  {
    width = (int)((double)pheight * a + .5);
    height = pheight;
  }
  else
  {
    height = (int)((double)pwidth / a + .5);
    width = pwidth;
  }

  left = (pwidth - width) / 2;
  top = (pheight - height) / 2;

  //unsigned long memAvailable = te::common::GetFreePhysicalMemory();

  int n = 1;
  te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(width, height/n);
  while(canvas->getWidth() == 0 || canvas->getHeight() == 0)
  {
    delete canvas;
    ++n;
    canvas = new te::qt::widgets::Canvas(width, height/n);
  }

  if(n == 1)
  {
    te::gm::Envelope e(*m_extent);
    canvas->calcAspectRatio(e.m_llx, e.m_lly, e.m_urx, e.m_ury, m_hAlign, m_vAlign);
    canvas->setWindow(e.m_llx, e.m_lly, e.m_urx, e.m_ury);

    std::list<te::map::AbstractLayer*> layerList;
    std::list<te::map::AbstractLayer*>::iterator it;
    mountLayerList(m_layerTree, layerList);

    for(it = layerList.begin(); it != layerList.end(); ++it)
    {
      MyLayer* layer =  (MyLayer*)(*it);
      MyLayerRenderer* renderer = new MyLayerRenderer(false);
      layer->setRenderer(renderer);
      layer->draw(canvas, e, m_srid);

      painter.drawPixmap(left, top, *(canvas->getPixmap()));
    }
  }
  else
  {
    // 4096 x 4096 pixels
    int w = 4096;
    int h = 4096;
    int wn = (int)((double)width / (double)w + 1.);
    n =  (int)((double)height / (double)h + 1.);

    double y1, y2;
    double x1 = m_extent->getLowerLeftX();
    double x2 = m_extent->getUpperRightX();

    delete canvas;
    canvas = new te::qt::widgets::Canvas(w, h);
    QPixmap pix(w, h);
    QPainter painterAux; 
    te::gm::Envelope env(*m_extent);
    double eh = m_extent->getHeight() / (double)n;
    double ew = m_extent->getWidth() / (double)wn;

    y2 = m_extent->getUpperRightY();
    y1 = y2 - eh;
    x1 = m_extent->getLowerLeftX();
    x2 = x1 + ew;
    canvas->setWindow(x1, y1, x2, y2);
    canvas->calcAspectRatio(x1, y1, x2, y2, m_hAlign, m_vAlign);
    double x1ini = x1;
    eh = y2 - y1;
    ew = x2 - x1;
    double wmin = ew / (double)w;
    double hmin = eh / (double)h;

    double d = (m_extent->getUpperRightY() - y1) / (y2 - y1);
    double ssy = h - (int)(d * (double)h + .5);
    d = (m_extent->getLowerLeftX() - x1) / (x2 - x1);
    double ssx = (int)(d * (double)w + .5);

    int yini = top;
    while((y1 + eh) > m_extent->getLowerLeftY())
    {
      int xini = left;
      x1 = x1ini;
      while(x1 < m_extent->getUpperRightX())
      {
        canvas->setWindow(x1, y1, x1+ew, y1+eh);
        env.init(x1, y1, x1+ew, y1+eh);
        pix.fill(QColor(0, 0, 0, 0));
        painterAux.begin(&pix);

        std::list<te::map::AbstractLayer*> layerList;
        std::list<te::map::AbstractLayer*>::iterator it;
        mountLayerList(m_layerTree, layerList);

        for(it = layerList.begin(); it != layerList.end(); ++it)
        {
          MyLayer* layer =  (MyLayer*)(*it);
          MyLayerRenderer* renderer = new MyLayerRenderer(false); // clear canvas
          layer->setRenderer(renderer);
          layer->draw(canvas, env, m_srid);

          int sx = 0;
          int sy = 0;
          int dx = w;
          int dy = h;
          if(yini == (int)top)
          {
            sy = ssy;
            dy = 0;
          }
          if(xini == (int)left)
          {
            sx = ssx;
            dx = 0;
          }
          if((x1 + ew) > m_extent->getUpperRightX())
          {
            QPointF pf(m_extent->getUpperRightX(), m_extent->getUpperRightY());
            QPoint p(canvas->getMatrix().map(pf).toPoint());
            dx = p.x();
          }
          if(y1 < m_extent->getLowerLeftY())
          {
            QPointF pf(m_extent->getLowerLeftX(), m_extent->getLowerLeftY());
            QPoint p(canvas->getMatrix().map(pf).toPoint());
            dy = p.y();
          }

          painterAux.drawPixmap(sx, sy, *canvas->getPixmap(), sx, sy, dx, dy);
        }

        painterAux.end();
        painter.drawPixmap(xini, yini, pix);

        xini += w;
        x1 += (ew + wmin);
      }

      yini += h;
      y1 -= (eh + hmin);
    }
  }
  painter.end();
  delete canvas;

  unsetWaitCursor();
}

void MyDisplay::clearTooltipPixmap()
{
  m_tooltipDisplayPixmap->fill(QColor(255, 255, 255, 0));
}