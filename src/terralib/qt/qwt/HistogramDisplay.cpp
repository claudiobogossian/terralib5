#include "HistogramDisplay.h"
#include "StringScaleDraw.h"

//C Library
#include <stdlib.h>

//QT includes
#include <QPen>
#include <QPainter>
#include <QWheelEvent>
#include <QBoxLayout>
#include <QGroupBox>
#include <QColorDialog>

//QWT includes
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_legend_item.h>
#include <qwt_plot_grid.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>
#include <qwt_scale_div.h>
#include <qwt_picker_machine.h>

//Terralib includes
#include "../../color.h"

//STL
#include <vector>
#include <map>

te::qt::qwt::HistogramDisplay::HistogramDisplay(int col, te::map::DataGridOperation* op, QWidget *parent):
  Plot("HISTOGRAM", op, parent),
  m_legendMenu(0),
  m_allHistogram(0),
  m_deselectedHistogram(0),
  m_pointedHistogram(0),
  m_queriedHistogram(0),
  m_pointedAndQueriedHistogram(0),
  m_selectionCursor(0)
{
  if(m_op == 0)
    return;

  te::da::DataSet* dataSet = m_op->getDataSet(); 
  if(dataSet == 0)
    return;

  m_yCol = -1;
  m_xStringScaleDraw = new te::qt::qwt::StringScaleDraw();

  m_xCol = col;
  te::da::DataSetType* dsType = m_op->getDataSetType();
  te::dt::Property *prop = dsType->getProperty(m_xCol);
  m_xType = prop->getType();

  if(m_xType == te::dt::DATETIME_TYPE)
  {
    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      if(dataSet->isNull(m_xCol))
        continue;
      te::dt::DateTime* t = dataSet->getDateTime(m_xCol);
      m_xTimeType = t->getDateTimeType();
      break;
    }
  }

  te::da::PrimaryKey *pk = dsType->getPrimaryKey();
  const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
  std::string pkName = pkProps[0]->getName();
  m_PKPos = dsType->getPropertyPosition(pkName);

  std::string propName = prop->getName();
  setHorizontalTitle(propName.c_str());

  plotLayout()->setAlignCanvasToScales(true);

  QwtPlotGrid *grid = new QwtPlotGrid;

  // nao mostra a grade horizontal
  grid->enableX(false);
  // mostra a grade vertical
  grid->enableY(true);

  // nao mostra as subgrades horizontais
  grid->enableXMin(false);
  // nao mostra as subgrades verticais
  grid->enableYMin(false);

  //a grade (mais espacada) e' mostrada em preto e pontilhado
  grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));

  grid->attach(this);

  if(createHistograms() == false)
    return;
  attachHistograms();

  if(m_legend == 0) // histogram nao foi criado
  {
    m_numberOfBars = 0;
    return;
  }

  //inicializa zoom e pan
  Plot::init();

  //controle para visualizar (on-off) cada histograma
  QObject::connect(this, SIGNAL(legendChecked(QwtPlotItem* , bool)), this, SLOT(showHistogramSlot(QwtPlotItem* , bool)));

  QObject::connect(m_legend, SIGNAL(legendContextMenu(QPoint&, QWidget*)), this, SLOT(legendMenuSlot(QPoint&, QWidget*)));

  //inicializa cursor de selecao e controle do numero de barras (valido para number histograms)
  m_selectionCursor = new HistogramSeletionCursor(canvas());
  m_selectionCursor->setRubberBandPen( QColor( Qt::magenta ) );
  m_selectionCursor->setTrackerPen( QColor( Qt::black ) );
  m_selectionCursor->setMousePattern( QwtEventPattern::MouseSelect1, Qt::LeftButton);
  m_selectionCursor->setRubberBand(QwtPicker::PolygonRubberBand);
  m_selectionCursor->setTrackerMode(QwtPicker::AlwaysOff);
  QwtPickerPolygonMachine* mac = new QwtPickerPolygonMachine();
  m_selectionCursor->setStateMachine(mac);
  QObject::connect(m_selectionCursor, SIGNAL(select(int, int)), this, SLOT(selectSlot(int, int)));
  QObject::connect(m_selectionCursor, SIGNAL(wheelEvent(int)), this, SLOT(wheelEventSlot(int)));

  QBoxLayout* layout = new QBoxLayout(QBoxLayout::BottomToTop, this);

  QGroupBox* gBox = new QGroupBox("Action:", this); // este nome tem que ser pequeno para nao invadir o canvas
  QBoxLayout* blayout = new QBoxLayout(QBoxLayout::TopToBottom, gBox);
  QButtonGroup* m_buttonGroup = new QButtonGroup(gBox);
  QRadioButton* b = new QRadioButton("Select", gBox);
  QRadioButton* selectButton = b;
  m_buttonGroup->addButton(b);
  blayout->addWidget(b);
  b = new QRadioButton("Toggle", gBox);
  m_buttonGroup->addButton(b);
  blayout->addWidget(b);
  b = new QRadioButton("Add", gBox);
  m_buttonGroup->addButton(b);
  blayout->addWidget(b);
  b = new QRadioButton("Unsel", gBox);
  m_buttonGroup->addButton(b);
  blayout->addWidget(b);
  b = new QRadioButton("Zoom", gBox);
  m_buttonGroup->addButton(b);
  blayout->addWidget(b);
  b = new QRadioButton("Pan", gBox);
  m_buttonGroup->addButton(b);
  blayout->addWidget(b);

  layout->addWidget(gBox);
  layout->setAlignment(gBox, Qt::AlignRight);
  int legHeight = m_legend->contentsWidget()->height();
  int boxHeight = gBox->height();
  int space = legHeight + boxHeight + 20;
  layout->addSpacing(space);

  selectButton->setChecked(true);
  buttonClickedSlot(selectButton);
  QObject::connect(m_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(buttonClickedSlot(QAbstractButton*)));
}

