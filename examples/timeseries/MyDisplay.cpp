#include "MyDisplay.h"
#include "MyLayer.h"
#include "MyLayerRenderer.h"
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/raster.h>

#include "STExamples.h"

//QT
#include <QPaintEvent>
#include <QMessageBox>
#include <QString>
#include <QMatrix>
#include <QRectF>

MyDisplay::MyDisplay(int w, int h, QWidget* parent, Qt::WindowFlags f) :
  te::qt::widgets::MapDisplay(w, h, parent, f),
  m_useChanged(false),
  m_timeSlider(0)
{
  m_temporalVectorialDisplayPixmap = new QPixmap(w, h);
  m_temporalVectorialDisplayPixmap->fill(QColor(255, 255, 255, 0));
  m_temporalImageDisplayPixmap = new QPixmap(w, h);
  m_temporalImageDisplayPixmap->fill(QColor(255, 255, 255, 0));

  QMenu *menu = this->getMenu();
  QAction* drawAllPointedsAction = new QAction("Fit All &Pointeds", menu);
  menu->addAction(drawAllPointedsAction);
  connect(drawAllPointedsAction, SIGNAL(triggered()), this, SLOT(drawAllPointedsSlot()));

  QAction* drawAllQueriedsAction = new QAction("Fit All &Querieds", menu);
  menu->addAction(drawAllQueriedsAction);
  connect(drawAllQueriedsAction, SIGNAL(triggered()), this, SLOT(drawAllQueriedsSlot()));

  QAction* drawAllPointedsAndQueriedsAction = new QAction("Fit &All Pointeds And Querieds", menu);
  menu->addAction(drawAllPointedsAndQueriedsAction);
  connect(drawAllPointedsAndQueriedsAction, SIGNAL(triggered()), this, SLOT(drawAllPointedsAndQueriedsSlot()));

  te::qt::widgets::ZoomAndPan* zp = this->getZoomAndPan();
  connect(zp, SIGNAL(execSelection(QRect)), this, SLOT(mouseSelectionSlot(QRect)));
  connect(zp, SIGNAL(execAddSelection(QRect)), this, SLOT(mouseAddSelectionSlot(QRect)));
}

MyDisplay::~MyDisplay()
{
  delete   m_temporalVectorialDisplayPixmap;
  delete   m_temporalImageDisplayPixmap;
  disconnect();
}

void MyDisplay::closeEvent(QCloseEvent *event)
{
  setAttribute(Qt::WA_DeleteOnClose);
  Q_EMIT closed(this);
  event->accept();
}

void MyDisplay::selectionChangedSlot(te::map::DataGridOperation* op)
{
  te::da::DataSetType* type = op->getDataSetType();
  std::string name = type->getName();

  std::list<te::map::AbstractLayer*> layerList;
  mountLayerList(m_layerTree, layerList);

  std::list<te::map::AbstractLayer*>::iterator it;
  for(it = layerList.begin(); it != layerList.end(); ++it)
  {
    te::map::Layer* layer = (te::map::Layer*)*it;
    std::string title = layer->getTitle();
    if(name == title)
    {
      //neste ponto o style do layer deve ser setado para depois desenhar...
      //.....vou fazer um renderer que usa DataGridOperation em vez do style
      //te::map::Layer* layer = (te::map::Layer*)al;
      //if(m_layerDrawMap.find(layer) == m_layerDrawMap.end())
      //  m_layerDrawMap[layer] = true;

      setWaitCursor();
//      te::qt::widgets::Canvas* canvas = getCanvas(layer);
//      canvas->clear();

      layerStyleChanged(layer);
      draw();

      unsetWaitCursor();
      break;
    }
  }
}

