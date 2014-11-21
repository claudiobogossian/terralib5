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
  \file TsCachedRaster.h
 
  \brief A test suit for the Cached Raster Class.
 */

#ifndef __TERRALIB_UNITTEST_MEMORY_CACHEDRASTER_INTERNAL_H
#define __TERRALIB_UNITTEST_MEMORY_CACHEDRASTER_INTERNAL_H

#include <terralib/memory.h>

// Boost
#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsCachedRaster

  \brief A test suit for the Cached Raster class interface.

  ....
  <br>
  This test suite will check the Cached Raster class
  </ul>
 */
class TsCachedRaster : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsCachedRaster );
  
  CPPUNIT_TEST( ReadWriteTest );
  
  CPPUNIT_TEST( ReadAheadTest );

  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void CreateTestRaster( unsigned int nBands, unsigned int nLines, 
      unsigned int nCols, boost::shared_ptr< te::rst::Raster >& rasterPointer );

    void ReadWriteTest();
    
    void ReadAheadTest();
};

#endif  // __TERRALIB_UNITTEST_MEMORY_CACHEDRASTER_INTERNAL_H

