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
 \file IntersectionOp.cpp
 */

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../datatype/StringProperty.h"

#include "../geometry/GeometryProperty.h"

#include "../statistics/core/Utils.h"

#include "IntersectionOp.h"

te::vp::IntersectionOp::IntersectionOp():
  m_outDsetName("")
{
}

void te::vp::IntersectionOp::setInput(te::da::DataSourcePtr inFirstDsrc,
                                      std::string inFirstDsetName,
                                      std::auto_ptr<te::da::DataSetType> inFirstDsetType,
                                      te::da::DataSourcePtr inSecondDsrc,
                                      std::string inSecondDsetName,
                                      std::auto_ptr<te::da::DataSetType> inSecondDsetType,
                                      const te::da::ObjectIdSet* firstOidSet,
                                      const te::da::ObjectIdSet* secondOidSet)
{
  m_inFirstDsrc = inFirstDsrc;
  m_inFirstDsetName = inFirstDsetName;
  m_inFirstDsetType = inFirstDsetType;
  m_inSecondDsrc = inSecondDsrc;
  m_inSecondDsetName = inSecondDsetName;
  m_inSecondDsetType = inSecondDsetType;

  m_firstOidSet = firstOidSet;
  m_secondOidSet = secondOidSet;
}

void te::vp::IntersectionOp::setParams( const bool& copyInputColumns,
                                        std::size_t inSRID)
{
  m_copyInputColumns = copyInputColumns;
  m_SRID = inSRID;
}

void te::vp::IntersectionOp::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDsetName = dsname;
}

te::gm::GeomType te::vp::IntersectionOp::getGeomResultType(te::gm::GeomType geom)
{
  if (geom == te::gm::PolygonType)
    return te::gm::MultiPolygonType;
  
  if (geom == te::gm::LineStringType)
    return te::gm::MultiLineStringType;
  
  if (geom == te::gm::PointType)
    return te::gm::MultiPointType;
  
  return geom;
}

bool te::vp::IntersectionOp::paramsAreValid()
{
  if (!m_inFirstDsetType.get())
    return false;

  if (!m_inFirstDsetType->hasGeom())
    return false;

  if (!m_inSecondDsetType.get())
    return false;

  if (!m_inSecondDsetType->hasGeom())
    return false;

  if (m_outDsetName.empty() || !m_outDsrc.get())
    return false;
  
  return true;
}

std::vector<te::dt::Property*> te::vp::IntersectionOp::getTabularProps(te::da::DataSetType* dsType)
{
  std::vector<te::dt::Property*> props;
  te::dt::Property* prop;

  for(std::size_t i = 0; i < dsType->getProperties().size(); ++i)
  {
    prop = dsType->getProperty(i);

    if(prop->getType() != te::dt::GEOMETRY_TYPE && prop->getType() != te::dt::NUMERIC_TYPE) 
    {
      props.push_back(prop);
    }
  }

  return props;
}
