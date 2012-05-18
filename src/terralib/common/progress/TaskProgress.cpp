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
  \file terralib/common/progress/TaskProgress.cpp

  \brief 
 */

// TerraLib
#include "ProgressManager.h"
#include "TaskProgress.h"


te::common::TaskProgress::TaskProgress():
  m_totalSteps(0),
  m_currentStep(0),
  m_currentPropStep(0),
  m_message(""),
  m_hasToUpdate(false),
  m_isActive(true),
  m_isMultiThread(false),
  m_useTimer(false)
{
  //get task id from progress manager singleton
  m_id = te::common::ProgressManager::getInstance().addTask(this);
}

te::common::TaskProgress::~TaskProgress()
{
  te::common::ProgressManager::getInstance().removeTask(m_id);
}

int te::common::TaskProgress::getId()
{
  return m_id;
}

int te::common::TaskProgress::getTotalSteps()
{
  return m_totalSteps;
}

void te::common::TaskProgress::setTotalSteps(int value)
{
  m_totalSteps = value;

  te::common::ProgressManager::getInstance().setTotalValues(m_id);
}

int te::common::TaskProgress::getProportionalValue()
{
  return m_currentPropStep;
}

int te::common::TaskProgress::getCurrentStep()
{
  return m_currentStep;
}

void te::common::TaskProgress::setCurrentStep(int value)
{
  if(m_isActive)
  {
    m_currentStep = value;

    double aux = (double) m_currentStep / (double) m_totalSteps;

    int val = int (100. * aux);

    if(val > m_currentPropStep)
    {
      m_currentPropStep = val;
      m_hasToUpdate = true;
    }
    else
    {
      m_hasToUpdate = false;
    }

    //inform progress manager singleton that current value has changed
    te::common::ProgressManager::getInstance().updateValue(m_id);
  }
}

void te::common::TaskProgress::pulse()
{
  //increment by one current value
  int val = getCurrentStep();

  setCurrentStep(++val);
}

std::string te::common::TaskProgress::getMessage()
{
  return m_message;
}

void te::common::TaskProgress::setMessage(std::string message)
{
  m_message = message;

  //inform progress manager singleton that message has changed
  te::common::ProgressManager::getInstance().updateMessage(m_id);
}

bool te::common::TaskProgress::isActive()
{
  return m_isActive;
}

void te::common::TaskProgress::cancel()
{
  m_isActive = false;

  //inform progress manager singleton that current was canceled
  te::common::ProgressManager::getInstance().cancelTask(m_id);
}

void te::common::TaskProgress::useMultiThread(bool flag)
{
  m_isMultiThread = flag;
}

void te::common::TaskProgress::useTimer(bool flag)
{
  m_useTimer = flag;
}

bool te::common::TaskProgress::hasToUpdate()
{
  return m_hasToUpdate;
}