te::qt::qwt::HistogramDisplay::~HistogramDisplay()
{
  if(m_numberOfBars)
    disconnect();

  delete m_allHistogram;
  delete m_deselectedHistogram;
  delete m_pointedHistogram;
  delete m_queriedHistogram;
  delete m_pointedAndQueriedHistogram;
  delete m_selectionCursor;

  if(axisScaleDraw(QwtPlot::xBottom) != m_xStringScaleDraw)
    delete m_xStringScaleDraw;
}

void te::qt::qwt::HistogramDisplay::closeEvent(QCloseEvent *event)
{
  setAttribute(Qt::WA_DeleteOnClose);
  Q_EMIT closed(this);
  event->accept();
}

bool te::qt::qwt::HistogramDisplay::createHistograms()
{
  std::map<double, int> deselectedValues, pointedValues, queriedValues, pointedAndQueriedValues;
  std::map<double, int>::iterator it;
  std::string pkv;
  int selection;
  QString v;

  m_allValues.clear();

  te::da::DataSet* dataSet = m_op->getDataSet(); 
  dataSet->moveBeforeFirst();
  if(m_xType == te::dt::STRING_TYPE)
  {
    std::map<QString, int> sallValues, sdeselectedValues, spointedValues, squeriedValues, spointedAndQueriedValues;

    while(dataSet->moveNext())
    {
      if(dataSet->isNull(m_xCol) == false)
        v = dataSet->getString(m_xCol).c_str();
      else
        v = " null value";

      m_xStringSet.insert(v);
      if(sallValues.find(v) == sallValues.end())
      {
        sallValues[v] = 1;
        sdeselectedValues[v] = 0;
        spointedValues[v] = 0;
        squeriedValues[v] = 0;
        spointedAndQueriedValues[v] = 0;
      }
      else
        sallValues[v] = sallValues[v] + 1;

      pkv = dataSet->getAsString(m_PKPos);
      selection = m_op->getDataSetSelectionStatus(pkv);
      if(!(selection == te::map::DataGridOperation::POINTED || selection == te::map::DataGridOperation::QUERIED))
        sdeselectedValues[v] = sdeselectedValues[v] + 1;
      else if(selection == te::map::DataGridOperation::POINTED)
        spointedValues[v] = spointedValues[v] + 1;
      else if(selection == te::map::DataGridOperation::QUERIED)
        squeriedValues[v] = squeriedValues[v] + 1;
      else if(selection == te::map::DataGridOperation::POINTED_AND_QUERIED)
        spointedAndQueriedValues[v] = spointedAndQueriedValues[v] + 1;  
    }

    std::map<QString, int>::iterator qit;
    int i = 0;
    for(qit = sallValues.begin(); qit != sallValues.end(); ++i, ++qit)
      m_allValues[i] = qit->second;
    i = 0;
    for(qit = sdeselectedValues.begin(); qit != sdeselectedValues.end(); ++i, ++qit)
      deselectedValues[i] = qit->second;
    i = 0;
    for(qit = spointedValues.begin(); qit != spointedValues.end(); ++i, ++qit)
      pointedValues[i] = qit->second;
    i = 0;
    for(qit = squeriedValues.begin(); qit != squeriedValues.end(); ++i, ++qit)
      queriedValues[i] = qit->second;
    i = 0;
    for(qit = spointedAndQueriedValues.begin(); qit != spointedAndQueriedValues.end(); ++i, ++qit)
      pointedAndQueriedValues[i] = qit->second;

    m_xSize = m_xStringSet.size();
    if(m_xSize <= 1)
      return false;

    m_barInterval = 1.;

    if(m_tableChanged)
    {
      m_tableChanged = false;
      m_xMin = m_XMIN = 0.;
      m_xMax = m_XMAX = m_xSize;

      double labelInterval = m_barInterval;
      while(m_xMax/labelInterval > m_numberOfBars)
        labelInterval += 1.;

      // Colocar string values no eixo x 
      m_xStringScaleDraw->setStringSet(m_xStringSet);
      setAxisScaleDraw(QwtPlot::xBottom, m_xStringScaleDraw);

      axisScaleDraw(QwtPlot::xBottom)->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
      setAxisScale(QwtPlot::xBottom, m_xMin-1., m_xMax, labelInterval);
      setAxisMaxMajor(QwtPlot::xBottom, m_numberOfBars);
      axisScaleDraw(QwtPlot::xBottom)->setLabelRotation(-60);
      setAxisScale(QwtPlot::xBottom, m_xMin, m_xMax, (double)labelInterval);
    }
  }
  else if(m_xType == te::dt::DATETIME_TYPE ||
    (m_xType >= te::dt::INT16_TYPE && m_xType <= te::dt::UINT64_TYPE) || 
    m_xType == te::dt::FLOAT_TYPE || m_xType == te::dt::DOUBLE_TYPE || m_xType == te::dt::NUMERIC_TYPE)
  {
    QString s;
    double v;
    while(dataSet->moveNext())
    {
      if(dataSet->isNull(m_xCol))
        continue;

      if(m_xTimeType == te::dt::TIME_INSTANT)
      {
        te::dt::TimeInstant* ti = (te::dt::TimeInstant*)dataSet->getDateTime(m_xCol);
        te::dt::TimeInstant t = *ti;
        delete ti;

        boost::gregorian::date basedate(1400, 01, 01);
        boost::gregorian::date_duration days = t.getDate().getDate() - basedate;

        long long int seconds = t.getTime().getTimeDuration().total_seconds();
        long long int dias = days.days();
        v = dias * 86400 + seconds;
      }
      else if(m_xTimeType == te::dt::DATE)
      {
        te::dt::Date* d = (te::dt::Date*)dataSet->getDateTime(m_xCol);
        te::dt::Date t = *d;
        delete d;
        boost::gregorian::date basedate(1400, 01, 01);
        boost::gregorian::date_duration days = t.getDate() - basedate;
        v = days.days();
      }
      else
      {
        s = dataSet->getAsString(m_xCol).c_str();
        v = s.toDouble();
      }

      if(m_allValues.find(v) == m_allValues.end())
      {
        m_allValues[v] = 1;
        deselectedValues[v] = 0;
        pointedValues[v] = 0;
        queriedValues[v] = 0;
        pointedAndQueriedValues[v] = 0;
      }
      else
      {
        m_allValues[v] = m_allValues[v] + 1;
      }

      pkv = dataSet->getAsString(m_PKPos);
      selection = m_op->getDataSetSelectionStatus(pkv);
      if(!(selection == te::map::DataGridOperation::POINTED || selection == te::map::DataGridOperation::QUERIED))
        deselectedValues[v] = deselectedValues[v] + 1;
      else if(selection == te::map::DataGridOperation::POINTED)
        pointedValues[v] = pointedValues[v] + 1;
      else if(selection == te::map::DataGridOperation::QUERIED)
        queriedValues[v] = queriedValues[v] + 1;
      else if(selection == te::map::DataGridOperation::POINTED_AND_QUERIED)
        pointedAndQueriedValues[v] = pointedAndQueriedValues[v] + 1;  
    }

    m_xSize = m_allValues.size();
    if(m_xSize <= 1)
      return false;

    if(m_tableChanged)
    {
      m_tableChanged = false;
      m_xMin = m_XMIN = m_allValues.begin()->first;
      m_xMax = m_XMAX = m_allValues.rbegin()->first;

      if(m_xType == te::dt::DATETIME_TYPE)
      {
        // Colocar string values no eixo x
        m_xStringScaleDraw->setTimeType(m_xTimeType);
        setAxisScaleDraw(QwtPlot::xBottom, m_xStringScaleDraw);
      }

      axisScaleDraw(QwtPlot::xBottom)->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
      setAxisMaxMajor(QwtPlot::xBottom, m_numberOfBars);
      axisScaleDraw(QwtPlot::xBottom)->setLabelRotation(-60);
      adjustHistogramHorizontalAxis();
    }
  }

  QColor qcor(m_color.getRed(), m_color.getGreen(), m_color.getBlue());
  if(m_allHistogram == 0)
    m_allHistogram = new Histogram(qcor, "All");
  m_allHistogram->setValues(m_allValues, m_xType, m_xTimeType, m_XMIN, m_barInterval);
  m_allHistogram->setColor(qcor);

  te::color::RGBAColor cor = m_op->getDefaultColor();
  qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue());
  if(m_deselectedHistogram == 0)
    m_deselectedHistogram = new Histogram(qcor, "Deselected");
  m_deselectedHistogram->setValues(deselectedValues, m_xType, m_xTimeType, m_XMIN, m_barInterval);
  m_deselectedHistogram->setColor(qcor);

  cor = m_op->getPointedColor();
  qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue());
  if(m_pointedHistogram == 0)
    m_pointedHistogram = new Histogram(qcor, "Pointed");
  m_pointedHistogram->setValues(pointedValues, m_xType, m_xTimeType, m_XMIN, m_barInterval);
  m_pointedHistogram->setColor(qcor);

  cor = m_op->getQueriedColor();
  qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue());
  if(m_queriedHistogram == 0)
    m_queriedHistogram = new Histogram(qcor, "Queried");
  m_queriedHistogram->setValues(queriedValues, m_xType, m_xTimeType, m_XMIN, m_barInterval);
  m_queriedHistogram->setColor(qcor);

  cor = m_op->getPointedAndQueriedColor();
  qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue());
  if(m_pointedAndQueriedHistogram == 0)
    m_pointedAndQueriedHistogram = new Histogram(qcor, "Point/Query");
  m_pointedAndQueriedHistogram->setValues(pointedAndQueriedValues, m_xType, m_xTimeType, m_XMIN, m_barInterval);
  m_pointedAndQueriedHistogram->setColor(qcor);
  return true;
}

