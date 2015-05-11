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
  \file GroupingAlgorithms.cpp
  
  \brief This file contains functions containing the algorithms for grouping values.
 */

// TerraLib
#include "../common/StringUtils.h"
#include "../datatype/Enums.h"
#include "GroupingAlgorithms.h"

// STL
#include <algorithm>
#include <stdlib.h>

// Boost
#include <boost/cstdint.hpp>


void te::map::GroupingByUniqueValues(std::vector<std::string>& inputValues, int dataType,
                                     std::vector<te::map::GroupingItem*>& legend, int precision)
{
  size_t i, j;

  size_t valSize = inputValues.size();
    
  // Sort the input values
  if (dataType == te::dt::INT16_TYPE || dataType == te::dt::UINT16_TYPE ||
      dataType == te::dt::INT32_TYPE || dataType == te::dt::UINT32_TYPE ||
      dataType == te::dt::INT64_TYPE || dataType == te::dt::UINT64_TYPE)
  {
    std::vector<boost::int64_t> v;

    for(i = 0; i < valSize; ++i)
      v.push_back(atoi(inputValues[i].c_str()));

    sort(v.begin(), v.end());

    for (i = 0; i < v.size(); ++i)
      inputValues[i] = te::common::Convert2String(v[i]);
  }
  else if(dataType == te::dt::FLOAT_TYPE || dataType == te::dt::DOUBLE_TYPE)
  {
    std::vector<double> v;

    for (i = 0; i < valSize; ++i)
    {
      double a = atof(inputValues[i].c_str());
      v.push_back(a);
    }

    stable_sort(v.begin(), v.end());

    for (i = 0; i < v.size(); ++i)
      inputValues[i] = te::common::Convert2String(v[i], precision);
  }
  else
  {
    sort(inputValues.begin(), inputValues.end());
  }

  // Check the elements that are equal, incrementing
  // the count variable associated to each one
  int count = 1;
  te::map::GroupingItem* legendItem;

  for (i = 0, j = 1; i < valSize - 1 && j < valSize; ++i, ++j)
  {
    if (inputValues[i] == inputValues[j])
      ++count;
    else
    {
      legendItem = new te::map::GroupingItem;
      legendItem->setValue(inputValues[i]);
      legendItem->setCount(count);
      legend.push_back(legendItem);
      count = 1;
    }
  }

   if((i > 1) && (inputValues[i] == inputValues[i-1])) 
   {
     legendItem = new te::map::GroupingItem;
     legendItem->setValue(inputValues[i]);
     legendItem->setCount(count);
     legend.push_back(legendItem);
   }
   else
   {
     legendItem = new te::map::GroupingItem;
     legendItem->setValue(inputValues[i]);
     legendItem->setCount(1);
     legend.push_back(legendItem);
   } 
}

double te::map::AdjustToPrecision(double val, int precision, bool reduce)
{
  double p = pow(10.0, (double)-precision);
  
  if (reduce)
    return (val - p);

  return (val + p);
}
