/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// Unit-Test TerraLib
#include "TsDateTime.h"

// TerraLib
#include "terralib/datatype/Date.h"
#include "terralib/datatype/DateTime.h"

//static te::dt::DateTime d(2010,8,9,15,58,39);
static te::dt::Date d(boost::gregorian::greg_year(2010), boost::gregorian::greg_month(8),
                          boost::gregorian::greg_day(9));

CPPUNIT_TEST_SUITE_REGISTRATION( TsDateTime );

void TsDateTime::setUp()
{
}

void TsDateTime::tearDown()
{
}

void TsDateTime::tcDateTime()
{
//#ifdef TE_COMPILE_ALL

  te::dt::Date d1;

//#endif
}

void TsDateTime::tcDateTimeParamsY()
{
//#ifdef TE_COMPILE_ALL

  te::dt::Date d1(boost::gregorian::greg_year(2010), boost::gregorian::greg_month(1),
                  boost::gregorian::greg_day(1)); //d1(2010,1,1) m_utc= -3
 
  CPPUNIT_ASSERT(d1.getYear() == 2010 && d1.getMonth() == 1 && d1.getDay() == 1);

  //Initial time...
  te::dt::Date d2(boost::gregorian::greg_year(1), boost::gregorian::greg_month(1),
                  boost::gregorian::greg_day(1)); //d2(1,1,1,0,0,0) m_utc= -3
  CPPUNIT_ASSERT(d2.getYear() == 1 && d2.getMonth() == 1 && d2.getDay() == 1);

  //It is accepting invalid year  
  te::dt::Date d3(boost::gregorian::greg_year(0), boost::gregorian::greg_month(1),
                          boost::gregorian::greg_day(1));; //it will be d3(0,1,1,0,0,0) m_utc= -3
  CPPUNIT_ASSERT(d3.getYear() == 0);

//#endif
}

void TsDateTime::tcDateTimeParamsDate()
{
//#ifdef TE_COMPILE_ALL

  te::dt::Date d1(boost::gregorian::greg_year(2010), boost::gregorian::greg_month(12),
                  boost::gregorian::greg_day(31)); // m_utc = -3
  //long long int d1_timeInSecs = d1.getTimeInSeconds();
  //te::dt::Date d1e; d1e.setTimeFromSeconds(d1_timeInSecs);
  //CPPUNIT_ASSERT(d1 == d1e);

  //It is accepting invalid date
  te::dt::Date d2(boost::gregorian::greg_year(2010), boost::gregorian::greg_month(31),
                  boost::gregorian::greg_day(32)); // m_utc= -3
  //long long int d2_timeInSecs = d2.getTimeInSeconds();
  //te::dt::Date d2e; d2e.setTimeFromSeconds(d2_timeInSecs);
  CPPUNIT_ASSERT(d2.getMonth() <= 12 && d2.getDay() <= 31 );

//#endif
}
void TsDateTime::tcDateTimeParamsDateHour()
{
//#ifdef TE_COMPILE_ALL

  //te::dt::Date d1(2010,12,31,23,59,59); // m_utc = -3
  //long long int d1_timeInSecs = d1.getTimeInSeconds();
  //te::dt::Date d1e; d1e.setTimeFromSeconds(d1_timeInSecs);
  //CPPUNIT_ASSERT( d1.getYear() == d1e.getYear() || d1.getMonth() == d1e.getMonth() || d1.getDay() == d1e.getDay());
  //CPPUNIT_ASSERT( d1.getHour() == d1e.getHour() || d1.getMin() == d1e.getMin() || d1.getSec() == d1e.getSec());


  ////It is accepting invalid values for date and/or hour
  //te::dt::Date d2(2010,12,31,60,60,60); // m_utc = -3
  //long long int d2_timeInSecs = d2.getTimeInSeconds();
  //te::dt::Date d2e; 
  //d2e.setTimeFromSeconds(d2_timeInSecs);
  //CPPUNIT_ASSERT_MESSAGE("Invalid date is being accepted", d2.getYear() == 2010 && d2.getMonth() == 12 && d2.getDay() == 31);
  //CPPUNIT_ASSERT_MESSAGE("Invalid hour is being accepted", d2.getHour() < 24 && d2.getMin() < 60 && d2.getSec()  < 60);

//#endif
}

