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
  \file terralib/unittest/rp/mosaic/TsMosaic.cpp

  \brief A test suit for the Mosaic interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE (mosaic_tests)

BOOST_AUTO_TEST_CASE(geoReferencedImagesMosaic_test)
{
  /* Openning input rasters */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/L5219075_07520040503_r3g2b1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster1Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/L5219076_07620040908_r3g2b1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/L71218075_07520070614_r3g2b1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster3Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/L71218076_07620060814_r3g2b1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster4Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster4Pointer.get() );
  
  /* Creating the algorithm parameters */
  
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
  
  algoInputParams.m_interpMethod = te::rst::NearestNeighbor;
  algoInputParams.m_noDataValue = 0;
  algoInputParams.m_forceInputNoDataValue = true;
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  algoInputParams.m_autoEqualize = true;

  te::rp::GeoMosaic::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =
    "terralib_unittest_rp_Mosaic_GeoReferencedImagesMosaic_Test.tif";
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::GeoMosaic algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(geoReferencedImagesMosaicWithReprojection_test)
{
  /* Openning input rasters */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster1Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop2.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop3_EPSG_22522.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster3Pointer.get() );
  
  /* Creating the algorithm parameters */
  
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
  
  algoInputParams.m_interpMethod = te::rst::NearestNeighbor;
  algoInputParams.m_noDataValue = 0;
  algoInputParams.m_forceInputNoDataValue = false;
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  algoInputParams.m_autoEqualize = true;

  te::rp::GeoMosaic::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =
    "terralib_unittest_rp_GeoReferencedImagesMosaicWithReprojectionTest_Test.tif";
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::GeoMosaic algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(tiePointsMosaic1_test)
{
  /* Openning input rasters */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster1Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop2.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop3_EPSG_22522.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster3Pointer.get() );
  
  /* Creating the algorithm parameters */
  
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
  
  /* Raster 0 to raster 1 tiePoints */

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
  
  /* Raster 1 to raster 2 tiePoints */

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
  
  algoInputParams.m_interpMethod = te::rst::NearestNeighbor;
  algoInputParams.m_noDataValue = 0;
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  algoInputParams.m_autoEqualize = true;

  te::rp::TiePointsMosaic::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =
    "terralib_unittest_rp_Mosaic_TiePointsMosaicTest1_Test.tif";
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::TiePointsMosaic algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(tiePointsMosaic2_test)
{
  /* Openning input rasters */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/jpeg/cbers_rgb342_crop1.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster1Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/jpeg/cbers_rgb342_crop2.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/jpeg/cbers_rgb342_crop3.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster3Pointer.get() );
  
  /* Creating the algorithm parameters */
  
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
  
  /* Raster 0 to raster 1 tiePoints */

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
  
  /* Raster 1 to raster 2 tiePoints */

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
  
  algoInputParams.m_interpMethod = te::rst::NearestNeighbor;
  algoInputParams.m_noDataValue = 0;
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  algoInputParams.m_autoEqualize = true;

  te::rp::TiePointsMosaic::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =
    "terralib_unittest_rp_Mosaic_TiePointsMosaicTest2_Test.tif";
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::TiePointsMosaic algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(sequenceMosaic_test)
{
  /* Openning input rasters */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/jpeg/cbers_rgb342_crop1.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster1Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/jpeg/cbers_rgb342_crop2.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/jpeg/cbers_rgb342_crop3.jpg";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRaster3Pointer.get() );
  
  /* Creating the algorithm parameters */
  
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
  
  algoInputParams.m_interpMethod = te::rst::NearestNeighbor;
  
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
  
  std::string connInfoRaster(".");
  std::unique_ptr< te::da::DataSource > dsPtr( te::da::DataSourceFactory::make("GDAL", connInfoRaster) );
  BOOST_CHECK( dsPtr.get() );
  dsPtr->open();
  BOOST_CHECK( dsPtr->isOpened() );
  algoOutputParams.m_outputDSPtr = dsPtr.get();
  
  /* Executing the algorithm */
  
  te::rp::SequenceMosaic algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
  
  BOOST_CHECK( algoOutputParams.m_sequencesInfo.size() == 2 );
  BOOST_CHECK( algoOutputParams.m_sequencesInfo[ 0 ].m_rasterFeederIndexes.size() == 1 );
  BOOST_CHECK( algoOutputParams.m_sequencesInfo[ 0 ].m_rasterFeederIndexes[ 0 ] == 0 );
  BOOST_CHECK( algoOutputParams.m_sequencesInfo[ 1 ].m_rasterFeederIndexes.size() == 2 );
  BOOST_CHECK( algoOutputParams.m_sequencesInfo[ 1 ].m_rasterFeederIndexes[ 0 ] == 1 );
  BOOST_CHECK( algoOutputParams.m_sequencesInfo[ 1 ].m_rasterFeederIndexes[ 1 ] == 2 );
  BOOST_CHECK( algoOutputParams.m_sequencesInfo[ 0 ].m_tiePoints.size() == 1 );
  BOOST_CHECK( algoOutputParams.m_sequencesInfo[ 1 ].m_tiePoints.size() == 2 );
}

BOOST_AUTO_TEST_SUITE_END()
