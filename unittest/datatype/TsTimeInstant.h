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
  \file TsTimeInstant.h
 
  \brief Test suite for the TimeInstant class.
 */

#ifndef __TERRALIB_UNITTEST_DATATYPE_INTERNAL_TIMEINSTANT_H
#define __TERRALIB_UNITTEST_DATATYPE_INTERNAL_TIMEINSTANT_H

// STL
#include <string>
#include <vector>

// STL
#include <cstdio>
#include <cstdlib>
#include <cstring>

//TerraLib 
#include <terralib/datatype.h>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsTimeInstant

  \brief Test suite for the TimeInstant Class.

  This test suite will check the following:
  <ul>
  <li> </li>
  <li></li>
  <li></li>
  <li></li>
  </ul>
 */
class TsTimeInstant : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsTimeInstant );

// It registers the class methods as Test Cases belonging to the suit 
  
  CPPUNIT_TEST( tcTimeInstant );
  CPPUNIT_TEST( tcTimeInstant2 );
  CPPUNIT_TEST( tcTimeInstant3 );
  CPPUNIT_TEST( tcGetTimeInstant );
  CPPUNIT_TEST( tcGetDate );
  CPPUNIT_TEST( tcGetTime );
  CPPUNIT_TEST( tcCheckOperators );
  CPPUNIT_TEST( tcClone );
  CPPUNIT_TEST( tcToString );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Empty Constructor. */
    void tcTimeInstant();

    /*! \brief Test Case: Constructor from a date and time offset */
    void tcTimeInstant2();
 
    /*! \brief Test Case: Constructor from a boost time instant type. */
    void tcTimeInstant3();

    /*! \brief Test Case: It returns the boost time instant type. */
    void tcGetTimeInstant(); 

    /*! \brief Test Case: It returns the date associated to time instant. */
    void tcGetDate(); 

    /*! \brief Test Case:It returns the time duration associated to time instant. */
    void tcGetTime();

   /*! \brief Test Case: Check operators (==, >,<, !=, -). */
    void tcCheckOperators();

    /*! \brief Test Case: Clone this object and return AbstractData. */
    void tcClone();

    /*! \brief It returns the time instant in the ISO textual format (YYYYMMDDTHHMMSS,fffffffff) where T is the date-time separator. */
   void tcToString();

};

#endif  // __TERRALIB_UNITTEST_DATATYPE_INTERNAL_TIMEINSTANT_H