void TsDateTime::tcDateTimeConstructor()
{
//#ifdef TE_COMPILE_ALL

  te::dt::Date d1(d);
  CPPUNIT_ASSERT( d1.getYear() == d.getYear() && d1.getMonth() == d.getMonth() && d1.getDay() == d.getDay());
  //CPPUNIT_ASSERT( d1.getHour() == d.getHour() && d1.getMin() == d.getMin() && d1.getSec() == d.getSec());

//#endif
}

void TsDateTime::tcDateTimeAssign()
{
//#ifdef TE_COMPILE_ALL

  //te::dt::Date d1 = d; //(2010,8,9,15,58,39)
  //te::dt::Date d2;
  //te::dt::Date d3;
  //d2.operator =(d);
  //d3 = d;

  //CPPUNIT_ASSERT( d1.getYear() == d.getYear() && d1.getMonth() == d.getMonth() && d1.getDay() == d.getDay());
  //CPPUNIT_ASSERT( d1.getHour() == d.getHour() && d1.getMin() == d.getMin() && d1.getSec() == d.getSec());
  //CPPUNIT_ASSERT( d2.getYear() == d.getYear() && d2.getMonth() == d.getMonth() && d2.getDay() == d.getDay());
  //CPPUNIT_ASSERT( d2.getHour() == d.getHour() && d2.getMin() == d.getMin() && d2.getSec() == d.getSec());
  //CPPUNIT_ASSERT( d3.getYear() == d.getYear() && d3.getMonth() == d.getMonth() && d3.getDay() == d.getDay());
  //CPPUNIT_ASSERT( d3.getHour() == d.getHour() && d3.getMin() == d.getMin() && d3.getSec() == d.getSec());

//#endif
}


void TsDateTime::tcDateTimeAddDeltaSecs()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)
  //d1+= 20;
  //CPPUNIT_ASSERT(d1.getSec()== 59);

//#endif
}

void TsDateTime::tcDateTimeAddDeltaSecs1()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);  //2010,8,9,15,58,39
  //te::dt::Date d2,d3,d4;
  //long long int dif1,dif2,dif3;
  ////add 20 segundos
  //d2 = d1 + 20;
  //d3 = d1 + 2000;
  //d4 = d1 + 250666; //adding (2 dias, 21 horas, 37 min, 46 seg) = d4(2010,8,12,13,36,25)
  //dif1 = d2 - d1;
  //dif2 = d3.operator -(d1);
  //dif3 = d4 - d1;
  //CPPUNIT_ASSERT(dif1 == 20);
  //CPPUNIT_ASSERT(dif2 == 2000);
  //CPPUNIT_ASSERT(dif3 == 250666);
  //CPPUNIT_ASSERT(d4.getDay()==12);
  //CPPUNIT_ASSERT(d4.getHour()==13);
  //CPPUNIT_ASSERT(d4.getMin()==36);
  //CPPUNIT_ASSERT(d4.getSec()==25);

  //CPPUNIT_ASSERT(d2 - 20 == d1);
  //CPPUNIT_ASSERT(d3 - 2000 == d1);
  //CPPUNIT_ASSERT(d4 - 250666 == d1);

//#endif
}

void TsDateTime::tcDateTimeSubDeltaSecs()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)

  ////subtracting  segundos
  //d1-= 20;
  //CPPUNIT_ASSERT(d1.getSec()== 19);
  //d1-= 60;
  //CPPUNIT_ASSERT(d1.getSec()== 19 && d1.getMin()== 57); //(2010,8,9,15,57,19)
  //d1-= 121;  
  //CPPUNIT_ASSERT(d1.getSec()== 18 && d1.getMin()== 55); //(2010,8,9,15,55,18)

