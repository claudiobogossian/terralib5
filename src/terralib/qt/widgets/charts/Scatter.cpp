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
  \file Scatter.cpp

  \brief A class to represent a scatter. 
*/

#include "Scatter.h"

#include <limits>

te::qt::widgets::Scatter::Scatter(const std::vector<double>& axisX, const std::vector<double>& axisY)
  : m_xValues(axisX),
    m_yValues(axisY),
    m_minX(std::numeric_limits<double>::max()),
    m_maxX(-std::numeric_limits<double>::max()),
    m_minY(std::numeric_limits<double>::max()),
    m_maxY(-std::numeric_limits<double>::max())
{
  
}

te::qt::widgets::Scatter::Scatter()
  : m_minX(std::numeric_limits<double>::max()),
  m_maxX(-std::numeric_limits<double>::max()),
  m_minY(std::numeric_limits<double>::max()),
  m_maxY(-std::numeric_limits<double>::max())
{
}

void te::qt::widgets::Scatter::calculateMinMaxValues()
{
  std::vector<double>::const_iterator itx = m_xValues.begin();
  std::vector<double>::const_iterator ity = m_yValues.begin();
  while(itx!=m_xValues.end())
  {
    if(*itx < m_minX)
      m_minX = *itx;
    if(*itx > m_maxX)
      m_maxX = *itx; 
    ++itx;
  }

  while(ity!=m_yValues.end())
  {
    if(*ity < m_minY)
      m_minY = *ity;
    if(*ity > m_maxY)
      m_maxY = *ity; 
    ++ity;
  }
}

std::size_t te::qt::widgets::Scatter::size()
{
  return m_xValues.size(); // equal to m_yValues.size();
}

double te::qt::widgets::Scatter::getX(unsigned int idx)
{
  return m_xValues[idx];
}

double* te::qt::widgets::Scatter::getX()
{
  return &m_xValues[0];
}

double te::qt::widgets::Scatter::getY(unsigned int idx)
{
  return m_yValues[idx];
}

double* te::qt::widgets::Scatter::getY()
{
  return &m_yValues[0];
}

std::vector<std::string>& te::qt::widgets::Scatter::getXString()
{
  return m_xString;
}

std::vector<std::string>& te::qt::widgets::Scatter::getYString()
{
  return m_yString;
}

double te::qt::widgets::Scatter::getMinX()
{
  return m_minX;
}

double te::qt::widgets::Scatter::getMaxX()
{
  return m_maxX;
}

double te::qt::widgets::Scatter::getMinY()
{
  return m_minY;
}

double te::qt::widgets::Scatter::getMaxY()
{
  return m_maxY;
}

void te::qt::widgets::Scatter::setMinX(double& new_minX)
{
   m_minX = new_minX;
}

void te::qt::widgets::Scatter::setMaxX(double& new_maxX)
{
  m_maxX = new_maxX;
}

void te::qt::widgets::Scatter::setMinY(double& new_minY)
{
  m_minY = new_minY;
}

void te::qt::widgets::Scatter::setMaxY(double& new_maxY)
{
  m_maxY = new_maxY;
}

void te::qt::widgets::Scatter::addX(double& xValue)
{
  m_xValues.push_back(xValue);
}

void te::qt::widgets::Scatter::addY(double& yValue)
{
  m_yValues.push_back(yValue);
}

te::qt::widgets::Scatter::~Scatter()
{
  
}

