/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file TsBlender.cpp

  \brief A test suit for the Blender interface.
 */

#include "TsBlender.h"
#include "../Config.h"

#include <terralib/raster/Raster.h>
#include <terralib/raster/Band.h>
#include <terralib/raster/BandProperty.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/geometry/GTFactory.h>
#include <terralib/geometry/LinearRing.h>
#include <terralib/common/progress/ConsoleProgressViewer.h>
#include <terralib/common/progress/ProgressManager.h>

#include <boost/scoped_array.hpp>

#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION( TsBlender );

void TsBlender::PixelByPixelNoBlendTest()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  std::auto_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  // Creating blender parameters
  
  std::vector< unsigned int > raster1Bands;
  raster1Bands.push_back( 0 );
  raster1Bands.push_back( 1 );
  raster1Bands.push_back( 2 );
  
  std::vector< unsigned int > raster2Bands;
  raster2Bands.push_back( 1 );
  raster2Bands.push_back( 2 );
  raster2Bands.push_back( 0 );
  
  const std::vector< double > pixelOffsets( 3, 0.0 );
  const std::vector< double > pixelScales( 3, 1.0 );
  
  te::gm::GTParameters transParams;
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 292, 538 ), te::gm::Coord2D( 0, 0 ) ) );
  transParams.m_tiePoints.push_back( 
    te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 
        transParams.m_tiePoints[ 0 ].first.x + inputRaster2Pointer->getNumberOfColumns() - 1, 
        transParams.m_tiePoints[ 0 ].first.y + inputRaster2Pointer->getNumberOfRows() - 1
      ), 
      te::gm::Coord2D( 
        inputRaster2Pointer->getNumberOfColumns() - 1,
        inputRaster2Pointer->getNumberOfRows() - 1 ) 
      ) 
    );        
  std::auto_ptr< te::gm::GeometricTransformation > transPtr( 
    te::gm::GTFactory::make( "RST" ) );
  CPPUNIT_ASSERT( transPtr->initialize( transParams ) );    
  
  // Initiating the blender instance
    
  te::rp::Blender blender;
  
  CPPUNIT_ASSERT( blender.initialize(
    *inputRaster1Pointer,
    raster1Bands,
    *inputRaster2Pointer,
    raster2Bands,
    te::rp::Blender::NoBlendMethod,
    te::rst::NearestNeighbor,
    te::rst::NearestNeighbor,
    0.0,
    false,
    false,
    pixelOffsets,
    pixelScales,
    pixelOffsets,
    pixelScales,
    0,
    0,
    *transPtr,
    1,
    false ) );
  
  // Creating the output image
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Blender_PixelByPixelNoBlendTest_Test.tif";
  
  std::vector< te::rst::BandProperty* > bandsProperties;
  for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
    raster1Bands.size() ; ++inRasterBandsIdx )
  {
    bandsProperties.push_back( new te::rst::BandProperty(
      *( inputRaster1Pointer->getBand( raster1Bands[ inRasterBandsIdx ] )->getProperty() ) ) );
  }
  
  te::rst::Grid* gridPtr = new te::rst::Grid( *( inputRaster1Pointer->getGrid() ) );
  gridPtr->setNumberOfColumns( gridPtr->getNumberOfColumns() * 2 );
  gridPtr->setNumberOfRows( gridPtr->getNumberOfRows() * 2 );
  
  std::auto_ptr< te::rst::Raster > outRasterPtr( te::rst::RasterFactory::make(
    "GDAL",
    gridPtr,
    bandsProperties,
    outputRasterInfo,
    0,
    0 ) );
    
  CPPUNIT_ASSERT( outRasterPtr.get() );
  
  boost::scoped_array< double > values( new double[ outRasterPtr->getNumberOfBands() ] );
  for( unsigned int row = 0 ; row < outRasterPtr->getNumberOfRows() ; ++row )
  {
    for( unsigned int col = 0 ; col < outRasterPtr->getNumberOfColumns() ; ++col )
    {
      blender.getBlendedValues( row, col, values.get() );
      
      for( unsigned int band = 0 ; band < outRasterPtr->getNumberOfBands() ; ++band )
      {
        outRasterPtr->setValue( col, row, values[ band ], band );
      }
    }
  }
}

