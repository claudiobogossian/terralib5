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
  \file terralib/qt/widgets/st/ObservationPropertiesWidget.cpp

  \brief A widget used to adjust a Observation layer's properties
*/

//Terralib
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "../utils/DoubleListWidget.h"
#include "ObservationPropertiesWidget.h"
#include "ui_ObservationPropertiesWidgetForm.h"

//QT
#include <QWidget>

te::qt::widgets::ObservationPropertiesWidget::ObservationPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ObservationPropertiesWidgetForm)
{
  m_ui->setupUi(this);

  //Adjusting the doubleListWidget that will be used to configure the query's fields.
  m_obsWidget.reset(new DoubleListWidget(m_ui->m_obsPropFrame));
  m_obsWidget->setLeftLabel("Properties");
  m_obsWidget->setRightLabel("Observed Properties");
  QGridLayout* obsLayout = new QGridLayout(m_ui->m_obsPropFrame);
  obsLayout->addWidget(m_obsWidget.get());
  obsLayout->setContentsMargins(0, 0, 0, 0);

// connect signal and slots
  //connect(m_ui->m_phenomenomComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPropertyComboBoxIndexChanged(QString)));
}

te::qt::widgets::ObservationPropertiesWidget::~ObservationPropertiesWidget()
{
}

Ui::ObservationPropertiesWidgetForm* te::qt::widgets::ObservationPropertiesWidget::getForm()
{
  return m_ui.get();
}

std::vector<int> te::qt::widgets::ObservationPropertiesWidget::getOutputValues()
{
  std::vector<int> indexes;
  std::vector<std::string> values = m_obsWidget->getOutputValues();

  for(size_t i = 0; i < values.size(); ++i)
    indexes.push_back(te::da::GetPropertyPos(m_dataType.get(), values[i]));

  return indexes;
}

std::vector<std::string> te::qt::widgets::ObservationPropertiesWidget::getOutputPropNames()
{
  return m_obsWidget->getOutputValues();
}

std::string te::qt::widgets::ObservationPropertiesWidget::getGeometryPropName()
{
  return m_ui->m_geometryComboBox->currentText().toStdString();
}

int te::qt::widgets::ObservationPropertiesWidget::getGeometryId()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_geometryComboBox->currentText().toStdString());
  else
    return -1;
}

std::string te::qt::widgets::ObservationPropertiesWidget::getIdPropName()
{
  return m_ui->m_idComboBox->currentText().toStdString();
}

int te::qt::widgets::ObservationPropertiesWidget::getIdIndex()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_idComboBox->currentText().toStdString());
  else
    return -1;
}

void te::qt::widgets::ObservationPropertiesWidget::setUp (const te::da::DataSetTypePtr dataType)
{
  QString item;
  std::vector<std::string> propertyNames;
  m_dataType = dataType;

  const std::vector<te::dt::Property*>& properties = dataType->getProperties();

  for (std::size_t i = 0; i < properties.size(); i++)
  {
    item = QString::fromStdString(properties.at(i)->getName());
    propertyNames.push_back(item.toStdString());

    if(properties.at(i)->getType() == te::dt::GEOMETRY_TYPE)
      m_ui->m_geometryComboBox->addItem(item);
    else
      m_ui->m_idComboBox->addItem(item);
  }

  m_obsWidget->setInputValues(propertyNames);
}

