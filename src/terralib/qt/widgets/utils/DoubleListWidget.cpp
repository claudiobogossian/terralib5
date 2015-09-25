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
  \file terralib/qt/widgets/utils/DoubleListWidget.cpp

  \brief ...
*/

// TerraLib
#include "ui_DoubleListWidgetForm.h"
#include "DoubleListWidget.h"

// Qt
#include <QIcon>


te::qt::widgets::DoubleListWidget::DoubleListWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::DoubleListWidgetForm)
{
  m_ui->setupUi(this);

// set icons
  m_ui->m_addToolButton->setIcon(QIcon::fromTheme("go-next"));
  m_ui->m_addAllToolButton->setIcon(QIcon::fromTheme("go-last"));
  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("go-previous"));
  m_ui->m_removeAllToolButton->setIcon(QIcon::fromTheme("go-first"));
  m_ui->m_upToolButton->setIcon(QIcon::fromTheme("go-up"));
  m_ui->m_downToolButton->setIcon(QIcon::fromTheme("go-down"));

// set selection mode
  m_ui->m_leftListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
  m_ui->m_rightListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

// connect signals and slots
  connect(m_ui->m_addToolButton, SIGNAL(pressed()), this, SLOT(onAddToolButtonPressed()));
  connect(m_ui->m_addAllToolButton, SIGNAL(pressed()), this, SLOT(onAddAllToolButtonPressed()));
  connect(m_ui->m_removeToolButton, SIGNAL(pressed()), this, SLOT(onRemoveToolButtonPressed()));
  connect(m_ui->m_removeAllToolButton, SIGNAL(pressed()), this, SLOT(onRemoveAllToolButtonPressed()));
  connect(m_ui->m_leftListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onLeftListItemSelectionChanged()));
  connect(m_ui->m_rightListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onRightListItemSelectionChanged()));
  connect(m_ui->m_upToolButton, SIGNAL(pressed()), this, SLOT(onUpToolButtonPressed()));
  connect(m_ui->m_downToolButton, SIGNAL(pressed()), this, SLOT(onDownToolButtonPressed()));
}

te::qt::widgets::DoubleListWidget::~DoubleListWidget()
{
}

Ui::DoubleListWidgetForm* te::qt::widgets::DoubleListWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::DoubleListWidget::setInputValues(std::vector<std::string> values)
{
  m_ui->m_leftListWidget->clear();

  if(values.empty() == false)
    m_ui->m_addAllToolButton->setEnabled(true);
  else
    m_ui->m_addAllToolButton->setEnabled(false);

  for(size_t i = 0; i < values.size(); ++i)
  {
    m_ui->m_leftListWidget->addItem(values[i].c_str());
  }
}

void te::qt::widgets::DoubleListWidget::setInputDataValues(std::vector<std::string> values, std::vector<int> ids)
{
  m_ui->m_leftListWidget->clear();

  if (values.size() != ids.size())
    return;

  if (values.empty() == false)
    m_ui->m_addAllToolButton->setEnabled(true);
  else
    m_ui->m_addAllToolButton->setEnabled(false);

  for (size_t i = 0; i < values.size(); ++i)
  {
    QListWidgetItem* item = new QListWidgetItem(m_ui->m_leftListWidget);
    item->setText(values[i].c_str());
    item->setData(Qt::UserRole, QVariant(ids[i]));

    m_ui->m_leftListWidget->addItem(item);
  }
}

void te::qt::widgets::DoubleListWidget::setOutputValues(std::vector<std::string> values)
{
  m_ui->m_rightListWidget->clear();

  if(values.empty() == false)
    m_ui->m_removeAllToolButton->setEnabled(true);
  else
    m_ui->m_removeAllToolButton->setEnabled(false);

  for(size_t i = 0; i < values.size(); ++i)
  {
    m_ui->m_rightListWidget->addItem(values[i].c_str());
  }
}