void TsBlender::EqualizationTest()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  std::auto_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  // Creating blender parameters
  
  std::vector< unsigned int > raster1Bands;
  raster1Bands.push_back( 0 );
  raster1Bands.push_back( 1 );
  raster1Bands.push_back( 2 );
  
  std::vector< unsigned int > raster2Bands;
  raster2Bands.push_back( 1 );
  raster2Bands.push_back( 2 );
  raster2Bands.push_back( 0 );
  
  std::vector< double > pixelOffsets1( raster1Bands.size(), 0.0 );
  std::vector< double > pixelScales1( raster1Bands.size(), 1.0 );
  std::vector< double > pixelOffsets2( raster1Bands.size(), 0.0 );
  std::vector< double > pixelScales2( raster1Bands.size(), 1.0 );
  std::vector< double > means1( raster1Bands.size() );
  std::vector< double > stddevs1( raster1Bands.size() );
  std::vector< double > means2( raster1Bands.size() );
  std::vector< double > stddevs2( raster1Bands.size() );
  
  for( unsigned int raster1BandsIdx = 0 ; raster1BandsIdx < raster1Bands.size() ;
    ++raster1BandsIdx )
  {
    unsigned int band1Idx = raster1Bands[ raster1BandsIdx ];
    unsigned int band2Idx = raster2Bands[ raster1BandsIdx ];
    
    te::rp::GetMeanValue( *inputRaster1Pointer->getBand( band1Idx ), 4, 
      means1[ raster1BandsIdx ] );
    te::rp::GetStdDevValue( *inputRaster1Pointer->getBand( band1Idx ), 4, 
      &means1[ raster1BandsIdx ], stddevs1[ raster1BandsIdx ] );
    
    te::rp::GetMeanValue( *inputRaster2Pointer->getBand( band2Idx ), 4, 
      means2[ raster1BandsIdx ] );
    te::rp::GetStdDevValue( *inputRaster2Pointer->getBand( band2Idx ), 4, 
      &means2[ raster1BandsIdx ], stddevs2[ raster1BandsIdx ] );    
    
    pixelScales2[ raster1BandsIdx ] = stddevs1[ raster1BandsIdx ] / 
      stddevs2[ raster1BandsIdx ];
    pixelOffsets2[ raster1BandsIdx ] = means1[ raster1BandsIdx ] - 
      ( pixelScales2[ raster1BandsIdx ] * means2[ raster1BandsIdx ] );        
  }
  
  te::gm::GTParameters transParams;
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 292, 538 ), te::gm::Coord2D( 0, 0 ) ) );
  transParams.m_tiePoints.push_back( 
    te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 
        transParams.m_tiePoints[ 0 ].first.x + inputRaster2Pointer->getNumberOfColumns() - 1, 
        transParams.m_tiePoints[ 0 ].first.y + inputRaster2Pointer->getNumberOfRows() - 1
      ), 
      te::gm::Coord2D( 
        inputRaster2Pointer->getNumberOfColumns() - 1,
        inputRaster2Pointer->getNumberOfRows() - 1 ) 
      ) 
    );        
  std::auto_ptr< te::gm::GeometricTransformation > transPtr( 
    te::gm::GTFactory::make( "RST" ) );
  CPPUNIT_ASSERT( transPtr->initialize( transParams ) );    
  
  // Initiating the blender instance
    
  te::rp::Blender blender;
  
  CPPUNIT_ASSERT( blender.initialize(
    *inputRaster1Pointer,
    raster1Bands,
    *inputRaster2Pointer,
    raster2Bands,
    te::rp::Blender::NoBlendMethod,
    te::rst::NearestNeighbor,
    te::rst::NearestNeighbor,
    0.0,
    false,
    false,
    pixelOffsets1,
    pixelScales1,
    pixelOffsets2,
    pixelScales2,
    0,
    0,
    *transPtr,
    1,
    false ) );
  
  // Creating the output image
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Blender_EqualizationTest_Test.tif";
  
  std::vector< te::rst::BandProperty* > bandsProperties;
  for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
    raster1Bands.size() ; ++inRasterBandsIdx )
  {
    bandsProperties.push_back( new te::rst::BandProperty(
      *( inputRaster1Pointer->getBand( raster1Bands[ inRasterBandsIdx ] )->getProperty() ) ) );
  }
  
  te::rst::Grid* gridPtr = new te::rst::Grid( *( inputRaster1Pointer->getGrid() ) );
  gridPtr->setNumberOfColumns( gridPtr->getNumberOfColumns() * 2 );
  gridPtr->setNumberOfRows( gridPtr->getNumberOfRows() * 2 );
  
  std::auto_ptr< te::rst::Raster > outRasterPtr( te::rst::RasterFactory::make(
    "GDAL",
    gridPtr,
    bandsProperties,
    outputRasterInfo,
    0,
    0 ) );
    
  CPPUNIT_ASSERT( outRasterPtr.get() );
  
  boost::scoped_array< double > values( new double[ outRasterPtr->getNumberOfBands() ] );
  for( unsigned int row = 0 ; row < outRasterPtr->getNumberOfRows() ; ++row )
  {
    for( unsigned int col = 0 ; col < outRasterPtr->getNumberOfColumns() ; ++col )
    {
      blender.getBlendedValues( row, col, values.get() );
      
      for( unsigned int band = 0 ; band < outRasterPtr->getNumberOfBands() ; ++band )
      {
        outRasterPtr->setValue( col, row, values[ band ], band );
      }
    }
  }
}

