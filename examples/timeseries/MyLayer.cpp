#include "MyLayer.h"
#include "MyLayerRenderer.h"
#include "MyGrid.h"

#include <terralib/dataaccess.h>

#include <terralib/qt/widgets.h>

MyLayer::MyLayer(const std::string& id, const std::string& title, AbstractLayer* parent) : 
  te::map::Layer(id, title, parent),
  m_grid(0),
  m_op(0),
  m_temporal(false),
  m_keepOnMemory(false),
  m_tooltipColumn(-1)
{
}

MyLayer::~MyLayer()
{
  std::set<QwtPlot*>::iterator it;
  for(it = m_plots.begin(); it != m_plots.end(); ++it)
    delete (*it);

  if(m_grid)
    delete m_grid;
  if(m_op)
  {
    delete m_op->getDataSet()->getTransactor();
    delete m_op->getDataSet();
    delete m_op;
  }
}

void MyLayer::setDataGridOperation(te::map::DataGridOperation* op)
{
  m_op = op;
}

te::map::DataGridOperation* MyLayer::getDataGridOperation()
{
  return m_op;
}

void MyLayer::createGrid()
{
  if(m_grid)
    return;

  te::da::DataSource* ds = getDataSource();
  m_grid = new MyGrid(this);
  if(m_op)
  {
    te::qt::widgets::DataGridModel* gridModel = new te::qt::widgets::DataGridModel(m_op->getDataSetType(), m_op->getDataSet(), m_op);
    m_grid->setModel(gridModel);
    m_grid->setVisible(true);

    // If the data set type has geometry, get the position of the geometry column
    te::da::DataSetType* dsType = m_op->getDataSetType();
    if(dsType->hasGeom() == true)
    {
    size_t geometryColumn = geometryColumn = dsType->getDefaultGeomPropertyPos();
      m_grid->hideColumn(geometryColumn);
    }
    m_grid->updateTableViewSelectionStatus();
    m_grid->viewport()->update();
  }
  else
  {
    te::da::DataSourceTransactor* t = ds->getTransactor();
    assert(t);

    te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();
    assert(loader);

    te::da::DataSetType* dsType = loader->getDataSetType(getId(), true);
    dsType->setCatalog(ds->getCatalog());
    assert(dsType);
    delete loader;

    m_grid->setWindowTitle(getId().c_str());


    te::da::DataSet* dataSet = t->getDataSet(getId());

    assert(dataSet);
    if(dsType->getPrimaryKey())
    {
      m_op = new te::map::DataGridOperation();
      m_op->init(dsType, dataSet);
    }
    te::qt::widgets::DataGridModel* gridModel = new te::qt::widgets::DataGridModel(dsType, dataSet, m_op);
    m_grid->setModel(gridModel);
    m_grid->setVisible(true);

    // If the data set type has geometry, get the position of the geometry column
    if(dsType->hasGeom() == true)
    {
      size_t geometryColumn = dsType->getDefaultGeomPropertyPos();  
      m_grid->hideColumn(geometryColumn);
    }
    m_grid->viewport()->update();
  }

  m_grid->show();  
}

void MyLayer::deleteGrid(bool b)
{
  if(b)
    delete m_grid;
  m_grid = 0;
}

MyGrid* MyLayer::getGrid()
{
  return m_grid;
}

std::set<QwtPlot*> MyLayer::getPlots()
{
  return m_plots;
}

void MyLayer::insertPlot(QwtPlot* p)
{
  m_plots.insert(p);
}

void MyLayer::removePlot(QwtPlot* p)
{
  std::set<QwtPlot*>::iterator it = m_plots.find(p);
  if(it != m_plots.end())
    m_plots.erase(it);
}

void MyLayer::setTemporal(bool b)
{
  m_temporal = b;
}

bool MyLayer::isTemporal()
{
  return m_temporal;
}

void MyLayer::setKeepOnMemory(bool b)
{
  m_keepOnMemory = b;
}

bool MyLayer::isKeepOnMemory()
{
  return m_keepOnMemory;
}

void MyLayer::setTooltipColumn(int c)
{
  m_tooltipColumn = c;
}

int MyLayer::getTooltipColumn()
{
  return m_tooltipColumn;
}
