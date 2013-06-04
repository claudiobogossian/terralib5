/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file Histogram.cpp

  \brief A class to represent a histogram. 
*/

//terralib
#include "Histogram.h"
#include "../../../datatype.h"

te::qt::widgets::Histogram::Histogram()
{

}

te::qt::widgets::Histogram::~Histogram()
{
  if(m_histogramType ==  te::dt::DATETIME_TYPE || m_histogramType == te::dt::STRING_TYPE)
    delete m_StringValues;
  else
    delete m_values;
}

int& te::qt::widgets::Histogram::getType()
{
  return m_histogramType;
}

void te::qt::widgets::Histogram::setType(int new_type)
{
 m_histogramType = new_type;
}

std::map<double,  unsigned int>* te::qt::widgets::Histogram::getValues()
{
  return m_values;
}

std::map<double, std::vector<te::da::ObjectId*>> te::qt::widgets::Histogram::getValuesOIDs()
{
  return m_valuesOIDs;
}

std::map<std::string,  unsigned int>* te::qt::widgets::Histogram::getStringValues()
{
  return m_StringValues;
}

void te::qt::widgets::Histogram::setValues(std::map<double,  unsigned int>* new_values)
{
  m_values = new_values;
}

std::map<std::string, std::vector<te::da::ObjectId*>> te::qt::widgets::Histogram::getStringOIDs()
{
  return m_stringOIDs;
}

void te::qt::widgets::Histogram::setStringOIDs( std::map<std::string, std::vector<te::da::ObjectId*>> new_ids)
{
  m_stringOIDs = new_ids;
}

void te::qt::widgets::Histogram::setStringValues(std::map<std::string,  unsigned int>* new_values)
{
  m_StringValues = new_values;
}

void te::qt::widgets::Histogram::setValuesOIDs( std::map<double, std::vector<te::da::ObjectId*>> new_ids)
{
  m_valuesOIDs = new_ids;
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

std::set <std::string>& te::qt::widgets::Histogram::getStringInterval()
{
  return m_StringIntervals;
}

void te::qt::widgets::Histogram::setStringInterval( std::set <std::string> new_Interval)
{
  m_StringIntervals = new_Interval;
}

void te::qt::widgets::Histogram::add(std::pair<double, int> new_value)
{
  m_values->insert(new_value);
}

