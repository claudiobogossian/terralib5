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
  \file terralib/unittest/raster/TsRaster.cpp
 
  \brief A test suit for the Cached Raster class.
 */

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/raster.h>
#include <terralib/geometry.h>
#include <terralib/memory/CachedRaster.h>
#include "../Config.h"

// Boost
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE ( raster_tests )

bool loadSHPFile( const std::string& shpFileName,
  std::vector< te::gm::Geometry* >& geomPtrs )
{
  std::string connInfo("file://");
  connInfo += shpFileName;
  std::unique_ptr< te::da::DataSource > ogrDataSourcePtr(
    te::da::DataSourceFactory::make("OGR", connInfo) );
  BOOST_CHECK( ogrDataSourcePtr.get() != 0 );

  ogrDataSourcePtr->open();
  
  BOOST_CHECK( ogrDataSourcePtr->isOpened() );
  
  std::vector<std::string> dataSetNames = ogrDataSourcePtr->getDataSetNames();
  std::auto_ptr< te::da::DataSourceTransactor > transactorPtr =
    ogrDataSourcePtr->getTransactor();
  
  for( unsigned int dataSetNamesIdx = 0 ; dataSetNamesIdx < dataSetNames.size() ;
    ++dataSetNamesIdx )
  {
    std::auto_ptr<te::da::DataSet> datasetPtr = transactorPtr->getDataSet(
      dataSetNames[ dataSetNamesIdx ] );
    
    const std::size_t nProperties = datasetPtr->getNumProperties();
    
    BOOST_CHECK( datasetPtr->moveBeforeFirst() );
    
    while( datasetPtr->moveNext() )
    {
      for( std::size_t propIdx = 0 ; propIdx < nProperties ; ++propIdx )
      {
        if( datasetPtr->getPropertyDataType( propIdx ) == te::dt::GEOMETRY_TYPE )
        {
          std::auto_ptr< te::gm::Geometry > geomPtr = datasetPtr->getGeometry(
            propIdx );
          
          if( geomPtr->getGeomTypeId() == te::gm::PolygonType )
          {
            geomPtrs.push_back( geomPtr.release() );
          }
          else if( geomPtr->getGeomTypeId() == te::gm::MultiPolygonType )
          {
            te::gm::MultiPolygon const * const mPolPtr = (te::gm::MultiPolygon const *)
              geomPtr.get();
              
            for( std::size_t gIdx = 0 ; gIdx < mPolPtr->getNumGeometries() ;
              ++gIdx )
            {
              geomPtrs.push_back( new te::gm::Polygon( *( (te::gm::Polygon*)
                mPolPtr->getGeometryN( gIdx ) ) ) );
            }
          }
        }
      }
    }
  }
  
  return true;
}

BOOST_AUTO_TEST_CASE (rasterConstructor_test)
{
}

BOOST_AUTO_TEST_CASE (rasterGrid_test)
{
}

BOOST_AUTO_TEST_CASE (rasterCopyConstructor_test)
{
}

BOOST_AUTO_TEST_CASE (metadata_persistence_test)
{
  std::map<std::string, std::string> dsinfo;
  dsinfo["URI"] = "raster_metadata_persistence_test.tif";  
  
  // Creating the output raster
  
  {
    std::vector<te::rst::BandProperty*> vecBandProp;
    vecBandProp.push_back( new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE ) );
    vecBandProp[ 0 ]->m_blkh = 100;
    vecBandProp[ 0 ]->m_blkw = 100;
    vecBandProp[ 0 ]->m_nblocksx = 1;
    vecBandProp[ 0 ]->m_nblocksy = 1;
    vecBandProp[ 0 ]->m_noDataValue = 0;
    vecBandProp[ 0 ]->m_type = te::dt::UCHAR_TYPE;
    vecBandProp[ 0 ]->m_categoryNames.push_back( "category_0" );
    vecBandProp[ 0 ]->m_categoryNames.push_back( "category_1" );
    vecBandProp[ 0 ]->m_description = "description0";
    vecBandProp[ 0 ]->m_metadata.push_back( 
       std::pair<std::string, std::string>( "meta0name", "meta0value" ) );
    vecBandProp[ 0 ]->m_metadata.push_back( 
       std::pair<std::string, std::string>( "meta1name", "meta1value" ) );
    
    const te::gm::Coord2D ulc( 0, 0 );
    te::rst::Grid* grid = new te::rst::Grid( 100, 100, 1.0, 1.0, &ulc, 0 );
    
    std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", 
      grid, vecBandProp, dsinfo));
    
    BOOST_REQUIRE( rst.get() );
    BOOST_CHECK( rst->getBand( 0 )->getProperty()->m_description
      == "description0" );    
    BOOST_REQUIRE( rst->getBand( 0 )->getProperty()->m_metadata.size()
      == 2 );        
    BOOST_CHECK( rst->getBand( 0 )->getProperty()->m_metadata[ 0 ].first
      == "meta0name" );        
    BOOST_CHECK( rst->getBand( 0 )->getProperty()->m_metadata[ 0 ].second
      == "meta0value" );            
    BOOST_CHECK( rst->getBand( 0 )->getProperty()->m_metadata[ 1 ].first
      == "meta1name" );        
    BOOST_CHECK( rst->getBand( 0 )->getProperty()->m_metadata[ 1 ].second
      == "meta1value" );       
    BOOST_REQUIRE( rst->getBand( 0 )->getProperty()->m_categoryNames.size()
      == 2 );    
    BOOST_CHECK( rst->getBand( 0 )->getProperty()->m_categoryNames[ 0 ]
      == "category_0" );
    BOOST_CHECK( rst->getBand( 0 )->getProperty()->m_categoryNames[ 1 ]
      == "category_1" );    
  }
  
  // Reopen raster
  
  {
    boost::shared_ptr< te::rst::Raster > inputRasterPtr ( 
      te::rst::RasterFactory::open( dsinfo ) );
    
    BOOST_REQUIRE( inputRasterPtr.get() );  
    BOOST_CHECK( inputRasterPtr->getBand( 0 )->getProperty()->m_description
      == "description0" );    
    BOOST_REQUIRE( inputRasterPtr->getBand( 0 )->getProperty()->m_metadata.size()
      == 2 );        
    BOOST_CHECK( inputRasterPtr->getBand( 0 )->getProperty()->m_metadata[ 0 ].first
      == "meta0name" );        
    BOOST_CHECK( inputRasterPtr->getBand( 0 )->getProperty()->m_metadata[ 0 ].second
      == "meta0value" );            
    BOOST_CHECK( inputRasterPtr->getBand( 0 )->getProperty()->m_metadata[ 1 ].first
      == "meta1name" );        
    BOOST_CHECK( inputRasterPtr->getBand( 0 )->getProperty()->m_metadata[ 1 ].second
      == "meta1value" ); 
    BOOST_REQUIRE( inputRasterPtr->getBand( 0 )->getProperty()->m_categoryNames.size()
      == 2 );    
    BOOST_CHECK( inputRasterPtr->getBand( 0 )->getProperty()->m_categoryNames[ 0 ]
      == "category_0" );
    BOOST_CHECK( inputRasterPtr->getBand( 0 )->getProperty()->m_categoryNames[ 1 ]
      == "category_1" );    
  }
}

