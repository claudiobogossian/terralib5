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
  \file terralib/unittest/rp/segmenter/TsSegmenter.cpp

  \brief A test suit for Segmenter interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/rp/Segmenter.h>
#include <terralib/rp/SegmenterRegionGrowingBaatzStrategy.h>
#include <terralib/rp/SegmenterRegionGrowingMeanStrategy.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/common/progress/ConsoleProgressViewer.h>
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>

// STL
#include <cmath>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE (segmenter_tests)

BOOST_AUTO_TEST_CASE(dummyStrategy_test)
{
  /* Progress interface */

  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );
  
  /* Open input raster */
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Access a raster datasource to create the output raster */

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Segmenter_DummyStrategy_Test.tif";
  
  /* Creating the algorithm parameters */
  
  te::rp::SegmenterRegionGrowingMeanStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 0.1;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = false;
  algoInputParams.m_blocksOverlapPercent = 0;
  algoInputParams.m_maxBlockSize = 0;
  algoInputParams.m_strategyName = "Dummy";
  algoInputParams.setSegStrategyParams( strategyParameters );
  algoInputParams.m_enableProgress = true;
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::Segmenter algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
  
  te::common::ProgressManager::getInstance().removeViewer( viewerId );
}

BOOST_AUTO_TEST_CASE(blockProcessingWithoutMerging_test)
{
  /* Progress interface */

  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );
  
  /* Open input raster */
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );
  
  /* Access a raster datasource to create the output raster */
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Segmenter_BlockProcessingWithoutMerging_Test.tif";
  
  /* Creating the algorithm parameters */
  
  te::rp::SegmenterRegionGrowingMeanStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 0.1;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = true;
  algoInputParams.m_blocksOverlapPercent = 0;
  algoInputParams.m_maxBlockSize = 100;
  algoInputParams.m_strategyName = "Dummy";
  algoInputParams.setSegStrategyParams( strategyParameters );
  algoInputParams.m_enableProgress = true;
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::Segmenter algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
  
  te::common::ProgressManager::getInstance().removeViewer( viewerId );
}

BOOST_AUTO_TEST_CASE(blockProcessingWithMerging_test)
{
  /* Progress interface */

  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );
  
  /* Open input raster */
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );
  
  /* Access a raster datasource to create the output raster */
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Segmenter_BlockProcessingWithMerging_Test.tif";
  
  /* Creating the algorithm parameters */
  
  te::rp::SegmenterRegionGrowingMeanStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 0.1;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = true;
  algoInputParams.m_blocksOverlapPercent = 20;
  algoInputParams.m_maxBlockSize = 100;
  algoInputParams.m_strategyName = "Dummy";
  algoInputParams.setSegStrategyParams( strategyParameters );
  algoInputParams.m_enableProgress = true;
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::Segmenter algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
  
  te::common::ProgressManager::getInstance().removeViewer( viewerId );
}

BOOST_AUTO_TEST_CASE(threadedProcessing_test)
{
  /* Progress interface */

  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );
  
  /* Open input raster */
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );
  
  /* Access a raster datasource to create the output raster */
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Segmenter_ThreadedProcessing_Test.tif";
  
  /* Creating the algorithm parameters */
  
  te::rp::SegmenterRegionGrowingMeanStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 0.1;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = true;
  algoInputParams.m_maxSegThreads = 4;
  algoInputParams.m_enableBlockProcessing = true;
  algoInputParams.m_blocksOverlapPercent = 20;
  algoInputParams.m_maxBlockSize = 100;
  algoInputParams.m_strategyName = "Dummy";
  algoInputParams.setSegStrategyParams( strategyParameters );
  algoInputParams.m_enableProgress = true;
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::Segmenter algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
  
  te::common::ProgressManager::getInstance().removeViewer( viewerId );
}

