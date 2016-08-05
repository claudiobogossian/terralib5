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
  \file terralib/unittest/rp/tie_points_locator/TsTiePointsLocator.cpp

  \brief A test suit for TiePointsLocator interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/rp/TiePointsLocator.h>
#include <terralib/rp/TiePointsLocatorMoravecStrategy.h>
#include <terralib/rp/TiePointsLocatorSURFStrategy.h>
#include <terralib/raster/Enums.h>
#include <terralib/raster/BandProperty.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/Band.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE (tiePointsLocator_tests)

void saveImagesAndTiePoints(

  const te::rst::Raster& raster1,
  const unsigned int band1,
  const te::rst::Raster& raster2,
  const unsigned int band2,
  const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
  const std::string& tifFileNameBeginning )
{
  std::map<std::string, std::string> rInfo1;
  rInfo1["URI"] = tifFileNameBeginning + "_raster1.tif";

  std::vector<te::rst::BandProperty*> bandsProperties1;
  bandsProperties1.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
  bandsProperties1[0]->m_colorInterp = te::rst::RedCInt;
  bandsProperties1[0]->m_noDataValue = 0;
  bandsProperties1.push_back(new te::rst::BandProperty( *bandsProperties1[0] ));
  bandsProperties1[1]->m_colorInterp = te::rst::GreenCInt;
  bandsProperties1.push_back(new te::rst::BandProperty( *bandsProperties1[0] ));
  bandsProperties1[2]->m_colorInterp = te::rst::BlueCInt;

  te::rst::Grid* newgrid1 = new te::rst::Grid( *raster1.getGrid() );

  std::auto_ptr< te::rst::Raster > outputRaster1Ptr(
    te::rst::RasterFactory::make( "GDAL", newgrid1, bandsProperties1, rInfo1, 0, 0));
  BOOST_CHECK( outputRaster1Ptr.get() );

  unsigned int line = 0;
  unsigned int col = 0;
  double value = 0;

  for( line = 0 ; line < raster1.getNumberOfRows() ; ++line )
    for( col = 0 ; col < raster1.getNumberOfColumns() ; ++col )
    {
      raster1.getValue( col, line, value, band1 );
      outputRaster1Ptr->setValue( col, line, value, 0 );
      outputRaster1Ptr->setValue( col, line, value, 1 );
      outputRaster1Ptr->setValue( col, line, value, 2 );
    }

  std::map<std::string, std::string> rInfo2;
  rInfo2["URI"] = tifFileNameBeginning + "_raster2.tif";

  std::vector<te::rst::BandProperty*> bandsProperties2;
  bandsProperties2.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
  bandsProperties2[0]->m_colorInterp = te::rst::RedCInt;
  bandsProperties2[0]->m_noDataValue = 0;
  bandsProperties2.push_back(new te::rst::BandProperty( *bandsProperties2[0] ));
  bandsProperties2[1]->m_colorInterp = te::rst::GreenCInt;
  bandsProperties2.push_back(new te::rst::BandProperty( *bandsProperties2[0] ));
  bandsProperties2[2]->m_colorInterp = te::rst::BlueCInt;

  te::rst::Grid* newgrid2 = new te::rst::Grid( *raster2.getGrid() );

  std::auto_ptr< te::rst::Raster > outputRaster2Ptr(
    te::rst::RasterFactory::make( "GDAL", newgrid2, bandsProperties2, rInfo2, 0, 0));
  BOOST_CHECK( outputRaster2Ptr.get() );

  for( line = 0 ; line < raster2.getNumberOfRows() ; ++line )
    for( col = 0 ; col < raster2.getNumberOfColumns() ; ++col )
    {
      raster2.getValue( col, line, value, band2 );
      outputRaster2Ptr->setValue( col, line, value, 0 );
      outputRaster2Ptr->setValue( col, line, value, 1 );
      outputRaster2Ptr->setValue( col, line, value, 2 );
    }

  std::vector< te::gm::GTParameters::TiePoint >::const_iterator itB = tiePoints.begin();
  const std::vector< te::gm::GTParameters::TiePoint >::const_iterator itE =
    tiePoints.end();

  while( itB != itE )
  {
    BOOST_CHECK( ((unsigned int)(itB->first.x)) <  outputRaster1Ptr->getNumberOfColumns() );
    BOOST_CHECK( ((unsigned int)(itB->first.y)) <  outputRaster1Ptr->getNumberOfRows() );
    outputRaster1Ptr->setValue( (unsigned int)(itB->first.x), (unsigned int)(itB->first.y), 255, 1 );

    BOOST_CHECK( ((unsigned int)(itB->second.x)) <  outputRaster2Ptr->getNumberOfColumns() );
    BOOST_CHECK( ((unsigned int)(itB->second.y)) <  outputRaster2Ptr->getNumberOfRows() );
    outputRaster2Ptr->setValue( (unsigned int)(itB->second.x), (unsigned int)(itB->second.y), 255, 1 );

    ++itB;
  }
}

