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
  \file GroupingItem.cpp

  \brief This class contains information about a grouping item associated to a layer.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../se/Symbolizer.h"
#include "GroupingItem.h"


te::map::GroupingItem::GroupingItem(const std::string& from, const std::string& to)
  : m_from(from), m_to(to), m_count(0), m_title("")
{

}

te::map::GroupingItem::~GroupingItem()
{
  te::common::FreeContents(m_symbolizers);
  m_symbolizers.clear();
}

te::map::GroupingItem::GroupingItem(const GroupingItem& rhs)
  : m_from(rhs.m_from), m_to(rhs.m_to), m_value(rhs.m_value),
    m_count(rhs.m_count), m_title(rhs.m_title)
{
  for(size_t t = 0; t < rhs.m_symbolizers.size(); ++t)
  {
    m_symbolizers.push_back(rhs.m_symbolizers[t]->clone());
  }
}

te::map::GroupingItem& te::map::GroupingItem::operator=(const GroupingItem& rhs)
{
  if(this != &rhs)
  {
    m_from = rhs.m_from;
    m_to = rhs.m_to;
    m_value = rhs.m_value;
    m_count = rhs.m_count;
    m_title = rhs.m_title;

    for(size_t t = 0; t < rhs.m_symbolizers.size(); ++t)
    {
      m_symbolizers.push_back(rhs.m_symbolizers[t]->clone());
    }
  }

  return *this;
}

const std::string& te::map::GroupingItem::getValue() const
{
  return m_value;
}

void te::map::GroupingItem::setValue(const std::string& value)
{
  m_value = value;
}

const std::string& te::map::GroupingItem::getLowerLimit() const
{
  return m_from;
}

void te::map::GroupingItem::setLowerLimit(const std::string& from)
{
  m_from = from;
}

const std::string& te::map::GroupingItem::getUpperLimit() const
{
  return m_to;
}

void te::map::GroupingItem::setUpperLimit(const std::string& to)
{
  m_to = to;
}

std::string te::map::GroupingItem::getTitle()
{
  if(m_title.empty())
  {
    if(!m_value.empty())
      m_title = m_value;
    else if(!m_from.empty() && !m_to.empty())
      m_title = m_from + " - " + m_to;
    else 
      m_title = "No Value";
  }

  return m_title;
}

void te::map::GroupingItem::setTitle(const std::string& title)
{
  m_title = title;
}

std::size_t te::map::GroupingItem::getCount() const
{
  return m_count;
}

void te::map::GroupingItem::setCount(std::size_t count)
{
  m_count = count;
}

const std::vector<te::se::Symbolizer*>& te::map::GroupingItem::getSymbolizers() const
{
  return m_symbolizers;
}

void te::map::GroupingItem::setSymbolizers(const std::vector<te::se::Symbolizer*>& symbolizers)
{
  te::common::FreeContents(m_symbolizers);
  m_symbolizers.clear();

  m_symbolizers = symbolizers;
}
