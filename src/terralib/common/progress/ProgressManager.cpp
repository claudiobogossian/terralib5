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
  \file terralib/common/progress/ProgressManager.cpp

  \brief Implementation of the singleton to keep a progress interface instance.
*/

// TerraLib
#include "../Exception.h"
#include "../Translator.h"
#include "AbstractProgress.h"
#include "ProgressManager.h"

void te::common::ProgressManager::setMainProgress(AbstractProgress* pi)
{
  m_progressMap.insert(std::map<int, AbstractProgress*>::value_type(-1, pi));
}

te::common::AbstractProgress* te::common::ProgressManager::getMainProgress()
{
  return getProgress(-1);
}

int te::common::ProgressManager::setProgress(AbstractProgress* pi)
{
  int id = getProgressId();

  m_progressMap.insert(std::map<int, AbstractProgress*>::value_type(id, pi));

  return id;
}

te::common::AbstractProgress* te::common::ProgressManager::getProgress(const int& id)
{
  std::map<int, AbstractProgress*>::iterator it = m_progressMap.find(id);

  if(it == m_progressMap.end())
  {
    throw te::common::Exception(TR_COMMON("Progress not found!"));
  }

  return it->second;
}

void te::common::ProgressManager::setTotalSteps(const int& value, const int& id)
{
    AbstractProgress* ap = getProgress(id);

    ap->setTotalSteps(value);
}

void te::common::ProgressManager::setCurrentStep(const int& step, const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->setCurrentStep(step);
}

int te::common::ProgressManager::getCurrentStep(const int& id)
{
  AbstractProgress* ap = getProgress(id);

  return ap->getCurrentStep();
}

int te::common::ProgressManager::getCurrentProportionalStep(const int& id)
{
  AbstractProgress* ap = getProgress(id);

  return ap->getCurrentProportionalStep();
}
     
void te::common::ProgressManager::setMessage(const std::string& message, const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->setMessage(message);
}

std::string te::common::ProgressManager::getMessage(const int& id)
{
  AbstractProgress* ap = getProgress(id);

  return ap->getMessage();
}

void te::common::ProgressManager::setTitle(const std::string& title, const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->setTitle(title);
}

void te::common::ProgressManager::setActive(const bool& status, const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->setActive(status);
}

bool te::common::ProgressManager::isActive(const int& id)
{
  AbstractProgress* ap = getProgress(id);

  return ap->isActive();
}

void te::common::ProgressManager::reset(const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->reset();
}
        
bool te::common::ProgressManager::hasToUpdate(const int& id)
{
  AbstractProgress* ap = getProgress(id);

  return ap->hasToUpdate();
}

void te::common::ProgressManager::pulse(const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->pulse();
}
        
void te::common::ProgressManager::cancel(const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->cancel();
}

void te::common::ProgressManager::setModal(const bool& flag, const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->setModal(flag);
}

void te::common::ProgressManager::setMultiThreadProgress(const bool& flag, const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->setMultiThreadProgress(flag);
}
      
void te::common::ProgressManager::useProgressTimer(const bool& flag, const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->useProgressTimer(flag);
}

void te::common::ProgressManager::startTimer(const int& id)
{
  AbstractProgress* ap = getProgress(id);

  ap->startTimer();
}

te::common::ProgressManager::ProgressManager():
m_progressCounter(0)
{
}

te::common::ProgressManager::~ProgressManager()
{
  m_progressMap.clear();
}

int te::common::ProgressManager::getProgressId()
{
  return m_progressCounter++;
}
