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
// Terralib

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
#include "../geometry/Line.h"
#include "../geometry/LineString.h"
#include "../geometry/Polygon.h"
#include "../geometry/Point.h"
#include "../geometry/Utils.h"

#include "../memory/DataSetItem.h"

#include "../raster.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"

#include "../rp/RasterAttributes.h"

#include "../statistics/core/Utils.h"

#include "Exception.h"
#include "VectorToRaster.h"

// BOOST
#include <boost/lexical_cast.hpp>


te::attributefill::VectorToRaster::VectorToRaster()
{
}

void te::attributefill::VectorToRaster::setInput( te::da::DataSourcePtr inVectorDsrc,
                                                  std::string inVectorName,
                                                  std::auto_ptr<te::da::DataSetTypeConverter> inVectorDsType)
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
  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(m_inVectorDsType->getResult());
  std::size_t propPos = boost::lexical_cast<std::size_t>(m_inVectorDsType->getResult()->getPropertyPosition(geomProp->getName()));

  te::gm::Envelope* env = inDataSet->getExtent(propPos).release();
  
  te::rst::Grid* grid = new te::rst::Grid(m_resolutionX, m_resolutionY, env, geomProp->getSRID());

// create bands
  std::vector<te::rst::BandProperty*> vecBandProp;

  for(std::size_t i = 0; i < m_selectedAttVec.size(); ++i)
  {
    te::rst::BandProperty* bProp = new te::rst::BandProperty(i, te::dt::DOUBLE_TYPE, "");
    
    if(m_setDummy == true)
      bProp->m_noDataValue = m_dummy;

    vecBandProp.push_back(bProp);
  }

// create raster info
  std::map<std::string, std::string> conInfo = m_outDsrc->getConnectionInfo();

