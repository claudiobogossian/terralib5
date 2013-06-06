/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/dataset/Sequence.cpp

  \brief It describes a sequence (a number generator).
*/

// TerraLib
#include "../datasource/DataSourceCatalog.h"
#include "Sequence.h"

// STL
#include <climits>

te::da::core::Sequence::Sequence()
  : m_increment(1),
    m_minValue(INT_MIN),
    m_maxValue(INT_MAX),
    m_startValue(1),
    m_cachedValues(1),
    m_catalog(0),
    m_cycled(false)
{
}

te::da::core::Sequence::Sequence(const std::string& name,
                                 boost::int64_t increment,
                                 boost::int64_t startValue)
 : m_name(name),
   m_increment(increment),
   m_minValue(INT_MIN),
   m_maxValue(INT_MAX),
   m_startValue(startValue),
   m_cachedValues(1),
   m_catalog(0),
   m_cycled(false)
{
}

te::da::core::Sequence::Sequence(const Sequence& rhs)
  : m_name(rhs.m_name),
    m_ownedByDataSet(rhs.m_ownedByDataSet),
    m_increment(rhs.m_increment),
    m_minValue(rhs.m_minValue),
    m_maxValue(rhs.m_maxValue),
    m_startValue(rhs.m_startValue),
    m_cachedValues(rhs.m_cachedValues),
    m_catalog(0),
    m_ownedByPropertyPos(rhs.m_ownedByPropertyPos),
    m_cycled(rhs.m_cycled)

{
}

te::da::core::Sequence& te::da::core::Sequence::operator=(const Sequence& rhs)
{
  if(this != &rhs)
  {
    m_name = rhs.m_name;
    m_ownedByDataSet = rhs.m_ownedByDataSet;
    m_increment = rhs.m_increment;
    m_minValue = rhs.m_minValue;
    m_maxValue = rhs.m_maxValue;
    m_startValue = rhs.m_startValue;
    m_cachedValues = rhs.m_cachedValues;
    m_catalog = 0;
    m_ownedByPropertyPos = rhs.m_ownedByPropertyPos;
    m_cycled = rhs.m_cycled;
  }

  return *this;
}

