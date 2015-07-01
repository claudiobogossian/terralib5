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
  \file TsDateTime.h
 
  \brief Test suite for the DateTime class.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATETIME_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATETIME_H

// STL
#include <string>
#include <vector>

// STL
#include <cstdio>
#include <cstdlib>
#include <cstring>

//TerraLib 
//#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/datatype.h>


// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsDateTime

  \brief Test suite for the DateTime Class.

  This test suite will check the following:
  <ul>
  <li> </li>
  <li></li>
  <li></li>
  <li></li>
  </ul>
 */
class TsDateTime : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsDateTime );

// It registers the class methods as Test Cases belonging to the suit 
  
  CPPUNIT_TEST( tcDateTime );
  CPPUNIT_TEST( tcDateTimeParamsY );
  CPPUNIT_TEST( tcDateTimeParamsDate );
  CPPUNIT_TEST( tcDateTimeParamsDateHour );
  CPPUNIT_TEST( tcDateTimeConstructor );
  CPPUNIT_TEST( tcDateTimeAssign );
  CPPUNIT_TEST( tcDateTimeAddDeltaSecs );
  CPPUNIT_TEST( tcDateTimeAddDeltaSecs1 );
  CPPUNIT_TEST( tcDateTimeSubDeltaSecs );
  CPPUNIT_TEST( tcDateTimeSubDeltaSecs1 );
  CPPUNIT_TEST( tcDateTimeEqualOp );
  CPPUNIT_TEST( tcDateTimeMinorOp );
  CPPUNIT_TEST( tcDateTimeMinorEqOp );
  CPPUNIT_TEST( tcDateTimeDifference );
  CPPUNIT_TEST( tcSetDate );
  CPPUNIT_TEST( tcSetAll );

  CPPUNIT_TEST( tcSetYear);
  CPPUNIT_TEST( tcSetMonth);
  CPPUNIT_TEST( tcSetDay);
  CPPUNIT_TEST( tcSetHour);
  CPPUNIT_TEST( tcSetMin);
  CPPUNIT_TEST( tcSetSec);
  CPPUNIT_TEST( tcSetUTC);
  CPPUNIT_TEST( tcSetTimeFromSeconds);

  CPPUNIT_TEST( tcGetYear);
  CPPUNIT_TEST( tcGetMonth);
  CPPUNIT_TEST( tcGetMonthName);

  CPPUNIT_TEST( tcGetDay);
  CPPUNIT_TEST( tcGetHour);
  CPPUNIT_TEST( tcGetMin);
  CPPUNIT_TEST( tcGetSec);
  CPPUNIT_TEST( tcGetUTC);
  CPPUNIT_TEST( tcGetNumberOfDays);
  CPPUNIT_TEST( tcGetDaysFromYear1);

  CPPUNIT_TEST( tcGetDaysFromYear);
  CPPUNIT_TEST( tcGetDaysFromMonth);
  CPPUNIT_TEST( tcGetNow);
  CPPUNIT_TEST( tcGetDateTimeFromSeconds);
  CPPUNIT_TEST( tcGetTimeInSeconds);
  CPPUNIT_TEST( tcGetTimeInSecondsD);
  CPPUNIT_TEST( tcGetTimeInSecondsDT);
  CPPUNIT_TEST( tcGetDiffInSeconds);
  CPPUNIT_TEST( tcGetDiffInSeconds1);
  CPPUNIT_TEST( tcGetWeek);
  CPPUNIT_TEST( tcGetWeekName);
  CPPUNIT_TEST( tcGetDate);
  CPPUNIT_TEST( tcGetDateTime);
  CPPUNIT_TEST( tcGetTime);

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: It creates a date time with local date. The date and time are obtained from the system clock. */
    void tcDateTime();

    /*! \brief Test Case: It creates a custom date time with a given params ymdhms*/
    void tcDateTimeParamsY();
    /*! \brief Test Case: It creates a custom date time with a given params ymdhms*/
    void tcDateTimeParamsDate();
    /*! \brief Test Case: It creates a custom date time with a given params ymdhms*/
    void tcDateTimeParamsDateHour();

    /*! \brief Test Case: Copy constructor. */
    void tcDateTimeConstructor();

    /*! \brief Test Case: Assignment operator. */
    void tcDateTimeAssign(); //??

    /*! \brief Test Case: Add 'delta' seconds and assign to this current date. */
    void tcDateTimeAddDeltaSecs();

    /*! \brief Test Case: Add 'delta' seconds and return date. */
    void tcDateTimeAddDeltaSecs1();

    /*! \brief Test Case: Subtract 'delta' seconds and assign to this current date. */
    void tcDateTimeSubDeltaSecs();

    /*! \brief Test Case: Subtract 'delta' seconds and return date. */
    void tcDateTimeSubDeltaSecs1();

    /*! \brief Test Case: Operator ==. */
    void tcDateTimeEqualOp();

    /*! \brief Test Case: Operator <. */
    void tcDateTimeMinorOp();

    /*! \brief Test Case: Operator <=. */
    void tcDateTimeMinorEqOp();

    /*! \brief Test Case: it returns the difference between the times in seconds. */
    void tcDateTimeDifference();

   /*! \brief Test Case:It initializes the date time. */
    void tcSetDate();

   /*! \brief Test Case:It initializes the date time with Y , M, D, H, Min, Sec, UTC, TimeFromSecs. */
    void tcSetYear();
    void tcSetMonth();
    void tcSetDay();
    void tcSetHour();
    void tcSetMin();
    void tcSetSec();
    void tcSetUTC();
    void tcSetTimeFromSeconds();

   /*! \brief Test Case:It get Y, M, D, H, Min, Sec, UTC, etc. */
    void tcSetAll(); //y,m,d,h,min,sec
    void tcGetYear();
    void tcGetMonth();
    void tcGetMonthName();
    void tcGetDay();
    void tcGetHour();
    void tcGetMin();
    void tcGetSec();
    void tcGetUTC();
    void tcGetNumberOfDays();
    void tcGetDaysFromYear1();

    void tcGetDaysFromYear();
    void tcGetDaysFromMonth();
    void tcGetNow();
    void tcGetDateTimeFromSeconds();
    void tcGetTimeInSeconds();
    void tcGetTimeInSecondsD();
    void tcGetTimeInSecondsDT();
    void tcGetDiffInSeconds();
    void tcGetDiffInSeconds1();

    void tcGetWeek();
    void tcGetWeekName();

    void tcGetDate();
    void tcGetDateTime();
    void tcGetTime();

  private:

    //te::common::DateTime d;
};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATETIME_H

