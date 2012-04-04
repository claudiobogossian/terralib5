#include "MyDisplay.h"
#include "MyLayer.h"
#include "MyLayerRenderer.h"
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>

//QT
#include <QPaintEvent>
#include <QMessageBox>
#include <QString>

MyDisplay::MyDisplay(int w, int h, QWidget* parent, Qt::WindowFlags f) :
  te::qt::widgets::MapDisplay(w, h, parent, f),
  m_useChanged(true)
{
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
    if(op == 0)
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

void MyDisplay::setExtent(const te::gm::Envelope& e)
{
  m_useChanged = false;
  te::qt::widgets::MapDisplay::setExtent(e);
  m_useChanged = true;
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