void TsBlender::PixelByPixelEucBlendTest()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  std::auto_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  // Creating blender parameters
  
  std::vector< unsigned int > raster1Bands;
  raster1Bands.push_back( 0 );
  raster1Bands.push_back( 1 );
  raster1Bands.push_back( 2 );
  
  std::vector< unsigned int > raster2Bands;
  raster2Bands.push_back( 1 );
  raster2Bands.push_back( 2 );
  raster2Bands.push_back( 0 );
  
  const std::vector< double > pixelOffsets( 3, 0.0 );
  const std::vector< double > pixelScales( 3, 1.0 );
  
  te::gm::GTParameters transParams;
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 292, 538 ), te::gm::Coord2D( 0, 0 ) ) );
  transParams.m_tiePoints.push_back( 
    te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 
        transParams.m_tiePoints[ 0 ].first.x + inputRaster2Pointer->getNumberOfColumns() - 1, 
        transParams.m_tiePoints[ 0 ].first.y + inputRaster2Pointer->getNumberOfRows() - 1
      ), 
      te::gm::Coord2D( 
        inputRaster2Pointer->getNumberOfColumns() - 1,
        inputRaster2Pointer->getNumberOfRows() - 1 ) 
      ) 
    );     
  std::auto_ptr< te::gm::GeometricTransformation > transPtr( 
    te::gm::GTFactory::make( "RST" ) );
  CPPUNIT_ASSERT( transPtr->initialize( transParams ) );    
  
  // Initiating the blender instance
    
  te::rp::Blender blender;
  
  CPPUNIT_ASSERT( blender.initialize(
    *inputRaster1Pointer,
    raster1Bands,
    *inputRaster2Pointer,
    raster2Bands,
    te::rp::Blender::EuclideanDistanceMethod,
    te::rst::NearestNeighbor,
    te::rst::NearestNeighbor,
    0.0,
    false,
    false,
    pixelOffsets,
    pixelScales,
    pixelOffsets,
    pixelScales,
    0,
    0,
    *transPtr,
    1,
    false ) );
  
  // Creating the output image
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Blender_PixelByPixelEucBlendTest_Test.tif";
  
  std::vector< te::rst::BandProperty* > bandsProperties;
  for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
    raster1Bands.size() ; ++inRasterBandsIdx )
  {
    bandsProperties.push_back( new te::rst::BandProperty(
      *( inputRaster1Pointer->getBand( raster1Bands[ inRasterBandsIdx ] )->getProperty() ) ) );
  }
  
  te::rst::Grid* gridPtr = new te::rst::Grid( *( inputRaster1Pointer->getGrid() ) );
  gridPtr->setNumberOfColumns( gridPtr->getNumberOfColumns() * 2 );
  gridPtr->setNumberOfRows( gridPtr->getNumberOfRows() * 2 );
  
  std::auto_ptr< te::rst::Raster > outRasterPtr( te::rst::RasterFactory::make(
    "GDAL",
    gridPtr,
    bandsProperties,
    outputRasterInfo,
    0,
    0 ) );
    
  CPPUNIT_ASSERT( outRasterPtr.get() );
  
  boost::scoped_array< double > values( new double[ outRasterPtr->getNumberOfBands() ] );
  for( unsigned int row = 0 ; row < outRasterPtr->getNumberOfRows() ; ++row )
  {
    for( unsigned int col = 0 ; col < outRasterPtr->getNumberOfColumns() ; ++col )
    {
      blender.getBlendedValues( row, col, values.get() );
      
      for( unsigned int band = 0 ; band < outRasterPtr->getNumberOfBands() ; ++band )
      {
        outRasterPtr->setValue( col, row, values[ band ], band );
      }
    }
  }
}

