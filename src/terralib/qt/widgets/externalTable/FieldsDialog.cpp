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
  \file terralib/qt/widgets/externaltable/FieldsDialog.cpp

  \brief A Qt Dilaog that allows users to modify which fields will be included in a query
*/

// TerraLib
#include "../../../dataaccess/query/Field.h"
#include "../../../dataaccess/query/PropertyName.h"
#include "../utils/DoubleListWidget.h"
#include "FieldsDialog.h"
#include "ui_FieldsDialogForm.h"

//QT
#include <QMessageBox>

// STL
#include <cassert>

te::qt::widgets::FieldsDialog::FieldsDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::FieldsDialogForm)
{
  m_ui->setupUi(this);

  //Adjusting the doubleListWidget that will be used to configure the query's fields.
  m_fieldsWidget.reset(new DoubleListWidget(m_ui->m_FieldsFrame));
  QGridLayout* fieldsLayout = new QGridLayout(m_ui->m_FieldsFrame);
  fieldsLayout->addWidget(m_fieldsWidget.get());
  fieldsLayout->setContentsMargins(0, 0, 0, 0);

  m_ui->m_helpPushButton->setPageReference("widgets/external_table/fields_dialog.html");

  //Connecting signals and slots
  connect(m_ui->m_OkPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_CancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::qt::widgets::FieldsDialog::~FieldsDialog()
{
}

void te::qt::widgets::FieldsDialog::clearInputValues()
{
  m_fieldsWidget->clearInputValues();
}

void te::qt::widgets::FieldsDialog::clearOutputValues()
{
  m_fieldsWidget->clearOutputValues();
}

void te::qt::widgets::FieldsDialog::setInputValues(std::vector<std::string> values)
{
  m_fieldsWidget->setInputValues(values);
}

void te::qt::widgets::FieldsDialog::setOutputValues(std::vector<std::string> values)
{
  m_fieldsWidget->setOutputValues(values);
}

void te::qt::widgets::FieldsDialog::setFixedOutputValues(std::vector<std::string> values, std::string iconName)
{
  m_fieldsWidget->setFixedOutputValues(values, iconName);
}

void te::qt::widgets::FieldsDialog::setLeftLabel(const QString& value)
{
  m_fieldsWidget->setLeftLabel(value);
}

void te::qt::widgets::FieldsDialog::setRightLabel(const QString& value)
{
  m_fieldsWidget->setRightLabel(value);
}

te::da::Fields* te::qt::widgets::FieldsDialog::getFields()
{
  te::da::Fields* fields = new te::da::Fields;

  std::vector<std::string> values = m_fieldsWidget->getOutputValues();

  for(size_t t = 0; t < values.size();  ++t)
  {
    te::da::Field* f = new te::da::Field(values[t]);

    fields->push_back(f);
  }

  return fields;
}

te::da::JoinType te::qt::widgets::FieldsDialog::getJoinType()
{
  if(m_ui->m_rightRadioButton->isChecked())
    return te::da::RIGHT_JOIN;
  else if(m_ui->m_joinRadioButton->isChecked())
    return te::da::JOIN;
  else
    return te::da::LEFT_JOIN;
}

void te::qt::widgets::FieldsDialog::onOkPushButtonClicked()
{
  this->accept();
}

void te::qt::widgets::FieldsDialog::onCancelPushButtonClicked()
{
  m_ui->m_leftRadioButton->setChecked(true);
  m_fieldsWidget->clearInputValues();
  m_fieldsWidget->clearOutputValues();
  this->reject();
}