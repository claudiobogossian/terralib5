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
  \file terralib/unittest/raster/TsRasterVectorizer.cpp

  \brief A test suit for the Raster Vectorizer class.
 */

// TerraLib
#include <terralib/geometry.h>
#include <terralib/raster.h>
#include <terralib/dataaccess.h>
#include <terralib/memory.h>
#include "../Config.h"

// STL
#include <memory>
#include <cstdio>

// Boost
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE ( rasterVectorizer_tests )

BOOST_AUTO_TEST_CASE (rasterVectorizer_test)
{
  /* Define raster info and load */

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR "/rasters/pattern1.tif";
  std::auto_ptr< te::rst::Raster > inrasterPtr( te::rst::RasterFactory::open(rinfo) );

  std::vector<te::gm::Geometry*> polygons;
  std::vector< double > polygonsValues;
  
  te::rst::Vectorizer vectorizerInstance( inrasterPtr.get(), 0, 0, false );
  vectorizerInstance.run( polygons, &polygonsValues);
  BOOST_CHECK( !polygons.empty() );
  BOOST_CHECK( polygons.size() == polygonsValues.size() );

  /* Exporting to disk */

  {
    std::auto_ptr<te::da::DataSetType> dataSetTypePtr1(new te::da::DataSetType("RasterVectorizerTestPolygons"));
    
    dataSetTypePtr1->add( new te::dt::SimpleProperty("value", te::dt::DOUBLE_TYPE, true) );
    dataSetTypePtr1->add( new te::dt::SimpleProperty("id", te::dt::DOUBLE_TYPE, true) );
    dataSetTypePtr1->add( new te::gm::GeometryProperty("polygon", inrasterPtr->getSRID(),
      te::gm::PolygonType, true) );
    
    std::auto_ptr<te::da::DataSetType> dataSetTypePtr2( new te::da::DataSetType( *dataSetTypePtr1 ) );
    
    std::auto_ptr< te::mem::DataSet > memDataSetPtr( new te::mem::DataSet( dataSetTypePtr1.get()) );
    
    for( unsigned int polygonsIdx = 0 ; polygonsIdx < polygons.size() ; ++polygonsIdx )
    {
      te::mem::DataSetItem* dsItemPtr = new te::mem::DataSetItem(memDataSetPtr.get());
      dsItemPtr->setDouble( 0, polygonsValues[ polygonsIdx ] );
      dsItemPtr->setDouble( 1, polygonsIdx );
      dsItemPtr->setGeometry( 2, polygons[ polygonsIdx ] );
      
      memDataSetPtr->add( dsItemPtr );
    }

    remove( "RasterVectorizerTestPolygons.dbf" );
    remove( "RasterVectorizerTestPolygons.prj" );
    remove( "RasterVectorizerTestPolygons.shp" );
    remove( "RasterVectorizerTestPolygons.shx" );
    
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = "RasterVectorizerTestPolygons.shp";
//    connInfo["DRIVER"] = "ESRI Shapefile";
    
    std::auto_ptr<te::da::DataSource> dsOGR( te::da::DataSourceFactory::make("OGR") );
    dsOGR->setConnectionInfo(connInfo);
    dsOGR->open();
    
    memDataSetPtr->moveBeforeFirst();
    
    te::da::Create(dsOGR.get(), dataSetTypePtr2.get(), memDataSetPtr.get());

    dsOGR->close();
  }
}

BOOST_AUTO_TEST_SUITE_END()
