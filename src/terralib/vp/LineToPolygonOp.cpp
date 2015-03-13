/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.
 
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
 \file LineToPolygonOp.cpp
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

#include "LineToPolygonOp.h"

te::vp::LineToPolygonOp::LineToPolygonOp():
  m_outDset("")
{
}

bool te::vp::LineToPolygonOp::paramsAreValid()
{
  if (!m_inDsrc.get())
    return false;

  if (m_inDsetName.empty())
    return false;

  if (m_outDset.empty() || !m_outDsrc.get())
    return false;

  return true;
}

void te::vp::LineToPolygonOp::setInput(te::da::DataSourcePtr inDsrc,
                                     std::string inDsetName,
                                     const te::da::ObjectIdSet* oidSet)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_oidSet = oidSet;
}

void te::vp::LineToPolygonOp::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDset = dsname;
}

std::auto_ptr<te::da::DataSetType> te::vp::LineToPolygonOp::buildOutDataSetType()
{
  std::auto_ptr<te::da::DataSetType> inDsType = m_inDsrc->getDataSetType(m_inDsetName);
  std::auto_ptr<te::da::DataSetType> outDsType(new te::da::DataSetType(m_outDset));

  std::string dSourceType = m_outDsrc->getType();

  std::vector<te::dt::Property*> vecProps = inDsType->getProperties();
  
  if(dSourceType == "OGR")
  {
    for(std::size_t i = 0; i < vecProps.size(); ++i)
    {
      if(vecProps[i]->getType() != te::dt::GEOMETRY_TYPE && vecProps[i]->getName() != "FID")
      {
        outDsType->add(vecProps[i]->clone());
      }
      else
      if(vecProps[i]->getType() == te::dt::GEOMETRY_TYPE)
      {
        te::gm::GeometryProperty* inGeom = static_cast<te::gm::GeometryProperty*>(vecProps[i]);
        te::gm::GeometryProperty* outGeom = new te::gm::GeometryProperty(inGeom->getName());
        outGeom->setGeometryType(te::gm::MultiPolygonType);
        outGeom->setSRID(inGeom->getSRID());
        outDsType->add(outGeom);
      }
    }
  }
  else
  {
    for(std::size_t i = 0; i < vecProps.size(); ++i)
    {
      if(vecProps[i]->getType() != te::dt::GEOMETRY_TYPE)
      {
        outDsType->add(vecProps[i]->clone());
      }
      else
      {
        te::gm::GeometryProperty* inGeom = static_cast<te::gm::GeometryProperty*>(vecProps[i]);
        te::gm::GeometryProperty* outGeom = new te::gm::GeometryProperty(inGeom->getName());
        outGeom->setGeometryType(te::gm::MultiPolygonType);
        outGeom->setSRID(inGeom->getSRID());
        outDsType->add(outGeom);
      }
    }
  }

  return outDsType;
}

te::gm::GeomType te::vp::LineToPolygonOp::getGeomResultType(te::gm::GeomType geom)
{
  if (geom == te::gm::PolygonType)
    return te::gm::MultiPolygonType;
  
  if (geom == te::gm::LineStringType)
    return te::gm::MultiLineStringType;
  
  if (geom == te::gm::PointType)
    return te::gm::MultiPointType;
  
  return geom;
}

