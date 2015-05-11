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
  \file terralib/common/progress/TaskProgress.cpp

  \brief This class is the implementation of a progress evalution of a task.
 */

// TerraLib
#include "ProgressManager.h"
#include "ProgressTimer.h"
#include "TaskProgress.h"

te::common::TaskProgress::TaskProgress(const std::string& message, unsigned int type, int totalSteps)
  : m_id(-1),
    m_type(type),
    m_totalSteps(totalSteps),
    m_currentStep(0),
    m_currentPropStep(0),
    m_message(message),
    m_hasToUpdate(false),
    m_isActive(true),
    m_isMultiThread(false),
    m_useTimer(false),
    m_timer(0)
{
 //get task id from progress manager singleton
  m_id = te::common::ProgressManager::getInstance().addTask(this);

  setTotalSteps(totalSteps);
}

te::common::TaskProgress::~TaskProgress()
{
  te::common::ProgressManager::getInstance().removeTask(m_id);

  delete m_timer;
}

int te::common::TaskProgress::getId() const
{
  return m_id;
}

unsigned int te::common::TaskProgress::getType() const
{
  return m_type;
}

int te::common::TaskProgress::getTotalSteps() const
{
  return m_totalSteps;
}

void te::common::TaskProgress::setTotalSteps(int value)
{
  if(value <= 0)
    return;

  m_totalSteps = value;

  if(m_timer)
  {
    m_timer->setTotalSteps(m_totalSteps);

    // reset timer clock
    m_timer->start();
  }

  te::common::ProgressManager::getInstance().setTotalValues(m_id);
}

int te::common::TaskProgress::getProportionalValue() const
{
  return m_currentPropStep;
}

int te::common::TaskProgress::getCurrentStep() const
{
  return m_currentStep;
}

void te::common::TaskProgress::setCurrentStep(int value)
{
  if(m_isActive)
  {
    m_currentStep = value;

    double aux = static_cast<double>(m_currentStep) / static_cast<double>(m_totalSteps);

    int val = static_cast<int>(100.0 * aux);

    if(val > m_currentPropStep)
    {
      m_currentPropStep = val;
      m_hasToUpdate = true;
    }
    else
    {
      m_hasToUpdate = false;
    }

    if(m_timer)
    {
      m_timer->tick();

      setMessage(m_timer->getMessage());
    }

    // inform the progress manager singleton that the current value has changed
    te::common::ProgressManager::getInstance().updateValue(m_id);
  }
}

void te::common::TaskProgress::pulse()
{
  // increment by one current value
  int val = getCurrentStep();

  setCurrentStep(++val);
}

const std::string& te::common::TaskProgress::getMessage() const
{
  return m_message;
}

void te::common::TaskProgress::setMessage(const std::string& message)
{
  m_message = message;

  // inform the progress manager singleton that the message has changed
  te::common::ProgressManager::getInstance().updateMessage(m_id);
}

bool te::common::TaskProgress::isActive() const
{
  return m_isActive;
}

void te::common::TaskProgress::cancel()
{
  m_isActive = false;

  // inform the progress manager singleton that the current task was canceled
  te::common::ProgressManager::getInstance().cancelTask(m_id);
}

void te::common::TaskProgress::useMultiThread(bool flag)
{
  m_isMultiThread = flag;

  // code restriction, must be fixed
  if(m_isMultiThread)
  {
    delete m_timer;
    m_timer = 0;
    m_useTimer = false;
  }
}

void te::common::TaskProgress::useTimer(bool flag)
{
  m_useTimer = flag;

  if(m_timer == 0 && m_useTimer)
  {
    // code restriction, must be fixed
    if(!m_isMultiThread)
    {
      m_timer = new ProgressTimer(getTotalSteps(), m_message);
      m_timer->start();
    }
  }
}

bool te::common::TaskProgress::hasToUpdate() const
{
  return m_hasToUpdate;
}
