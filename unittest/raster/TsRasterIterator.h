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
  \file TsRaterIterator.h
 
  \brief A test suit for the Raster Iterator Class.
 */

#ifndef __TERRALIB_UNITTEST_RASTER_INTERNAL_RASTERITERATOR_H
#define __TERRALIB_UNITTEST_RASTER_INTERNAL_RASTERITERATOR_H

#include <terralib/raster.h>
#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsRaterIterator

  \brief A test suit for the Raster Iterator class interface.

  ....
  <br>
  This test suite will check the Raster Iterator class
  </ul>
 */
class TsRasterIterator : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsRasterIterator );
  
  CPPUNIT_TEST(tcRasterIteratorConstructor1 );
  
  CPPUNIT_TEST(tcRasterIteratorConstructor2 );

  CPPUNIT_TEST(tcRasterIteratorConstructor3 );

  CPPUNIT_TEST(tcRasterIteratorGetRow );

  CPPUNIT_TEST(tcRasterIteratorGetCol );
  
  CPPUNIT_TEST(PolygonIteratorTest1 );

  // add other tests (tcXxxx)

  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void CreateTestRaster( unsigned int nBands, unsigned int nLines, 
      unsigned int nCols, boost::shared_ptr< te::rst::Raster >& rasterPointer );    
    
    bool GetDetailedExtent( const te::rst::Grid& grid, 
      te::gm::LinearRing& detailedExtent );    
    
    void tcRasterIteratorConstructor1();

    void tcRasterIteratorConstructor2();
    
    void tcRasterIteratorConstructor3();

    void tcRasterIteratorGetRow();

    void tcRasterIteratorGetCol();

    void PolygonIteratorTest1();

    // add other tests...
};

#endif  // __TERRALIB_UNITTEST_RASTER_INTERNAL_RASTERITERATOR_H