BOOST_AUTO_TEST_CASE(moravecStrategySameImage_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorMoravecStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "Moravec";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 1000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRasterPointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_MoravecStrategySameImage" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)866 );
}

BOOST_AUTO_TEST_CASE(moravecStrategyUpsampledImage_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  std::map<std::string, std::string> inputRaster2Info;
  inputRaster2Info["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop_upsampled.tif";

  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    inputRaster2Info ) );
  BOOST_CHECK( inputRaster2Pointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorMoravecStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "Moravec";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRaster2Pointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 1000;
  algoInputParams.m_pixelSizeXRelation = inputRasterPointer->getResolutionX() /
   inputRaster2Pointer->getResolutionX();
  algoInputParams.m_pixelSizeYRelation = inputRasterPointer->getResolutionY() /
   inputRaster2Pointer->getResolutionY();
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRaster2Pointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_MoravecStrategyUpsampledImage" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)866 );
}

BOOST_AUTO_TEST_CASE(moravecStrategy90ClkWRotationImage_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  std::map<std::string, std::string> inputRaster2Info;
  inputRaster2Info["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop_90clockw_rotation.tif";

  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    inputRaster2Info ) );
  BOOST_CHECK( inputRaster2Pointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorMoravecStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "Moravec";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRaster2Pointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 1000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRaster2Pointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_MoravecStrategy90ClkWRotationImage" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)854 );
}

