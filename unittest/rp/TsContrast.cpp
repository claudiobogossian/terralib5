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
  \file TsContrast.cpp
 
  \brief A test suit for the DataSource interface.
 */

#include "TsContrast.h"
#include "../Config.h"

#include <terralib/rp/Contrast.h>
#include <terralib/raster/RasterFactory.h>

#include <terralib/dataaccess/datasource/DataSourceFactory.h>

#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsContrast );


void TsContrast::LinearContrast()
{
  // open input raster

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
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

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::LinearContrastT;
  algoInputParams.m_lCMinInput = 50;
  algoInputParams.m_lCMaxInput = 200;
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_outDataSourcePtr = outDataSourcePtr.get();
  algoOutputParams.m_outDataSetName = "terralib_unittest_rp_Contrast_noOptimization_Test.tif";

  // Executing the algorithm

  te::rp::Contrast algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams, algoOutputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute() );
}

void TsContrast::HistogramEqualizationContrast()
{
  // open input raster

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
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

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::LinearContrastT;
  algoInputParams.m_hECMaxInput = 255; // set this parameter to normalize raster [0, 255]
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_outDataSourcePtr = outDataSourcePtr.get();
  algoOutputParams.m_outDataSetName = "terralib_unittest_rp_HistogramEqualizationContrast_Test.tif";

  // Executing the algorithm

  te::rp::Contrast algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams, algoOutputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute() );
}

void TsContrast::SetMeanAndStdContrast()
{
  // open input raster

  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_UNITTEST_LOCALE "/data/rasters/cbers2b_rgb342_crop.tif";

  boost::shared_ptr< te::rst::Raster > inputRasterPointer( te::rst::RasterFactory::open(
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

  te::rp::Contrast::InputParameters algoInputParams;
  te::rp::Contrast::OutputParameters algoOutputParams;

  algoInputParams.m_type = te::rp::Contrast::InputParameters::SetMeanAndStdContrastT;
  algoInputParams.m_sMASCMeanInput = 127;
  algoInputParams.m_sMASCStdInput = 100;
  algoInputParams.m_inRasterPtr = inputRasterPointer.get();
  algoInputParams.m_inRasterBands.push_back( 0 );
  algoInputParams.m_inRasterBands.push_back( 1 );
  algoInputParams.m_inRasterBands.push_back( 2 );
  algoOutputParams.m_outDataSourcePtr = outDataSourcePtr.get();
  algoOutputParams.m_outDataSetName = "terralib_unittest_rp_SetMeanAndStdContrast_Test.tif";

  // Executing the algorithm

  te::rp::Contrast algorithmInstance;

  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams, algoOutputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute() );
}

