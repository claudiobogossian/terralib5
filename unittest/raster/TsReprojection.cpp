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
  \file TsReprojection.cpp
 
  \brief A test suit for the raster Reprojection class.
 */

#include "TsReprojection.h"
#include "../Config.h"

#include <terralib/raster/Interpolator.h>

#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsReprojection );

void TsReprojection::tcReprojection1()
{
  // openning input raster

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtr ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtr.get() );

  // Reprojecting
  
  auxRasterInfo["URI"] = "TsReprojection_tcReprojection1.tif";
  boost::shared_ptr< te::rst::Raster > outputRasterPtr( 
    inputRasterPtr->transform( 32621, auxRasterInfo, 1 ) );
  CPPUNIT_ASSERT( outputRasterPtr.get() );
}

void TsReprojection::tcReprojection2()
{
}

void TsReprojection::tcReprojection3()
{
}
