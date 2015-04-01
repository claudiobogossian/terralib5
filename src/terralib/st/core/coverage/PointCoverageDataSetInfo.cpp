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

te::st::PointCoverageDataSetInfo::PointCoverageDataSetInfo(const ObservationDataSetInfo& info,  
                              const std::vector<std::string>& vlPropNames)
  : m_obsDsInfo(info),
    m_vlPropNames(vlPropNames)
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
        
const std::vector<std::string>& te::st::PointCoverageDataSetInfo::getValuePropNames() const
{
  return m_vlPropNames;
}

te::st::PointCoverageDataSetInfo::~PointCoverageDataSetInfo()
{
}



