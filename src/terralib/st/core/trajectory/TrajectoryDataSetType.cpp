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
  \file TrajectoryDataSetType.cpp

  \brief This file contains a class called TrajectoryDataSetType 
*/

//ST
#include "TrajectoryDataSetType.h"
#include "../observation/ObservationDataSetType.h"

te::st::TrajectoryDataSetType::TrajectoryDataSetType(int tPropIdx, int gPropIdx, int idPropIdx, 
                            const std::string& id)
  : m_obsType(tPropIdx, gPropIdx, gPropIdx), 
    m_idPropIdx(idPropIdx),
    m_id(id)
{    
}

te::st::TrajectoryDataSetType::TrajectoryDataSetType(const std::vector<int>& tPropIdxs, int gPropIdx, 
                            int idPropIdx, const std::string& id)
  : m_obsType(tPropIdxs, std::vector<int>(1,gPropIdx), gPropIdx), 
    m_idPropIdx(idPropIdx),
    m_id(id)
{    
}

te::st::TrajectoryDataSetType::TrajectoryDataSetType(const ObservationDataSetType& type, 
                            int idPropIdx, const std::string& id)
  : m_obsType(type), 
    m_idPropIdx(idPropIdx),
    m_id(id)
{    
}

const te::st::ObservationDataSetType& 
te::st::TrajectoryDataSetType::getType() const
{
  return m_obsType;
}

int te::st::TrajectoryDataSetType::getGeomPropIdx() const
{
  return m_obsType.getGeomPropIdx();
}

int te::st::TrajectoryDataSetType::getTimePropIdx() const
{
  return m_obsType.getBeginTimePropIdx(); 
}

int te::st::TrajectoryDataSetType::getBeginTimePropIdx() const
{
  return m_obsType.getBeginTimePropIdx(); 
}

int te::st::TrajectoryDataSetType::getEndTimePropIdx() const
{
  return m_obsType.getEndTimePropIdx(); 
}

int te::st::TrajectoryDataSetType::getIdPropIdx() const
{
  return m_idPropIdx;
}

void te::st::TrajectoryDataSetType::setIdPropIdx(int idx)
{
  m_idPropIdx=idx;
}


std::string te::st::TrajectoryDataSetType::getId() const
{
  return m_id;
}

void te::st::TrajectoryDataSetType::setId(const std::string& id)
{
  m_id = id;
}

te::st::TrajectoryDataSetType::~TrajectoryDataSetType()
{
}

