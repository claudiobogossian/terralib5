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
  \file RasterCoverageDataSetInfo.cpp

  \brief This file contains a class called RasterCoverageDataSetInfo
*/
//TerraLib
#include "../../../datatype/DateTime.h"

//ST
#include "RasterCoverageDataSetInfo.h"

te::st::RasterCoverageDataSetInfo::RasterCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, 
                              const std::string& dsName, int tPropIdx, int rstPropIdx)
  : m_obsDsInfo(dsinfo, dsName, tPropIdx, rstPropIdx),
    m_rstPropIdx(rstPropIdx), 
    m_time(0)
{  
}

te::st::RasterCoverageDataSetInfo::RasterCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, 
                              const std::string& dsName, int rstPropIdx, te::dt::DateTime* time)
  : m_obsDsInfo(dsinfo, dsName, -1, rstPropIdx),
    m_rstPropIdx(rstPropIdx), 
    m_time(time)
{  
}

te::st::RasterCoverageDataSetInfo::RasterCoverageDataSetInfo(const ObservationDataSetInfo& info, int rstPropIdx)
  : m_obsDsInfo(info),
    m_rstPropIdx(rstPropIdx), 
    m_time(0)
{  
}    

te::st::RasterCoverageDataSetInfo::RasterCoverageDataSetInfo(const ObservationDataSetInfo& info, int rstPropIdx, 
                                                             te::dt::DateTime* time)
  : m_obsDsInfo(info),
    m_rstPropIdx(rstPropIdx), 
    m_time(time)
{  
}   
      
te::st::CoverageType te::st::RasterCoverageDataSetInfo::getCoverageType() const
{
  return te::st::RASTER_COVERAGE;
}
        
const te::st::ObservationDataSetInfo& te::st::RasterCoverageDataSetInfo::getObservationDataSetInfo() const
{
  return m_obsDsInfo;
}
        
const te::da::DataSourceInfo& te::st::RasterCoverageDataSetInfo::getDataSourceInfo() const
{
  return m_obsDsInfo.getDataSourceInfo();
}
        
const std::vector<int>& te::st::RasterCoverageDataSetInfo::getTimePropIdxs() const
{
  return m_obsDsInfo.getTimePropIdxs();
}
 
int te::st::RasterCoverageDataSetInfo::getRasterPropIdx() const
{
  return m_rstPropIdx;
}

std::auto_ptr<te::dt::DateTime> te::st::RasterCoverageDataSetInfo::getTime() const
{
  return std::auto_ptr<te::dt::DateTime>(static_cast<te::dt::DateTime*>(m_time->clone())); 
}

te::st::RasterCoverageDataSetInfo::~RasterCoverageDataSetInfo()
{
}



