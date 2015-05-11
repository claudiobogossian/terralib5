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
  \file TsHexUtils.h
 
  \brief Test suite for Hex utils.
 */

#ifndef __TERRALIB_UNITTEST_COMMON_INTERNAL_HEXUTILS_H
#define __TERRALIB_UNITTEST_COMMON_INTERNAL_HEXUTILS_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsHexUtils

  \brief Test suite for hex utils.
 */
class TsHexUtils : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsHexUtils );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcHex2Char );
  CPPUNIT_TEST( tcHex2Char2 ); 
  CPPUNIT_TEST( tcChar2Hex);

  CPPUNIT_TEST( tcGetDecimalFromHexNotCS ); 
  CPPUNIT_TEST( tcGetDecimalFromHexUCase );
  CPPUNIT_TEST( tcGetDecimalFromHexLCase );
  
  CPPUNIT_TEST( tcHex2Binary_1 );
  CPPUNIT_TEST( tcHex2Binary_2 );
  CPPUNIT_TEST( tcHex2Binary_3 );
  CPPUNIT_TEST( tcHex2Binary2 );
  CPPUNIT_TEST( tcBinary2Hex2_1 );
  CPPUNIT_TEST( tcBinary2Hex_size );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Get Decimal from Hex (no case sensitive). */
    void tcGetDecimalFromHexNotCS();
    
    /*! \brief Test Case: Get Decimal from Hex (Upper case). */
    void tcGetDecimalFromHexUCase();

    /*! \brief Test Case: Get Decimal from Hex (Lower case). */
    void tcGetDecimalFromHexLCase();

    /*! \brief Test Case: It Converts char to Hex. */
    void tcChar2Hex();
    /*! \brief Test Case: It Converts Hex to Char. */
    void tcHex2Char();
    /*! \brief Test Case: It Converts Hex to Char. */
    void tcHex2Char2();
    /*! \brief Test Case: It Converts Hex to Binary. */
    void tcHex2Binary_1();
    /*! \brief Test Case: It  Converts Hex to Binary. */
    void tcHex2Binary_2();
    /*! \brief Test Case: It Converts Hex to Binary. */
    void tcHex2Binary_3();
    /*! \brief Test Case: It  Converts Hex to Binary. */
    void tcHex2Binary2();
    /*! \brief Test Case: It Converts Binary to Hex */
    void tcBinary2Hex2_1();
    /*! \brief Test Case: It Converts Binary to Hex. */
    void tcBinary2Hex_size();
};

#endif  // __TERRALIB_UNITTEST_COMMON_INTERNAL_HEXUTILS_H

