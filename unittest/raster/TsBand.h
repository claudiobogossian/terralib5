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
  \file TsBand.h
 
  \brief A test suit for the Raster Class.
 */

#ifndef __TERRALIB_UNITTEST_RASTER_INTERNAL_BAND_H
#define __TERRALIB_UNITTEST_RASTER_INTERNAL_BAND_H

#include <terralib/raster/Raster.h>
#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsBand

  \brief A test suit for the Raster Band class interface.

  ....
  <br>
  This test suite will check the Raster Band class
  </ul>
 */
class TsBand : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsBand );
  
  CPPUNIT_TEST(tcBandConstructor );

  CPPUNIT_TEST(tcGetRaster );

  CPPUNIT_TEST(tcOperatorEqual );

  CPPUNIT_TEST(tcGetValue );

  CPPUNIT_TEST(tcSetValue );

  CPPUNIT_TEST(tcGetIValue );

  CPPUNIT_TEST(tcSetIValue );

  // call other tests (tcXxxx)

  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void tcBandConstructor();

    void tcGetRaster();

    void tcOperatorEqual();
    
    void tcGetValue();

    void tcSetValue();

    void tcGetIValue();

    void tcSetIValue();


    // add other tests...(tcXxxx)
};

#endif  // __TERRALIB_UNITTEST_RASTER_INTERNAL_BAND_H

