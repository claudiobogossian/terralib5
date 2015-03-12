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

int te::qt::widgets::ObservationPropertiesWidget::getGeometryId()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_geometryComboBox->currentText().toStdString());
  else
    return -1;
}

int te::qt::widgets::ObservationPropertiesWidget::getTempPropName()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_nameComboBox->currentText().toStdString());
  else
    return -1;
}

int te::qt::widgets::ObservationPropertiesWidget::getResultTime()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_resultComboBox->currentText().toStdString());
  else
    return -1;
}

int te::qt::widgets::ObservationPropertiesWidget::getValidTime()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_validComboBox->currentText().toStdString());
  else
    return -1;
}

void te::qt::widgets::ObservationPropertiesWidget::setUp (const te::da::DataSetTypePtr dataType)
{
  QString item;
  std::vector<std::string> propertiyNames;
  m_dataType = dataType;

  const std::vector<te::dt::Property*>& properties = dataType->getProperties();

  for (std::size_t i = 0; i < properties.size(); i++)
  {
    item = item = QString::fromStdString(properties.at(i)->getName());
    propertiyNames.push_back(item.toStdString());

    if(properties.at(i)->getType() == te::dt::GEOMETRY_TYPE)
    {
      m_ui->m_geometryComboBox->addItem(item);
    }
    else if(properties.at(i)->getType() == te::dt::DATETIME_TYPE)
    {
      m_ui->m_nameComboBox->addItem(item);
      m_ui->m_resultComboBox->addItem(item);
      m_ui->m_validComboBox->addItem(item);
    }
  }

  m_obsWidget->setInputValues(propertiyNames);
}

