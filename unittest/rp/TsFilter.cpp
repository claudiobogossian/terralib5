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
  \file TsFilter.cpp

  \brief A test suit for the Filter interface.
 */

#include "TsFilter.h"
#include "../Config.h"

#include <terralib/rp/Filter.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterFactory.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsFilter );

void TsFilter::SobelFilter()
{
  // openning input raster

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );

  // Creating the algorithm parameters

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::SobelFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = TERRALIB_DATA_DIR "/rasters/terralib_unittest_rp_Filter_SobelFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm

  te::rp::Filter algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsFilter::RobertsFilter()
{
  // openning input raster

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );

  // Creating the algorithm parameters

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::RobertsFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = TERRALIB_DATA_DIR "/rasters/terralib_unittest_rp_Filter_RobertsFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm

  te::rp::Filter algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsFilter::MeanFilter()
{
  // openning input raster

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );

  // Creating the algorithm parameters

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::MeanFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = TERRALIB_DATA_DIR "/rasters/terralib_unittest_rp_Filter_MeanFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm

  te::rp::Filter algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsFilter::ModeFilter()
{
  // openning input raster

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );

  // Creating the algorithm parameters

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::ModeFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = TERRALIB_DATA_DIR "/rasters/terralib_unittest_rp_Filter_ModeFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm

  te::rp::Filter algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsFilter::MedianFilter()
{
  // openning input raster

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );

  // Creating the algorithm parameters

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::MedianFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = TERRALIB_DATA_DIR "/rasters/terralib_unittest_rp_Filter_MedianFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm

  te::rp::Filter algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsFilter::DilationFilter()
{
  // openning input raster

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );

  // Creating the algorithm parameters

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::DilationFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = TERRALIB_DATA_DIR "/rasters/terralib_unittest_rp_Filter_DilationFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm

  te::rp::Filter algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsFilter::ErosionFilter()
{
  // openning input raster

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );

  // Creating the algorithm parameters

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::ErosionFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = TERRALIB_DATA_DIR "/rasters/terralib_unittest_rp_Filter_ErosionFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm

  te::rp::Filter algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsFilter::UserDefinedWindow()
{
  // openning input raster

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );

  // Creating the algorithm parameters

  boost::numeric::ublas::matrix<double> window(3, 3);
  window(0, 0) = 1;
  window(0, 1) = 2;
  window(0, 2) = 1;
  window(1, 0) = 0;
  window(1, 1) = 0;
  window(1, 2) = 0;
  window(2, 0) = -1;
  window(2, 1) = -2;
  window(2, 2) = -1;

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::UserDefinedWindowT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 1;

  algoInputParams.m_window = window;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = TERRALIB_DATA_DIR "/rasters/terralib_unittest_rp_Filter_UserDefinedWindow.tif";
  algoOutputParams.m_rType = "GDAL";

  // Executing the algorithm

  te::rp::Filter algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