//#endif
}

void TsDateTime::tcDateTimeSubDeltaSecs1()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)
  //te::dt::Date d2;
  ////subtract 20 segundos
  //d2 = d1 - 20;
  //CPPUNIT_ASSERT(d2.getSec()== 19);

//#endif
}

void TsDateTime::tcDateTimeEqualOp()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)

  //te::dt::Date d2(d1);
  //CPPUNIT_ASSERT(d1 == d2);

//#endif
}

void TsDateTime::tcDateTimeMinorOp()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)
  //te::dt::Date d2 = d1 - 20;
  //CPPUNIT_ASSERT(d2.getSec() == 19);
  //CPPUNIT_ASSERT(d2 < d1);

//#endif
}

void TsDateTime::tcDateTimeMinorEqOp()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)

  //te::dt::Date d2 = d1 - 20;
  //te::dt::Date d3(d2);
  //CPPUNIT_ASSERT(d2 <= d1);
  //CPPUNIT_ASSERT(d3 <= d2);

//#endif
}

void TsDateTime::tcDateTimeDifference()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)

  //te::dt::Date d2 = d1 + 20;
  //te::dt::Date d3 = d1 - 20;

  //long long int dif = d2 - d1;
  //CPPUNIT_ASSERT( dif == 20);
  //long long int dif1 = d1 - d2;
  //CPPUNIT_ASSERT( dif1 == -20);

//#endif
}

void TsDateTime::tcSetDate()
{
//#ifdef TE_COMPILE_ALL

  //te::dt::Date d1(2010,8,9,15,58,39);   //(2010,8,9,15,58,39)
  //te::dt::Date d2(2010,2,1,20,31,20);

//#endif
}

void TsDateTime::tcSetAll()
{
//#ifdef TE_COMPILE_ALL

  //te::dt::Date d2(2010,8,9,15,58,39);   //(2010,8,9,15,58,39)
  //te::dt::Date d1;
  //d1.setYear(2010);
  //d1.setMonth(8);
  //d1.setDay(9);
  //d1.setHour(15);
  //d1.setMin(58);
  //d1.setSec(39);

  //int diasM= d1.getDaysFromMonth();
  //int diasM1 = te::dt::Date::getDaysFromMonth(d1.getYear(),d1.getMonth());
  //int diasY1 = d1.getDaysFromYear();
  //int diasY2 = te::dt::Date::getDaysFromYear(d1.getYear());

  //CPPUNIT_ASSERT( d1 == d2);
  //CPPUNIT_ASSERT( d1.getDate() == d2.getDate());
  //CPPUNIT_ASSERT( d1.getYear() == d2.getYear());
  //CPPUNIT_ASSERT( d1.getMonth() == d2.getMonth());
  //CPPUNIT_ASSERT( d1.getDay() == d2.getDay());
  //CPPUNIT_ASSERT( d1.getTime() == d2.getTime());
  //CPPUNIT_ASSERT( d1.getHour() == d2.getHour());
  //CPPUNIT_ASSERT( d1.getMin() == d2.getMin());
  //CPPUNIT_ASSERT( d1.getSec() == d2.getSec());
  //CPPUNIT_ASSERT( d1.getDaysFromYear() == d2.getDaysFromYear());
  //CPPUNIT_ASSERT( d1.getMonthName() == d2.getMonthName());

  //CPPUNIT_ASSERT( diasM  == 31);
  //CPPUNIT_ASSERT( diasM1 == 31);
  //CPPUNIT_ASSERT( diasY1 == 365);
  //CPPUNIT_ASSERT( diasY2 == 365);
  //CPPUNIT_ASSERT (d1.getMonthName() == "august");
  //d1.setMonth("september");
  //CPPUNIT_ASSERT (d1.getMonthName() == "september");
  //CPPUNIT_ASSERT (d1.getMonth() == 9);

//#endif
}

