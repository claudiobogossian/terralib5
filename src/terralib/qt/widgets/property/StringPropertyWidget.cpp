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
  \file terralib/qt/widgets/property/StringPropertyWidget.h

  \brief Defines the StringPropertyWidget class.
*/

// Terralib
#include "../../../datatype/Enums.h"
#include "../../../datatype/StringProperty.h"
#include "StringPropertyWidget.h"
#include "ui_StringPropertyWidgetForm.h"


te::qt::widgets::StringPropertyWidget::StringPropertyWidget(QWidget* parent)
  : SimplePropertyWidget(parent),
    m_ui(new Ui::StringPropertyWidgetForm)
{
  m_ui->setupUi(this);

  //fill combo box 
  m_ui->m_typeComboBox->addItem(tr("String"), te::dt::STRING);
  m_ui->m_typeComboBox->addItem(tr("Fixed String"), te::dt::FIXED_STRING);
  m_ui->m_typeComboBox->addItem(tr("Var String"), te::dt::VAR_STRING);
}  

te::qt::widgets::StringPropertyWidget::~StringPropertyWidget()
{

}

void te::qt::widgets::StringPropertyWidget::fillProperty(te::dt::SimpleProperty* sp)
{
  int index = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(index).toInt();
  
  te::dt::StringProperty* strP = dynamic_cast<te::dt::StringProperty*>(sp);

  if(strP)
  {
    strP->setSubtype((te::dt::StringType)type);

    if(m_ui->m_sizeLineEdit->text().isEmpty() == false)
    {
      bool ok = false;
      unsigned int size = m_ui->m_sizeLineEdit->text().toUInt(&ok);
      
      if(ok)
        strP->setSize(size);
    }
  }
}
