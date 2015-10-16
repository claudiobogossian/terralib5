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
  \file TsRaterIterator.cpp
 
  \brief A test suit for the Raster iterator class.
 */

#include "TsRasterIterator.h"
#include "../Config.h"

#include <terralib/raster/RasterIterator.h>
#include <terralib/raster/PositionIterator.h>
#include <terralib/geometry/Utils.h>

//#include <terralib/dataaccess/datasource/DataSourceFactory.h>

#include <boost/shared_ptr.hpp>

#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION( TsRasterIterator );

void TsRasterIterator::CreateTestRaster( unsigned int nBands, unsigned int nLines, 
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

void TsRasterIterator::tcRasterIteratorConstructor1()
{
}

void TsRasterIterator::tcRasterIteratorConstructor2()
{
}

void TsRasterIterator::tcRasterIteratorConstructor3()
{
}

void TsRasterIterator::tcRasterIteratorGetRow()
{
}

void TsRasterIterator::tcRasterIteratorGetCol()
{
}

void TsRasterIterator::PolygonIteratorTest1()
{
  boost::shared_ptr< te::rst::Raster > rasterPointer;
  CreateTestRaster( 3, 3, 3, rasterPointer );
  
  std::auto_ptr< te::gm::Polygon > polPtr( (te::gm::Polygon*)
    te::gm::GetGeomFromEnvelope( rasterPointer->getGrid()->getExtent(),
      rasterPointer->getGrid()->getSRID() ) );
    
  double pixelValueCounter = 0;

  for( unsigned int band = 0 ; band < rasterPointer->getNumberOfBands() ; 
    ++band )
  {    
    te::rst::PolygonIterator< double > it = 
      te::rst::PolygonIterator< double >::begin( rasterPointer.get(),
      polPtr.get() );
    const te::rst::PolygonIterator< double > itEnd = 
      te::rst::PolygonIterator< double >::end( rasterPointer.get(),
      polPtr.get() );      
      
    while( it != itEnd )
    {
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pixelValueCounter, it[ band ], 
        0.0000000001 );
      
      ++pixelValueCounter;
      ++it;
    }
  }
}
