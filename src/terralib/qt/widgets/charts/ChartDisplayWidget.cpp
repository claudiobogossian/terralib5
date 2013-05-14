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
  \file terralib/qt/widgets/charts/ChartDisplayWidget.cpp

  \brief A dialog used to display a set of charts.
*/

//Terralib
#include "ChartDisplay.h"
#include "ChartDisplayWidget.h"
#include "ChartProperties.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "ui_ChartDisplayWidgetForm.h"

//QT
#include <QtGui/QWidget>

//QWT
#include <qwt_plot_seriesitem.h>

te::qt::widgets::ChartDisplayWidget::ChartDisplayWidget(QwtPlotSeriesItem* chart, int type, te::qt::widgets::ChartDisplay* display, QWidget* parent,  Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ChartDisplayWidgetForm),
    m_chart(chart),
    m_type(type),
    m_display(display)
{
    m_ui->setupUi(this);

// connect signal and slots
  connect(m_ui->m_settingsToolButton, SIGNAL(clicked()), this, SLOT(onSettingsToolButtonnTriggered()));
}

te::qt::widgets::ChartDisplayWidget::~ChartDisplayWidget(){}

QwtPlotSeriesItem* te::qt::widgets::ChartDisplayWidget::getChart()
{
  return m_chart;
}

void te::qt::widgets::ChartDisplayWidget::setChart(QwtPlotSeriesItem* newChart)
{
  m_chart = newChart;
  m_chart->attach(m_display);
  m_display->show();
  m_display->replot();
}

te::qt::widgets::ChartDisplay* te::qt::widgets::ChartDisplayWidget::getDisplay()
{
  return m_display;
}

void te::qt::widgets::ChartDisplayWidget::setDisplay(te::qt::widgets::ChartDisplay* newDisplay)
{
  m_display = newDisplay;
  QGridLayout* layout = new QGridLayout(m_ui->m_plotFrame);
  layout->addWidget(m_display);
  m_chart->attach(m_display);
  m_display->show();
  m_display->replot();
}

int te::qt::widgets::ChartDisplayWidget::getType()
{
  return m_type;
}

void te::qt::widgets::ChartDisplayWidget::onSettingsToolButtonnTriggered()
{
    te::qt::widgets::ChartProperties dlg(this, this->parentWidget());
    dlg.exec();
}
