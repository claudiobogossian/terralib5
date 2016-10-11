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
\file terralib/qt/widgets/progress/ProgressViewerTaskWidget.cpp

\brief A progress dialog.
*/

// Terralib
#include "../../../core/translator/Translator.h"
#include "ProgressViewerTaskWidget.h"
#include "ProgressResetEvent.h"
#include "ProgressSetMessageEvent.h"
#include "ProgressSetValueEvent.h"


// Qt
#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QStyle>

te::qt::widgets::ProgressViewerTaskWidget::ProgressViewerTaskWidget(QWidget* parent, bool hideToolBar, std::string message)
  : QWidget(parent), AbstractProgressViewer(),
    m_taskId(-1),
    m_totalSteps(0),
    m_currentStep(0),
    m_propStep(0),
    m_message(message),
    m_task(0)
{
  // build custom widget
  m_mainGridLayout = new QGridLayout(this);
  m_mainGridLayout->setContentsMargins(1, 1, 1, 1);
  m_mainGridLayout->setHorizontalSpacing(2);

  m_button = new QToolButton(this);
  m_button->setIcon(m_button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));

  m_label = new QLabel(this);

  if (hideToolBar)
  {
    m_mainGridLayout->addWidget(m_button, 0, 0);
    m_mainGridLayout->addWidget(m_label, 0, 1);

    m_progressBar = 0;
  }
  else
  {
    m_progressBar = new QProgressBar(this);

    m_mainGridLayout->addWidget(m_button, 1, 0);
    m_mainGridLayout->addWidget(m_progressBar, 1, 1);
    m_mainGridLayout->addWidget(m_label, 0, 0, 1, 2);

    // set default range
    m_progressBar->setRange(0, 100);
  }

  m_button->setEnabled(false);

  if (!m_message.empty())
    m_label->setText(QString::fromUtf8(m_message.c_str()));

  // connect signal cancel
  connect(m_button, SIGNAL(released()), this, SLOT(cancel()));
}

te::qt::widgets::ProgressViewerTaskWidget::~ProgressViewerTaskWidget()
{
}

void te::qt::widgets::ProgressViewerTaskWidget::addTask(te::common::TaskProgress* t, int id)
{
}

void te::qt::widgets::ProgressViewerTaskWidget::removeTask(int taskId)
{
  cancelTask(taskId);

  if (m_task && m_task->getId() == taskId)
  {
    delete m_task;
    updateMessage(-1);

    QCoreApplication::postEvent(this, new ProgressResetEvent);

    QCoreApplication::processEvents();
  }
}

void te::qt::widgets::ProgressViewerTaskWidget::cancelTask(int taskId)
{
  if (m_task && m_task->getId() == taskId)
  {
    m_totalSteps = 0;
    m_currentStep = 0;
    m_propStep = 0;
  }
}

void te::qt::widgets::ProgressViewerTaskWidget::setTotalValues(int taskId)
{
  if (m_task && m_task->getId() == taskId)
  {
    m_totalSteps = m_task->getTotalSteps();
  }
}

void te::qt::widgets::ProgressViewerTaskWidget::updateValue(int taskId)
{
  double currentStep = 0;

  if (m_task && m_task->getId() == taskId)
  {
    currentStep += m_task->getCurrentStep();

    double aux = static_cast<double>(currentStep) / static_cast<double>(m_totalSteps);

    int val = static_cast<int>(100.0 * aux);

    if (val != m_propStep && val >= 0.0)
    {
      m_propStep = val;

      QCoreApplication::postEvent(this, new ProgressSetValueEvent(m_propStep));

      QCoreApplication::processEvents();
    }
  }
}

void te::qt::widgets::ProgressViewerTaskWidget::updateMessage(int taskId)
{
  if (m_task && m_task->getId() == taskId)
  {
    m_message = m_task->getMessage();

    QCoreApplication::postEvent(this, new ProgressSetMessageEvent(m_message));

    QCoreApplication::processEvents();
  }
}

void te::qt::widgets::ProgressViewerTaskWidget::setTask(te::common::TaskProgress* task)
{
  m_task = task;
  m_taskId = task->getId();

  m_button->setEnabled(true);
}

void te::qt::widgets::ProgressViewerTaskWidget::customEvent(QEvent* e)
{
  if(e->type() == ProgressSetValueEvent::type())
  {
    if (m_progressBar)
      m_progressBar->setValue(static_cast<ProgressSetValueEvent*>(e)->m_value);
  }
  else if(e->type() == ProgressResetEvent::type())
  {
    if (m_progressBar)
      m_progressBar->reset();
  }
  else if (e->type() == ProgressSetMessageEvent::type())
  {
    m_label->setText(static_cast<ProgressSetMessageEvent*>(e)->m_value.c_str());
    return;
  }
}

void te::qt::widgets::ProgressViewerTaskWidget::cancel()
{
  if (m_task)
    m_task->cancel();
}
