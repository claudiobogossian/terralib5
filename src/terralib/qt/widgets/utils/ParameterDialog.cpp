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
  \file terralib/qt/widgets/utils/ParameterDialog.cpp

  \brief A dialog to be used as input of key/value pairs.
*/

// TerraLib
#include "ui_ParameterDialogForm.h"
#include "ParameterDialog.h"


te::qt::widgets::ParameterDialog::ParameterDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ParameterDialogForm)
{
  m_ui->setupUi(this);

  connect(m_ui->m_paramNameLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onParamNameTextEdited(const QString&)));
  connect(m_ui->m_paramValueLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onParamValueTextEdited(const QString&)));
}

te::qt::widgets::ParameterDialog::~ParameterDialog()
{
}

QString te::qt::widgets::ParameterDialog::getName() const
{
  return m_ui->m_paramNameLineEdit->text().trimmed();
}

void te::qt::widgets::ParameterDialog::setName(const QString& name)
{
  m_pname = name;
  m_ui->m_paramNameLineEdit->setText(name);
}

QString te::qt::widgets::ParameterDialog::getValue() const
{
  return m_ui->m_paramValueLineEdit->text().trimmed();
}

void te::qt::widgets::ParameterDialog::setValue(const QString& value)
{
  m_pvalue = value;
  m_ui->m_paramValueLineEdit->setText(value);
}

void te::qt::widgets::ParameterDialog::onParamNameTextEdited(const QString& nextTxt)
{
  if(nextTxt != m_pname)
    m_ui->m_okPushButton->setEnabled(true);
  else if(m_pvalue == m_ui->m_paramValueLineEdit->text())
    m_ui->m_okPushButton->setEnabled(false);
}

void te::qt::widgets::ParameterDialog::onParamValueTextEdited(const QString& nextTxt)
{
  if(nextTxt != m_pvalue)
    m_ui->m_okPushButton->setEnabled(true);
  else if(m_pname == m_ui->m_paramNameLineEdit->text())
    m_ui->m_okPushButton->setEnabled(false);
}

