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
 \file AggregationOp.cpp
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

#include "AggregationOp.h"

te::vp::AggregationOp::AggregationOp():
  m_outDset("")
{
}

void te::vp::AggregationOp::setInput(std::auto_ptr<te::da::DataSource> inDsrc,
                                     std::auto_ptr<te::da::DataSet> inDset,
                                     std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inDsrc = inDsrc;
  m_inDset = inDset;
  m_inDsetType = inDsetType;
}

void te::vp::AggregationOp::setParams(std::vector<te::dt::Property*>& groupProps,
                                      std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >&statSum)
{
  m_groupProps = groupProps;
  m_statSum = statSum;
}

void te::vp::AggregationOp::setOutput(std::auto_ptr<te::da::DataSource> outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDset = dsname;
}

te::gm::GeomType te::vp::AggregationOp::getGeomResultType(te::gm::GeomType geom)
{
  if (geom == te::gm::PolygonType)
    return te::gm::MultiPolygonType;
  
  if (geom == te::gm::LineStringType)
    return te::gm::MultiLineStringType;
  
  if (geom == te::gm::PointType)
    return te::gm::MultiPointType;
  
  return geom;
}

bool te::vp::AggregationOp::paramsAreValid()
{
  if (!m_inDset.get() || !m_inDsetType.get())
    return false;
  
  if (!m_inDsetType->hasGeom())
    return false;
  
  if (m_groupProps.empty())
    return false;

  if (m_outDset.empty() || !m_outDsrc.get())
    return false;
  
  return true;
}

bool  te::vp::AggregationOp::save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType)
{
  // do any adaptation necessary to persist the output dataset
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outDsType.get(), m_outDsrc->getCapabilities());
  te::da::DataSetType* dsTypeResult = converter->getResult();
  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(result.get(), converter));
  
  std::map<std::string, std::string> options;
  // create the dataset
  m_outDsrc->createDataSet(dsTypeResult, options);
  
  // copy from memory to output datasource
  result->moveBeforeFirst();
  m_outDsrc->add(dsTypeResult->getName(),result.get(), options);
  
  // create the primary key if it is possible
  if (m_outDsrc->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
  {
    std::string pk_name = dsTypeResult->getName() + "_pkey";
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
    pk->add(dsTypeResult->getProperty(0));
    m_outDsrc->addPrimaryKey(m_outDset,pk);
  }
  
  return true;
}
