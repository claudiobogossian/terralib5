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
#include "NumericStatisticalComplexSummary.h"

te::stat::NumericStatisticalComplexSummary::NumericStatisticalComplexSummary()
  : m_minVal(0,0),
    m_maxVal(0,0),
    m_mean(0,0),
    m_sum(0,0),
    m_count(0),
    m_validCount(0),
    m_stdDeviation(0,0),
    m_kernel(0,0),
    m_variance(0,0),
    m_skewness(0,0),
    m_kurtosis(0,0),
    m_amplitude(0,0),
    m_median(0,0),
    m_varCoeff(0,0)
{
}

te::stat::NumericStatisticalComplexSummary::NumericStatisticalComplexSummary(const NumericStatisticalComplexSummary& rhs)
  : m_minVal(0,0),
    m_maxVal(0,0),
    m_mean(0,0),
    m_sum(0,0),
    m_count(0),
    m_validCount(0),
    m_stdDeviation(0,0),
    m_kernel(0,0),
    m_variance(0,0),
    m_skewness(0,0),
    m_kurtosis(0,0),
    m_amplitude(0,0),
    m_median(0,0),
    m_varCoeff(0,0)
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
  m_percentEachClass = rhs.m_percentEachClass;
}

te::stat::NumericStatisticalComplexSummary::~NumericStatisticalComplexSummary()
{
  clear();
}

te::stat::NumericStatisticalComplexSummary& te::stat::NumericStatisticalComplexSummary::operator=(const NumericStatisticalComplexSummary& rhs)
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
    m_percentEachClass = rhs.m_percentEachClass;
  }

  return *this;
}

void te::stat::NumericStatisticalComplexSummary::clear()
{
  m_minVal = std::complex<double> (0,0);
  m_maxVal = std::complex<double> (0,0);
  m_mean = std::complex<double> (0,0);
  m_sum = std::complex<double> (0,0);
  m_count = 0;
  m_validCount = 0;
  m_stdDeviation = std::complex<double> (0,0);
  m_kernel = std::complex<double> (0,0);
  m_variance = std::complex<double> (0,0);
  m_skewness = std::complex<double> (0,0);
  m_kurtosis = std::complex<double> (0,0);
  m_amplitude = std::complex<double> (0,0);
  m_median = std::complex<double> (0,0);
  m_varCoeff = std::complex<double> (0,0);
  m_mode.clear();
  m_percentEachClass.clear();
}