void TsDateTime::tcSetYear()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcSetMonth()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcSetDay()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1;
  //d1.setDay(32);
  //CPPUNIT_ASSERT (d1.getDay() > 31 );
//#endif
}

void TsDateTime::tcSetHour()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1;
  //d1.setHour(25);
  //CPPUNIT_ASSERT (d1.getHour() > 23);


//#endif
}

void TsDateTime::tcSetMin()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1;
  //d1.setMin(60);
  //CPPUNIT_ASSERT (d1.getMin() > 59);

//#endif
}

void TsDateTime::tcSetSec()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1;
  //d1.setSec(60);
  //CPPUNIT_ASSERT (d1.getSec() > 59);

//#endif
}

void TsDateTime::tcSetUTC()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcSetTimeFromSeconds()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)
  //te::dt::Date d2;
  //d2.setTimeFromSeconds(63416966319);
  //CPPUNIT_ASSERT( d1 == d2);

//#endif
}

void TsDateTime::tcGetYear()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcGetMonth()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcGetMonthName()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcGetDay()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcGetHour()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcGetMin()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcGetSec()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcGetUTC()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcGetNumberOfDays()
{
//#ifdef TE_COMPILE_ALL
  //int dfyear = te::dt::Date::getNumberOfDays(1,1,31);
  //CPPUNIT_ASSERT( dfyear == 30); //shoud not be 31?

  //dfyear = te::dt::Date::getNumberOfDays(1,2,1);
  //CPPUNIT_ASSERT( dfyear == 31); //shoud not be 32?

  //dfyear = te::dt::Date::getNumberOfDays(1,2,28);
  //CPPUNIT_ASSERT( dfyear == 58);
  //dfyear = te::dt::Date::getNumberOfDays(1,12,31);
  //CPPUNIT_ASSERT( dfyear == 364);
  //dfyear = te::dt::Date::getNumberOfDays(2,12,31);
  //CPPUNIT_ASSERT( dfyear == 729);
  //dfyear = te::dt::Date::getNumberOfDays(2000,12,31);
  //CPPUNIT_ASSERT( dfyear == 730484);
  //dfyear = te::dt::Date::getNumberOfDays(3,12,31);
  //CPPUNIT_ASSERT( dfyear == 1094);
  //dfyear = te::dt::Date::getNumberOfDays(12,12,31);
  //CPPUNIT_ASSERT( dfyear == 4382);
  //dfyear = te::dt::Date::getNumberOfDays(2001,12,31);
  //CPPUNIT_ASSERT( dfyear == 730849);

//#endif
}

void TsDateTime::tcGetDaysFromYear1()
{
//#ifdef TE_COMPILE_ALL
  //int dyear = te::dt::Date::getDaysFromYear(2010);
  //CPPUNIT_ASSERT( dyear == 365);

  //dyear = te::dt::Date::getDaysFromYear(2000);
  //CPPUNIT_ASSERT( dyear == 366);

//#endif
}

void TsDateTime::tcGetDaysFromYear()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)
  //int dfyear = d1.getDaysFromYear();
  //CPPUNIT_ASSERT( dfyear == 365);
  //te::dt::Date d2(2000,8,9);
  //dfyear = d2.getDaysFromYear();
  //CPPUNIT_ASSERT( dfyear == 366);
  //d2.setYear(3);
  //dfyear = d2.getDaysFromYear();
  //CPPUNIT_ASSERT( dfyear == 365);
  //d2.setYear(12);
  //dfyear = d2.getDaysFromYear();
  //CPPUNIT_ASSERT( dfyear == 366);


//#endif
}

