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
  \file terralib/unittest/rp/contrast/TsContrast.cpp
 
  \brief A test suit for the Contrast interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/raster.h>
#include <terralib/dataaccess.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE (contrast_tests)

BOOST_AUTO_TEST_CASE(linearContrast_test)
{
  /* Open input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Access a raster datasource to create the output raster */

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Contrast_LinearnoOptimization_Test.tif";

  /* Creating the algorithm parameters */

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::LinearContrastT;
  algoInputParams.m_lCMinInput.resize( 3, 50 );
  algoInputParams.m_lCMaxInput.resize( 3, 200 );
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_createdOutRasterInfo = outputRasterInfo;
  algoOutputParams.m_createdOutRasterDSType =  "GDAL";

  /* Executing the algorithm */

  te::rp::Contrast algorithmInstance;
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(histogramEqualizationContrast_test)
{
  /* Open input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Access a raster datasource to create the output raster */

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_HistogramEqualizationContrast_Test.tif";

  /* Creating the algorithm parameters */

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::HistogramEqualizationContrastT;
  algoInputParams.m_hECMaxInput.resize( 3, 255 ); // set this parameter to normalize raster [0, 255]
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_createdOutRasterInfo = outputRasterInfo;
  algoOutputParams.m_createdOutRasterDSType = "GDAL";

  /* Executing the algorithm */

  te::rp::Contrast algorithmInstance;

  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(squareContrast_test)
{
  /* Open input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Access a raster datasource to create the output raster */

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Contrast_SquareContrast_Test.tif";

  /* Creating the algorithm parameters */

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::SquareContrastT;
  algoInputParams.m_squareCMinInput.resize( 3, 50 );
  algoInputParams.m_squareCMaxInput.resize( 3, 200 );
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_createdOutRasterInfo = outputRasterInfo;
  algoOutputParams.m_createdOutRasterDSType =  "GDAL";

  /* Executing the algorithm */

  te::rp::Contrast algorithmInstance;
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(squareRootContrast_test)
{
  /* Open input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Access a raster datasource to create the output raster */

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Contrast_SquareRootContrast_Test.tif";

  /* Creating the algorithm parameters */

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::SquareRootContrastT;
  algoInputParams.m_squareRootCMinInput.resize( 3, 50 );
  algoInputParams.m_squareRootCMaxInput.resize( 3, 200 );
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_createdOutRasterInfo = outputRasterInfo;
  algoOutputParams.m_createdOutRasterDSType =  "GDAL";

  /* Executing the algorithm */

  te::rp::Contrast algorithmInstance;
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(logContrast_test)
{
  /* Open input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Access a raster datasource to create the output raster */

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Contrast_LogContrast_Test.tif";

  /* Creating the algorithm parameters */

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::LogContrastT;
  algoInputParams.m_logCMinInput.resize( 3, 50 );
  algoInputParams.m_logCMaxInput.resize( 3, 200 );
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_createdOutRasterInfo = outputRasterInfo;
  algoOutputParams.m_createdOutRasterDSType =  "GDAL";

  /* Executing the algorithm */

  te::rp::Contrast algorithmInstance;
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(setMeanAndStdContrast_test)
{
  /* Open input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Access a raster datasource to create the output raster */

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_SetMeanAndStdContrast_Test.tif";

  /* Creating the algorithm parameters */

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::MeanAndStdContrastT;
  algoInputParams.m_sMASCMeanInput.resize( 3, 127 );
  algoInputParams.m_sMASCStdInput.resize( 3, 100 );
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_createdOutRasterInfo = outputRasterInfo;
  algoOutputParams.m_createdOutRasterDSType = "GDAL";

  /* Executing the algorithm */

  te::rp::Contrast algorithmInstance;
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_CASE(decorrelationEnhancementTContrast_test)
{
  /* Open input raster */

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  BOOST_CHECK( inputRasterPointer.get() );

  /* Access a raster datasource to create the output raster */

  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_Contrast_DecorrelationEnhancement_Test.tif";

  /* Creating the algorithm parameters */

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::DecorrelationEnhancementT;
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_createdOutRasterInfo = outputRasterInfo;
  algoOutputParams.m_createdOutRasterDSType =  "GDAL";

  /* Executing the algorithm */

  te::rp::Contrast algorithmInstance;
  BOOST_CHECK( algorithmInstance.initialize( algoInputParams ) );
  BOOST_CHECK( algorithmInstance.execute( algoOutputParams ) );
}

BOOST_AUTO_TEST_SUITE_END()
