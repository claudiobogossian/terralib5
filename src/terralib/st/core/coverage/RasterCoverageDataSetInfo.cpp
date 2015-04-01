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
   const std::string& dsName, const std::string& rstPropName, te::dt::DateTime* time) 
   :  m_obsDsInfo(dsinfo, dsName),
      m_rstPropName(rstPropName)
{
  m_obsDsInfo.setTime(time);
}

te::st::RasterCoverageDataSetInfo::RasterCoverageDataSetInfo(const ObservationDataSetInfo& info, 
  const std::string& rstPropName)
  : m_obsDsInfo(info),
    m_rstPropName(rstPropName)
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
        
const std::string& te::st::RasterCoverageDataSetInfo::getRasterPropName() const
{
  return m_rstPropName;
}

te::st::RasterCoverageDataSetInfo::~RasterCoverageDataSetInfo()
{
}



