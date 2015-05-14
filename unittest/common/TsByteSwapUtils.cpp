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
#include "TsByteSwapUtils.h"

#include <terralib/common.h>
#include <terralib/common/ByteSwapUtils.h>

// STL
#include <cstdlib>
#include <string>
#include <vector>

// Boost
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// http://www.boost.org/doc/libs/1_43_0/libs/integer/test/cstdint_test.cpp

/*
Int16: 2 bytes
Int32 and int: 4 bytes
Int64 : 8 bytes

Type Capacity
Min/Max Value
Int 16 -32,768 / 32,767
Int 32 -2,147,483,648 / 2,147,483,647
Int 64 -9,223,372,036,854,775,808 / 9,223,372,036,854,775,807 

*/

CPPUNIT_TEST_SUITE_REGISTRATION( TsByteSwapUtils );

void TsByteSwapUtils::setUp()
{
}

void TsByteSwapUtils::tearDown()
{
}

void TsByteSwapUtils::tcSwap2Bytes()
{
  char input_letter[] = "AB";
  te::common::Swap2Bytes(input_letter);
  std::cout << "Input AB " << input_letter  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_letter, "BA", 2 ) == 0);

  char input_num[] = "12";
  te::common::Swap2Bytes(input_num);
  std::cout << "Input 12" << input_num  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_num, "21", 2 ) == 0);

  char input_symb[] = "!@";
  te::common::Swap2Bytes(input_symb);
  std::cout << "Input !@ " << input_symb  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_symb, "@!", 2 ) == 0);

}

void TsByteSwapUtils::tcSwap4Bytes()
{
  char input_letter[] = "ABCD";
  te::common::Swap4Bytes(input_letter);
  std::cout << "Input ABCD=" << input_letter  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_letter, "DCBA", 4 ) == 0);

  char input_num[] = "1234";
  te::common::Swap4Bytes(input_num);
  std::cout << "Input 1234=" << input_num  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_num, "4321", 4 ) == 0);

  char input_symb[] = "!@#$";
  te::common::Swap4Bytes(input_symb);
  std::cout << "Input !@#$= " << input_symb  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_symb, "$#@!", 4 ) == 0);
}

void TsByteSwapUtils::tcSwapBytes()
{
  char input_letter[] = "ABCDabcdefghij";
  int len = strlen(input_letter);
  te::common::SwapBytes(input_letter);
  std::cout << " Input ABCDabcdefghij " << input_letter  << std::endl;
  CPPUNIT_ASSERT_MESSAGE("SwapBytes of n bytes (ABCDabcdefghij) is not working!", strncmp(input_letter, "jihgfedcbaDCBA", len ) == 0);

  char input_num[] = "12345678";
  te::common::SwapBytes(input_num);
  std::cout << " Input 12345678 " << input_num  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_num, "87654321", 8 ) == 0);

  char input_symb[] = "!@#$%&*(";
  te::common::SwapBytes(input_symb);
  std::cout << " Input !@#$%&*( " << input_symb  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_symb, "(*&%$#@!", 8 ) == 0);
}

void TsByteSwapUtils::tcSwap8Bytes()
{
  char input_letter[] = "ABCDabcd";
  te::common::Swap8Bytes(input_letter);
  std::cout << " Input ABCDabcd " << input_letter  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_letter, "dcbaDCBA", 8 ) == 0);

  char input_num[] = "12345678";
  te::common::Swap8Bytes(input_num);
  std::cout << " Input 12345678 " << input_num  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_num, "87654321", 8 ) == 0);

  char input_symb[] = "!@#$%&*(";
  te::common::Swap8Bytes(input_symb);
  std::cout << " Input !@#$%&*( " << input_symb  << std::endl;
  CPPUNIT_ASSERT(strncmp(input_symb, "(*&%$#@!", 8 ) == 0);
}

