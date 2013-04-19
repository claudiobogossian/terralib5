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

  \brief A dialog used to create a nem Histogram chart.
*/

//Terralib
#include "BasicFillDialog.h"
#include "BasicStrokeDialog.h"
#include "ChartDisplay.h"
#include "ChartStyleDialog.h"
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
#include "ui_histogramCreatorDialogForm.h"

//QT
#include <QtGui/QDialog>

te::qt::widgets::HistogramCreatorDialog::HistogramCreatorDialog(te::da::DataSet* dataSet, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::HistogramCreatorDialogForm),
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

  m_histogramStyle = new te::qt::widgets::HistogramStyle(); 
  m_chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Histogram"));

// connect signal and slots
  connect(m_ui->m_stylePushButton, SIGNAL(clicked()), this, SLOT(onStylePushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_propertyComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPropertyComboBoxIndexChanged(QString)));
  connect(m_ui->m_fillPushButton, SIGNAL(clicked()), this, SLOT(onFillPushButtonClicked()));
  connect(m_ui->m_strokePushButton, SIGNAL(clicked()), this, SLOT(onStrokePushButtonClicked()));
}

te::qt::widgets::HistogramCreatorDialog::~HistogramCreatorDialog()
{

}

void te::qt::widgets::HistogramCreatorDialog::onStylePushButtonClicked()
{
  m_chartDisplay->setStyle(te::qt::widgets::ChartStyleDialog::getStyle(m_chartDisplay->getStyle(), this, 0, "Histogram", m_ui->m_propertyComboBox->currentText(), "Frequency"));
}

void te::qt::widgets::HistogramCreatorDialog::onOkPushButtonClicked()
{
  std::size_t rpos = te::da::GetFirstPropertyPos(m_dataSet, te::dt::RASTER_TYPE);

  if(rpos != std::string::npos)
  {
    int teste  =  m_ui->m_propertyComboBox->currentIndex();
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

  QDialog dlg(this);
  QGridLayout* lay = new QGridLayout(&dlg);
  dlg.setLayout(lay);

  m_chartDisplay->setParent(&dlg);

  lay->addWidget(m_chartDisplay);

  m_histogramChart = new te::qt::widgets::HistogramChart(m_histogram);

  QPen barPen;
  QBrush barBrush;

  te::qt::widgets::Config(barPen, m_histogramStyle->getStroke());
  te::qt::widgets::Config(barBrush, m_histogramStyle->getFill());
  barBrush.setStyle(Qt::SolidPattern);

  m_histogramChart->setPen(barPen);
  m_histogramChart->setBrush(barBrush);

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
  std::size_t rpos = te::da::GetFirstPropertyPos(m_dataSet, te::dt::RASTER_TYPE);
  if(rpos != std::string::npos)
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
  else 
  {
    m_ui->m_slicesSpinBox->setEnabled(false);
  }
}

void te::qt::widgets::HistogramCreatorDialog::onFillPushButtonClicked()
{
  m_histogramStyle->setFill(te::qt::widgets::BasicFillDialog::getFill(m_histogramStyle->getFill(), 0, "Bar Fill"));
}

void te::qt::widgets::HistogramCreatorDialog::onStrokePushButtonClicked()
{
    m_histogramStyle->setStroke(te::qt::widgets::BasicStrokeDialog::getStroke(m_histogramStyle->getStroke(), 0, "Bar Fill"));
}
