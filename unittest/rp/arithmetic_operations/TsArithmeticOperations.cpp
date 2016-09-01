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
  \file terralib/unittest/rp/arithmetic_operations/TsArithmeticOperations.cpp

  \brief Main file of test suit for Arithmetic Operations algorithm.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/raster.h>
#include <terralib/memory.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (arithmetic_operations_tests)

void loadDoubleRaster( const std::string& rasterFileName, std::auto_ptr< te::rst::Raster >& rasterPtr )
{
  /* Open input raster */

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = rasterFileName;
  std::auto_ptr<te::rst::Raster> rin(te::rst::RasterFactory::open(rinfo));

  std::vector< te::rst::BandProperty * > bandsProperties;

  unsigned int bandIdx = 0;

  for( bandIdx = 0 ; bandIdx < rin->getNumberOfBands() ; ++bandIdx )
  {
    bandsProperties.push_back( new te::rst::BandProperty(
      *rin->getBand( bandIdx )->getProperty() ) );
    bandsProperties[ bandIdx ]->m_type = te::dt::DOUBLE_TYPE;
  }

  rasterPtr.reset( new te::mem::ExpansibleRaster( 50, new te::rst::Grid(
    *rin->getGrid() ), bandsProperties ) );

  const unsigned int nRows = rin->getNumberOfRows();
  const unsigned int nCols = rin->getNumberOfColumns();
  unsigned int row = 0;
  unsigned int col = 0;
  double value = 0;
  for( bandIdx = 0 ; bandIdx < rin->getNumberOfBands() ; ++bandIdx )
  {
    for( row = 0 ; row < nRows ; ++row )
    {
      for( col = 0 ; col < nCols ; ++col )
      {
        rin->getValue( col, row, value, bandIdx );
        rasterPtr->setValue( col, row, value, bandIdx );
      }
    }
  }
}

BOOST_AUTO_TEST_CASE (rasterSum_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   *  band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 + R0:1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RasterSum.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double inputValue2 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      rin->getValue(c, r, inputValue2, 1);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( inputValue1 + inputValue2,
        outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (rasterSubtraction_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 - R0:1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RasterSubtraction.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double inputValue2 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      rin->getValue(c, r, inputValue2, 1);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( ( inputValue1 - inputValue2 ),
        outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (rasterProduct_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 * R0:1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RasterProduct.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double inputValue2 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      rin->getValue(c, r, inputValue2, 1);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( inputValue1 * inputValue2, outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (rasterDivision_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 / R0:1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RasterDivision.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double inputValue2 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      rin->getValue(c, r, inputValue2, 1);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( inputValue1 / inputValue2, outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (realSum_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 + 1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealSum.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( inputValue1 + 1.0, outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (realSubtraction_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 - 1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealSubtraction.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( inputValue1 - 1.0, outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (realSubtractionInverse_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "1 - R0:0";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealSubtractionInverse.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( 1.0 - inputValue1, outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (realProduct_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 * 2";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealProduct.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( inputValue1 * 2.0, outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (realDivision_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 / 2";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealDivision.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( inputValue1 / 2.0, outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (realDivisionInverse_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin );

  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "1 / R0:0";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealDivisionInverse.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
  
  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE( 1.0 / inputValue1, outputValue, 0.0000001 );
    }
}

BOOST_AUTO_TEST_CASE (diffGrid_test)
{
  std::map<std::string, std::string> rinfo1;
  rinfo1["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers_rgb342_crop1.tif";
  std::auto_ptr<te::rst::Raster> rin1(te::rst::RasterFactory::open(rinfo1));  
  
  std::map<std::string, std::string> rinfo2;
  rinfo2["URI"] = TERRALIB_DATA_DIR"/geotiff/cbers_rgb342_crop2.tif";
  std::auto_ptr<te::rst::Raster> rin2(te::rst::RasterFactory::open(rinfo2));  
  
  /* Defining input parameters, the arithmetic operation will be
   * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 + R1:0";
  inputParams.m_normalize = true;
  inputParams.m_inputRasters.push_back(rin1.get());
  inputParams.m_inputRasters.push_back(rin2.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_diffGrid.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));
}

BOOST_AUTO_TEST_CASE(exponential_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 ^ 2";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_exponential.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(pow(inputValue1, 2.0), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_CASE(sqrt_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "sqrt(R0:0)";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_sqrt.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(sqrt(inputValue1), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_CASE(sin_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "sin(R0:0)";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_sin.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(sin(inputValue1), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_CASE(asin_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/landsat8_22768_ndvi.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "asin(R0:0)";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_asin.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);

      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(asin(inputValue1), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_CASE(cos_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "cos(R0:0)";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_cos.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(cos(inputValue1), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_CASE(acos_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/landsat8_22768_ndvi.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "acos(R0:0)";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_acos.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);

      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(acos(inputValue1), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_CASE(log_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "log(R0:0)";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_log.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(log10(inputValue1), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_CASE(tan_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "tan(R0:0)";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_tan.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(tan(inputValue1), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_CASE(atan_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "atan(R0:0)";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_atan.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(atan(inputValue1), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_CASE(ln_test)
{
  /* Load input raster as a doubles raster */

  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(TERRALIB_DATA_DIR"/geotiff/cbers2b_rgb342_crop.tif",
    rin);

  /* Defining input parameters, the arithmetic operation will be
  * band 0 + band 1 - band 2 */

  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "ln(R0:0)";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

  /* Create output raster info */

  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_ln.tif";

  /* Defining output parameters */

  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  BOOST_CHECK(algorithmInstance.initialize(inputParams));
  BOOST_CHECK(algorithmInstance.execute(outputParams));

  /* Open output raster */

  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      BOOST_CHECK_CLOSE(log(inputValue1), outputValue, 0.0000001);
    }
}

BOOST_AUTO_TEST_SUITE_END()
