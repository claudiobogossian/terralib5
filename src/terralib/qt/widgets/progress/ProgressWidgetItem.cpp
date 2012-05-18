/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/progress/ProgressWidgetItem.cpp

  \brief Custom widget used to represent a single task.
 */

// TerraLib
#include "ProgressSetMessageEvent.h"
#include "ProgressSetValueEvent.h"
#include "ProgressWidgetItem.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QtGui/QSizePolicy>
#include <QtGui/QStyle>


te::qt::widgets::ProgressWidgetItem::ProgressWidgetItem(QWidget* parent, int taskId) : 
  QWidget(parent),
  m_taskId(taskId)
{
  //build custom widget
  m_mainGridLayout = new QGridLayout(this);
  m_mainGridLayout->setContentsMargins(1,1,1,1);
  m_mainGridLayout->setHorizontalSpacing(2);
  
  m_frame = new QFrame(this);
  m_frame->setFrameShape(QFrame::StyledPanel);
  m_frame->setFrameShadow(QFrame::Sunken);

  m_mainGridLayout->addWidget(m_frame);

  m_frameGridLayout = new QGridLayout(m_frame);
  m_frameGridLayout->setContentsMargins(1,1,1,1);
  m_frameGridLayout->setVerticalSpacing(1);

  m_label = new QLabel(m_frame);
  m_label->setText("Label Test");

  m_progressBar = new QProgressBar(m_frame);

  m_button = new QCommandLinkButton(m_frame);
  m_button->setIcon(m_button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
  m_button->setFixedHeight(23);
  m_button->setFixedWidth(23);
  
  m_frameGridLayout->addWidget(m_label, 0, 0);
  m_frameGridLayout->addWidget(m_progressBar, 1, 0);
  m_frameGridLayout->addWidget(m_button, 1, 1, 1, 1, Qt::AlignCenter);

  //set default range
  m_progressBar->setRange(0,100);

  //enable this object to handle events
  installEventFilter(this);

  //connect signal cancel
  connect(m_button, SIGNAL(released()), this, SLOT(cancel()));

  this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed); 
}

te::qt::widgets::ProgressWidgetItem::~ProgressWidgetItem()
{
}

void te::qt::widgets::ProgressWidgetItem::setValue(int step)
{
  QCoreApplication::postEvent(this, new te::qt::widgets::ProgressSetValueEvent(step));

  QCoreApplication::processEvents();
}

void te::qt::widgets::ProgressWidgetItem::setLabel(std::string message)
{
  QCoreApplication::postEvent(this, new ProgressSetMessageEvent(message));
}

void te::qt::widgets::ProgressWidgetItem::reset()
{
  m_progressBar->reset();
}

void te::qt::widgets::ProgressWidgetItem::cancel()
{
  emit taskCanceled(m_taskId);
}

bool te::qt::widgets::ProgressWidgetItem::eventFilter(QObject* obj, QEvent* event)
{
  if(obj == this && event->type() == te::qt::widgets::ProgressSetValueEvent::type())
  {
    te::qt::widgets::ProgressSetValueEvent* e = 
      static_cast<te::qt::widgets::ProgressSetValueEvent*>(event);

    m_progressBar->setValue(e->m_value);

    return true;
  }
  else if(obj == this && event->type() == ProgressSetMessageEvent::type())
  {
    ProgressSetMessageEvent* e = 
      static_cast<ProgressSetMessageEvent*>(event);

     m_label->setText(e->m_value.c_str());

    return true;
  }

  return true;
}