void te::qt::qwt::HistogramDisplay::attachHistograms()
{
  //A ordem de desenho é de acordo com os attaches
  m_pointedAndQueriedHistogram->attach(this);
  m_queriedHistogram->attach(this);
  m_pointedHistogram->attach(this);
  m_deselectedHistogram->attach(this);
  m_allHistogram->attach(this);

  m_histogramVec.push_back(m_pointedAndQueriedHistogram);
  m_histogramVec.push_back(m_queriedHistogram);
  m_histogramVec.push_back(m_pointedHistogram);
  m_histogramVec.push_back(m_deselectedHistogram);
  m_histogramVec.push_back(m_allHistogram);

  m_histogramMap[m_allHistogram] = false;
  m_histogramMap[m_deselectedHistogram] = false;
  m_histogramMap[m_pointedHistogram] = false;
  m_histogramMap[m_queriedHistogram] = false;
  m_histogramMap[m_pointedAndQueriedHistogram] = false;

  //create histogram legends
  m_legend = new Legend;
  m_legend->setItemMode(QwtLegend::CheckableItem);
  insertLegend(m_legend);

  //set visibility
  showHistogramSlot(m_allHistogram, true);
  showHistogramSlot(m_deselectedHistogram, true);
  showHistogramSlot(m_pointedHistogram, true);
  showHistogramSlot(m_queriedHistogram, true);
  showHistogramSlot(m_pointedAndQueriedHistogram, true);
}

