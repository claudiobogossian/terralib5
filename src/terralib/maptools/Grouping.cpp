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
#include "../common/STLUtils.h"
#include "../datatype/Enums.h"
#include "Grouping.h"
#include "GroupingItem.h"


te::map::Grouping::Grouping(const std::string& propertyName, te::map::GroupingType type, size_t precision)
  : m_propertyName(propertyName),
    m_type(type),
    m_precision(precision),
    m_isVisible(true),
    m_summary("AVERAGE")
{
  m_propertyType = te::dt::UNKNOWN_TYPE;
}

te::map::Grouping::Grouping(const Grouping& rhs)
  : m_propertyName(rhs.m_propertyName),
    m_type((te::map::GroupingType)rhs.m_propertyType),
    m_precision(rhs.m_precision),
    m_isVisible(true),
    m_summary(rhs.m_summary)
{
  setNumSlices(rhs.m_numSlices);
  setStdDeviation(rhs.m_stdDeviation);

  std::vector<te::map::GroupingItem*> items;
  for(std::size_t i = 0; i < rhs.m_items.size(); ++i)
  {
    te::map::GroupingItem* item = new te::map::GroupingItem(*rhs.m_items[i]);
    items.push_back(item);
  }

  setGroupingItems(items);
}

te::map::Grouping::~Grouping()
{
  te::common::FreeContents(m_items);
}

std::string te::map::Grouping::getPropertyName() const
{
  return m_propertyName;
}


void te::map::Grouping::setPropertyName(const std::string& name)
{
  m_propertyName = name;
}

int te::map::Grouping::getPropertyType() const
{
  return m_propertyType;
}

void te::map::Grouping::setPropertyType(const int& type)
{
  m_propertyType = type;
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

const double te::map::Grouping::getStdDeviation() const
{
  return m_stdDeviation;
}

void te::map::Grouping::setStdDeviation(double stdDeviation)
{
  m_stdDeviation = stdDeviation;
}

const std::vector<te::map::GroupingItem*>& te::map::Grouping::getGroupingItems() const
{
  return m_items;
}

void te::map::Grouping::setGroupingItems(std::vector<te::map::GroupingItem*>& items)
{
  m_items = items;

  m_numSlices = m_items.size();
}

bool te::map::Grouping::isVisible() const
{
  return m_isVisible;
}

void te::map::Grouping::setVisibility(bool visible)
{
  m_isVisible = visible;
}

std::string te::map::Grouping::getSummary() const
{
  return m_summary;
}

void te::map::Grouping::setSummary(const std::string& summary)
{
  m_summary = summary;
}
