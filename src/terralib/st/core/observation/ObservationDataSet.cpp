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
  \file ObservationDataSet.cpp

  \brief This file contains a class to represent an observation data set. 
*/

//TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimePeriod.h"
#include "../../../datatype/DateTimeInstant.h"
#include "../../../datatype/DateTimeProperty.h"
#include "../../../geometry/Utils.h"
#include "../../../geometry/GeometryProperty.h"

//ST
#include "ObservationDataSet.h"
#include "Observation.h"

te::st::ObservationDataSet::ObservationDataSet(te::da::DataSet* ds, const ObservationDataSetType& obst) 
  : m_ds(ds),
    m_obst(obst)
{
}

te::st::ObservationDataSet::ObservationDataSet(te::da::DataSet* ds, const ObservationDataSetType& obst, 
                           te::dt::DateTimePeriod* text)
  : m_ds(ds),
    m_obst(obst)
{
  m_obst.setTemporalExtent(text);
}

te::st::ObservationDataSet::ObservationDataSet(te::da::DataSet* ds, const ObservationDataSetType& obst, 
                           te::dt::DateTimePeriod* text, te::gm::Geometry* sext)
  : m_ds(ds),
    m_obst(obst)
{
  m_obst.setTemporalExtent(text);
  m_obst.setSpatialExtent(sext);
}

te::da::DataSet* te::st::ObservationDataSet::getData() const
{
  return m_ds.get();
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSet::release()
{
  std::auto_ptr<te::da::DataSet> result(m_ds.release());
  return result;
}

const te::st::ObservationDataSetType& te::st::ObservationDataSet::getType() const
{
  return m_obst;
}

const te::gm::Geometry* te::st::ObservationDataSet::getSpatialExtent()
{
  if(m_obst.hasSpatialExtent())
    return m_obst.getSpatialExtent(); 
  
  std::size_t idx = te::da::GetPropertyPos(m_ds.get(), m_obst.getGeomPropName());
  std::auto_ptr<te::gm::Envelope> env(m_ds->getExtent(idx));
  m_obst.setSpatialExtent(te::gm::GetGeomFromEnvelope(env.get(), -1));
  return m_obst.getSpatialExtent();
}

void te::st::ObservationDataSet::setSpatialExtent(te::gm::Geometry* ext)
{
  m_obst.setSpatialExtent(ext);
}

const te::dt::DateTimePeriod* te::st::ObservationDataSet::getTemporalExtent()
{
  return m_obst.getTemporalExtent(); 
}

void te::st::ObservationDataSet::setTemporalExtent(te::dt::DateTimePeriod* ext)
{
  m_obst.setTemporalExtent(ext); 
}

bool te::st::ObservationDataSet::moveNext()
{
  return m_ds->moveNext();
}

bool te::st::ObservationDataSet::movePrevious()
{
  return m_ds->movePrevious();
}

bool te::st::ObservationDataSet::moveFirst()
{
  return m_ds->moveFirst();
}

bool te::st::ObservationDataSet::moveBeforeFirst()
{
  return m_ds->moveBeforeFirst();
}

bool te::st::ObservationDataSet::moveLast()
{
  return m_ds->moveLast();
}

bool te::st::ObservationDataSet::isAtBegin() const
{
  return m_ds->isAtBegin();
}

bool te::st::ObservationDataSet::isBeforeBegin() const
{
  return m_ds->isBeforeBegin();
}

bool te::st::ObservationDataSet::isAtEnd() const
{
  return m_ds->isAtEnd();
}

bool te::st::ObservationDataSet::isAfterEnd() const
{
  return m_ds->isAfterEnd();
}

std::auto_ptr<te::st::Observation> te::st::ObservationDataSet::getObservation() const
{
  std::auto_ptr<te::dt::DateTime> phTime(m_ds->getDateTime(m_obst.getBeginTimePropName()));
  std::auto_ptr<te::dt::DateTime> vlTime(0);
  std::auto_ptr<te::dt::DateTime> rsTime(0);

  if(m_obst.hasVlTimeProp())
    vlTime = m_ds->getDateTime(m_obst.getVlBeginTimePropInfo()->getName());

  if(m_obst.hasRsTimeProp())
    rsTime = m_ds->getDateTime(m_obst.getRsTimePropInfo()->getName());

  boost::ptr_vector<te::dt::AbstractData> obsData;
  for(std::size_t i = 0; i<m_obst.getObsPropNames().size(); ++i)
    obsData.push_back(m_ds->getValue(m_obst.getObsPropNames()[i]));

  return std::auto_ptr<te::st::Observation>(new 
    Observation(phTime.release(),
                static_cast<te::dt::DateTimeInstant*>(rsTime.release()),
                static_cast<te::dt::DateTimePeriod*>(vlTime.release()),
                0, obsData));
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSet::getBeginTimeProperty() const
{
  return m_obst.getBeginTimePropInfo();
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSet::getEndTimeProperty() const
{
  return m_obst.getEndTimePropInfo();
}

const te::dt::DateTimeProperty* 
  te::st::ObservationDataSet::getVlBeginTimeProperty() const
{
  return m_obst.getVlBeginTimePropInfo();
}

const te::dt::DateTimeProperty* 
  te::st::ObservationDataSet::getVlEndTimeProperty() const
{
  return m_obst.getVlEndTimePropInfo();
}

const te::dt::DateTimeProperty* 
  te::st::ObservationDataSet::getRsTimeProperty() const
{
  return m_obst.getRsTimePropInfo();
}

te::st::ObservationDataSet::~ObservationDataSet()
{
}



