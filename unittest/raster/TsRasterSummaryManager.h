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
  \file TsRasterSummaryManager.h
 
  \brief A test suit for the RasterSummaryManager Class.
 */

#ifndef __TERRALIB_UNITTEST_RASTER_INTERNAL_RASTERSUMMARYMANAGER_H
#define __TERRALIB_UNITTEST_RASTER_INTERNAL_RASTERSUMMARYMANAGER_H

#include <terralib/raster.h>
#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsRasterSummaryManager

  \brief A test suit for the RasterSummaryManager class interface.

  ....
  <br>
  This test suite will check the RasterSummaryManager class
  </ul>
 */
class TsRasterSummaryManager : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsRasterSummaryManager );
  
  CPPUNIT_TEST( tcRasterSummaryManagerConstructor );
  
  CPPUNIT_TEST(tcRasterSummaryManagerAdd );

  CPPUNIT_TEST(tcRasterSummaryManagerFind );

  CPPUNIT_TEST(tcRasterSummaryManagerRemove );

  CPPUNIT_TEST(tcRasterSummaryManagerGet );


  // add other tests (tcXxxx)

  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void tcRasterSummaryManagerConstructor();

    void tcRasterSummaryManagerAdd();
    
    void tcRasterSummaryManagerFind();
        
    void tcRasterSummaryManagerRemove();

    void tcRasterSummaryManagerGet();


    // add other tests...
};

#endif  // __TERRALIB_UNITTEST_RASTER_INTERNAL_RASTERSUMMARYMANAGER_H

