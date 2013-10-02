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
  \file TimeSeriesDataSetType.cpp

  \brief This file contains a class called TimeSeriesDataSetType 
*/

//ST
#include "TimeSeriesDataSetType.h"
#include "../observation/ObservationDataSetType.h"

te::st::TimeSeriesDataSetType::TimeSeriesDataSetType(int tPropIdx, int vPropIdx, int gPropIdx, 
                              int idPropIdx, const std::string& id)
  : m_obsType(tPropIdx, vPropIdx, gPropIdx), 
    m_vlPropIdxs(1,vPropIdx),
    m_idPropIdx(idPropIdx),
    m_id(id)
{    
}

te::st::TimeSeriesDataSetType::TimeSeriesDataSetType(const std::vector<int>& tPropIdxs, 
                            const std::vector<int>& vPropIdxs, 
                            int gPropIdx, int idPropIdx, const std::string& id)
  : m_obsType(tPropIdxs, vPropIdxs, gPropIdx), 
    m_vlPropIdxs(vPropIdxs),
    m_idPropIdx(idPropIdx),
    m_id(id)
{    
}

te::st::TimeSeriesDataSetType::TimeSeriesDataSetType(const ObservationDataSetType& type, 
                              const std::vector<int>& vPropIdxs,
                              int idPropIdx, const std::string& id)
  : m_obsType(type), 
    m_vlPropIdxs(vPropIdxs),
    m_idPropIdx(idPropIdx),
    m_id(id)
{    
}

const te::st::ObservationDataSetType& 
te::st::TimeSeriesDataSetType::getType() const
{
  return m_obsType;
}

int te::st::TimeSeriesDataSetType::getTimePropIdx() const
{
  return m_obsType.getBeginTimePropIdx(); 
}

int te::st::TimeSeriesDataSetType::getBeginTimePropIdx() const
{
  return m_obsType.getBeginTimePropIdx(); 
}

int te::st::TimeSeriesDataSetType::getEndTimePropIdx() const
{
  return m_obsType.getEndTimePropIdx(); 
}

const std::vector<int>& 
te::st::TimeSeriesDataSetType::getValuePropIdx() const
{
  return m_vlPropIdxs;
}

int te::st::TimeSeriesDataSetType::getGeomPropIdx() const
{
  return m_obsType.getGeomPropIdx();
}

int te::st::TimeSeriesDataSetType::getIdPropIdx() const
{
  return m_idPropIdx;
}

void te::st::TimeSeriesDataSetType::setIdPropIdx(int idx)
{
  m_idPropIdx=idx;
}

std::string te::st::TimeSeriesDataSetType::getId() const
{
  return m_id;
}

void te::st::TimeSeriesDataSetType::setId(const std::string& id)
{
  m_id = id;
}

te::st::TimeSeriesDataSetType::~TimeSeriesDataSetType()
{
}

