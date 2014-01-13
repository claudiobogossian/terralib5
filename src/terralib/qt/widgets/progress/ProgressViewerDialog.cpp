/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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

  \brief A progress dialog.
*/

// Terralib
#include "../../../common/Translator.h"
#include "ProgressResetEvent.h"
#include "ProgressSetMessageEvent.h"
#include "ProgressSetValueEvent.h"
#include "ProgressViewerDialog.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>

te::qt::widgets::ProgressViewerDialog::ProgressViewerDialog(QWidget* parent)
  : AbstractProgressViewer(),
    m_totalSteps(0),
    m_currentStep(0),
    m_propStep(0),
    m_message("")
{
  m_dlgProgress = new QProgressDialog(parent);
  m_dlgProgress->setWindowModality(Qt::NonModal);
  m_dlgProgress->setRange(0, 100);
  m_dlgProgress->setWindowTitle(parent->windowTitle());

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

    QCoreApplication::postEvent(this, new ProgressResetEvent);

    QCoreApplication::processEvents();
  }
}

void te::qt::widgets::ProgressViewerDialog::cancelTask(int taskId)
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

void te::qt::widgets::ProgressViewerDialog::setTotalValues(int taskId)
{
  m_totalSteps += m_tasks[taskId]->getTotalSteps();
}

void te::qt::widgets::ProgressViewerDialog::updateValue(int /*taskId*/)
{
  m_dlgProgress->show();

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

void te::qt::widgets::ProgressViewerDialog::updateMessage(int /*taskId*/)
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

void te::qt::widgets::ProgressViewerDialog::customEvent(QEvent* e)
{
  if(e->type() == ProgressSetValueEvent::type())
  {
    m_dlgProgress->setValue(static_cast<ProgressSetValueEvent*>(e)->m_value);
  }
  else if(e->type() == ProgressSetMessageEvent::type())
  {
    m_dlgProgress->setLabelText(static_cast<ProgressSetMessageEvent*>(e)->m_value.c_str());
  }
  else if(e->type() == ProgressResetEvent::type())
  {
    m_dlgProgress->reset();
  }
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
