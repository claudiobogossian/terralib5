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

void TsExpansibleRaster::writeValues( te::rst::Raster& raster )
{
  const unsigned int nBands = raster.getNumberOfBands();
  const unsigned int nLines = raster.getNumberOfRows();
  const unsigned int nCols = raster.getNumberOfColumns();
  unsigned int band = 0;
  unsigned int line = 0;
  unsigned int col = 0;
  double pixelValue = 0;
  
  for( band = 0 ; band < nBands ; ++band )
    for( line = 0 ; line < nLines ; ++line )
      for( col = 0 ; col < nCols ; ++col )
      {
        raster.setValue( col, line, pixelValue, band );
        ++pixelValue;
      }
}

void TsExpansibleRaster::fillWithNoDataValues( te::rst::Raster& raster )
{
  const unsigned int nBands = raster.getNumberOfBands();
  const unsigned int nLines = raster.getNumberOfRows();
  const unsigned int nCols = raster.getNumberOfColumns();
  unsigned int band = 0;
  unsigned int line = 0;
  unsigned int col = 0;
  
  for( band = 0 ; band < nBands ; ++band )
  {
    const double noDataValue = raster.getBand( band )->getProperty()->m_noDataValue;
    
    for( line = 0 ; line < nLines ; ++line )
      for( col = 0 ; col < nCols ; ++col )
      {
        raster.setValue( col, line, noDataValue, band );
      }
  }
}

void TsExpansibleRaster::testValues( te::rst::Raster& raster )
{
  const unsigned int nBands = raster.getNumberOfBands();
  const unsigned int nLines = raster.getNumberOfRows();
  const unsigned int nCols = raster.getNumberOfColumns();
  unsigned int band = 0;
  unsigned int line = 0;
  unsigned int col = 0;
  double pixelValue = 0;
  double readValue = 0;
  
  for( band = 0 ; band < nBands ; ++band )
    for( line = 0 ; line < nLines ; ++line )
      for( col = 0 ; col < nCols ; ++col )
      {
        raster.getValue( col, line, readValue, band );
        
        CPPUNIT_ASSERT( readValue == pixelValue );
        
        ++pixelValue;
      }
}

void TsExpansibleRaster::readWriteTest()
{
  std::vector< te::rst::BandProperty * > bandsProps;
  bandsProps.push_back( new te::rst::BandProperty( 0, te::dt::UINT32_TYPE ) );
  bandsProps[ 0 ]->m_blkw = 5;
  bandsProps[ 0 ]->m_blkh = 10;
  bandsProps[ 0 ]->m_nblocksx = 2;
  bandsProps[ 0 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 1, te::dt::UINT16_TYPE ) );
  bandsProps[ 1 ]->m_blkw = 10;
  bandsProps[ 1 ]->m_blkh = 10;
  bandsProps[ 1 ]->m_nblocksx = 1;
  bandsProps[ 1 ]->m_nblocksy = 1;
  bandsProps.push_back( new te::rst::BandProperty( 2, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 2 ]->m_blkw = 10;
  bandsProps[ 2 ]->m_blkh = 5;
  bandsProps[ 2 ]->m_nblocksx = 1;
  bandsProps[ 2 ]->m_nblocksy = 2; 
  
  te::mem::ExpansibleRaster rasterInstance( new te::rst::Grid( 10, 10 ), bandsProps, 2 );

  writeValues( rasterInstance );
  testValues( rasterInstance );
}