void TsDateTime::tcGetDaysFromMonth()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(d);   //(2010,8,9,15,58,39)
  //int dfmonth = d1.getDaysFromMonth();
  //CPPUNIT_ASSERT( dfmonth == 31);
  //te::dt::Date d2(2000,2);
  //dfmonth = d2.getDaysFromMonth();
  //CPPUNIT_ASSERT( dfmonth == 29);
  //d2.setYear(2001);
  //dfmonth = d2.getDaysFromMonth();
  //CPPUNIT_ASSERT( dfmonth == 28);
  //d2.setYear(12);
  //dfmonth = d2.getDaysFromMonth();
  //CPPUNIT_ASSERT( dfmonth == 29);
  //d2.setMonth(6);
  //dfmonth = d2.getDaysFromMonth();
  //CPPUNIT_ASSERT( dfmonth == 30);

//#endif
}

void TsDateTime::tcGetNow()
{
//#ifdef TE_COMPILE_ALL

//#endif
}

void TsDateTime::tcGetDateTimeFromSeconds()
{
//#ifdef TE_COMPILE_ALL
  //long long int d_timeInSecs = d.getTimeInSeconds();
  //te::dt::Date d1 = te::dt::Date::getDateTimeFromSeconds(10); 
  //te::dt::Date d2 = te::dt::Date::getDateTimeFromSeconds(2*60*60);

  //te::dt::Date d3 = te::dt::Date::getDateTimeFromSeconds(10*(24*60*60));
  //te::dt::Date d3x = te::dt::Date::getDateTimeFromSeconds(10*60*60 -1 ); //dia 9 e nao 10

  //te::dt::Date d4 = te::dt::Date::getDateTimeFromSeconds(35*(24*60*60)); 
  //te::dt::Date d5 = te::dt::Date::getDateTimeFromSeconds(365*(24*60*60));
  //te::dt::Date d5x = te::dt::Date::getDateTimeFromSeconds(365*(24*60*60)-1);

  //te::dt::Date d6 = te::dt::Date::getDateTimeFromSeconds(63416908800);
  //te::dt::Date d7 = te::dt::Date::getDateTimeFromSeconds(d_timeInSecs);
  //
  //te::dt::Date d1a(1,1,1,0,0,10);
  //te::dt::Date d2a(1,1,1,2,0,0);
  //te::dt::Date d3a(1,1,11,0,0,0);
  //te::dt::Date d3xa(1,1,1,9,59,59);

  //te::dt::Date d4a(1,2,5,0,0,0);
  //te::dt::Date d5a(2,1,1,0,0,0);
  //te::dt::Date d5xa(1,12,31,23,59,59);

  //te::dt::Date d6a(2010,8,9,0,0,0);

  //te::dt::Date d7a(1,1,1,2,0,0);


  //CPPUNIT_ASSERT( d1 == d1a );
  //CPPUNIT_ASSERT( d2 == d2a );
  //CPPUNIT_ASSERT( d3 == d3a );
  //CPPUNIT_ASSERT( d3x == d3xa );
  //CPPUNIT_ASSERT( d4 == d4a );
  //CPPUNIT_ASSERT( d5 == d5 );
  //CPPUNIT_ASSERT( d5x == d5xa );
  //CPPUNIT_ASSERT( d6 == d6a );

  //CPPUNIT_ASSERT( d7 == d );

  //#endif
}

void TsDateTime::tcGetTimeInSeconds()
{
//#ifdef TE_COMPILE_ALL

  //te::dt::Date d0(0,1,1); //should be invalid year = 0
  //te::dt::Date d1(1,1,1);
  //te::dt::Date d2(2,1,1);
  //te::dt::Date d3 = d2+100;
  //te::dt::Date d4 = d1+100;
  //te::dt::Date dd(2010,8,9,15,58,39);   //(2010,8,9,15,58,39)
  //long long int d0_timeInSecs = d0.getTimeInSeconds();

  //long long int d1_timeInSecs = d1.getTimeInSeconds();
  //long long int d2_timeInSecs = d2.getTimeInSeconds();
  //long long int d3_timeInSecs = d3.getTimeInSeconds();
  //long long int d4_timeInSecs = d4.getTimeInSeconds();
  //long long int dd_timeInSecs = dd.getTimeInSeconds();

  //CPPUNIT_ASSERT( (d3_timeInSecs - d2_timeInSecs) == 100);
  //CPPUNIT_ASSERT( (d4_timeInSecs - d1_timeInSecs) == 100);
  //CPPUNIT_ASSERT( (dd_timeInSecs - d1_timeInSecs) == 63416966319);


  ////Invalid data
  //te::dt::Date d5(1,0,0);
  //long long int d5_timeInSecs = d5.getTimeInSeconds();
  //CPPUNIT_ASSERT_MESSAGE("Date invalid month=0, day=0", d5_timeInSecs < 0);

//#endif
}

