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
  \file ObservationDataSetInfo.cpp

  \brief This file contains a class called ObservationDataSetInfo
*/

//TerraLib
#include "../../../common/StringUtils.h"

//ST
#include "../observation/ObservationDataSetType.h"
#include "ObservationDataSetInfo.h"

te::st::ObservationDataSetInfo::ObservationDataSetInfo(const te::da::DataSourceInfo& dsInfo, const std::string& dsName, 
                                int phTimePropIdx, int obsPropIdx, int gPropIdx)
  : m_dsInfo(dsInfo),
    m_dsName(dsName),
    m_phTimePropIdx(2,-1),
    m_geomPropIdx(gPropIdx)
{  
  m_observedPropIdx.push_back(obsPropIdx);
  m_phTimePropIdx[0]=phTimePropIdx;
}

te::st::ObservationDataSetInfo::ObservationDataSetInfo(const te::da::DataSourceInfo& dsinfo, 
                                const std::string& dsName, 
                                const std::vector<int>& phTimeIdx, 
                                const std::vector<int>& obsPropIdx, int gPropIdx)
  : m_dsInfo(dsinfo),
    m_dsName(dsName),
    m_phTimePropIdx(2,-1),
    m_observedPropIdx(obsPropIdx),
    m_geomPropIdx(gPropIdx)
{  
  for(std::size_t i=0; i<phTimeIdx.size(); i++)
    m_phTimePropIdx[i]=phTimeIdx[i];
}

te::st::ObservationDataSetInfo::ObservationDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                                const std::vector<int>& phTimeIdx, const std::vector<int>& obsPropIdx, 
                                const std::vector<int>& valTimeIdx, int resTimeIdx, int gPropIdx)
  : m_dsInfo(dsinfo),
    m_dsName(dsName),
    m_phTimePropIdx(2,-1),
    m_observedPropIdx(obsPropIdx),
    m_geomPropIdx(gPropIdx),
    m_validTimePropIdx(2,-1),
    m_resultTimePropIdx(resTimeIdx)
{  
  for(std::size_t i=0; i<phTimeIdx.size(); i++)
    m_phTimePropIdx[i]=phTimeIdx[i];
  
  for(std::size_t i=0; i<valTimeIdx.size(); i++)
    m_validTimePropIdx[i]=valTimeIdx[i];
}

const te::da::DataSourceInfo& te::st::ObservationDataSetInfo::getDataSourceInfo() const
{
  return m_dsInfo;
}

std::string te::st::ObservationDataSetInfo::getDataSetName() const
{
  return m_dsName;
}

void te::st::ObservationDataSetInfo::setDataSetName(const std::string& name )
{
  m_dsName = name;
}

const std::vector<int>& te::st::ObservationDataSetInfo::getTimePropIdxs() const
{
  return m_phTimePropIdx;
}

int te::st::ObservationDataSetInfo::getBeginTimePropIdx() const
{
  return m_phTimePropIdx[0];
}

int te::st::ObservationDataSetInfo::getEndTimePropIdx() const
{
  return m_phTimePropIdx[1];
}

void te::st::ObservationDataSetInfo::setTimePropIdxs(const std::vector<int>& idxs)
{
  for(std::size_t i=0; i<idxs.size(); i++)
    m_phTimePropIdx[i]=idxs[i];
}

void te::st::ObservationDataSetInfo::setBeginTimePropIdx(int idx)
{
  m_phTimePropIdx[0]=idx;
}

void te::st::ObservationDataSetInfo::setEndTimePropIdx(int idx)
{
  m_phTimePropIdx[1]=idx;
}

const std::vector<int>& te::st::ObservationDataSetInfo::getObsPropIdxs() const
{
  return m_observedPropIdx;
}

void te::st::ObservationDataSetInfo::setObsPropIdxs(const std::vector<int>& props, int gPropIdx)
{
  m_observedPropIdx = props;
  m_geomPropIdx = gPropIdx;
}

int te::st::ObservationDataSetInfo::getGeomPropIdx() const
{
  return m_geomPropIdx;
}

int te::st::ObservationDataSetInfo::getVlBeginTimePropIdx() const
{
  return m_validTimePropIdx[0];
}

int te::st::ObservationDataSetInfo::getVlEndTimePropIdx() const
{
  return m_validTimePropIdx[1];
}      

const std::vector<int>& te::st::ObservationDataSetInfo::getVlTimePropIdxs() const
{
  return m_validTimePropIdx;
}

void te::st::ObservationDataSetInfo::setVlBeginTimePropIdx(int idx)
{
  m_validTimePropIdx[0]=idx;
}

void te::st::ObservationDataSetInfo::setVlEndTimePropIdx(int idx)
{
  m_validTimePropIdx[1]=idx;
}

void te::st::ObservationDataSetInfo::setVlTimePropIdxs(const std::vector<int>& idxs)
{
  for(std::size_t i=0; i<idxs.size(); i++)
    m_validTimePropIdx[i]=idxs[i];
}

int te::st::ObservationDataSetInfo::getRsTimePropIdx() const
{
  return m_resultTimePropIdx;
}

void te::st::ObservationDataSetInfo::setRsTimePropIdx(int idx)
{
  m_resultTimePropIdx = idx;
} 

std::string te::st::ObservationDataSetInfo::getId() const
{
  return te::common::Convert2UCase(m_dsInfo.getId()+"&"+m_dsName);
}

te::st::ObservationDataSetInfo::~ObservationDataSetInfo()
{
}