void MyDisplay::deleteCanvas(te::map::AbstractLayer* layer)
{
  MyLayer* l = (MyLayer*)layer;
  if(l->isTemporal() == false)
  {
    te::qt::widgets::Canvas* c = te::qt::widgets::MapDisplay::getCanvas(l);
    if(c)
    {
      m_layerCanvasMap.erase(l);
      delete c;
    }
  }
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
  //    delete t;
    }
    if(layer->getRenderer() == 0)
    {
      MyLayerRenderer* renderer = new MyLayerRenderer(false);
      ((te::map::Layer*)al)->setRenderer(renderer);
    }
    else
    {
      MyLayerRenderer* renderer = new MyLayerRenderer(m_useChanged);
      ((te::map::Layer*)al)->setRenderer(renderer);
    }

    if(((MyLayer*)al)->isTemporal() == false)
      al->draw(canvas, *m_extent, m_srid);
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
        canvas->setPointMarker(te::map::MarkerPixel);
        canvas->setPointMarkerColor(te::color::RGBAColor(220 ,0, 0, 255));
      }
      else
      {
        canvas->setPointMarker(te::map::MarkerPixel);
        canvas->setPointMarkerColor(te::color::RGBAColor(0, 220, 0, 255));
      }
    }

    std::vector<te::gm::Geometry*>::iterator it = geoms.begin();
    while(it != geoms.end())
    {
      // Verifies the SRID. Case differents, converts coordinates...
      if((*it)->getSRID() > 0)
      {
        if(m_srid < 0)
        {
          std::list<te::map::AbstractLayer*> layerList;
          mountLayerList(m_layerTree, layerList);

          std::list<te::map::AbstractLayer*>::iterator it;
          delete m_extent;
          m_extent = new te::gm::Envelope();
          for(it = layerList.begin(); it != layerList.end(); ++it)
          {
            te::gm::Envelope env = getLayerExtent(*it);

            int srid = (*it)->getSRID();
            if(m_srid < 0)
              m_srid = srid;

            if(srid != m_srid)
              env.transform(srid, m_srid);
            m_extent->Union(env);
          }
          setSRID(m_srid);
          setExtent(*m_extent);
        }
      }
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

  double w, h, nw, nh, sw, sh;
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
  else
  {
    if(m_layerCanvasMap.begin() != m_layerCanvasMap.end())
      c = m_layerCanvasMap.begin()->second;

    if(c == 0)
      return;

    m = c->getMatrix();

    p1 = m.map(QPointF(m_envelope.getLowerLeftX(), m_envelope.getLowerLeftY()));
    p2 = m.map(QPointF(m_envelope.getUpperRightX(), m_envelope.getUpperRightY()));
    wrec = QRect(QPoint(p1.x(), p2.y()), QPoint(p2.x(), p1.y()));

    QPainter painter(this);
    rec = rect();

    w = rec.width();
    h = rec.height();
    nw = wrec.width();
    nh = wrec.height();

    sh = h/wrec.height();
    sw = w/wrec.width();
    if(sw < sh)
    {
      nw *= sw;
      nh *= sw;
    }
    else if(sh < sw)
    {
      nw *= sh;
      nh *= sh;
    }

    if(m_hAlign == te::map::Center)
    {
      rec.setLeft((w - nw) / 2);
      rec.setRight(rec.left() + nw);
    }
    else if(m_hAlign == te::map::Left)
      rec.setRight(nw);
    else if(m_hAlign == te::map::Right)
      rec.setLeft(rec.width() - nw);

    if(m_vAlign == te::map::Center)
    {
      rec.setTop((h - nh) / 2);
      rec.setBottom(rec.top() + nh);
    }
    else if(m_vAlign == te::map::Top)
      rec.setBottom(nh);
    else if(m_vAlign == te::map::Bottom)
      rec.setTop(rec.height() - nh);

    painter.drawPixmap(rec, *m_temporalVectorialDisplayPixmap, wrec);
    painter.drawPixmap(rec, *m_temporalImageDisplayPixmap, wrec);
  }
}

void MyDisplay::setExtent(const te::gm::Envelope& e)
{
  m_useChanged = false;
  te::qt::widgets::MapDisplay::setExtent(e);
  m_useChanged = true;
  if(m_timeSlider)
    clearTemporalPixmaps(m_timeSlider->getLayers());
}

void MyDisplay::setSRID(const int& srid)
{
  m_useChanged = false;
  te::map::MapDisplay::setSRID(srid);
  m_useChanged = true;
}

void MyDisplay::mouseSelectionSlot(QRect rec)
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

    op->setRowsAsPointed(visRows);
    layerStyleChanged(layer);
    draw();       
    Q_EMIT selectionChanged(op);
  }
  unsetWaitCursor();
}

void MyDisplay::mouseAddSelectionSlot(QRect rec)
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
      else
      {
        const te::gm::Envelope* env = g->getMBR();
        QRectF r(env->m_llx, env->m_lly, env->getWidth(), env->getHeight());
        if(recWorld.intersects(r))
          visRows.push_back(op->getVisualRow(op->getLogicalRow(pkv)));
      }

      delete g;
    }
    if(visRows.empty())
      continue;

    op->addRowsToPointed(visRows);
    layerStyleChanged(layer);
    draw();       
    Q_EMIT selectionChanged(op);
  }
  unsetWaitCursor();
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
    execZoomArea(rec);
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
    execZoomArea(rec);
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
    execZoomArea(rec);
  }
  unsetWaitCursor();
}

void MyDisplay::setTimeSlider(TimeSlider* t)
{
  m_timeSlider = t;
}