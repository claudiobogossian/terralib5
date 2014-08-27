/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/rp/RasterHistogramWidget.cpp

  \brief This file has the RasterHistogramWidget class.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Band.h"
#include "../../../raster/Raster.h"
#include "../charts/ChartDisplay.h"
#include "../charts/ChartStyle.h"
#include "../charts/Histogram.h"
#include "../charts/HistogramChart.h"
#include "../charts/HistogramStyle.h"
#include "RasterHistogramWidget.h"
#include "ui_RasterHistogramWidgetForm.h"

// Qt
#include <QGridLayout>

//Qwt
#include <qwt/qwt_legend.h>

//STL
#include <memory>

te::qt::widgets::RasterHistogramWidget::RasterHistogramWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::RasterHistogramWidgetForm)
{
  m_ui->setupUi(this);

  m_ui->m_applyToolButton->setIcon(QIcon::fromTheme("chart-bar"));

  //Creating and adjusting the chart Display's style.
  m_chartStyle = new te::qt::widgets::ChartStyle();
  m_chartStyle->setTitle(tr(""));
  m_chartStyle->setAxisX(tr("Gray Level"));
  m_chartStyle->setAxisY(tr("Frequency"));
  m_chartStyle->setGridChecked(true);

  //build form
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);

  //Creating and adjusting the chart Display
  m_chartDisplay = new te::qt::widgets::ChartDisplay(m_ui->m_widget, QString::fromStdString(""), m_chartStyle);
  m_chartDisplay->adjustDisplay();
  m_chartDisplay->show();
  m_chartDisplay->replot();

  layout->addWidget(m_chartDisplay, 0, 0);
  layout->setContentsMargins(0,0,0,0);

  m_histogramInput = new te::qt::widgets::Histogram();
  m_histogramChartInput = new te::qt::widgets::HistogramChart(m_histogramInput);
  m_histogramChartInput->setPen(Qt::black);
  m_histogramChartInput->setBrush(QBrush(Qt::blue));
  m_histogramChartInput->attach(m_chartDisplay);
  m_histogramChartInput->setTitle(tr("Input"));

  m_histogramOutput = new te::qt::widgets::Histogram();
  m_histogramChartOutput = new te::qt::widgets::HistogramChart(m_histogramOutput);
  m_histogramChartOutput->setPen(Qt::black, 3.);
  m_histogramChartOutput->setBrush(QBrush(QColor(255, 0, 0, 127)));
  m_histogramChartOutput->setStyle(QwtPlotHistogram::Outline);
  m_histogramChartOutput->attach(m_chartDisplay);
  m_histogramChartOutput->setTitle(tr("Output"));

  m_chartDisplay->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

  //connects
  connect(m_ui->m_applyToolButton, SIGNAL(clicked()), this, SLOT(onApplyToolButtonClicked()));
  connect(m_chartDisplay, SIGNAL(leftPointSelected(const QPointF &)), this, SLOT(onLeftPointSelected(const QPointF &)));
  connect(m_chartDisplay, SIGNAL(rigthPointSelected(const QPointF &)), this, SLOT(onRigthPointSelected(const QPointF &)));
}

te::qt::widgets::RasterHistogramWidget::~RasterHistogramWidget()
{
}

Ui::RasterHistogramWidgetForm* te::qt::widgets::RasterHistogramWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::RasterHistogramWidget::setInputRaster(te::rst::Raster* raster)
{
  m_inputRaster.reset(raster);

  //set bands from input raster
  m_ui->m_bandComboBox->clear();

  for(unsigned int i = 0; i < m_inputRaster->getNumberOfBands(); ++i)
  {
    QString strBand;
    strBand.setNum(i);

    m_ui->m_bandComboBox->addItem(strBand);
  }
}

void te::qt::widgets::RasterHistogramWidget::setOutputRaster(te::rst::Raster* raster)
{
  m_outputRaster.reset(raster);
}

void te::qt::widgets::RasterHistogramWidget::onApplyToolButtonClicked()
{
  int index = m_ui->m_bandComboBox->currentIndex();

  if(m_inputRaster.get())
  {
    m_histogramInput->setValues(std::map<te::dt::AbstractData*, unsigned int>());

    std::map<double, unsigned int> values =  m_inputRaster->getBand(index)->getHistogramR();

    for(std::map<double, unsigned int>::iterator it = values.begin(); it != values.end(); ++it)
    {
      m_histogramInput->insert(std::make_pair(new te::dt::Double(it->first), it->second));
    }

    m_histogramInput->setMinValue(m_inputRaster->getBand(index)->getMinValue().real());

    m_histogramChartInput->setData();
  }

  if(m_outputRaster.get())
  {
    m_histogramOutput->setValues(std::map<te::dt::AbstractData*, unsigned int>());

    std::map<double, unsigned int> values =  m_outputRaster->getBand(index)->getHistogramR();

    for(std::map<double, unsigned int>::iterator it = values.begin(); it != values.end(); ++it)
    {
      m_histogramOutput->insert(std::make_pair(new te::dt::Double(it->first), it->second));
    }

    m_histogramOutput->setMinValue(m_outputRaster->getBand(index)->getMinValue().real());

    m_histogramChartOutput->setData();
  }

  m_chartDisplay->updateLayout();

  m_chartDisplay->replot();
}

void te::qt::widgets::RasterHistogramWidget::onLeftPointSelected(const QPointF& point)
{
  int xMin = (int)point.x();

  emit minValueSelected(xMin, m_ui->m_bandComboBox->currentIndex());
}

void te::qt::widgets::RasterHistogramWidget::onRigthPointSelected(const QPointF& point)
{
  int xMax = (int)point.x();

  emit maxValueSelected(xMax, m_ui->m_bandComboBox->currentIndex());
}
