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
#include "../../../raster/Raster.h"
#include "../../../se/Fill.h"
#include "../../../se/Stroke.h"
#include "../../../se/Utils.h"
#include "../charts/ChartDisplay.h"
#include "../charts/ChartStyle.h"
#include "../charts/Histogram.h"
#include "../charts/HistogramChart.h"
#include "../charts/HistogramStyle.h"
#include "../charts/Utils.h"
#include "RasterHistogramWidget.h"
#include "ui_RasterHistogramWidgetForm.h"

// Qt
#include <QGridLayout>

//STL
#include <memory>

te::qt::widgets::RasterHistogramWidget::RasterHistogramWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::RasterHistogramWidgetForm)
{
  m_ui->setupUi(this);

  m_ui->m_redToolButton->setIcon(QIcon::fromTheme("bullet-red"));
  m_ui->m_greenToolButton->setIcon(QIcon::fromTheme("bullet-green"));
  m_ui->m_blueToolButton->setIcon(QIcon::fromTheme("bullet-blue"));
  m_ui->m_monoToolButton->setIcon(QIcon::fromTheme("bullet-black"));

  connect(m_ui->m_redToolButton, SIGNAL(toggled(bool)), this, SLOT(onRedToolButtonToggled(bool)));
  connect(m_ui->m_greenToolButton, SIGNAL(toggled(bool)), this, SLOT(onGreenToolButtonToggled(bool)));
  connect(m_ui->m_blueToolButton, SIGNAL(toggled(bool)), this, SLOT(onBlueToolButtonToggled(bool)));
  connect(m_ui->m_monoToolButton, SIGNAL(toggled(bool)), this, SLOT(onMonoToolButtonToggled(bool)));

  //Creating and adjusting the chart Display's style.
  m_chartStyle = new te::qt::widgets::ChartStyle();
  m_chartStyle->setTitle(tr(""));
  m_chartStyle->setAxisX(tr("Gray Level"));
  m_chartStyle->setAxisY(tr("Frequency"));

  //build form
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);

  //Creating and adjusting the chart Display
  m_chartDisplay = new te::qt::widgets::ChartDisplay(m_ui->m_widget, QString::fromStdString(""), m_chartStyle);
  m_chartDisplay->adjustDisplay();
  m_chartDisplay->show();
  m_chartDisplay->replot();

  layout->addWidget(m_chartDisplay, 0, 0);
  layout->setContentsMargins(0,0,0,0);
}

te::qt::widgets::RasterHistogramWidget::~RasterHistogramWidget()
{
}

Ui::RasterHistogramWidgetForm* te::qt::widgets::RasterHistogramWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::RasterHistogramWidget::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  //list bands
  m_ui->m_redComboBox->clear();
  m_ui->m_greenComboBox->clear();
  m_ui->m_blueComboBox->clear();
  m_ui->m_monoComboBox->clear();

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
  {
    QString strBand;
    strBand.setNum(i);

    m_ui->m_redComboBox->addItem(strBand);
    m_ui->m_greenComboBox->addItem(strBand);
    m_ui->m_blueComboBox->addItem(strBand);
    m_ui->m_monoComboBox->addItem(strBand);
  }
}

void te::qt::widgets::RasterHistogramWidget::onRedToolButtonToggled(bool flag)
{
  int band = m_ui->m_redComboBox->currentText().toInt();

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());
  std::auto_ptr<te::da::DataSetType> dsType((te::da::DataSetType*)m_layer->getSchema().get());

  te::qt::widgets::Histogram* hist = te::qt::widgets::createHistogram(ds.get(), dsType.get(), band);

  te::se::Fill* fill = te::se::CreateFill("#009900", "1.0");

  te::se::Stroke* stroke = te::se::CreateStroke("#000000", "1");

  te::qt::widgets::HistogramChart* histChart = new te::qt::widgets::HistogramChart(hist);

  m_chartDisplay->adjustDisplay();
  histChart->attach(m_chartDisplay);
  m_chartDisplay->show();
  m_chartDisplay->replot();

 // delete histChart;
}

void te::qt::widgets::RasterHistogramWidget::onGreenToolButtonToggled(bool flag)
{
}

void te::qt::widgets::RasterHistogramWidget::onBlueToolButtonToggled(bool flag)
{
}

void te::qt::widgets::RasterHistogramWidget::onMonoToolButtonToggled(bool flag)
{
}
