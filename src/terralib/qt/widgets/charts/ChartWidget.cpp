/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/ChartWidget.cpp

  \brief A base widget to be used on the chart settings.
*/

//Terralib
#include "ChartWidget.h"

te::qt::widgets::ChartWidget::ChartWidget(QWidget* parent)
  : QWidget(parent)
{

}

te::qt::widgets::ChartWidget::~ChartWidget()
{

}


QwtPlotSeriesItem* te::qt::widgets::ChartWidget::getChart()
{
  return m_chart;
}


te::qt::widgets::ChartDisplay* te::qt::widgets::ChartWidget::getDisplay()
{
  return m_display;
}

void te::qt::widgets::ChartWidget::setChart(QwtPlotSeriesItem* newChart)
{
  m_chart = newChart;
}


void te::qt::widgets::ChartWidget::setDisplay(te::qt::widgets::ChartDisplay* newDisplay)
{
  m_display = newDisplay;
}