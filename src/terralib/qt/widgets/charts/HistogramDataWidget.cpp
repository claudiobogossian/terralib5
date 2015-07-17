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
  \file terralib/qt/widgets/charts/histogramDataWidget.cpp

  \brief A widget used to adjust a histogram's input data.
*/

//Terralib
#include "../../../qt/widgets/charts/Utils.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "../../../raster.h"
#include "../../../raster/RasterSummaryManager.h"
#include "../../../statistics/core/Enums.h"
#include "../../../statistics/core/Utils.h"
#include "Histogram.h"
#include "HistogramDataWidget.h"
#include "ui_HistogramDataWidgetForm.h"

#include <iostream>

void updateSummary(te::da::DataSet* dataSet, Ui::HistogramDataWidgetForm* ui)
{
  ui->m_summaryComboBox->clear();

  size_t selectedPropertyIdx = 0;
  for (size_t i = 0; i < dataSet->getNumProperties(); i++)
  {
    if(ui->m_propertyComboBox->currentText().toStdString() == dataSet->getPropertyName(i))
      selectedPropertyIdx = i;
  }

  int propType = dataSet->getPropertyDataType(selectedPropertyIdx);

  if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
  {
    ui->m_summaryComboBox->addItem(QString::fromStdString("None"), QVariant(-1));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MIN_VALUE).c_str()), QVariant(te::stat::MIN_VALUE));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MAX_VALUE).c_str()), QVariant(te::stat::MAX_VALUE));
    //ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::COUNT).c_str()), QVariant(te::stat::COUNT));
    //ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VALID_COUNT).c_str()), QVariant(te::stat::VALID_COUNT));
  }
  else
  {
    ui->m_summaryComboBox->addItem(QString::fromStdString("None"), QVariant(-1));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MIN_VALUE).c_str()), QVariant(te::stat::MIN_VALUE));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MAX_VALUE).c_str()), QVariant(te::stat::MAX_VALUE));
    //ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::COUNT).c_str()), QVariant(te::stat::COUNT));
    //ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VALID_COUNT).c_str()), QVariant(te::stat::VALID_COUNT));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEAN).c_str()), QVariant(te::stat::MEAN));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::SUM).c_str()), QVariant(te::stat::SUM));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::STANDARD_DEVIATION).c_str()), QVariant(te::stat::STANDARD_DEVIATION));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VARIANCE).c_str()), QVariant(te::stat::VARIANCE));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::SKEWNESS).c_str()), QVariant(te::stat::SKEWNESS));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::KURTOSIS).c_str()), QVariant(te::stat::KURTOSIS));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::AMPLITUDE).c_str()), QVariant(te::stat::AMPLITUDE));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MEDIAN).c_str()), QVariant(te::stat::MEDIAN));
    ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::VAR_COEFF).c_str()), QVariant(te::stat::VAR_COEFF));
    //ui->m_summaryComboBox->addItem(QString(te::stat::GetStatSummaryFullName(te::stat::MODE).c_str()), QVariant(te::stat::MODE));
  }
  ui->m_summaryComboBox->setCurrentIndex(0);
}

te::qt::widgets::HistogramDataWidget::HistogramDataWidget(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::HistogramDataWidgetForm),
    m_dataSet (dataSet),
    m_dataType(dataType)
{
  m_ui->setupUi(this);

  QString item;

  std::size_t rpos = te::da::GetFirstPropertyPos(m_dataSet.get(), te::dt::RASTER_TYPE);

  if(rpos != std::string::npos)
    {
      //Adjusting the widget to work with a raster file.
      std::auto_ptr<te::rst::Raster> raster =  m_dataSet->getRaster(rpos);

      const te::rst::RasterSummary* rsMin = te::rst::RasterSummaryManager::getInstance().get(raster.get(), te::rst::SUMMARY_MIN, true);
      const te::rst::RasterSummary* rsMax = te::rst::RasterSummaryManager::getInstance().get(raster.get(), te::rst::SUMMARY_MAX, true);

      const std::complex<double>* cmin = rsMin->at(0).m_minVal;
      const std::complex<double>* cmax = rsMax->at(0).m_maxVal;

      double min = cmin->real();
      double max = cmax->real();

      size_t size = raster->getNumberOfBands();
      m_ui->m_slicesSpinBox->setMinimum(0);
      m_ui->m_slicesSpinBox->setValue(30);

      if (min >= 0 && max <= 255)
        m_ui->m_slicesSpinBox->setMaximum(255);
      else
        m_ui->m_slicesSpinBox->setMaximum(max);

      m_ui->m_slicesSpinBox->setValue(255);

      for (size_t i = 0; i < size; i++)
      {
        item = QString::number(i);
        m_ui->m_propertyComboBox->addItem((QString::fromStdString("Band: ") + item), QVariant::fromValue(i));
      }
    }
  else
  {
    for (std::size_t i = 0; i < dataSet->getNumProperties(); i++)
    {
      if(dataSet->getPropertyDataType(i) != te::dt::GEOMETRY_TYPE)
      {
        item = QString::fromStdString(dataSet->getPropertyName(i));
        m_ui->m_propertyComboBox->addItem(item, QVariant::fromValue(i));
      }
    }
  }

  updateSummary(m_dataSet.get(), getForm());

  if(te::da::HasLinkedTable(dataType))
  {
    m_ui->m_summaryComboBox->show();
    m_ui->m_summaryLabel->show();
  }
  else
  {
    m_ui->m_summaryComboBox->hide();
    m_ui->m_summaryLabel->hide();
  }

// connect signal and slots
  connect(m_ui->m_propertyComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPropertyComboBoxIndexChanged(QString)));
}

