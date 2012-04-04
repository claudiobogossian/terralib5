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
  \file TsSegmenter.cpp
 
  \brief A test suit for the Segmenter interface.
 */

#include "TsSegmenter.h"
#include "../Config.h"

#include <terralib/rp/Segmenter.h>
#include <terralib/rp/SegmenterRegionGrowingStrategy.h>
#include <terralib/raster/RasterFactory.h>

#include <terralib/dataaccess/datasource/DataSourceFactory.h>

#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsSegmenter );

void TsSegmenter::BlockProcessingWithoutMerging()
{
  // open input raster
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPointer.get() );
  
  // access a raster datasource to create the output raster
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters";
  
  boost::shared_ptr< te::da::DataSource > outDataSourcePtr( 
    te::da::DataSourceFactory::make("GDAL") );
  CPPUNIT_ASSERT( outDataSourcePtr.get() );
  
  outDataSourcePtr->open(outputRasterInfo);  
  CPPUNIT_ASSERT( outDataSourcePtr->isOpened() );
  
  // Creating the algorithm parameters
  
  te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 30;
  
  
  te::rp::Segmenter::Parameters algoParams;
  algoParams.m_inRasterPtr = inputRasterPointer.get();
  algoParams.m_inRasterBands.push_back( 0 );
  algoParams.m_inRasterBands.push_back( 1 );
  algoParams.m_inRasterBands.push_back( 2 );
  algoParams.m_outRasterPtr = 0;
  algoParams.m_outRasterBand = 0;
  algoParams.m_outDataSourcePtr = outDataSourcePtr.get();
  algoParams.m_outDataSetName = 
    "terralib_unittest_rp_Segmenter_BlockProcessingWithoutMerging_Test.tif";
  algoParams.m_enableThreadedProcessing = false;
  algoParams.m_maxSegThreads = 0;
  algoParams.m_enableBlockProcessing = true;
  algoParams.m_enableBlockMerging = false;
  algoParams.m_maxBlockSize = 0;
  algoParams.m_strategyName = "Dummy";
  algoParams.setSegStrategyParams( strategyParameters );
  
  // Executing the algorithm
  
  te::rp::Segmenter algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute() );
}

void TsSegmenter::BlockProcessingWithMerging()
{
  // open input raster
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPointer.get() );
  
  // access a raster datasource to create the output raster
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters";
  
  boost::shared_ptr< te::da::DataSource > outDataSourcePtr( 
    te::da::DataSourceFactory::make("GDAL") );
  CPPUNIT_ASSERT( outDataSourcePtr.get() );
  
  outDataSourcePtr->open(outputRasterInfo);  
  CPPUNIT_ASSERT( outDataSourcePtr->isOpened() );
  
  // Creating the algorithm parameters
  
  te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 30;
  
  
  te::rp::Segmenter::Parameters algoParams;
  algoParams.m_inRasterPtr = inputRasterPointer.get();
  algoParams.m_inRasterBands.push_back( 0 );
  algoParams.m_inRasterBands.push_back( 1 );
  algoParams.m_inRasterBands.push_back( 2 );
  algoParams.m_outRasterPtr = 0;
  algoParams.m_outRasterBand = 0;
  algoParams.m_outDataSourcePtr = outDataSourcePtr.get();
  algoParams.m_outDataSetName = 
    "terralib_unittest_rp_Segmenter_BlockProcessingWithMerging_Test.tif";
  algoParams.m_enableThreadedProcessing = false;
  algoParams.m_maxSegThreads = 0;
  algoParams.m_enableBlockProcessing = true;
  algoParams.m_enableBlockMerging = true;
  algoParams.m_maxBlockSize = 0;
  algoParams.m_strategyName = "Dummy";
  algoParams.setSegStrategyParams( strategyParameters );
  
  // Executing the algorithm
  
  te::rp::Segmenter algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute() );
}

void TsSegmenter::ThreadedProcessing()
{
  // open input raster
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPointer.get() );
  
  // access a raster datasource to create the output raster
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters";
  
  boost::shared_ptr< te::da::DataSource > outDataSourcePtr( 
    te::da::DataSourceFactory::make("GDAL") );
  CPPUNIT_ASSERT( outDataSourcePtr.get() );
  
  outDataSourcePtr->open(outputRasterInfo);  
  CPPUNIT_ASSERT( outDataSourcePtr->isOpened() );
  
  // Creating the algorithm parameters
  
  te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 30;
  
  
  te::rp::Segmenter::Parameters algoParams;
  algoParams.m_inRasterPtr = inputRasterPointer.get();
  algoParams.m_inRasterBands.push_back( 0 );
  algoParams.m_inRasterBands.push_back( 1 );
  algoParams.m_inRasterBands.push_back( 2 );
  algoParams.m_outRasterPtr = 0;
  algoParams.m_outRasterBand = 0;
  algoParams.m_outDataSourcePtr = outDataSourcePtr.get();
  algoParams.m_outDataSetName = 
    "terralib_unittest_rp_Segmenter_ThreadedProcessing_Test.tif";
  algoParams.m_enableThreadedProcessing = true;
  algoParams.m_maxSegThreads = 4;
  algoParams.m_enableBlockProcessing = true;
  algoParams.m_enableBlockMerging = true;
  algoParams.m_maxBlockSize = 0;
  algoParams.m_strategyName = "Dummy";
  algoParams.setSegStrategyParams( strategyParameters );
  
  // Executing the algorithm
  
  te::rp::Segmenter algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute() );
}

void TsSegmenter::RegionGrowingStrategy()
{
  // open input raster
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPointer.get() );
  
  // access a raster datasource to create the output raster
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters";
  
  boost::shared_ptr< te::da::DataSource > outDataSourcePtr( 
    te::da::DataSourceFactory::make("GDAL") );
  CPPUNIT_ASSERT( outDataSourcePtr.get() );
  
  outDataSourcePtr->open(outputRasterInfo);  
  CPPUNIT_ASSERT( outDataSourcePtr->isOpened() );
  
  // Creating the algorithm parameters
  
  te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 30;
  
  
  te::rp::Segmenter::Parameters algoParams;
  algoParams.m_inRasterPtr = inputRasterPointer.get();
  algoParams.m_inRasterBands.push_back( 0 );
  algoParams.m_inRasterBands.push_back( 1 );
  algoParams.m_inRasterBands.push_back( 2 );
  algoParams.m_outRasterPtr = 0;
  algoParams.m_outRasterBand = 0;
  algoParams.m_outDataSourcePtr = outDataSourcePtr.get();
  algoParams.m_outDataSetName = 
    "terralib_unittest_rp_Segmenter_RegionGrowingStrategy_Test.tif";
  algoParams.m_enableThreadedProcessing = false;
  algoParams.m_maxSegThreads = 0;
  algoParams.m_enableBlockProcessing = false;
  algoParams.m_enableBlockMerging = false;
  algoParams.m_maxBlockSize = 0;
  algoParams.m_strategyName = "RegionGrowing";
  algoParams.setSegStrategyParams( strategyParameters );
  
  // Executing the algorithm
  
  te::rp::Segmenter algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute() );
}

