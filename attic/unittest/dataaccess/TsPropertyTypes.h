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
  \file TsDataTypes.h
 
  \brief Test suite for the PropertyType class hierarchy.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_PROPERTYTYPES_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_PROPERTYTYPES_H

// STL
#include <string>
#include <vector>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsPropertyTypes

  \brief Test suite for the PropertyType class hierarchy.

  This test suite will perform checks in the PropertyType class hierarchy
  without using any associated DataSource.
  <br>
  This test suite will check the following:
  <ul>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  </ul>
 */
class TsPropertyTypes : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsPropertyTypes );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcCreatePropertyTypes );
  CPPUNIT_TEST( tcClone );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: creating all type of properties. */
    void tcCreatePropertyTypes();

    /*! \brief Test Case: cloning properties. */
    void tcClone();
};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_PROPERTYTYPES_H

