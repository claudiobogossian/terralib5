#include "TimeSeriesDisplay.h"
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

te::qt::qwt::TimeSeriesDisplay::TimeSeriesDisplay(const TimeSeries& ts, std::vector<std::pair<std::string, int> > cols, int xCol, QWidget* parent):
  Plot("TIMESERIES", 0, parent),
  m_legendMenu(0)
{
  m_xCol = xCol;
  std::vector<std::pair<std::string, int> >::iterator vit;
  for(vit = cols.begin(); vit != cols.end(); ++vit)
    m_columns.insert(vit->second);

  m_xStringScaleDraw = new te::qt::qwt::StringScaleDraw();

  m_yType = te::dt::DOUBLE_TYPE; // tem que ser numero

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
  grid->enableXMin(true);
  grid->enableYMin(true);

  createCurves(ts);

  //inicializa zoom e pan
  Plot::init();

  //controle para visualizar (on-off) cada Curve
  QObject::connect(this, SIGNAL(legendChecked(QwtPlotItem* , bool)), this, SLOT(showTimeSeriesSlot(QwtPlotItem* , bool)));

  QObject::connect(m_legend, SIGNAL(legendContextMenu(QPoint&, QWidget*)), this, SLOT(legendMenuSlot(QPoint&, QWidget*)));
}

te::qt::qwt::TimeSeriesDisplay::~TimeSeriesDisplay()
{
  if(m_legend)
  {
    m_legend->disconnect();
    disconnect();
  }

  if(axisScaleDraw(QwtPlot::xBottom) != m_xStringScaleDraw)
    delete m_xStringScaleDraw;
}

void te::qt::qwt::TimeSeriesDisplay::createCurves(const TimeSeries& ts)
{
  std::map<std::string, std::vector<std::pair<double, double> > >::const_iterator mit = ts.m_curveMap.begin();
  std::vector<std::pair<double, double> >::const_iterator vit;
  QColor qcor;
  
  m_xTimeType = ts.m_timeType;
  m_xStringScaleDraw->setTimeType(m_xTimeType);
  setAxisScaleDraw(QwtPlot::xBottom, m_xStringScaleDraw);
  axisScaleDraw(QwtPlot::xBottom)->setLabelRotation(-60);
  axisScaleDraw(QwtPlot::xBottom)->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  std::vector<Curve*> curves;
  std::vector<Curve*>::iterator it;

  while(mit != ts.m_curveMap.end())
  {
    std::string title = mit->first;
    std::map<std::string, QColor>::const_iterator ii = ts.m_colorMap.find(title);
    qcor = ii->second;
    Curve* curve = new Curve(title.c_str());
    curve->setColor(QColor(150, 100, 150));
    curve->setLineColor(qcor);
    curve->setValues(mit->second, ts.m_xMin, ts.m_yMin, ts.m_xMax, ts.m_yMax);
    curves.push_back(curve);
    ++mit;
  }

  it = curves.begin();
  while(it != curves.end())
  {
    (*it)->attach(this);
    m_curveVec.push_back(*it);
    m_curveMap[*it] = false;
    ++it;
  }

  //create Curve legends
  m_legend = new Legend;
  m_legend->setItemMode(QwtLegend::CheckableItem);
  insertLegend(m_legend);

  //set visibility
  it = m_curveVec.begin();
  while(it != m_curveVec.end())
  {
    showTimeSeriesSlot(*it, true);
    ++it;
  }
}

void te::qt::qwt::TimeSeriesDisplay::updateVisibity()
{
  std::map<Curve*, bool>::iterator mit = m_curveMap.begin();
  while(mit != m_curveMap.end())
  {
    Curve* c = mit->first;
    bool b = mit->second;
    c->setVisible(b);
    QWidget *w = legend()->find(c);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(b);
    ++mit;
  }

  replot();
}

void te::qt::qwt::TimeSeriesDisplay::drawItems(QPainter* painter, const QRectF& canvasRect, const QwtScaleMap map[axisCnt] ) const
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

void te::qt::qwt::TimeSeriesDisplay::showTimeSeriesSlot(QwtPlotItem* item, bool on)
{
  Curve* hitem = (Curve*)item;
  m_curveMap[hitem] = on;
  updateVisibity();
}

void te::qt::qwt::TimeSeriesDisplay::legendMenuSlot(QPoint& p, QWidget* w)
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

    action = new QAction("&Color...", m_legendMenu);
    m_legendMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(legendColorSlot()));
  }
  m_legendMenu->exec(p);
}

void te::qt::qwt::TimeSeriesDisplay::legendToFrontSlot()
{
  Curve* hitem = (Curve*)m_legend->find(m_legendWidget);
  std::vector<Curve*> hVec;
  std::vector<Curve*>::iterator it = m_curveVec.begin();
  if(*it != hitem)
  {
    std::vector<Curve*>::reverse_iterator rit = m_curveVec.rbegin();
    while(rit != m_curveVec.rend())
    {
      Curve* h = *rit;
      if(h == hitem)
      {
        rit++;
        if(rit != m_curveVec.rend())
        {
          hVec.push_back(*rit);
          (*rit)->detach();
        }
      }
      hVec.push_back(h);
      h->detach();
      ++rit;
    }
  
    m_curveVec.clear();
    rit = hVec.rbegin();
    while(rit != hVec.rend())
    {
      (*rit)->attach(this);
      m_curveVec.push_back(*rit);
      rit++;
    }
  }
  updateVisibity();
}

void te::qt::qwt::TimeSeriesDisplay::legendToBackSlot()
{
  Curve* hitem = (Curve*)m_legend->find(m_legendWidget);
  std::vector<Curve*> hVec;
  std::vector<Curve*>::reverse_iterator rit = m_curveVec.rbegin();
  if(*rit != hitem)
  {
    std::vector<Curve*>::iterator it = m_curveVec.begin();
    while(it != m_curveVec.end())
    {
      Curve* h = *it;
      if(h == hitem)
      {
        it++;
        if(it != m_curveVec.end())
        {
          hVec.push_back(*it);
          (*it)->detach();
        }
      }
      hVec.push_back(h);
      h->detach();
      ++it;
    }
  
    m_curveVec.clear();
    it = hVec.begin();
    while(it != hVec.end())
    {
      (*it)->attach(this);
      m_curveVec.push_back(*it);
      it++;
    }
  }
  updateVisibity();
}

void te::qt::qwt::TimeSeriesDisplay::legendColorSlot()
{
  QColor oldColor, color;

  Curve* hitem = (Curve*)m_legend->find(m_legendWidget);
  oldColor = hitem->getLineColor();

  color = QColorDialog::getColor(oldColor, this);
  if (color.isValid()) 
  {
    hitem->setLineColor(color);
    replot();
  }
}

std::set<int> te::qt::qwt::TimeSeriesDisplay::getColumns()
{
  return m_columns;
}
 