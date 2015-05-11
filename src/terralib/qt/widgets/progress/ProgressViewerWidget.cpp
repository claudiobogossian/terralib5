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
  \file terralib/qt/widgets/progress/ProgressViewerWidget.cpp

  \brief A class that defines the interface of a qt widget to group a set of ProgressWidgetItem.
*/

// Terralib
#include "CreateProgressWidgetItemEvent.h"
#include "ProgressViewerWidget.h"
#include "ProgressWidgetItem.h"
#include "RemoveProgressWidgetItemEvent.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QCloseEvent>
#include <QScrollBar>

te::qt::widgets::ProgressViewerWidget::ProgressViewerWidget(QWidget* parent)
  : QDialog(parent, Qt::Tool),
    AbstractProgressViewer()
{
  //create custom widget
  m_MainLayout = new QGridLayout(this);
  m_MainLayout->setContentsMargins(1, 1, 1, 1);

  m_scroll = new QScrollArea(this);
  m_scroll->setBackgroundRole(QPalette::Light);
  m_MainLayout->addWidget(m_scroll);
  m_scroll->setWidgetResizable(true);

  m_widget = new QWidget(m_scroll);
  m_scroll->setWidget(m_widget);

  m_widgetLayout = new QGridLayout(m_widget);
  m_widgetLayout->setContentsMargins(1, 1, 1, 1);
  m_widgetLayout->setVerticalSpacing(1);
  m_widgetLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
}

te::qt::widgets::ProgressViewerWidget::~ProgressViewerWidget()
{
}

void te::qt::widgets::ProgressViewerWidget::addTask(te::common::TaskProgress* t, int id)
{
  m_tasks.insert(std::map<int, te::common::TaskProgress*>::value_type(id, t));

  QCoreApplication::postEvent(this, new CreateProgressWidgetItemEvent(t->getMessage().c_str(), id, t->getTotalSteps()));
  QCoreApplication::processEvents();
}

void te::qt::widgets::ProgressViewerWidget::removeTask(int taskId)
{
  std::map<int, te::common::TaskProgress*>::iterator it = m_tasks.find(taskId);

  if(it != m_tasks.end())
    m_tasks.erase(it);

  QCoreApplication::postEvent(this, new RemoveProgressWidgetItemEvent(taskId));

  QCoreApplication::processEvents();
}

void te::qt::widgets::ProgressViewerWidget::cancelTask(int taskId)
{
  removeTask(taskId);
}

void te::qt::widgets::ProgressViewerWidget::setTotalValues(int /*taskId*/)
{
}

void te::qt::widgets::ProgressViewerWidget::updateValue(int taskId)
{
  std::map<int, te::common::TaskProgress*>::iterator it = m_tasks.find(taskId);

  if(it != m_tasks.end())
  {
    std::map<int, te::qt::widgets::ProgressWidgetItem*>::iterator itItem = m_items.find(taskId);

    if(itItem != m_items.end())
    {
      itItem->second->setValue(it->second->getProportionalValue());
    } 
  }
}

void te::qt::widgets::ProgressViewerWidget::updateMessage(int taskId)
{
  std::map<int, te::common::TaskProgress*>::iterator it = m_tasks.find(taskId);

  if(it != m_tasks.end())
  {
    std::map<int, te::qt::widgets::ProgressWidgetItem*>::iterator itItem = m_items.find(taskId);

    if(itItem != m_items.end())
    {
      itItem->second->setLabel(it->second->getMessage());
    }
  }
}

void te::qt::widgets::ProgressViewerWidget::customEvent(QEvent* e)
{
  if(e->type() == CreateProgressWidgetItemEvent::type())
  {
    CreateProgressWidgetItemEvent* createEvent = static_cast<CreateProgressWidgetItemEvent*>(e);
    
    ProgressWidgetItem* item = new ProgressWidgetItem(m_widget, createEvent->m_taskId, createEvent->m_totalSteps);

    item->setLabel(createEvent->m_label.toStdString());

    m_items.insert(std::map<int, ProgressWidgetItem*>::value_type(createEvent->m_taskId, item));

    connect(item, SIGNAL(taskCanceled(int)), this, SLOT(cancel(int)));

    m_widgetLayout->addWidget(item);

    item->show();

    return;
  }

  if(e->type() == RemoveProgressWidgetItemEvent::type())
  {
    RemoveProgressWidgetItemEvent* removeEvent = static_cast<RemoveProgressWidgetItemEvent*>(e);
    
    std::map<int, te::qt::widgets::ProgressWidgetItem*>::iterator itItem = m_items.find(removeEvent->m_taskId);
    if(itItem == m_items.end())
      return;

    delete itItem->second;
    m_items.erase(itItem);
  }
}

void te::qt::widgets::ProgressViewerWidget::cancel(int id)
{
  std::map<int, te::common::TaskProgress*>::iterator itTask = m_tasks.find(id);

  if(itTask != m_tasks.end())
  {
    itTask->second->cancel();
  }
}

