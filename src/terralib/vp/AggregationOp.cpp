/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
 \file AggregationOp.cpp
 */

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../datatype/StringProperty.h"

#include "../geometry/GeometryProperty.h"

#include "../statistics/core/Utils.h"

#include "AggregationOp.h"

te::vp::AggregationOp::AggregationOp():
  m_outDset("")
{
}

void te::vp::AggregationOp::setInput(te::da::DataSourcePtr inDsrc,
                                     std::string inDsetName,
                                     std::auto_ptr<te::da::DataSetType> inDsetType,
                                     const te::da::ObjectIdSet* oidSet)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inDsetType = inDsetType;
  m_oidSet = oidSet;
}

void te::vp::AggregationOp::setParams(std::vector<te::dt::Property*>& groupProps,
                                      std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >&statSum)
{
  m_groupProps = groupProps;
  m_statSum = statSum;
}

void te::vp::AggregationOp::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDset = dsname;
}

te::gm::GeomType te::vp::AggregationOp::getGeomResultType(te::gm::GeomType geom)
{
  if ((geom == te::gm::PolygonType) ||
      (geom == te::gm::PolygonZType) ||
      (geom == te::gm::PolygonMType) ||
      (geom == te::gm::PolygonZMType) ||
      (geom == te::gm::MultiPolygonZType) ||
      (geom == te::gm::MultiPolygonMType) ||
      (geom == te::gm::MultiPolygonZMType))
    return te::gm::MultiPolygonType;
  
  if ((geom == te::gm::LineStringType) ||
      (geom == te::gm::LineStringZType) || 
      (geom == te::gm::LineStringMType) ||
      (geom == te::gm::LineStringZMType) ||
      (geom == te::gm::MultiLineStringZType) ||
      (geom == te::gm::MultiLineStringMType) ||
      (geom == te::gm::MultiLineStringZMType))
    return te::gm::MultiLineStringType;
  
  if ((geom == te::gm::PointType) ||
      (geom == te::gm::PointZType) ||
      (geom == te::gm::PointMType) ||
      (geom == te::gm::PointZMType) ||
      (geom == te::gm::PointKdType) ||
      (geom == te::gm::MultiPointZType) ||
      (geom == te::gm::MultiPointMType) ||
      (geom == te::gm::MultiPointZMType))
    return te::gm::MultiPointType;
  
  return geom;
}



bool te::vp::AggregationOp::paramsAreValid()
{
  if (!m_inDsetType.get())
    return false;
  
  if (!m_inDsetType->hasGeom())
    return false;
  
  if (m_groupProps.empty())
    return false;

  if (m_outDset.empty() || !m_outDsrc.get())
    return false;
  
  return true;
}