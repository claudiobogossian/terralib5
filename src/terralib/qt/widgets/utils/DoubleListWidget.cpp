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
#include "ui_DoubleListWidgetForm.h"
#include "DoubleListWidget.h"

// Qt
#include <QtGui/QIcon>


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

// connect signals and slots
  connect(m_ui->m_addToolButton, SIGNAL(pressed()), this, SLOT(onAddToolButtonPressed()));
  connect(m_ui->m_addAllToolButton, SIGNAL(pressed()), this, SLOT(onAddAllToolButtonPressed()));
  connect(m_ui->m_removeToolButton, SIGNAL(pressed()), this, SLOT(onRemoveToolButtonPressed()));
  connect(m_ui->m_removeAllToolButton, SIGNAL(pressed()), this, SLOT(onRemoveAllToolButtonPressed()));
  connect(m_ui->m_leftListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(onLeftListPressed(QListWidgetItem*)));
  connect(m_ui->m_rightListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(onRightListPressed(QListWidgetItem*)));
}

te::qt::widgets::DoubleListWidget::~DoubleListWidget()
{
}

Ui::DoubleListWidgetForm* te::qt::widgets::DoubleListWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::DoubleListWidget::onAddToolButtonPressed()
{
  int count = m_ui->m_leftListWidget->count();

  for(int i = 0; i < count; ++i)
  {
    if(m_ui->m_leftListWidget->item(i)->isSelected())
    {
      QListWidgetItem* item = m_ui->m_leftListWidget->takeItem(i);

      m_ui->m_rightListWidget->addItem(item);
    }
  }

  if(m_ui->m_leftListWidget->count() == 0)
  {
    m_ui->m_addToolButton->setEnabled(false);
    m_ui->m_addAllToolButton->setEnabled(false);
  }
}

void te::qt::widgets::DoubleListWidget::onAddAllToolButtonPressed()
{
  int count = m_ui->m_leftListWidget->count();

  for(int i = 0; i < count; ++i)
  {
    QListWidgetItem* item = m_ui->m_leftListWidget->takeItem(i);

    m_ui->m_rightListWidget->addItem(item);
  }

  m_ui->m_addToolButton->setEnabled(false);
  m_ui->m_addAllToolButton->setEnabled(false);
}

void te::qt::widgets::DoubleListWidget::onRemoveToolButtonPressed()
{
  int count = m_ui->m_rightListWidget->count();

  for(int i = 0; i < count; ++i)
  {
    if(m_ui->m_rightListWidget->item(i)->isSelected())
    {
      QListWidgetItem* item = m_ui->m_rightListWidget->takeItem(i);

      m_ui->m_leftListWidget->addItem(item);
    }
  }

  if(m_ui->m_rightListWidget->count() == 0)
  {
    m_ui->m_removeToolButton->setEnabled(false);
    m_ui->m_removeAllToolButton->setEnabled(false);
  }
}

void te::qt::widgets::DoubleListWidget::onRemoveAllToolButtonPressed()
{
  int count = m_ui->m_rightListWidget->count();

  for(int i = 0; i < count; ++i)
  {
    QListWidgetItem* item = m_ui->m_rightListWidget->takeItem(i);

    m_ui->m_leftListWidget->addItem(item);
  }

  m_ui->m_removeToolButton->setEnabled(false);
  m_ui->m_removeAllToolButton->setEnabled(false);
}

void te::qt::widgets::DoubleListWidget::onLeftListPressed(QListWidgetItem* item)
{
  if(item)
  {
    m_ui->m_addToolButton->setEnabled(true);
    m_ui->m_addAllToolButton->setEnabled(true);
  }
}

void te::qt::widgets::DoubleListWidget::onRightListPressed(QListWidgetItem* item)
{
  if(item)
  {
    m_ui->m_removeToolButton->setEnabled(true);
    m_ui->m_removeAllToolButton->setEnabled(true);
  }
}