BOOST_AUTO_TEST_CASE(regionGrowingMeanStrategy_test)
{
  /* Progress interface */

  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );
  
  /* Open input raster */
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );
  
  /* Access a raster datasource to create the output raster */
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Segmenter_RegionGrowingMeanStrategy_Test.tif";
  
  /* Creating the algorithm parameters */
  
  te::rp::SegmenterRegionGrowingMeanStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 100;
  strategyParameters.m_segmentsSimilarityThreshold = 0.03;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = false;
  algoInputParams.m_blocksOverlapPercent = 0;
  algoInputParams.m_maxBlockSize = 0;
  algoInputParams.m_strategyName = "RegionGrowingMean";
  algoInputParams.setSegStrategyParams( strategyParameters );
  algoInputParams.m_enableProgress = true;
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::Segmenter algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
  
  te::common::ProgressManager::getInstance().removeViewer( viewerId );
}

BOOST_AUTO_TEST_CASE(regionGrowingMeanStrategyBlockProcessing_test)
{
  /* Progress interface */

  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );
  
  /* Open input raster */
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );
  
  /* Access a raster datasource to create the output raster */
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Segmenter_RegionGrowingMeanStrategyBlockProcessing_Test.tif";
  
  /* Creating the algorithm parameters */
  
  te::rp::SegmenterRegionGrowingMeanStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 100;
  strategyParameters.m_segmentsSimilarityThreshold = 0.1;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = true;
  algoInputParams.m_blocksOverlapPercent = 20;
  algoInputParams.m_maxBlockSize = 100;
  algoInputParams.m_strategyName = "RegionGrowingMean";
  algoInputParams.setSegStrategyParams( strategyParameters );
  algoInputParams.m_enableProgress = true;
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::Segmenter algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
  
  te::common::ProgressManager::getInstance().removeViewer( viewerId );
}

BOOST_AUTO_TEST_CASE(regionGrowingBaatzStrategy_test)
{
  /* Progress interface */

  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );
  
  /* Open input raster */
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );
  
  /* Access a raster datasource to create the output raster */
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Segmenter_RegionGrowingBaatzStrategy_Test.tif";
  
  /* Creating the algorithm parameters */
  
  te::rp::SegmenterRegionGrowingBaatzStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 100;
  strategyParameters.m_segmentsSimilarityThreshold = 0.5;
  strategyParameters.m_bandsWeights.resize(
    (unsigned int)inputRasterPointer->getNumberOfBands(),
    1.0 / ((double)inputRasterPointer->getNumberOfBands()) );
  strategyParameters.m_colorWeight = 0.9;
  strategyParameters.m_compactnessWeight = 0.5;
  strategyParameters.m_segmentsSimIncreaseSteps = 10;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = false;
  algoInputParams.m_blocksOverlapPercent = 0;
  algoInputParams.m_maxBlockSize = 0;
  algoInputParams.m_strategyName = "RegionGrowingBaatz";
  algoInputParams.setSegStrategyParams( strategyParameters );
  algoInputParams.m_enableProgress = true;
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::Segmenter algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  te::common::ProgressManager::getInstance().removeViewer( viewerId );
}

BOOST_AUTO_TEST_CASE(regionGrowingBaatzStrategyBlockProcessing_test)
{
  /* Progress interface */

  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );
  
  /* Open input raster */
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );
  
  /* Access a raster datasource to create the output raster */
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Segmenter_RegionGrowingBaatzStrategyBlockProcessing_Test.tif";
  
  /* Creating the algorithm parameters */
  
  te::rp::SegmenterRegionGrowingBaatzStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 100;
  strategyParameters.m_segmentsSimilarityThreshold = 0.03;
  strategyParameters.m_bandsWeights.resize(
    (unsigned int)inputRasterPointer->getNumberOfBands(),
    1.0 / ((double)inputRasterPointer->getNumberOfBands()) );
  strategyParameters.m_colorWeight = 1.0;
  strategyParameters.m_compactnessWeight = 0.5;
  strategyParameters.m_segmentsSimIncreaseSteps = 10;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = true;
  algoInputParams.m_blocksOverlapPercent = 20;
  algoInputParams.m_maxBlockSize = 100;
  algoInputParams.m_strategyName = "RegionGrowingBaatz";
  algoInputParams.setSegStrategyParams( strategyParameters );
  algoInputParams.m_enableProgress = true;
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";
  
  /* Executing the algorithm */
  
  te::rp::Segmenter algorithmInstance;
  
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );

  te::common::ProgressManager::getInstance().removeViewer( viewerId );
}

BOOST_AUTO_TEST_SUITE_END()
