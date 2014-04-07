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
  \file TrajectoryDataSet.cpp

  \brief This file contains a class to represent a trajectory data set.
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
#include "TrajectoryDataSet.h"
#include "Trajectory.h"
#include "../observation/ObservationDataSet.h"
#include "../observation/ObservationDataSetType.h"
#include "../interpolator/NearestGeometryAtTimeInterp.h"

te::st::TrajectoryDataSet::TrajectoryDataSet(te::da::DataSet* ds, int tPropIdx, int gPropIdx, 
                                             int idPropIdx, const std::string& id)
: m_obsDs(new ObservationDataSet(ds, tPropIdx, gPropIdx, gPropIdx)),
  m_type(tPropIdx, gPropIdx, idPropIdx, id),
  m_id(id)
{
  
}

te::st::TrajectoryDataSet::TrajectoryDataSet(te::da::DataSet* ds, int tPropIdx, int gPropIdx, 
                                             int idPropIdx, const std::string& id, 
                                             te::dt::DateTimePeriod* text, const te::gm::Envelope& sext)
: m_obsDs(new ObservationDataSet(ds, tPropIdx, gPropIdx, gPropIdx, text, sext)),
  m_type(tPropIdx, gPropIdx, idPropIdx, id),
  m_id(id)
{
  
}

te::st::TrajectoryDataSet::TrajectoryDataSet(te::da::DataSet* ds, const std::vector<int>& tPropIdxs, 
        int gPropIdx, int idPropIdx, const std::string& id)
: m_obsDs(new ObservationDataSet(ds, tPropIdxs, std::vector<int>(1,gPropIdx), gPropIdx)),
  m_type(tPropIdxs, gPropIdx, idPropIdx, id),
  m_id(id)
{
  
}

te::st::TrajectoryDataSet::TrajectoryDataSet(te::da::DataSet* ds, const std::vector<int>& tPropIdxs, 
        int gPropIdx, int idPropIdx, const std::string& id, te::dt::DateTimePeriod* text, 
        const te::gm::Envelope& sext)
: m_obsDs(new ObservationDataSet(ds, tPropIdxs, std::vector<int>(1,gPropIdx), gPropIdx, text, sext)),
  m_type(tPropIdxs, gPropIdx, idPropIdx, id),
  m_id(id)
{
  
}

te::st::TrajectoryDataSet::TrajectoryDataSet(te::da::DataSet* ds, const TrajectoryDataSetType& type,
                                             te::dt::DateTimePeriod* text, const te::gm::Envelope& sext)
: m_obsDs(new ObservationDataSet(ds, type.getType(), text, sext)),
  m_type(type),
  m_id(type.getId())
{
  
}

te::st::TrajectoryDataSet::TrajectoryDataSet( ObservationDataSet* obs, 
                                              const TrajectoryDataSetType& type, const std::string& id)
  : m_obsDs(obs),
    m_type(type),
    m_id(id)
{  
}

te::st::ObservationDataSet* te::st::TrajectoryDataSet::getObservationSet() const
{
  return m_obsDs.get();
}

const te::st::TrajectoryDataSetType& te::st::TrajectoryDataSet::getType() const
{
  return m_type;
}

std::string te::st::TrajectoryDataSet::getId() const
{
  return m_id;
}       

void te::st::TrajectoryDataSet::setId(const std::string& id)
{
  m_id = id;
} 

std::size_t te::st::TrajectoryDataSet::size() const
{
  return m_obsDs->getData()->size();
}

bool te::st::TrajectoryDataSet::moveNext()
{
  return m_obsDs->moveNext();
}

bool te::st::TrajectoryDataSet::movePrevious()
{
  return m_obsDs->movePrevious();
}

bool te::st::TrajectoryDataSet::moveFirst()
{
  return m_obsDs->moveFirst();
}

bool te::st::TrajectoryDataSet::moveBeforeFirst()
{
  return m_obsDs->moveBeforeFirst();
}

bool te::st::TrajectoryDataSet::moveLast()
{
  return m_obsDs->moveLast();
}

bool te::st::TrajectoryDataSet::isAtBegin() const
{
  return m_obsDs->isAtBegin();
}

bool te::st::TrajectoryDataSet::isBeforeBegin() const
{
  return m_obsDs->isBeforeBegin();
}

bool te::st::TrajectoryDataSet::isAtEnd() const
{
  return m_obsDs->isAtEnd();
}

bool te::st::TrajectoryDataSet::isAfterEnd() const
{
  return m_obsDs->isAfterEnd();
}

std::auto_ptr<te::gm::Geometry> te::st::TrajectoryDataSet::getGeometry() const
{
  int geomPropIdx = m_type.getGeomPropIdx();
  return std::auto_ptr<te::gm::Geometry>(m_obsDs->getData()->getGeometry(geomPropIdx));
}

std::auto_ptr<te::dt::DateTime> te::st::TrajectoryDataSet::getTime() const
{
  //TO DO: arrumar pro caso quando for period dividido em duas colunas
  int phTimePropIdx = m_type.getBeginTimePropIdx();
  return std::auto_ptr<te::dt::DateTime>(m_obsDs->getData()->getDateTime(phTimePropIdx));
}

te::dt::DateTimePeriod* te::st::TrajectoryDataSet::getTemporalExtent() const
{
  return m_obsDs->getTemporalExtent();
}

const te::gm::Envelope& te::st::TrajectoryDataSet::getSpatialExtent() const
{
  return m_obsDs->getSpatialExtent();
}

std::auto_ptr<te::st::Trajectory> te::st::TrajectoryDataSet::getTrajectory(te::st::AbstractTrajectoryInterp* interp)
{
  Trajectory* result = new Trajectory(interp,m_id);
  te::da::DataSet* ds = m_obsDs->getData();
  while(ds->moveNext())
  {
    std::auto_ptr<te::dt::DateTime> time(ds->getDateTime(m_type.getBeginTimePropIdx()));
    std::auto_ptr<te::gm::Geometry> geom(ds->getGeometry(m_type.getGeomPropIdx()));
    result->add(time.release(), geom.release());
  }
  return std::auto_ptr<te::st::Trajectory>(result);
}

std::auto_ptr<te::st::Trajectory> te::st::TrajectoryDataSet::getTrajectory()
{
  return getTrajectory(&NearestGeometryAtTimeInterp::getInstance()); 
}

std::auto_ptr<te::da::DataSet> te::st::TrajectoryDataSet::release()
{
  std::auto_ptr<te::da::DataSet> result(m_obsDs->release());
  return result;
}

te::st::TrajectoryDataSet::~TrajectoryDataSet()
{
}




