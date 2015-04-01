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
  \file TimeSeriesDataSet.cpp

  \brief This file contains a class to represent a time series data set.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimePeriod.h"
#include "../../../datatype/DateTimeInstant.h"
#include "../../../datatype/SimpleData.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Utils.h"

//ST
#include "TimeSeriesDataSet.h"
#include "TimeSeries.h"
#include "../observation/ObservationDataSet.h"
#include "../observation/ObservationDataSetType.h"

te::st::TimeSeriesDataSet::TimeSeriesDataSet(te::da::DataSet* ds, const ObservationDataSetType& type,
  const std::vector<std::string>& propNames)
: m_obsDs(new ObservationDataSet(ds, type)),
  m_vlPropNames(propNames),
  m_id("")
{  
}

te::st::TimeSeriesDataSet::TimeSeriesDataSet(te::da::DataSet* ds, const ObservationDataSetType& type,
  const std::vector<std::string>& propNames, const std::string& id)
: m_obsDs(new ObservationDataSet(ds, type)),
  m_vlPropNames(propNames),
  m_id(id)
{  
}

te::st::TimeSeriesDataSet::TimeSeriesDataSet( ObservationDataSet* obs, const std::vector<std::string>& propNames,
  const std::string& id)
  : m_obsDs(obs),
    m_vlPropNames(propNames),
    m_id(id)
{  
}

te::st::ObservationDataSet* te::st::TimeSeriesDataSet::getObservationSet() const
{
  return m_obsDs.get();
}

const std::vector<std::string>& te::st::TimeSeriesDataSet::getValuePropNames() const
{
  return m_vlPropNames;
}

void te::st::TimeSeriesDataSet::setValuePropNames(const std::vector<std::string>& n)
{
  m_vlPropNames = n;
}

std::string te::st::TimeSeriesDataSet::getId() const
{
  return m_id;
}       

void te::st::TimeSeriesDataSet::setId(const std::string& id)
{
  m_id = id;
} 

std::size_t te::st::TimeSeriesDataSet::size() const
{
  return m_obsDs->getData()->size();
}

bool te::st::TimeSeriesDataSet::moveNext()
{
  return m_obsDs->moveNext();
}

bool te::st::TimeSeriesDataSet::movePrevious()
{
  return m_obsDs->movePrevious();
}

bool te::st::TimeSeriesDataSet::moveFirst()
{
  return m_obsDs->moveFirst();
}

bool te::st::TimeSeriesDataSet::moveBeforeFirst()
{
  return m_obsDs->moveBeforeFirst();
}

bool te::st::TimeSeriesDataSet::moveLast()
{
  return m_obsDs->moveLast();
}

bool te::st::TimeSeriesDataSet::isAtBegin() const
{
  return m_obsDs->isAtBegin();
}

bool te::st::TimeSeriesDataSet::isBeforeBegin() const
{
  return m_obsDs->isBeforeBegin();
}

bool te::st::TimeSeriesDataSet::isAtEnd() const
{
  return m_obsDs->isAtEnd();
}

bool te::st::TimeSeriesDataSet::isAfterEnd() const
{
  return m_obsDs->isAfterEnd();
}

std::auto_ptr<te::dt::DateTime> te::st::TimeSeriesDataSet::getTime() const
{
  //TO DO: arrumar pro caso quando for period dividido em duas colunas
  std::string phTimePropName = m_obsDs->getType().getBeginTimePropName();
  return std::auto_ptr<te::dt::DateTime>(m_obsDs->getData()->getDateTime(phTimePropName));
}

std::auto_ptr<te::dt::AbstractData> te::st::TimeSeriesDataSet::getValue(std::size_t idx) const
{
  return std::auto_ptr<te::dt::AbstractData>(m_obsDs->getData()->getValue(idx));
}

std::auto_ptr<te::dt::AbstractData> te::st::TimeSeriesDataSet::getValue() const
{
  return std::auto_ptr<te::dt::AbstractData>(m_obsDs->getData()->getValue(m_vlPropNames[0]));
}

double te::st::TimeSeriesDataSet::getDouble(std::size_t idx) const
{
  return m_obsDs->getData()->getDouble(idx);
}

double te::st::TimeSeriesDataSet::getDouble() const
{
  return m_obsDs->getData()->getDouble(m_vlPropNames[0]);
}

