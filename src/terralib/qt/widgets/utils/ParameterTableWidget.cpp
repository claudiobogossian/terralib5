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
  \file terralib/qt/widgets/utils/ParameterTableWidget.cpp

  \brief ...
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../Exception.h"
#include "ParameterDialog.h"
#include "ParameterTableWidget.h"
#include "ui_ParameterDialogForm.h"
#include "ui_ParameterTableWidgetForm.h"

// Qt
#include <QIcon>

te::qt::widgets::ParameterTableWidget::ParameterTableWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ParameterTableWidgetForm)
{
  m_ui->setupUi(this);

  m_ui->m_addItemToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeItemToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_editItemToolButton->setIcon(QIcon::fromTheme("preferences-system"));

  connect(m_ui->m_addItemToolButton, SIGNAL(pressed()), this, SLOT(addToolButtonPressed()));
  connect(m_ui->m_removeItemToolButton, SIGNAL(pressed()), this, SLOT(removeToolButtonPressed()));
  connect(m_ui->m_editItemToolButton, SIGNAL(pressed()), this, SLOT(editToolButtonPressed()));
}

te::qt::widgets::ParameterTableWidget::~ParameterTableWidget()
{
}

Ui::ParameterTableWidgetForm* te::qt::widgets::ParameterTableWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::ParameterTableWidget::add(const std::string& name, const std::string& value)
{
  int newrow = m_ui->m_paramTableWidget->rowCount();

  //check if already exist a key with this name
  for(int i = 0; i < newrow; ++i)
  {
    QTableWidgetItem* itemName = m_ui->m_paramTableWidget->item(i, 0);

    if(itemName->text().toStdString() == name)
    {
      QTableWidgetItem* itemValue = new QTableWidgetItem(QString::fromStdString(value));
      m_ui->m_paramTableWidget->setItem(i, 1, itemValue);

      return;
    }
  }

  //new entry
  m_ui->m_paramTableWidget->insertRow(newrow);

  QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(name));
  m_ui->m_paramTableWidget->setItem(newrow, 0, itemName);

  QTableWidgetItem* itemValue = new QTableWidgetItem(QString::fromStdString(value));
  m_ui->m_paramTableWidget->setItem(newrow, 1, itemValue);

  m_ui->m_paramTableWidget->resizeRowsToContents();
  m_ui->m_paramTableWidget->resizeColumnToContents(0);
}

std::map<std::string, std::string> te::qt::widgets::ParameterTableWidget::getMap() const
{
  std::map<std::string, std::string> kvpairs;

  if(m_ui->m_paramTableWidget->columnCount() < 2)
    throw Exception(TE_TR("Can not get key-value pairs from a table with less than 2 columns!"));

  int nrows = m_ui->m_paramTableWidget->rowCount();

  for(int i = 0; i < nrows; ++i)
  {
    QTableWidgetItem* itemName = m_ui->m_paramTableWidget->item(i, 0);

    QTableWidgetItem* itemValue = m_ui->m_paramTableWidget->item(i, 1);

    kvpairs[itemName->text().toStdString()] = itemValue->text().toStdString();
  }

  return kvpairs;
}

void te::qt::widgets::ParameterTableWidget::addToolButtonPressed()
{
  std::auto_ptr<ParameterDialog> pdialog(new ParameterDialog(this));

  pdialog->setWindowTitle(tr("Add new parameter"));

  int retval = pdialog->exec();

  if(retval == QDialog::Rejected)
    return;

  int newrow = m_ui->m_paramTableWidget->rowCount();

  m_ui->m_paramTableWidget->insertRow(newrow);

  QTableWidgetItem* itemName = new QTableWidgetItem(pdialog->getName());
  m_ui->m_paramTableWidget->setItem(newrow, 0, itemName);

  QTableWidgetItem* itemValue = new QTableWidgetItem(pdialog->getValue());
  m_ui->m_paramTableWidget->setItem(newrow, 1, itemValue);

  m_ui->m_paramTableWidget->resizeRowsToContents();
  m_ui->m_paramTableWidget->resizeColumnToContents(0);
}

void te::qt::widgets::ParameterTableWidget::removeToolButtonPressed()
{
  int row = m_ui->m_paramTableWidget->currentRow();

  if(row >= 0)
    m_ui->m_paramTableWidget->removeRow(row);

  m_ui->m_paramTableWidget->resizeRowsToContents();
  m_ui->m_paramTableWidget->resizeColumnToContents(0);
}

void te::qt::widgets::ParameterTableWidget::editToolButtonPressed()
{
  int row = m_ui->m_paramTableWidget->currentRow();

  if(row < 0)
    return;

  std::auto_ptr<ParameterDialog> pdialog(new ParameterDialog(this));

  pdialog->setWindowTitle(tr("Edit parameter"));

  QTableWidgetItem* itemName = m_ui->m_paramTableWidget->item(row, 0);

  if(itemName != 0)
    pdialog->setName(itemName->text());

  QTableWidgetItem* itemValue = m_ui->m_paramTableWidget->item(row, 1);

  if(itemValue != 0)
    pdialog->setValue(itemValue->text());

  int retval = pdialog->exec();

  if(retval == QDialog::Rejected)
    return;

  itemName->setText(pdialog->getName());
  itemValue->setText(pdialog->getValue());

  m_ui->m_paramTableWidget->update();

  m_ui->m_paramTableWidget->resizeRowsToContents();
  m_ui->m_paramTableWidget->resizeColumnToContents(0);
}

