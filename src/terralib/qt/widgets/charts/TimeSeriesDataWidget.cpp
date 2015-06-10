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
  \file terralib/qt/widgets/charts/TimeSeriesDataWidget.cpp

  \brief A widget used to adjust a timeSeries' input data.
*/

//Terralib
#include "../../../qt/widgets/charts/Utils.h"
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "../../../st/core/timeseries/TimeSeries.h"
#include "TimeSeriesDataWidget.h"
#include "ui_TimeSeriesDataWidgetForm.h"

//QT
#include <QMessageBox>

te::qt::widgets::TimeSeriesDataWidget::TimeSeriesDataWidget(te::da::DataSet* dataSet, te::da::DataSetType* dataType, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::TimeSeriesDataWidgetForm),
    m_dataSet (dataSet),
    m_dataType(dataType)
{
  m_ui->setupUi(this);
  QString item;

  for (std::size_t i = 0; i < m_dataSet->getNumProperties(); i++)
  {
    switch(m_dataSet->getPropertyDataType(i))
    {
      case(te::dt::DATETIME_TYPE):
        item = QString::fromStdString(m_dataSet->getPropertyName(i));
        m_ui->m_timeComboBox->addItem(item, QVariant::fromValue(i));
        break;
      case(te::dt::GEOMETRY_TYPE):
        break;
      default:
        item = QString::fromStdString(m_dataSet->getPropertyName(i));
        m_ui->m_valueComboBox->addItem(item, QVariant::fromValue(i));
        m_ui->m_idComboBox->addItem(item, QVariant::fromValue(i));
    }
  }

// connect signal and slots
  connect(m_ui->m_timeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onTimeComboBoxIndexChanged(QString)));
  connect(m_ui->m_valueComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onValueComboBoxIndexChanged(QString)));
  connect(m_ui->m_idComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onIdComboBoxIndexChanged(QString)));
}

te::qt::widgets::TimeSeriesDataWidget::~TimeSeriesDataWidget()
{
}

Ui::TimeSeriesDataWidgetForm* te::qt::widgets::TimeSeriesDataWidget::getForm()
{
  return m_ui.get();
}

te::st::TimeSeries* te::qt::widgets::TimeSeriesDataWidget::getTimeSeries()
{
  te::st::TimeSeries* timeSeries = new te::st::TimeSeries(m_ui->m_idComboBox->currentText().toStdString());

  m_dataSet->moveBeforeFirst();
  while(m_dataSet->moveNext())
  {
    //Get time and value of time series
    std::auto_ptr<te::dt::DateTime> time(m_dataSet->getDateTime(m_ui->m_timeComboBox->currentText().toStdString()));
    std::auto_ptr<te::dt::AbstractData> value(m_dataSet->getValue(m_ui->m_valueComboBox->currentText().toStdString()));
    timeSeries->add(time.release(), value.release());
  }

  return timeSeries;
}





