#include "MyDisplay.h"
#include "MyLayer.h"
#include "MyLayerRenderer.h"
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

MyDisplay::MyDisplay(int w, int h, te::map::AbstractLayer* root, QWidget* parent, Qt::WindowFlags f) :
  te::qt::widgets::MapDisplay(w, h, parent, f),
  m_rootFolderLayer(root),
  m_init(true),
  m_timeSlider(0)
{
  setAcceptDrops(true);

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

  m_mouseHandler = new MouseHandler(this);

  connect(m_mouseHandler, SIGNAL(execZoomArea(QRect)), this, SLOT(execZoomAreaSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execZoomIn(QPoint)), this, SLOT(execZoomInSlot(QPoint)));
  connect(m_mouseHandler, SIGNAL(execZoomOut(QPoint)), this, SLOT(execZoomOutSlot(QPoint)));
  connect(m_mouseHandler, SIGNAL(execPan(QPoint, QPoint)), this, SLOT(execPanSlot(QPoint, QPoint)));
  connect(m_mouseHandler, SIGNAL(execSelection(QRect)), this, SLOT(mouseSelectionSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execAddSelection(QRect)), this, SLOT(mouseAddSelectionSlot(QRect)));
  connect(m_mouseHandler, SIGNAL(execToggleSelection(QRect)), this, SLOT(mouseToggleSelectionSlot(QRect)));

  m_mouseHandler->setCursor();
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
  if(m_init)
  {
    m_init = false;
    return;
  }
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
      if(c == 0 || layer->getVisibility() == 0)
        continue;
      painter.drawPixmap(0, 0, *(c->getPixmap()));
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
      if(layer->getId() == "40: locations")
        canvas->setLineColor(te::color::RGBAColor(220, 220, 0, 255));
      else
        canvas->setLineColor(te::color::RGBAColor(0, 220, 220, 255));
    }
    else
    {
      if(layer->getId() == "40: locations")
      {
        //canvas->setPointMarker(te::map::MarkerPixel);
        //canvas->setPointMarkerColor(te::color::RGBAColor(220 ,0, 0, 255));
      }
      else
      {
        //canvas->setPointMarker(te::map::MarkerPixel);
        //canvas->setPointMarkerColor(te::color::RGBAColor(0, 220, 0, 255));
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
  }
}

void MyDisplay::setExtent(const te::gm::Envelope& e)
{
  //m_useChanged = false;
  m_drawOnlyChanged.clear();
  te::qt::widgets::MapDisplay::setExtent(e);
  //if(e.isValid())
  //  m_useChanged = true;
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
      else if(!(gtype == te::gm::PointType || gtype == te::gm::PointMType || gtype == te::gm::PointKdType || gtype == te::gm::PointZType || gtype == te::gm::PointZMType))
      {
        const te::gm::Envelope* env = g->getMBR();
        QRectF r(env->m_llx, env->m_lly, env->getWidth(), env->getHeight());
        if(recWorld.intersects(r))
          visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
      }
      else
      {
        const te::gm::Envelope* env = g->getMBR();
        QPointF pf(env->m_llx, env->m_lly);
        if(recWorld.contains(pf))
          visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
      }

      delete g;
    }
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