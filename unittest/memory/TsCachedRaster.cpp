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
  \file TsCachedRaster.cpp
 
  \brief A test suit for the Cached Raster class.
 */

#include "TsCachedRaster.h"
#include "../Config.h"

#include <terralib/raster.h>
#include <terralib/dataaccess.h>

#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsCachedRaster );

void TsCachedRaster::CreateTestRaster( unsigned int nBands, unsigned int nLines, 
  unsigned int nCols, boost::shared_ptr< te::rst::Raster >& rasterPointer )
{
  std::vector< te::rst::BandProperty * > bandsProps;
  for( unsigned int bandsPropsIdx = 0 ; bandsPropsIdx < nBands ; ++bandsPropsIdx )
  {
    bandsProps.push_back( new te::rst::BandProperty( bandsPropsIdx, 
      te::dt::UINT32_TYPE ) );    
  }
  
  rasterPointer.reset( te::rst::RasterFactory::make( "MEM", 
    new te::rst::Grid( nCols, nLines ), bandsProps, 
    std::map< std::string, std::string >(), 0, 0 ) );
    
  unsigned int band = 0;
  unsigned int line = 0;
  unsigned int col = 0;
  double pixelValue = 0;
  
  for( band = 0 ; band < nBands ; ++band )
    for( line = 0 ; line < nLines ; ++line )
      for( col = 0 ; col < nCols ; ++col )
      {
        rasterPointer->setValue( col, line, pixelValue, band );
        ++pixelValue;
      }
}

void TsCachedRaster::ReadWriteTest()
{
  // create the input test raster
  
  const unsigned int nBands = 10;
  const unsigned int nLines = 10;
  const unsigned int nCols = 10;

  boost::shared_ptr< te::rst::Raster > inputRasterPointer;
  CreateTestRaster( nBands, nLines, nCols, inputRasterPointer );

  // using the cached raster adaptor.
  {
    te::mem::CachedRaster cachedRaster( 2, *inputRasterPointer, 0 );
    
    unsigned int band = 0;
    unsigned int line = 0;
    unsigned int col = 0;
    double pixelValue = 0;
    
    for( band = 0 ; band < nBands ; ++band )
      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          cachedRaster.getValue( col, line, pixelValue, band );
          cachedRaster.setValue( col, line, pixelValue + 10.0, band );
        }
  }
  
  // Verifying the values
  
  {
    unsigned int band = 0;
    unsigned int line = 0;
    unsigned int col = 0;
    double pixelValue = 0;
    double readPixelValue = 0;
    
    for( band = 0 ; band < nBands ; ++band )
      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          inputRasterPointer->getValue( col, line, readPixelValue, band );
          CPPUNIT_ASSERT_DOUBLES_EQUAL( pixelValue + 10.0, readPixelValue, 0.0000001 );
          ++pixelValue;
        }
  }
}

void TsCachedRaster::ReadAheadTest()
{
  // create the input test raster
  
  const unsigned int nBands = 10;
  const unsigned int nLines = 10;
  const unsigned int nCols = 10;

  boost::shared_ptr< te::rst::Raster > inputRasterPointer;
  CreateTestRaster( nBands, nLines, nCols, inputRasterPointer );

  // using the cached raster adaptor.
  {
    te::mem::CachedRaster cachedRaster( 2, *inputRasterPointer, 1 );
    
    unsigned int band = 0;
    unsigned int line = 0;
    unsigned int col = 0;
    double pixelValue = 0;
    
    for( band = 0 ; band < nBands ; ++band )
      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          cachedRaster.getValue( col, line, pixelValue, band );
          cachedRaster.setValue( col, line, pixelValue + 10.0, band );
        }
  }
  
  // Verifying the values
  
  {
    unsigned int band = 0;
    unsigned int line = 0;
    unsigned int col = 0;
    double pixelValue = 0;
    double readPixelValue = 0;
    
    for( band = 0 ; band < nBands ; ++band )
      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          inputRasterPointer->getValue( col, line, readPixelValue, band );
          CPPUNIT_ASSERT_DOUBLES_EQUAL( pixelValue + 10.0, readPixelValue, 0.0000001 );
          ++pixelValue;
        }
  }
}
