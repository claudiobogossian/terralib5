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

  \brief A widget used to adjust a temporal layer's properties
*/

//Terralib
#include "../../../dataaccess.h"
#include "../../../datatype/Property.h"
#include "TemporalPropertiesWidget.h"
#include "ui_TemporalPropertiesWidgetForm.h"

//QT
#include <QWidget>

te::qt::widgets::TemporalPropertiesWidget::TemporalPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
: QWidget(parent, f),
  m_ui(new Ui::TemporalPropertiesWidgetForm)
{
  m_ui->setupUi(this);

// connect signal and slots
  connect(m_ui->m_phenomenomComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPropertyComboBoxIndexChanged(QString)));
}

te::qt::widgets::TemporalPropertiesWidget::~TemporalPropertiesWidget()
{
}

Ui::TemporalPropertiesWidgetForm* te::qt::widgets::TemporalPropertiesWidget::getForm()
{
  return m_ui.get();
}

int te::qt::widgets::TemporalPropertiesWidget::getPhenomenonTime()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_phenomenomComboBox->currentText().toStdString());
  else
    return -1;
}

int te::qt::widgets::TemporalPropertiesWidget::getResultTime()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_resultComboBox->currentText().toStdString());
  else
    return -1;
}

int te::qt::widgets::TemporalPropertiesWidget::getValidTime()
{
  if(m_dataType)
    return m_dataType->getPropertyPosition(m_ui->m_validComboBox->currentText().toStdString());
  else
    return -1;
}

void te::qt::widgets::TemporalPropertiesWidget::setUp (const te::da::DataSetTypePtr dataType)
{
  QString item;
  m_dataType = dataType;

  const std::vector<te::dt::Property*>& properties = dataType.get()->getProperties();

  for (std::size_t i = 0; i < properties.size(); i++)
  {
    if(properties.at(i)->getType() == te::dt::DATETIME_TYPE)
    {
      item = QString::fromStdString(properties.at(i)->getName());
      m_ui->m_phenomenomComboBox->addItem(item);
      m_ui->m_resultComboBox->addItem(item);
      m_ui->m_validComboBox->addItem(item);
    }
  }
}

void te::qt::widgets::TemporalPropertiesWidget::onPropertyComboBoxIndexChanged (QString text)
{

}