void te::qt::qwt::HistogramDisplay::updateHistograms()
{
  createHistograms();
  replot();
  show();
}

void te::qt::qwt::HistogramDisplay::showHistogramSlot(QwtPlotItem* item, bool on)
{
  Histogram* hitem = (Histogram*)item;
  m_histogramMap[hitem] = on;
  updateVisibity();
}

void te::qt::qwt::HistogramDisplay::legendMenuSlot(QPoint& p, QWidget* w)
{
  m_legendWidget = w;

  if(m_legendMenu == 0)
  {
    m_legendMenu = new QMenu(this);

    QAction* action = new QAction("&ToFront", m_legendMenu);
    m_legendMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(legendToFrontSlot()));

    action = new QAction("&ToBack", m_legendMenu);
    m_legendMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(legendToBackSlot()));

    //action = new QAction("&Color...", m_legendMenu);
    //m_legendMenu->addAction(action);
    //connect(action, SIGNAL(triggered()), this, SLOT(legendColorSlot()));
  }
  m_legendMenu->exec(p);
}

void te::qt::qwt::HistogramDisplay::legendToFrontSlot()
{
  Histogram* hitem = (Histogram*)m_legend->find(m_legendWidget);
  std::vector<Histogram*> hVec;
  std::vector<Histogram*>::iterator it = m_histogramVec.begin();
  if(*it != hitem)
  {
    std::vector<Histogram*>::reverse_iterator rit = m_histogramVec.rbegin();
    while(rit != m_histogramVec.rend())
    {
      Histogram* h = *rit;
      if(h == hitem)
      {
        rit++;
        if(rit != m_histogramVec.rend())
        {
          hVec.push_back(*rit);
          (*rit)->detach();
        }
      }
      hVec.push_back(h);
      h->detach();
      ++rit;
    }
  
    m_histogramVec.clear();
    rit = hVec.rbegin();
    while(rit != hVec.rend())
    {
      (*rit)->attach(this);
      m_histogramVec.push_back(*rit);
      rit++;
    }
  }
  updateVisibity();
}

