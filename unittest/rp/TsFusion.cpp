/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file TsFusion.cpp

  \brief A test suit for the fusion classes interfaces.
 */

#include "TsFusion.h"
#include "../Config.h"

#include <terralib/rp/IHSFusion.h>
#include <terralib/rp/PCAFusion.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterFactory.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsFusion );

void TsFusion::ihs()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  boost::shared_ptr< te::rst::Raster > lowResRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( lowResRasterPtr.get() );  
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_hrc_crop.tif";
  boost::shared_ptr< te::rst::Raster > highResRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( highResRasterPtr.get() );   
  
  // Creating the algorithm parameters
  
  te::rp::IHSFusion::InputParameters algoInputParams;
  
  algoInputParams.m_lowResRasterPtr = lowResRasterPtr.get();
  algoInputParams.m_lowResRasterRedBandIndex = 0;
  algoInputParams.m_lowResRasterGreenBandIndex = 1;
  algoInputParams.m_lowResRasterBlueBandIndex = 2;
  algoInputParams.m_highResRasterPtr = highResRasterPtr.get();
  algoInputParams.m_highResRasterBand = 0;
  algoInputParams.m_enableProgress = false;
  algoInputParams.m_interpMethod = te::rst::Interpolator::NearestNeighbor;
  algoInputParams.m_RGBMin = 0;
  algoInputParams.m_RGBMax = 0;

  te::rp::IHSFusion::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =  
    "terralib_unittest_rp_fusion_ihs.tif";  
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::IHSFusion algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

void TsFusion::pca()
{
  // opening input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  boost::shared_ptr< te::rst::Raster > lowResRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( lowResRasterPtr.get() );  
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_hrc_crop.tif";
  boost::shared_ptr< te::rst::Raster > highResRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( highResRasterPtr.get() );   
  
  // Creating the algorithm parameters
  
  te::rp::PCAFusion::InputParameters algoInputParams;
  
  algoInputParams.m_lowResRasterPtr = lowResRasterPtr.get();
  algoInputParams.m_lowResRasterBands.push_back( 0 );
  algoInputParams.m_lowResRasterBands.push_back( 1 );
  algoInputParams.m_lowResRasterBands.push_back( 2 );
  algoInputParams.m_highResRasterPtr = highResRasterPtr.get();
  algoInputParams.m_highResRasterBand = 0;
  algoInputParams.m_enableProgress = true;
  algoInputParams.m_enableThreadedProcessing = false;
  algoInputParams.m_interpMethod = te::rst::Interpolator::NearestNeighbor;

  te::rp::PCAFusion::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =  
    "terralib_unittest_rp_fusion_pca.tif";  
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::PCAFusion algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

