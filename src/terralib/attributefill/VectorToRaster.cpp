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
 \file VectorToRaster.cpp
 */

#include "../common.h"

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

#include "../memory/DataSetItem.h"

#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"

#include "../rp/RasterAttributes.h"

#include "../statistics/core/Utils.h"

#include "VectorToRaster.h"


te::attributefill::VectorToRaster::VectorToRaster()
{
}

void te::attributefill::VectorToRaster::setInput( te::da::DataSourcePtr inVectorDsrc,
                                                  std::string inVectorName,
                                                  std::auto_ptr<te::da::DataSetType> inVectorDsType)
{
  m_inVectorDsrc = inVectorDsrc;
  m_inVectorName = inVectorName;
  m_inVectorDsType = inVectorDsType;
}

void te::attributefill::VectorToRaster::setParams(std::vector<std::string> selectedAttVec,
                                                  double resolutionX,
                                                  double resolutionY,
                                                  int columns,
                                                  int rows,
                                                  bool setDummy,
                                                  int dummy)
{
  m_selectedAttVec = selectedAttVec;
  m_resolutionX = resolutionX;
  m_resolutionY = resolutionY;
  m_columns = columns;
  m_rows = rows;
  m_setDummy = setDummy;
  m_dummy = dummy;
}

void te::attributefill::VectorToRaster::setOutput(te::da::DataSourcePtr outDsrc, std::string dsName)
{
  m_outDsrc = outDsrc;
  m_outDset = dsName;
}

bool te::attributefill::VectorToRaster::paramsAreValid()
{
  if(!m_inVectorDsType.get())
    return false;

  if(m_outDset.empty() || !m_outDsrc.get())
    return false;

  if(m_selectedAttVec.size() == 0)
    return false;

  if(m_resolutionX == 0 || m_resolutionY == 0)
    return false;

  if(m_columns == 0 || m_rows == 0)
    return false;

  return true;
}

bool te::attributefill::VectorToRaster::run()
{
  std::auto_ptr<te::da::DataSet> inDataSet = m_inVectorDsrc->getDataSet(m_inVectorName);
  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(m_inVectorDsType.get());
  std::size_t propPos = m_inVectorDsType->getPropertyPosition(geomProp->getName());

  std::auto_ptr<te::gm::Envelope> env = inDataSet->getExtent(propPos);
  
  te::rst::Grid* grid = new te::rst::Grid(m_resolutionX, m_resolutionY, env.get(), geomProp->getSRID());

// create bands
  std::vector<te::rst::BandProperty*> vecBandProp;

  for(std::size_t i = 0; i < m_selectedAttVec.size(); ++i)
  {
    te::dt::Property* prop =  m_inVectorDsType->getProperty(m_selectedAttVec[i]);
    te::rst::BandProperty* bProp = new te::rst::BandProperty(i, prop->getType(), prop->getName());
    vecBandProp.push_back(bProp);
  }

// create raster info
  std::map<std::string, std::string> conInfo = m_outDsrc->getConnectionInfo();

// create raster
  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid, vecBandProp, conInfo));

// get vector data
  std::string geomName = geomProp->getName();

  for(std::size_t i = 0; i < m_selectedAttVec.size(); ++i)
  {
    std::vector<te::gm::Geometry*> geomVec;
    std::vector<double> valueVec;
    
    inDataSet->moveBeforeFirst();
    while(inDataSet->moveNext())
    {
      geomVec.push_back(static_cast<te::gm::Geometry*>(inDataSet->getGeometry(geomName)->clone()));
      valueVec.push_back(inDataSet->getDouble(m_selectedAttVec[i]));
    }
// rasterize
    rst->rasterize(geomVec, valueVec, i);

    te::common::FreeContents(geomVec);
  }

  return true;
}

bool te::attributefill::VectorToRaster::save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType)
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
