/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

// Terralib
#include <terralib/datatype/Exception.h>

// Unit-Test TerraLib
#include "TsDate.h"


static boost::gregorian::date d(2010,8,9);
static boost::gregorian::date dg(2010,12,31);
static te::dt::Date ds(2010,01,01);  //date start
static te::dt::Date d28(2010,02,28); //feb 28
static te::dt::Date d30(2010,04,30); //abr
static te::dt::Date d31(2010,05,31); //mai
static te::dt::Date de(2010,12,31); //date end
static te::dt::Date di(2010,06,10); //date intermediary
static te::dt::Date dmar(2010,03,03);
static te::dt::Date d29(2008,02,29);

static te::dt::Date dx(2010,12,31);

static te::dt::TimeDuration dt(20,30,50,11);

CPPUNIT_TEST_SUITE_REGISTRATION( TsDate );

void TsDate::setUp()
{
}

void TsDate::tearDown()
{
}

void TsDate::tcDate()
{
//#ifdef TE_COMPILE_ALL
  te::dt::Date dempty;
  te::dt::Date d1(d);  //from gregorian date
  te::dt::Date d2(dx); //from Date
  std::cout << std::endl;
  std::cout << " Empty constructor: " << dempty.toString() << std::endl;
  std::cout << " Date from gregorian date : " << d1.toString() << std::endl;
  std::cout << " Date from date: " << d2.toString() << std::endl;

  CPPUNIT_ASSERT(d2.getYear() == 2010 && d2.getMonth() == 12 && d2.getDay() == 31 );


//#endif
}

void TsDate::tcDateCheck()
{
//#ifdef TE_COMPILE_ALL
  te::dt::Date d1(2010,1,1);
  CPPUNIT_ASSERT(d1.getYear() == 2010 && d1.getMonth() == 1 && d1.getDay() == 1 );
  
  CPPUNIT_ASSERT(ds.getYear() == 2010 && ds.getMonth() == 1 && ds.getDay() == 1 );
  CPPUNIT_ASSERT(de.getYear() == 2010 && de.getMonth() == 12 && de.getDay() == 31 );
  CPPUNIT_ASSERT(d28.getYear() == 2010 && d28.getMonth() == 02 && d28.getDay() == 28 );
  CPPUNIT_ASSERT(d29.getYear() == 2008 && d29.getMonth() == 02 && d29.getDay() == 29 );
  CPPUNIT_ASSERT(d30.getYear() == 2010 && d30.getMonth() == 04 && d30.getDay() == 30 );
  CPPUNIT_ASSERT(d31.getYear() == 2010 && d31.getMonth() == 05 && d31.getDay() == 31 );

// Checking year range [1400..9999]
  te::dt::Date d2(1400,1,1);
  CPPUNIT_ASSERT(d2.getYear() == 1400 && d2.getMonth() == 1 && d2.getDay() == 1 );

  te::dt::Date d3(9999,8,31);
  CPPUNIT_ASSERT(d3.getYear() == 9999 && d3.getMonth() == 8 && d3.getDay() == 31 );
  std::cout << std::endl;
  std::cout << " Testing range of values Year: " << d2.toString() << std::endl;
  std::cout << " Testing range of values Year : " << d3.toString() << std::endl;
  
  //Is it accepting invalid year ?? TODO:  how to use try/catch exception da boost
  try {
    te::dt::Date d4(0,12,31);
    std::cout << " Date from date: " << d4.toString() << std::endl;
    CPPUNIT_ASSERT(d4.getYear() == 0);
  }
  catch (te::dt::Exception e)
  { 
    std::cout << " Year out of range d(0,12,31)" <<  std::endl;
    throw e ;
  }

//#endif
} 

void TsDate::tcDateConstructor()
{
//#ifdef TE_COMPILE_ALL

  te::dt::Date d1(2010,12,31);
  te::dt::Date d(d1);

  CPPUNIT_ASSERT( d1.getYear() == d.getYear() && d1.getMonth() == d.getMonth() && d1.getDay() == d.getDay());
 
//#endif
}

void TsDate::tcDateTimeDuration()
{
//#ifdef TE_COMPILE_ALL
  te::dt::Date dx(2010,12,31);
  te::dt::TimeDuration dt(20,30,50,11);

  te::dt::TimeInstant d1(dx,dt);
  te::dt::TimeInstant d2;
  te::dt::TimeInstant d3;
  d2.operator =(d1);
  d3 = d2;

  CPPUNIT_ASSERT( d1.getDate().getYear() == d2.getDate().getYear() && d1.getDate().getMonth() == d2.getDate().getMonth() && d1.getDate().getDay() == d2.getDate().getDay());
  CPPUNIT_ASSERT( d1.getTime().getHours() == d2.getTime().getHours() && d1.getTime().getMinutes() == d2.getTime().getMinutes() && d1.getTime().getSeconds() == d2.getTime().getSeconds());
  CPPUNIT_ASSERT( d2.getDate().getYear() == d3.getDate().getYear() && d2.getDate().getMonth() == d3.getDate().getMonth() && d2.getDate().getDay() == d3.getDate().getDay());
  CPPUNIT_ASSERT( d2.getTime().getHours() == d3.getTime().getHours() && d2.getTime().getMinutes() == d3.getTime().getMinutes() && d2.getTime().getSeconds() == d3.getTime().getSeconds());
  CPPUNIT_ASSERT( d3.getDate().getYear() == d1.getDate().getYear() && d3.getDate().getMonth() == d1.getDate().getMonth() && d3.getDate().getDay() == d1.getDate().getDay());
  CPPUNIT_ASSERT( d3.getTime().getHours() == d1.getTime().getHours() && d3.getTime().getMinutes() == d1.getTime().getMinutes() && d3.getTime().getSeconds() == d1.getTime().getSeconds());

//#endif
}

