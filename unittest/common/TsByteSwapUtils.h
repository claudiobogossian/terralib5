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
  \file TsByteSwapUtils.h
 
  \brief Test suite for ByteSwapUtils.
 */

#ifndef __TERRALIB_UNITTEST_COMMON_INTERNAL_BYTESWAPUTILS_H
#define __TERRALIB_UNITTEST_COMMON_INTERNAL_BYTESWAPUTILS_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsByteSwapUtils

  \brief Test suite for ByteSwapUtils.
 */
class TsByteSwapUtils : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsByteSwapUtils );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcSwap2Bytes );
  CPPUNIT_TEST( tcSwap4Bytes ); 
  CPPUNIT_TEST( tcSwapBytes );
  CPPUNIT_TEST( tcSwap8Bytes );
  
  CPPUNIT_TEST( tcSwapBytes_bint16 ); //boost 
  CPPUNIT_TEST( tcSwapBytes_bint32 );
  CPPUNIT_TEST( tcSwapBytes_buint32 );
  CPPUNIT_TEST( tcSwapBytes_bint16 );
  CPPUNIT_TEST( tcSwapBytes_float );
  CPPUNIT_TEST( tcSwapBytes_double );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:
 
    /*! \brief Test Case: Swap an array of 2 bytes -sizeof(<type>)== 2. */
    void tcSwap2Bytes();

    /*! \brief Test Case: Swap an array of 4 bytes. */
    void tcSwap4Bytes();

    /*! \brief Test Case: Swap an array of n-bytes. */
    void tcSwapBytes();

    /*! \brief Test Case: Swap an array of 8-bytesr. */
    void tcSwap8Bytes();

    /*! \brief Test Case: Swap 2 bytes (boost::int16). */
    void tcSwapBytes_bint16();

    /*! \brief Test Case: Swap 4 bytes (boost::int32). */
    void tcSwapBytes_bint32();

    /*! \brief Test Case: Swap 4 bytes (boost::uint32). */
    void tcSwapBytes_buint32();

    /*! \brief Test Case: Swap 4 bytes (float). */
    void tcSwapBytes_float();

    /*! \brief Test Case: Swap 8 bytes (double). */
    void tcSwapBytes_double();
};

#endif  // __TERRALIB_UNITTEST_COMMON_INTERNAL_BYTESWAPUTILS_H

