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
  \file TsFunctions.cpp

  \brief A test suit for raster processing Functions.
 */

#include "TsRadarFunctions.h"
#include "../Config.h"

#include <terralib/raster/RasterFactory.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsFunctions );

void TsFunctions::SampleFunction()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
  CPPUNIT_ASSERT( te::rp::radar::SampleFunction( *diskRasterPtr ) );
}


