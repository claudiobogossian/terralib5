/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file HistogramChart.cpp

  \brief A class to represent a histogram chart.
*/

//Terralib
#include "HistogramChart.h"
#include "Histogram.h"
#include "StringScaleDraw.h"
#include "../../../datatype.h"

//QWT
#include <qwt_column_symbol.h>
#include <qwt_plot.h>

te::qt::widgets::HistogramChart::HistogramChart(Histogram* histogram) :
  QwtPlotHistogram(),
  m_histogram(histogram)
{
  //Vector that will be populated by the histogram's data
  QVector<QwtIntervalSample> samples;

  if((histogram->getType() >= te::dt::INT16_TYPE && histogram->getType() <= te::dt::UINT64_TYPE) || 
    histogram->getType() == te::dt::FLOAT_TYPE || histogram->getType() == te::dt::DOUBLE_TYPE || 
    histogram->getType() == te::dt::NUMERIC_TYPE)
  {
    std::map<double,  unsigned int>::const_iterator it;
    std::map<int,  unsigned int> vmap;

    int i = 0;
    it = histogram->getValues()->begin();
    double ini = histogram->getMinValue();

    double vx = ini + histogram->getInterval();

    while(vx <= histogram->getValues()->rbegin()->first)
    {
      vmap[i] = 0;
      if(fabs(vx) < 0.000000000001)
        vx = 0.;
      while(it != histogram->getValues()->end())
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
      vx += histogram->getInterval();
      ++i;
    }

    setData(new QwtIntervalSeriesData(samples));
  }

  else if (histogram->getType() == te::dt::DATETIME_TYPE || histogram->getType() == te::dt::STRING_TYPE)
  {
    std::map<std::string,  unsigned int>::iterator it;
    it  = histogram->getStringValues()->begin();
    m_histogramScaleDraw = new StringScaleDraw(histogram->getStringInterval());
    QVector<QwtIntervalSample> samples(histogram->getStringValues()->size());
    double LabelInterval = 0.0;

    while (it != histogram->getStringValues()->end())
    {
      QwtInterval qwtInterval(LabelInterval, LabelInterval+5);
      qwtInterval.setBorderFlags(QwtInterval::ExcludeMaximum);
      samples[LabelInterval] = QwtIntervalSample(it->second, qwtInterval);
      LabelInterval++;
       it++;
    }
    
    setData(new QwtIntervalSeriesData(samples));
  }
  else
  {

    std::map<double,  unsigned int>::const_iterator it;
    it = histogram->getValues()->begin();
    double interval = 0.0;
  
    while (it != histogram->getValues()->end())
    {
      QwtInterval qwtInterval(interval, interval+1);
      samples.push_back(QwtIntervalSample(it->second, qwtInterval));
      interval++;
      it++;
    }

    setData(new QwtIntervalSeriesData(samples));
  }
}

te::qt::widgets::HistogramChart::~HistogramChart()
{  
  delete m_histogram;
//  delete m_histogramScaleDraw;
}

te::qt::widgets::StringScaleDraw* te::qt::widgets::HistogramChart::getScaleDraw()
{
  return m_histogramScaleDraw;
}

void te::qt::widgets::HistogramChart::setScaleDraw( StringScaleDraw* new_scaleDraw)
{
  m_histogramScaleDraw = new_scaleDraw;
}

void te::qt::widgets::HistogramChart::attach(QwtPlot* plot)
{
  if (m_histogram->getType() == te::dt::DATETIME_TYPE || m_histogram->getType() == te::dt::STRING_TYPE)
  {
    plot->setAxisScaleDraw(QwtPlot::xBottom, m_histogramScaleDraw);
    plot->axisScaleDraw(QwtPlot::xBottom)->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    plot->axisScaleDraw(QwtPlot::xBottom)->setLabelRotation(-60);
  }

  QwtPlotHistogram::attach(plot);
}