int te::st::TimeSeriesDataSet::getInt(std::size_t idx) const
{
  return m_obsDs->getData()->getInt32(idx);
}

int te::st::TimeSeriesDataSet::getInt() const
{
  return m_obsDs->getData()->getInt32(m_vlPropNames[0]);
}

std::auto_ptr<te::gm::Geometry> te::st::TimeSeriesDataSet::getGeometry() const
{
  if(m_obsDs->getType().hasGeomProp())
    return std::auto_ptr<te::gm::Geometry>(m_obsDs->getData()->getGeometry(m_obsDs->getType().getGeomPropName()));
  else if (m_obsDs->getType().hasGeometry())
    return std::auto_ptr<te::gm::Geometry>(dynamic_cast<te::gm::Geometry*>(m_obsDs->getType().getGeometry()->clone()));
  return std::auto_ptr<te::gm::Geometry>();
}

const te::dt::DateTimePeriod* te::st::TimeSeriesDataSet::getTemporalExtent() const
{
  return m_obsDs->getType().getTemporalExtent();
}

std::auto_ptr<te::st::TimeSeries> te::st::TimeSeriesDataSet::getTimeSeries(te::st::AbstractTimeSeriesInterp* interp)
{
  return std::auto_ptr<te::st::TimeSeries>(getTimeSeries(m_vlPropNames[0],interp));
}

std::auto_ptr<te::st::TimeSeries> 
te::st::TimeSeriesDataSet::getTimeSeries(const std::string& propN, te::st::AbstractTimeSeriesInterp* interp)
{
  std::auto_ptr<te::st::TimeSeries> result(new TimeSeries(interp,m_id));
  te::da::DataSet* ds = m_obsDs->getData();
  
  if(ds->moveNext())
  {
    //Get the time series location if there is one
    std::auto_ptr<te::gm::Geometry> geom = getGeometry();
    if(geom.get()!=0)
      result->setLocation(geom.release());
    
    //Get time and value of time series
    std::auto_ptr<te::dt::DateTime> time(ds->getDateTime(m_obsDs->getType().getBeginTimePropName()));
    std::auto_ptr<te::dt::AbstractData> value(ds->getValue(propN));
    result->add(time.release(), value.release());
  }

  while(ds->moveNext())
  {
    //Get time and value of time series
    std::auto_ptr<te::dt::DateTime> time(ds->getDateTime(m_obsDs->getType().getBeginTimePropName()));
    std::auto_ptr<te::dt::AbstractData> value(ds->getValue(propN));
    result->add(time.release(), value.release());
  }
  return result;
}

void te::st::TimeSeriesDataSet::getTimeSeriesSet(  te::st::AbstractTimeSeriesInterp* interp, 
                                std::vector<te::st::TimeSeries*>& result)
{
  std::size_t sz = m_vlPropNames.size();
  for(unsigned int i = 0; i<sz; ++i)
  {
    TimeSeries* ts = new TimeSeries(interp,m_id);
    result.push_back(ts);
  }
  
  te::da::DataSet* ds = m_obsDs->getData();
  
  if(ds->moveNext())
  {
    for(unsigned int i = 0; i<sz; ++i)
    {
      //Get the time series location if there is one
      std::auto_ptr<te::gm::Geometry> geom = getGeometry();
      if(geom.get()!=0)
        result[i]->setLocation(geom.release());
      
      //Get time and value of time series
      std::auto_ptr<te::dt::DateTime> time(ds->getDateTime(m_obsDs->getType().getBeginTimePropName()));
      std::auto_ptr<te::dt::AbstractData> value(ds->getValue(m_vlPropNames[i]));
      result[i]->add(time.release(), value.release());
    }
  }

  while(ds->moveNext())
  {
    for(unsigned int i = 0; i<sz; ++i)
    {
      //Get time and value of time series
      std::auto_ptr<te::dt::DateTime> time(ds->getDateTime(m_obsDs->getType().getBeginTimePropName()));
      std::auto_ptr<te::dt::AbstractData> value(ds->getValue(m_vlPropNames[i]));
      result[i]->add(time.release(), value.release());
    }
  }
  return;
}

std::auto_ptr<te::da::DataSet> te::st::TimeSeriesDataSet::release()
{
  std::auto_ptr<te::da::DataSet> result(m_obsDs->release());
  return result;
}

te::st::TimeSeriesDataSet::~TimeSeriesDataSet()
{
}




