#include "ScatterDisplay.h"
#include "StringScaleDraw.h"

//C Library
#include <stdlib.h>

//QT
#include <QPen>
#include <QPainter>
#include <QWheelEvent>
#include <QBoxLayout>
#include <QGroupBox>
#include <QColorDialog>

//QWT
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_legend_item.h>
#include <qwt_plot_grid.h>
#include <qwt_column_symbol.h>
#include <qwt_scale_div.h>
#include <qwt_picker_machine.h>

//TerraLib
#include "../../color.h"

te::qt::qwt::ScatterDisplay::ScatterDisplay(int colx, int coly, te::map::DataGridOperation* op, QWidget *parent):
  Plot("SCATTER", op, parent),
  m_legendMenu(0),
  m_allScatter(0),
  m_deselectedScatter(0),
  m_pointedScatter(0),
  m_queriedScatter(0),
  m_pointedAndQueriedScatter(0),
  m_selectionCursor(0)
{
  m_xCol = colx;
  m_yCol = coly;

  m_xStringScaleDraw = new te::qt::qwt::StringScaleDraw();
  m_yStringScaleDraw = new te::qt::qwt::StringScaleDraw();

  te::da::DataSet* dataSet = m_op->getDataSet(); 
  te::da::DataSetType* dsType = m_op->getDataSetType();
  te::dt::Property *propx = dsType->getProperty(m_xCol);
  m_xType = propx->getType();
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

  te::dt::Property *propy = dsType->getProperty(m_yCol);
  m_yType = propy->getType();
  if(m_yType == te::dt::DATETIME_TYPE)
  {
    dataSet->moveBeforeFirst();
    while(dataSet->moveNext())
    {
      if(dataSet->isNull(m_yCol))
        continue;
      te::dt::DateTime* t = dataSet->getDateTime(m_yCol);
      m_yTimeType = t->getDateTimeType();
      break;
    }
  }

  te::da::PrimaryKey *pk = dsType->getPrimaryKey();
  const std::vector<te::dt::Property*>& pkProps = pk->getProperties();
  std::string pkName = pkProps[0]->getName();
  m_PKPos = dsType->getPropertyPosition(pkName);

  std::string propNamex = propx->getName();
  std::string propNamey = propy->getName();
  setHorizontalTitle(propNamex.c_str());
  setVerticalTitle(propNamey.c_str());

  plotLayout()->setAlignCanvasToScales(true);

  QwtPlotGrid *grid = new QwtPlotGrid;

  // mostra a grade horizontal
  grid->enableX(true);
  // mostra a grade vertical
  grid->enableY(true);

  //a grade (mais espacada) e' mostrada em preto e linha solido
  grid->setMajPen(QPen(Qt::black, 0, Qt::SolidLine));

  //a grade e' mostrada em cinza e pontilhado
  grid->setMinPen(QPen(Qt::gray, 0, Qt::DotLine));

  grid->attach(this);

  if(m_xType == te::dt::STRING_TYPE)
  {
    if(m_yType == te::dt::STRING_TYPE)
    {
      grid->enableXMin(false);
      grid->enableYMin(false);
    }
    else if((m_yType >= te::dt::INT16_TYPE && m_yType <= te::dt::UINT64_TYPE) || m_yType == te::dt::FLOAT_TYPE || m_yType == te::dt::DOUBLE_TYPE || m_yType == te::dt::NUMERIC_TYPE)
    {
      grid->enableXMin(false);
      grid->enableYMin(true);
    }
  }
  else if(m_yType == te::dt::STRING_TYPE)
  {
    if((m_xType >= te::dt::INT16_TYPE && m_xType <= te::dt::UINT64_TYPE) || m_xType == te::dt::FLOAT_TYPE || m_xType == te::dt::DOUBLE_TYPE || m_xType == te::dt::NUMERIC_TYPE)
    {
      grid->enableXMin(true);
      grid->enableYMin(false);
    }
  }
  else if((m_xType >= te::dt::INT16_TYPE && m_xType <= te::dt::UINT64_TYPE) || m_xType == te::dt::FLOAT_TYPE || m_xType == te::dt::DOUBLE_TYPE || m_xType == te::dt::NUMERIC_TYPE)
  {
    if((m_yType >= te::dt::INT16_TYPE && m_yType <= te::dt::UINT64_TYPE) || m_yType == te::dt::FLOAT_TYPE || m_yType == te::dt::DOUBLE_TYPE || m_yType == te::dt::NUMERIC_TYPE)
    {
      grid->enableXMin(true);
      grid->enableYMin(true);
    }
  }
  createScatters();
  attachScatters();

  if(m_legend == 0) // Scatter nao foi criado
    return;

  //inicializa zoom e pan
  Plot::init();

  //controle para visualizar (on-off) cada Scatter
  QObject::connect(this, SIGNAL(legendChecked(QwtPlotItem* , bool)), this, SLOT(showScatterSlot(QwtPlotItem* , bool)));

  QObject::connect(m_legend, SIGNAL(legendContextMenu(QPoint&, QWidget*)), this, SLOT(legendMenuSlot(QPoint&, QWidget*)));

  //inicializa cursor de selecao
  m_selectionCursor = new ScatterSelectionCursor(canvas());
  m_selectionCursor->setRubberBandPen( QColor( Qt::magenta ) );
  m_selectionCursor->setTrackerPen( QColor( Qt::black ) );
  m_selectionCursor->setMousePattern( QwtEventPattern::MouseSelect1, Qt::LeftButton);
  //m_selectionCursor->setRubberBand(QwtPicker::RectRubberBand);
  m_selectionCursor->setRubberBand(QwtPicker::PolygonRubberBand);
  m_selectionCursor->setTrackerMode(QwtPicker::AlwaysOff);
  //QwtPickerClickRectMachine* mac = new QwtPickerClickRectMachine();
  QwtPickerPolygonMachine* mac = new QwtPickerPolygonMachine();
  m_selectionCursor->setStateMachine(mac);
  QObject::connect(m_selectionCursor, SIGNAL(select(QRect)), this, SLOT(selectSlot(QRect)));

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

te::qt::qwt::ScatterDisplay::~ScatterDisplay()
{
  if(m_legend)
  {
    m_legend->disconnect();
    disconnect();
  }

  delete m_allScatter;
  delete m_deselectedScatter;
  delete m_pointedScatter;
  delete m_queriedScatter;
  delete m_pointedAndQueriedScatter;
  delete m_selectionCursor;

  if(axisScaleDraw(QwtPlot::xBottom) != m_xStringScaleDraw)
    delete m_xStringScaleDraw;
  if(axisScaleDraw(QwtPlot::yLeft) != m_yStringScaleDraw)
    delete m_yStringScaleDraw;
}


void te::qt::qwt::ScatterDisplay::closeEvent(QCloseEvent *event)
{
  setAttribute(Qt::WA_DeleteOnClose);
  Q_EMIT closed(this);
  event->accept();
}

bool te::qt::qwt::ScatterDisplay::createScatters()
{
  std::vector<std::pair<double, double> > allValues, deselectedValues, pointedValues, queriedValues, pointedAndQueriedValues;
  std::vector<std::pair<double, double> >::iterator it;
  QString s;
  double vx, vy;
  std::string pkv;
  int selection;

  te::da::DataSet* dataSet = m_op->getDataSet(); 
  dataSet->moveBeforeFirst();
  while(dataSet->moveNext())
  {
    if(m_xType == te::dt::STRING_TYPE)
    {
      if(dataSet->isNull(m_xCol) == false)
        s = dataSet->getString(m_xCol).c_str();
      else
        s = " null value";
      m_xStringSet.insert(s);
    }
    if(m_yType == te::dt::STRING_TYPE)
    {
      if(dataSet->isNull(m_yCol) == false)
        s = dataSet->getString(m_yCol).c_str();
      else
        s = " null value";
      m_yStringSet.insert(s);
    }
  }

  dataSet->moveBeforeFirst();
  while(dataSet->moveNext())
  {
    if(m_xType == te::dt::STRING_TYPE)
    {
      if(dataSet->isNull(m_xCol) == false)
        s = dataSet->getString(m_xCol).c_str();
      else
        s = " null value";
      vx = getXValue(s);
    }
    else if(m_xType == te::dt::DATETIME_TYPE ||
      (m_xType >= te::dt::INT16_TYPE && m_xType <= te::dt::UINT64_TYPE) || 
      m_xType == te::dt::FLOAT_TYPE || m_xType == te::dt::DOUBLE_TYPE || m_xType == te::dt::NUMERIC_TYPE)
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
        vx = dias * 86400 + seconds;
      }
      else if(m_xTimeType == te::dt::DATE)
      {
        te::dt::Date* d = (te::dt::Date*)dataSet->getDateTime(m_xCol);
        te::dt::Date t = *d;
        delete d;
        boost::gregorian::date basedate(1400, 01, 01);
        boost::gregorian::date_duration days = t.getDate() - basedate;
        vx = days.days();
      }
      else
      {
        s = dataSet->getAsString(m_xCol).c_str();
        vx = s.toDouble();
      }
    }

    if(m_yType == te::dt::STRING_TYPE)
    {
      if(dataSet->isNull(m_yCol) == false)
        s = dataSet->getString(m_yCol).c_str();
      else
        s = " null value";
      vy = getYValue(s);
    }
    else if(m_yType == te::dt::DATETIME_TYPE ||
      (m_yType >= te::dt::INT16_TYPE && m_yType <= te::dt::UINT64_TYPE) || 
      m_yType == te::dt::FLOAT_TYPE || m_yType == te::dt::DOUBLE_TYPE || m_yType == te::dt::NUMERIC_TYPE)
    {
      if(dataSet->isNull(m_yCol))
        continue;

      if(m_yTimeType == te::dt::TIME_INSTANT)
      {
        te::dt::TimeInstant* ti = (te::dt::TimeInstant*)dataSet->getDateTime(m_yCol);
        te::dt::TimeInstant t = *ti;
        delete ti;

        boost::gregorian::date basedate(1400, 01, 01);
        boost::gregorian::date_duration days = t.getDate().getDate() - basedate;

        long long int seconds = t.getTime().getTimeDuration().total_seconds();
        long long int dias = days.days();
        vy = dias * 86400 + seconds;
      }
      else if(m_yTimeType == te::dt::DATE)
      {
        te::dt::Date* d = (te::dt::Date*)dataSet->getDateTime(m_yCol);
        te::dt::Date t = *d;
        delete d;
        boost::gregorian::date basedate(1400, 01, 01);
        boost::gregorian::date_duration days = t.getDate() - basedate;
        vy = days.days();
      }
      else
      {
        s = dataSet->getAsString(m_yCol).c_str();
        vy = s.toDouble();
      }
    }

    if(m_xType == te::dt::STRING_TYPE)
    {
      m_xMin = m_XMIN = 0.;
      m_xMax = m_XMAX = m_xStringSet.size();
    }
    if(m_yType == te::dt::STRING_TYPE)
    {
      m_yMin = m_YMIN = 0;
      m_yMax = m_YMAX = m_yStringSet.size();
    }

    std::pair<double, double> pv(vx, vy);
    allValues.push_back(pv);

    pkv = dataSet->getAsString(m_PKPos);
    selection = m_op->getDataSetSelectionStatus(pkv);
    if(!(selection == te::map::DataGridOperation::POINTED || selection == te::map::DataGridOperation::QUERIED))
      deselectedValues.push_back(pv);
    else if(selection == te::map::DataGridOperation::POINTED)
      pointedValues.push_back(pv);
    else if(selection == te::map::DataGridOperation::QUERIED)
      queriedValues.push_back(pv);
    else if(selection == te::map::DataGridOperation::POINTED_AND_QUERIED)
      pointedAndQueriedValues.push_back(pv);  
  }

  if(allValues.empty())
    return false;

  if(m_tableChanged)
  {
    m_tableChanged = false;

    if(m_xType == te::dt::STRING_TYPE)
      m_xStringScaleDraw->setStringSet(m_xStringSet);
    else if(m_xType == te::dt::DATETIME_TYPE)
      m_xStringScaleDraw->setTimeType(m_xTimeType);
    if(m_xType == te::dt::STRING_TYPE || m_xType == te::dt::DATETIME_TYPE)
    {
      setAxisScaleDraw(QwtPlot::xBottom, m_xStringScaleDraw);
      axisScaleDraw(QwtPlot::xBottom)->setLabelRotation(-60);
      axisScaleDraw(QwtPlot::xBottom)->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }

    if(m_yType == te::dt::STRING_TYPE)
      m_yStringScaleDraw->setStringSet(m_yStringSet);
    else if(m_yType == te::dt::DATETIME_TYPE)
      m_yStringScaleDraw->setTimeType(m_yTimeType);
    if(m_yType == te::dt::STRING_TYPE || m_yType == te::dt::DATETIME_TYPE)
      setAxisScaleDraw(QwtPlot::yLeft, m_yStringScaleDraw);
  }

  adjustScatterLabels();

  QColor qcor(m_color.getRed(), m_color.getGreen(), m_color.getBlue(), 200);
  if(m_allScatter == 0)
    m_allScatter = new Scatter("All");
  m_allScatter->setValues(allValues);
  m_allScatter->setColor(qcor);

  te::color::RGBAColor cor = m_op->getDefaultColor();
  qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue(), 200);
  if(m_deselectedScatter == 0)
    m_deselectedScatter = new Scatter("Deselected");
  m_deselectedScatter->setValues(deselectedValues);
  m_deselectedScatter->setColor(qcor);

  cor = m_op->getPointedColor();
  qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue(), 200);
  if(m_pointedScatter == 0)
    m_pointedScatter = new Scatter("Pointed");
  m_pointedScatter->setValues(pointedValues);
  m_pointedScatter->setColor(qcor);

  cor = m_op->getQueriedColor();
  qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue(), 200);
  if(m_queriedScatter == 0)
    m_queriedScatter = new Scatter("Queried");
  m_queriedScatter->setValues(queriedValues);
  m_queriedScatter->setColor(qcor);

  cor = m_op->getPointedAndQueriedColor();
  qcor = QColor(cor.getRed(), cor.getGreen(), cor.getBlue(), 200);
  if(m_pointedAndQueriedScatter == 0)
    m_pointedAndQueriedScatter = new Scatter("Point/Query");
  m_pointedAndQueriedScatter->setValues(pointedAndQueriedValues);
  m_pointedAndQueriedScatter->setColor(qcor);

  return true;
}