te::qt::widgets::HistogramDataWidget::~HistogramDataWidget()
{
}

Ui::HistogramDataWidgetForm* te::qt::widgets::HistogramDataWidget::getForm()
{
  return m_ui.get();
}

te::qt::widgets::Histogram* te::qt::widgets::HistogramDataWidget::getHistogram() 
{
  std::size_t rpos = te::da::GetFirstPropertyPos(m_dataSet.get(), te::dt::RASTER_TYPE);
  te::qt::widgets::Histogram* histogram;

  if(rpos != std::string::npos)
  {
    histogram = te::qt::widgets::createHistogram(m_dataSet.get(), m_dataType.get(), m_ui->m_propertyComboBox->itemData(m_ui->m_propertyComboBox->currentIndex()).toInt(), m_ui->m_slicesSpinBox->value(), -1);
  }
  else
  {
    //Getting the Columns that will be used to populate the chart

    size_t selectedPropertyIdx = 0;

    for (size_t i = 0; i < m_dataSet->getNumProperties(); i++)
    {
      if(m_ui->m_propertyComboBox->currentText().toStdString() == m_dataSet.get()->getPropertyName(i))
      {
        selectedPropertyIdx = i;
      }
    }

    int propType = m_dataSet->getPropertyDataType(selectedPropertyIdx);
    int stat = m_ui->m_summaryComboBox->itemData(m_ui->m_summaryComboBox->currentIndex()).toInt();

    if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
    {
      histogram = te::qt::widgets::createHistogram(m_dataSet.get(), m_dataType.get(), (int)selectedPropertyIdx, stat);
    }
    else
    {
      histogram = te::qt::widgets::createHistogram(m_dataSet.get(), m_dataType.get(), (int)selectedPropertyIdx, m_ui->m_slicesSpinBox->value(), stat);
    }
  }
  return histogram;
}

void te::qt::widgets::HistogramDataWidget::setHistogramProperty(int propId)
{
  m_ui->m_propertyComboBox->setCurrentIndex(m_ui->m_propertyComboBox->findData(propId));
  m_ui->m_propertyComboBox->setEnabled(false);
}

void te::qt::widgets::HistogramDataWidget::onPropertyComboBoxIndexChanged (QString text)
{
  std::size_t rpos = te::da::GetFirstPropertyPos(m_dataSet.get(), te::dt::RASTER_TYPE);
  if(rpos == std::string::npos)
  {
    size_t selectedPropertyIdx= te::da::GetPropertyPos(m_dataSet.get(),  m_ui->m_propertyComboBox->currentText().toStdString());
    int propType = m_dataSet->getPropertyDataType(selectedPropertyIdx);

    if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
    {
      m_ui->m_slicesSpinBox->setEnabled(false);
    }
    else
    {
      m_ui->m_slicesSpinBox->setEnabled(true);
    }
  }
  else
  {
    std::auto_ptr<te::rst::Raster> raster = m_dataSet->getRaster(rpos);

    const te::rst::RasterSummary* rsMin = te::rst::RasterSummaryManager::getInstance().get(raster.get(), te::rst::SUMMARY_MIN, true);
    const te::rst::RasterSummary* rsMax = te::rst::RasterSummaryManager::getInstance().get(raster.get(), te::rst::SUMMARY_MAX, true);

    const std::complex<double>* cmin = rsMin->at(m_ui->m_propertyComboBox->currentIndex()).m_minVal;
    const std::complex<double>* cmax = rsMax->at(m_ui->m_propertyComboBox->currentIndex()).m_maxVal;

    double min = cmin->real();
    double max = cmax->real();

    if (min >= 0 && max <= 255)
      m_ui->m_slicesSpinBox->setMaximum(255);
    else
      m_ui->m_slicesSpinBox->setMaximum(max);

    m_ui->m_slicesSpinBox->setValue(255);
  }

  updateSummary(m_dataSet.get(), getForm());

  if(te::da::HasLinkedTable(m_dataType.get()))
  {
    m_ui->m_summaryComboBox->show();
    m_ui->m_summaryLabel->show();
  }
  else
  {
    m_ui->m_summaryComboBox->hide();
    m_ui->m_summaryLabel->hide();
  }
}