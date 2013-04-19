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
#include "ui_histogramDataWidgetForm.h"

//QT
#include <QtGui/QWidget>

#include <iostream>

te::qt::widgets::HistogramDataWidget::HistogramDataWidget(te::da::DataSet* dataSet, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::HistogramDataWidgetForm),
    m_dataSet (dataSet)
{
  m_ui->setupUi(this);

  QString item;

  std::size_t rpos = te::da::GetFirstPropertyPos(dataSet, te::dt::RASTER_TYPE);

  if(rpos != std::string::npos)
    {
      size_t size =  dataSet->getRaster(rpos)->getNumberOfBands();
      for (size_t i = 0; i < size; i++)
      {
        item = QString::number(i);
        m_ui->m_propertyComboBox->addItem(item);
      }
    }
  else
  {
    for (std::size_t i = 0; i < dataSet->getNumProperties(); i++)
    {
      item = QString::fromStdString(dataSet->getPropertyName(i));
      m_ui->m_propertyComboBox->addItem(item);
    }
  }

// connect signal and slots
  connect(m_ui->m_propertyComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPropertyComboBoxIndexChanged(QString)));
}

te::qt::widgets::HistogramDataWidget::~HistogramDataWidget()
{

}

te::qt::widgets::Histogram* te::qt::widgets::HistogramDataWidget::getHistogram() 
{
  std::size_t rpos = te::da::GetFirstPropertyPos(m_dataSet, te::dt::RASTER_TYPE);

  if(rpos != std::string::npos)
  {
    m_histogram = te::qt::widgets::createHistogram(m_dataSet, m_ui->m_propertyComboBox->currentIndex());
  }
  else
  {
    //Getting the Columns that will be used to populate the graph

    size_t selectedPropertyIdx = 0;

    for (size_t i = 0; i < m_dataSet->getNumProperties(); i++)
    {
      if(m_ui->m_propertyComboBox->currentText().toStdString() == m_dataSet->getPropertyName(i))
        selectedPropertyIdx = i;
    }

    int propType = m_dataSet->getPropertyDataType(selectedPropertyIdx);

    if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
    {
      m_histogram = te::qt::widgets::createHistogram(m_dataSet, selectedPropertyIdx);
    }
    else
    {
      m_histogram = te::qt::widgets::createHistogram(m_dataSet, selectedPropertyIdx,m_ui->m_slicesSpinBox->value());
    }
  }
  return m_histogram;
}

void te::qt::widgets::HistogramDataWidget::onPropertyComboBoxIndexChanged (QString text)
{
  std::size_t rpos = te::da::GetFirstPropertyPos(m_dataSet, te::dt::RASTER_TYPE);
  if(rpos != std::string::npos)
  {
    m_ui->m_slicesSpinBox->setEnabled(false);
  }
  else 
  {
    int selectedPropertyIdx= te::da::GetPropertyPos(m_dataSet,  m_ui->m_propertyComboBox->currentText().toStdString());
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
}
