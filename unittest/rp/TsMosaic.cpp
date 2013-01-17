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
  \file TsMosaic.cpp

  \brief A test suit for the Mosaic interface.
 */

#include "TsMosaic.h"
#include "../Config.h"

#include <terralib/rp/Mosaic.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterFactory.h>

#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsMosaic );

void TsMosaic::GeoReferencedImagesMosaic()
{
  // openning input rasters
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TE_DATA_LOCALE "/data/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster1Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster1Pointer.get() );    
  
  auxRasterInfo["URI"] = TE_DATA_LOCALE "/data/rasters/cbers_rgb342_crop2.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster2Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster2Pointer.get() );
  
  auxRasterInfo["URI"] = TE_DATA_LOCALE "/data/rasters/cbers_rgb342_crop3_EPSG_22522.tif";
  boost::shared_ptr< te::rst::Raster > inputRaster3Pointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRaster3Pointer.get() );    
  
    
  // Creating the algorithm parameters
  
  te::rp::Mosaic::InputParameters algoInputParams;
  
  std::vector< const te::rst::Raster* > rasters;
  rasters.push_back( inputRaster1Pointer.get() );
  rasters.push_back( inputRaster2Pointer.get() );
  rasters.push_back( inputRaster3Pointer.get() );
  te::rp::FeederConstRasterVector feeder( rasters );
  algoInputParams.m_feederRasterPtr = &feeder;
  
  std::vector< unsigned int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 0 ] = 1;
  bands[ 1 ] = 2;
  bands[ 2 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  bands[ 2 ] = 2;
  bands[ 0 ] = 1;
  bands[ 1 ] = 0;
  algoInputParams.m_inputRastersBands.push_back( bands );
  
  algoInputParams.m_interpMethod = te::rst::Interpolator::NearestNeighbor;
  algoInputParams.m_noDataValue = 0;
  algoInputParams.m_forceInputNoDataValue = true;
  algoInputParams.m_blendMethod = te::rp::Blender::NoBlendMethod;
  algoInputParams.m_autoEqualize = true;  

  te::rp::Mosaic::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =  
    "terralib_unittest_rp_Mosaic_GeoReferencedImagesMosaic_Test.tif";  
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::Mosaic algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}



