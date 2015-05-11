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
  \file terralib/classification/Utils.cpp

  \brief Utility functions for Classification.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"
#include "Enums.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <math.h>

double te::cl::GetEuclideanDistance(std::vector<double> v1, std::vector<double> v2)
{
  assert(v1.size() == v2.size());

  double distance = 0.0;
  double v1minusv2;
  for (unsigned int i = 0; i < v1.size(); i++)
  {
    v1minusv2 = v1[i] - v2[i];
    distance += v1minusv2 * v1minusv2;
  }
  if (distance < 0)
    distance = 0.0;

  return sqrt(distance);
}
