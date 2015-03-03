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

//Boost
#include <boost/lexical_cast.hpp>

std::string te::stat::GetStatSummaryShortName(const int& e)
{
  switch(e)
  {
    case te::stat::MIN_VALUE:
      return TE_TR("MIN_VALUE");
      
    case te::stat::MAX_VALUE:
      return TE_TR("MAX_VALUE");
    
    case te::stat::MEAN:
      return TE_TR("MEAN");
    
    case te::stat::SUM:
      return TE_TR("SUM");
    
    case te::stat::COUNT:
      return TE_TR("COUNT");
    
    case te::stat::VALID_COUNT:
      return TE_TR("VALID_COUNT");
    
    case te::stat::STANDARD_DEVIATION:
      return TE_TR("STANDARD_DEVIATION");
    
    case te::stat::VARIANCE:
      return TE_TR("VARIANCE");
    
    case te::stat::SKEWNESS:
      return TE_TR("SKEWNESS");
    
    case te::stat::KURTOSIS:
      return TE_TR("KURTOSIS");
    
    case te::stat::AMPLITUDE:
      return TE_TR("AMPLITUDE");
    
    case te::stat::MEDIAN:
      return TE_TR("MEDIAN");
    
    case te::stat::VAR_COEFF:
      return TE_TR("VAR_COEFF");
    
    case te::stat::MODE:
      return TE_TR("MODE");

    default:
      return ("");
  }
}

std::string te::stat::GetStatSummaryFullName(const int& e)
{
  switch(e)
  {
    case te::stat::MIN_VALUE:
      return TE_TR("Minimum value");
      
    case te::stat::MAX_VALUE:
      return TE_TR("Maximum value");
    
    case te::stat::MEAN:
      return TE_TR("Mean");
    
    case te::stat::SUM:
      return TE_TR("Sum of values");
    
    case te::stat::COUNT:
      return TE_TR("Total number of values");
    
    case te::stat::VALID_COUNT:
      return TE_TR("Total not null values");
    
    case te::stat::STANDARD_DEVIATION:
      return TE_TR("Standard deviation");
    
    case te::stat::VARIANCE:
      return TE_TR("Variance");
    
    case te::stat::SKEWNESS:
      return TE_TR("Skewness");
    
    case te::stat::KURTOSIS:
      return TE_TR("Kurtosis");
    
    case te::stat::AMPLITUDE:
      return TE_TR("Amplitude");
    
    case te::stat::MEDIAN:
      return TE_TR("Median");
    
    case te::stat::VAR_COEFF:
      return TE_TR("Coefficient variation");
    
    case te::stat::MODE:
      return TE_TR("Mode");

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
      else if(type ==  te::dt::NUMERIC_TYPE)
        numval = boost::lexical_cast<double>(dataSet->getNumeric(index));
      result.push_back(numval);
    }
  }while(dataSet->moveNext());

  return result;
}