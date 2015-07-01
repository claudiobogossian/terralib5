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
 \file BufferOp.cpp
 */

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../geometry/GeometryProperty.h"

#include "../statistics/core/Utils.h"

#include "BufferOp.h"

te::vp::BufferOp::BufferOp():
  m_outDsetName("")
{
}

void te::vp::BufferOp::setInput(te::da::DataSourcePtr inDsrc,
                                std::string inDsetName,
                                std::auto_ptr<te::da::DataSetType> inDsetType,
                                const te::da::ObjectIdSet* oidSet)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inDsetType = inDsetType;
  m_oidSet = oidSet;
}

void te::vp::BufferOp::setParams(const double& distance,
                                const int& bufferPolygonRule,
                                const int& bufferBoundariesRule,
                                const bool& copyInputColumns,
                                const int& levels)
{
  m_distance = distance;
  m_bufferPolygonRule =  bufferPolygonRule;
  m_bufferBoundariesRule = bufferBoundariesRule;
  m_copyInputColumns = copyInputColumns;
  m_levels = levels;
}

void te::vp::BufferOp::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDsetName = dsname;
}

bool te::vp::BufferOp::paramsAreValid()
{
  if (!m_inDsetType->hasGeom())
    return false;

  if (m_outDsetName.empty() || !m_outDsrc.get())
    return false;
  
  return true;
}

te::da::DataSetType* te::vp::BufferOp::GetDataSetType()
{
  te::da::DataSetType* dsType = new te::da::DataSetType(m_outDsetName);

  //Primary key
  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(m_outDsetName + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  dsType->add(pkProperty);

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(m_outDsetName + "_pk", dsType);
  pk->add(pkProperty);
  dsType->setPrimaryKey(pk);

  te::dt::SimpleProperty* levelProperty = new te::dt::SimpleProperty("level", te::dt::INT32_TYPE);
  dsType->add(levelProperty);

  te::dt::SimpleProperty* distanceProperty = new te::dt::SimpleProperty("distance", te::dt::DOUBLE_TYPE);
  dsType->add(distanceProperty);

  
  if(m_copyInputColumns)
  {
    std::vector<te::dt::Property*> props = m_inDsetType->getProperties();

    for(std::size_t i = 0; i < props.size(); ++i)
    {
      const std::string name = props[i]->getName();
      int type = props[i]->getType();

      if(type != te::dt::GEOMETRY_TYPE && name != "FID")
      {
        te::dt::SimpleProperty* s_prop = new te::dt::SimpleProperty(name, type);
        dsType->add(s_prop);
      }
    }
  }

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(m_inDsetType.get());
  std::auto_ptr<te::gm::GeometryProperty> p (static_cast<te::gm::GeometryProperty*>(gp->clone()));
  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
  geometry->setGeometryType(te::gm::MultiPolygonType);
  geometry->setSRID(p->getSRID());
  dsType->add(geometry);

  return dsType;
}