void te::qt::qwt::ScatterDisplay::attachScatters()
{

  //A ordem de desenho é de acordo com os attaches
  m_pointedAndQueriedScatter->attach(this);
  m_queriedScatter->attach(this);
  m_pointedScatter->attach(this);
  m_deselectedScatter->attach(this);
  m_allScatter->attach(this);

  m_scatterVec.push_back(m_pointedAndQueriedScatter);
  m_scatterVec.push_back(m_queriedScatter);
  m_scatterVec.push_back(m_pointedScatter);
  m_scatterVec.push_back(m_deselectedScatter);
  m_scatterVec.push_back(m_allScatter);

  m_scatterMap[m_allScatter] = false;
  m_scatterMap[m_deselectedScatter] = false;
  m_scatterMap[m_pointedScatter] = false;
  m_scatterMap[m_queriedScatter] = false;
  m_scatterMap[m_pointedAndQueriedScatter] = false;

  //create Scatter legends
  m_legend = new Legend;
  m_legend->setItemMode(QwtLegend::CheckableItem);
  insertLegend(m_legend);

  //set visibility
  showScatterSlot(m_allScatter, true);
  showScatterSlot(m_deselectedScatter, true);
  showScatterSlot(m_pointedScatter, true);
  showScatterSlot(m_queriedScatter, true);
  showScatterSlot(m_pointedAndQueriedScatter, true);
}

