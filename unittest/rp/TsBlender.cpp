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

#include <terralib/rp/Blender.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/Band.h>
#include <terralib/raster/BandProperty.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/geometry/GTFactory.h>

#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION( TsBlender );

void TsBlender::NoBlendTest()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TE_DATA_DIR "/data/rasters/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TE_DATA_DIR "/data/rasters/cbers_rgb342_crop1.tif";
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
  
  const std::vector< double > pixelOffsets1( 3, 0.0 );
  const std::vector< double > pixelScales1( 3, 1.0 );
  const std::vector< double > pixelOffsets2( 3, 0.0 );
  const std::vector< double > pixelScales2( 3, 1.0 );
  
  te::gm::GTParameters transParams;
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 777, 748 ), te::gm::Coord2D( 485, 210 ) ) );
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 677, 648 ), te::gm::Coord2D( 385, 110 ) ) );     
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
    te::rst::Interpolator::NearestNeighbor,
    te::rst::Interpolator::NearestNeighbor,
    0.0,
    false,
    pixelOffsets1,
    pixelScales1,
    pixelOffsets2,
    pixelScales2,
    0,
    0,
    *transPtr) );
  
  // Creating the output image
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Blender_noBlend_Test.tif";
  
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
  
  std::vector< double > values( outRasterPtr->getNumberOfBands() );
  for( unsigned int row = 0 ; row < outRasterPtr->getNumberOfRows() ; ++row )
  {
    for( unsigned int col = 0 ; col < outRasterPtr->getNumberOfColumns() ; ++col )
    {
      blender.getBlendedValues( row, col, values );
      
      for( unsigned int band = 0 ; band < values.size() ; ++band )
      {
        outRasterPtr->setValue( col, row, values[ band ], band );
      }
    }
  }
}

void TsBlender::EucBlendTest()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TE_DATA_DIR "/data/rasters/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TE_DATA_DIR "/data/rasters/cbers_rgb342_crop1.tif";
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
  
  const std::vector< double > pixelOffsets1( 3, 0.0 );
  const std::vector< double > pixelScales1( 3, 1.0 );
  const std::vector< double > pixelOffsets2( 3, 0.0 );
  const std::vector< double > pixelScales2( 3, 1.0 );
  
  te::gm::GTParameters transParams;
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 777, 748 ), te::gm::Coord2D( 485, 210 ) ) );
  transParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
     te::gm::Coord2D( 677, 648 ), te::gm::Coord2D( 385, 110 ) ) );     
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
    te::rst::Interpolator::NearestNeighbor,
    te::rst::Interpolator::NearestNeighbor,
    0.0,
    false,
    pixelOffsets1,
    pixelScales1,
    pixelOffsets2,
    pixelScales2,
    0,
    0,
    *transPtr) );
  
  // Creating the output image
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Blender_EucBlend_Test.tif";
  
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
  
  std::vector< double > values( outRasterPtr->getNumberOfBands() );
  for( unsigned int row = 0 ; row < outRasterPtr->getNumberOfRows() ; ++row )
  {
    for( unsigned int col = 0 ; col < outRasterPtr->getNumberOfColumns() ; ++col )
    {
      blender.getBlendedValues( row, col, values );
      
      for( unsigned int band = 0 ; band < values.size() ; ++band )
      {
        outRasterPtr->setValue( col, row, values[ band ], band );
      }
    }
  }
}

