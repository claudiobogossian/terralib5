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
  \file terralib/maptools/Chart.cpp

  \brief This class represents the informations needed to build map charts.
*/

// TerraLib
#include "../se/Utils.h"
#include "Chart.h"

// STL
#include <cassert>
#include <cmath>
#include <cstdlib>

te::map::Chart::Chart(ChartType type, const std::vector<std::string>& properties)
  : m_type(type),
    m_properties(properties),
    m_contourColor(te::color::RGBAColor(0, 0, 0, TE_OPAQUE)),
    m_contourWidth(1),
    m_height(24),
    m_barWidth(16),
    m_isVisible(true),
    m_maxValue(0.0),
    m_avoidConflicts(true)
{
  assert(!properties.empty());

  // Generates random colors to each property
  for(std::size_t i = 0; i < m_properties.size(); ++i)
  {
    te::color::RGBAColor color(rand() % 255, rand() % 255, rand() % 255, TE_OPAQUE);
    m_colors.push_back(color);
  }
}

te::map::Chart::Chart(ChartType type, const std::vector<std::string>& properties, const std::vector<te::color::RGBAColor>& colors)
  : m_type(type),
    m_properties(properties),
    m_colors(colors),
    m_contourColor(te::color::RGBAColor(0, 0, 0, TE_OPAQUE)),
    m_contourWidth(1),
    m_height(24),
    m_barWidth(16),
    m_isVisible(true),
    m_maxValue(0.0),
    m_avoidConflicts(true)
{
  assert(!properties.empty());
  assert(properties.size() == colors.size());
}

te::map::Chart::~Chart()
{
}

te::map::ChartType te::map::Chart::getType() const
{
  return m_type;
}

const std::vector<std::string>& te::map::Chart::getProperties() const
{
  return m_properties;
}

const te::color::RGBAColor& te::map::Chart::getColor(std::size_t i) const
{
   assert(i < m_colors.size());

   return m_colors[i];
}

void te::map::Chart::setColor(std::size_t i, const te::color::RGBAColor& color)
{
  assert(i < m_colors.size());

  m_colors[i] = color;
}

const te::color::RGBAColor& te::map::Chart::getContourColor() const
{
  return m_contourColor;
}

void te::map::Chart::setContourColor(const te::color::RGBAColor& color)
{
  m_contourColor = color;
}

std::size_t te::map::Chart::getContourWidth() const
{
  return m_contourWidth;
}

void te::map::Chart::setContourWidth(std::size_t width)
{
  m_contourWidth = width;
}

std::size_t te::map::Chart::getHeight() const
{
  return m_height;
}

void te::map::Chart::setHeight(std::size_t height)
{
  m_height = height;
}

std::size_t te::map::Chart::getWidth() const
{
  // TODO: need review! Cause: dynamic chart sizes...
  switch(m_type)
  {
    case Pie:
      return m_height;
    
    case Bar:
      return m_barWidth * m_properties.size();
  }

  return 0;
}

std::size_t te::map::Chart::getBarWidth() const
{
  return m_barWidth;
}

void te::map::Chart::setBarWidth(std::size_t width)
{
  m_barWidth = width;
}

void te::map::Chart::setMaxValue(double value)
{
  m_maxValue = value;
}

double te::map::Chart::getMaxValue() const
{
  return m_maxValue;
}

bool te::map::Chart::isVisible() const
{
  return m_isVisible;
}

void te::map::Chart::setVisibility(bool visible)
{
  m_isVisible = visible;
}

void te::map::Chart::setAvoidConflicts(bool on)
{
  m_avoidConflicts = on;
}

bool te::map::Chart::getAvoidConflicts() const
{
  return m_avoidConflicts;
}
