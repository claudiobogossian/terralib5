#include "MyGrid.h"
#include <terralib/qt/widgets.h>
#include <terralib/dataaccess.h>
#include <QApplication>
#include <QMessageBox>

MyGrid::MyGrid(std::vector<te::map::MapDisplay*>* m, QWidget* parent) :
  te::qt::widgets::DataGridView(parent),
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
  std::multimap<std::string, QwtPlot*>::iterator it;
  for(it = m_plotMMap.begin(); it != m_plotMMap.end(); ++it)
  {
    QwtPlot* p = (QwtPlot*)(it->second);
    p->disconnect();
    p->close();
    delete p;
  }
  disconnect();
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
  std::string name = dsType->getName();

  std::pair<std::multimap<std::string, QwtPlot*>::iterator, std::multimap<std::string, QwtPlot*>::iterator> ii;
  std::multimap<std::string, QwtPlot*>::iterator it; //Iterator to be used along with ii
  ii = m_plotMMap.equal_range(name); //We get the first and last entry in ii;
  for(it = ii.first; it != ii.second; ++it)
  {
    std::string histogramName = it->second->title().text().toStdString();
    if(histogramName == propName) // histograma ja aberto (apenas update)
    {
      if(it->second->windowTitle() == "Histogram")
      {
        ((te::qt::qwt::HistogramDisplay*)(it->second))->update();
        ((te::qt::qwt::HistogramDisplay*)(it->second))->show();
      }
      break;
    }
  }
    
  if(it == ii.second) // criar novo plot (histograma)
  {
    te::qt::qwt::HistogramDisplay* h = new te::qt::qwt::HistogramDisplay(col, operation);
    if(h->getNumberOfBars() == 0) // histogram nao pode ser criado
    {
      delete h;
      return;
    }
    h->setWindowTitle("Histogram");
    h->show();

    for(it = ii.first; it != ii.second; ++it) // faca conexao deste plot com outros plots (scater, histograma) ja abertos deste layer
    {
      QwtPlot* plot = it->second;
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

    m_plotMMap.insert(std::pair<std::string, QwtPlot*>(name, h));
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
  std::string scatname = colNameX + " vs " + colNameY;

  std::pair<std::multimap<std::string, QwtPlot*>::iterator, std::multimap<std::string, QwtPlot*>::iterator> ii;
  std::multimap<std::string, QwtPlot*>::iterator it; //Iterator to be used along with ii
  ii = m_plotMMap.equal_range(name); //We get the first and last entry in ii;
  for(it = ii.first; it != ii.second; ++it)
  {
    std::string scatterName = it->second->title().text().toStdString();
    if(scatterName == scatname) // scatter ja aberto (apenas update)
    {
      QString title = "Scatter: ";
      title += scatname.c_str();
      if(it->second->windowTitle() == title)
      {
        ((te::qt::qwt::ScatterDisplay*)(it->second))->update();
        ((te::qt::qwt::ScatterDisplay*)(it->second))->show();
      }
      break;
    }
  }
    
  if(it == ii.second) // criar novo plot (scatter)
  {
    te::qt::qwt::ScatterDisplay* esc = new te::qt::qwt::ScatterDisplay(logicalColumnX, logicalColumnY, operation);
    if(esc->getLegend() == 0) // scatter nao pode ser criado
    {
      delete esc;
      return;
    }
    QString title = "Scatter: ";
    title += scatname.c_str();
    esc->setWindowTitle(title);
    esc->show();

    for(it = ii.first; it != ii.second; ++it) // faca conexao deste plot com outros plots (scater, histograma) ja abertos deste layer
    {
      QwtPlot* plot = it->second;
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

    m_plotMMap.insert(std::pair<std::string, QwtPlot*>(name, esc));
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
