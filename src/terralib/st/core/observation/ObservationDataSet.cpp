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
  \file ObservationDataSet.cpp

  \brief This file contains a class to represent an observation data set. 
*/

//TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimePeriod.h"
#include "../../../datatype/DateTimeInstant.h"

//ST
#include "ObservationDataSet.h"
#include "Observation.h"

te::st::ObservationDataSet::ObservationDataSet(te::da::DataSet* ds, const ObservationDataSetType& obst) 
  : m_ds(ds),
    m_obst(obst),
    m_tpExtent(),
    m_spExtent()
{
}

te::st::ObservationDataSet::ObservationDataSet(te::da::DataSet* ds, const ObservationDataSetType& obst, 
                           te::dt::DateTimePeriod* text)
  : m_ds(ds),
    m_obst(obst),
    m_tpExtent(text)
{
}

te::st::ObservationDataSet::ObservationDataSet(te::da::DataSet* ds, const ObservationDataSetType& obst, 
                           te::dt::DateTimePeriod* text, const te::gm::Envelope& sext)
  : m_ds(ds),
    m_obst(obst),
    m_tpExtent(text),
    m_spExtent(sext)
{
}

te::st::ObservationDataSet::ObservationDataSet(te::da::DataSet* ds, int phTimeIdx, int obsPropIdx, 
                            int geomPropIdx)
  : m_ds(ds),
    m_obst(phTimeIdx, obsPropIdx, geomPropIdx),
    m_tpExtent(),
    m_spExtent()
{
}

te::st::ObservationDataSet::ObservationDataSet( te::da::DataSet* ds, const std::vector<int>& phTimeIdxs, 
                              const std::vector<int>& obsPropIdxs, int geomPropIdx)
  : m_ds(ds),
    m_obst(phTimeIdxs, obsPropIdxs, geomPropIdx),
    m_tpExtent(),
    m_spExtent()
{
}

te::st::ObservationDataSet::ObservationDataSet( te::da::DataSet* ds, int phTimeIdx, int obsPropIdx, 
              int geomPropIdx, te::dt::DateTimePeriod* text)
  : m_ds(ds),
    m_obst(phTimeIdx, obsPropIdx, geomPropIdx),
    m_tpExtent(text)
{
}

te::st::ObservationDataSet::ObservationDataSet( te::da::DataSet* ds, int phTimeIdx, int obsPropIdx, 
              int geomPropIdx, te::dt::DateTimePeriod* text, const te::gm::Envelope& sext)
  : m_ds(ds),
    m_obst(phTimeIdx, obsPropIdx, geomPropIdx),
    m_tpExtent(text),
    m_spExtent(sext)
{
}

te::st::ObservationDataSet::ObservationDataSet( te::da::DataSet* ds, const std::vector<int>& phTimeIdxs, 
              const std::vector<int>& obsPropIdxs, int geomPropIdx, te::dt::DateTimePeriod* text)
  : m_ds(ds),
    m_obst(phTimeIdxs, obsPropIdxs, geomPropIdx),
    m_tpExtent(text)
{
}

te::st::ObservationDataSet::ObservationDataSet( te::da::DataSet* ds, const std::vector<int>& phTimeIdxs, 
              const std::vector<int>& obsPropIdxs, int geomPropIdx, te::dt::DateTimePeriod* text, 
              const te::gm::Envelope& sext)
  : m_ds(ds),
    m_obst(phTimeIdxs, obsPropIdxs, geomPropIdx),
    m_tpExtent(text),
    m_spExtent(sext)
{
}

te::da::DataSet* te::st::ObservationDataSet::getData() const
{
  return m_ds.get();
}

std::auto_ptr<te::da::DataSet> te::st::ObservationDataSet::release()
{
  delete m_tpExtent.release();
  std::auto_ptr<te::da::DataSet> result(m_ds.release());
  return result;
}

const te::st::ObservationDataSetType& te::st::ObservationDataSet::getType() const
{
  return m_obst;
}

const te::gm::Envelope& te::st::ObservationDataSet::getSpatialExtent()
{
  if(m_spExtent.isValid())
    return m_spExtent;
  
  std::auto_ptr<te::gm::Envelope> env(m_ds->getExtent(m_obst.getGeomPropIdx()));
  m_spExtent = *(env.get());
  return m_spExtent;
}

void te::st::ObservationDataSet::setSpatialExtent(const te::gm::Envelope& ext)
{
  m_spExtent = ext;
}

te::dt::DateTimePeriod* te::st::ObservationDataSet::getTemporalExtent()
{
  return m_tpExtent.get();
}

void te::st::ObservationDataSet::setTemporalExtent(te::dt::DateTimePeriod* ext)
{
  m_tpExtent.reset(ext);
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
  std::auto_ptr<te::dt::DateTime> phTime(m_ds->getDateTime(m_obst.getBeginTimePropIdx()));
  std::auto_ptr<te::dt::DateTime> vlTime(0);
  std::auto_ptr<te::dt::DateTime> rsTime(0);

  if(m_obst.getVlBeginTimePropIdx()>-1)
    vlTime = m_ds->getDateTime(m_obst.getVlBeginTimePropIdx());

  if(m_obst.getRsTimePropIdx()>-1)
    rsTime = m_ds->getDateTime(m_obst.getRsTimePropIdx());

  boost::ptr_vector<te::dt::AbstractData> obsData;
  for(std::size_t i = 0; i<m_obst.getObsPropIdxs().size(); ++i)
    obsData.push_back(m_ds->getValue(m_obst.getObsPropIdxs()[i]));

  return std::auto_ptr<te::st::Observation>(new 
    Observation(phTime.release(),static_cast<te::dt::DateTimeInstant*>(rsTime.release()),
                static_cast<te::dt::DateTimePeriod*>(vlTime.release()),obsData));
}

te::st::ObservationDataSet::~ObservationDataSet()
{
}