void te::qt::qwt::HistogramDisplay::legendToBackSlot()
{
  Histogram* hitem = (Histogram*)m_legend->find(m_legendWidget);
  std::vector<Histogram*> hVec;
  std::vector<Histogram*>::reverse_iterator rit = m_histogramVec.rbegin();
  if(*rit != hitem)
  {
    std::vector<Histogram*>::iterator it = m_histogramVec.begin();
    while(it != m_histogramVec.end())
    {
      Histogram* h = *it;
      if(h == hitem)
      {
        it++;
        if(it != m_histogramVec.end())
        {
          hVec.push_back(*it);
          (*it)->detach();
        }
      }
      hVec.push_back(h);
      h->detach();
      ++it;
    }
  
    m_histogramVec.clear();
    it = hVec.begin();
    while(it != hVec.end())
    {
      (*it)->attach(this);
      m_histogramVec.push_back(*it);
      it++;
    }
  }
  updateVisibity();
}

void te::qt::qwt::HistogramDisplay::legendColorSlot()
{
  QColor oldColor, color;
  te::color::RGBAColor cor;

  Histogram* hitem = (Histogram*)m_legend->find(m_legendWidget);
  QString title = hitem->title().text();
  if(title == "All")
    cor = m_color;
  else if(title == "Deselected")
    cor = m_op->getDefaultColor();
  else if(title == "Pointed")
    cor = m_op->getPointedColor();
  else if(title == "Queried")
    cor = m_op->getQueriedColor();
  else if(title == "Point/Query")
    cor = m_op->getPointedAndQueriedColor();

  oldColor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue());

  color = QColorDialog::getColor(oldColor, this);
  if (color.isValid()) 
  {
    hitem->setColor(color);

    cor.setColor(color.red(), color.green(), color.blue(), 200);
    if(title == "All")
      m_color = cor;
    else if(title == "Deselected")
      m_op->setDefaultColor(cor);
    else if(title == "Pointed")
      m_op->setPointedColor(cor);
    else if(title == "Queried")
      m_op->setQueriedColor(cor);
    else if(title == "Point/Query")
      m_op->setPointedAndQueriedColor(cor);

    replot();
    Q_EMIT selectionChanged(m_op);
  }
}

