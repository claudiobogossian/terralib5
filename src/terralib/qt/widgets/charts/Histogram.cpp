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
  \file Histogram.h

  \brief A class to represent a histogram. 
*/

//terralib
#include "Histogram.h"


te::qt::widgets::Histogram::Histogram()
{

}

te::qt::widgets::Histogram::Histogram(std::map<double, int> values, double minValue)
{
  m_values = values;
  m_minValue = minValue;
}

te::qt::widgets::Histogram::~Histogram()
{

}

std::map<double, int>& te::qt::widgets::Histogram::getValues()
{
  return m_values;
}

void te::qt::widgets::Histogram::setValues(std::map<double, int> new_values)
{
  m_values = new_values;
}

double& te::qt::widgets::Histogram::getMinValue()
{
  return m_minValue;
}
      
void te::qt::widgets::Histogram::setMinValue(double new_minValue)
{
  m_minValue = new_minValue;
}

double& te::qt::widgets::Histogram::getInterval()
{
  return m_interval;
}

void te::qt::widgets::Histogram::setInterval(double new_Interval)
{
  m_interval = new_Interval;
}

void te::qt::widgets::Histogram::add(std::pair<double, int> new_value)
{
  m_values.insert(new_value);
}

