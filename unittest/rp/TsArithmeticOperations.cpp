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
  \file TsArithmeticOperations.cpp

  \brief A test suit for the Arithmetic Operations algorithm.
 */

// TerraLib
#include "../Config.h"
#include <terralib/gdal.h>
#include <terralib/gdal/Utils.h>
#include <terralib/raster.h>
#include <terralib/memory/ExpansibleRaster.h>
#include <terralib/rp/ArithmeticOperations.h>
#include "TsArithmeticOperations.h"

// STL
//#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION( TsArithmeticOperations );

void TsArithmeticOperations::loadDoubleRaster( const std::string& rasterFileName, std::auto_ptr< te::rst::Raster >& rasterPtr )
{
// open input raster
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
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

void TsArithmeticOperations::RasterSum()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 + R0:1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RasterSum.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double inputValue2 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      rin->getValue(c, r, inputValue2, 1);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( inputValue1 + inputValue2, 
        outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::RasterSubtraction()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 - R0:1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RasterSubtraction.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double inputValue2 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      rin->getValue(c, r, inputValue2, 1);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( ( inputValue1 - inputValue2 ), 
        outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::RasterProduct()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 * R0:1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RasterProduct.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double inputValue2 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      rin->getValue(c, r, inputValue2, 1);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( inputValue1 * inputValue2, outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::RasterDivision()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 / R0:1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RasterDivision.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double inputValue2 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      rin->getValue(c, r, inputValue2, 1);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( inputValue1 / inputValue2, outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::RealSum()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 + 1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealSum.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( inputValue1 + 1.0, outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::RealSubtraction()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 - 1";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealSubtraction.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( inputValue1 - 1.0, outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::RealSubtractionInverse()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "1 - R0:0";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealSubtractionInverse.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0 - inputValue1, outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::RealProduct()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 * 2";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealProduct.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( inputValue1 * 2.0, outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::RealDivision()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 / 2";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealDivision.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( inputValue1 / 2.0, outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::RealDivisionInverse()
{
  // load input raster as a doubles raster
  std::auto_ptr< te::rst::Raster > rin;
  loadDoubleRaster(  TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif",
    rin );

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "1 / R0:0";
  inputParams.m_normalize = false;
  inputParams.m_inputRasters.push_back(rin.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_RealDivisionInverse.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
  
// open output raster
  double inputValue1 = 0;
  double outputValue = 0;
  for (unsigned int r = 0; r < rin->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rin->getNumberOfColumns(); c++)
    {
      rin->getValue(c, r, inputValue1, 0);
      outputParams.m_outputRasterPtr->getValue(c, r, outputValue, 0);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0 / inputValue1, outputValue, 0.0000001 );
    }
}

void TsArithmeticOperations::diffGrid()
{
  std::map<std::string, std::string> rinfo1;
  rinfo1["URI"] = TERRALIB_DATA_DIR"/rasters/cbers_rgb342_crop1.tif";
  std::auto_ptr<te::rst::Raster> rin1(te::rst::RasterFactory::open(rinfo1));  
  
  std::map<std::string, std::string> rinfo2;
  rinfo2["URI"] = TERRALIB_DATA_DIR"/rasters/cbers_rgb342_crop2.tif";
  std::auto_ptr<te::rst::Raster> rin2(te::rst::RasterFactory::open(rinfo2));  
  

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 + R1:0";
  inputParams.m_normalize = true;
  inputParams.m_inputRasters.push_back(rin1.get());
  inputParams.m_inputRasters.push_back(rin2.get());

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "terralib_unittest_arithmetic_diffGrid.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));
}