void TsBlender::PixelByPixelSumBlendTest()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  std::auto_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  // Creating blender parameters
  
  std::vector< unsigned int > raster1Bands;
  raster1Bands.push_back( 0 );
  raster1Bands.push_back( 1 );
  raster1Bands.push_back( 2 );
  
  std::vector< unsigned int > raster2Bands;
  raster2Bands.push_back( 1 );
  raster2Bands.push_back( 2 );
  raster2Bands.push_back( 0 );
  
  const std::vector< double > pixelOffsets( 3, 0.0 );
  const std::vector< double > pixelScales( 3, 0.5 );
  
  te::gm::GTParameters transParams;
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 292, 538 ), te::gm::Coord2D( 0, 0 ) ) );
  transParams.m_tiePoints.push_back( 
    te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 
        transParams.m_tiePoints[ 0 ].first.x + inputRaster2Pointer->getNumberOfColumns() - 1, 
        transParams.m_tiePoints[ 0 ].first.y + inputRaster2Pointer->getNumberOfRows() - 1
      ), 
      te::gm::Coord2D( 
        inputRaster2Pointer->getNumberOfColumns() - 1,
        inputRaster2Pointer->getNumberOfRows() - 1 ) 
      ) 
    );     
  std::auto_ptr< te::gm::GeometricTransformation > transPtr( 
    te::gm::GTFactory::make( "RST" ) );
  CPPUNIT_ASSERT( transPtr->initialize( transParams ) );    
  
  // Initiating the blender instance
    
  te::rp::Blender blender;
  
  CPPUNIT_ASSERT( blender.initialize(
    *inputRaster1Pointer,
    raster1Bands,
    *inputRaster2Pointer,
    raster2Bands,
    te::rp::Blender::SumMethod,
    te::rst::NearestNeighbor,
    te::rst::NearestNeighbor,
    0.0,
    false,
    false,
    pixelOffsets,
    pixelScales,
    pixelOffsets,
    pixelScales,
    0,
    0,
    *transPtr,
    1,
    false ) );
  
  // Creating the output image
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Blender_PixelByPixelSumBlendTest_Test.tif";
  
  std::vector< te::rst::BandProperty* > bandsProperties;
  for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
    raster1Bands.size() ; ++inRasterBandsIdx )
  {
    bandsProperties.push_back( new te::rst::BandProperty(
      *( inputRaster1Pointer->getBand( raster1Bands[ inRasterBandsIdx ] )->getProperty() ) ) );
  }
  
  te::rst::Grid* gridPtr = new te::rst::Grid( *( inputRaster1Pointer->getGrid() ) );
  gridPtr->setNumberOfColumns( gridPtr->getNumberOfColumns() * 2 );
  gridPtr->setNumberOfRows( gridPtr->getNumberOfRows() * 2 );
  
  std::auto_ptr< te::rst::Raster > outRasterPtr( te::rst::RasterFactory::make(
    "GDAL",
    gridPtr,
    bandsProperties,
    outputRasterInfo,
    0,
    0 ) );
    
  CPPUNIT_ASSERT( outRasterPtr.get() );
  
  boost::scoped_array< double > values( new double[ outRasterPtr->getNumberOfBands() ] );
  for( unsigned int row = 0 ; row < outRasterPtr->getNumberOfRows() ; ++row )
  {
    for( unsigned int col = 0 ; col < outRasterPtr->getNumberOfColumns() ; ++col )
    {
      blender.getBlendedValues( row, col, values.get() );
      
      for( unsigned int band = 0 ; band < outRasterPtr->getNumberOfBands() ; ++band )
      {
        outRasterPtr->setValue( col, row, values[ band ], band );
      }
    }
  }
}

