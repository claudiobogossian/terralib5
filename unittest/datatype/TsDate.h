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
  \file TsDate.h
 
  \brief Test suite for the Date class.
 */

#ifndef __TERRALIB_UNITTEST_DATATYPE_INTERNAL_DATE_H
#define __TERRALIB_UNITTEST_DATATYPE_INTERNAL_DATE_H

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
  \class TsDate

  \brief Test suite for the Date, DateDuration and DatePeriod Classes.

  This test suite will check the following:
  <ul>
  <li> </li>
  <li></li>
  <li></li>
  <li></li>
  </ul>
 */
class TsDate : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsDate );

// It registers the class methods as Test Cases belonging to the suit 
  
  CPPUNIT_TEST( tcDate );
  CPPUNIT_TEST( tcDateCheck );
  CPPUNIT_TEST( tcDateConstructor );
  CPPUNIT_TEST( tcDateTimeDuration );
  CPPUNIT_TEST( tcCheckOperators );
  CPPUNIT_TEST( tcClone );
  CPPUNIT_TEST( tcToString );

// Testing DateDuration Class
  CPPUNIT_TEST( tcDateDuration );

// Testing DatePeriod Class
  CPPUNIT_TEST( tcDatePeriod );


  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: It creates a date time with local date. The date and time are obtained from the system clock. */
    void tcDate();

    /*! \brief Test Case: It checks date values (YY,MM,DD) */
    void tcDateCheck();
 
    /*! \brief Test Case: Copy constructor. */
    void tcDateConstructor();

    /*! \brief Test Case: Check TimeInstant. */
    void tcDateTimeDuration(); 

    /*! \brief Test Case: Check operators (==, >,<, !=, -). */
    void tcCheckOperators();

    /*! \brief Test Case: Clone this object and return AbstractData. */
    void tcClone();

    /*! \brief It returns the date in the ISO textual format (YYYYMMDD). */
   void tcToString();

   void tcDateDuration();

   void tcDatePeriod();

};

#endif  // __TERRALIB_UNITTEST_DATATYPE_INTERNAL_DATE_H