void te::qt::widgets::DoubleListWidget::setOutputDataValues(std::vector<std::string> values, std::vector<int> ids)
{
  m_ui->m_rightListWidget->clear();

  if (values.size() != ids.size())
    return;

  if (values.empty() == false)
    m_ui->m_removeAllToolButton->setEnabled(true);
  else
    m_ui->m_removeAllToolButton->setEnabled(false);

  for (size_t i = 0; i < values.size(); ++i)
  {
    QListWidgetItem* item = new QListWidgetItem(m_ui->m_rightListWidget);
    item->setText(values[i].c_str());
    item->setData(Qt::UserRole, QVariant(ids[i]));

    m_ui->m_rightListWidget->addItem(item);
  }
}

void te::qt::widgets::DoubleListWidget::setFixedOutputValues(std::vector<std::string> values, std::string iconName)
{
  for(size_t i = 0; i < values.size(); ++i)
  {
    QListWidgetItem* item = new QListWidgetItem(m_ui->m_rightListWidget);
    item->setText(values[i].c_str());
    item->setFlags(Qt::ItemIsEnabled);
    item->setIcon(QIcon::fromTheme(iconName.c_str()));
  }
}

void te::qt::widgets::DoubleListWidget::setLeftLabel(std::string value)
{
  m_ui->m_leftItemsLabel->setText(value.c_str());
}

void te::qt::widgets::DoubleListWidget::setRightLabel(std::string value)
{
  m_ui->m_rightItemsLabel->setText(value.c_str());
}

std::vector<std::string> te::qt::widgets::DoubleListWidget::getOutputValues()
{
  std::vector<std::string> vec;

  int count = m_ui->m_rightListWidget->count();

  for(int i = 0; i < count; ++i)
  {
    vec.push_back(m_ui->m_rightListWidget->item(i)->text().toLatin1().data());
  }

  return vec;
}

std::vector<int> te::qt::widgets::DoubleListWidget::getOutputDataValues()
{
  std::vector<int> vec;

  int count = m_ui->m_rightListWidget->count();

  for (int i = 0; i < count; ++i)
  {
    QVariant v = m_ui->m_rightListWidget->item(i)->data(Qt::UserRole);

    if (v.isValid())
      vec.push_back(v.toInt());
  }

  return vec;
}

void te::qt::widgets::DoubleListWidget::clearInputValues()
{
  m_ui->m_leftListWidget->clear();
}

void te::qt::widgets::DoubleListWidget::clearOutputValues()
{
  m_ui->m_rightListWidget->clear();
}

void te::qt::widgets::DoubleListWidget::onAddToolButtonPressed()
{
  QList<QListWidgetItem*> list = m_ui->m_leftListWidget->selectedItems();

  if(list.empty() == false)
    m_ui->m_removeAllToolButton->setEnabled(true);

  for(int i = 0; i < list.size(); ++i)
  {
    int row = m_ui->m_leftListWidget->row(list[i]);

    QListWidgetItem* item = m_ui->m_leftListWidget->takeItem(row);

    m_ui->m_rightListWidget->addItem(item);
  }

  if(m_ui->m_leftListWidget->count() == 0)
  {
    m_ui->m_addToolButton->setEnabled(false);
    m_ui->m_addAllToolButton->setEnabled(false);
  }

  emit itemChanged();
}

void te::qt::widgets::DoubleListWidget::onAddAllToolButtonPressed()
{
  m_ui->m_leftListWidget->selectAll();

  QList<QListWidgetItem*> list = m_ui->m_leftListWidget->selectedItems();

  if(list.empty() == false)
    m_ui->m_removeAllToolButton->setEnabled(true);

  for(int i = 0; i < list.size(); ++i)
  {
    int row = m_ui->m_leftListWidget->row(list[i]);

    QListWidgetItem* item = m_ui->m_leftListWidget->takeItem(row);

    m_ui->m_rightListWidget->addItem(item);
  }

  m_ui->m_addToolButton->setEnabled(false);
  m_ui->m_addAllToolButton->setEnabled(false);

  emit itemChanged();
}

void te::qt::widgets::DoubleListWidget::onRemoveToolButtonPressed()
{
  QList<QListWidgetItem*> list = m_ui->m_rightListWidget->selectedItems();

  if(list.empty() == false)
    m_ui->m_addAllToolButton->setEnabled(true);

  for(int i = 0; i < list.size(); ++i)
  {
    int row = m_ui->m_rightListWidget->row(list[i]);

    QListWidgetItem* item = m_ui->m_rightListWidget->takeItem(row);

    m_ui->m_leftListWidget->addItem(item);
  }

  if(m_ui->m_rightListWidget->count() == 0)
  {
    m_ui->m_removeToolButton->setEnabled(false);
    m_ui->m_removeAllToolButton->setEnabled(false);
    m_ui->m_upToolButton->setEnabled(false);
    m_ui->m_downToolButton->setEnabled(false);
  }

  emit itemChanged();
}

