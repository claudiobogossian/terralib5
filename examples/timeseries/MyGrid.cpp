#include "MyGrid.h"
#include "MyWindow.h"
#include <terralib/qt/widgets.h>
#include <terralib/dataaccess.h>
#include <QApplication>
#include <QMessageBox>
#include <QCloseEvent>

MyGrid::MyGrid(std::vector<te::map::MapDisplay*>* m, QWidget* w, QWidget* parent) :
  te::qt::widgets::DataGridView(parent),
  m_mainControlWidget(w),
  m_mapDisplayVec(m)
{
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

  te::qt::widgets::HeaderView* horizHeaderView = getHorizontalHeaderView();
  QMenu* horizHeaderMenu = new QMenu(horizHeaderView);
  horizHeaderMenu->addAction(sortAscAction);
  horizHeaderMenu->addAction(sortDescAction);
  horizHeaderMenu->addAction(plotHistogramAction);
  horizHeaderMenu->addAction(plotScatterAction);
  horizHeaderMenu->addAction(plotTimeSeriesAction);

  horizHeaderView->setHorizontalHeaderMenu(horizHeaderMenu);

  // Set the actions for the menu of the grid viewport
  QAction* removeAllSelectionsAction = new QAction(QObject::tr("Remove All Selections"), this);
  removeAllSelectionsAction->setStatusTip(QObject::tr("Remove All Selections"));
  QObject::connect(removeAllSelectionsAction, SIGNAL(triggered()), this, SLOT(removeAllSelections()));

  QMenu* viewportMenu = new QMenu(this);
  viewportMenu->addAction(removeAllSelectionsAction);
  setViewportMenu(viewportMenu);  

  // faca conexao com os map displays
  std::vector<te::map::MapDisplay*>::iterator v;
  for(v = m_mapDisplayVec->begin(); v != m_mapDisplayVec->end(); ++v)
  {
    MyDisplay* display = (MyDisplay*)*v;
    QObject::connect(display, SIGNAL(selectionChanged(te::map::DataGridOperation*)), this, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    QObject::connect(this, SIGNAL(selectionChanged(te::map::DataGridOperation*)), display, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
  }
}

MyGrid::~MyGrid()
{
  disconnect();
}

void MyGrid::closeEvent(QCloseEvent *event)
{
  setAttribute(Qt::WA_DeleteOnClose);
  Q_EMIT closed(this);
  event->accept();
}

void MyGrid::plotHistogram()
{
  te::qt::widgets::DataGridModel* gridModel = (te::qt::widgets::DataGridModel*)model();
  te::map::DataGridOperation* operation = gridModel->getDataGridOperation();
  te::qt::widgets::HeaderView* header = (te::qt::widgets::HeaderView*)getHorizontalHeaderView();
  int visCol = header->getContextVisualColumnClicked();
  int col = operation->getLogicalColumn(visCol);

  te::da::DataSetType* dsType = operation->getDataSetType();
  te::dt::Property *prop = dsType->getProperty(col);

  std::string propName = prop->getName();
  QString plotName = "Histogram: ";
  plotName += propName.c_str();
  std::string name = dsType->getName();

  te::qt::qwt::HistogramDisplay* histogramWidget = (te::qt::qwt::HistogramDisplay*)((MyWindow*)(m_mainControlWidget))->getPlot(this->windowTitle().toStdString(), plotName.toStdString());
  if(histogramWidget)
  {
    // histograma ja aberto, entao, update e abra a tela
    histogramWidget->update();
    if(histogramWidget->isMinimized())
      histogramWidget->showNormal();
    else
      histogramWidget->show();
  }
  else
  {
    // criar novo plot (histograma)
    te::qt::qwt::HistogramDisplay* h = new te::qt::qwt::HistogramDisplay(col, operation);
    if(h->getNumberOfBars() == 0) // histogram nao pode ser criado
    {
      delete h;
      return;
    }

    h->setWindowTitle(plotName);
    h->show();

    // faca conexao deste plot com outros plots (scater, histograma) ja abertos deste layer
    std::vector<QwtPlot*> plots = ((MyWindow*)(m_mainControlWidget))->getPlots(this->windowTitle().toStdString());
    std::vector<QwtPlot*>::iterator it;
    for(it = plots.begin(); it != plots.end(); ++it)
    {
      QwtPlot* plot = *it;
      QObject::connect(h, SIGNAL(selectionChanged(te::map::DataGridOperation*)), plot, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
      QObject::connect(plot, SIGNAL(selectionChanged(te::map::DataGridOperation*)), h, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    }

    // faca conexao deste plot com o grid
    QObject::connect(h, SIGNAL(selectionChanged(te::map::DataGridOperation*)), this, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    QObject::connect(this, SIGNAL(selectionChanged(te::map::DataGridOperation*)), h, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));

    // faca conexao deste plot com os map displays
    std::vector<te::map::MapDisplay*>::iterator v;
    for(v = m_mapDisplayVec->begin(); v != m_mapDisplayVec->end(); ++v)
    {
      MyDisplay* display = (MyDisplay*)*v;
      QObject::connect(display, SIGNAL(selectionChanged(te::map::DataGridOperation*)), h, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
      QObject::connect(h, SIGNAL(selectionChanged(te::map::DataGridOperation*)), display, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    }

    //faca conexao para remover este HistogramDisplay* quando ele for closed
    QObject::connect(h, SIGNAL(closed(QwtPlot*)), m_mainControlWidget, SLOT(removePlotSlot(QwtPlot*)));

    ((MyWindow*)(m_mainControlWidget))->insertGridOperation(operation, h);
    ((MyWindow*)(m_mainControlWidget))->insertPlot(this->windowTitle().toStdString(), h);
  }
}

void MyGrid::plotScatter()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  te::qt::widgets::DataGridModel* gridModel = (te::qt::widgets::DataGridModel*)model();
  te::map::DataGridOperation* operation = gridModel->getDataGridOperation();
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
  std::string scatname = "Scatter: " + colNameX + " vs " + colNameY;

  te::qt::qwt::ScatterDisplay* scatterWidget = (te::qt::qwt::ScatterDisplay*)((MyWindow*)(m_mainControlWidget))->getPlot(this->windowTitle().toStdString(), scatname);

  if(scatterWidget)
  {
    // scatter ja aberto, entao, update e abra a tela
    scatterWidget->update();
    if(scatterWidget->isMinimized())
      scatterWidget->showNormal();
    else
      scatterWidget->show();
  }
  else
  {
    // criar novo plot (scatter)
    te::qt::qwt::ScatterDisplay* esc = new te::qt::qwt::ScatterDisplay(logicalColumnX, logicalColumnY, operation);
    if(esc->getLegend() == 0) // scatter nao pode ser criado
    {
      delete esc;
      return;
    }
    esc->setWindowTitle(scatname.c_str());
    esc->show();

    // faca conexao deste plot com outros plots (scater, histograma) ja abertos deste layer
    std::vector<QwtPlot*> plots = ((MyWindow*)(m_mainControlWidget))->getPlots(this->windowTitle().toStdString());
    std::vector<QwtPlot*>::iterator it;
    for(it = plots.begin(); it != plots.end(); ++it)
    {
      QwtPlot* plot = *it;
      QObject::connect(esc, SIGNAL(selectionChanged(te::map::DataGridOperation*)), plot, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
      QObject::connect(plot, SIGNAL(selectionChanged(te::map::DataGridOperation*)), esc, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    }

    // faca conexao deste plot com o grid
    QObject::connect(esc, SIGNAL(selectionChanged(te::map::DataGridOperation*)), this, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    QObject::connect(this, SIGNAL(selectionChanged(te::map::DataGridOperation*)), esc, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));

    // faca conexao deste plot com os map displays
    std::vector<te::map::MapDisplay*>::iterator v;
    for(v = m_mapDisplayVec->begin(); v != m_mapDisplayVec->end(); ++v)
    {
      MyDisplay* display = (MyDisplay*)*v;
      QObject::connect(display, SIGNAL(selectionChanged(te::map::DataGridOperation*)), esc, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
      QObject::connect(esc, SIGNAL(selectionChanged(te::map::DataGridOperation*)), display, SLOT(selectionChangedSlot(te::map::DataGridOperation*)));
    }

    //faca conexao para remover este ScatterDisplay* quando ele for closed
    QObject::connect(esc, SIGNAL(closed(QwtPlot*)), m_mainControlWidget, SLOT(removePlotSlot(QwtPlot*)));

    ((MyWindow*)(m_mainControlWidget))->insertGridOperation(operation, esc);
    ((MyWindow*)(m_mainControlWidget))->insertPlot(this->windowTitle().toStdString(), esc);
  }
  QApplication::restoreOverrideCursor();
}

void MyGrid::plotTimeSeries()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  QString s;
  te::qt::widgets::DataGridModel* gridModel = (te::qt::widgets::DataGridModel*)model();
  te::map::DataGridOperation* operation = gridModel->getDataGridOperation();
  te::da::DataSet* dataSet = operation->getDataSet();
  te::da::DataSetType* dsType = operation->getDataSetType();

  std::vector<int> selectedColumns = operation->getSelectedColumns();
  if(selectedColumns.size() < 2)
  {
    QMessageBox::information(this, tr("Time Series Plot"), tr("Select 2 columns in the grid to be plotted!"));
    QApplication::restoreOverrideCursor();
    return;
  }

  int i, xTimeType, xType, logicalColumnX;
  std::string colNameX;
  std::vector<std::pair<std::string, int> >logicalColumns;

  // verifique se existe uma coluna do tipo date time
  for(i = 0; i < (int)selectedColumns.size(); ++i)
  {
    logicalColumnX = operation->getLogicalColumn(selectedColumns[i]);
    colNameX = dsType->getProperty(logicalColumnX)->getName();

    te::dt::Property *propx = dsType->getProperty(logicalColumnX);
    xType = propx->getType();
    if(xType == te::dt::DATETIME_TYPE)
    {
      dataSet->moveBeforeFirst();
      while(dataSet->moveNext())
      {
        if(dataSet->isNull(logicalColumnX))
          continue;
        te::dt::DateTime* t = dataSet->getDateTime(logicalColumnX);
        xTimeType = t->getDateTimeType();
        break;
      }
      break;
    }
    logicalColumns.push_back(std::pair<std::string, int>(colNameX, logicalColumnX));
  }
  if(i == selectedColumns.size())
  {
    QMessageBox::information(this, tr("Time Series Plot Error"), tr("One of the columns must be of type date time!"));
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

  //criar as series temporais
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
      s = dataSet->getAsString(logicalColumnY).c_str();
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
  te::qt::qwt::TimeSeriesDisplay* tsd = new te::qt::qwt::TimeSeriesDisplay(ts);
  tsd->setWindowTitle("Time Series");
  tsd->show();

//  m_plotMMap.insert(std::pair<std::string, QwtPlot*>(name, tsd));
  QApplication::restoreOverrideCursor();
}

void MyGrid::selectionChangedSlot(te::map::DataGridOperation*)
{
  updateTableViewSelectionStatus();
}

void MyGrid::rowClicked(int clickedVisualRow)
{
  DataGridView::rowClicked(clickedVisualRow);
  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  Q_EMIT selectionChanged(op);
}

void MyGrid::removeAllSelections()
{
  DataGridView::removeAllSelections();
  QAbstractItemModel* item = model();
  te::qt::widgets::DataGridModel* model = (te::qt::widgets::DataGridModel*)item;
  te::map::DataGridOperation* op = model->getDataGridOperation();
  Q_EMIT selectionChanged(op);
}
