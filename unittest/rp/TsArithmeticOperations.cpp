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
#include <terralib/rp/ArithmeticOperations.h>
#include "TsArithmeticOperations.h"

// STL
//#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION( TsArithmeticOperations );

void TsArithmeticOperations::SumSubtraction()
{
// open input raster
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  std::auto_ptr<te::rst::Raster> rin(te::rst::RasterFactory::open(rinfo)); 

// defining input parameters, the arithmetic operation will be 
// band 0 + band 1 - band 2
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 + R0:1 - R0:2";
  inputParams.m_normalize = false;
  boost::shared_ptr<te::rst::Raster> rasterVector(rin);
  inputParams.m_rasterVec.push_back(rasterVector);

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "arithmetic_operations_sum_subtraction_unittest.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));

  outputParams.reset();
  
// open output raster
  rin.reset(te::rst::RasterFactory::open(rinfo)); 
  std::auto_ptr<te::rst::Raster> rout(te::rst::RasterFactory::open(orinfo));
  double pixel_value;
  double pixel_calculator;
  for (unsigned int r = 0; r < rout->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rout->getNumberOfColumns(); c++)
    {
      // comparing the results pixel by pixel
      rin->getValue(c, r, pixel_value, 0);
      pixel_calculator = pixel_value;
      rin->getValue(c, r, pixel_value, 1);
      pixel_calculator += pixel_value;
      rin->getValue(c, r, pixel_value, 2);
      pixel_calculator -= pixel_value;

      rout->getValue(c, r, pixel_value, 0);
      CPPUNIT_ASSERT(pixel_value == pixel_calculator);
    }
}

void TsArithmeticOperations::ProductDivision()
{
// open input raster
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = TERRALIB_DATA_DIR"/rasters/cbers2b_rgb342_crop.tif";
  std::auto_ptr<te::rst::Raster> rin(te::rst::RasterFactory::open(rinfo)); 

// defining input parameters, the arithmetic operation will be 
// band / same band, to check if in the final result, the mean 
// will be 1.0
  te::rp::ArithmeticOperations::InputParameters inputParams;
  inputParams.m_arithmeticString = "R0:0 / R0:0 * 2.0";
  inputParams.m_normalize = false;
  boost::shared_ptr<te::rst::Raster> rasterVector(rin);
  inputParams.m_rasterVec.push_back(rasterVector);

// create output raster info
  std::map<std::string, std::string> orinfo;
  orinfo["URI"] = "arithmetic_operations_product_division_unittest.tif";

// defining output parameters
  te::rp::ArithmeticOperations::OutputParameters outputParams;
  outputParams.m_rInfo = orinfo;
  outputParams.m_rType = "GDAL";

  te::rp::ArithmeticOperations algorithmInstance;

  CPPUNIT_ASSERT(algorithmInstance.initialize(inputParams));
  CPPUNIT_ASSERT(algorithmInstance.execute(outputParams));

  outputParams.reset();
  
// open output raster
  rin.reset(te::rst::RasterFactory::open(rinfo)); 
  std::auto_ptr<te::rst::Raster> rout(te::rst::RasterFactory::open(orinfo));
  double pixel_value;
  for (unsigned int r = 0; r < rout->getNumberOfRows(); r++)
    for (unsigned int c = 0; c < rout->getNumberOfColumns(); c++)
    {
      // comparing the results pixel by pixel
      rout->getValue(c, r, pixel_value, 0);
      CPPUNIT_ASSERT(pixel_value == 2.0);
    }
}
