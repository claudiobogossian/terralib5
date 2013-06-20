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
  \file SummaryFunctions.cpp

  \brief Statistical summary functions.
*/

//Terralib
#include "Config.h"
#include "Exception.h"
#include "SummaryFunctions.h"

// BOOST
#include <boost/lexical_cast.hpp>

//STL
#include <numeric>

void te::stat::GetStringStatisticalSummary(std::vector<std::string>& values, te::stat::StringStatisticalSummary& ss)
{
  std::sort(values.begin(), values.end());
  
  ss.m_minVal = *values.begin();
  ss.m_maxVal = values[values.size() - 1];

  ss.m_count = values.size();

  for(std::size_t i = 0; i < values.size(); ++i)
  {
    if(!values[i].empty())
    {
      ++ss.m_validCount;
    }
  }

  ss.m_mode = Mode(values);
}

void te::stat::GetNumericStatisticalSummary(std::vector<double>& values, te::stat::NumericStatisticalSummary& ss)
{
  std::sort(values.begin(), values.end());

  ss.m_minVal = *values.begin();
  ss.m_maxVal = values[values.size() - 1];
  ss.m_count = values.size();
  ss.m_validCount = values.size();
  
  int init = 0;
  ss.m_sum = std::accumulate(values.begin(), values.end(), init);

  ss.m_mean = ss.m_sum/ss.m_count;

  for(int i = 0; i < ss.m_count; ++i)
  {
    double v= values[i];
    ss.m_variance += pow((v-ss.m_mean),2); 
    ss.m_skewness += pow((v-ss.m_mean),3);   
    ss.m_kurtosis += pow((v-ss.m_mean),4); 
  }

  ss.m_variance /= ss.m_count; 
  ss.m_stdDeviation = pow(ss.m_variance, 0.5); 
  ss.m_skewness /= ss.m_count;
  ss.m_skewness /= pow(ss.m_stdDeviation, 3); 
  ss.m_kurtosis /= ss.m_count;
  ss.m_kurtosis /= pow(ss.m_stdDeviation, 4); 

  ss.m_varCoeff = (100* ss.m_stdDeviation) / ss.m_mean;
  ss.m_amplitude = ss.m_maxVal - ss.m_minVal;

  if((ss.m_count % 2) == 0)
    ss.m_median = (values[(ss.m_count/2)] + values[(ss.m_count/2-1)])/2;
  else
    ss.m_median = values[(ss.m_count-1)/2];


  ss.m_mode = Mode(values);
}

double te::stat::Mode(const std::vector<double>& values)
{
  bool found;
  double mode = 0.0;
  std::map<double, int> mapMode;

  for(std::size_t i = 0; i < values.size(); ++i)
  {
    found = false;

    if(!mapMode.empty())
    {
      std::map<double, int>::iterator itMode = mapMode.begin();

      while(itMode != mapMode.end())
      {
        if(itMode->first == values[i])
        {
          ++itMode->second;
          found = true;
        }
        
        ++itMode;
      }
      if(found == false)
      {
        mapMode.insert( std::map<double, int>::value_type( values[i] , 1 ) );
      }
    }
    else
      mapMode.insert( std::map<double, int>::value_type( values[i] , 1 ) );
  }

  std::map<double, int>::iterator itMode = mapMode.begin();
  int repeat = 0;

  while(itMode != mapMode.end())
  {
    if(repeat < itMode->second)
    {
      repeat = itMode->second;
      mode = itMode->first;
    }

    ++itMode;
  }

  return mode;
}

std::string te::stat::Mode(const std::vector<std::string>& values)
{
  bool found;
  std::string mode = "";
  std::map<std::string, int> mapMode;

  for(std::size_t i = 0; i < values.size(); ++i)
  {
    found = false;

    if(!mapMode.empty())
    {
      std::map<std::string, int>::iterator itMode = mapMode.begin();

      while(itMode != mapMode.end())
      {
        if(itMode->first == values[i])
        {
          ++itMode->second;
          found = true;
        }
        
        ++itMode;
      }
      if(found == false)
      {
        mapMode.insert( std::map<std::string, int>::value_type( values[i] , 1 ) );
      }
    }
    else
      mapMode.insert( std::map<std::string, int>::value_type( values[i] , 1 ) );
  }

  std::map<std::string, int>::iterator itMode = mapMode.begin();
  int repeat = 0;

  while(itMode != mapMode.end())
  {
    if(repeat < itMode->second)
    {
      repeat = itMode->second;
      mode = itMode->first;
    }

    ++itMode;
  }

  return mode;
}