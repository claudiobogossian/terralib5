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

#include "RetypeColumnDialog.h"

#include <ui_RetypeColumnDialogForm.h>

// TerraLib
#include <terralib/datatype/Enums.h>
#include <terralib/datatype/DateTimeProperty.h>
#include <terralib/datatype/NumericProperty.h>
#include <terralib/datatype/Property.h>
#include <terralib/datatype/SimpleProperty.h>
#include <terralib/datatype/StringProperty.h>


int GetType(const int& te_type)
{
  switch (te_type)
  {
    case te::dt::CHAR_TYPE:
    case te::dt::UCHAR_TYPE:
    case te::dt::STRING_TYPE:

      return 2;
    break;

    case te::dt::INT16_TYPE:
    case te::dt::UINT16_TYPE:
    case te::dt::INT32_TYPE:
    case te::dt::UINT32_TYPE:
    case te::dt::INT64_TYPE:
    case te::dt::UINT64_TYPE:
      return 0;
    break;

    case te::dt::FLOAT_TYPE:
    case te::dt::DOUBLE_TYPE:
    case te::dt::NUMERIC_TYPE:
      return 1;
    break;

    case te::dt::DATETIME_TYPE:
      return 3;
    break;

    default:
      return 4;
  };

}

QString GetSType(const int& te_type)
{
  int type = GetType(te_type);

  switch(type) 
  {
    case 0:
      return "INT";
    break;

    case 1:
      return "REAL";
    break;

    case 2:
      return "STRING";
    break;

    case 3:
      return "DATETIME";
    break;

    default:
      return "UNKNOWN TYPE";
    break;
  };
}

int GetIType (const int& type)
{
  switch (type)
  {
    case 0:
      return te::dt::INT32_TYPE;
    break;

    case 1:
      return te::dt::DOUBLE_TYPE;
    break;

    case 2:
      return te::dt::STRING_TYPE;
    break;

    case 3:
      return te::dt::DATETIME_TYPE;
    break;
  };

  return -1;
}

te::qt::widgets::RetypeColumnDialog::RetypeColumnDialog(QWidget* parent) :
QDialog(parent)
{
  m_ui.reset(new Ui::RetypeColumnDialogForm);
  m_ui->setupUi(this);

  connect (m_ui->m_okPushButton, SIGNAL(pressed()), SLOT(accept()));
  connect (m_ui->m_cancelPushButton, SIGNAL(pressed()), SLOT(reject()));
}

te::qt::widgets::RetypeColumnDialog::~RetypeColumnDialog()
{
}

void te::qt::widgets::RetypeColumnDialog::setTableName(const QString& name)
{
  m_ui->m_tableNameLineEdit->setText(name);
}

void te::qt::widgets::RetypeColumnDialog::setType(const int& type)
{
  m_ui->m_typeLineEdit->setText(GetSType(type));
}

void te::qt::widgets::RetypeColumnDialog::setColumnName(const QString& name)
{
  m_ui->m_columnNameLineEdit->setText(name);
}

void te::qt::widgets::RetypeColumnDialog::setColummnSize(const int& size)
{
  m_ui->m_sizeLineEdit->setText(QString::number(size));
}

int te::qt::widgets::RetypeColumnDialog::getColumnType() const
{
  return GetIType(m_ui->m_typeComboBox->currentIndex());
}

int te::qt::widgets::RetypeColumnDialog::getColumnSize() const
{
  return m_ui->m_sizeComboBox->currentText().toInt();
}

std::auto_ptr<te::dt::Property> te::qt::widgets::RetypeColumnDialog::getProperty()
{
  std::auto_ptr<te::dt::Property> res;
  std::string name = m_ui->m_columnNameLineEdit->text().toStdString();

  int cType = getColumnType();

  switch (cType)
  {
    case te::dt::INT32_TYPE:
    case te::dt::DOUBLE_TYPE:
      res.reset(new te::dt::SimpleProperty(name, cType));
    break;

    case te::dt::STRING_TYPE:
      res.reset(new te::dt::StringProperty(name, te::dt::STRING));
    break;

    case te::dt::DATETIME_TYPE:
      res.reset(new te::dt::DateTimeProperty(name, te::dt::DATE_PERIOD));
    break;
  };

  return res;
}
