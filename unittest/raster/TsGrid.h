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
  \file TsGrid.h
 
  \brief A test suit for the Raster Class.
 */

#ifndef __TERRALIB_UNITTEST_RASTER_INTERNAL_GRID_H
#define __TERRALIB_UNITTEST_RASTER_INTERNAL_GRID_H

#include <terralib/raster.h>
#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsGrid

  \brief A test suit for the Grid class interface.

  ....
  <br>
  This test suite will check the Grid class (spatial support for raster data)
  </ul>
 */
class TsGrid : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsGrid );
  
  CPPUNIT_TEST(tcGridConstructor1 );
  
  CPPUNIT_TEST(tcGridConstructor1NoEnv );
  
  CPPUNIT_TEST(tcGridConstructor2 );
  
  CPPUNIT_TEST(tcGridConstructor2NoUlc );

  CPPUNIT_TEST(tcGridConstructor3 );
  
  CPPUNIT_TEST(tcGridConstructor4 );
  
  CPPUNIT_TEST(tcGridConstructor5 );

  // add other tests (tcXxxx)

  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void tcGridConstructor1();
    
    void tcGridConstructor1NoEnv();

    void tcGridConstructor2();
    
    void tcGridConstructor2NoUlc();
    
    void tcGridConstructor3();
    
    void tcGridConstructor4();
    
    void tcGridConstructor5();

    // add other tests...
};

#endif  // __TERRALIB_UNITTEST_RASTER_INTERNAL_GRID_H

