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
  \file terralib/qt/widgets/progress/ProgressViewerDialog.cpp

  \brief A class that defines the interface of a qt bar progress viewer.
         This widget is a dialog box with progress information and a
         cancel button.
 */

// Terralib
#include "ProgressViewerDialog.h"
#include "ProgressSetMessageEvent.h"
#include "ProgressSetValueEvent.h"
#include "terralib/common/Translator.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>

te::qt::widgets::ProgressViewerDialog::ProgressViewerDialog(QWidget* parent) : AbstractProgressViewer(),
  m_totalSteps(0),
  m_currentStep(0),
  m_propStep(0),
  m_message("")
{
  m_dlgProgress = new QProgressDialog(parent);
  m_dlgProgress->setWindowModality(Qt::NonModal);
  m_dlgProgress->setRange(0, 100);

  installEventFilter(this);

  connect(m_dlgProgress, SIGNAL(canceled()), this, SLOT(cancel()));
}

te::qt::widgets::ProgressViewerDialog::~ProgressViewerDialog()
{
  delete m_dlgProgress;
}

void te::qt::widgets::ProgressViewerDialog::addTask(te::common::TaskProgress* t, int id)
{
  m_tasks.insert(std::map<int, te::common::TaskProgress*>::value_type(id, t));

  updateMessage(-1);
}

void te::qt::widgets::ProgressViewerDialog::removeTask(int taskId)
{
  std::map<int, te::common::TaskProgress*>::iterator it = m_tasks.find(taskId);

  if(it != m_tasks.end())
  {
    m_tasks.erase(it);
    updateMessage(-1);
  }

  if(m_tasks.empty())
  {
    m_totalSteps = 0;
    m_currentStep = 0;
    m_propStep = 0;

    m_dlgProgress->reset();
  }
}

void te::qt::widgets::ProgressViewerDialog::cancelTask(int taskId)
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

void te::qt::widgets::ProgressViewerDialog::setTotalValues(int taskId)
{
  m_totalSteps += m_tasks[taskId]->getTotalSteps();
}

void te::qt::widgets::ProgressViewerDialog::updateValue(int taskId)
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

void te::qt::widgets::ProgressViewerDialog::updateMessage(int taskId)
{
  if(m_tasks.size() == 1)
  {
    m_message = m_tasks.begin()->second->getMessage();
  }
  else
  {
    m_message = TR_QT_WIDGETS("Multi Tasks");
  }

  QCoreApplication::postEvent(this, new ProgressSetMessageEvent(m_message));
}

bool te::qt::widgets::ProgressViewerDialog::eventFilter(QObject* obj, QEvent* event)
{
  if(obj == this && event->type() == ProgressSetValueEvent::type())
  {
    ProgressSetValueEvent* e = 
      static_cast<ProgressSetValueEvent*>(event);

    m_dlgProgress->setValue(e->m_value);

    return true;
  }
  else if(obj == this && event->type() == ProgressSetMessageEvent::type())
  {
    ProgressSetMessageEvent* e = 
      static_cast<ProgressSetMessageEvent*>(event);

    m_dlgProgress->setLabelText(e->m_value.c_str());

    return true;
  }

  return true;
}

void te::qt::widgets::ProgressViewerDialog::cancel()
{
  std::map<int, te::common::TaskProgress*>::iterator it = m_tasks.begin();

  while(it != m_tasks.end())
  {
    it->second->cancel();

    ++it;
  }
}
