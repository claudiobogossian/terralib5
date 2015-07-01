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
  \file terralib/qt/af/settings/ScatterFrame.cpp

  \brief A frame used to adjust a scater's chart parameters, weather it is about it's data or it's visual style.
*/

//Terralib
#include "ui_ScatterFrameWidgetForm.h"
#include "ScatterChart.h"
#include "ScatterDataWidget.h"
#include "ScatterFrame.h"
#include "ScatterStyleWidget.h"

te::qt::widgets::ScatterFrame::ScatterFrame(QWidget* parent)
  : ChartWidget(parent),
    m_ui(new Ui::ScatterFrameWidgetForm)
{
  m_ui->setupUi(this);
  m_label = "Scatter";

  m_styleWidget = new te::qt::widgets::ScatterStyleWidget();

}

te::qt::widgets::ScatterFrame::~ScatterFrame()
{
  delete m_styleWidget;
}

QwtPlotSeriesItem* te::qt::widgets::ScatterFrame::getChart()
{
  m_chart->setScatterStyle(m_styleWidget->getScatterStyle());
  return m_chart;
}

void te::qt::widgets::ScatterFrame::setChart(QwtPlotSeriesItem* newChart)
{
  m_chart = static_cast<te::qt::widgets::ScatterChart*>(newChart);
  m_styleWidget = new te::qt::widgets::ScatterStyleWidget(m_chart->getScatterStyle());
  QGridLayout* styleLayout = new QGridLayout(m_ui->m_scatterStyleFrame);
  styleLayout->addWidget(m_styleWidget);
}