void TsBlender::FullRasterBlendTest()
{
  // Progress interface
  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );  
  
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  std::auto_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  // Creating the valid area delimiters
  
  std::auto_ptr< te::gm::MultiPolygon > r1ValidDataDelimiterPtr(
    new te::gm::MultiPolygon( 0, te::gm::MultiPolygonType, 
    inputRaster1Pointer->getSRID(), 0 ) );
  {
    const double& LLX = inputRaster1Pointer->getGrid()->getExtent()->m_llx;
    const double& LLY = inputRaster1Pointer->getGrid()->getExtent()->m_lly;
    const double& URX = inputRaster1Pointer->getGrid()->getExtent()->m_urx;
    const double& URY = inputRaster1Pointer->getGrid()->getExtent()->m_ury;
    const int& SRID =  inputRaster1Pointer->getSRID();
    
    te::gm::LinearRing* ringPtr = new te::gm::LinearRing( 5, 
       te::gm::LineStringType, SRID, 0 );  
    ringPtr = new te::gm::LinearRing(5, te::gm::LineStringType, SRID);
    ringPtr->setPoint( 0, LLX, URY );
    ringPtr->setPoint( 1, URX, URY );
    ringPtr->setPoint( 2, URX, LLY );
    ringPtr->setPoint( 3, LLX, LLY );
    ringPtr->setPoint( 4, LLX, URY );    
    
    te::gm::Polygon* polPtr = new te::gm::Polygon( 0, te::gm::PolygonType, SRID, 0 );           
    polPtr->add( ringPtr );
       
    r1ValidDataDelimiterPtr->add( polPtr );
  }
  
  std::auto_ptr< te::gm::MultiPolygon > r2ValidDataDelimiterPtr(
    new te::gm::MultiPolygon( 0, te::gm::MultiPolygonType, 
    inputRaster2Pointer->getSRID(), 0 ) );
  {
    const double& LLX = inputRaster2Pointer->getGrid()->getExtent()->m_llx;
    const double& LLY = inputRaster2Pointer->getGrid()->getExtent()->m_lly;
    const double& URX = inputRaster2Pointer->getGrid()->getExtent()->m_urx;
    const double& URY = inputRaster2Pointer->getGrid()->getExtent()->m_ury;
    const int& SRID =  inputRaster2Pointer->getSRID();
    
    te::gm::LinearRing* ringPtr = new te::gm::LinearRing( 5, 
       te::gm::LineStringType, SRID, 0 );  
    ringPtr = new te::gm::LinearRing(5, te::gm::LineStringType, SRID);
    ringPtr->setPoint( 0, LLX, URY );
    ringPtr->setPoint( 1, URX, URY );
    ringPtr->setPoint( 2, URX, LLY );
    ringPtr->setPoint( 3, LLX, LLY );
    ringPtr->setPoint( 4, LLX, URY );    
    
    te::gm::Polygon* polPtr = new te::gm::Polygon( 0, te::gm::PolygonType, SRID, 0 );           
    polPtr->add( ringPtr );
       
    r2ValidDataDelimiterPtr->add( polPtr );
  }  
  
  // Creating the output image
  
  std::auto_ptr< te::rst::Raster > outRasterPtr;
  
  {
    std::map<std::string, std::string> outputRasterInfo;
    outputRasterInfo["URI"] = "terralib_unittest_rp_Blender_FullRasterBlendTest_Test.tif";
    
    te::rst::Grid* gridPtr = new te::rst::Grid( 
      *( inputRaster1Pointer->getGrid() ) );
    gridPtr->setNumberOfColumns( inputRaster1Pointer->getGrid()->getNumberOfColumns() * 2 );
    gridPtr->setNumberOfRows( inputRaster1Pointer->getGrid()->getNumberOfRows() * 2 );    
    
    std::vector< te::rst::BandProperty* > bandsProperties;
    for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
      inputRaster1Pointer->getNumberOfBands() ; ++inRasterBandsIdx )
    {
      bandsProperties.push_back( new te::rst::BandProperty(
        *( inputRaster1Pointer->getBand( 0 )->getProperty() ) ) );
      bandsProperties[ inRasterBandsIdx ]->m_blkh = 1;
      bandsProperties[ inRasterBandsIdx ]->m_blkw = gridPtr->getNumberOfColumns();
      bandsProperties[ inRasterBandsIdx ]->m_nblocksx = 1;
      bandsProperties[ inRasterBandsIdx ]->m_nblocksy = gridPtr->getNumberOfRows();
      bandsProperties[ inRasterBandsIdx ]->m_noDataValue = 0.0;
    }
    
    outRasterPtr.reset( te::rst::RasterFactory::make(
      "GDAL",
      gridPtr,
      bandsProperties,
      outputRasterInfo,
      0,
      0 ) );
    CPPUNIT_ASSERT( outRasterPtr.get() );
    
    double uLX = 0;
    double uLY = 0;
    inputRaster1Pointer->getGrid()->gridToGeo( 0, 0, uLX, uLY );
    
    double outULCol = 0;
    double outULRow = 0;    
    outRasterPtr->getGrid()->geoToGrid( uLX, uLY, outULCol, outULRow );
    
    const unsigned int inputNRows = inputRaster1Pointer->getNumberOfRows();
    const unsigned int inputNCols = inputRaster1Pointer->getNumberOfColumns();
    const unsigned int outputNRows = outRasterPtr->getNumberOfRows();
    const unsigned int outputNCols = outRasterPtr->getNumberOfColumns();    
    unsigned int row = 0;
    unsigned int col = 0;
    double value = 0;
    double outNoDataValue = 0;
    
    for( unsigned int band = 0; band < inputRaster1Pointer->getNumberOfBands();
      ++band )
    {
      outNoDataValue = outRasterPtr->getBand( band )->getProperty()->m_noDataValue;
      
      for( row = 0 ; row < outputNRows ; ++row )
      {
        for( col = 0; col < outputNCols ; ++col )
        {
          outRasterPtr->setValue( col, row, outNoDataValue, band );
        }
      }      
      
      for( row = 0 ; row < inputNRows ; ++row )
      {
        for( col = 0; col < inputNCols ; ++col )
        {
          inputRaster1Pointer->getValue( col, row, value, band );
          outRasterPtr->setValue((unsigned int)(col + outULCol), (unsigned int)(row + outULRow), value, band );
        }
      }
    }
  }
  
  // Creating blender parameters
  
  std::vector< unsigned int > raster1Bands;
  raster1Bands.push_back( 0 );
  raster1Bands.push_back( 1 );
  raster1Bands.push_back( 2 );
  
  std::vector< unsigned int > raster2Bands;
  raster2Bands.push_back( 1 );
  raster2Bands.push_back( 2 );
  raster2Bands.push_back( 0 );
  
  const std::vector< double > pixelOffsets( 3, 0.0 );
  const std::vector< double > pixelScales( 3, 1.0 );
 
  te::gm::GTParameters transParams;
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 292, 538 ), te::gm::Coord2D( 0, 0 ) ) );
  transParams.m_tiePoints.push_back( 
    te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 
        transParams.m_tiePoints[ 0 ].first.x + inputRaster2Pointer->getNumberOfColumns() - 1, 
        transParams.m_tiePoints[ 0 ].first.y + inputRaster2Pointer->getNumberOfRows() - 1
      ), 
      te::gm::Coord2D( 
        inputRaster2Pointer->getNumberOfColumns() - 1,
        inputRaster2Pointer->getNumberOfRows() - 1 ) 
      ) 
    );        
  std::auto_ptr< te::gm::GeometricTransformation > transPtr( 
    te::gm::GTFactory::make( "RST" ) );
  CPPUNIT_ASSERT( transPtr->initialize( transParams ) );    
  
  // Initiating the blender instance
    
  te::rp::Blender blender;
  
  CPPUNIT_ASSERT( blender.initialize(
    *outRasterPtr,
    raster1Bands,
    *inputRaster2Pointer,
    raster2Bands,
    te::rp::Blender::NoBlendMethod,
    te::rst::NearestNeighbor,
    te::rst::NearestNeighbor,
    0.0,
    false,
    false,
    pixelOffsets,
    pixelScales,
    pixelOffsets,
    pixelScales,
    r1ValidDataDelimiterPtr.get(),
    r2ValidDataDelimiterPtr.get(),
    *transPtr,
    1,
    true ) );
  
  CPPUNIT_ASSERT( blender.blendIntoRaster1() );
  
  te::common::ProgressManager::getInstance().removeViewer( viewerId ); 
}

