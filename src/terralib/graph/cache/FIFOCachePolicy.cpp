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

  \brief This class is used to implement the LRU cache policy.
*/

#include "FIFOCachePolicy.h"

te::graph::FIFOCachePolicy::FIFOCachePolicy()
{
}

te::graph::FIFOCachePolicy::~FIFOCachePolicy()
{
  m_FIFO.clear();
}

void te::graph::FIFOCachePolicy::added(int value)
{
  m_FIFO.push_back(value);
}

void te::graph::FIFOCachePolicy::update(int value)
{
  std::vector<int>::iterator itFIFO = m_FIFO.begin();

  while(itFIFO != m_FIFO.end())
  {
    if(*itFIFO == value)
    {
      m_FIFO.erase(itFIFO);
      m_FIFO.push_back(value);
      break;
    }
    ++itFIFO;
  }
}

void te::graph::FIFOCachePolicy::toRemove(int& value)
{
  value = *m_FIFO.begin();

  m_FIFO.erase(m_FIFO.begin());
}

void te::graph::FIFOCachePolicy::accessed(int value)
{
}
