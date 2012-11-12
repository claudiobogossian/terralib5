#include "MyLayer.h"
#include "MyLayerRenderer.h"
#include "MyGrid.h"

#include <terralib/dataaccess.h>
#include <terralib/common.h>

#include <terralib/qt/widgets.h>

extern unsigned long long getAvailableMemory();

MyLayer::MyLayer(const std::string& id, const std::string& title, AbstractLayer* parent) : 
  te::map::Layer(id, title, parent),
  m_grid(0),
  m_op(0),
  m_temporal(false),
  m_keepOnMemory(false)
{
  setDataSetName(id);
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
//te::common::Logger::initialize("MyLayer");
//QString msg, msgg, name = getId().c_str();
//unsigned long maa, mbb;

    if(this->getDataSource()->getType() != "OGR")
      delete m_op->getDataSet()->getTransactor();
//maa = getAvailableMemory();
//msg.setNum((qulonglong)maa/(1<<10));
//msg.insert(0, "before delete dataset " + name + ":");
//msg += " KBytes";
//te::common::Logger::logInfo("MyLayer", msg.toStdString().c_str());
    delete m_op->getDataSet();
//mbb = getAvailableMemory();
//msg.setNum((qulonglong)mbb/(1<<10));
//msg.insert(0, "after delete dataset " + name + ":");
//msgg.setNum((qulonglong)((mbb - maa) / (1<<10)));
//msg += "  KBytes, liberou:" + msgg + " KBytes";
//te::common::Logger::logInfo("MyLayer", msg.toStdString().c_str());
//te::common::Logger::logInfo("MyLayer", "\n");

//maa = getAvailableMemory();
//msg.setNum((qulonglong)maa/(1<<10));
//msg.insert(0, "before delete datagridoperation " + name + ":");
//msg += " KBytes";
//te::common::Logger::logInfo("MyLayer", msg.toStdString().c_str());
    delete m_op;
//mbb = getAvailableMemory();
//msg.setNum((qulonglong)mbb/(1<<10));
//msg.insert(0, "after delete datagridoperation");
//msgg.setNum((qulonglong)((mbb - maa) / (1<<10)));
//msg += "  KBytes, liberou:" + msgg + " KBytes";
//te::common::Logger::logInfo("MyLayer", msg.toStdString().c_str());
//te::common::Logger::logInfo("MyLayer", "\n");
//te::common::Logger::finalize("MyLayer");
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

void MyLayer::createGrid(QWidget* w)
{
  if(m_grid)
    return;

  te::da::DataSource* ds = getDataSource();
  m_grid = new MyGrid(this, w);
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

    //te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();
    //assert(loader);

    //te::da::DataSetType* dsType = loader->getDataSetType(getId(), true);
    //dsType->setCatalog(ds->getCatalog());
    //assert(dsType);
    //delete loader;
    te::da::DataSetTypePtr dsType = ds->getCatalog()->getDataSetType(getId());

    m_grid->setWindowTitle(getId().c_str());

//te::common::Logger::initialize("MyLayer CreateGrid");
//QString msg, msgg, sname = getId().c_str();
//unsigned long maa, mbb;
//maa = getAvailableMemory();
//msg.setNum((qulonglong)maa/(1<<10));
//msg.insert(0, "before getDataSet " + sname + ":");
//msg += " KBytes";
//te::common::Logger::logInfo("MyLayer CreateGrid", msg.toStdString().c_str());
    te::da::DataSet* dataSet = t->getDataSet(getId());
//mbb = getAvailableMemory();
//msg.setNum((qulonglong)mbb/(1<<10));
//msg.insert(0, "after getDataSet " + sname + ":");
//msgg.setNum((qulonglong)((maa - mbb) / (1<<10)));
//msg += "  KBytes, consumiu:" + msgg + " KBytes";
//te::common::Logger::logInfo("MyLayer CreateGrid", msg.toStdString().c_str());
//te::common::Logger::logInfo("MyLayer CreateGrid", "\n");

    assert(dataSet);
    if(dsType->getPrimaryKey())
    {
//maa = getAvailableMemory();
//msg.setNum((qulonglong)maa/(1<<10));
//msg.insert(0, "before new DataGridOperation " + sname + ":");
//msg += " KBytes";
//te::common::Logger::logInfo("MyLayer CreateGrid", msg.toStdString().c_str());
      m_op = new te::map::DataGridOperation();
      m_op->init(dsType.get(), dataSet);
      //m_op->init(dsType, dataSet);
//mbb = getAvailableMemory();
//msg.setNum((qulonglong)mbb/(1<<10));
//msg.insert(0, "after new DataGridOperation init " + sname + ":");
//msgg.setNum((qulonglong)((maa - mbb) / (1<<10)));
//msg += "  KBytes, consumiu:" + msgg + " KBytes";
//te::common::Logger::logInfo("MyLayer CreateGrid", msg.toStdString().c_str());
//te::common::Logger::logInfo("MyLayer CreateGrid", "\n");
    }
//te::common::Logger::finalize("MyLayer CreateGrid");
    //te::qt::widgets::DataGridModel* gridModel = new te::qt::widgets::DataGridModel(dsType, dataSet, m_op);
    te::qt::widgets::DataGridModel* gridModel = new te::qt::widgets::DataGridModel(dsType.get(), dataSet, m_op);
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

std::set<QwtPlot*>& MyLayer::getPlots()
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

void MyLayer::addToTooltipColumns(int c)
{
  std::vector<int>::iterator it;
  for(it = m_tooltipColumns.begin(); it != m_tooltipColumns.end(); ++it)
  {
    if(c == *it)
      break;
  }
  if(it == m_tooltipColumns.end())
    m_tooltipColumns.push_back(c);
}

std::vector<int> MyLayer::getTooltipColumns()
{
  return m_tooltipColumns;
}

void MyLayer::clearTooltipColumns()
{
  m_tooltipColumns.clear();
}