void TsBlender::ThreadedFullRasterBlendTest()
{
  // Progress interface
  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );  
  
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  std::auto_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  // Creating the valid area delimiters
  
  std::auto_ptr< te::gm::MultiPolygon > r1ValidDataDelimiterPtr(
    new te::gm::MultiPolygon( 0, te::gm::MultiPolygonType, 
    inputRaster1Pointer->getSRID(), 0 ) );
  {
    const double& LLX = inputRaster1Pointer->getGrid()->getExtent()->m_llx;
    const double& LLY = inputRaster1Pointer->getGrid()->getExtent()->m_lly;
    const double& URX = inputRaster1Pointer->getGrid()->getExtent()->m_urx;
    const double& URY = inputRaster1Pointer->getGrid()->getExtent()->m_ury;
    const int& SRID =  inputRaster1Pointer->getSRID();
    
    te::gm::LinearRing* ringPtr = new te::gm::LinearRing( 5, 
       te::gm::LineStringType, SRID, 0 );  
    ringPtr = new te::gm::LinearRing(5, te::gm::LineStringType, SRID);
    ringPtr->setPoint( 0, LLX, URY );
    ringPtr->setPoint( 1, URX, URY );
    ringPtr->setPoint( 2, URX, LLY );
    ringPtr->setPoint( 3, LLX, LLY );
    ringPtr->setPoint( 4, LLX, URY );    
    
    te::gm::Polygon* polPtr = new te::gm::Polygon( 0, te::gm::PolygonType, SRID, 0 );           
    polPtr->add( ringPtr );
       
    r1ValidDataDelimiterPtr->add( polPtr );
  }
  
  std::auto_ptr< te::gm::MultiPolygon > r2ValidDataDelimiterPtr(
    new te::gm::MultiPolygon( 0, te::gm::MultiPolygonType, 
    inputRaster2Pointer->getSRID(), 0 ) );
  {
    const double& LLX = inputRaster2Pointer->getGrid()->getExtent()->m_llx;
    const double& LLY = inputRaster2Pointer->getGrid()->getExtent()->m_lly;
    const double& URX = inputRaster2Pointer->getGrid()->getExtent()->m_urx;
    const double& URY = inputRaster2Pointer->getGrid()->getExtent()->m_ury;
    const int& SRID =  inputRaster2Pointer->getSRID();
    
    te::gm::LinearRing* ringPtr = new te::gm::LinearRing( 5, 
       te::gm::LineStringType, SRID, 0 );  
    ringPtr = new te::gm::LinearRing(5, te::gm::LineStringType, SRID);
    ringPtr->setPoint( 0, LLX, URY );
    ringPtr->setPoint( 1, URX, URY );
    ringPtr->setPoint( 2, URX, LLY );
    ringPtr->setPoint( 3, LLX, LLY );
    ringPtr->setPoint( 4, LLX, URY );    
    
    te::gm::Polygon* polPtr = new te::gm::Polygon( 0, te::gm::PolygonType, SRID, 0 );           
    polPtr->add( ringPtr );
       
    r2ValidDataDelimiterPtr->add( polPtr );
  }   
  
  // Creating the output image
  
  std::auto_ptr< te::rst::Raster > outRasterPtr;
  
  {
    std::map<std::string, std::string> outputRasterInfo;
    outputRasterInfo["URI"] = "terralib_unittest_rp_Blender_ThreadedFullRasterBlendTest_Test.tif";
    
    te::rst::Grid* gridPtr = new te::rst::Grid( *( inputRaster1Pointer->getGrid() ) );
    gridPtr->setNumberOfColumns( inputRaster1Pointer->getGrid()->getNumberOfColumns() * 2 );
    gridPtr->setNumberOfRows( inputRaster1Pointer->getGrid()->getNumberOfRows() * 2 );    
    
    std::vector< te::rst::BandProperty* > bandsProperties;
    for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx <
      inputRaster1Pointer->getNumberOfBands() ; ++inRasterBandsIdx )
    {
      bandsProperties.push_back( new te::rst::BandProperty(
        *( inputRaster1Pointer->getBand( 0 )->getProperty() ) ) );
      bandsProperties[ inRasterBandsIdx ]->m_blkh = 1;
      bandsProperties[ inRasterBandsIdx ]->m_blkw = gridPtr->getNumberOfColumns();
      bandsProperties[ inRasterBandsIdx ]->m_nblocksx = 1;
      bandsProperties[ inRasterBandsIdx ]->m_nblocksy = gridPtr->getNumberOfRows();      
      bandsProperties[ inRasterBandsIdx ]->m_noDataValue = 0.0;
    }
        
    outRasterPtr.reset( te::rst::RasterFactory::make(
      "GDAL",
      gridPtr,
      bandsProperties,
      outputRasterInfo,
      0,
      0 ) );
    CPPUNIT_ASSERT( outRasterPtr.get() );
    
    double uLX = 0;
    double uLY = 0;
    inputRaster1Pointer->getGrid()->gridToGeo( 0, 0, uLX, uLY );
    
    double outULCol = 0;
    double outULRow = 0;    
    outRasterPtr->getGrid()->geoToGrid( uLX, uLY, outULCol, outULRow );
    
    const unsigned int inputNRows = inputRaster1Pointer->getNumberOfRows();
    const unsigned int inputNCols = inputRaster1Pointer->getNumberOfColumns();
    const unsigned int outputNRows = outRasterPtr->getNumberOfRows();
    const unsigned int outputNCols = outRasterPtr->getNumberOfColumns();    
    unsigned int row = 0;
    unsigned int col = 0;
    double value = 0;
    double outNoDataValue = 0;
    
    for( unsigned int band = 0; band < inputRaster1Pointer->getNumberOfBands();
      ++band )
    {
      outNoDataValue = outRasterPtr->getBand( band )->getProperty()->m_noDataValue;
      
      for( row = 0 ; row < outputNRows ; ++row )
      {
        for( col = 0; col < outputNCols ; ++col )
        {
          outRasterPtr->setValue( col, row, outNoDataValue, band );
        }
      }      
      
      for( row = 0 ; row < inputNRows ; ++row )
      {
        for( col = 0; col < inputNCols ; ++col )
        {
          inputRaster1Pointer->getValue( col, row, value, band );
          outRasterPtr->setValue( (unsigned int)(col + outULCol), (unsigned int)(row + outULRow), value, band );
        }
      }
    }
  }
  
  // Creating blender parameters
  
  std::vector< unsigned int > raster1Bands;
  raster1Bands.push_back( 0 );
  raster1Bands.push_back( 1 );
  raster1Bands.push_back( 2 );
  
  std::vector< unsigned int > raster2Bands;
  raster2Bands.push_back( 1 );
  raster2Bands.push_back( 2 );
  raster2Bands.push_back( 0 );
  
  const std::vector< double > pixelOffsets( 3, 0.0 );
  const std::vector< double > pixelScales( 3, 1.0 );
  
  te::gm::GTParameters transParams;
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 292, 538 ), te::gm::Coord2D( 0, 0 ) ) );
  transParams.m_tiePoints.push_back( 
    te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 
        transParams.m_tiePoints[ 0 ].first.x + inputRaster2Pointer->getNumberOfColumns() - 1, 
        transParams.m_tiePoints[ 0 ].first.y + inputRaster2Pointer->getNumberOfRows() - 1
      ), 
      te::gm::Coord2D( 
        inputRaster2Pointer->getNumberOfColumns() - 1,
        inputRaster2Pointer->getNumberOfRows() - 1 ) 
      ) 
    );        
  std::auto_ptr< te::gm::GeometricTransformation > transPtr( 
    te::gm::GTFactory::make( "RST" ) );
  CPPUNIT_ASSERT( transPtr->initialize( transParams ) );    
  
  // Initiating the blender instance
    
  te::rp::Blender blender;
  
  CPPUNIT_ASSERT( blender.initialize(
    *outRasterPtr,
    raster1Bands,
    *inputRaster2Pointer,
    raster2Bands,
    te::rp::Blender::NoBlendMethod,
    te::rst::NearestNeighbor,
    te::rst::NearestNeighbor,
    0.0,
    false,
    false,
    pixelOffsets,
    pixelScales,
    pixelOffsets,
    pixelScales,
    r1ValidDataDelimiterPtr.get(),
    r2ValidDataDelimiterPtr.get(),
    *transPtr,
    0,
    true ) );
  
  CPPUNIT_ASSERT( blender.blendIntoRaster1() );
  
  te::common::ProgressManager::getInstance().removeViewer( viewerId ); 
}
