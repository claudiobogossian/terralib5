/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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

#include "AddColumnDialog.h"

#include "../../../datatype/DateTimeProperty.h"
#include "../../../datatype/SimpleProperty.h"

#include <ui_AddColumnDialogForm.h>

te::dt::Property* GetProperty(const QString& propertyType)
{
  if(propertyType.compare(QObject::tr("Integer")) == 0)
    return new te::dt::SimpleProperty("", te::dt::INT32_TYPE);
  else if(propertyType.compare(QObject::tr("Real")) == 0)
    return new te::dt::SimpleProperty("", te::dt::DOUBLE_TYPE);
  else if(propertyType.compare(QObject::tr("String")) == 0)
    return new te::dt::SimpleProperty("", te::dt::STRING_TYPE);
  else if(propertyType.compare(QObject::tr("Date")) == 0)
    return new te::dt::DateTimeProperty("");

  return 0;
}

te::qt::widgets::AddColumnDialog::AddColumnDialog(QWidget* parent) :
QDialog(parent)
{
  m_ui.reset(new Ui::AddColumnDialogForm);
  m_ui->setupUi(this);

  m_ui->m_helpBTN->setPageReference("widgets/table/addColumn.html");

  connect(m_ui->m_cancelBTN, SIGNAL(pressed()), SLOT(reject()));
  connect(m_ui->m_okBTN, SIGNAL(pressed()), SLOT(accept()));
}

te::qt::widgets::AddColumnDialog::~AddColumnDialog()
{
}

te::dt::Property* te::qt::widgets::AddColumnDialog::getNewProperty() const
{
  te::dt::Property* pr = GetProperty(m_ui->m_colTypeCBB->currentText());

  pr->setName(m_ui->m_colNameLNE->text().toStdString());

  return pr;
}

void te::qt::widgets::AddColumnDialog::setTableName(const std::string& name)
{
  m_ui->m_tblNameCBB->addItem(name.c_str());
}
