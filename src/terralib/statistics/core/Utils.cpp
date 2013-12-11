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
  \file terralib\statistics\core\Utils.cpp
   
  \brief Utility functions for Statistics.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "Config.h"
#include "Enums.h"
#include "Exception.h"
#include "Utils.h"

std::string te::stat::GetStatSummaryShortName(const int& e)
{
  switch(e)
  {
    case te::stat::MIN_VALUE:
      return TR_STATISTICS("MIN_VALUE");
      
    case te::stat::MAX_VALUE:
      return TR_STATISTICS("MAX_VALUE");
    
    case te::stat::MEAN:
      return TR_STATISTICS("MEAN");
    
    case te::stat::SUM:
      return TR_STATISTICS("SUM");
    
    case te::stat::COUNT:
      return TR_STATISTICS("COUNT");
    
    case te::stat::VALID_COUNT:
      return TR_STATISTICS("VALID_COUNT");
    
    case te::stat::STANDARD_DEVIATION:
      return TR_STATISTICS("STANDARD_DEVIATION");
    
    case te::stat::VARIANCE:
      return TR_STATISTICS("VARIANCE");
    
    case te::stat::SKEWNESS:
      return TR_STATISTICS("SKEWNESS");
    
    case te::stat::KURTOSIS:
      return TR_STATISTICS("KURTOSIS");
    
    case te::stat::AMPLITUDE:
      return TR_STATISTICS("AMPLITUDE");
    
    case te::stat::MEDIAN:
      return TR_STATISTICS("MEDIAN");
    
    case te::stat::VAR_COEFF:
      return TR_STATISTICS("VAR_COEFF");
    
    case te::stat::MODE:
      return TR_STATISTICS("MODE");

    default:
      return ("");
  }
}

std::string te::stat::GetStatSummaryFullName(const int& e)
{
  switch(e)
  {
    case te::stat::MIN_VALUE:
      return TR_STATISTICS("Minimum value");
      
    case te::stat::MAX_VALUE:
      return TR_STATISTICS("Maximum value");
    
    case te::stat::MEAN:
      return TR_STATISTICS("Mean");
    
    case te::stat::SUM:
      return TR_STATISTICS("Sum of values");
    
    case te::stat::COUNT:
      return TR_STATISTICS("Total number of values");
    
    case te::stat::VALID_COUNT:
      return TR_STATISTICS("Total not null values");
    
    case te::stat::STANDARD_DEVIATION:
      return TR_STATISTICS("Standard deviation");
    
    case te::stat::VARIANCE:
      return TR_STATISTICS("Variance");
    
    case te::stat::SKEWNESS:
      return TR_STATISTICS("Skewness");
    
    case te::stat::KURTOSIS:
      return TR_STATISTICS("Kurtosis");
    
    case te::stat::AMPLITUDE:
      return TR_STATISTICS("Amplitude");
    
    case te::stat::MEDIAN:
      return TR_STATISTICS("Median");
    
    case te::stat::VAR_COEFF:
      return TR_STATISTICS("Coefficient variation");
    
    case te::stat::MODE:
      return TR_STATISTICS("Mode");

    default:
      return ("");
  }
}

std::vector<std::string> te::stat::GetStringData(te::da::DataSet* dataSet, const std::string propName)
{
  std::vector<std::string> result;
  std::string value="";

  dataSet->moveFirst();

  do
  {
    if (!dataSet->isNull(propName))
    {
      value = dataSet->getString(propName);
      result.push_back(value);
    }
  }while(dataSet->moveNext());

  return result;
}

std::vector<double> te::stat::GetNumericData(te::da::DataSet* dataSet, const std::string propName)
{
  std::vector<double> result;
  double numval;

  size_t index=0;
  for (index=0; index<dataSet->getNumProperties(); ++index)
    if (dataSet->getPropertyName(index) == propName)
      break;
  
  std::size_t type = dataSet->getPropertyDataType(index);
  dataSet->moveFirst();
  do
  {
    if (!dataSet->isNull(propName))
    {
      if (type == te::dt::INT16_TYPE)
        numval = dataSet->getInt16(index);
      else if (type == te::dt::INT32_TYPE)
        numval = dataSet->getInt32(index);
      else if (type == te::dt::INT64_TYPE)
        numval = (double)dataSet->getInt64(index);
      else if (type == te::dt::FLOAT_TYPE)
        numval = dataSet->getFloat(index);
      else if (type == te::dt::DOUBLE_TYPE)
        numval = dataSet->getDouble(index);
      result.push_back(numval);
    }
  }while(dataSet->moveNext());

  return result;
}