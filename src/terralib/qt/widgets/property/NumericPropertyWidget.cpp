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
  \file terralib/qt/widgets/property/NumericPropertyWidget.h

  \brief Defines the NumericPropertyWidget class.
*/

// Terralib
#include "../../../datatype/NumericProperty.h"
#include "NumericPropertyWidget.h"
#include "ui_NumericPropertyWidgetForm.h"


te::qt::widgets::NumericPropertyWidget::NumericPropertyWidget(QWidget* parent)
  : SimplePropertyWidget(parent),
    m_ui(new Ui::NumericPropertyWidgetForm)
{
  m_ui->setupUi(this);

}

te::qt::widgets::NumericPropertyWidget::~NumericPropertyWidget()
{

}

void te::qt::widgets::NumericPropertyWidget::fillProperty(te::dt::SimpleProperty* sp)
{
  te::dt::NumericProperty* np = dynamic_cast<te::dt::NumericProperty*>(sp);

  if(np)
  {
    if(m_ui->m_scaleLineEdit->text().isEmpty() == false)
    {
      bool ok = false;
      unsigned int scale = m_ui->m_scaleLineEdit->text().toUInt(&ok);
      
      if(ok)
        np->setScale(scale);
    }
    
    if(m_ui->m_precisionLineEdit->text().isEmpty() == false)
    {
      bool ok = false;
      unsigned int precision = m_ui->m_precisionLineEdit->text().toUInt(&ok);
      
      if(ok)
        np->setPrecision(precision);
    }
  }
}
