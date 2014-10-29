/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file TsMosaic.cpp

  \brief A test suit for the Mosaic interface.
 */

#include "TsMosaic.h"
#include "../Config.h"

#include <terralib/rp/TiePointsMosaic.h>
#include <terralib/rp/GeoMosaic.h>
#include <terralib/rp/SequenceMosaic.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>

#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsMosaic );

void TsMosaic::GeoReferencedImagesMosaicTest()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/L5219075_07520040503_r3g2b1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/L5219076_07620040908_r3g2b1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/L71218075_07520070614_r3g2b1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster3Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/L71218076_07620060814_r3g2b1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster4Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster4Pointer.get() );    
  
    
  // Creating the algorithm parameters
  
  te::rp::GeoMosaic::InputParameters algoInputParams;
  
  std::vector< const te::rst::Raster* > rasters;
  rasters.push_back( inputRaster1Pointer.get() );
  rasters.push_back( inputRaster2Pointer.get() );
  rasters.push_back( inputRaster3Pointer.get() );
  rasters.push_back( inputRaster4Pointer.get() );
  te::rp::FeederConstRasterVector feeder( rasters );
  algoInputParams.m_feederRasterPtr = &feeder;
  
  std::vector< unsigned int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  algoInputParams.m_inputRastersBands.push_back( bands );
  algoInputParams.m_inputRastersBands.push_back( bands );
  algoInputParams.m_inputRastersBands.push_back( bands );
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  algoInputParams.m_interpMethod = te::rst::Interpolator::NearestNeighbor;
  algoInputParams.m_noDataValue = 0;
  algoInputParams.m_forceInputNoDataValue = true;
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  algoInputParams.m_autoEqualize = true;  

  te::rp::GeoMosaic::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =  
    "terralib_unittest_rp_Mosaic_GeoReferencedImagesMosaic_Test.tif";  
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::GeoMosaic algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsMosaic::GeoReferencedImagesMosaicWithReprojectionTest()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop3_EPSG_22522.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster3Pointer.get() );    
  
    
  // Creating the algorithm parameters
  
  te::rp::GeoMosaic::InputParameters algoInputParams;
  
  std::vector< const te::rst::Raster* > rasters;
  rasters.push_back( inputRaster1Pointer.get() );
  rasters.push_back( inputRaster2Pointer.get() );
  rasters.push_back( inputRaster3Pointer.get() );
  te::rp::FeederConstRasterVector feeder( rasters );
  algoInputParams.m_feederRasterPtr = &feeder;
  
  std::vector< unsigned int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 0 ] = 1;
  bands[ 1 ] = 2;
  bands[ 2 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 2 ] = 2;
  bands[ 0 ] = 1;
  bands[ 1 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  algoInputParams.m_interpMethod = te::rst::Interpolator::NearestNeighbor;
  algoInputParams.m_noDataValue = 0;
  algoInputParams.m_forceInputNoDataValue = false;
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  algoInputParams.m_autoEqualize = true;  

  te::rp::GeoMosaic::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =  
    "terralib_unittest_rp_GeoReferencedImagesMosaicWithReprojectionTest_Test.tif";  
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::GeoMosaic algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsMosaic::TiePointsMosaicTest1()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop3_EPSG_22522.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster3Pointer.get() );    
  
    
  // Creating the algorithm parameters
  
  te::rp::TiePointsMosaic::InputParameters algoInputParams;
  
  std::vector< const te::rst::Raster* > rasters;
  rasters.push_back( inputRaster1Pointer.get() );
  rasters.push_back( inputRaster2Pointer.get() );
  rasters.push_back( inputRaster3Pointer.get() );
  te::rp::FeederConstRasterVector feeder( rasters );
  algoInputParams.m_feederRasterPtr = &feeder;
  
  std::vector< unsigned int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 0 ] = 1;
  bands[ 1 ] = 2;
  bands[ 2 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 2 ] = 2;
  bands[ 0 ] = 1;
  bands[ 1 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  // raster 0 to raster 1 tiePoints
  std::vector< te::gm::GTParameters::TiePoint > r0ToR1tiePoints( 3 );
  r0ToR1tiePoints[ 0 ].first.x = 40;
  r0ToR1tiePoints[ 0 ].first.y = 38;
  r0ToR1tiePoints[ 0 ].second.x = 334;
  r0ToR1tiePoints[ 0 ].second.y = 575;
  r0ToR1tiePoints[ 1 ].first.x = 465;
  r0ToR1tiePoints[ 1 ].first.y = 71;
  r0ToR1tiePoints[ 1 ].second.x = 759;
  r0ToR1tiePoints[ 1 ].second.y = 608;
  r0ToR1tiePoints[ 2 ].first.x = 496;
  r0ToR1tiePoints[ 2 ].first.y = 429;
  r0ToR1tiePoints[ 2 ].second.x = 789;
  r0ToR1tiePoints[ 2 ].second.y = 964;  
  algoInputParams.m_tiePoints.push_back( r0ToR1tiePoints );
  
  // raster 1 to raster 2 tiePoints
  std::vector< te::gm::GTParameters::TiePoint > r1ToR2tiePoints( 3 );
  r1ToR2tiePoints[ 0 ].first.x = 476;
  r1ToR2tiePoints[ 0 ].first.y = 358;
  r1ToR2tiePoints[ 0 ].second.x = 4;
  r1ToR2tiePoints[ 0 ].second.y = 183;
  r1ToR2tiePoints[ 1 ].first.x = 590;
  r1ToR2tiePoints[ 1 ].first.y = 930;
  r1ToR2tiePoints[ 1 ].second.x = 117;
  r1ToR2tiePoints[ 1 ].second.y = 754;
  r1ToR2tiePoints[ 2 ].first.x = 717;
  r1ToR2tiePoints[ 2 ].first.y = 666;
  r1ToR2tiePoints[ 2 ].second.x = 244;
  r1ToR2tiePoints[ 2 ].second.y = 492;  
  algoInputParams.m_tiePoints.push_back( r1ToR2tiePoints );
  
  algoInputParams.m_tiePointsLinkType = 
    te::rp::TiePointsMosaic::InputParameters::AdjacentRastersLinkingTiePointsT;
  
  algoInputParams.m_interpMethod = te::rst::Interpolator::NearestNeighbor;
  algoInputParams.m_noDataValue = 0;
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  algoInputParams.m_autoEqualize = true;  

  te::rp::TiePointsMosaic::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =  
    "terralib_unittest_rp_Mosaic_TiePointsMosaicTest1_Test.tif";  
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::TiePointsMosaic algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsMosaic::TiePointsMosaicTest2()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop3.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster3Pointer.get() );    
  
    
  // Creating the algorithm parameters
  
  te::rp::TiePointsMosaic::InputParameters algoInputParams;
  
  std::vector< const te::rst::Raster* > rasters;
  rasters.push_back( inputRaster1Pointer.get() );
  rasters.push_back( inputRaster2Pointer.get() );
  rasters.push_back( inputRaster3Pointer.get() );
  te::rp::FeederConstRasterVector feeder( rasters );
  algoInputParams.m_feederRasterPtr = &feeder;
  
  std::vector< unsigned int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 0 ] = 1;
  bands[ 1 ] = 2;
  bands[ 2 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 2 ] = 2;
  bands[ 0 ] = 1;
  bands[ 1 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  // raster 0 to raster 1 tiePoints
  std::vector< te::gm::GTParameters::TiePoint > r0ToR1tiePoints( 3 );
  r0ToR1tiePoints[ 0 ].first.x = 40;
  r0ToR1tiePoints[ 0 ].first.y = 38;
  r0ToR1tiePoints[ 0 ].second.x = 334;
  r0ToR1tiePoints[ 0 ].second.y = 575;
  r0ToR1tiePoints[ 1 ].first.x = 465;
  r0ToR1tiePoints[ 1 ].first.y = 71;
  r0ToR1tiePoints[ 1 ].second.x = 759;
  r0ToR1tiePoints[ 1 ].second.y = 608;
  r0ToR1tiePoints[ 2 ].first.x = 496;
  r0ToR1tiePoints[ 2 ].first.y = 429;
  r0ToR1tiePoints[ 2 ].second.x = 789;
  r0ToR1tiePoints[ 2 ].second.y = 964;  
  algoInputParams.m_tiePoints.push_back( r0ToR1tiePoints );
  
  // raster 1 to raster 2 tiePoints
  std::vector< te::gm::GTParameters::TiePoint > r1ToR2tiePoints( 3 );
  r1ToR2tiePoints[ 0 ].first.x = 476;
  r1ToR2tiePoints[ 0 ].first.y = 358;
  r1ToR2tiePoints[ 0 ].second.x = 4;
  r1ToR2tiePoints[ 0 ].second.y = 183;
  r1ToR2tiePoints[ 1 ].first.x = 590;
  r1ToR2tiePoints[ 1 ].first.y = 930;
  r1ToR2tiePoints[ 1 ].second.x = 117;
  r1ToR2tiePoints[ 1 ].second.y = 754;
  r1ToR2tiePoints[ 2 ].first.x = 717;
  r1ToR2tiePoints[ 2 ].first.y = 666;
  r1ToR2tiePoints[ 2 ].second.x = 244;
  r1ToR2tiePoints[ 2 ].second.y = 492;  
  algoInputParams.m_tiePoints.push_back( r1ToR2tiePoints );
  
  algoInputParams.m_tiePointsLinkType = 
    te::rp::TiePointsMosaic::InputParameters::AdjacentRastersLinkingTiePointsT;
  
  algoInputParams.m_interpMethod = te::rst::Interpolator::NearestNeighbor;
  algoInputParams.m_noDataValue = 0;
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  algoInputParams.m_autoEqualize = true;  

  te::rp::TiePointsMosaic::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =  
    "terralib_unittest_rp_Mosaic_TiePointsMosaicTest2_Test.tif";  
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::TiePointsMosaic algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsMosaic::SequenceMosaicTest()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop2.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop3.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster3Pointer.get() );    
  
    
  // Creating the algorithm parameters
  
  te::rp::SequenceMosaic::InputParameters algoInputParams;
  
  std::vector< const te::rst::Raster* > rasters;
  rasters.push_back( inputRaster1Pointer.get() );
  rasters.push_back( inputRaster2Pointer.get() );
  rasters.push_back( inputRaster3Pointer.get() );
  te::rp::FeederConstRasterVector feeder( rasters );
  algoInputParams.m_feederRasterPtr = &feeder;
  
  std::vector< unsigned int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 0 ] = 1;
  bands[ 1 ] = 2;
  bands[ 2 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 2 ] = 2;
  bands[ 0 ] = 1;
  bands[ 1 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  algoInputParams.m_geomTransfName = "Affine";
  
  algoInputParams.m_interpMethod = te::rst::Interpolator::NearestNeighbor;
  
  algoInputParams.m_noDataValue = 0.0;
  
  algoInputParams.m_forceInputNoDataValue = false;
  
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  
  algoInputParams.m_autoEqualize = true;
  
  algoInputParams.m_useRasterCache = true;
  
  algoInputParams.m_enableMultiThread = true;
  
  algoInputParams.m_enableProgress = true;
  
  algoInputParams.m_locatorParams.m_geomTransfMaxError = 1.0;
  
  algoInputParams.m_tiePointsLocationBandIndex = 0;
  
  algoInputParams.m_locatorParams.m_maxTiePoints = 500;
  
  algoInputParams.m_outDataSetsNamePrefix = 
    "terralib_unittest_rp_Mosaic_SequenceMosaic_Test_";
    
  algoInputParams.m_outDataSetsNameSufix = ".tif";
    
  algoInputParams.m_minRequiredTiePointsCoveredAreaPercent = 1;
  
  te::rp::SequenceMosaic::OutputParameters algoOutputParams;
  
  std::map<std::string, std::string> connInfoRaster;
  connInfoRaster["SOURCE"] = ".";
  std::auto_ptr< te::da::DataSource > dsPtr( te::da::DataSourceFactory::make("GDAL") );
  CPPUNIT_ASSERT( dsPtr.get() );
  dsPtr->setConnectionInfo( connInfoRaster );
  dsPtr->open();
  CPPUNIT_ASSERT( dsPtr->isOpened() );
  algoOutputParams.m_outputDSPtr = dsPtr.get();
  
  // Executing the algorithm
  
  te::rp::SequenceMosaic algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
  
  CPPUNIT_ASSERT( algoOutputParams.m_sequencesInfo.size() == 2 );
  CPPUNIT_ASSERT( algoOutputParams.m_sequencesInfo[ 0 ].m_rasterFeederIndexes.size() == 1 );
  CPPUNIT_ASSERT( algoOutputParams.m_sequencesInfo[ 0 ].m_rasterFeederIndexes[ 0 ] == 0 );
  CPPUNIT_ASSERT( algoOutputParams.m_sequencesInfo[ 1 ].m_rasterFeederIndexes.size() == 2 );
  CPPUNIT_ASSERT( algoOutputParams.m_sequencesInfo[ 1 ].m_rasterFeederIndexes[ 0 ] == 1 );
  CPPUNIT_ASSERT( algoOutputParams.m_sequencesInfo[ 1 ].m_rasterFeederIndexes[ 1 ] == 2 );
  CPPUNIT_ASSERT( algoOutputParams.m_sequencesInfo[ 0 ].m_tiePoints.size() == 1 );
  CPPUNIT_ASSERT( algoOutputParams.m_sequencesInfo[ 1 ].m_tiePoints.size() == 2 );
}

