/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/ProgressManager.cpp

  \brief Implementation of the singleton to keep a progress interface instance.
*/

// TerraLib
#include "../Exception.h"
#include "../Translator.h"

#include "AbstractProgress.h"
#include "ProgressManager.h"



te::common::ProgressManager::ProgressManager() : m_progressInterface(0)
{
}

te::common::ProgressManager::~ProgressManager()
{  
  delete m_progressInterface;
}

void te::common::ProgressManager::setProgress(AbstractProgress* pi)
{
  m_progressInterface = pi;
}

te::common::AbstractProgress* te::common::ProgressManager::getProgress()
{
  return m_progressInterface;
}

void te::common::ProgressManager::setTotalSteps(const int& value)
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->setTotalSteps(value);
}

void te::common::ProgressManager::setCurrentStep(const int& step)
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->setCurrentStep(step);
}

int te::common::ProgressManager::getCurrentStep()
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  return m_progressInterface->getCurrentStep();
}

int te::common::ProgressManager::getCurrentProportionalStep()
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  return m_progressInterface->getCurrentProportionalStep();
}
     
void te::common::ProgressManager::setMessage(const std::string& message)
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->setMessage(message);
}

std::string te::common::ProgressManager::getMessage()
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  return m_progressInterface->getMessage();
}

void te::common::ProgressManager::setTitle(const std::string& title)
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->setTitle(title);
}

void te::common::ProgressManager::setActive(const bool& status)
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->setActive(status);
}

bool te::common::ProgressManager::isActive()
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  return m_progressInterface->isActive();
}

void te::common::ProgressManager::reset()
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->reset();
}
        
bool te::common::ProgressManager::hasToUpdate()
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  return m_progressInterface->hasToUpdate();
}

void te::common::ProgressManager::pulse()
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->pulse();
}
        
void te::common::ProgressManager::cancel()
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->cancel();
}

void te::common::ProgressManager::setMultiThreadProgress(const bool& flag)
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->setMultiThreadProgress(flag);
}
      
void te::common::ProgressManager::useProgressTimer(const bool& flag)
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->useProgressTimer(flag);
}

void te::common::ProgressManager::startTimer()
{
  if(!m_progressInterface)
    throw te::common::Exception(TR_COMMON("No progress interface was defined!"));

  m_progressInterface->startTimer();
}
