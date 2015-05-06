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

te::qt::widgets::TimeSeriesPropertiesWidget::TimeSeriesPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
: QWidget(parent, f),
  m_ui(new Ui::TimeSeriesPropertiesWidgetForm)
{
  m_ui->setupUi(this);
  QString item;
}

te::qt::widgets::TimeSeriesPropertiesWidget::~TimeSeriesPropertiesWidget()
{
}

Ui::TimeSeriesPropertiesWidgetForm* te::qt::widgets::TimeSeriesPropertiesWidget::getForm()
{
  return m_ui.get();
}

std::string te::qt::widgets::TimeSeriesPropertiesWidget::getIdPropName()
{
  return m_ui->m_idComboBox->currentText().toStdString();
}

int te::qt::widgets::TimeSeriesPropertiesWidget::getIdIndex()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_idComboBox->currentText().toStdString());
  else
    return -1;
}

std::string te::qt::widgets::TimeSeriesPropertiesWidget::getValuePropName()
{
  return m_ui->m_valueComboBox->currentText().toStdString();
}

int te::qt::widgets::TimeSeriesPropertiesWidget::getValueIndex()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_valueComboBox->currentText().toStdString());
  else
    return -1;
}

std::string te::qt::widgets::TimeSeriesPropertiesWidget::getGeometryPropName()
{
  return m_ui->m_locationComboBox->currentText().toStdString();
}

int te::qt::widgets::TimeSeriesPropertiesWidget::getGeometryId()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_locationComboBox->currentText().toStdString());
  else
    return -1;
}

void te::qt::widgets::TimeSeriesPropertiesWidget::setUp(const te::da::DataSetTypePtr dataType)
{
  QString item;
  m_dataType = dataType;

  const std::vector<te::dt::Property*>& properties = dataType->getProperties();

  for (std::size_t i = 0; i < properties.size(); i++)
  {
    if(properties.at(i)->getType() == te::dt::GEOMETRY_TYPE)
    {
      item = QString::fromStdString(properties.at(i)->getName());
      m_ui->m_locationComboBox->addItem(item);
    }
    else if(properties.at(i)->getType() != te::dt::DATETIME_TYPE)
    {
      item = QString::fromStdString(properties.at(i)->getName());
      m_ui->m_idComboBox->addItem(item);
      m_ui->m_valueComboBox->addItem(item);
    }
  }
}

void te::qt::widgets::TimeSeriesPropertiesWidget::onPropertyComboBoxIndexChanged (QString text)
{

}
