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
  \file TsExpansibleRaster.cpp
 
  \brief A test suit for the Cached Raster class interface.

 */

#include "TsExpansibleRaster.h"
#include "../Config.h"

#include <terralib/memory/ExpansibleRaster.h>
#include <terralib/raster/BandProperty.h>
#include <terralib/raster/Grid.h>

#include <terralib/dataaccess/datasource/DataSourceFactory.h>

#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsExpansibleRaster );

void TsExpansibleRaster::ReadWriteTest()
{
  std::vector< te::rst::BandProperty * > bandsProps;
  bandsProps.push_back( new te::rst::BandProperty( 0, te::dt::UINT32_TYPE ) );
  bandsProps[ 0 ]->m_blkw = 10;
  bandsProps[ 0 ]->m_blkh = 10;
  bandsProps[ 0 ]->m_nblocksx = 1;
  bandsProps[ 0 ]->m_nblocksy = 1;
  bandsProps.push_back( new te::rst::BandProperty( 1, te::dt::UINT16_TYPE ) );
  bandsProps[ 1 ]->m_blkw = 5;
  bandsProps[ 1 ]->m_blkh = 10;
  bandsProps[ 1 ]->m_nblocksx = 2;
  bandsProps[ 1 ]->m_nblocksy = 1;  
  bandsProps.push_back( new te::rst::BandProperty( 2, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 2 ]->m_blkw = 10;
  bandsProps[ 2 ]->m_blkh = 5;
  bandsProps[ 2 ]->m_nblocksx = 1;
  bandsProps[ 2 ]->m_nblocksy = 2; 
  
  te::mem::ExpansibleRaster rasterInstance( new te::rst::Grid( 10, 10 ), bandsProps, 2 );

  // writing values
  
  {
    unsigned int band = 0;
    unsigned int line = 0;
    unsigned int col = 0;
    double pixelValue = 0;
    
    for( band = 0 ; band < 3 ; ++band )
      for( line = 0 ; line < 10 ; ++line )
        for( col = 0 ; col < 10 ; ++col )
        {
          rasterInstance.setValue( col, line, pixelValue, band );
          ++pixelValue;
        }
  }
  
  // Verifying the values
  
  {
    unsigned int band = 0;
    unsigned int line = 0;
    unsigned int col = 0;
    double pixelValue = 0;
    double readPixelValue = 0;
    
    for( band = 0 ; band < 3 ; ++band )
      for( line = 0 ; line < 10 ; ++line )
        for( col = 0 ; col < 10 ; ++col )
        {
          rasterInstance.getValue( col, line, readPixelValue, band );
          CPPUNIT_ASSERT_DOUBLES_EQUAL( pixelValue, readPixelValue, 0.0000001 );
          ++pixelValue;
        }
  }
}


