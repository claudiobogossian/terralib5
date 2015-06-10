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
  \file terralib/statistics/NumericStatisticalSummary.cpp
 
  \brief A struct that can be used to keep the summary of statistics.
*/

// TerraLib
#include "NumericStatisticalSummary.h"

te::stat::NumericStatisticalSummary::NumericStatisticalSummary()
  : m_minVal(0),
    m_maxVal(0),
    m_mean(0),
    m_sum(0),
    m_count(0),
    m_validCount(0),
    m_stdDeviation(0),
    m_kernel(0),
    m_variance(0),
    m_skewness(0),
    m_kurtosis(0),
    m_amplitude(0),
    m_median(0),
    m_varCoeff(0)
{
}

te::stat::NumericStatisticalSummary::NumericStatisticalSummary(const NumericStatisticalSummary& rhs)
  : m_minVal(0),
    m_maxVal(0),
    m_mean(0),
    m_sum(0),
    m_count(0),
    m_validCount(0),
    m_stdDeviation(0),
    m_kernel(0),
    m_variance(0),
    m_skewness(0),
    m_kurtosis(0),
    m_amplitude(0),
    m_median(0),
    m_varCoeff(0)
{
  m_minVal = rhs.m_minVal;
  m_maxVal = rhs.m_maxVal;
  m_mean = rhs.m_mean;
  m_sum = rhs.m_sum;
  m_count = rhs.m_count;
  m_validCount = rhs.m_validCount;
  m_stdDeviation = rhs.m_stdDeviation;
  m_kernel = rhs.m_kernel;
  m_variance = rhs.m_variance;
  m_skewness = rhs.m_skewness;
  m_kurtosis = rhs.m_kurtosis;
  m_amplitude = rhs.m_amplitude;
  m_median = rhs.m_median;
  m_varCoeff = rhs.m_varCoeff;
  m_mode.resize(rhs.m_mode.size());
  std::copy(rhs.m_mode.begin(), rhs.m_mode.end(), m_mode.begin());
}

te::stat::NumericStatisticalSummary::~NumericStatisticalSummary()
{
  clear();
}

te::stat::NumericStatisticalSummary& te::stat::NumericStatisticalSummary::operator=(const NumericStatisticalSummary& rhs)
{
  if(this != &rhs)
  {
    clear();

    m_minVal = rhs.m_minVal;
    m_maxVal = rhs.m_maxVal;
    m_mean = rhs.m_mean;
    m_sum = rhs.m_sum;
    m_count = rhs.m_count;
    m_validCount = rhs.m_validCount;
    m_stdDeviation = rhs.m_stdDeviation;
    m_kernel = rhs.m_kernel;
    m_variance = rhs.m_variance;
    m_skewness = rhs.m_skewness;
    m_kurtosis = rhs.m_kurtosis;
    m_amplitude = rhs.m_amplitude;
    m_median = rhs.m_median;
    m_varCoeff = rhs.m_varCoeff;
    m_mode.resize(rhs.m_mode.size());
    std::copy(rhs.m_mode.begin(), rhs.m_mode.end(), m_mode.begin());
  }

  return *this;
}

void te::stat::NumericStatisticalSummary::clear()
{
  m_minVal = 0;
  m_maxVal = 0;
  m_mean = 0;
  m_sum = 0;
  m_count = 0;
  m_validCount = 0;
  m_stdDeviation = 0;
  m_kernel = 0;
  m_variance = 0;
  m_skewness = 0;
  m_kurtosis = 0;
  m_amplitude = 0;
  m_median = 0;
  m_varCoeff = 0;
  m_mode.clear();
}

