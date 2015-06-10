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
  \file terralib\attributefill\core\Utils.cpp
   
  \brief Utility functions for Attribute Fill.
*/

// TerraLib
#include "../common/Translator.h"
#include "Config.h"
#include "Enums.h"
#include "Utils.h"

std::string te::attributefill::GetOperationFullName(const int& e)
{
  switch(e)
  {
    case te::attributefill::VALUE:
      return "Value";
    case te::attributefill::MIN_VALUE:
      return "Minimum value";
    case te::attributefill::MAX_VALUE:
      return "Maximum value";
    case te::attributefill::MEAN:
      return "Mean";
    case te::attributefill::SUM:
      return "Sum of values";
    case te::attributefill::COUNT:
      return "Total number of values";
    case te::attributefill::VALID_COUNT:
      return "Total not null values";
    case te::attributefill::STANDARD_DEVIATION:
      return "Standard deviation";
    case te::attributefill::VARIANCE:
      return "Variance";
    case te::attributefill::SKEWNESS:
      return "Skewness";
    case te::attributefill::KURTOSIS:
      return "Kurtosis";
    case te::attributefill::AMPLITUDE:
      return "Amplitude";
    case te::attributefill::MEDIAN:
      return "Median";
    case te::attributefill::VAR_COEFF:
      return "Coefficient variation";
    case te::attributefill::MODE:
      return "Mode";
    case te::attributefill::HIGHEST_OCCURRENCE:
      return "Class with highest occurrence";
    case te::attributefill::HIGHEST_INTERSECTION:
      return "Class with highest intersection area";
    case te::attributefill::PERCENT_CLASS:
      return "Percentage per Class";
    case te::attributefill::MIN_DISTANCE:
      return "Minimum Distance";
    case te::attributefill::PRESENCE:
      return "Presence";
    case te::attributefill::WEIGHTED:
      return "Weighted by Area";
    case te::attributefill::WEIGHTED_SUM:
      return "Weighted Sum by Area";
    case te::attributefill::PERCENT_EACH_CLASS:
      return "Percentage of each Class by Area";
    case te::attributefill::PERCENT_TOTAL_AREA:
      return "Percentage of Total Area";
    default:
      return "";
  }
}