void TsDateTime::tcGetTimeInSecondsD() 
{
//#ifdef TE_COMPILE_ALL
  //long long int timeInSecD = te::dt::Date::getTimeInSeconds(2010,8,9);
  //long long int timeInSecD1 = te::dt::Date::getTimeInSeconds(2010,8,10); //+24hs = 84600 sec
  //long long int timeInSec  = te::dt::Date::getTimeInSeconds(2010,8,9,15,58,39);
  //long long int timeInSec1 = te::dt::Date::getTimeInSeconds(2010,8,9,15,58,59); //+20 sec
  //CPPUNIT_ASSERT(timeInSecD1 - timeInSecD == 86400); 
  //CPPUNIT_ASSERT(timeInSecD == 63416908800); 
  //CPPUNIT_ASSERT(timeInSec  == 63416966319); 
  //CPPUNIT_ASSERT(timeInSec1 - timeInSec == 20);

//#endif
}

void TsDateTime::tcGetTimeInSecondsDT()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date dd(2010,8,9,15,58,39);   //(2010,8,9,15,58,39)
  //long long int dd_timeInSecs = dd.getTimeInSeconds(dd);
  //CPPUNIT_ASSERT (dd_timeInSecs == 63416966319);

//#endif
}

void TsDateTime::tcGetDiffInSeconds()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date dd(2010,8,9,15,58,39);   //(2010,8,9,15,58,39)
  //te::dt::Date d;
  //long long int dif = d.getDiffInSeconds(dd);
  //long long int dif1 = dd.getDiffInSeconds(d);

  //te::dt::Date df(2011,8,9,15,58,39);   //(2010,8,9,15,58,39)
  //long long int diff = df.getDiffInSeconds(dd);
  //long long int ts1 = dd.getTimeInSeconds();
  //long long int ts2 = df.getTimeInSeconds();
  //long long int diff1 = ts2 - ts1 ; //should be equal diff
  //long long int diff_date = df - dd ; //should be equal diff

  //CPPUNIT_ASSERT ( diff == diff1);
  //CPPUNIT_ASSERT ( diff_date == diff);


//#endif
}

void TsDateTime::tcGetDiffInSeconds1()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(2010,8,9 ,15,58,39);
  //te::dt::Date d2(2010,8,10,15,58,39);
  //te::dt::Date d; 
  //long long int dif = te::dt::Date::getDiffInSeconds(d2,d1);
  //CPPUNIT_ASSERT ( dif == 86400); // 1 dia = 24*60*60 = 86400
  //d2+= 172800; //(+2 dias)
  //dif = te::dt::Date::getDiffInSeconds(d2,d1); // = 3 dias=259200sec
  //CPPUNIT_ASSERT ( dif == 259200);


//#endif
}

void TsDateTime::tcGetWeek()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d1(2010,8,9,15,58,39);   //monday
  //te::dt::Date d2(2011,8,9,15,58,39);   //tuesday
  //te::dt::Date d3(2010,2,31,15,58,39);   //???invalid day

  //int idx1, idx2, idx3;
  //idx1 = d1.getWeekDay();
  //idx2 = d2.getWeekDay();
  //idx3 = d3.getWeekDay();
  //CPPUNIT_ASSERT ( idx1 == 1 && idx2 == 2);

//#endif
}

