#include "Histogram.h"

//C Library
#include <stdlib.h>

//QT
#include <QPen>

//QWT
#include <qwt_plot_canvas.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>

te::qt::qwt::Histogram::Histogram(const QColor &symbolColor, const QString& title) :
  QwtPlotHistogram(title)
{
  setStyle(QwtPlotHistogram::Columns);

  setColor(symbolColor);
}

void te::qt::qwt::Histogram::setColor(const QColor &symbolColor)
{
//define como cada barrinha do histograma e' desenhada
  QColor color = symbolColor;
  color.setAlpha(180);

  setPen(QPen(Qt::black));
  setBrush(QBrush(color));

  QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
  symbol->setFrameStyle(QwtColumnSymbol::Raised);
  symbol->setLineWidth(2);
  symbol->setPalette(QPalette(color));
  setSymbol(symbol);
}

void te::qt::qwt::Histogram::setValues(const std::map<double, int> values, int type, int timeType, double min, double interval)
{
//define os valores do eixo x. O valor minimo e os intervalos
  int size = values.size();
  if(size <= 1)
    return;

  if(type == te::dt::STRING_TYPE)
  {
    int v;
    std::map<double, int>::const_iterator it;

    double dv;
    QVector<QwtIntervalSample> samples(size);
    it = values.begin();
    while (it != values.end())
    {
      v = it->second;
      dv = it->first - .5;
      QwtInterval qinterval(dv, dv+1.);
      qinterval.setBorderFlags(QwtInterval::ExcludeMaximum);      
      samples[it->first] = QwtIntervalSample(v, qinterval);
      it++;
    }
    setData(new QwtIntervalSeriesData(samples));
  }
  else
  {
    std::map<double, int>::const_iterator it;
    std::map<int, int> vmap;

    QVector<QwtIntervalSample> samples;
    int i = 0;
    it = values.begin();
    double ini = min;
    double vx = ini + interval;
    while(vx < values.rbegin()->first)
    {
      vmap[i] = 0;
      if(fabs(vx) < 0.000000000001)
        vx = 0.;
      while(it != values.end())
      {
        if(it->first >= ini && it->first < vx)
          vmap[i] += it->second;
        else
          break;
        ++it;
      }

      QwtInterval qinterval(ini, vx);
      qinterval.setBorderFlags(QwtInterval::ExcludeMaximum);  
      samples.push_back(QwtIntervalSample(vmap[i], qinterval));

      ini = vx;
      vx += interval;
      ++i;
    }
    vmap[i] = 0;
    if(fabs(vx) < 0.000000000001)
      vx = 0.;
    while(it != values.end())
    {
      if(it->first >= ini && it->first < vx)
        vmap[i] += it->second;
      else
        break;
      ++it;
    }

    QwtInterval qinterval(ini, vx);
    qinterval.setBorderFlags(QwtInterval::ExcludeMaximum);  
    samples.push_back(QwtIntervalSample(vmap[i], qinterval));

    setData(new QwtIntervalSeriesData(samples));
  }
}
