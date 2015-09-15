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
  \file terralib/statistics/StringStatisticalSummary.cpp
 
  \brief A struct that can be used to keep the summary of statistics.
*/

// TerraLib
#include "StringStatisticalSummary.h"

te::stat::StringStatisticalSummary::StringStatisticalSummary()
  : m_minVal(""),
    m_maxVal(""),
    m_mode(""),
    m_count(0),
    m_validCount(0)
{
}

te::stat::StringStatisticalSummary::StringStatisticalSummary(const StringStatisticalSummary& rhs)
  : m_minVal(""),
    m_maxVal(""),
    m_mode(""),
    m_count(0),
    m_validCount(0)
{
  m_minVal = rhs.m_minVal;
  m_maxVal = rhs.m_maxVal;
  m_mode = rhs.m_mode;
  m_count = rhs.m_count;
  m_validCount = rhs.m_validCount;
}

te::stat::StringStatisticalSummary::~StringStatisticalSummary()
{
  clear();
}

te::stat::StringStatisticalSummary& te::stat::StringStatisticalSummary::operator=(const StringStatisticalSummary& rhs)
{
  if(this != &rhs)
  {
    clear();

    m_minVal = rhs.m_minVal;
    m_maxVal = rhs.m_maxVal;
    m_mode = rhs.m_mode;
    m_count = rhs.m_count;
    m_validCount = rhs.m_validCount;
  }

  return *this;
}

void te::stat::StringStatisticalSummary::clear()
{
  m_minVal = "";
  m_maxVal = "";
  m_maxVal = "";
  m_count = 0;
  m_validCount = 0;
}