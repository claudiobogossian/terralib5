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
 \file VectorToRaster.cpp
 */

#include "../common.h"
#include "../common/progress/TaskProgress.h"
#include "../common/Translator.h"

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
#include "../geometry/Polygon.h"

#include "../memory/DataSetItem.h"

#include "../raster.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"

#include "../rp/RasterAttributes.h"

#include "../statistics/core/Utils.h"

#include "Exception.h"
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
  
  if(setDummy == true)
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

  te::gm::Envelope* env = inDataSet->getExtent(propPos).release();
  
  te::rst::Grid* grid = new te::rst::Grid(m_resolutionX, m_resolutionY, env, geomProp->getSRID());

// create bands
  std::vector<te::rst::BandProperty*> vecBandProp;

  for(std::size_t i = 0; i < m_selectedAttVec.size(); ++i)
  {
    te::dt::Property* prop =  m_inVectorDsType->getProperty(m_selectedAttVec[i]);
    te::rst::BandProperty* bProp = new te::rst::BandProperty(i, te::dt::DOUBLE_TYPE, "");
    
    if(m_setDummy == true)
      bProp->m_noDataValue = m_dummy;

    vecBandProp.push_back(bProp);
  }

// create raster info
  std::map<std::string, std::string> conInfo = m_outDsrc->getConnectionInfo();

// create raster
  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid, vecBandProp, conInfo));

// get vector data
  std::string geomName = geomProp->getName();
  std::map<te::gm::Geometry*, std::vector<double> > vectorMap;

  inDataSet->moveBeforeFirst();
  while(inDataSet->moveNext())
  {
    te::gm::Geometry* geom = static_cast<te::gm::Geometry*>(inDataSet->getGeometry(geomName)->clone());
    std::vector<double> valueVec;

    for(std::size_t b = 0; b < m_selectedAttVec.size(); ++b)
    {
      valueVec.push_back(inDataSet->getDouble(m_selectedAttVec[b]));
    }

    vectorMap.insert(std::pair<te::gm::Geometry*, std::vector<double> >(geom, valueVec));
  }

  te::common::TaskProgress task("Rasterizing...");
  task.setTotalSteps(m_selectedAttVec.size() * vectorMap.size());
  task.useTimer(true);

  for(std::size_t i = 0; i < m_selectedAttVec.size(); ++i)
  {
    std::map<te::gm::Geometry*, std::vector<double> >::iterator vectorIt = vectorMap.begin();

    while(vectorIt != vectorMap.end())
    {
      te::gm::Polygon* polygon = static_cast<te::gm::Polygon*>(vectorIt->first);
      polygon->setSRID(geomProp->getSRID());

      if(!polygon)
        continue;

      te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(rst.get(), polygon);
      te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(rst.get(), polygon);
      
      while (it != itend)
      {
        rst->setValue(it.getColumn(), it.getRow(), vectorIt->second[i], i);
        ++it;
      }

      ++vectorIt;
      task.pulse();
    }

    if (task.isActive() == false)
      throw te::attributefill::Exception(TE_TR("Operation canceled!"));
  }
  return true;
}