void TsExpansibleRaster::addTopLinesTest()
{
  std::vector< te::rst::BandProperty * > bandsProps;
  bandsProps.push_back( new te::rst::BandProperty( 0, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 0 ]->m_noDataValue = 0;
  bandsProps[ 0 ]->m_blkw = 5;
  bandsProps[ 0 ]->m_blkh = 10;
  bandsProps[ 0 ]->m_nblocksx = 2;
  bandsProps[ 0 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 1, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 1 ]->m_noDataValue = 0;
  bandsProps[ 1 ]->m_blkw = 5;
  bandsProps[ 1 ]->m_blkh = 10;
  bandsProps[ 1 ]->m_nblocksx = 2;
  bandsProps[ 1 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 2, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 2 ]->m_noDataValue = 0;
  bandsProps[ 2 ]->m_blkw = 5;
  bandsProps[ 2 ]->m_blkh = 10;
  bandsProps[ 2 ]->m_nblocksx = 2;
  bandsProps[ 2 ]->m_nblocksy = 1;   
  
  te::mem::ExpansibleRaster rasterInstance( new te::rst::Grid( 10, 10 ), bandsProps, 2 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );

  double readValue = 0;
  
  fillWithNoDataValues( rasterInstance );
  
  double xCoord = 0;
  double yCoord = 0;
  rasterInstance.getGrid()->gridToGeo( 5, 5, xCoord, yCoord );
  
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 0.0 );  
  
  rasterInstance.setValue( 5, 5, 1, 1 );
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  rasterInstance.addTopLines( 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 20 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );

  double col = 0;
  double row = 0;  
  rasterInstance.getGrid()->geoToGrid( xCoord, yCoord, col, row );
  
  rasterInstance.getValue( (unsigned int)col, (unsigned int)row, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  writeValues( rasterInstance );
  testValues( rasterInstance );  
}

void TsExpansibleRaster::addBottomLinesTest()
{
  std::vector< te::rst::BandProperty * > bandsProps;
  bandsProps.push_back( new te::rst::BandProperty( 0, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 0 ]->m_noDataValue = 0;
  bandsProps[ 0 ]->m_blkw = 5;
  bandsProps[ 0 ]->m_blkh = 10;
  bandsProps[ 0 ]->m_nblocksx = 2;
  bandsProps[ 0 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 1, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 1 ]->m_noDataValue = 0;
  bandsProps[ 1 ]->m_blkw = 5;
  bandsProps[ 1 ]->m_blkh = 10;
  bandsProps[ 1 ]->m_nblocksx = 2;
  bandsProps[ 1 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 2, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 2 ]->m_noDataValue = 0;
  bandsProps[ 2 ]->m_blkw = 5;
  bandsProps[ 2 ]->m_blkh = 10;
  bandsProps[ 2 ]->m_nblocksx = 2;
  bandsProps[ 2 ]->m_nblocksy = 1;   
  
  te::mem::ExpansibleRaster rasterInstance( new te::rst::Grid( 10, 10 ), bandsProps, 2 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );

  double readValue = 0;
  
  fillWithNoDataValues( rasterInstance );
  
  double xCoord = 0;
  double yCoord = 0;
  rasterInstance.getGrid()->gridToGeo( 5, 5, xCoord, yCoord );
  
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 0.0 );  
  
  rasterInstance.setValue( 5, 5, 1, 1 );
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  rasterInstance.addBottomLines( 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 20 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );
  
  double col = 0;
  double row = 0;
  rasterInstance.getGrid()->geoToGrid( xCoord, yCoord, col, row );
  
  rasterInstance.getValue( (unsigned int)col, (unsigned int)row, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  writeValues( rasterInstance );
  testValues( rasterInstance );    
}

void TsExpansibleRaster::addLeftColumnsTest()
{
  std::vector< te::rst::BandProperty * > bandsProps;
  bandsProps.push_back( new te::rst::BandProperty( 0, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 0 ]->m_noDataValue = 0;
  bandsProps[ 0 ]->m_blkw = 5;
  bandsProps[ 0 ]->m_blkh = 10;
  bandsProps[ 0 ]->m_nblocksx = 2;
  bandsProps[ 0 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 1, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 1 ]->m_noDataValue = 0;
  bandsProps[ 1 ]->m_blkw = 5;
  bandsProps[ 1 ]->m_blkh = 10;
  bandsProps[ 1 ]->m_nblocksx = 2;
  bandsProps[ 1 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 2, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 2 ]->m_noDataValue = 0;
  bandsProps[ 2 ]->m_blkw = 5;
  bandsProps[ 2 ]->m_blkh = 10;
  bandsProps[ 2 ]->m_nblocksx = 2;
  bandsProps[ 2 ]->m_nblocksy = 1;   
  
  te::mem::ExpansibleRaster rasterInstance( new te::rst::Grid( 10, 10 ), bandsProps, 2 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );

  double readValue = 0;
  
  fillWithNoDataValues( rasterInstance );
  
  double xCoord = 0;
  double yCoord = 0;
  rasterInstance.getGrid()->gridToGeo( 5, 5, xCoord, yCoord );
  
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 0.0 );  
  
  rasterInstance.setValue( 5, 5, 1, 1 );
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  rasterInstance.addLeftColumns( 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 15 );

  double col = 0;
  double row = 0;  
  rasterInstance.getGrid()->geoToGrid( xCoord, yCoord, col, row );
  
  rasterInstance.getValue( (unsigned int)col, (unsigned int)row, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  writeValues( rasterInstance );
  testValues( rasterInstance );  
}

void TsExpansibleRaster::addRightColumnsTest()
{
  std::vector< te::rst::BandProperty * > bandsProps;
  bandsProps.push_back( new te::rst::BandProperty( 0, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 0 ]->m_noDataValue = 0;
  bandsProps[ 0 ]->m_blkw = 5;
  bandsProps[ 0 ]->m_blkh = 10;
  bandsProps[ 0 ]->m_nblocksx = 2;
  bandsProps[ 0 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 1, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 1 ]->m_noDataValue = 0;
  bandsProps[ 1 ]->m_blkw = 5;
  bandsProps[ 1 ]->m_blkh = 10;
  bandsProps[ 1 ]->m_nblocksx = 2;
  bandsProps[ 1 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 2, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 2 ]->m_noDataValue = 0;
  bandsProps[ 2 ]->m_blkw = 5;
  bandsProps[ 2 ]->m_blkh = 10;
  bandsProps[ 2 ]->m_nblocksx = 2;
  bandsProps[ 2 ]->m_nblocksy = 1;   
  
  te::mem::ExpansibleRaster rasterInstance( new te::rst::Grid( 10, 10 ), bandsProps, 2 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );

  double readValue = 0;
  
  fillWithNoDataValues( rasterInstance );
  
  double xCoord = 0;
  double yCoord = 0;
  rasterInstance.getGrid()->gridToGeo( 5, 5, xCoord, yCoord );
  
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 0.0 );  
  
  rasterInstance.setValue( 5, 5, 1, 1 );
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  rasterInstance.addRightColumns( 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 15 );
  
  double col = 0;
  double row = 0;
  rasterInstance.getGrid()->geoToGrid( xCoord, yCoord, col, row );
  
  rasterInstance.getValue( (unsigned int)col, (unsigned int)row, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  writeValues( rasterInstance );
  testValues( rasterInstance );    
}

void TsExpansibleRaster::addTopBandsTest()
{
  std::vector< te::rst::BandProperty * > bandsProps;
  bandsProps.push_back( new te::rst::BandProperty( 0, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 0 ]->m_noDataValue = 0;
  bandsProps[ 0 ]->m_blkw = 5;
  bandsProps[ 0 ]->m_blkh = 10;
  bandsProps[ 0 ]->m_nblocksx = 2;
  bandsProps[ 0 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 1, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 1 ]->m_noDataValue = 0;
  bandsProps[ 1 ]->m_blkw = 5;
  bandsProps[ 1 ]->m_blkh = 10;
  bandsProps[ 1 ]->m_nblocksx = 2;
  bandsProps[ 1 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 2, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 2 ]->m_noDataValue = 0;
  bandsProps[ 2 ]->m_blkw = 5;
  bandsProps[ 2 ]->m_blkh = 10;
  bandsProps[ 2 ]->m_nblocksx = 2;
  bandsProps[ 2 ]->m_nblocksy = 1;   
  
  te::mem::ExpansibleRaster rasterInstance( new te::rst::Grid( 10, 10 ), bandsProps, 2 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );

  double readValue = 0;
  
  fillWithNoDataValues( rasterInstance );
  
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 0.0 );  
  
  rasterInstance.setValue( 5, 5, 1, 1 );
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  rasterInstance.addTopBands( 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 6 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );

  rasterInstance.getValue( 5, 5, readValue, 4 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  writeValues( rasterInstance );
  testValues( rasterInstance );  
}

void TsExpansibleRaster::addBottomBandsTest()
{
  std::vector< te::rst::BandProperty * > bandsProps;
  bandsProps.push_back( new te::rst::BandProperty( 0, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 0 ]->m_noDataValue = 0;
  bandsProps[ 0 ]->m_blkw = 5;
  bandsProps[ 0 ]->m_blkh = 10;
  bandsProps[ 0 ]->m_nblocksx = 2;
  bandsProps[ 0 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 1, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 1 ]->m_noDataValue = 0;
  bandsProps[ 1 ]->m_blkw = 5;
  bandsProps[ 1 ]->m_blkh = 10;
  bandsProps[ 1 ]->m_nblocksx = 2;
  bandsProps[ 1 ]->m_nblocksy = 1;   
  bandsProps.push_back( new te::rst::BandProperty( 2, te::dt::DOUBLE_TYPE ) );
  bandsProps[ 2 ]->m_noDataValue = 0;
  bandsProps[ 2 ]->m_blkw = 5;
  bandsProps[ 2 ]->m_blkh = 10;
  bandsProps[ 2 ]->m_nblocksx = 2;
  bandsProps[ 2 ]->m_nblocksy = 1;   
  
  te::mem::ExpansibleRaster rasterInstance( new te::rst::Grid( 10, 10 ), bandsProps, 2 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );

  double readValue = 0;
  
  fillWithNoDataValues( rasterInstance );
  
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 0.0 );  
  
  rasterInstance.setValue( 5, 5, 1, 1 );
  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  rasterInstance.addBottomBands( 3 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfBands() == 6 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfRows() == 10 );
  CPPUNIT_ASSERT( rasterInstance.getNumberOfColumns() == 10 );

  rasterInstance.getValue( 5, 5, readValue, 1 );
  CPPUNIT_ASSERT( readValue == 1.0 );  
  
  writeValues( rasterInstance );
  testValues( rasterInstance );  
}

