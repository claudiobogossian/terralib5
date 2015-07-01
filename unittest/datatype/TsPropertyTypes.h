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

#ifndef __TERRALIB_UNITTEST_DATATYPE_INTERNAL_PROPERTYTYPES_H
#define __TERRALIB_UNITTEST_DATATYPE_INTERNAL_PROPERTYTYPES_H

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
  <li>SimpleProperty;</li>
  <li>NumericProperty;</li>
  <li>StringProperty;</li>
  <li>DateTimeProperty;</li>
  <li>BitProperty;</li>
  <li>ArrayProperty;</li>
  <li>GeometryProperty;</li>
  <li>RasterProperty;</li>
  <li>CompositeProperty;</li>
  <li>Other Test Suites are implemented to test ByteaArray, DateTime, Array etc;</li>
  </ul>
 */
class TsPropertyTypes : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsPropertyTypes );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcSimpleProperty );
  CPPUNIT_TEST( tcNumericProperty );
  CPPUNIT_TEST( tcStringProperty );
  CPPUNIT_TEST( tcDateTimeProperty );
  CPPUNIT_TEST( tcBitProperty );
  CPPUNIT_TEST( tcArrayProperty );
  CPPUNIT_TEST( tcGeometryPropertyFirstConstructor );
  CPPUNIT_TEST( tcGeometryPropertyCopyConstructor );
  CPPUNIT_TEST( tcGeometryPropertyFirstConstructorAssign );
  CPPUNIT_TEST( tcGeometryProperty );
  CPPUNIT_TEST( tcRasterProperty );
  CPPUNIT_TEST( tcRasterPropertyClone );
  CPPUNIT_TEST( tcRasterPropertyAssignOp );
  CPPUNIT_TEST( tcCompositeProperty );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Numeric properties. */
    void tcNumericProperty();

    /*! \brief Test Case: String properties (fixed, var, unlimited) . */
    void tcStringProperty();

   /*! \brief Test Case: Simple properties (enum 2-12) . */
    void tcSimpleProperty();

   /*! \brief Test Case: Geometry properties with first constructor (enum 17) . */
    void tcGeometryPropertyFirstConstructor();

    /*! \brief Test Case: Using first constructor and assign. */
    void tcGeometryPropertyFirstConstructorAssign();

    /*! \brief Test Case: Geometry properties -copy constructor . */
    void tcGeometryPropertyCopyConstructor();

   /*! \brief Test Case: Geometry properties (enum 17) . */
    void tcGeometryProperty();

   /*! \brief Test Case: Composite properties (enum 20) . */
    void tcCompositeProperty();

    /*! \brief Test Case: Bit properties. */
    void tcBitProperty();

    /*! \brief Test Case: Array properties. */
    void tcArrayProperty();

    /*! \brief Test Case: Array properties. */
    void tcRasterProperty();

    /*! \brief Test Case: Array properties. */
    void tcRasterPropertyClone();

    /*! \brief Test Case: Array properties. */
    void tcRasterPropertyAssignOp();

    /*! \brief Test Case: DateTimeProperty properties. */
    void tcDateTimeProperty();

};

#endif  // __TERRALIB_UNITTEST_DATATYPE_INTERNAL_PROPERTYTYPES_H

