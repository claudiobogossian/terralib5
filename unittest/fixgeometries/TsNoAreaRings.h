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
 
  \brief A test suit for the Fix Geometry.
 */

#ifndef __TERRALIB_UNITTEST_FIXGEOMETRIES_INTERNAL_NO_AREA_RINGS_H
#define __TERRALIB_UNITTEST_FIXGEOMETRIES_INTERNAL_NO_AREA_RINGS_H

#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsNoAreaRings

  \brief A test suit for the Fix Geometries class interface.

  ....
  <br>
  This test suite will check the Fix Geometries class
  </ul>
 */
class TsNoAreaRings : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsNoAreaRings );
  
// It registers the class methods as Test Cases belonging to the suit   
  CPPUNIT_TEST(tcCreatePolygon);

  // call other tests (tcXxxx)

  CPPUNIT_TEST_SUITE_END();

  protected :
    
    void tcCreatePolygon();


    // add other tests...(tcXxxx)
};

#endif  // __TERRALIB_UNITTEST_FIXGEOMETRIES_INTERNAL_NO_AREA_RINGS_H