BOOST_AUTO_TEST_CASE (rasterize_test)
{
  std::vector< te::gm::Geometry* > geomPtrs;
  BOOST_CHECK( loadSHPFile( TERRALIB_DATA_DIR "/shape/poligono_unico.shp",
    geomPtrs ) );
  
  std::vector<te::rst::BandProperty*> vecBandProp;
  vecBandProp.push_back( new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE ) );
  vecBandProp[ 0 ]->m_blkh = 100;
  vecBandProp[ 0 ]->m_blkw = 100;
  vecBandProp[ 0 ]->m_nblocksx = 1;
  vecBandProp[ 0 ]->m_nblocksy = 1;
  vecBandProp[ 0 ]->m_noDataValue = 0;
  vecBandProp[ 0 ]->m_type = te::dt::UCHAR_TYPE;
  
  std::map<std::string, std::string> dsinfo;
  dsinfo["URI"] = "TsRastertcRasterize.tif";
  
  te::rst::Grid* grid = new te::rst::Grid( (unsigned int)
    vecBandProp[ 0 ]->m_blkw, (unsigned int)vecBandProp[ 0 ]->m_blkh,
    new te::gm::Envelope( *geomPtrs[ 0 ]->getMBR() ), geomPtrs[ 0 ]->getSRID() );
  
  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid,
    vecBandProp, dsinfo));
  
  std::vector< double > values;
  values.push_back( 1.0 );
  
  rst->rasterize( geomPtrs, values, 0 );
}

BOOST_AUTO_TEST_CASE (raste_clip_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop3_EPSG_22522.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtr ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtr.get() );
  
  // Creating polygon
  
  unsigned int nCols = inputRasterPtr->getNumberOfColumns();
  unsigned int nRows = inputRasterPtr->getNumberOfRows();
  
  double x = 0;
  double y = 0;
  
  std::unique_ptr< te::gm::LinearRing > linearRingPtr( new te::gm::LinearRing(4, 
    te::gm::LineStringType, inputRasterPtr->getSRID() ) );
  inputRasterPtr->getGrid()->gridToGeo( -0.5, -0.5, x, y ); 
  linearRingPtr->setPoint(0, x, y ); 
  inputRasterPtr->getGrid()->gridToGeo( (double)( nCols / 2 ) , -0.5, x, y ); 
  linearRingPtr->setPoint(1, x, y ); 
  inputRasterPtr->getGrid()->gridToGeo( -0.5 , (double)( nRows / 2 ), x, y ); 
  linearRingPtr->setPoint(2, x, y );   
  inputRasterPtr->getGrid()->gridToGeo( -0.5, -0.5, x, y ); 
  linearRingPtr->setPoint(3, x, y );
  
  std::unique_ptr< te::gm::Polygon > polygonPtr( new te::gm::Polygon(0, te::gm::PolygonType,
    inputRasterPtr->getSRID() ) );
  polygonPtr->add( linearRingPtr.release() );
  
  /* Clipping */  
  
  std::vector< te::gm::Geometry const *> geometries;
  geometries.push_back( polygonPtr.get() );
  
  auxRasterInfo["URI"] = "TsRaster_clipping.tif";
  boost::shared_ptr< te::rst::Raster > outputRasterPtr(
    inputRasterPtr->clip( geometries, auxRasterInfo, "GDAL" ) );
  BOOST_CHECK( outputRasterPtr.get() );
}

BOOST_AUTO_TEST_SUITE_END ()