void TsDateTime::tcGetWeekName()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d;;

  //te::dt::Date d1(2010,8,9,15,58,39);
  //te::dt::Date d2(2011,8,9,15,58,39);

  //std::string weekName1 = d1.getWeekDayName();
  //std::string weekName2 = d2.getWeekDayName();
  //CPPUNIT_ASSERT ( weekName1 == "monday" && weekName2 == "tuesday");


//#endif
}

void TsDateTime::tcGetDate()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d2(2010,8,9,5,5,5);
  //std::string dateF = d2.getDate();
  //std::string format_F1 = "2010-08-09" ;
  //CPPUNIT_ASSERT(dateF == format_F1);

  //te::dt::Date d1(2010,8,9,15,58,39);
  //std::string dateF1 = d1.getDate();
  //std::string separator0 = "09/08/2010" ;
  //std::string separator1 = "09-08-2010" ;
  //std::string separator2 = "09.08.2010" ;

//#endif
}

void TsDateTime::tcGetDateTime()
{
//#ifdef TE_COMPILE_ALL
  //te::dt::Date d0(2010,8,9,15,58,39);
  //te::dt::Date d1(2010,8,9,15,58,39,-3,0);
  //te::dt::Date d2(2010,8,9,5,5,5,5,30);

  //std::string dateF0 = d0.getDateTime();
  //std::string format_F0 = "2010-08-09 15:58:39Z" ;
  //CPPUNIT_ASSERT(dateF0 == format_F0);

  //std::string dateF1 = d1.getDateTime();
  //std::string format_F1 = "2010-08-09 15:58:39-3:00" ;
  //CPPUNIT_ASSERT(dateF1 == format_F1);

  //std::string dateF2 = d2.getDateTime(te::dt::Date::DateFormat::YYYY_MM);
  //std::string format_F2 = "2010-08 05:05:05+05:30" ;
  //CPPUNIT_ASSERT(dateF2 == format_F2);

  //std::string dateF3 = d2.getDateTime(te::dt::Date::DateFormat::YYYYMMDD);
  //std::string format_F3 = "20100809 05:05:05+00:00" ;

  //std::string dateF4 = d2.getDateTime(te::dt::Date::DateFormat::YYYY_MM_DD,te::dt::Date::Separator::SLASH,te::dt::Date::TimeFormat::HH_MM_SS);
  //std::string format_F4 = "2010/08/09 05:05:05" ;
  //CPPUNIT_ASSERT(dateF4 == format_F4);

  //std::string dateF5 = d2.getDateTime(te::dt::Date::DateFormat::YYYY_MM_DD,te::dt::Date::Separator::SLASH,te::dt::Date::TimeFormat::HH_MM_SS_TZ,te::dt::Date::Separator::DOT);
  //std::string format_F5 = "2010/08/09 05.05.05+00.00" ;
  //CPPUNIT_ASSERT(dateF5 == format_F5);


  //std::string format0 = "09/08/2010 15:58:39" ;
  //std::string format1 = "09/august/2010 15:58:39" ;
  //std::string format2 = "08/09/2010 15:58:39" ;
  //std::string format3 = "august/09/2010 15:58:39" ;
  //std::string format4 = "2010/august/09 15:58:39" ;
  //std::string format5 = "2010/august/09 15:58:39" ;
  //std::string format6 = "monday 09/2010 15:58:39" ;
  //std::string format7 = "monday 09/august/2010 15:58:39" ;
  //std::string format8 = "monday 09/2010 15:58:39" ;
  //std::string format9 = "monday august/09/2010 15:58:39" ;
  //std::string format10 ="monday 2010/08/09 15:58:39" ;
  //std::string format11 = "monday 2010/august/09 15:58:39" ;
  
//#endif
}

void TsDateTime::tcGetTime()
{
//#ifdef TE_COMPILE_ALL

  //te::dt::Date d2(2010,8,9,5,5,5);

  //std::string tt = d2.getTime();
  //std::string format_t1 = "05:05:05Z" ;
  //CPPUNIT_ASSERT(tt == format_t1);

//#endif
}
