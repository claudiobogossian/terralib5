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

#include "AlterDataDialog.h"

#include <ui_AlterDataDialogForm.h>

te::qt::widgets::AlterDataDialog::AlterDataDialog(QWidget* parent) :
QDialog(parent)
{
  m_ui.reset(new Ui::AlterDataDialogForm);
  m_ui->setupUi(this);

  m_ui->m_helpLabel->setVisible(false);

  connect (m_ui->m_closePushButton, SIGNAL(pressed()), SLOT(reject()));
  connect (m_ui->m_executePushButton, SIGNAL(pressed()), SLOT(accept()));

  connect (m_ui->m_divPushButton, SIGNAL(pressed()), SLOT(divPressed()));
  connect (m_ui->m_plusPushButton, SIGNAL(pressed()), SLOT(plusPressed()));
  connect (m_ui->m_minusPushButton, SIGNAL(pressed()), SLOT(minusPressed()));
  connect (m_ui->m_multiplyPushButton, SIGNAL(pressed()), SLOT(multiplyPressed()));

  connect (m_ui->m_columnsComboBox, SIGNAL(currentIndexChanged(const QString&)), SLOT(columnChanged(const QString&)));
}

te::qt::widgets::AlterDataDialog::~AlterDataDialog()
{
}

void te::qt::widgets::AlterDataDialog::setSelectedColumn(const QString& colName)
{
  m_ui->m_selectedColumnLineEdit->setText(colName);
}

void te::qt::widgets::AlterDataDialog::setDataColumns(const std::vector<QString>& cols)
{
  std::vector<QString>::const_iterator it;

  m_ui->m_columnsComboBox->clear();

  m_ui->m_columnsComboBox->blockSignals(true);

  for(it=cols.begin(); it!=cols.end(); ++it)
    m_ui->m_columnsComboBox->addItem(*it);

  m_ui->m_columnsComboBox->blockSignals(false);
}

bool te::qt::widgets::AlterDataDialog::alterAllData()
{
  return m_ui->m_allRadioButton->isChecked();
}

QString te::qt::widgets::AlterDataDialog::getExpression()
{
  return m_ui->m_expressionTextEdit->toPlainText();
}

void te::qt::widgets::AlterDataDialog::plusPressed()
{
  m_ui->m_expressionTextEdit->insertPlainText("+ ");
}

void te::qt::widgets::AlterDataDialog::minusPressed()
{
  m_ui->m_expressionTextEdit->insertPlainText("- ");
}

void te::qt::widgets::AlterDataDialog::multiplyPressed()
{
  m_ui->m_expressionTextEdit->insertPlainText("* ");
}

void te::qt::widgets::AlterDataDialog::divPressed()
{
  m_ui->m_expressionTextEdit->insertPlainText("/ ");
}

void te::qt::widgets::AlterDataDialog::columnChanged(const QString& col)
{
  m_ui->m_expressionTextEdit->insertPlainText(col + " ");
}

void te::qt::widgets::AlterDataDialog::setHelpLabelText(const QString& text)
{
  m_ui->m_helpLabel->setVisible(true);
  m_ui->m_helpLabel->setText(text);
}