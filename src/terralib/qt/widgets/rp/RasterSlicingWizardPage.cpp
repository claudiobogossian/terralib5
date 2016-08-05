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


//Terralib
#include "../../../raster/Band.h"
#include "../../../raster/Raster.h"
#include "../../../se/ColorMap.h"
#include "../charts/ChartDisplay.h"
#include "../charts/ChartStyle.h"
#include "../charts/Histogram.h"
#include "../charts/HistogramChart.h"
#include "../charts/HistogramStyle.h"
#include "../se/ColorMapWidget.h"
#include "../utils/ScopedCursor.h"
#include "RasterSlicingWizardPage.h"
#include "ui_ColorMapWidgetForm.h"
#include "ui_RasterSlicingWizardPageForm.h"

// QT
#include <QComboBox>
#include <QString>

te::qt::widgets::RasterSlicingWizardPage::RasterSlicingWizardPage(QWidget *parent) :
  QWizardPage(parent)
{
  m_ui.reset(new Ui::RasterSlicingWizardPageForm);

  m_ui->setupUi(this);

  //configure page
  this->setTitle(tr("Raster Slicing"));
  this->setSubTitle(tr("Define the raster slicing parameters."));

  //Creating and adjusting the chart Display's style.
  m_chartStyle = new te::qt::widgets::ChartStyle();
  m_chartStyle->setTitle(tr(""));
  m_chartStyle->setAxisX(tr(""));
  m_chartStyle->setAxisY(tr(""));
  m_chartStyle->setGridChecked(true);

  //Creating and adjusting the chart Display
  QGridLayout* chartLayout = new QGridLayout(m_ui->m_histWidget);

  m_chartDisplay = new te::qt::widgets::ChartDisplay(m_ui->m_histWidget, QString::fromStdString(""), m_chartStyle);
  m_chartDisplay->adjustDisplay();
  m_chartDisplay->show();
  m_chartDisplay->replot();

  chartLayout->addWidget(m_chartDisplay, 0, 0);
  chartLayout->setContentsMargins(0, 0, 0, 0);

  m_histogram = new te::qt::widgets::Histogram();
  m_histogramChart = new te::qt::widgets::HistogramChart(m_histogram);
  m_histogramChart->setPen(Qt::black);
  m_histogramChart->setBrush(QBrush(Qt::blue));
  m_histogramChart->attach(m_chartDisplay);
  m_histogramChart->setTitle(tr("Input"));
  
  m_colorMapWidget = new te::qt::widgets::ColorMapWidget(m_ui->m_slicesWidget);
  m_colorMapWidget->setColorMap(new te::se::ColorMap());
  
  QGridLayout* slicesWidgetLayout = new QGridLayout(m_ui->m_slicesWidget);

  slicesWidgetLayout->addWidget(m_colorMapWidget);
  slicesWidgetLayout->setContentsMargins(0, 0, 0, 0);

  m_extent.makeInvalid();
  m_srid = 0;

  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), this, SLOT(onApplyPushButtonClicked()));
  connect(m_ui->m_bandComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onHistBandComboBoxIndexCHanged(int)));
  connect(m_colorMapWidget->getForm()->m_bandComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSliceBandComboBoxIndexCHanged(int)));
  connect(m_colorMapWidget->getForm()->m_tableWidget, SIGNAL(cellChanged(int, int)), this, SIGNAL(completeChanged()));
}

te::qt::widgets::RasterSlicingWizardPage::~RasterSlicingWizardPage()
{
}

te::se::ColorMap * te::qt::widgets::RasterSlicingWizardPage::getColorMap()
{
  return m_colorMapWidget->getColorMap();
}

std::string te::qt::widgets::RasterSlicingWizardPage::getCurrentBand()
{
  return m_ui->m_bandComboBox->currentText().toStdString();
}

const te::gm::Envelope& te::qt::widgets::RasterSlicingWizardPage::getExtent()
{
  return m_extent;
}

int te::qt::widgets::RasterSlicingWizardPage::getSRID()
{
  return m_srid;
}

