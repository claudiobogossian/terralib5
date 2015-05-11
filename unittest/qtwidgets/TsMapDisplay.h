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
  \file TsMapDisplay.h
 
  \brief Test suite for the Qt Map Display widget.
 */

#ifndef __TERRALIB_UNITTEST_QTWIDGETS_INTERNAL_CANVAS_H
#define __TERRALIB_UNITTEST_QTWIDGETS_INTERNAL_CANVAS_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsMapDisplay

  \brief Test suite for the Qt canvas widget.

  This test suite will check the following:
  <ul>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  </ul>
 */
class TsMapDisplay : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsMapDisplay );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcSetExtent );
  CPPUNIT_TEST( tcShowLayer );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: it will test if the map display is working properly when showing a given geogragic area. */
    void tcSetExtent();

    /*! \brief Test Case: it will show a new layer in the map display. */
    void tcShowLayer();
};

#endif  // __TERRALIB_UNITTEST_QTWIDGETS_INTERNAL_CANVAS_H

