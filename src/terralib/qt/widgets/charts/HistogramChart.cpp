/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file ScatterChart.cpp

  \brief A class to represent a histogram chart.
*/

//QWT
#include <qwt_plot_histogram.h>
#include "HistogramChart.h"
#include "Histogram.h"

//exemplo
#include <qpen.h>
#include <qwt_column_symbol.h>

te::qt::widgets::HistogramChart::HistogramChart(Histogram* histogram) :
  QwtPlotHistogram(),
  m_histogram(histogram)
{
  //Vector that will be populated by the histogram's data
  QVector<QwtIntervalSample> samples;

  std::map<double, int>::const_iterator it;
  std::map<int, int> vmap;

  int i = 0;
  it = histogram->getValues().begin();
  double ini = histogram->getMinValue();
  
  double vx = ini + histogram->getInterval();
  while(vx <= histogram->getValues().rbegin()->first)
  {
    vmap[i] = 0;
    if(fabs(vx) < 0.000000000001)
      vx = 0.;
    while(it != histogram->getValues().end())
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
  vmap[i] = 0;
  if(fabs(vx) < 0.000000000001)
    vx = 0.;
  while(it != histogram->getValues().end())
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

  QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
  setSymbol(symbol);
}

te::qt::widgets::HistogramChart::~HistogramChart()
{  
  delete m_histogram;  
}