void TsByteSwapUtils::tcSwapBytes_bint16()
{
  //boost::int16_t     int16   = INT16_C(-32767);
  //Int 16 -32768 to  32767
  boost::int16_t i16min = -32768;
  boost::int16_t i16min_swap2 = -32768;
  boost::int16_t i16max =  32767;
  boost::int16_t i16max_swap2 =  32767;
  boost::int16_t i16med =  32767/2;
  boost::int16_t i16med_swap2 =  32767/2;
  int ss; 
  
  //Swap bytes 2x and the result must be equal the input.
  std::cout << " Input  min  " << i16min  << std::endl;
  ss = sizeof(i16min);
  te::common::SwapBytes(i16min);
  std::cout << " Input  swap " << i16min  << std::endl;
  te::common::SwapBytes(i16min);
  std::cout << " Swap again  " << i16min  << std::endl;

  CPPUNIT_ASSERT(i16min == i16min_swap2);

  std::cout << " Input  max  " << i16max  << std::endl;
  ss = sizeof(i16max);
  te::common::SwapBytes(i16max);
  std::cout << " Input  swap " << i16max  << std::endl;
  te::common::SwapBytes(i16max);
  std::cout << " Swap again  " << i16max  << std::endl;
  CPPUNIT_ASSERT(i16max == i16max_swap2);

  std::cout << " Input  med  " << i16med  << std::endl;
  ss = sizeof(i16med);
  te::common::SwapBytes(i16med);
  std::cout << " Input  swap " << i16med  << std::endl;
  te::common::SwapBytes(i16med);
  std::cout << "Swap again  " << i16med  << std::endl;
  CPPUNIT_ASSERT(i16med == i16med_swap2);
}

void TsByteSwapUtils::tcSwapBytes_bint32()
{
  //boost::int32_t         int32         = INT32_C(-2147483647);
  //Int 32 -2,147,483,648 to 2,147,483,647
  boost::int32_t i32min = -2147483647; //-2147483648; 
  //if -2147483648 is used ...warning C4146: unary minus operator applied to unsigned type, result still unsigned
  boost::int32_t i32min_swap2 = -2147483647;  //-2147483648; 
  boost::int32_t i32max =  2147483683;
  boost::int32_t i32max_swap2 =  2147483683;
  boost::int32_t i32med =  2147483683/2;
  boost::int32_t i32med_swap2 =  2147483683/2;
  
  //Swap bytes 2x and the result must be equal the input.
  CPPUNIT_ASSERT(sizeof(i32min) == 4);

  te::common::SwapBytes(i32min);
  te::common::SwapBytes(i32min);
  CPPUNIT_ASSERT(i32min == i32min_swap2);

  te::common::SwapBytes(i32max);
  te::common::SwapBytes(i32max);
  CPPUNIT_ASSERT(i32max == i32max_swap2);

  te::common::SwapBytes(i32med);
  te::common::SwapBytes(i32med);
  CPPUNIT_ASSERT(i32med == i32med_swap2);
}

void TsByteSwapUtils::tcSwapBytes_buint32()
{
  // boost::uint32_t        uint32  = UINT32_C(4294967295);
  // uint32 type 0 to 4294967295 
  boost::uint32_t ui32max =  4294967295;
  boost::uint32_t ui32max_swap2 =  4294967295;
  boost::uint32_t ui32med =  4294967295/2;
  boost::uint32_t ui32med_swap2 =  4294967295/2;
  
  //Swap bytes 2x and the result must be equal the input.
  CPPUNIT_ASSERT(sizeof(ui32max) == 4);

  te::common::SwapBytes(ui32max);
  te::common::SwapBytes(ui32max);
  CPPUNIT_ASSERT(ui32max == ui32max_swap2);

  te::common::SwapBytes(ui32med);
  te::common::SwapBytes(ui32med);
  CPPUNIT_ASSERT(ui32med == ui32med_swap2);
}

