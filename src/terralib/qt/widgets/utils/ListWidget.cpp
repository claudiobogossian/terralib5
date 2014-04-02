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

/*!
  \file terralib/qt/widgets/utils/DoubleListWidget.cpp

  \brief ...
*/

// TerraLib
#include "../../../common/Translator.h"
#include "ui_ListWidgetForm.h"
#include "ListWidget.h"

// Qt
#include <QIcon>
#include <QInputDialog>


te::qt::widgets::ListWidget::ListWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ListWidgetForm)
{
  m_ui->setupUi(this);

// set icons
  m_ui->m_addToolButton->setIcon(QIcon::fromTheme(""));
  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme(""));
  m_ui->m_editToolButton->setIcon(QIcon::fromTheme(""));
  m_ui->m_upToolButton->setIcon(QIcon::fromTheme(""));
  m_ui->m_downToolButton->setIcon(QIcon::fromTheme(""));

// set selection mode
  m_ui->m_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

// connect signals and slots
  connect(m_ui->m_addToolButton, SIGNAL(pressed()), this, SLOT(onAddToolButtonPressed()));
  connect(m_ui->m_removeToolButton, SIGNAL(pressed()), this, SLOT(onRemoveToolButtonPressed()));
  connect(m_ui->m_editToolButton, SIGNAL(pressed()), this, SLOT(onEditToolButtonPressed()));
  connect(m_ui->m_upToolButton, SIGNAL(pressed()), this, SLOT(onUpToolButtonPressed()));
  connect(m_ui->m_downToolButton, SIGNAL(pressed()), this, SLOT(onDownToolButtonPressed()));
  connect(m_ui->m_listWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(onListPressed(QListWidgetItem*)));
  
}

te::qt::widgets::ListWidget::~ListWidget()
{
}

Ui::ListWidgetForm* te::qt::widgets::ListWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::ListWidget::setLabel(std::string value)
{
  m_ui->m_label->setText(value.c_str());
}

std::vector<std::string> te::qt::widgets::ListWidget::getValues()
{
  std::vector<std::string> vec;

  int count = m_ui->m_listWidget->count();

  for(int i = 0; i < count; ++i)
  {
    vec.push_back(m_ui->m_listWidget->item(i)->text().toLatin1().data());
  }

  return vec;
}

void te::qt::widgets::ListWidget::onAddToolButtonPressed()
{
  bool ok;

  QString text = QInputDialog::getText(this, TE_TR("Add Value"), TE_TR("Value:"), QLineEdit::Normal, "", &ok);

  if(ok && !text.isEmpty())
  {
    m_ui->m_listWidget->addItem(text);
  }
}

void te::qt::widgets::ListWidget::onRemoveToolButtonPressed()
{
  if(m_ui->m_listWidget->currentItem())
  {
    int row = m_ui->m_listWidget->row(m_ui->m_listWidget->currentItem());

    QListWidgetItem* item = m_ui->m_listWidget->takeItem(row);

    delete item;
  }
}

void te::qt::widgets::ListWidget::onEditToolButtonPressed()
{
  if(m_ui->m_listWidget->currentItem())
  {
    bool ok;

    QString text = QInputDialog::getText(this, TE_TR("Add Value"), TE_TR("Value:"), QLineEdit::Normal, m_ui->m_listWidget->currentItem()->text(), &ok);

    if(ok && !text.isEmpty())
    {
      m_ui->m_listWidget->currentItem()->setText(text);
    }
  }
}

void te::qt::widgets::ListWidget::onUpToolButtonPressed()
{
  if(m_ui->m_listWidget->currentItem())
  {
    int row = m_ui->m_listWidget->row(m_ui->m_listWidget->currentItem());

    if(row != 0)
    {
      QListWidgetItem* item = m_ui->m_listWidget->takeItem(row);

      m_ui->m_listWidget->insertItem(row - 1, item);

      m_ui->m_listWidget->setCurrentItem(item);
    }
  }
}

void te::qt::widgets::ListWidget::onDownToolButtonPressed()
{
  if(m_ui->m_listWidget->currentItem())
  {
    int row = m_ui->m_listWidget->row(m_ui->m_listWidget->currentItem());

    int count = m_ui->m_listWidget->count();

    if(row != count - 1)
    {
      QListWidgetItem* item = m_ui->m_listWidget->takeItem(row);

      m_ui->m_listWidget->insertItem(row + 1, item);

      m_ui->m_listWidget->setCurrentItem(item);
    }
  }
}
void te::qt::widgets::ListWidget::onListPressed(QListWidgetItem* item)
{
}