bool te::qt::widgets::RasterSlicingWizardPage::trimRaster()
{
  bool useVisibleArea = m_ui->m_visibleAreaCheckBox->isChecked();
  bool extValid = m_extent.isValid();
  bool isWithinRaster = m_raster->getExtent()->contains(m_extent);
  
  if (useVisibleArea && extValid && isWithinRaster)
    return true;
  else
    return false;
}

void te::qt::widgets::RasterSlicingWizardPage::setRaster(te::rst::Raster* raster)
{
  m_raster.reset(raster);
  m_colorMapWidget->setRaster(m_raster.get());

  const unsigned int bandsNumber = m_raster->getNumberOfBands();

  m_ui->m_bandComboBox->clear();
  for (unsigned int band = 0; band < bandsNumber; ++band)
  {
    m_ui->m_bandComboBox->addItem(QString::number(band));
  }
}

void te::qt::widgets::RasterSlicingWizardPage::setExtent(const te::gm::Envelope& extent)
{
  m_extent = extent;
}

void te::qt::widgets::RasterSlicingWizardPage::setSRID(int srid)
{
  m_srid = srid;
}

void te::qt::widgets::RasterSlicingWizardPage::onApplyPushButtonClicked()
{
  te::qt::widgets::ScopedCursor cursor(Qt::WaitCursor);

  int band = m_ui->m_bandComboBox->currentIndex();
  int slices = m_ui->m_slicesNumberSpinBox->value();
  bool useVisibleArea = m_ui->m_visibleAreaCheckBox->isChecked();
  bool extValid = m_extent.isValid();
  bool isWithinRaster = m_raster->getExtent()->contains(m_extent);

  if (m_raster.get())
  {
    m_histogram->setValues(std::map<te::dt::AbstractData*, unsigned int>());
    std::map<double, unsigned int> values;

     if (useVisibleArea && extValid && isWithinRaster) //Slicing the desired protion of the input raster
     {
       std::map<std::string, std::string> rinfo = m_raster->getInfo();
       rinfo["FORCE_MEM_DRIVER"] = "TRUE";

       te::rst::Raster* tempRaster = m_raster->trim(&m_extent, rinfo);
       values = tempRaster->getBand(band)->getHistogramR(0, 0, 0, 0, slices);
       delete tempRaster;
     }
     else
       values = m_raster->getBand(band)->getHistogramR(0, 0, 0, 0, slices);
   
    for (std::map<double, unsigned int>::iterator it = values.begin(); it != values.end(); ++it)
    {
      m_histogram->insert(std::make_pair(new te::dt::Double(it->first), it->second));
    }

    m_histogram->setMinValue(m_raster->getBand(band)->getMinValue().real());

    m_histogramChart->setData();
  }

  m_chartDisplay->updateLayout();
  m_chartDisplay->replot();
}

void te::qt::widgets::RasterSlicingWizardPage::onHistBandComboBoxIndexCHanged(int index)
{
  m_colorMapWidget->getForm()->m_bandComboBox->setCurrentIndex(index);
}

void te::qt::widgets::RasterSlicingWizardPage::onSliceBandComboBoxIndexCHanged(int index)
{
  m_ui->m_bandComboBox->setCurrentIndex(index);
}

void te::qt::widgets::RasterSlicingWizardPage::getParameters( 
  unsigned int& inputRasterBand, bool& createPaletteRaster,
  unsigned int& slicesNumber, bool& eqHistogram )
{
  inputRasterBand = (unsigned int)m_ui->m_bandComboBox->currentText().toUInt();
  createPaletteRaster = m_ui->m_createPaletteRasterCheckBox->isChecked();
  slicesNumber = (unsigned int)m_ui->m_slicesNumberSpinBox->value();
  eqHistogram = m_ui->m_equalizeHistogramCheckBox->isChecked();
}

bool te::qt::widgets::RasterSlicingWizardPage::isComplete() const
{
  if (m_colorMapWidget->getForm()->m_tableWidget->rowCount() > 0)
    return true;
  else
    return false;
}