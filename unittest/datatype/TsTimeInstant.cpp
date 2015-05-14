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

// Unit-Test TerraLib
#include "TsTimeInstant.h"

//STL
#include <iostream>

static boost::gregorian::date d(2010,8,9);
static boost::posix_time::ptime bt;

static te::dt::Date ds(2010,01,01);    //date start
static te::dt::Date de(2010,12,31);    //date end
static te::dt::TimeDuration td(20,30,50,11);
static te::dt::TimeDuration td1(23,59,59);
static te::dt::TimeInstant TT(ds,td);

CPPUNIT_TEST_SUITE_REGISTRATION( TsTimeInstant );

void TsTimeInstant::setUp()
{
}

void TsTimeInstant::tearDown()
{
}

void TsTimeInstant::tcTimeInstant()
{
//#ifdef TE_COMPILE_ALL

  te::dt::TimeInstant tt;
  std::cout << std::endl;
  std::cout << " print tt: " << tt.toString() << std::endl;
  tt = TT;
  std::cout << " print tt after assign: " << tt.toString() << std::endl;
  CPPUNIT_ASSERT( tt.toString() =="20100101T203050.000011");

//#endif
}

void TsTimeInstant::tcTimeInstant2()
{
//#ifdef TE_COMPILE_ALL

  te::dt::TimeInstant tt(ds,td);
  CPPUNIT_ASSERT(tt.getDate().getYear() == 2010 && tt.getDate().getMonth() == 1 && tt.getDate().getDay() == 1 );
  CPPUNIT_ASSERT(tt.getTime().getHours() == 20 && tt.getTime().getMinutes() == 30 && tt.getTime().getSeconds() == 50  && tt.getTime().getFractionalSeconds() == 11) ;
 
//#endif
}

void TsTimeInstant::tcTimeInstant3()
{
//#ifdef TE_COMPILE_ALL

  te::dt::TimeInstant tt(ds,td);
  CPPUNIT_ASSERT(tt.getDate().getYear() == 2010 && tt.getDate().getMonth() == 1 && tt.getDate().getDay() == 1 );
  CPPUNIT_ASSERT(tt.getTime().getHours() == 20 && tt.getTime().getMinutes() == 30 && tt.getTime().getSeconds() == 50  && tt.getTime().getFractionalSeconds() == 11) ;
  
  te::dt::TimeInstant bt(tt.getTimeInstant()); 
  CPPUNIT_ASSERT(bt.getDate().getYear() == 2010 && bt.getDate().getMonth() == 1 && bt.getDate().getDay() == 1 );
  CPPUNIT_ASSERT(bt.getTime().getHours() == 20 && bt.getTime().getMinutes() == 30 && bt.getTime().getSeconds() == 50  && bt.getTime().getFractionalSeconds() == 11) ;
 
//#endif
}

void TsTimeInstant::tcGetTimeInstant()
{
//#ifdef TE_COMPILE_ALL

  boost::posix_time::ptime b = TT.getTimeInstant();
  te::dt::TimeInstant tt(ds,td);

  CPPUNIT_ASSERT(tt.getDate().getYear() == 2010 && tt.getDate().getMonth() == 1 && tt.getDate().getDay() == 1 );
  CPPUNIT_ASSERT(tt.getTime().getHours() == 20 && tt.getTime().getMinutes() == 30 && tt.getTime().getSeconds() == 50  && tt.getTime().getFractionalSeconds() == 11) ;
 
//#endif
}

void TsTimeInstant::tcGetDate()
{
//#ifdef TE_COMPILE_ALL

  te::dt::TimeInstant tt(ds,td);
  CPPUNIT_ASSERT(tt.getDate().getYear() == 2010 && tt.getDate().getMonth() == 1 && tt.getDate().getDay() == 1 );

  //#endif
}

void TsTimeInstant::tcGetTime()
{
//#ifdef TE_COMPILE_ALL

  te::dt::TimeInstant tt(ds,td);
  CPPUNIT_ASSERT(tt.getTime().getHours() == 20 && tt.getTime().getMinutes() == 30 && tt.getTime().getSeconds() == 50  && tt.getTime().getFractionalSeconds() == 11) ;
  te::dt::TimeInstant tt1(ds,td1);
  CPPUNIT_ASSERT(tt1.getTime().getHours() == 23 && tt1.getTime().getMinutes() == 59 && tt1.getTime().getSeconds() == 59  && tt1.getTime().getFractionalSeconds() == 0) ;

  //#endif
}

void TsTimeInstant::tcCheckOperators()
{
//#ifdef TE_COMPILE_ALL
// Test all operators...

  te::dt::TimeInstant tt(ds,td);
  te::dt::TimeInstant tt1;
  tt1 = tt;

  te::dt::TimeInstant tt2(de,td);
  te::dt::TimeDuration m59(20,30,59,11);
  te::dt::TimeInstant tt3(de,m59); //plus 9 seconds in tt2

  std::cout << std::endl;
  std::cout << " tt1: " << tt1.toString() << std::endl;
  std::cout << " tt2: " << tt2.toString() << std::endl;
  std::cout << " tt3: " << tt3.toString() << std::endl;

  CPPUNIT_ASSERT(tt1.operator==(tt));
  long res; 
  res = tt2 - tt1;
  CPPUNIT_ASSERT(tt2.operator-(tt1)  != 0);

  CPPUNIT_ASSERT(tt3.operator-(tt2)  == 9);

  CPPUNIT_ASSERT(tt1 < tt2);
  CPPUNIT_ASSERT(tt1.operator<(tt2));

  CPPUNIT_ASSERT(tt2 > tt1);
  CPPUNIT_ASSERT(tt2.operator>(tt1));

  CPPUNIT_ASSERT(tt2 != tt1);
  CPPUNIT_ASSERT(tt2.operator!=(tt1)); 

  CPPUNIT_ASSERT( tt1.toString() =="20100101T203050.000011");
  CPPUNIT_ASSERT( tt2.toString() =="20101231T203050.000011");
  CPPUNIT_ASSERT( tt3.toString() =="20101231T203059.000011");

//#endif
}

void TsTimeInstant::tcClone()
{
//#ifdef TE_COMPILE_ALL

  te::dt::TimeInstant tt;
  tt = TT;
  te::dt::TimeInstant tclone =  *static_cast<te::dt::TimeInstant*>(tt.clone());
  CPPUNIT_ASSERT(tt.operator==(tclone));

//#endif
}

void TsTimeInstant::tcToString()
{
//#ifdef TE_COMPILE_ALL

  te::dt::TimeInstant tt;
  tt = TT;
  te::dt::TimeInstant tclone =  *static_cast<te::dt::TimeInstant*>(tt.clone());
  CPPUNIT_ASSERT(tt.operator==(tclone));
  std::string show = tclone.toString();
  CPPUNIT_ASSERT( tclone.toString() =="20100101T203050.000011");

//#endif
}