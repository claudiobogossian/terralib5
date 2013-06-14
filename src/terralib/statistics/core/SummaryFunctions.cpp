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
  \file Aggregation.h

  \brief Statistical summary functions.
*/

//Terralib
#include "Config.h"
#include "Exception.h"
#include "SummaryFunctions.h"

// BOOST
#include <boost/lexical_cast.hpp>

void te::stat::getStringStatisticalSummary(std::vector<std::string>& values, te::stat::StringStatisticalSummary& ss)
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
}

void te::stat::getNumericStatisticalSummary(std::vector<double>& values, te::stat::NumericStatisticalSummary& ss)
{

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