void te::qt::qwt::HistogramDisplay::updateVisibity()
{

  std::map<Histogram*, bool>::iterator mit = m_histogramMap.begin();
  while(mit != m_histogramMap.end())
  {
    Histogram* h = mit->first;
    bool b = mit->second;
    h->setVisible(b);
    QWidget *w = legend()->find(h);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(b);
    ++mit;
  }

  replot();
}

//invertendo a ordem de desenho para ficar igual ao do terraview (de baixo para cima)
void te::qt::qwt::HistogramDisplay::drawItems( QPainter *painter, const QRectF &canvasRect,
        const QwtScaleMap map[axisCnt] ) const
{
  const QwtPlotItemList& itmList = itemList();
  int size = itmList.size();
  for(int i = size-1; i >= 0; --i)
  {
    QwtPlotItem *item = itmList[i];
    if ( item && item->isVisible() )
    {
        painter->save();

        painter->setRenderHint( QPainter::Antialiasing,
            item->testRenderHint( QwtPlotItem::RenderAntialiased ) );

        item->draw( painter,
            map[item->xAxis()], map[item->yAxis()],
            canvasRect );

        painter->restore();
    }
  }
}

void te::qt::qwt::HistogramDisplay::selectSlot(int x0, int x1)
{
  std::vector<int> visRows;
  double d0 = invTransform(QwtPlot::xBottom, x0);
  double d1 = invTransform(QwtPlot::xBottom, x1);
  double aux = d1;
  if(d0 > d1)
  {
    d1 = d0;
    d0 = aux;
  }

  if((m_xType >= te::dt::INT16_TYPE && m_xType <= te::dt::UINT64_TYPE) || 
    m_xType == te::dt::FLOAT_TYPE || m_xType == te::dt::DOUBLE_TYPE || m_xType == te::dt::NUMERIC_TYPE)
    select(d0, d1, visRows);
  else if(m_xType == te::dt::STRING_TYPE)
    selectString(d0, d1, visRows);
  else if(m_xType == te::dt::DATETIME_TYPE)
    selectDateTime(d0, d1, visRows);

  if(m_selectionMode == SELECT)
  {
    if(visRows.empty())
      m_op->removePointedStatusOfAllRows();
    else
      m_op->setRowsAsPointed(visRows);
  }
  else if(m_selectionMode == TOGGLE)
    m_op->toggleRowsPointingStatus(visRows);
  else if(m_selectionMode == ADD)
    m_op->addRowsToPointed(visRows);
  else if(m_selectionMode == UNSEL)
    m_op->removePointedStatusOfRows(visRows);

  updateHistograms();

  Q_EMIT selectionChanged(m_op);
}

