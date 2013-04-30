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
  \file TsExpansibleRaster.h
 
  \brief A test suit for the Expansible Raster Class.
 */

#ifndef __TERRALIB_UNITTEST_MEMORY_EXPANSIBLERASTER_INTERNAL_H
#define __TERRALIB_UNITTEST_MEMORY_EXPANSIBLERASTER_INTERNAL_H

#include <terralib/memory/Raster.h>
#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsExpansibleRaster

  \brief A test suit for the Cached Raster class interface.

  ....
  <br>
  This test suite will check the Cached Raster class
  </ul>
 */
class TsExpansibleRaster : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsExpansibleRaster );
  
  CPPUNIT_TEST( readWriteTest );
  
  CPPUNIT_TEST( addTopLinesTest );
  
  CPPUNIT_TEST( addBottomLinesTest );
  
  CPPUNIT_TEST( addLeftColumnsTest );
  
  CPPUNIT_TEST( addRightColumnsTest );
  
  CPPUNIT_TEST( addTopBandsTest );
  
  CPPUNIT_TEST( addBottomBandsTest );
  
  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void writeValues( te::rst::Raster& raster );
    
    void fillWithNoDataValues( te::rst::Raster& raster );
    
    void testValues( te::rst::Raster& raster );

    void readWriteTest();
    
    void addTopLinesTest();
    
    void addBottomLinesTest();
    
    void addLeftColumnsTest();
    
    void addRightColumnsTest();
    
    void addTopBandsTest();
    
    void addBottomBandsTest();
};

#endif  // __TERRALIB_UNITTEST_MEMORY_EXPANSIBLERASTER_INTERNAL_H

