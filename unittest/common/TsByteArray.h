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
  \file TsByteArray.h
 
  \brief Test suite for the ByteArray class.
 */

#ifndef __TERRALIB_UNITTEST_COMMON_INTERNAL_BYTEARRAY_H
#define __TERRALIB_UNITTEST_COMMON_INTERNAL_BYTEARRAY_H

// STL
#include <string>
#include <vector>

// STL
#include <cstdio>
#include <cstdlib>
#include <cstring>

//TerraLib 
#include <terralib/common.h>
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
  CPPUNIT_TEST( tcInitOk );

  CPPUNIT_TEST( tcSetDataAfterByteArrayDataSize );
  CPPUNIT_TEST( tcSetDataAfterByteArraySize );
  CPPUNIT_TEST( tcByteArrayDataSizeOk );

  CPPUNIT_TEST( tcByteArrayConstructor );
  CPPUNIT_TEST( tcByteArrayConstructor1 );

  CPPUNIT_TEST( tcAssign );
  CPPUNIT_TEST( tcGetData );
  CPPUNIT_TEST( tcGetDataAfterInit );

  CPPUNIT_TEST( tcSetData );
  CPPUNIT_TEST( tcSetData1 );
  CPPUNIT_TEST( tcSetData2 );
  CPPUNIT_TEST( tcSetDataOk );

  CPPUNIT_TEST( tcSetDataAfterInit );
  CPPUNIT_TEST( tcSetDataAfterInit1 );
  CPPUNIT_TEST( tcSetDataAfterInitOk );

  CPPUNIT_TEST( tcDataSize );
  CPPUNIT_TEST( tcDataSizeOk );

  CPPUNIT_TEST( tcClear );

  //CPPUNIT_TEST( tcGetHexString );
 

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


    /*! \brief Test Case: It set the data for array using strlen as size. */
    void tcInit();

    /*! \brief Test Case: It set the data for array using strlen as size. */
    void tcInit1();

    /*! \brief Test Case: It set the data for array using strlen+1 as size. */
    void tcInitOk();

    /*! \brief Test Case: Creates a new byte array with strlen and test getData. */
    void tcSetDataAfterByteArrayDataSize();

    /*! \brief Test Case: Creates a new byte array of 100, setData with strlen and test getData. */
    void tcSetDataAfterByteArraySize();

    /*! \brief Test Case: Creates a new byte array passing strlen+1 with differents constructors and test setData. */
    void tcByteArrayDataSizeOk();

    /*! \brief Test Case: Copy Constructor. */
    void tcByteArrayConstructor(); //??

    /*! \brief Test Case: Copy Constructor with size bigger than data. */
    void tcByteArrayConstructor1(); //??

    /*! \brief Test Case: Assign operator. */
    void tcAssign();

    /*! \brief Test Case: Returns the array data after constructor. */
    void tcGetData();

    /*! \brief Test Case: Returns the array data after init. */
    void tcGetDataAfterInit();

    /*! \brief Test Case: Returns the array data after init. */
    void tcGetDataAfterInit1();

    /*! \brief Test Case: Copy the data to the byte array. */
    void tcSetData();

    /*! \brief Test Case: Copy the data to the byte array passing strlen. */
    void tcSetData1();

    /*! \brief Test Case: Copy a bigger data to an existing byte array passing strlen. */
    void tcSetData2();

    /*! \brief Test Case: Copy the data to the byte array passing strlen+1. */
    void tcSetDataOk();

    /*! \brief Test Case: Using setData (with a smaller data) after init method, passing strlen */
    void tcSetDataAfterInit();

    /*! \brief Test Case: Using setData (with a bigger data) after init method, passing strlen. */
    void tcSetDataAfterInit1();

    /*! \brief Test Case: Using setData (with a smaller data) after init method, passing strlen+1. */
    void tcSetDataAfterInitOk();

    /*! \brief Test Case: Return number of bytes used in the array. */
    void tcDataSize();

    /*! \brief Test Case: Return number of bytes used in the array, passing strlen+1 */
    void tcDataSizeOk();

    /*! \brief Test Case: Clear the byte array. */
    void tcClear();

    /*! \brief Test Case: It return an hexadecimal string representation for the byte array. */
    void tcGetHexString();

  private:

    //te::gm::ByteArray* boxA; 

};

#endif  // __TERRALIB_UNITTEST_COMMON_INTERNAL_BYTEARRAY_H

