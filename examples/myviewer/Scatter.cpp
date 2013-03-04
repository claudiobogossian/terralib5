#include "Scatter.h"

#include <stdlib.h>

//QWT
#include <qwt_plot_canvas.h>
#include <qwt_column_symbol.h>
#include <qwt_scale_map.h>

//QT
#include <QPen>

//STL
#include <limits>

te::qt::qwt::Scatter::Scatter(const QString& title) :
  QwtPlotCurve(title),
  m_xval(0),
  m_yval(0)
{
  setStyle(QwtPlotCurve::NoCurve);

  m_symbol = new QwtSymbol(QwtSymbol::XCross, Qt::NoBrush, QPen(Qt::darkMagenta), QSize(5, 5));
  setSymbol(m_symbol);
}

void te::qt::qwt::Scatter::setColor(const QColor& c)
{
  QPen pen(c);
  m_symbol->setPen(pen);
  setSymbol(m_symbol);
}

QColor te::qt::qwt::Scatter::getColor()
{
  return m_symbol->pen().color();
}

void te::qt::qwt::Scatter::setValues(const std::vector<std::pair<double, double> >& values)
{
  std::vector<std::pair<double, double> >::const_iterator it;
  int size = values.size();
  delete m_xval;
  delete m_yval;
  m_xval = 0;
  m_yval = 0;

  if(size == 0)
  {
    setRawSamples(m_xval, m_yval, size);
    return;
  }

  double vx, vy;
  double xmin = std::numeric_limits<double>::max();
  double xmax = -(std::numeric_limits<double>::max());
  double ymin = std::numeric_limits<double>::max();
  double ymax = -(std::numeric_limits<double>::max());

  m_xval = new double[size];
  m_yval = new double[size];
  int i = 0;
  for(it = values.begin(); it != values.end(); ++it)
  {
    vx = it->first;
    m_xval[i] = vx;
    if(vx < xmin)
      xmin = vx;
    if(vx > xmax)
      xmax = vx;

    vy = it->second;
    m_yval[i++] = vy;
    if(vy < ymin)
      ymin = vy;
    if(vy > ymax)
      ymax = vy;
  }

  setRawSamples(m_xval, m_yval, size);

  m_xmap = new QwtScaleMap(); 
  m_ymap = new QwtScaleMap();

  m_xmap->setScaleInterval(xmin, xmax);
  m_ymap->setScaleInterval(ymin, ymax);
////..............................................................................................
//setPen(QColor(Qt::red));
//setStyle(QwtPlotCurve::Lines);
//
////ordernar em funcao do eixo x
//
//int j, a;
//for(i=0; i<(size-1); i++)
//{
//  for(j=0;j<(size-(i+1));j++)
//  {
//    if(m_xval[j] > m_xval[j+1])
//    {
//      a = m_xval[j];
//      m_xval[j] = m_xval[j+1];
//      m_xval[j+1] = a;
//
//      a = m_yval[j];
//      m_yval[j] = m_yval[j+1];
//      m_yval[j+1] = a;
//    }
//  }
//}
//
//setRawSamples(m_xval, m_yval, size);
}

