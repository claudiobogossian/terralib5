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
  \file PointCoverageDataSetInfo.cpp

  \brief This file contains a class called PointCoverageDataSetInfo
*/
//TerraLib
#include "../../../datatype/DateTime.h"

//ST
#include "PointCoverageDataSetInfo.h"

te::st::PointCoverageDataSetInfo::PointCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, 
                              const std::string& dsName, int tPropIdx, int ptPropIdx, int vlPropIdx, te::dt::DateTime* time)
  : m_obsDsInfo(dsinfo, dsName, tPropIdx, vlPropIdx, ptPropIdx),
    m_vlPropIdx(1,vlPropIdx),
    m_time(time)
{  
}

te::st::PointCoverageDataSetInfo::PointCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, 
                              const std::string& dsName, int ptPropIdx, int vlPropIdx, te::dt::DateTime* time)
  : m_obsDsInfo(dsinfo, dsName, -1, vlPropIdx, ptPropIdx),
    m_vlPropIdx(1,vlPropIdx),
    m_time(time)
{  
}

te::st::PointCoverageDataSetInfo::PointCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              const std::vector<int>& tPropIdx, int ptPropIdx, const std::vector<int>& vlPropIdx, 
                              te::dt::DateTime* time)
  : m_obsDsInfo(dsinfo, dsName, tPropIdx, vlPropIdx, ptPropIdx),
    m_vlPropIdx(vlPropIdx),
    m_time(time)
{  
}     

te::st::PointCoverageDataSetInfo::PointCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              int ptPropIdx, const std::vector<int>& vlPropIdx, te::dt::DateTime* time)
  : m_obsDsInfo(dsinfo, dsName, std::vector<int>(1,-1), vlPropIdx, ptPropIdx),
    m_vlPropIdx(vlPropIdx),
    m_time(time)
{  
}     

te::st::PointCoverageDataSetInfo::PointCoverageDataSetInfo(const ObservationDataSetInfo& info,  
                              const std::vector<int>& vlPropIdxs, te::dt::DateTime* time)
  : m_obsDsInfo(info),
    m_vlPropIdx(vlPropIdxs),
    m_time(time)
{  
}       

te::st::CoverageType te::st::PointCoverageDataSetInfo::getCoverageType() const
{
  return te::st::POINT_COVERAGE;
}
        
const te::st::ObservationDataSetInfo& te::st::PointCoverageDataSetInfo::getObservationDataSetInfo() const
{
  return m_obsDsInfo;
}
        
const te::da::DataSourceInfo& te::st::PointCoverageDataSetInfo::getDataSourceInfo() const
{
  return m_obsDsInfo.getDataSourceInfo();
}
        
const std::vector<int>& te::st::PointCoverageDataSetInfo::getTimePropIdxs() const
{
  return m_obsDsInfo.getTimePropIdxs();
}
 
int te::st::PointCoverageDataSetInfo::getPointPropIdx() const
{
  return m_obsDsInfo.getGeomPropIdx(); 
}

const std::vector<int>& te::st::PointCoverageDataSetInfo::getValuePropIdxs() const
{
  return m_vlPropIdx;
}

std::auto_ptr<te::dt::DateTime> te::st::PointCoverageDataSetInfo::getTime() const
{
  return std::auto_ptr<te::dt::DateTime>(static_cast<te::dt::DateTime*>(m_time->clone())); 
}