void te::qt::qwt::HistogramDisplay::select(double d0, double d1, std::vector<int>& visRows)
{
  std::map<double, int> rangeMap;
  std::map<double, int>::iterator it;

  double ini, fim;

  double v = 0.;
  if(d0 > 0.)
  {
    while(v < d0)
      v += m_barInterval;
  }
  else
  {
    while((v-m_barInterval) > d0)
      v -= m_barInterval;
  }
  ini = v;

  while((v+m_barInterval) < d1)
    v += m_barInterval;
  fim = v;

  if(fabs(fim - m_xMax) < 0.00000001)
    fim = m_xMax * 1.00000001;

  if(ini != fim)
  {
    if(m_xType >= te::dt::INT16_TYPE && m_xType <= te::dt::UINT64_TYPE)
    {
      ini = (int)ini;
      fim = (int)fim;
      if(ini == fim)
        fim += 1.;
      if(fim == (int)m_xMax)
        ++fim;
    }

    it = m_allValues.begin();
    while(it != m_allValues.end())
    {
      v = it->first;
      if(v >= ini)
        break;
      ++it;
    }
    if(v < fim)
      rangeMap[v] = it->second;
  
    while(++it != m_allValues.end())
    {
      v = it->first;
      if(v >= fim)
        break;

      rangeMap[v] = it->second;
    }

    //executar query para selecionar quais objetos tem a coluna = selectSet
    //e apontar esses obetos.

    //teste: vou fazer usando o dataSet (sem fazer query)
    std::string pkv;
    QString sv;
    te::da::DataSet* dataSet = m_op->getDataSet(); 
    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      if(dataSet->isNull(m_xCol) == false)
      {
        sv = dataSet->getAsString(m_xCol).c_str();
        v = sv.toDouble();
        it = rangeMap.find(v);
        if(it == rangeMap.end())
          continue;

        pkv = dataSet->getAsString(m_PKPos);
        int logRow = m_op->getLogicalRow(pkv);
        visRows.push_back(m_op->getVisualRow(logRow));
        
        it->second = it->second - 1;
        if(it->second == 0)
          rangeMap.erase(it);
        if(rangeMap.empty())
          break;
      }
    }
  }
}

void te::qt::qwt::HistogramDisplay::selectString(double d0, double d1, std::vector<int>& visRows)
{
  int ini = (d0 + 1.5);
  int fim = (d1 - .5);
  std::map<QString, int> rangeMap;
  std::map<double, int>::iterator it;
  std::map<QString, int>::iterator qit;
  std::set<QString>::iterator sit;

  int i = 0;
  it = m_allValues.begin();
  sit = m_xStringSet.begin();
  while(it != m_allValues.end() && i < ini)
  {
    ++i;
    ++it;
    sit++;
  }
  
  while(it != m_allValues.end() && i <= fim)
  {
    rangeMap[*sit] = it->second;
    ++i;
    ++it;
    ++sit;
  }

  ////executar query para selecionar quais objetos tem a coluna = selectSet
  ////e apontar esses objetos.

  ////teste: vou fazer usando o dataSet (sem fazer query)
  std::string pkv;
  QString v;
  te::da::DataSet* dataSet = m_op->getDataSet(); 
  dataSet->moveBeforeFirst();
  while(dataSet->moveNext())
  {
    if(dataSet->isNull(m_xCol) == false)
      v = dataSet->getString(m_xCol).c_str();
    else
      v = " null value";

    qit = rangeMap.find(v);
    if(qit == rangeMap.end())
      continue;

    pkv = dataSet->getAsString(m_PKPos);
    int logRow = m_op->getLogicalRow(pkv);
    visRows.push_back(m_op->getVisualRow(logRow));
        
    qit->second = qit->second - 1;
    if(qit->second == 0)
      rangeMap.erase(qit);
    if(rangeMap.empty())
      break;
  }
}