void te::qt::widgets::DoubleListWidget::onRemoveAllToolButtonPressed()
{
  m_ui->m_rightListWidget->selectAll();

  QList<QListWidgetItem*> list = m_ui->m_rightListWidget->selectedItems();

  if(list.empty() == false)
    m_ui->m_addAllToolButton->setEnabled(true);

  for(int i = 0; i < list.size(); ++i)
  {
    int row = m_ui->m_rightListWidget->row(list[i]);

    QListWidgetItem* item = m_ui->m_rightListWidget->takeItem(row);

    m_ui->m_leftListWidget->addItem(item);
  }

  m_ui->m_removeToolButton->setEnabled(false);
  m_ui->m_removeAllToolButton->setEnabled(false);
  m_ui->m_upToolButton->setEnabled(false);
  m_ui->m_downToolButton->setEnabled(false);

  emit itemChanged();
}

void te::qt::widgets::DoubleListWidget::onDownToolButtonPressed()
{
  int count = m_ui->m_rightListWidget->count();

  if(count == m_ui->m_rightListWidget->selectedItems().size())
    return;

  for(int i = count-1; i >= 0; --i)
  {
    QListWidgetItem* currentItem = m_ui->m_rightListWidget->item(i);

    if(i == count-1)
      continue;

    if(currentItem->isSelected())
    {
      QListWidgetItem* itemAfter = m_ui->m_rightListWidget->item(i+1);

      bool wasSelected = itemAfter->isSelected();

      m_ui->m_rightListWidget->takeItem(i);
      m_ui->m_rightListWidget->takeItem(i);

      m_ui->m_rightListWidget->insertItem(i, itemAfter);
      m_ui->m_rightListWidget->insertItem(i+1, currentItem);

      currentItem->setSelected(true);

      if(wasSelected)
        itemAfter->setSelected(true);
    }
  }
}

void te::qt::widgets::DoubleListWidget::onUpToolButtonPressed()
{
  int count = m_ui->m_rightListWidget->count();

  if(count == m_ui->m_rightListWidget->selectedItems().size())
    return;

  for(int i = 0; i < count; ++i)
  {
    QListWidgetItem* currentItem = m_ui->m_rightListWidget->item(i);

    if(i == 0)
      continue;

    if(currentItem->isSelected())
    {
      QListWidgetItem* itemBefore = m_ui->m_rightListWidget->item(i-1);

      bool wasSelected = itemBefore->isSelected();

      m_ui->m_rightListWidget->takeItem(i);
      m_ui->m_rightListWidget->takeItem(i-1);

      m_ui->m_rightListWidget->insertItem(i-1, currentItem);
      m_ui->m_rightListWidget->insertItem(i, itemBefore);

      currentItem->setSelected(true);

      if(wasSelected)
        itemBefore->setSelected(true);
    }
  }
}

void te::qt::widgets::DoubleListWidget::onLeftListItemSelectionChanged()
{
  if(m_ui->m_leftListWidget->selectedItems().size() > 0)
  {
    m_ui->m_addToolButton->setEnabled(true);
    m_ui->m_addAllToolButton->setEnabled(true);
  }
  else
  {
    m_ui->m_addToolButton->setEnabled(false);
  }
}

void te::qt::widgets::DoubleListWidget::onRightListItemSelectionChanged()
{
  if(m_ui->m_rightListWidget->selectedItems().size() > 0)
  {
    m_ui->m_removeToolButton->setEnabled(true);
    m_ui->m_removeAllToolButton->setEnabled(true);
    m_ui->m_upToolButton->setEnabled(true);
    m_ui->m_downToolButton->setEnabled(true);
  }
  else
  {
    m_ui->m_removeToolButton->setEnabled(false);
    m_ui->m_upToolButton->setEnabled(false);
    m_ui->m_downToolButton->setEnabled(false);
  }
}

