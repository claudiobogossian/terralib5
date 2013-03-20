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

#include "ui_histogramCreatorDialog.h"
#include "HistogramCreatorDialog.h"
#include "ChartStyleDialog.h"
#include "Histogram.h"
#include "HistogramChart.h"
#include "ChartDisplay.h"
#include "../../../qt/widgets/charts/Utils.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"

//QT
#include <QtGui/QDialog>

#include <iostream>

te::qt::widgets::HistogramCreatorDialog::HistogramCreatorDialog(te::da::DataSet* dataSet, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::HistogramDialog),
    m_dataSet (dataSet)
{
  m_ui->setupUi(this);
  QString item;
  std::vector<te::dt::Property*>& properties = dataSet->getType()->getProperties();

  for (double i = 0; i < properties.size(); i++)
  {
    item = item.fromStdString(properties[i]->getName());
    m_ui->m_propertyComboBox->addItem(item);
  }

// connect signal and slots
  connect(m_ui->m_stylePushButton, SIGNAL(clicked()), this, SLOT(onStylePushButtonClicked()));
  connect(m_ui->m_barStylePushButton, SIGNAL(clicked()), this, SLOT(onBarStylePushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_propertyComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPropertyComboBoxIndexChanged(QString)));
}

te::qt::widgets::HistogramCreatorDialog::~HistogramCreatorDialog()
{

}

void te::qt::widgets::HistogramCreatorDialog::onStylePushButtonClicked()
{
    te::qt::widgets::ChartStyleDialog dlg(this);
    dlg.exec();
}

void te::qt::widgets::HistogramCreatorDialog::onBarStylePushButtonClicked()
{

}

void te::qt::widgets::HistogramCreatorDialog::onOkPushButtonClicked()
{

  //Acquiring the dataset Properties types
  m_type = m_dataSet->getType();

  //Getting the Columns that will be used to populate the graph
  QString aux = m_ui->m_propertyComboBox->currentText();
  std::string selectedProperty = aux.toStdString();
  int selectedPropertyIdx= m_type->getPropertyPosition(selectedProperty);

  m_histogram = te::qt::widgets::createHistogram(m_dataSet, selectedPropertyIdx,m_ui->m_slicesSpinBox->value());
//passar como parametro o HistogramStyle
  m_histogramChart = new te::qt::widgets::HistogramChart(m_histogram);

  QDialog dlg(this);
  QGridLayout* lay = new QGridLayout(&dlg);
  dlg.setLayout(lay);
  
//passar como parametro o chartstyle
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(&dlg);

  lay->addWidget(chartDisplay);

  m_histogramChart->attach(chartDisplay);

  chartDisplay->show();

  chartDisplay->replot();

  dlg.exec();
  this->close();
}

void te::qt::widgets::HistogramCreatorDialog::onCancelPushButtonClicked()
{

}

void te::qt::widgets::HistogramCreatorDialog::onHelpPushButtonClicked()
{

}

void te::qt::widgets::HistogramCreatorDialog::onPropertyComboBoxIndexChanged (QString text)
{
  m_type = m_dataSet->getType();
  QString aux = m_ui->m_propertyComboBox->currentText();
  std::string selectedProperty = aux.toStdString();
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