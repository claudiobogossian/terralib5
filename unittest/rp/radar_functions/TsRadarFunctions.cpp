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
  \file terralib/unittest/rp/radar_functions/TsRadarFunctions.cpp

  \brief A test suit for raster processing Radar Functions.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/raster/RasterFactory.h>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE (radar_functions_tests)

BOOST_AUTO_TEST_CASE(sampleFunction_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );

  BOOST_CHECK( diskRasterPtr.get() );
  BOOST_CHECK( te::rp::radar::SampleFunction( *diskRasterPtr ) );
}

BOOST_AUTO_TEST_SUITE_END()
