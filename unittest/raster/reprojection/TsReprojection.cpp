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
  \file terralib/unittest/raster/TsReprojection.cpp
 
  \brief A test suit for the Raster Reprojection class.
 */

// TerraLib
#include <terralib/raster.h>
#include "../Config.h"

// Boost
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE ( reprojection_tests )

BOOST_AUTO_TEST_CASE (reprojection1_test)
{
  /* Openning input raster */

  std::map<std::string, std::string> auxRasterInfo;

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtr ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( inputRasterPtr.get() );

  /* Reprojecting */
  
  auxRasterInfo["URI"] = "TsReprojection_tcReprojection1.tif";
  boost::shared_ptr< te::rst::Raster > outputRasterPtr(
    inputRasterPtr->transform( 32621, auxRasterInfo, 1 ) );
  BOOST_CHECK( outputRasterPtr.get() );
}

BOOST_AUTO_TEST_CASE (reprojectin2_test)
{
}

BOOST_AUTO_TEST_CASE (reprojection3_test)
{
}

BOOST_AUTO_TEST_SUITE_END()
