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
  \file TsSkeleton.cpp

  \brief A test suit for the Resiter interface.
 */

#include "TsSkeleton.h"
#include "../Config.h"

#include <terralib/rp/Skeleton.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/raster/Interpolator.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsSkeleton );

void TsSkeleton::Pattern1Test()
{
  // opening input raster
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/pattern1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );  
  
  auxRasterInfo.clear();
  auxRasterInfo["MEM_SRC_RASTER_DRIVER_TYPE"] = "MEM";  
  auxRasterInfo["FORCE_MEM_DRIVER"] = "TRUE";
  inputRasterPtrPointer.reset( inputRasterPtrPointer->resample( 
    te::rst::NearestNeighbor, -2, auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );  
  
  // Creating the algorithm parameters
  
  te::rp::Skeleton::InputParameters algoInputParams;
  
  algoInputParams.m_inputRasterPtr = inputRasterPtrPointer.get();
  algoInputParams.m_inputRasterBand = 0;
  algoInputParams.m_inputMaskRasterPtr = 0;
  algoInputParams.m_diffusionThreshold = 0.5;
  algoInputParams.m_diffusionRegularization = 0.5;
  algoInputParams.m_diffusionMaxIterations = 0;
  algoInputParams.m_enableMultiThread = true;
  algoInputParams.m_skeletonThreshold = 0.75;

  te::rp::Skeleton::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] = "terralib_unittest_rp_skeleton_test1.tif";
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::Skeleton algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}
