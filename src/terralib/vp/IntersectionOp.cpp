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

te::vp::IntersectionOp::IntersectionOp()
  : m_firstOidSet(0),
  m_secondOidSet(0),
  m_isFistQuery(false),
  m_isSecondQuery(false)
{
}

void te::vp::IntersectionOp::setInput(te::da::DataSourcePtr inFirstDsrc,
                                      std::string inFirstDsetName,
                                      std::auto_ptr<te::da::DataSetTypeConverter> firstConverter,
                                      te::da::DataSourcePtr inSecondDsrc,
                                      std::string inSecondDsetName,
                                      std::auto_ptr<te::da::DataSetTypeConverter> secondConverter,
                                      const te::da::ObjectIdSet* firstOidSet,
                                      const te::da::ObjectIdSet* secondOidSet)
{
  m_inFirstDsrc = inFirstDsrc;
  m_inFirstDsetName = inFirstDsetName;
  m_firstConverter = firstConverter;
  m_inSecondDsrc = inSecondDsrc;
  m_inSecondDsetName = inSecondDsetName;
  m_secondConverter = secondConverter;

  m_firstOidSet = firstOidSet;
  m_secondOidSet = secondOidSet;
}

void te::vp::IntersectionOp::setInput(te::da::DataSourcePtr inFirstDsrc,
                                      std::string inFirstDsetName,
                                      std::auto_ptr<te::da::DataSetType> firstDsType,
                                      std::auto_ptr<te::da::DataSet> firstDs,
                                      std::auto_ptr<te::da::DataSetTypeConverter> firstConverter,
                                      te::da::DataSourcePtr inSecondDsrc,
                                      std::string inSecondDsetName,
                                      std::auto_ptr<te::da::DataSetType> secondDsType,
                                      std::auto_ptr<te::da::DataSet> secondDs,
                                      std::auto_ptr<te::da::DataSetTypeConverter> secondConverter,
                                      const te::da::ObjectIdSet* firstOidSet,
                                      const te::da::ObjectIdSet* secondOidSet)
{
  m_inFirstDsrc = inFirstDsrc;
  m_inFirstDsetName = inFirstDsetName;
  m_firstConverter = firstConverter;
  m_inSecondDsrc = inSecondDsrc;
  m_inSecondDsetName = inSecondDsetName;
  m_secondConverter = secondConverter;

  m_firstDsType = firstDsType;
  m_firstDs = firstDs;
  m_secondDsType = secondDsType;
  m_secondDs = secondDs;

  m_firstOidSet = firstOidSet;
  m_secondOidSet = secondOidSet;
}

void te::vp::IntersectionOp::setParams(const std::vector<std::pair<std::string, std::string> >& attributeVec)
{
  m_attributeVec = attributeVec;
}

void te::vp::IntersectionOp::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDsetName = dsname;
}

te::gm::GeomType te::vp::IntersectionOp::setGeomResultType(te::gm::GeomType firstGeom, te::gm::GeomType secondGeom)
{
  if ((firstGeom == te::gm::PointType) ||
    (firstGeom == te::gm::PointZType) ||
    (firstGeom == te::gm::PointMType) ||
    (firstGeom == te::gm::PointZMType) ||
    (firstGeom == te::gm::PointKdType) ||

    (secondGeom == te::gm::PointType) ||
    (secondGeom == te::gm::PointZType) ||
    (secondGeom == te::gm::PointMType) ||
    (secondGeom == te::gm::PointZMType) ||
    (secondGeom == te::gm::PointKdType) ||

    (firstGeom == te::gm::MultiPointType) ||
    (firstGeom == te::gm::MultiPointZType) ||
    (firstGeom == te::gm::MultiPointMType) ||
    (firstGeom == te::gm::MultiPointZMType) ||

    (secondGeom == te::gm::MultiPointType) ||
    (secondGeom == te::gm::MultiPointZType) ||
    (secondGeom == te::gm::MultiPointMType) ||
    (secondGeom == te::gm::MultiPointZMType))

    return te::gm::MultiPointType;

  else if ((firstGeom == te::gm::LineStringType) ||
    (firstGeom == te::gm::LineStringZType) ||
    (firstGeom == te::gm::LineStringMType) ||
    (firstGeom == te::gm::LineStringZMType) ||

    (secondGeom == te::gm::LineStringType) ||
    (secondGeom == te::gm::LineStringZType) ||
    (secondGeom == te::gm::LineStringMType) ||
    (secondGeom == te::gm::LineStringZMType) ||

    (firstGeom == te::gm::MultiLineStringType) ||
    (firstGeom == te::gm::MultiLineStringZType) ||
    (firstGeom == te::gm::MultiLineStringMType) ||
    (firstGeom == te::gm::MultiLineStringZMType) ||

    (secondGeom == te::gm::MultiLineStringType) ||
    (secondGeom == te::gm::MultiLineStringZType) ||
    (secondGeom == te::gm::MultiLineStringMType) ||
    (secondGeom == te::gm::MultiLineStringZMType))

    return te::gm::MultiLineStringType;

  else
    return te::gm::MultiPolygonType;
}

bool te::vp::IntersectionOp::paramsAreValid()
{
  if (!m_firstConverter->getResult())
    return false;

  if (!m_firstConverter->getResult()->hasGeom())
    return false;

  if (!m_secondConverter->getResult())
    return false;

  if (!m_secondConverter->getResult()->hasGeom())
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

te::da::DataSetType* te::vp::IntersectionOp::getOutputDsType()
{
  te::da::DataSetType* dsType = new te::da::DataSetType(m_outDsetName);

  //std::auto_ptr<te::da::DataSetType> firstDst = m_inFirstDsrc->getDataSetType(m_inFirstDsetName);
  //std::auto_ptr<te::da::DataSetType> secondDst = m_inSecondDsrc->getDataSetType(m_inSecondDsetName);

  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(m_outDsetName + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  dsType->add(pkProperty);

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(m_outDsetName + "_pk", dsType);
  pk->add(pkProperty);
  dsType->setPrimaryKey(pk);
  
  for (std::size_t i = 0; i < m_attributeVec.size(); ++i)
  {
    te::dt::Property* p = 0;
    if (m_attributeVec[i].first == m_firstDsType->getName())
    {
      p = m_firstDsType->getProperty(m_attributeVec[i].second);
    }
    else
    {
      p = m_secondDsType->getProperty(m_attributeVec[i].second);
    }

    p->setName(m_attributeVec[i].first + "_" + p->getName());

    dsType->add(p->clone());
  }

  te::gm::GeomType newType = setGeomResultType(te::da::GetFirstGeomProperty(m_firstDsType.get())->getGeometryType(), te::da::GetFirstGeomProperty(m_secondDsType.get())->getGeometryType());

  te::gm::GeometryProperty* newGeomProp = new te::gm::GeometryProperty("geom");
  newGeomProp->setGeometryType(newType);
  newGeomProp->setSRID(te::da::GetFirstGeomProperty(m_firstDsType.get())->getSRID());

  dsType->add(newGeomProp);

  return dsType;
}

void te::vp::IntersectionOp::setIsFirstQuery()
{
  m_isFistQuery = true;
}

void te::vp::IntersectionOp::setIsSecondQuery()
{
  m_isSecondQuery = true;
}