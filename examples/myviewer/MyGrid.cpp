#include "MyGrid.h"
#include "MyLayer.h"
#include <terralib/qt/widgets.h>
#include <terralib/dataaccess.h>
#include <QApplication>
#include <QMessageBox>
#include <QCloseEvent>
#include <QScrollBar>

MyGrid::MyGrid(MyLayer* layer, QWidget* parent) :
  te::qt::widgets::DataGridView(parent),
  m_layer(layer),
  m_localSelection(false)
{
  setWindowFlags(Qt::Window);

  // Set the actions for the menu of the vertical header of the grid
  QAction* promotePointedRowsAction = new QAction(QObject::tr("Promote the Pointed Rows"), this);
  promotePointedRowsAction->setStatusTip(QObject::tr("Promote the rows pointed"));
  QObject::connect(promotePointedRowsAction, SIGNAL(triggered()), this, SLOT(promotePointedRows()));

  QAction* promoteQueriedRowsAction = new QAction(QObject::tr("Promote the Queried Rows"), this);
  promoteQueriedRowsAction->setStatusTip(QObject::tr("Promote the rows queried"));
  QObject::connect(promoteQueriedRowsAction, SIGNAL(triggered()), this, SLOT(promoteQueriedRows()));

  te::qt::widgets::HeaderView* vertHeaderView = getVerticalHeaderView();
  QMenu* vertHeaderMenu = new QMenu(vertHeaderView);
  vertHeaderMenu->addAction(promotePointedRowsAction);
  vertHeaderMenu->addAction(promoteQueriedRowsAction);

  vertHeaderView->setVerticalHeaderMenu(vertHeaderMenu);

  // Set the actions for the menu of the horizontal header of the grid
  QAction* sortAscAction = new QAction(QObject::tr("Sort Selected Columns in Ascendent Order"), this);
  sortAscAction->setStatusTip(QObject::tr("Sort the selected columns in ascendent order"));
  QObject::connect(sortAscAction, SIGNAL(triggered()), this, SLOT(sortColumnsInAscendentOrder()));

  QAction* sortDescAction = new QAction(QObject::tr("Sort Selected Columns in Descendent Order"), this);
  sortDescAction->setStatusTip(QObject::tr("Sort the selected columns in descendent order"));
  QObject::connect(sortDescAction, SIGNAL(triggered()), this, SLOT(sortColumnsInDescendentOrder()));

  QAction* plotHistogramAction = new QAction(QObject::tr("Plot Histogram..."), this);
  plotHistogramAction->setStatusTip(QObject::tr("Plot Histogram"));
  QObject::connect(plotHistogramAction, SIGNAL(triggered()), this, SLOT(plotHistogram()));

  QAction* plotScatterAction = new QAction(QObject::tr("Plot Scatter..."), this);
  plotScatterAction->setStatusTip(QObject::tr("Plot Scatter"));
  QObject::connect(plotScatterAction, SIGNAL(triggered()), this, SLOT(plotScatter()));

  QAction* plotTimeSeriesAction = new QAction(QObject::tr("Plot Time Series..."), this);
  plotTimeSeriesAction->setStatusTip(QObject::tr("Plot Time Series"));
  QObject::connect(plotTimeSeriesAction, SIGNAL(triggered()), this, SLOT(plotTimeSeries()));

  QAction* addTooltipAction = new QAction(QObject::tr("Add Tooltip"), this);
  addTooltipAction->setStatusTip(QObject::tr("Add Tooltip"));
  QObject::connect(addTooltipAction, SIGNAL(triggered()), this, SLOT(addTooltip()));

  QAction* clearTooltipAction = new QAction(QObject::tr("Clear Tooltip"), this);
  clearTooltipAction->setStatusTip(QObject::tr("Clear Tooltip"));
  QObject::connect(clearTooltipAction, SIGNAL(triggered()), this, SLOT(clearTooltip()));

  QMenu* tooltipMenu = new QMenu("Tooltip", this);
  tooltipMenu->addAction(addTooltipAction);
  tooltipMenu->addAction(clearTooltipAction);

  te::qt::widgets::HeaderView* horizHeaderView = getHorizontalHeaderView();
  QMenu* horizHeaderMenu = new QMenu(horizHeaderView);
  horizHeaderMenu->addAction(sortAscAction);
  horizHeaderMenu->addAction(sortDescAction);
  horizHeaderMenu->addAction(plotHistogramAction);
  horizHeaderMenu->addAction(plotScatterAction);
  horizHeaderMenu->addAction(plotTimeSeriesAction);
  horizHeaderMenu->addMenu(tooltipMenu);

  horizHeaderView->setHorizontalHeaderMenu(horizHeaderMenu);

  // Set the actions for the menu of the grid viewport
  QAction* removeAllSelectionsAction = new QAction(QObject::tr("Remove All Selections"), this);
  removeAllSelectionsAction->setStatusTip(QObject::tr("Remove All Selections"));
  QObject::connect(removeAllSelectionsAction, SIGNAL(triggered()), this, SLOT(removeAllSelections()));

  QMenu* viewportMenu = new QMenu(this);
  viewportMenu->addAction(removeAllSelectionsAction);
  setViewportMenu(viewportMenu);  
}

MyGrid::~MyGrid()
{
  disconnect();
}

void MyGrid::closeEvent(QCloseEvent *event)
{
  setAttribute(Qt::WA_DeleteOnClose);
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)this->model();
  model->init(0, 0);
  Q_EMIT closed(this);
  event->accept();
}

MyLayer* MyGrid::getLayer()
{
  return m_layer;
}

void MyGrid::plotHistogram()
{
  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  if(op)
    Q_EMIT plotHistogram(this);
}

void MyGrid::plotScatter()
{
  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  if(op)
    Q_EMIT plotScatter(this);
}

void MyGrid::plotTimeSeries()
{
  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  if(op)
    Q_EMIT plotTimeSeries(this);
}

void MyGrid::addTooltip()
{
  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  if(op)
    Q_EMIT addTooltip(this);
}

void MyGrid::clearTooltip()
{
  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  if(op)
    Q_EMIT clearTooltip(this);
}

void MyGrid::selectionChangedSlot(te::map::DataGridOperation*)
{
  updateTableViewSelectionStatus();

  if(m_localSelection)
  {
    m_localSelection = false;
    return;
  }

  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  if(op)
  {
    int i;
    int rows = op->getNumberOfRows();
    for(i = 0; i < rows; ++i)
    {
      if(op->getVisualRowStatus(i) != te::map::DataGridOperation::DESELECTED)
      {
        verticalScrollBar()->setValue(i);
        break;
      }
    }
  }
}

void MyGrid::rowClicked(int clickedVisualRow)
{
  m_localSelection = true;

  DataGridView::rowClicked(clickedVisualRow);
  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  if(op)
    Q_EMIT selectionChanged(op);
}

void MyGrid::removeAllSelections()
{
  m_localSelection = true;

  DataGridView::removeAllSelections();
  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  if(op)
    Q_EMIT selectionChanged(op);
}
