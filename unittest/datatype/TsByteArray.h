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
  \file TsByteArray.h
 
  \brief Test suite for the ByteArray class.
 */

#ifndef __TERRALIB_UNITTEST_DATATYPE_INTERNAL_BYTEARRAY_H
#define __TERRALIB_UNITTEST_DATATYPE_INTERNAL_BYTEARRAY_H

// STL
#include <string>
#include <vector>

// STL
#include <cstdio>
#include <cstdlib>
#include <cstring>

//TerraLib 
#include <terralib/common.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>


// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsByteArray

  \brief Test suite for the ByteArray Class.

  This test suite will check the following:
  <ul>
  <li> </li>
  <li></li>
  <li></li>
  <li></li>
  </ul>
 */
class TsByteArray : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsByteArray );

// It registers the class methods as Test Cases belonging to the suit 
  
  CPPUNIT_TEST( tcByteArray );
  CPPUNIT_TEST( tcByteArraySize );
  CPPUNIT_TEST( tcInit );
  CPPUNIT_TEST( tcInit1 );
  CPPUNIT_TEST( tcByteArrayConstructor );
  CPPUNIT_TEST( tcTake );
  CPPUNIT_TEST( tcTake1 );
  CPPUNIT_TEST( tcCopy );
  CPPUNIT_TEST( tcCopy1 );
  CPPUNIT_TEST( tcClone);
  CPPUNIT_TEST( tcGetTypeCode);
  CPPUNIT_TEST( tcToString);
  CPPUNIT_TEST( tcClear);
  CPPUNIT_TEST( tcSetBytesUsed );
  CPPUNIT_TEST( tcOperator );

  CPPUNIT_TEST_SUITE_END();
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Constructs an empty byte array. */
    void tcByteArray();

    /*! \brief Test Case: Creates a new byte array and allocate size bytes*/
    void tcByteArraySize();

    /*! \brief Test Case: Creates a new byte array initializing it with data using all capacity. */
    void tcInit();

    /*! \brief Test Case: Creates a new byte array initializing it with data less than capacity. */
    void tcInit1();

    /*! \brief Test Case: It takes the ownership of the external data buffer. */
    void tcTake();

     /*! \brief Test Case: It takes the ownership of the external data buffer. */
    void tcTake1();

    /*! \brief Test Case: It copies the data from the given pointer to the byte array. */
    void tcCopy();

     /*! \brief Test Case: It copies the data from the given pointer to the byte array and will expand the internal buffer if its capacity is less than offset + size bytes. */
    void tcCopy1();

    /*! \brief Test Case: Clone ByteArray. */
    void tcClone();

    /*! \brief Test Case: Copy Constructor with size bigger than data. */
    void tcByteArrayConstructor(); 

    /*! \brief Test Case: Returns the array data after constructor. */
    void tcGetData();

    /*! \brief Test Case: Clear the byte array. */
    void tcClear();

    /*! \brief Test Case: It return an string representation of ByteArray. */
    void tcToString();

    /*! \brief Test Case: Get Type code  of ByteArray. */
    void tcGetTypeCode();

    /*! \brief Test Case: Set bytes used by ByteArray. */
    void tcSetBytesUsed();

    /*! \brief Test Case: Test operator << (writes simple data types (int,double, float) to bytearray */
    void tcOperator();

  private:

    //te::gm::ByteArray* boxA; 

};

#endif  // __TERRALIB_UNITTEST_DATATYPE_INTERNAL_BYTEARRAY_H

