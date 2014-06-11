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
  \file terralib/qt/widgets/st/TimePropertiesWidget.cpp

  \brief A widget used to adjust a TimeSeries layer's properties
*/

//Terralib
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "TimeSeriesPropertiesWidget.h"
#include "ui_TimeSeriesPropertiesWidgetForm.h"

//QT
#include <QWidget>

te::qt::widgets::TimeSeriesPropertiesWidget::TimeSeriesPropertiesWidget(te::da::DataSet* dataSet, QWidget* parent, Qt::WindowFlags f)
: QWidget(parent, f),
  m_ui(new Ui::TimeSeriesPropertiesWidgetForm),
  m_dataSet (dataSet)
{
  m_ui->setupUi(this);
  QString item;

  for (std::size_t i = 0; i < dataSet->getNumProperties(); i++)
  {
    if(dataSet->getPropertyDataType(i) == te::dt::GEOMETRY_TYPE)
    {
      item = QString::fromStdString(dataSet->getPropertyName(i));
      m_ui->m_locationComboBox->addItem(item);
    }
    else if(dataSet->getPropertyDataType(i) != te::dt::DATETIME_TYPE)
    {
      item = QString::fromStdString(dataSet->getPropertyName(i));
      m_ui->m_idComboBox->addItem(item);
    }
  }

// connect signal and slots
  //connect(m_ui->m_phenomenomComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPropertyComboBoxIndexChanged(QString)));
}

te::qt::widgets::TimeSeriesPropertiesWidget::~TimeSeriesPropertiesWidget()
{
}

Ui::TimeSeriesPropertiesWidgetForm* te::qt::widgets::TimeSeriesPropertiesWidget::getForm()
{
  return m_ui.get();
}

void te::qt::widgets::TimeSeriesPropertiesWidget::onPropertyComboBoxIndexChanged (QString text)
{

}
