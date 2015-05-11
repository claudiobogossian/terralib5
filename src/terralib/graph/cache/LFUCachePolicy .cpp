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
  \file LFUCachePolicy.cpp

  \brief This class is used to implement the LFU cache policy.
*/

#include "LFUCachePolicy.h"

#include <limits>

te::graph::LFUCachePolicy::LFUCachePolicy()
{
}

te::graph::LFUCachePolicy::~LFUCachePolicy()
{
  m_LFU.clear();
}

void te::graph::LFUCachePolicy::added(int value)
{
  m_LFU.insert(std::map<int, double>::value_type(value, 1.));
}

void te::graph::LFUCachePolicy::update(int value)
{
  accessed(value);
}

void te::graph::LFUCachePolicy::toRemove(int& value)
{
  std::map<int, double>::iterator itLFU = m_LFU.begin();
  std::map<int, double>::iterator itMin = m_LFU.end();

  double minValue = std::numeric_limits<double>::max();

  while(itLFU != m_LFU.end())
  {
    if(itLFU->second < minValue)
    {
      minValue = itLFU->second;
      itMin = itLFU;
    }
    ++itLFU;
  }

  if(itMin != m_LFU.end())
  {
    value = itMin->first;
    m_LFU.erase(itMin);
  }
}

void te::graph::LFUCachePolicy::accessed(int value)
{
  std::map<int, double>::iterator itLFU = m_LFU.begin();

  while(itLFU != m_LFU.end())
  {
    if(itLFU->first == value)
    {
      itLFU->second += 1.;
      break;
    }
    ++itLFU;
  }
}


