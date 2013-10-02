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
  \file TimeSeriesDataSetInfo.cpp

  \brief This file contains a class called TimeSeriesDataSetInfo
*/

//TerraLib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../common/StringUtils.h"

//ST
#include "TimeSeriesDataSetInfo.h"

te::st::TimeSeriesDataSetInfo::TimeSeriesDataSetInfo( const te::da::DataSourceInfo& dsinfo, 
                              const std::string& dsName, int tPropIdx, int vPropIdx, int gPropIdx, 
                              int idPropIdx, const std::string& id)
  : m_obsDsInfo(dsinfo, dsName, tPropIdx, vPropIdx, gPropIdx),
    m_vlPropIdxs(1,vPropIdx),
    m_idPropIdx(idPropIdx),
    m_id(id)
{  
}

te::st::TimeSeriesDataSetInfo::TimeSeriesDataSetInfo( const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              int tPropIdx, const std::vector<int>& vPropIdxs, int gPropIdx,
                              int idPropIdx, const std::string& id)
  : m_obsDsInfo(dsinfo, dsName, std::vector<int>(1,tPropIdx), vPropIdxs, gPropIdx),
    m_vlPropIdxs(vPropIdxs),
    m_idPropIdx(idPropIdx),
    m_id(id)
{  
}


te::st::TimeSeriesDataSetInfo::TimeSeriesDataSetInfo( const te::st::ObservationDataSetInfo& info, 
                                                      int idPropIdx, const std::string& id)
  : m_obsDsInfo(info),
    m_vlPropIdxs(info.getObsPropIdxs()),
    m_idPropIdx(idPropIdx),
    m_id(id)
{
}

const te::st::ObservationDataSetInfo& 
te::st::TimeSeriesDataSetInfo::getObservationDataSetInfo() const
{
  return m_obsDsInfo;
}

const te::da::DataSourceInfo& 
te::st::TimeSeriesDataSetInfo::getDataSourceInfo() const
{
  return m_obsDsInfo.getDataSourceInfo();
}

const std::vector<int>& 
te::st::TimeSeriesDataSetInfo::getTimePropIdxs() const
{
  return m_obsDsInfo.getTimePropIdxs();
}

const std::vector<int>& 
te::st::TimeSeriesDataSetInfo::getValuePropIdxs() const
{
  return m_vlPropIdxs;
}

int te::st::TimeSeriesDataSetInfo::getGeomPropIdx() const
{
  return m_obsDsInfo.getGeomPropIdx();
}

int te::st::TimeSeriesDataSetInfo::getIdPropIdx() const
{
  return m_idPropIdx;
}

std::string te::st::TimeSeriesDataSetInfo::getId() const
{
  return m_id;
}