double te::qt::qwt::ScatterDisplay::getXValue(const QString& s)
{
  int i = 0;
  std::set<QString>::iterator it = m_xStringSet.begin();

  while(it != m_xStringSet.end())
  {
    if(*it == s)
      break;
    ++i;
    ++it;
  }

  return (double)i;
}

double te::qt::qwt::ScatterDisplay::getYValue(const QString& s)
{
  int i = 0;
  std::set<QString>::iterator it = m_yStringSet.begin();

  while(it != m_yStringSet.end())
  {
    if(*it == s)
      break;
    ++i;
    ++it;
  }

  return (double)i;
}

void te::qt::qwt::ScatterDisplay::updateScatters()
{
  createScatters();
  replot();
  show();
}

void te::qt::qwt::ScatterDisplay::showScatterSlot(QwtPlotItem* item, bool on)
{
  Scatter* hitem = (Scatter*)item;
  m_scatterMap[hitem] = on;
  updateVisibity();
}

void te::qt::qwt::ScatterDisplay::legendMenuSlot(QPoint& p, QWidget* w)
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

void te::qt::qwt::ScatterDisplay::legendToFrontSlot()
{
  Scatter* hitem = (Scatter*)m_legend->find(m_legendWidget);
  std::vector<Scatter*> hVec;
  std::vector<Scatter*>::iterator it = m_scatterVec.begin();
  if(*it != hitem)
  {
    std::vector<Scatter*>::reverse_iterator rit = m_scatterVec.rbegin();
    while(rit != m_scatterVec.rend())
    {
      Scatter* h = *rit;
      if(h == hitem)
      {
        rit++;
        if(rit != m_scatterVec.rend())
        {
          hVec.push_back(*rit);
          (*rit)->detach();
        }
      }
      hVec.push_back(h);
      h->detach();
      ++rit;
    }
  
    m_scatterVec.clear();
    rit = hVec.rbegin();
    while(rit != hVec.rend())
    {
      (*rit)->attach(this);
      m_scatterVec.push_back(*rit);
      rit++;
    }
  }
  updateVisibity();
}

