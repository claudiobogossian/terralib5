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
  \file terralib/common/AbstractProgress.cpp

  \brief The AbstractProgress is an abstract class used to define the main progress functions.
*/

// TerraLib
#include "AbstractProgress.h"
#include "ProgressParams.h"
#include "ProgressTimer.h"


te::common::AbstractProgress::AbstractProgress() : m_timer(0)
{
  reset();
}

te::common::AbstractProgress::~AbstractProgress()
{  
  delete m_timer;
}

void te::common::AbstractProgress::setTotalSteps(const int& value)
{
	m_progressParams.m_minValue = 0;
	m_progressParams.m_maxValue = value - 1;

  m_progressParams.m_totalSteps = value;

  if(m_timer)
  {
    m_timer->setTotalSteps(value);
  }
}

void te::common::AbstractProgress::setCurrentStep(const int& step)
{
  if(m_isActive)
  {
    if(m_timer)
    {
      m_timer->tick();

      setMessage(m_timer->getMessage());
    }

    m_progressParams.m_currentStep = step;

	  double aux = (double) m_progressParams.m_currentStep / (double) m_progressParams.m_totalSteps;

    int val = int (100. * aux);

	  if(val > m_progressParams.m_currentPropStep)
	  {
		  m_progressParams.m_currentPropStep = val;
		  m_hasToUpdate = true;
	  }
	  else
	  {
		  m_hasToUpdate = false;
	  }
  }
}

int te::common::AbstractProgress::getCurrentStep()
{
  return m_progressParams.m_currentStep;
}

int te::common::AbstractProgress::getCurrentProportionalStep()
{
  return m_progressParams.m_currentPropStep;
}

void te::common::AbstractProgress::setMessage(const std::string& message)
{
	m_progressParams.m_message = message;
}

std::string te::common::AbstractProgress::getMessage()
{
  return m_progressParams.m_message;
}

void te::common::AbstractProgress::setTitle(const std::string& title)
{
}

void te::common::AbstractProgress::setActive(const bool& status)
{
  m_isActive = status;
}


bool te::common::AbstractProgress::isActive()
{
	return m_isActive;
}

void te::common::AbstractProgress::reset()
{
  m_progressParams.m_minValue = 0;
  m_progressParams.m_maxValue = 0;
  m_progressParams.m_totalSteps = 0;
  m_progressParams.m_currentStep = 0;
  m_progressParams.m_currentPropStep = 0;
  m_progressParams.m_message = "";

  m_isActive = true;
  m_hasToUpdate = false;
  m_isMultiThread = false;
}

bool te::common::AbstractProgress::hasToUpdate()
{
  return m_hasToUpdate;
}

void te::common::AbstractProgress::pulse()
{
  setCurrentStep(m_progressParams.m_currentStep + 1);
}

void te::common::AbstractProgress::cancel()
{
  m_isActive = false;
}

void te::common::AbstractProgress::setMultiThreadProgress(const bool& flag)
{
  m_isMultiThread = flag;

  if(flag)
  {
    useProgressTimer(false);
  }
}

void te::common::AbstractProgress::useProgressTimer(const bool& flag)
{
  delete m_timer;
  m_timer = 0;

  if(m_isMultiThread)
  {
    return;
  }

  if(flag)
  {
    m_timer = new ProgressTimer(m_progressParams.m_totalSteps);
  }
}

void te::common::AbstractProgress::startTimer()
{
  if(m_timer)
  {
    m_timer->start();
  }
}
