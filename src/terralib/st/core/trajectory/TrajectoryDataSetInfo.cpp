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
  \file TrajectoryDataSetInfo.cpp

  \brief This file contains a class called TrajectoryDataSetInfo
*/

//TerraLib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../common/StringUtils.h"

//ST
#include "TrajectoryDataSetInfo.h"

te::st::TrajectoryDataSetInfo::TrajectoryDataSetInfo( const te::da::DataSourceInfo& dsinfo, 
                                                      const std::string& dsName, int tPropIdx, 
                                                      int gPropIdx, int idPropIdx, const std::string& id)
  : m_obsDsInfo(dsinfo, dsName, tPropIdx, gPropIdx, gPropIdx),
    m_idPropIdx(idPropIdx),
    m_id(id)
{  
}

te::st::TrajectoryDataSetInfo::TrajectoryDataSetInfo( const te::st::ObservationDataSetInfo& info, 
                                                      int idPropIdx, const std::string& id)
  : m_obsDsInfo(info),
    m_idPropIdx(idPropIdx),
    m_id(id)
{
}

const te::st::ObservationDataSetInfo& 
te::st::TrajectoryDataSetInfo::getObservationDataSetInfo() const
{
  return m_obsDsInfo;
}

const te::da::DataSourceInfo& 
te::st::TrajectoryDataSetInfo::getDataSourceInfo() const
{
  return m_obsDsInfo.getDataSourceInfo();
}

const std::vector<int>& 
te::st::TrajectoryDataSetInfo::getTimePropIdxs() const
{
  return m_obsDsInfo.getTimePropIdxs();
}

int te::st::TrajectoryDataSetInfo::getGeomPropIdx() const
{
  return m_obsDsInfo.getGeomPropIdx();
}

int te::st::TrajectoryDataSetInfo::getIdPropIdx() const
{
  return m_idPropIdx;
}

std::string te::st::TrajectoryDataSetInfo::getId() const
{
  return m_id;
}


