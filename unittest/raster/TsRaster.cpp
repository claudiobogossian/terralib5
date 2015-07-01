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
  \file TsRaster.cpp
 
  \brief A test suit for the Cached Raster class.
 */

#include "TsRaster.h"
#include "../Config.h"

#include <terralib/memory/CachedRaster.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/raster/BandProperty.h>
#include <terralib/raster/Grid.h>

#include <terralib/dataaccess/datasource/DataSourceFactory.h>

#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsRaster );

void TsRaster::tcRasterConstructor()
{
}

void TsRaster::tcRasterGrid()
{
}

void TsRaster::tcRasterCopyConstructor()
{
}
