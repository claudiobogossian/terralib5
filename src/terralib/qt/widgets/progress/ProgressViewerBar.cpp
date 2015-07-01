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
  \file terralib/qt/widgets/progress/ProgressViewerBar.cpp

  \brief A class that defines the interface of a qt bar progress viewer.
*/

// Terralib
#include "../../../common/Translator.h"
#include "ProgressSetMessageEvent.h"
#include "ProgressSetValueEvent.h"
#include "ProgressResetEvent.h"
#include "ProgressViewerBar.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QApplication>

te::qt::widgets::ProgressViewerBar::ProgressViewerBar(QWidget* parent)
  : QWidget(parent), 
    AbstractProgressViewer(),
    m_totalSteps(0),
    m_currentStep(0),
    m_propStep(0)
{
  m_progressBar = new QProgressBar(this);
  m_progressBar->setRange(0, 100);
  m_progressBar->setTextVisible(false);
  m_progressBar->setFixedHeight(16);

  m_button = new QPushButton(this);
  m_button->setText("...");
  m_button->setFixedSize(24, 18);

  m_layout = new QGridLayout(this);

  m_layout->setContentsMargins(0, 0, 0, 0);
  m_layout->setHorizontalSpacing(1);
  m_layout->addWidget(m_progressBar, 0, 0);
  m_layout->addWidget(m_button, 0, 1);

  connect(m_button, SIGNAL(released()), this, SLOT(onReleased()));
}

te::qt::widgets::ProgressViewerBar::~ProgressViewerBar()
{
}

void te::qt::widgets::ProgressViewerBar::addTask(te::common::TaskProgress* t, int id)
{
  m_tasks.insert(std::map<int, te::common::TaskProgress*>::value_type(id, t));
}

void te::qt::widgets::ProgressViewerBar::removeTask(int taskId)
{
  std::map<int, te::common::TaskProgress*>::iterator it = m_tasks.find(taskId);

  if(it != m_tasks.end())
  {
    m_tasks.erase(it);
  }

  if(m_tasks.empty())
  {
    m_totalSteps = 0;
    m_currentStep = 0;
    m_propStep = 0;

    QCoreApplication::postEvent(this, new ProgressResetEvent);
    QCoreApplication::processEvents();
  }
}

void te::qt::widgets::ProgressViewerBar::cancelTask(int taskId)
{
  std::map<int, te::common::TaskProgress*>::iterator it = m_tasks.find(taskId);

  if(it != m_tasks.end())
  {
    // update total and current values
    m_totalSteps -= it->second->getTotalSteps();
    m_currentStep -= it->second->getCurrentStep();

    double aux = static_cast<double>(m_currentStep) / static_cast<double>(m_totalSteps);

    m_propStep = static_cast<int>(100.0 * aux);
  }
}

void te::qt::widgets::ProgressViewerBar::setTotalValues(int taskId)
{
  m_totalSteps += m_tasks[taskId]->getTotalSteps();
}

void te::qt::widgets::ProgressViewerBar::updateValue(int /*taskId*/)
{
  m_currentStep++;

  double aux = static_cast<double>(m_currentStep) / static_cast<double>(m_totalSteps);

  int val = static_cast<int>(100.0 * aux);

  if(val != m_propStep && val >= 0.0)
  {
    m_propStep = val;

    QCoreApplication::postEvent(this, new ProgressSetValueEvent(m_propStep));
    QCoreApplication::processEvents();
  }
}

void te::qt::widgets::ProgressViewerBar::updateMessage(int /*taskId*/)
{
}

void te::qt::widgets::ProgressViewerBar::setButtonText(const std::string& value)
{
  m_button->setText(value.c_str());
}

void te::qt::widgets::ProgressViewerBar::customEvent(QEvent* e)
{
  if(e->type() == ProgressSetValueEvent::type())
  {
    m_progressBar->setValue(static_cast<ProgressSetValueEvent*>(e)->m_value);
    return;
  }

  if(e->type() == ProgressResetEvent::type())
    m_progressBar->reset();
}

void te::qt::widgets::ProgressViewerBar::onReleased()
{
  emit clicked();
}
