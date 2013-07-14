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
#include "../../../dataaccess2/dataset/ObjectIdSet.h"

te::qt::widgets::Histogram::Histogram()
{

}

te::qt::widgets::Histogram::~Histogram()
{
  HistogramValues::iterator it = m_values.begin();
  while(it != m_values.end())
  {
    delete it->first;
    ++it;
  }

  te::qt::widgets::IntervalToObjectIdSet::iterator it2= m_valuesOids.begin();
  while(it2 != m_valuesOids.end())
  {
    delete it2->oid;
    ++it2;
  }
}

int& te::qt::widgets::Histogram::getType()
{
  return m_histogramType;
}

void te::qt::widgets::Histogram::setType(int new_type)
{
 m_histogramType = new_type;
}

std::map<double, unsigned int> te::qt::widgets::Histogram::getValues()
{ 
  std::map<double, unsigned int> res;

  //Casting every interval into a double value
  for(HistogramValues::iterator it = m_values.begin(); it != m_values.end(); ++it)
    res.insert(std::make_pair(static_cast<te::dt::Double*>(it->first)->getValue(), it->second));

  return res;
}

std::map<std::string, unsigned int> te::qt::widgets::Histogram::getStringValues()
{ 
  std::map<std::string, unsigned int> res;
  //Casting every interval into a string value
  for(HistogramValues::iterator it = m_values.begin(); it != m_values.end(); ++it) 
    res.insert(std::make_pair(it->first->toString(), it->second));
  return res;
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

void te::qt::widgets::Histogram::insert(std::pair<te::dt::AbstractData*, unsigned int> new_value, std::vector<te::da::ObjectId*> valuesOIds)
{
  m_values.insert(new_value);
  adjustOids(new_value.first, valuesOIds);
}

void te::qt::widgets::Histogram::insert(std::pair<te::dt::AbstractData*, unsigned int> new_value)
{
  m_values.insert(new_value);
}

te::da::ObjectIdSet* te::qt::widgets::Histogram::find(te::dt::AbstractData* interval)
{
  te::qt::widgets::IntervalToObjectIdSet::nth_index<0>::type::iterator it0, it1;
  IntervalToObjectId aux(interval, 0);

  tie(it0, it1) = m_valuesOids.equal_range(aux);

  te::da::ObjectIdSet* oids = new te::da::ObjectIdSet;

  while(it0 != it1) 
  {
    te::da::ObjectId* oid = new te::da::ObjectId(); 

    for(boost::ptr_vector<te::dt::AbstractData>::const_iterator it = it0->oid->getValue().begin(); it != it0->oid->getValue().end(); ++it)
    {
      oid->addValue((it)->clone());
    }

    oids->add(oid);
    ++it0;
  }
  return oids;
}

const te::dt::AbstractData* te::qt::widgets::Histogram::find(const te::da::ObjectId* oid)
{
  te::qt::widgets::IntervalToObjectIdSet::nth_index<1>::type::iterator it= m_valuesOids.get<1>().find(oid->getValueAsString());
  te::dt::AbstractData* interval = it->interval;
  return interval;
}

void te::qt::widgets::Histogram::adjustOids(te::dt::AbstractData* interval, std::vector<te::da::ObjectId*> valuesOIds)
{
  for(size_t i = 0; i < valuesOIds.size(); ++i)
  {
    m_valuesOids.insert(te::qt::widgets::IntervalToObjectId(interval, valuesOIds.at(i)));
  }
}