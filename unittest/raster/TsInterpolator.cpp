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
  \file TsInterpolator.cpp
 
  \brief A test suit for the Raster Interpolator class.
 */

#include "TsInterpolator.h"
#include "../Config.h"

#include <terralib/raster/Interpolator.h>
#include <terralib/raster/RasterFactory.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsInterpolator );

void TsInterpolator::createTestRaster( unsigned int nBands, unsigned int nLines, 
  unsigned int nCols, std::auto_ptr< te::rst::Raster >& rasterPointer )
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
  {
    for( line = 0 ; line < nLines ; ++line )
    {
      for( col = 0 ; col < nCols ; ++col )
      {
        rasterPointer->setValue( col, line, pixelValue, band );
        ++pixelValue;
      }
    }
  }
}

void TsInterpolator::tcNearestNeighbor()
{
  std::auto_ptr< te::rst::Raster > rasterPointer;
  createTestRaster( 2, 4, 4, rasterPointer );
  
  std::vector< std::complex<double> > noDataValues;
  noDataValues.resize( rasterPointer->getNumberOfBands(), std::complex<double>(
    0.0, 0.0 ) );
  
  te::rst::Interpolator interp( rasterPointer.get(), te::rst::NearestNeighbor,
   noDataValues );
  
  std::complex<double> v;
  
  interp.getValue( -0.7, -0.7, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );
  
  interp.getValue( ((double)rasterPointer->getNumberOfColumns()) - 0.3 , -0.7, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );

  interp.getValue( ((double)rasterPointer->getNumberOfColumns()) - 0.3 ,
    ((double)rasterPointer->getNumberOfRows()) - 0.3, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );    
  
  interp.getValue( -0.7 ,
    ((double)rasterPointer->getNumberOfRows()) - 0.3, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );
  
  interp.getValue( 1.5 , 1.5, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 10.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 ); 
  
  interp.getValue( 1.5 , 1.5, v, 1 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 26.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );   
}

void TsInterpolator::tcBilinear()
{
  std::auto_ptr< te::rst::Raster > rasterPointer;
  createTestRaster( 2, 4, 4, rasterPointer );
  
  std::vector< std::complex<double> > noDataValues;
  noDataValues.resize( rasterPointer->getNumberOfBands(), std::complex<double>(
    0.0, 0.0 ) );
  
  te::rst::Interpolator interp( rasterPointer.get(), te::rst::Bilinear,
   noDataValues );
  
  std::complex<double> v;
  
  interp.getValue( -0.7, -0.7, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );
  
  interp.getValue( ((double)rasterPointer->getNumberOfColumns()) - 0.3 , -0.7, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );

  interp.getValue( ((double)rasterPointer->getNumberOfColumns()) - 0.3 ,
    ((double)rasterPointer->getNumberOfRows()) - 0.3, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );    
  
  interp.getValue( -0.7 ,
    ((double)rasterPointer->getNumberOfRows()) - 0.3, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );
  
  interp.getValue( 1.5 , 1.5, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.5, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 ); 
  
  interp.getValue( 1.5 , 1.5, v, 1 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 23.5, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );   
}

void TsInterpolator::tcBicubic()
{
  std::auto_ptr< te::rst::Raster > rasterPointer;
  createTestRaster( 2, 4, 4, rasterPointer );
  
  std::vector< std::complex<double> > noDataValues;
  noDataValues.resize( rasterPointer->getNumberOfBands(), std::complex<double>(
    0.0, 0.0 ) );
  
  te::rst::Interpolator interp( rasterPointer.get(), te::rst::Bicubic,
   noDataValues );
  
  std::complex<double> v;
  
  interp.getValue( -0.7, -0.7, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );
  
  interp.getValue( ((double)rasterPointer->getNumberOfColumns()) - 0.3 , -0.7, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );

  interp.getValue( ((double)rasterPointer->getNumberOfColumns()) - 0.3 ,
    ((double)rasterPointer->getNumberOfRows()) - 0.3, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );    
  
  interp.getValue( -0.7 ,
    ((double)rasterPointer->getNumberOfRows()) - 0.3, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );
  
  interp.getValue( 1.5 , 1.5, v, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.5, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 ); 
  
  interp.getValue( 1.5 , 1.5, v, 1 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 23.5, v.real(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.imag(), 0.0000000001 );   
}
