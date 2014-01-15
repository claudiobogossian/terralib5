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
  \file terralib/common/progress/ProgressTimer.cpp

  \brief The ProgressTimer is a utility class that can be used to calculate the estimated time to finish a task.
*/

// TerraLib
#include "../StringUtils.h"
#include "../Translator.h"
#include "ProgressTimer.h"

te::common::ProgressTimer::ProgressTimer(int totalSteps, std::string message)
  : m_totalSteps(totalSteps),
    m_count(0),
    m_startTime(0),
    m_remainingTime(0.0),
    m_speedTime(0.0),
    m_message(message)
{
}

te::common::ProgressTimer::~ProgressTimer()
{
}

void te::common::ProgressTimer::start()
{
// start time
  time(&m_startTime);

  m_count = 0;
}

void te::common::ProgressTimer::tick()
{
  ++m_count;

  time_t curtime;

  // current time
  time(&curtime);

  // difference in seconds
  double diffInSec = difftime(curtime, m_startTime);

  m_speedTime = (static_cast<double>(m_count) / diffInSec);

  int remainingSteps = m_totalSteps - m_count;

  double remainingTimeInSec = (static_cast<double>(remainingSteps) / m_speedTime);

  m_remainingTime = remainingTimeInSec / 60.0;
}

void te::common::ProgressTimer::setTotalSteps(int totalSteps)
{
  m_totalSteps = totalSteps;
}

void te::common::ProgressTimer::setMessage(std::string message)
{
  m_message = message;
}

double te::common::ProgressTimer::getRemainingTimeInMin() const
{
  return m_remainingTime;
}

double te::common::ProgressTimer::getSpeedTimeInSec() const
{
  return m_speedTime;
}

std::string te::common::ProgressTimer::getMessage()
{
  std::string strTime = m_message + "\n";

  if(m_remainingTime < 1.0)
  {
    strTime += TR_COMMON("Remaining Time: Less than one minute");
  }
  else
  {
    strTime += TR_COMMON("Remaining Time: ") + te::common::Convert2String(m_remainingTime, 1);
    strTime += TR_COMMON(" minute(s) - Speed: ") + te::common::Convert2String(m_speedTime, 2);
    strTime += TR_COMMON(" Steps/Second");
  }

  return strTime;
}
