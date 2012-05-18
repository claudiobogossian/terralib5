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
  \file terralib/qt/widgets/progress/WidgetProgressItem.cpp

  \brief A class that defines the interface of a qt bar progress viewer.
         Can be use inside a status bar.
 */

// Terralib
#include "BarProgressViewer.h"
#include "ProgressSetMessageEvent.h"
#include "ProgressSetValueEvent.h"
#include "terralib/common/Translator.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>

te::qt::widgets::BarProgressViewer::BarProgressViewer(QWidget* parent) : 
  QWidget(parent), 
  AbstractProgressViewer(),
  m_totalSteps(0),
  m_currentStep(0),
  m_propStep(0)
{
  m_barProgress = new QProgressBar(this);
  m_barProgress->setRange(0, 100);
  m_barProgress->setTextVisible(false);
  m_barProgress->setFixedHeight(16);

  m_button = new QPushButton(this);
  m_button->setText("...");
  m_button->setFixedSize(20, 16);

  m_layout = new QGridLayout(this);

  m_layout->setContentsMargins(0,0,0,0);
  m_layout->setHorizontalSpacing(1);
  m_layout->addWidget(m_barProgress, 0, 0);
  m_layout->addWidget(m_button, 0, 1);

  installEventFilter(this);

  connect(m_button, SIGNAL(released()), this, SLOT(on_Released()));
}

te::qt::widgets::BarProgressViewer::~BarProgressViewer()
{
}

void te::qt::widgets::BarProgressViewer::addTask(te::common::TaskProgress* t, int id)
{
  m_tasks.insert(std::map<int, te::common::TaskProgress*>::value_type(id, t));
}

void te::qt::widgets::BarProgressViewer::removeTask(int taskId)
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

    m_barProgress->reset();
  }
}

void te::qt::widgets::BarProgressViewer::cancelTask(int taskId)
{
  std::map<int, te::common::TaskProgress*>::iterator it = m_tasks.find(taskId);

  if(it != m_tasks.end())
  {
    //update total and current values
    m_totalSteps -= it->second->getTotalSteps();
    m_currentStep -= it->second->getCurrentStep();

    double aux = (double) m_currentStep / (double) m_totalSteps;

    m_propStep = int (100. * aux);
  }
}

void te::qt::widgets::BarProgressViewer::setTotalValues(int taskId)
{
  m_totalSteps += m_tasks[taskId]->getTotalSteps();
}

void te::qt::widgets::BarProgressViewer::updateValue(int taskId)
{
  m_currentStep++;

  double aux = (double) m_currentStep / (double) m_totalSteps;

  int val = int (100. * aux);

  if(val != m_propStep && val >= 0.)
  {
    m_propStep = val;

    QCoreApplication::postEvent(this, new ProgressSetValueEvent(m_propStep));
    QCoreApplication::processEvents();
  }
}

void te::qt::widgets::BarProgressViewer::updateMessage(int taskId)
{
}

bool te::qt::widgets::BarProgressViewer::eventFilter(QObject* obj, QEvent* event)
{
  if(obj == this && event->type() == ProgressSetValueEvent::type())
  {
    ProgressSetValueEvent* e = 
      static_cast<ProgressSetValueEvent*>(event);

    m_barProgress->setValue(e->m_value);

    return true;
  }

  return true;
}

void te::qt::widgets::BarProgressViewer::on_Released()
{
  emit clicked();
}
