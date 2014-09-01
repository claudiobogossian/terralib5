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
 \file RasterToVector.cpp
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
#include "../raster/RasterProperty.h"

#include "../rp/RasterAttributes.h"

#include "../statistics/core/Utils.h"

#include "RasterToVector.h"

te::processing::RasterToVector::RasterToVector()
{
}

void te::processing::RasterToVector::setInput(te::da::DataSourcePtr inRasterDsrc,
                                              std::string inRasterName,
                                              std::auto_ptr<te::da::DataSetType> inRasterDsType,
                                              te::da::DataSourcePtr inVectorDsrc,
                                              std::string inVectorName,
                                              std::auto_ptr<te::da::DataSetType> inVectorDsType)
{
  m_inRasterDsrc = inRasterDsrc;
  m_inRasterName = inRasterName;
  m_inRasterDsType = inRasterDsType;
  m_inVectorDsrc = inVectorDsrc;
  m_inVectorName = inVectorName;
  m_inVectorDsType = inVectorDsType;
}

void te::processing::RasterToVector::setParams(std::vector<te::stat::StatisticalSummary> statSum)
{
  m_statSum = statSum;
}

void te::processing::RasterToVector::setOutput(te::da::DataSourcePtr outDsrc, std::string dsName)
{
  m_outDsrc = outDsrc;
  m_outDset = dsName;
}

bool te::processing::RasterToVector::paramsAreValid()
{
  if (!m_inVectorDsType.get())
    return false;
  
  if (!m_inVectorDsType->hasGeom())
    return false;

  if (m_outDset.empty() || !m_outDsrc.get())
    return false;
  
  return true;
}

bool te::processing::RasterToVector::run()
{
  te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(m_inRasterDsType.get());
  
  std::auto_ptr<te::da::DataSet> dsRaster = m_inRasterDsrc->getDataSet(m_inRasterName);
  std::auto_ptr<te::rst::Raster> raster = dsRaster->getRaster(rasterProp->getName());
  
  //te::rp::RasterAttributes* rasterAtt = new te::rp::RasterAttributes();

  return true;
}

bool  te::processing::RasterToVector::save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType)
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