void te::qt::qwt::ScatterDisplay::legendToBackSlot()
{
  Scatter* hitem = (Scatter*)m_legend->find(m_legendWidget);
  std::vector<Scatter*> hVec;
  std::vector<Scatter*>::reverse_iterator rit = m_scatterVec.rbegin();
  if(*rit != hitem)
  {
    std::vector<Scatter*>::iterator it = m_scatterVec.begin();
    while(it != m_scatterVec.end())
    {
      Scatter* h = *it;
      if(h == hitem)
      {
        it++;
        if(it != m_scatterVec.end())
        {
          hVec.push_back(*it);
          (*it)->detach();
        }
      }
      hVec.push_back(h);
      h->detach();
      ++it;
    }
  
    m_scatterVec.clear();
    it = hVec.begin();
    while(it != hVec.end())
    {
      (*it)->attach(this);
      m_scatterVec.push_back(*it);
      it++;
    }
  }
  updateVisibity();
}

void te::qt::qwt::ScatterDisplay::legendColorSlot()
{
  QColor oldColor, color;
  te::color::RGBAColor cor;

  Scatter* hitem = (Scatter*)m_legend->find(m_legendWidget);
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

void te::qt::qwt::ScatterDisplay::updateVisibity()
{
  std::map<Scatter*, bool>::iterator mit = m_scatterMap.begin();
  while(mit != m_scatterMap.end())
  {
    Scatter* h = mit->first;
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
void te::qt::qwt::ScatterDisplay::drawItems( QPainter *painter, const QRectF &canvasRect,
        const QwtScaleMap map[axisCnt] ) const
{
  const QwtPlotItemList& itmList = itemList();
  int size = itmList.size();
  for(int i = size-1; i >= 0; --i)
  {
    QwtPlotItem *item = itmList[i];
    if(item && item->isVisible() )
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

void te::qt::qwt::ScatterDisplay::selectSlot(QRect rec)
{
  double x0 = invTransform(QwtPlot::xBottom, rec.left());
  double x1 = invTransform(QwtPlot::xBottom, rec.right());
  double y0 = invTransform(QwtPlot::yLeft, rec.top());
  double y1 = invTransform(QwtPlot::yLeft, rec.bottom());

  QRectF recf(QPointF(x0, y0), QPointF(x1, y1));

  std::vector<int> visRows;
  select(recf, visRows);

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

  updateScatters();

  Q_EMIT selectionChanged(m_op);
}

void te::qt::qwt::ScatterDisplay::select(QRectF rec, std::vector<int>& visRows)
{
  QString s;
  double vx, vy;
  std::string pkv;

  double x0 = rec.left();
  double x1 = rec.right();
  double y0 = rec.bottom();
  double y1 = rec.top();

  te::da::DataSet* dataSet = m_op->getDataSet(); 
  dataSet->moveBeforeFirst();
  while(dataSet->moveNext())
  {
    if(m_xType == te::dt::STRING_TYPE)
    {
      if(dataSet->isNull(m_xCol) == false)
        s = dataSet->getString(m_xCol).c_str();
      else
        s = " null value";
      vx = getXValue(s);
    }
    else if(m_xType == te::dt::DATETIME_TYPE ||
      (m_xType >= te::dt::INT16_TYPE && m_xType <= te::dt::UINT64_TYPE) || 
      m_xType == te::dt::FLOAT_TYPE || m_xType == te::dt::DOUBLE_TYPE || m_xType == te::dt::NUMERIC_TYPE)
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
        vx = dias * 86400 + seconds;
      }
      else if(m_xTimeType == te::dt::DATE)
      {
        te::dt::Date* d = (te::dt::Date*)dataSet->getDateTime(m_xCol);
        te::dt::Date t = *d;
        delete d;
        boost::gregorian::date basedate(1400, 01, 01);
        boost::gregorian::date_duration days = t.getDate() - basedate;
        vx = days.days();
      }
      else
      {
        s = dataSet->getAsString(m_xCol).c_str();
        vx = s.toDouble();
      }
    }

    if(m_yType == te::dt::STRING_TYPE)
    {
      if(dataSet->isNull(m_yCol) == false)
        s = dataSet->getString(m_yCol).c_str();
      else
        s = " null value";
      vy = getYValue(s);
    }
    else if(m_yType == te::dt::DATETIME_TYPE ||
      (m_yType >= te::dt::INT16_TYPE && m_yType <= te::dt::UINT64_TYPE) || 
      m_yType == te::dt::FLOAT_TYPE || m_yType == te::dt::DOUBLE_TYPE || m_yType == te::dt::NUMERIC_TYPE)
    {
      if(dataSet->isNull(m_yCol))
        continue;

      if(m_yTimeType == te::dt::TIME_INSTANT)
      {
        te::dt::TimeInstant* ti = (te::dt::TimeInstant*)dataSet->getDateTime(m_yCol);
        te::dt::TimeInstant t = *ti;
        delete ti;

        boost::gregorian::date basedate(1400, 01, 01);
        boost::gregorian::date_duration days = t.getDate().getDate() - basedate;

        long long int seconds = t.getTime().getTimeDuration().total_seconds();
        long long int dias = days.days();
        vy = dias * 86400 + seconds;
      }
      else if(m_yTimeType == te::dt::DATE)
      {
        te::dt::Date* d = (te::dt::Date*)dataSet->getDateTime(m_yCol);
        te::dt::Date t = *d;
        delete d;
        boost::gregorian::date basedate(1400, 01, 01);
        boost::gregorian::date_duration days = t.getDate() - basedate;
        vy = days.days();
      }
      else
      {
        s = dataSet->getAsString(m_yCol).c_str();
        vy = s.toDouble();
      }
    }

    if(vx > x0 && vx < x1 && vy > y0 && vy < y1)
    {
      pkv = dataSet->getAsString(m_PKPos);
      int logRow = m_op->getLogicalRow(pkv);
      visRows.push_back(m_op->getVisualRow(logRow));
    }
  }
}

void te::qt::qwt::ScatterDisplay::buttonClickedSlot(QAbstractButton* button)
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

    //canvas()->setCursor(Qt::OpenHandCursor);
    canvas()->setCursor(*m_panCursor);
  }

  if(m_selectionMode == ZOOM || m_selectionMode == PAN)
    m_selectionCursor->setEnabled(false);
  else
  {
    m_selectionCursor->setEnabled(true);
    m_zoomer->setMousePattern( QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::ControlModifier );
    m_panner->setMouseButton( Qt::MidButton);
    canvas()->setCursor(Qt::CrossCursor);
  }

  // para que os botoes F1 (undo zoom/pan) e F2 (redo zoom/pan) funcionem
  canvas()->setFocus(Qt::ActiveWindowFocusReason);
}

void te::qt::qwt::ScatterDisplay::selectionChangedSlot(te::map::DataGridOperation*)
{
  if(isVisible())
    updateScatters();
}
