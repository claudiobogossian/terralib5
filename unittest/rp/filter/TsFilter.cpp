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
  \file terralib/unittest/rp/filter/TsFilter.cpp

  \brief A test suit for the Filter interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/raster.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE (filter_tests)

BOOST_AUTO_TEST_CASE(sobelFilter_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtrPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::SobelFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = "terralib_unittest_rp_Filter_SobelFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  /* Executing the algorithm */

  te::rp::Filter algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}


BOOST_AUTO_TEST_CASE(robertsFilter_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtrPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::RobertsFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = "terralib_unittest_rp_Filter_RobertsFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  /* Executing the algorithm */

  te::rp::Filter algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(meanFilter_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtrPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::MeanFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = "terralib_unittest_rp_Filter_MeanFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  /* Executing the algorithm */

  te::rp::Filter algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}


BOOST_AUTO_TEST_CASE(modeFilter_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtrPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::ModeFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = "terralib_unittest_rp_Filter_ModeFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  /* Executing the algorithm */

  te::rp::Filter algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(medianFilter_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtrPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::MedianFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = "terralib_unittest_rp_Filter_MedianFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  /* Executing the algorithm */

  te::rp::Filter algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(dilationFilter_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtrPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::DilationFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] =  "terralib_unittest_rp_Filter_DilationFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  /* Executing the algorithm */

  te::rp::Filter algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(erosionFilter_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtrPointer.get() );

  /* Creating the algorithm parameters */

  te::rp::Filter::InputParameters algoInputParams;

  algoInputParams.m_filterType = te::rp::Filter::InputParameters::ErosionFilterT;

  algoInputParams.m_inRasterPtr = inputRasterPtrPointer.get();

  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );

  algoInputParams.m_iterationsNumber = 4;

  te::rp::Filter::OutputParameters algoOutputParams;

  algoOutputParams.m_rInfo["URI"] = "terralib_unittest_rp_Filter_ErosionFilter.tif";
  algoOutputParams.m_rType = "GDAL";

  /* Executing the algorithm */

  te::rp::Filter algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(userDefinedWindow_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtrPointer.get() );

  /* Creating the algorithm parameters */

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

  algoOutputParams.m_rInfo["URI"] = "terralib_unittest_rp_Filter_UserDefinedWindow.tif";
  algoOutputParams.m_rType = "GDAL";

  /* Executing the algorithm */

  te::rp::Filter algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_SUITE_END()