// create raster
  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid, vecBandProp, conInfo));

  te::rst::FillRaster(rst.get(), rst->getBand(0)->getProperty()->m_noDataValue);

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
  task.setTotalSteps((int)(m_selectedAttVec.size() * vectorMap.size()));
  task.useTimer(true);

  for(std::size_t i = 0; i < m_selectedAttVec.size(); ++i)
  {
    std::map<te::gm::Geometry*, std::vector<double> >::iterator vectorIt = vectorMap.begin();

    while(vectorIt != vectorMap.end())
    {
      switch (vectorIt->first->getGeomTypeId())
      {
      case te::gm::PolygonType:
      case te::gm::MultiPolygonType:
      {
                                     std::vector<te::gm::Geometry*> geomVec;

                                     if (vectorIt->first->getGeomTypeId() == te::gm::MultiPolygonType)
                                       te::gm::Multi2Single(vectorIt->first, geomVec);
                                     else
                                       geomVec.push_back(vectorIt->first);
                                     
                                     for (std::size_t g = 0; g < geomVec.size(); ++g)
                                     {
                                       te::gm::Polygon* polygon = dynamic_cast<te::gm::Polygon*>(geomVec[g]);

                                       if (!polygon)
                                         continue;

                                       polygon->setSRID(geomProp->getSRID());

                                       te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(rst.get(), polygon);
                                       te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(rst.get(), polygon);

                                       while (it != itend)
                                       {
                                         rst->setValue(it.getColumn(), it.getRow(), vectorIt->second[i], i);
                                         ++it;
                                       }
                                     }
                                     break;
      }
      case te::gm::LineStringType:
      case te::gm::MultiLineStringType:
      {
                                        std::vector<te::gm::Geometry*> geomVec;

                                        if (vectorIt->first->getGeomTypeId() == te::gm::MultiLineStringType)
                                          te::gm::Multi2Single(vectorIt->first, geomVec);
                                        else
                                          geomVec.push_back(vectorIt->first);


                                        for (std::size_t g = 0; g < geomVec.size(); ++g)
                                        {
                                          te::gm::LineString* lineString = static_cast<te::gm::LineString*>(geomVec[g]);

                                          if (!lineString)
                                            continue;

                                          std::size_t nPoints = lineString->getNPoints();
                                          
                                          for (std::size_t n = 0; n < nPoints - 1; ++n)
                                          {
                                            std::auto_ptr<te::gm::Line> line(new te::gm::Line(*lineString->getPointN(n),
                                                                                              *lineString->getPointN(n + 1),
                                                                                              te::gm::LineStringType,
                                                                                              geomProp->getSRID()));

                                            te::rst::LineIterator<double> it = te::rst::LineIterator<double>::begin(rst.get(), line.get());
                                            te::rst::LineIterator<double> itend = te::rst::LineIterator<double>::end(rst.get(), line.get());

                                            while (it != itend)
                                            {
                                              double val = (*it)[0];
                                              rst->setValue(it.getColumn(), it.getRow(), vectorIt->second[i], i);
                                              ++it;
                                            }
                                          }
                                        }
                                        //te::gm::LineString* lineString = 0;

                                        //if (vectorIt->first->getGeomTypeId() == te::gm::MultiLineStringType)
                                        //{
                                        //  te::gm::MultiLineString* mLineString = dynamic_cast<te::gm::MultiLineString*>(vectorIt->first);
                                        //  lineString = dynamic_cast<te::gm::LineString*>(mLineString->getGeometryN(0));
                                        //}
                                        //else
                                        //{
                                        //  lineString = static_cast<te::gm::LineString*>(vectorIt->first);
                                        //}

                                        //if (!lineString)
                                        //  continue;

                                        //std::size_t nPoints = lineString->getNPoints();
                                        //
                                        //for (std::size_t n = 0; n < nPoints-1; ++n)
                                        //{
                                        //  std::auto_ptr<te::gm::Line> line(new te::gm::Line(*lineString->getPointN(n),
                                        //                                                     *lineString->getPointN(n + 1),
                                        //                                                     te::gm::LineStringType,
                                        //                                                     geomProp->getSRID()));
                                        //  te::rst::LineIterator<double> it = te::rst::LineIterator<double>::begin(rst.get(), line.get());
                                        //  te::rst::LineIterator<double> itend = te::rst::LineIterator<double>::end(rst.get(), line.get());
                                        //  
                                        //  while (it != itend)
                                        //  {
                                        //    rst->setValue(it.getColumn(), it.getRow(), vectorIt->second[i], i);
                                        //    ++it;
                                        //  }
                                        //}
                                        
                                        break;
      }
      case te::gm::PointType:
      case te::gm::MultiPointType:
      {
                                   std::vector<te::gm::Geometry*> geomVec;

                                   if (vectorIt->first->getGeomTypeId() == te::gm::MultiPointType)
                                     te::gm::Multi2Single(vectorIt->first, geomVec);
                                   else
                                     geomVec.push_back(vectorIt->first);

                                   for (std::size_t g = 0; g < geomVec.size(); ++g)
                                   {
                                     std::vector<te::gm::Point*> pointVec;
                                     te::gm::Point* point = static_cast<te::gm::Point*>(geomVec[g]);

                                     if (!point)
                                       continue;

                                     pointVec.push_back(point);

                                     point->setSRID(geomProp->getSRID());

                                     te::rst::PointSetIterator<double> it = te::rst::PointSetIterator<double>::begin(rst.get(), pointVec);
                                     te::rst::PointSetIterator<double> itend = te::rst::PointSetIterator<double>::end(rst.get(), pointVec);

                                     while (it != itend)
                                     {
                                       rst->setValue(it.getColumn(), it.getRow(), vectorIt->second[i], i);
                                       ++it;
                                     }
                                   }
                                   
                                   break;
      }
      default:
        break;
      }
      
      ++vectorIt;

      task.pulse();
    }

    if (task.isActive() == false)
      throw te::attributefill::Exception(TE_TR("Operation canceled!"));
  }
  return true;
}