void te::qt::qwt::HistogramDisplay::selectDateTime(double d0, double d1, std::vector<int>& visRows)
{
  std::map<double, int> rangeMap;
  std::map<double, int>::iterator it;

  double ini, fim;

  double v = 0.;
  if(d0 > 0.)
  {
    while(v < d0)
      v += m_barInterval;
  }
  else
  {
    while((v-m_barInterval) > d0)
      v -= m_barInterval;
  }
  ini = v;

  while((v+m_barInterval) < d1)
    v += m_barInterval;
  fim = v;

  if(ini != fim)
  {
    it = m_allValues.begin();
    while(it != m_allValues.end())
    {
      v = it->first;
      if(v >= ini)
        break;
      ++it;
    }
    if(v < fim)
      rangeMap[v] = it->second;
  
    while(++it != m_allValues.end())
    {
      v = it->first;
      if(v >= fim)
        break;

      rangeMap[v] = it->second;
    }

    std::string pkv;
    te::da::DataSet* dataSet = m_op->getDataSet(); 
    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      if(dataSet->isNull(m_xCol) == false)
      {
        if(m_xTimeType == te::dt::TIME_INSTANT)
        {
          te::dt::TimeInstant* ti = (te::dt::TimeInstant*)dataSet->getDateTime(m_xCol);
          te::dt::TimeInstant t = *ti;
          delete ti;

          boost::gregorian::date basedate(1400, 01, 01);
          boost::gregorian::date_duration days = t.getDate().getDate() - basedate;

          long long int seconds = t.getTime().getTimeDuration().total_seconds();
          long long int dias = days.days();
          v = dias * 86400 + seconds;
        }
        else if(m_xTimeType == te::dt::DATE)
        {
          te::dt::Date* d = (te::dt::Date*)dataSet->getDateTime(m_xCol);
          te::dt::Date t = *d;
          delete d;
          boost::gregorian::date basedate(1400, 01, 01);
          boost::gregorian::date_duration days = t.getDate() - basedate;
          v = days.days();
        }

        it = rangeMap.find(v);
        if(it == rangeMap.end())
          continue;

        pkv = dataSet->getAsString(m_PKPos);
        int logRow = m_op->getLogicalRow(pkv);
        visRows.push_back(m_op->getVisualRow(logRow));
        
        it->second = it->second - 1;
        if(it->second == 0)
          rangeMap.erase(it);
        if(rangeMap.empty())
          break;
      }
    }
  }
}

void te::qt::qwt::HistogramDisplay::buttonClickedSlot(QAbstractButton* button)
{
  m_radioButton = (QRadioButton*)button;
  QString s = m_radioButton->text().toUpper();

  if(s == "SELECT")
    m_selectionMode = SELECT;
  else if(s == "TOGGLE")
    m_selectionMode = TOGGLE;
  else if(s == "ADD")
    m_selectionMode = ADD;
  else if(s == "UNSEL")
    m_selectionMode = UNSEL;
  else if(s == "ZOOM")
  {
    m_selectionMode = ZOOM;
    m_zoomer->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton);
    m_panner->setMouseButton( Qt::MidButton);

    canvas()->setCursor(*m_zoomCursor);
 }
  else if(s == "PAN")
  {
    m_selectionMode = PAN;
    m_panner->setMouseButton(Qt::LeftButton);
    m_zoomer->setMousePattern( QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::ControlModifier );

    canvas()->setCursor(Qt::OpenHandCursor);
  }

  if(m_selectionMode == ZOOM || m_selectionMode == PAN)
    m_selectionCursor->setEnabled(false);
  else
  {
    m_selectionCursor->setEnabled(true);
    m_zoomer->setMousePattern( QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::ControlModifier );
    m_panner->setMouseButton( Qt::MidButton);
    canvas()->setCursor(Qt::SizeHorCursor);
  }

  // para que os botoes F1 (undo zoom/pan) e F2 (redo zoom/pan) funcionem
  canvas()->setFocus(Qt::ActiveWindowFocusReason);
}

void te::qt::qwt::HistogramDisplay::selectionChangedSlot(te::map::DataGridOperation*)
{
  if(isVisible())
      updateHistograms();
}

void te::qt::qwt::HistogramDisplay::setNumberOfBars(int n)
{
  if(m_xType != te::dt::STRING_TYPE)
  {
    if(n % 2)
    {
      if(n > m_numberOfBars)
        m_numberOfBars = n + 1;
      else
        m_numberOfBars = n - 1;
    }

    if(m_numberOfBars < 2)
      m_numberOfBars = 2;

    double labelInterval = m_barInterval;

    adjustHistogramHorizontalAxis();
    updateHistograms();
  }
}

int te::qt::qwt::HistogramDisplay::getNumberOfBars()
{
  return m_numberOfBars;
}

void te::qt::qwt::HistogramDisplay::wheelEventSlot(int delta)
{
  int n = m_numberOfBars;

  if(delta > 0)
    ++n;
  else
    --n;

  setNumberOfBars(n);
}

void te::qt::qwt::HistogramDisplay::setMaxNumberOfHorizontalLabels(int n)
{
  m_maxNumberOfHorizontalLabels = n;
  updateHistograms();
}
