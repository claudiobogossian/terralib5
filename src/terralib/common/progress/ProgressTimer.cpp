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
  \file terralib/common/ProgressTimer.cpp

  \brief The ProgressTimer is a util class used to calculate the estimated
         time to finish loop job.
*/

// TerraLib
#include "ProgressTimer.h"

#include "../StringUtils.h"
#include "../Translator.h"



te::common::ProgressTimer::ProgressTimer(int totalSteps) :
  m_totalSteps(totalSteps),
  m_count(0),
  m_startTime(NULL),
  m_remainingTime(0.),
  m_speedTime(0.)
{
}

te::common::ProgressTimer::~ProgressTimer()
{
}

void te::common::ProgressTimer::start()
{
  //start time
   time(&m_startTime); 
}

void te::common::ProgressTimer::tick()
{
  ++m_count;

  time_t curtime;

  //current time
  time(&curtime);

  //difference in seconds
  double diffInSec = difftime(curtime, m_startTime);

  int remainingSteps = m_totalSteps - m_count;

  m_speedTime = ((double)m_count / diffInSec);

  double remainingTimeInSec = ((double)remainingSteps / m_speedTime);

  m_remainingTime = remainingTimeInSec / 60.;
}

void te::common::ProgressTimer::setTotalSteps(const int& totalSteps)
{
  m_totalSteps = totalSteps;
}

double te::common::ProgressTimer::getRemainingTimeInMin()
{
  return m_remainingTime;
}

double te::common::ProgressTimer::getSpeedTimeInSec()
{
  return m_speedTime;
}

std::string te::common::ProgressTimer::getMessage()
{
  std::string strTime = "";

  if(m_remainingTime < 1.)
  {
	  strTime = TR_COMMON("Remaining Time: Less than one minute");
  }
  else
  {
    strTime  = TR_COMMON("Remaining Time: ") + te::common::Convert2String(m_remainingTime, 1);
    strTime += TR_COMMON(" minute(s) - Speed: ") + te::common::Convert2String(m_speedTime, 2);
    strTime += TR_COMMON(" Steps/Second");
  }

  return strTime;
}
