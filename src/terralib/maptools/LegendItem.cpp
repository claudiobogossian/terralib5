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
  \file LegendItem.cpp

  \brief This class contains information about a legend item associated to a layer.
*/

// TerraLib
#include "LegendItem.h"


te::map::LegendItem::LegendItem(const std::string& from, const std::string& to)
  : m_from(from), m_to(to),m_count(0), m_title("")
{
  m_color.setColor(0, 0, 0, 255);
}

te::map::LegendItem::~LegendItem()
{  
}

te::map::LegendItem::LegendItem(const LegendItem& rhs)
  : m_from(rhs.m_from), m_to(rhs.m_to), m_count(rhs.m_count),
    m_title(rhs.m_title), m_color(rhs.m_color)
{
}

te::map::LegendItem& te::map::LegendItem::operator=(const LegendItem& rhs)
{
  if(this != &rhs)
  {
    m_from = rhs.m_from;
    m_to = rhs.m_to;
    m_count = rhs.m_count;
    m_title = rhs.m_title;
    m_color = rhs.m_color;
  }

  return *this;
}

const std::string& te::map::LegendItem::getLowerLimit() const
{
  return m_from;
}

void te::map::LegendItem::setLowerLimit(const std::string& from)
{
  m_from = from;
}

const std::string& te::map::LegendItem::getUpperLimit() const
{
  return m_to;
}

void te::map::LegendItem::setUpperLimit(const std::string& to)
{
  m_to = to;
}

std::string te::map::LegendItem::getTitle()
{
  if(m_title.empty() == true)
  {
    if(m_from.empty() == false && m_to.empty() == false)
      m_title = m_from + " - " + m_to;
    else if(m_to.empty() == true)
      m_title = m_from;
  }

  return m_title;
}

void te::map::LegendItem::setTitle(const std::string& title)
{
  m_title = title;
}

std::size_t te::map::LegendItem::getCount() const
{
  return m_count;
}

void te::map::LegendItem::setCount(std::size_t count)
{
  m_count = count;
}

const te::color::RGBAColor& te::map::LegendItem::getColor() const
{
  return m_color;
}

void te::map::LegendItem::setColor(const te::color::RGBAColor& color)
{
  m_color = color;
}
