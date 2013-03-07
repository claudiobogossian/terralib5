#include "Curve.h"

#include <stdlib.h>

//QWT
#include <qwt_plot_canvas.h>
#include <qwt_column_symbol.h>
#include <qwt_scale_map.h>

//QT
#include <QPen>

te::qt::qwt::Curve::Curve(const QString& title) :
  QwtPlotCurve(title),
  m_xval(0),
  m_yval(0)
{
  setStyle(QwtPlotCurve::Lines);

  m_symbol = new QwtSymbol(QwtSymbol::XCross, Qt::NoBrush, QPen(Qt::darkMagenta), QSize(5, 5));
  setSymbol(m_symbol);
}

void te::qt::qwt::Curve::setColor(const QColor& c)
{
  QPen pen(c);
  m_symbol->setPen(pen);
  setSymbol(m_symbol);
}

void te::qt::qwt::Curve::setLineColor(const QColor& c)
{
  m_lineColor = c;
  setPen(m_lineColor);
}

QColor te::qt::qwt::Curve::getColor()
{
  return m_symbol->pen().color();
}

QColor te::qt::qwt::Curve::getLineColor()
{
  return m_lineColor;
}

void te::qt::qwt::Curve::setValues(const std::vector<std::pair<double, double> >& values, double xmin, double ymin, double xmax, double ymax)
{
  double vx, vy;
  int size = values.size();

  m_xval = new double[size];
  m_yval = new double[size];
  int i = 0;
  std::vector<std::pair<double, double> >::const_iterator it;
  for(it = values.begin(); it != values.end(); ++it)
  {
    vx = it->first;
    m_xval[i] = vx;

    vy = it->second;
    m_yval[i++] = vy;
  }

  setRawSamples(m_xval, m_yval, size);

  m_xmap = new QwtScaleMap(); 
  m_ymap = new QwtScaleMap();

  m_xmap->setScaleInterval(xmin, xmax);
  m_ymap->setScaleInterval(ymin, ymax);

  setRawSamples(m_xval, m_yval, size);
}

