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
  inputRasterInfo["URI"] = TE_DATA_LOCALE 
    "/data/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPointer.get() );
  
  // access a raster datasource to create the output raster
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_LOCALE 
    "/data/rasters/terralib_unittest_rp_Segmenter_BlockProcessingWithoutMerging_Test.tif";
  
  // Creating the algorithm parameters
  
  te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 0.3;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = true;
  algoInputParams.m_enableBlockMerging = false;
  algoInputParams.m_maxBlockSize = 0;
  algoInputParams.m_strategyName = "Dummy";
  algoInputParams.setSegStrategyParams( strategyParameters );
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::Segmenter algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsSegmenter::BlockProcessingWithMerging()
{
  // open input raster
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPointer.get() );
  
  // access a raster datasource to create the output raster
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_LOCALE 
    "/data/rasters/terralib_unittest_rp_Segmenter_BlockProcessingWithMerging_Test.tif";
  
  // Creating the algorithm parameters
  
  te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 0.3;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = true;
  algoInputParams.m_enableBlockMerging = true;
  algoInputParams.m_maxBlockSize = 0;
  algoInputParams.m_strategyName = "Dummy";
  algoInputParams.setSegStrategyParams( strategyParameters );
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";  
  
  // Executing the algorithm
  
  te::rp::Segmenter algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsSegmenter::ThreadedProcessing()
{
  // open input raster
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPointer.get() );
  
  // access a raster datasource to create the output raster
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_LOCALE 
    "/data/rasters/terralib_unittest_rp_Segmenter_ThreadedProcessing_Test.tif";
  
  // Creating the algorithm parameters
  
  te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 0.3;
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = true;
  algoInputParams.m_maxSegThreads = 4;
  algoInputParams.m_enableBlockProcessing = true;
  algoInputParams.m_enableBlockMerging = true;
  algoInputParams.m_maxBlockSize = 0;
  algoInputParams.m_strategyName = "Dummy";
  algoInputParams.setSegStrategyParams( strategyParameters );
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";   
  
  // Executing the algorithm
  
  te::rp::Segmenter algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsSegmenter::RegionGrowingStrategy()
{
  // open input raster
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";
  
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPointer.get() );
  
  // access a raster datasource to create the output raster
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = TE_DATA_LOCALE 
    "/data/rasters/terralib_unittest_rp_Segmenter_RegionGrowingStrategy_Test.tif";
  
  // Creating the algorithm parameters
  
  te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
  strategyParameters.m_minSegmentSize = 50;
  strategyParameters.m_segmentsSimilarityThreshold = 0.3;
  
  
  te::rp::Segmenter::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_maxSegThreads = 0;
  algoInputParams.m_enableBlockProcessing = false;
  algoInputParams.m_enableBlockMerging = false;
  algoInputParams.m_maxBlockSize = 0;
  algoInputParams.m_strategyName = "RegionGrowing";
  algoInputParams.setSegStrategyParams( strategyParameters );
  
  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = outputRasterInfo;
  algoOutputParams.m_rType = "GDAL";   
  
  // Executing the algorithm
  
  te::rp::Segmenter algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

