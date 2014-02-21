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

#include "RenameColumnDialog.h"

#include <ui_RenameColumnDialogForm.h>

// Qt
#include <QtGui/QDialogButtonBox>
#include <QtGui/QPushButton>

te::qt::widgets::RenameColumnDialog::RenameColumnDialog(QWidget* parent) :
QDialog(parent)
{
  m_ui.reset(new Ui::RenameColumnDialogForm);
  m_ui->setupUi(this);

  connect(m_ui->m_buttonBox->button(QDialogButtonBox::Ok), SIGNAL(pressed()), SLOT(accept()));
  connect(m_ui->m_buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(pressed()), SLOT(reject()));
}

te::qt::widgets::RenameColumnDialog::~RenameColumnDialog()
{
}

QString te::qt::widgets::RenameColumnDialog::getOldName()
{
  return m_ui->m_oldNameLineEdit->text();
}

QString te::qt::widgets::RenameColumnDialog::getNewName()
{
  return m_ui->m_newNameLineEdit->text();
}

void te::qt::widgets::RenameColumnDialog::setOldColumnName(const QString& name)
{
  return m_ui->m_oldNameLineEdit->setText(name);
}
