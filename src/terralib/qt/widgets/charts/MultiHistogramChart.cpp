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
  \file MultiHistogramChart.cpp

  \brief A class that contains multiple histogram charts, it is responsible for attaching all of them to the same Chart Display
*/

//Terralib

#include "MultiHistogramChart.h"
#include "HistogramChart.h"
#include "ChartDisplay.h"

te::qt::widgets::MultiHistogramChart::MultiHistogramChart():
QwtPlotHistogram()
{
}

te::qt::widgets::MultiHistogramChart::~MultiHistogramChart()
{

}

std::vector<te::qt::widgets::HistogramChart*>& te::qt::widgets::MultiHistogramChart::getCharts()
{
  return m_histogramCharts;
}

void te::qt::widgets::MultiHistogramChart::setCharts(std::vector<te::qt::widgets::HistogramChart*> new_Charts)
{
  m_histogramCharts = new_Charts;
}

void te::qt::widgets::MultiHistogramChart::attach(std::vector<te::qt::widgets::HistogramChart*>& charts,  te::qt::widgets::ChartDisplay* display)
{
  for(unsigned int i = 0; i <charts.size(); i++)
  {
    charts[i]->attach(display);
  }
}