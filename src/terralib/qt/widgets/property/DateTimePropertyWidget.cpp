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
  \file terralib/qt/widgets/property/DateTimePropertyWidget.h

  \brief Defines the DateTimePropertyWidget class.
*/

// Terralib
#include "../../../datatype/DateTimeProperty.h"
#include "../../../datatype/Enums.h"
#include "DateTimePropertyWidget.h"
#include "ui_DateTimePropertyWidgetForm.h"


te::qt::widgets::DateTimePropertyWidget::DateTimePropertyWidget(QWidget* parent)
  : SimplePropertyWidget(parent),
    m_ui(new Ui::DateTimePropertyWidgetForm)
{
  m_ui->setupUi(this);

  //fill combo box 
  m_ui->m_typeComboBox->addItem(tr("Date"), te::dt::DATE);
  m_ui->m_typeComboBox->addItem(tr("Date Period"), te::dt::DATE_PERIOD);
  m_ui->m_typeComboBox->addItem(tr("Date Durantion"), te::dt::DATE_DURATION);
  m_ui->m_typeComboBox->addItem(tr("Time Duration"), te::dt::TIME_DURATION);
  m_ui->m_typeComboBox->addItem(tr("Time Instante"), te::dt::TIME_INSTANT);
  m_ui->m_typeComboBox->addItem(tr("Time Period"), te::dt::TIME_PERIOD);
  m_ui->m_typeComboBox->addItem(tr("Time Instante wih Zone"), te::dt::TIME_INSTANT_TZ);
  m_ui->m_typeComboBox->addItem(tr("Time Period with Zone"), te::dt::TIME_PERIOD_TZ);
}

te::qt::widgets::DateTimePropertyWidget::~DateTimePropertyWidget()
{

}

void te::qt::widgets::DateTimePropertyWidget::fillProperty(te::dt::SimpleProperty* sp)
{
  int index = m_ui->m_typeComboBox->currentIndex();
  int dateTimeType = m_ui->m_typeComboBox->itemData(index).toInt();

  te::dt::DateTimeProperty* dtp = dynamic_cast<te::dt::DateTimeProperty*>(sp);

  if(dtp)
  {
    dtp->setSubtype((te::dt::DateTimeType)dateTimeType);
  }
}
