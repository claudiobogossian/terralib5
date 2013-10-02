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
  \file ObservationDataSetType.cpp

  \brief This file contains a class called ObservationDataSetType
*/

//ST
#include "ObservationDataSetType.h"

te::st::ObservationDataSetType::ObservationDataSetType(int phTimeIdx, int obsPropIdx, int geomPropIdx)
  : m_phTimePropIdx(2,-1),
    m_geomPropIdx(geomPropIdx),
    m_validTimePropIdx(2,-1),
    m_resultTimePropIdx(-1)
{  
  m_observedPropIdx.push_back(obsPropIdx);
  m_phTimePropIdx[0]=phTimeIdx;
}

te::st::ObservationDataSetType::ObservationDataSetType(int phTimeIdx, const std::vector<int>& obsPropIdx, int geomPropIdx)
  : m_phTimePropIdx(2,-1),
    m_observedPropIdx(obsPropIdx),
    m_geomPropIdx(geomPropIdx),
    m_validTimePropIdx(2,-1),
    m_resultTimePropIdx(-1)
{  
  m_phTimePropIdx[0]=phTimeIdx;
}

te::st::ObservationDataSetType::ObservationDataSetType(const std::vector<int>& phTimeIdx, 
                      const std::vector<int>& obsPropIdx, int geomPropIdx)
  : m_phTimePropIdx(2,-1),
    m_observedPropIdx(obsPropIdx),
    m_geomPropIdx(geomPropIdx),
    m_validTimePropIdx(2,-1),
    m_resultTimePropIdx(-1)
{
  for(std::size_t i=0; i<phTimeIdx.size(); i++)
    m_phTimePropIdx[i]=phTimeIdx[i];
}

te::st::ObservationDataSetType::ObservationDataSetType(const std::vector<int>& phTimeIdx, const std::vector<int>& obsPropIdx, 
                                int geomPropIdx, const std::vector<int>& valTimeIdx, int resTimeIdx)
  : m_phTimePropIdx(2,-1),
    m_observedPropIdx(obsPropIdx),
    m_geomPropIdx(geomPropIdx),
    m_validTimePropIdx(2,-1),
    m_resultTimePropIdx(resTimeIdx)
{  
  for(std::size_t i=0; i<phTimeIdx.size(); i++)
    m_phTimePropIdx[i]=phTimeIdx[i];

  for(std::size_t i=0; i<valTimeIdx.size(); i++)
    m_validTimePropIdx[i]=valTimeIdx[i];
}

const std::vector<int>& te::st::ObservationDataSetType::getTimePropIdxs() const
{
  return m_phTimePropIdx;
}

int te::st::ObservationDataSetType::getBeginTimePropIdx() const
{
  return m_phTimePropIdx[0];
}

int te::st::ObservationDataSetType::getEndTimePropIdx() const
{
  return m_phTimePropIdx[1];
}

void te::st::ObservationDataSetType::setTimePropIdxs(const std::vector<int>& idxs)
{
  for(std::size_t i=0; i<idxs.size(); i++)
    m_phTimePropIdx[i]=idxs[i];
}

void te::st::ObservationDataSetType::setBeginTimePropIdx(int idx)
{
  m_phTimePropIdx[0]=idx;
}

void te::st::ObservationDataSetType::setEndTimePropIdx(int idx)
{
  m_phTimePropIdx[1]=idx;
}

const std::vector<int>& te::st::ObservationDataSetType::getObsPropIdxs() const
{
  return m_observedPropIdx;
}

void te::st::ObservationDataSetType::setObsPropIdxs(const std::vector<int>& props)
{
  m_observedPropIdx = props;
}

int te::st::ObservationDataSetType::getGeomPropIdx() const
{
  return m_geomPropIdx;
}

void te::st::ObservationDataSetType::setGeomPropIdx(int idx)
{
  m_geomPropIdx = idx;
}

int te::st::ObservationDataSetType::getVlBeginTimePropIdx() const
{
  return m_validTimePropIdx[0];
}

int te::st::ObservationDataSetType::getVlEndTimePropIdx() const
{
  return m_validTimePropIdx[1];
}      

const std::vector<int>& te::st::ObservationDataSetType::getVlTimePropIdxs() const
{
  return m_validTimePropIdx;
}

void te::st::ObservationDataSetType::setVlBeginTimePropIdx(int idx)
{
  m_validTimePropIdx[0]=idx;
}

void te::st::ObservationDataSetType::setVlEndTimePropIdx(int idx)
{
  m_validTimePropIdx[1]=idx;
}

void te::st::ObservationDataSetType::setVlTimePropIdxs(const std::vector<int>& idxs)
{
  for(std::size_t i=0; i<idxs.size(); i++)
    m_validTimePropIdx[i]=idxs[i];
}

int te::st::ObservationDataSetType::getRsTimePropIdx() const
{
  return m_resultTimePropIdx;
}

void te::st::ObservationDataSetType::setRsTimePropIdx(int idx)
{
  m_resultTimePropIdx = idx;
} 