void TsByteSwapUtils::tcSwapBytes_float()
{
  // http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm

  float fl = 10.25f ;
  float fl_swap2 = 10.25f ;
  te::common::SwapBytes(fl);
  te::common::SwapBytes(fl);
  CPPUNIT_ASSERT(fl == fl_swap2);

  float fl1 = 4.2038954e-045f ;
  float fl1_swap2 = 4.2038954e-045f;
  te::common::SwapBytes(fl1);
  te::common::SwapBytes(fl1);
  CPPUNIT_ASSERT(fl1 == fl1_swap2);

  float fl2 = 1.99999976f ;
  float fl2_swap2 = 1.99999976f;
  te::common::SwapBytes(fl2);
  te::common::SwapBytes(fl2);
  CPPUNIT_ASSERT(fl2 == fl2_swap2);

  float fl3 = +1.4012985e-045f ;
  float fl3_swap2 = +1.4012985e-045f;
  te::common::SwapBytes(fl3);
  te::common::SwapBytes(fl3);
  CPPUNIT_ASSERT(fl3 == fl3_swap2);

  float fl5 = (float)0xFFFFFFFD ;
  float fl5_swap2 = (float)0xFFFFFFFD ;
  te::common::SwapBytes(fl5);
  te::common::SwapBytes(fl5);
  CPPUNIT_ASSERT(fl5 == fl5_swap2);

  float fl6 = (float)(-3) ;
  float fl6_swap2 = (float)(-3) ;
  te::common::SwapBytes(fl6);
  te::common::SwapBytes(fl6);
  CPPUNIT_ASSERT(fl6 == fl6_swap2);

/*
http://processing.org/reference/float.html
Data type for floating-point numbers, a number that has a decimal point.
Floats are not precise, so avoid adding small values (such as 0.0001) 
may not always increment because of rounding error. 
If you want to increment a value in small intervals, use an int, 
and divide by a float value before using it. (See above example.)

Floating-point numbers can be as large as 3.40282347E+38 and as 
low as -3.40282347E+38. They are stored as 32 bits (4 bytes) of 
information. The float data type is inherited from Java.

float f = 0;
for (int i = 0 ; i < 100000; i++) {  
  f = f + 0.0001;  // Bad idea! See below.
}

for (int i = 0; i < 100000; i++) {
  // The variable 'f' will work better here, less affected by rounding
  float f = i / 1000.0;  // Count by thousandths
}

*/
  float f; float f_orig;
  for (int i = 1000000000; i > 0; i--)
  {
    //The variable 'f' will work better here, less affected by rounding
    f = (float)(i / 100000.0);
    f_orig = (float)(i / 100000.0 );
    te::common::SwapBytes(f);
    te::common::SwapBytes(f);
    CPPUNIT_ASSERT(f == f_orig);
    i-=5000;
  }
}

void TsByteSwapUtils::tcSwapBytes_double()
{
  //http://stackoverflow.com/questions/4738768/printing-double-without-losing-precision?rq=1
  double d = 3.1415926535897932384626433832795 ; // 3.1415926535897931;
  double dorig = 3.1415926535897932384626433832795 ; // 3.1415926535897931;
  CPPUNIT_ASSERT(sizeof(d) == 8)   ;

  std::cout.precision(52); //16 ok
  //std::cout << "Pi: " << std::cout.fixed << d <<std::endl;
  //std::cout << "Pi: " << d <<std::endl;
  te::common::SwapBytes(d);
  te::common::SwapBytes(d);
  CPPUNIT_ASSERT(d == dorig) ;

  //2^-52 + 300300300300
  double d_52 = 300300300300 +0.0000000000000002220446049250313080847263336181640625  ;
  double dorig_52 = 300300300300 + 0.0000000000000002220446049250313080847263336181640625  ;
  te::common::SwapBytes(d_52);
  te::common::SwapBytes(d_52);
  CPPUNIT_ASSERT(d_52 == dorig_52) ;
  //std::cout << "d_52: " << d_52 << std::endl;
 
  double p;// 2^i 
  double p_orig; 
  double base = 2;
  for (int i = 52; i > 0; i--)
  {
    p = pow(base,-i) ; 
    p_orig = pow(base,-i) ;
    //std::cout << "Pi: " << boost::lexical_cast<std::string>(p) << std::endl;
    //std::cout << "double 2^-" << i <<  "                   = " << std::cout.fixed << p << std::endl;
    te::common::SwapBytes(p);
    te::common::SwapBytes(p);
    //std::cout << "double 2^-" << i << "swaped twice" << " = "  << std::cout.fixed << p <<std::endl;
    CPPUNIT_ASSERT(p == p_orig);
  }
}
