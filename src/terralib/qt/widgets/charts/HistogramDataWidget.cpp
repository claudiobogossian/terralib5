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
  \file terralib/qt/widgets/charts/histogramDataWidget.cpp

  \brief A widget used to adjust a histogram's input data.
*/

//Terralib
#include "../../../qt/widgets/charts/Utils.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "../../../raster.h"
#include "../../../raster/RasterSummary.h"
#include "../../../raster/RasterSummaryManager.h"
#include "Histogram.h"
#include "HistogramDataWidget.h"
#include "ui_HistogramDataWidgetForm.h"

//QT
#include <QtGui/QWidget>

#include <iostream>

te::qt::widgets::HistogramDataWidget::HistogramDataWidget(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::HistogramDataWidgetForm),
    m_dataSet (dataSet),
    m_dataType(dataType)
{
  m_ui->setupUi(this);

  QString item;

  std::size_t rpos = te::da::GetFirstPropertyPos(dataSet, te::dt::RASTER_TYPE);

  if(rpos != std::string::npos)
    {

      //Adjusting the widget to work with a raster file.
      size_t size =  dataSet->getRaster(rpos)->getNumberOfBands();
      m_ui->m_slicesSpinBox->setMaximum(size);
      m_ui->m_slicesSpinBox->setMinimum(0);
      m_ui->m_slicesSpinBox->setValue(0);

      for (size_t i = 0; i < size; i++)
      {
        item = QString::number(i);
        m_ui->m_propertyComboBox->addItem((QString::fromStdString("Band: ") + item));
      }
    }
  else
  {

    for (std::size_t i = 0; i < dataSet->getNumProperties(); i++)
    {
      if(dataSet->getPropertyDataType(i) != te::dt::GEOMETRY_TYPE)
      {
        item = QString::fromStdString(dataSet->getPropertyName(i));
        m_ui->m_propertyComboBox->addItem(item);
      }
    }
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

  //Getting the Columns that will be used to populate the chart

  size_t selectedPropertyIdx = 0;

  for (size_t i = 0; i < m_dataSet->getNumProperties(); i++)
  {
    if(m_ui->m_propertyComboBox->currentText().toStdString() == m_dataSet.get()->getPropertyName(i))
      selectedPropertyIdx = i;
  }

  int propType = m_dataSet->getPropertyDataType(selectedPropertyIdx);

  if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
  {
    histogram = te::qt::widgets::createHistogram(m_dataSet.get(), m_dataType.get(), selectedPropertyIdx);
  }
  else
  {
    histogram = te::qt::widgets::createHistogram(m_dataSet.get(), m_dataType.get(), selectedPropertyIdx,m_ui->m_slicesSpinBox->value());
  }

  return histogram;
}

void te::qt::widgets::HistogramDataWidget::onPropertyComboBoxIndexChanged (QString text)
{
  int selectedPropertyIdx= te::da::GetPropertyPos(m_dataSet.get(),  m_ui->m_propertyComboBox->currentText().toStdString());
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