void TsDate::tcCheckOperators()
{
//#ifdef TE_COMPILE_ALL
// Test all operators...
  te::dt::Date d1(ds); 
  te::dt::Date d2(ds);
  te::dt::Date d3(2010,01,05);
  te::dt::AbstractData* d4 = ds.clone();

  CPPUNIT_ASSERT(d2.operator==(d1));

  CPPUNIT_ASSERT(static_cast<te::dt::Date*>(d4)->operator==(d1));
  CPPUNIT_ASSERT(d2.operator-(d1)  == 0);
  CPPUNIT_ASSERT(d3.operator-(ds)  == 4);
  CPPUNIT_ASSERT(de.operator-(ds)  == 364); 
  CPPUNIT_ASSERT(ds.operator<(de));
  CPPUNIT_ASSERT(de.operator>(ds));
  CPPUNIT_ASSERT(ds.operator!=(de)); 
  std::string ds_show = ds.toString();
  std::string d4_show = static_cast<te::dt::Date*>(d4)->toString();
  CPPUNIT_ASSERT(ds.operator==(*static_cast<te::dt::Date*>(d4)));
// Difference involving feb ( 20100101 - 20100303) 
  CPPUNIT_ASSERT(dmar.operator-(ds)  == 61); 

//#endif
}
void TsDate::tcClone()
{
//#ifdef TE_COMPILE_ALL

  te::dt::Date d(ds);
  te::dt::Date dclone =  *static_cast<te::dt::Date*>(d.clone());
  CPPUNIT_ASSERT(d.operator==(dclone));

//#endif
}

void TsDate::tcToString()
{
//#ifdef TE_COMPILE_ALL

  te::dt::Date d(ds);
  te::dt::Date dclone =  *static_cast<te::dt::Date*>(ds.clone());
  CPPUNIT_ASSERT(ds.operator==(dclone));
  std::string show = dclone.toString();
  CPPUNIT_ASSERT( dclone.toString() =="20100101");

//#endif
}

void TsDate::tcDateDuration()
{
//#ifdef TE_COMPILE_ALL

  te::dt::DateDuration dempty;
  std::string empty = dempty.toString();
  boost::gregorian::date_duration boostduration(5);
  te::dt::DateDuration dboost(boostduration);
  std::string sh_dboost  = dboost.toString();
  
  te::dt::DateDuration d(2);
  te::dt::DateDuration dclone =  *static_cast<te::dt::DateDuration*>(d.clone());
  CPPUNIT_ASSERT(d.operator==(dclone));
  std::string show = d.toString();
  te::dt::DateDuration d1(4);
  std::string show1 = d1.toString();

  te::dt::DateDuration d3 = d1;


  CPPUNIT_ASSERT(d3 == d1);
  CPPUNIT_ASSERT(d < d1);
  CPPUNIT_ASSERT(d1 > d);
  CPPUNIT_ASSERT(d != d1);
  CPPUNIT_ASSERT(d1.toString() == "4");

//#endif
}

void TsDate::tcDatePeriod()
{
//#ifdef TE_COMPILE_ALL

  te::dt::DatePeriod p1(ds,de); 
  te::dt::DatePeriod pboost(d,dg);
  te::dt::DatePeriod p2(pboost);
  te::dt::DatePeriod p3 = p1;
  te::dt::Date dsplus1(2010,01,02);
  te::dt::DatePeriod p1plus1(ds,dsplus1);
  te::dt::DatePeriod pclone =  *static_cast<te::dt::DatePeriod*>(pboost.clone());
  CPPUNIT_ASSERT(pboost.operator==(pclone));

  std::cout << std::endl;
  std::cout << " Testing DatePeriod class with:" << std::endl;
  std::cout << "    DatePeriod p1    : " << p1.toString() << std::endl;
  std::cout << "    DatePeriod pboost: " << pboost.toString() << std::endl;
  std::cout << "    DatePeriod p2    : " << p2.toString() << std::endl;
  std::cout << "    DatePeriod p1plus1:" << p1plus1.toString() << std::endl;
  std::cout << "    DatePeriod p3:     " << p3.toString() << std::endl;
  std::cout << "    DatePeriod pboost clone: " << pclone.toString() << std::endl;

  std::cout << std::endl;
  std::cout << "    DatePeriod p1 InitialDate: " << p1.getInitialDate().toString() << std::endl;
  std::cout << "    DatePeriod p1 FinalDate:   "   << p1.getFinalDate().toString() << std::endl;
  std::cout << "    DatePeriod: p1 > pboost:   " << p1.operator>(pboost) << std::endl;
  std::cout << "    DatePeriod: pboost < p1:   " << pboost.operator<(p1) << std::endl;
  std::cout << "    DatePeriod: p1 != pboost:  " << p1.operator!=(pboost) << std::endl;
  std::cout << "    DatePeriod: p1 == pboost:  " << p1.operator==(pboost) << std::endl;
  std::cout << "    DatePeriod: p1 == p3:      " << p1.operator==(p3) << std::endl;

// Messages...  
  boost::gregorian::date_period bdp = p3.getDatePeriod(); 
  CPPUNIT_ASSERT(p1.getInitialDate() == ds);
  CPPUNIT_ASSERT(p1.getFinalDate() == de);
  CPPUNIT_ASSERT(p3.getDatePeriod() == bdp);

  CPPUNIT_ASSERT(p1.operator>(pboost));
  CPPUNIT_ASSERT(p1 > pboost);
  CPPUNIT_ASSERT(pboost.operator<(p1));
  CPPUNIT_ASSERT(pboost < p1);
  CPPUNIT_ASSERT(p3.operator==(p1));
  CPPUNIT_ASSERT(p3 == p1);


//#endif
}