BOOST_AUTO_TEST_CASE(moravecStrategyHalfRotated90Image_test)
{
  /* Openning input rasters */

  std::map<std::string, std::string> inputRaster1Info;
  inputRaster1Info["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    inputRaster1Info ) );
  BOOST_CHECK( inputRaster1Pointer.get() );

  std::map<std::string, std::string> inputRaster2Info;
  inputRaster2Info["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop_contrast_halfsampled_90rotation.tif";

  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    inputRaster2Info ) );
  BOOST_CHECK( inputRaster2Pointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorMoravecStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "Moravec";
  algoInputParams.m_inRaster1Ptr = inputRaster1Pointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRaster2Pointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_pixelSizeXRelation = 20.0 / 40.0;
  algoInputParams.m_pixelSizeYRelation = 20.0 / 40.0;
  algoInputParams.m_maxTiePoints = 1000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRaster1Pointer, 0, *inputRaster2Pointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_MoravecStrategyHalfRotated90Image" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)200 );
}

BOOST_AUTO_TEST_CASE(moravecStrategySameImageDifBoxes_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorMoravecStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "Moravec";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_raster1TargetAreaColStart = 200;
  algoInputParams.m_raster1TargetAreaLineStart = 200;
  algoInputParams.m_raster1TargetAreaWidth = 500;
  algoInputParams.m_raster1TargetAreaHeight = 500;
  algoInputParams.m_raster2TargetAreaColStart = 300;
  algoInputParams.m_raster2TargetAreaLineStart = 300;
  algoInputParams.m_raster2TargetAreaWidth = 400;
  algoInputParams.m_raster2TargetAreaHeight = 400;
  algoInputParams.m_maxTiePoints = 1000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRasterPointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_MoravecStrategySameImageDifBoxes" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)189 );
}

BOOST_AUTO_TEST_CASE(moravecStrategyMultipleThreads_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorMoravecStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "Moravec";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = true;
  algoInputParams.m_maxTiePoints = 1000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRasterPointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_MoravecStrategyMultipleThreads" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)700 );
}

BOOST_AUTO_TEST_CASE(moravecStrategyRescaleFactor_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers2b_hrc_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorMoravecStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "Moravec";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 1000;
  algoInputParams.m_subSampleOptimizationRescaleFactor = 0.5;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRasterPointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_MoravecStrategyRescaleFactor" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)992 );
}

BOOST_AUTO_TEST_CASE(surfStrategySameImage_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorSURFStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "SURF";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 2000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRasterPointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_SurfStrategySameImage" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)1500 );
}

BOOST_AUTO_TEST_CASE(surfStrategyUpsampledImage_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  std::map<std::string, std::string> inputRaster2Info;
  inputRaster2Info["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop_upsampled.tif";

  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    inputRaster2Info ) );
  BOOST_CHECK( inputRaster2Pointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorSURFStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "SURF";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRaster2Pointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 2000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.m_pixelSizeXRelation = inputRasterPointer->getResolutionX() /
   inputRaster2Pointer->getResolutionX();
  algoInputParams.m_pixelSizeYRelation = inputRasterPointer->getResolutionY() /
   inputRaster2Pointer->getResolutionY();
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRaster2Pointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_SurfStrategyUpsampledImage" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)1700 );
}

BOOST_AUTO_TEST_CASE(surfStrategy90ClkWRotationImage_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  std::map<std::string, std::string> inputRaster2Info;
  inputRaster2Info["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop_90clockw_rotation.tif";

  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    inputRaster2Info ) );
  BOOST_CHECK( inputRaster2Pointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorSURFStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "SURF";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRaster2Pointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 2000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRaster2Pointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_SurfStrategy90ClkWRotationImage" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)1268 );
}

BOOST_AUTO_TEST_CASE(surfStrategySameImageDifBoxes_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorSURFStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "SURF";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_raster1TargetAreaColStart = 0;
  algoInputParams.m_raster1TargetAreaLineStart = 0;
  algoInputParams.m_raster1TargetAreaWidth = 700;
  algoInputParams.m_raster1TargetAreaHeight = 600;
  algoInputParams.m_raster2TargetAreaColStart = 20;
  algoInputParams.m_raster2TargetAreaLineStart = 30;
  algoInputParams.m_raster2TargetAreaWidth = 600;
  algoInputParams.m_raster2TargetAreaHeight = 700;
  algoInputParams.m_maxTiePoints = 2000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRasterPointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_SurfStrategySameImageDifBoxes" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)1081 );
}

BOOST_AUTO_TEST_CASE(surfStrategyHalfRotated90Image_test)
{
  /* Openning input rasters */

  std::map<std::string, std::string> inputRaster1Info;
  inputRaster1Info["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    inputRaster1Info ) );
  BOOST_CHECK( inputRaster1Pointer.get() );

  std::map<std::string, std::string> inputRaster2Info;
  inputRaster2Info["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop_contrast_halfsampled_90rotation.tif";

  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    inputRaster2Info ) );
  BOOST_CHECK( inputRaster2Pointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorSURFStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "SURF";
  algoInputParams.m_inRaster1Ptr = inputRaster1Pointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRaster2Pointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 3000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.m_pixelSizeXRelation = 20.0 / 40.0;
  algoInputParams.m_pixelSizeYRelation = 20.0 / 40.0;
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRaster1Pointer, 0, *inputRaster2Pointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_SurfStrategyHalfRotated90Image" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)882 );
}

BOOST_AUTO_TEST_CASE(surfStrategyMultipleThreads_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers_b2_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorSURFStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "SURF";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = true;
  algoInputParams.m_maxTiePoints = 2000;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRasterPointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_SurfStrategyMultipleThreads" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)400 );
}

BOOST_AUTO_TEST_CASE(surfStrategyRescaleFactor_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR
    "/rasters/cbers2b_hrc_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::TiePointsLocatorSURFStrategy::Parameters specPars;

  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategyName = "SURF";
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 0 );
  algoInputParams.m_inRaster2Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 0 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 2000;
  algoInputParams.m_subSampleOptimizationRescaleFactor = 0.5;
  algoInputParams.m_geomTransfName = "RST";
  algoInputParams.setSpecStrategyParams( specPars );

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  /* Executing the algorithm */

  te::rp::TiePointsLocator algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  /* Saving images and tie-points */

  saveImagesAndTiePoints( *inputRasterPointer, 0, *inputRasterPointer, 0,
    algoOutputParams.m_tiePoints, "terralib_rp_tiepointslocator_test_SurfStrategyRescaleFactor" );

  BOOST_CHECK( algoOutputParams.m_tiePoints.size() >= (size_t)1540 );
}
 BOOST_AUTO_TEST_SUITE_END()
