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
#include "../../../geometry/GeometryProperty.h"
#include "../../../datatype/DateTimeProperty.h"
#include "../../../common/StringUtils.h"

//ST
#include "TrajectoryDataSetInfo.h"

te::st::TrajectoryDataSetInfo::TrajectoryDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              const std::string& tPropName, const std::string& gPropName, 
                              const std::string& idPropName, const std::string& id) 
 : m_obsDsInfo(dsinfo, dsName)
{
  //create properties
  te::dt::DateTimeProperty* tp = new te::dt::DateTimeProperty(tPropName);
  m_obsDsInfo.setTimePropInfo(tp);

  te::gm::GeometryProperty* gp = new te::gm::GeometryProperty(gPropName);
  m_obsDsInfo.setGeomPropInfo(gp);

  std::vector<std::string> aux;
  aux.push_back(gPropName);
  m_obsDsInfo.setObsPropInfo(aux);

  m_obsDsInfo.setIdPropInfo(idPropName);
  m_obsDsInfo.setId(id);
}


te::st::TrajectoryDataSetInfo::TrajectoryDataSetInfo( const te::st::ObservationDataSetInfo& info)
  : m_obsDsInfo(info)
{
}

const te::st::ObservationDataSetInfo& 
te::st::TrajectoryDataSetInfo::getObservationDataSetInfo() const
{
  return m_obsDsInfo;
}

te::st::TrajectoryDataSetInfo::~TrajectoryDataSetInfo()
{
}


