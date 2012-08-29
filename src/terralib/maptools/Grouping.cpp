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
  \file Grouping.cpp

  \brief This class contains the parameters needed for grouping the values of a Property.
*/

// TerraLib
#include "Grouping.h"


te::map::Grouping::Grouping(te::dt::Property* p, te::map::GroupingType type, size_t precision)
  : m_property(p), m_type(type), m_precision(precision)
{
}

te::map::Grouping::~Grouping()
{  
}

const te::map::GroupingType te::map::Grouping::getType() const
{
  return m_type;
}

void te::map::Grouping::setType(GroupingType type)
{
  m_type = type;
}

const size_t te::map::Grouping::getPrecision() const
{
  return m_precision;
}

void te::map::Grouping::setPrecision(size_t precision)
{
  m_precision = precision;
}

const size_t te::map::Grouping::getNumSlices() const
{
  return m_numSlices;
}

void te::map::Grouping::setNumSlices(size_t numSlices)
{
  m_numSlices = numSlices;
}

const float te::map::Grouping::getStdDeviation() const
{
  return m_stdDeviation;
}

void te::map::Grouping::setStdDeviation(float stdDeviation)
{
  m_stdDeviation = stdDeviation;
}
