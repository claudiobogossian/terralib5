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
  \file terralib/qt/widgets/charts/HistogramCreatorDialog.cpp

  \brief A widget used to create a nem Histogram chart.
*/

//Terralib
#include "ChartDisplay.h"
#include "ChartStyleDialog.h"
#include "../utils/ColorPickerToolButton.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "HistogramCreatorDialog.h"
#include "Histogram.h"
#include "HistogramChart.h"
#include "HistogramStyle.h"
#include "../../../qt/widgets/charts/Utils.h"
#include "../../../qt/widgets/se/Utils.h"
#include "../../../raster.h"
#include "../../../raster/RasterSummary.h"
#include "../../../raster/RasterSummaryManager.h"
#include "ui_histogramCreatorDialog.h"


//QT
#include <QtGui/QDialog>

#include <iostream>

te::qt::widgets::HistogramCreatorDialog::HistogramCreatorDialog(te::da::DataSet* dataSet, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::HistogramDialog),
    m_dataSet (dataSet)
{
  m_ui->setupUi(this);

  // Color Picker
  m_colorPicker = new te::qt::widgets::ColorPickerToolButton(this);
  m_colorPicker->setFixedSize(107, 24);

  // Adjusting the color picker
  QGridLayout* layout = new QGridLayout(m_ui->m_colorPickerFrame);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(m_colorPicker);

  QString item;

  if(dataSet->getRaster())
    {
      size_t size =  dataSet->getRaster()->getNumberOfBands();
      for (unsigned int i = 0; i < size; i++)
      {
        item = QString::number(i);
        m_ui->m_propertyComboBox->addItem(item);
      }
    }
  else
  {
    std::vector<te::dt::Property*>& properties = dataSet->getType()->getProperties();

    for (double i = 0; i < properties.size(); i++)
    {
      item = QString::fromStdString(properties[i]->getName());
      m_ui->m_propertyComboBox->addItem(item);
    }
  }

  m_histogramStyle = new te::qt::widgets::HistogramStyle(); 
  m_chartDisplay = new te::qt::widgets::ChartDisplay();

// connect signal and slots
  connect(m_ui->m_stylePushButton, SIGNAL(clicked()), this, SLOT(onStylePushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_propertyComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPropertyComboBoxIndexChanged(QString)));
  connect(m_colorPicker, SIGNAL(colorChanged(const QColor&)), SLOT(onColorChanged(const QColor&)));
}

te::qt::widgets::HistogramCreatorDialog::~HistogramCreatorDialog()
{

}

void te::qt::widgets::HistogramCreatorDialog::onStylePushButtonClicked()
{
  te::qt::widgets::ChartStyleDialog dlg(this, 0, "Histogram", m_ui->m_propertyComboBox->currentText(), "Frequency");
  dlg.exec();
}

void te::qt::widgets::HistogramCreatorDialog::onOkPushButtonClicked()
{

  if(m_dataSet->getRaster())
  {
    int teste  =  m_ui->m_propertyComboBox->currentIndex();
    m_histogram = te::qt::widgets::createHistogram(m_dataSet, m_ui->m_propertyComboBox->currentIndex());
  }
  else
  {
    //Acquiring the dataset Properties types
    m_type = m_dataSet->getType();

    //Getting the Columns that will be used to populate the graph
    int selectedPropertyIdx= m_type->getPropertyPosition(m_ui->m_propertyComboBox->currentText().toStdString());
    int propType = m_dataSet->getType()->getProperty(selectedPropertyIdx)->getType();

    if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
    {
      m_histogram = te::qt::widgets::createHistogram(m_dataSet, selectedPropertyIdx);
    }
    else
    {
      m_histogram = te::qt::widgets::createHistogram(m_dataSet, selectedPropertyIdx,m_ui->m_slicesSpinBox->value());
    }
  }

  QDialog dlg(this);
  QGridLayout* lay = new QGridLayout(&dlg);
  dlg.setLayout(lay);

  m_chartDisplay->setParent(&dlg);

  lay->addWidget(m_chartDisplay);

  m_histogramChart = new te::qt::widgets::HistogramChart(m_histogram);
  m_histogramChart->setSymbol(m_histogramStyle->getSymbol());
  m_histogramChart->attach(m_chartDisplay);

  m_chartDisplay->show();

  m_chartDisplay->replot();

  this->close();
  dlg.exec();
}

void te::qt::widgets::HistogramCreatorDialog::onHelpPushButtonClicked()
{

}

void te::qt::widgets::HistogramCreatorDialog::onPropertyComboBoxIndexChanged (QString text)
{
  m_type = m_dataSet->getType();
  QString aux = m_ui->m_propertyComboBox->currentText();
  std::string selectedProperty = aux.toStdString();
  if(!(m_dataSet->getRaster()))
  {
    int selectedPropertyIdx= m_type->getPropertyPosition(selectedProperty);
    int propType = m_dataSet->getType()->getProperty(selectedPropertyIdx)->getType();

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
    m_ui->m_slicesSpinBox->setEnabled(false);
  }
}

void te::qt::widgets::HistogramCreatorDialog::onColorChanged(const QColor& color)
{
  // The new fill color
  m_histogramStyle->setColor(color);

  m_colorPicker->setColor(m_histogramStyle->getColor());
}
