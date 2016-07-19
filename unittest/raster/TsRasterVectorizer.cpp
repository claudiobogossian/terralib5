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
  \file TsRaterVectorizer.cpp

  \brief A test suit for the Raster Vectorizer class.
 */

// TerraLib
#include "../Config.h"
#include "TsRasterVectorizer.h"
#include <terralib/geometry.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/Vectorizer.h>
#include <terralib/dataaccess/dataset/DataSetType.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/utils/Utils.h>
#include <terralib/memory/DataSet.h>
#include <terralib/memory/DataSetItem.h>

#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION(TsRasterVectorizer);

void TsRasterVectorizer::tcRasterVectorizer()
{
// define raster info and load
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/pattern1.tif";
  std::auto_ptr< te::rst::Raster > inrasterPtr( te::rst::RasterFactory::open(rinfo) );

  std::vector<te::gm::Geometry*> polygons;
  std::vector< double > polygonsValues;
  inrasterPtr->vectorize(polygons, 0, 0, &polygonsValues);

  std::cout << "vectorizer created " << polygons.size() << " polygons" << std::endl;
  for (unsigned int i = 0; i < polygons.size(); i++)
    std::cout << "  polygon " << i << ": " << polygons[i]->toString() << std::endl;
  
  // creating a dataset in memory
  
  std::auto_ptr< te::mem::DataSet > memDataSetPtr;
  
  {
    std::auto_ptr<te::da::DataSetType> dataSetTypePtr(new te::da::DataSetType("RasterVectorizerTestPolygons"));
    
    dataSetTypePtr->add( new te::dt::SimpleProperty("Value", te::dt::DOUBLE_TYPE, true) );  
    dataSetTypePtr->add( new te::gm::GeometryProperty("polygon", inrasterPtr->getSRID(), 
      te::gm::PolygonType, true) );  
    
    memDataSetPtr.reset( new te::mem::DataSet( dataSetTypePtr.get()) );
    
    for( unsigned int polygonsIdx = 0 ; polygonsIdx < polygons.size() ; ++polygonsIdx )
    {
      te::mem::DataSetItem* dsItemPtr = new te::mem::DataSetItem(memDataSetPtr.get());
      dsItemPtr->setDouble( 0, polygonsValues[ polygonsIdx ] );
      dsItemPtr->setGeometry( 1, polygons[ polygonsIdx ] );
      
      memDataSetPtr->add( dsItemPtr );
    }
  }
  
  // Exporting to disk
  
  {
    std::auto_ptr<te::da::DataSetType> dataSetTypePtr(new te::da::DataSetType("RasterVectorizerTestPolygons"));
    
    dataSetTypePtr->add( new te::dt::SimpleProperty("Value", te::dt::DOUBLE_TYPE, true) );  
    dataSetTypePtr->add( new te::gm::GeometryProperty("polygon", inrasterPtr->getSRID(), 
      te::gm::PolygonType, true) );  
    
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = "./RasterVectorizerTestPolygons";
    connInfo["DRIVER"] = "ESRI Shapefile";
    
    std::auto_ptr<te::da::DataSource> dsOGR( te::da::DataSourceFactory::make("OGR") );
    dsOGR->setConnectionInfo(connInfo);
    dsOGR->open();
    
    memDataSetPtr->moveBeforeFirst();
    
    te::da::Create(dsOGR.get(), dataSetTypePtr.get(), memDataSetPtr.get());

    dsOGR->close();
  }  
}
