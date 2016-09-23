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
  \file terralib/common/StringUtils.cpp

  \brief Utility functions for dealing with strings.

  \ingroup common
*/

#include "StringUtils.h"

#include <sstream>

std::vector<std::string> te::common::SplitString(const std::string& str, const char& delimiter)
{
  std::stringstream ss (str);
  std::string item;

  std::vector<std::string> elements;

  while (std::getline(ss, item, delimiter))
  {
    elements.push_back(item);
  }

  return elements;
}
