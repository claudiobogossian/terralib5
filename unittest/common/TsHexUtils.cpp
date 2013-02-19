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
#include "TsHexUtils.h"

#include <terralib/common.h>
#include <terralib/common/HexUtils.h>

// STL
#include <cstdlib>
#include <string>
#include <vector>

// Boost
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsHexUtils );

static std::vector<size_t> dec_val;
static std::vector<unsigned char> dec_val_uchar;
size_t dec_invalid;
unsigned char dec_invalid_uc;
static std::vector<unsigned char> hex_lcase;
static std::vector<unsigned char> hex_ucase;
static std::vector<unsigned char> hex_invalid_0_F;
static std::vector<unsigned char> hex_upper;
static std::vector<unsigned char> hex_lower;

void TsHexUtils::setUp()
{
  // Reading  input data (hex values) from a json file and output values (decimal values 0-15)
  boost::property_tree::ptree test_data;
  boost::property_tree::read_json("../data/module_common/hexutils/hexutils_inp_out.json", test_data);
  te::common::Convert(test_data.get_child("input.hex_Lcase"), hex_lcase);
  te::common::Convert(test_data.get_child("input.hex_Ucase"), hex_ucase);
  te::common::Convert(test_data.get_child("input.hex_A_F"), hex_upper);
  te::common::Convert(test_data.get_child("input.hex_a_f"), hex_lower);
  te::common::Convert(test_data.get_child("input.hex_outof_0_9_A_F"), hex_invalid_0_F);
  te::common::Convert(test_data.get_child("outp.dec_val"), dec_val);
  //te::common::Convert(test_data.get_child("outp.dec_val"), dec_val_uchar); //boost error
  dec_invalid =boost::lexical_cast<size_t>(test_data.get_child("outp.dec_val_invalid").data());
  //dec_invalid_uc =boost::lexical_cast<unsigned char>(test_data.get_child("outp.dec_val_invalid").data());//boost error

  // Boost can not read expected output data (unsigned char) from json file using lexical_cast
  // Then, the decimal values expected are defined below and will be checked against the real returned values.
  dec_val_uchar.push_back(0);
  dec_val_uchar.push_back(1);
  dec_val_uchar.push_back(2);
  dec_val_uchar.push_back(3);
  dec_val_uchar.push_back(4);
  dec_val_uchar.push_back(5);
  dec_val_uchar.push_back(6);
  dec_val_uchar.push_back(7);
  dec_val_uchar.push_back(8);
  dec_val_uchar.push_back(9);
  dec_val_uchar.push_back(10);
  dec_val_uchar.push_back(11);
  dec_val_uchar.push_back(12);
  dec_val_uchar.push_back(13);
  dec_val_uchar.push_back(14);
  dec_val_uchar.push_back(15);
 }

void TsHexUtils::tearDown()
{
  dec_val.clear();
  hex_lcase.clear();  
  hex_ucase.clear();  
  hex_invalid_0_F.clear();  
  hex_upper.clear();  
  hex_lower.clear();
  dec_val_uchar.clear();
}

void TsHexUtils::tcGetDecimalFromHexNotCS()
{
  // hex_value: [0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F]
  // hex_value: [0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f]
   
  // dec-value: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
 
  // initial tests ...
  char input1_hex = 'A';   // 10
  char input2_hex = 'a';   // 10
  char input3_hex = '1';   // 1
  char input4_hex = 'f';   // 15
  char input5_hex = 'I';   // 15  //invalid input return 15. How to detect if the input value is f/F or other invalid values?
   
  CPPUNIT_ASSERT( te::common::GetDecimalFromHexNotCS(input1_hex) == 10);
  CPPUNIT_ASSERT( te::common::GetDecimalFromHexNotCS(input2_hex) == 10);
  CPPUNIT_ASSERT( te::common::GetDecimalFromHexNotCS(input3_hex) == 1);
  CPPUNIT_ASSERT( te::common::GetDecimalFromHexNotCS(input4_hex) == 15);
  CPPUNIT_ASSERT( te::common::GetDecimalFromHexNotCS(input5_hex) == 15);

  // calling function over input data from json file and checking the output against the expected output (decimal: dec_val or dec_val_uchar)
  std::vector<unsigned char>::iterator it = hex_lcase.begin();
  std::vector<unsigned char>::iterator it_output_uc = dec_val_uchar.begin();
  std::vector<size_t>::iterator it_output = dec_val.begin();
  
  std::cout << std::endl;
  while ( it < hex_lcase.end() &&  it_output < dec_val.end() && it_output_uc < dec_val_uchar.end())
  {
    std::cout << "Input unsigned char: " << *it <<" GetDecimalFromHexUCase: " << te::common::GetDecimalFromHexNotCS(*it) << std::endl;
    std::cout << "Input unsigned char: " << *it <<" Output size_t: " << *it_output  <<" Output uchar: " << *it_output_uc <<  std::endl;
    unsigned char res_uc = te::common::GetDecimalFromHexNotCS(*it);
    CPPUNIT_ASSERT( te::common::GetDecimalFromHexNotCS(*it) == *it_output);
    CPPUNIT_ASSERT( te::common::GetDecimalFromHexNotCS(*it) == *it_output_uc);
    CPPUNIT_ASSERT( res_uc == *it_output_uc);

    it++ ;
    it_output++ ;
    it_output_uc++;
  }

  // testing invalid values as input (out of the range 0-9, A-F)
  it = hex_invalid_0_F.begin();

  while ( it < hex_invalid_0_F.end() )
  {
    std::cout << "Invalid input =>Input: " << *it << std::endl;
    CPPUNIT_ASSERT( te::common::GetDecimalFromHexNotCS(*it) == dec_invalid);
    it++ ;
  }
}

void TsHexUtils::tcGetDecimalFromHexUCase()
{
  // calling function over input data and checking the output
  std::vector<unsigned char>::iterator it = hex_ucase.begin();
  std::vector<size_t>::iterator it_output = dec_val.begin();
  std::cout << std::endl;

  while ( it < hex_ucase.end() &&  it_output < dec_val.end() )
  {
    std::cout << "Input unsigned char: " << *it <<" GetDecimalFromHexUCase: " << te::common::GetDecimalFromHexUCase(*it)  << std::endl;
    std::cout << "Input unsigned char: " << *it <<" Output size_t: " << *it_output  << std::endl;
    unsigned char ret_value = te::common::GetDecimalFromHexUCase(*it);
    CPPUNIT_ASSERT(ret_value == *it_output);
    CPPUNIT_ASSERT( te::common::GetDecimalFromHexUCase(*it) == *it_output);
    it++ ;
    it_output++ ;
  }

  // invalid input values should return always dec_invalid = 15
  it= hex_lower.begin();
  while ( it < hex_lower.end() )
  {
    std::cout << "Input Invlaid: " << *it <<" Output: " << te::common::GetDecimalFromHexUCase(*it) <<std::endl;
    unsigned char ret_value = te::common::GetDecimalFromHexUCase(*it);
    CPPUNIT_ASSERT(ret_value == 15);
    CPPUNIT_ASSERT( te::common::GetDecimalFromHexUCase(*it) == dec_invalid);
    it++ ;
  }
}

void TsHexUtils::tcGetDecimalFromHexLCase()
{
  // calling function over input data and checking the output
  std::vector<unsigned char>::iterator it = hex_lcase.begin();
  std::vector<size_t>::iterator it_output = dec_val.begin();
  std::cout << std::endl;

  while ( it < hex_lcase.end() &&  it_output < dec_val.end() )
  {
    std::cout << "std_::string =>Input: " << *it <<" Output: " << *it_output  << std::endl;
    std::cout << "Input: " << *it <<" Output: " << *it_output  << std::endl;
    CPPUNIT_ASSERT( te::common::GetDecimalFromHexLCase(*it) == *it_output);
    it++ ;
    it_output++ ;
  }
  // invalid input values
  it= hex_upper.begin();
  while ( it < hex_upper.end() )
  {
    std::cout << "Input Invalid: " << *it <<" Output: " << te::common::GetDecimalFromHexLCase(*it) <<std::endl;
    unsigned char ret_value = te::common::GetDecimalFromHexLCase(*it);
    CPPUNIT_ASSERT(ret_value == 15);
    CPPUNIT_ASSERT( te::common::GetDecimalFromHexLCase(*it) == dec_invalid);
    it++ ;
  }
}

void TsHexUtils::tcChar2Hex()
{
  // Reading from json file into ascii_tab property tree and converting to ascii_json vector
  std::vector<std::pair<std::string, std::string>>  ascii_json;
  std::vector<std::pair<std::string, std::string>>::iterator itpair; 
  boost::property_tree::ptree ascii_tab;
  boost::property_tree::read_json("../data/module_common/hexutils/ascii_tab.json", ascii_tab);
  te::common::Convert(ascii_tab.get_child("ascii_t"), ascii_json);
  std::cout << std::endl;

  for(itpair = ascii_json.begin(); itpair < ascii_json.end(); itpair++)
  {
    std::cout << "First: " << (*itpair).first << " Second: " << (*itpair).second  << std::endl;
    char *res = new char[];
    //char* rt = new char[2];
    //rt = "41";
    char c1 = *(((*itpair).first).c_str());
    unsigned char c = *(((*itpair).first).c_str());    
    te::common::Char2Hex(c1,res);
    std::cout << " Output res: " << res << std::endl;
    res[2] = '\0';
    std::cout << " Input: " << ((*itpair).first).c_str() << " Output Char2Hex : " << res  << std::endl;
    const char* itout = ((*itpair).second).c_str();
    //char c_itout = *(((*itpair).first).c_str());
    CPPUNIT_ASSERT(*res == *itout);
  }
}

void TsHexUtils::tcHex2Char()
{
  //Initial tests ...
  char hex1[]= "41";  //character 'A' in hex
  char res = te::common::Hex2Char(hex1);

  std::cout << std::endl;
  std::cout << " Input: " << hex1 << " Output res: " << res  << std::endl;

  char hex2[] = "39";  //character '9' in hex 
  res = te::common::Hex2Char(hex2);
  std::cout << " Input: " << hex2 << " Output res2: " << res  << std::endl;

  char hex3[] = "61";  //character 'a' in hex 
  res = te::common::Hex2Char(hex3);
  std::cout << " Input: " << hex3 << " Output res: " << res  << std::endl;

  char hex4[] = "4a";  //character 'J' in hex  "4a" retorna ´j´
  res = te::common::Hex2Char(hex4);
  std::cout << " Input J " << hex4 << " Output res: " << res  << std::endl;

  char hex5[] = "46";  //character 'F'  in hex 
  res = te::common::Hex2Char(hex5);
  std::cout << " Input: " << hex5 << " Output res: " << res  << std::endl;

  char hexG[] = "47";  //character 'G'  in hex 
  res = te::common::Hex2Char(hexG);
  std::cout << " Input: " << hexG << " Output res: " << res  << std::endl;

  char hexa[] = "61";  //character 'a'  in hex 
  res = te::common::Hex2Char(hexa);
  std::cout << " Input: " << hexa << " Output res: " << res  << std::endl;

  char hexf[] = "66";  //character 'f'  in hex 
  res = te::common::Hex2Char(hexf);
  std::cout << " Input: " << hexf << " Output res: " << res  << std::endl;

  char hexg[] = "67";  //character 'g'  in hex 
  res = te::common::Hex2Char(hexg);
  std::cout << " Input: " << hexg << " Output res: " << res  << std::endl;

  char hexZ[] = "5A";  //character 'Z'  in hex  
  res = te::common::Hex2Char(hexZ);
  std::cout << " Input Z: " << hexZ << " Output res: " << res  << std::endl;

  char hexZ_lc[] = "5a";  //character 'Z'  in hex   ("5a"  retorna 'z' e nao maiusculo)
  res = te::common::Hex2Char(hexZ_lc);
  std::cout << " Input Z: " << hexZ_lc << " Output res: " << res  << std::endl;

  char hexm[] = "6d";  //character 'm'  in hex (it does not accept lower case - return wrong value
  res = te::common::Hex2Char(hexm);
  std::cout << " Input m  " << hexm << " Output res: " << res  << std::endl;

  char hex_xx[] = "7B";  //character '{'  in hex 
  char res_xx = te::common::Hex2Char(hex_xx);
  std::cout << " Input {: " << hex_xx << " Output res: " << res_xx  << std::endl;
  std::cout << std::endl;

  // Block of final test - reading from  json 
  {
    // Reading from json file into ascii_tab property tree and converting to ascii_json vector 
    std::vector<std::pair<std::string, std::string>>  ascii_json;
    std::vector<std::pair<std::string, std::string>>::iterator itpair; 
    boost::property_tree::ptree ascii_tab;
    boost::property_tree::read_json("../data/module_common/hexutils/ascii_tab.json", ascii_tab);
  
    te::common::Convert(ascii_tab.get_child("ascii_t"), ascii_json);
    std::cout << std::endl;

    for(itpair = ascii_json.begin(); itpair < ascii_json.end(); itpair++)
    {
      std::cout << "First: " << (*itpair).first << " Second: " <<(*itpair).second  << std::endl;
      const char* h = ((*itpair).second).c_str();    
      char res_ascii = te::common::Hex2Char(((*itpair).second).c_str());
      const char cres_ascci = res_ascii;
      const char* itout = ((*itpair).first).c_str();
      char c_itout = *(((*itpair).first).c_str());
      CPPUNIT_ASSERT(res_ascii == c_itout);
      CPPUNIT_ASSERT(te::common::Hex2Char(((*itpair).second).c_str()) == *(((*itpair).first).c_str()) );
    }
  }
}

void TsHexUtils::tcHex2Char2()
{
  char hex1[]= "0x41";  //character 'A' in hex
  unsigned char res1 = te::common::Hex2Char2(hex1);
  std::cout << "Output tcHex2Char2 res1: " << res1  << std::endl;
  char hex2[]=  "0x39";  //character '9' in hex 
  unsigned char res2 = te::common::Hex2Char2(hex2);
  std::cout << "Output tcHex2Char2 res2: " << res2  << std::endl;

  {
    char hex1[]= "41";  //character 'A' in hex
    char res1 = te::common::Hex2Char2(hex1);
    std::cout << " Input: " << hex1 << " Output tcHex2Char2 res1: " << res1  << std::endl;

    char hex2[] = "39";  //character '9' in hex 
    char res2 = te::common::Hex2Char2(hex2);
    std::cout << " Input: " << hex2 << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hex3[] = "61";  //character 'A' in hex 
    res2 = te::common::Hex2Char2(hex3);
    std::cout << " Input: " << hex3 << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hex4[] = "4a";  //character 'J' in hex  "4a" retorna ´j´
    res2 = te::common::Hex2Char2(hex4);
    std::cout << " Input: " << hex4 << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hex5[] = "46";  //character 'F'  in hex 
    res2 = te::common::Hex2Char2(hex5);
    std::cout << " Input: " << hex5 << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hexG[] = "47";  //character 'G'  in hex 
    res2 = te::common::Hex2Char2(hexG);
    std::cout << " Input: " << hexG << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hexa[] = "61";  //character 'a'  in hex 
    res2 = te::common::Hex2Char2(hexa);
    std::cout << " Input: " << hexa << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hexf[] = "66";  //character 'f'  in hex 
    res2 = te::common::Hex2Char2(hexf);
    std::cout << " Input: " << hexf << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hexg[] = "67";  //character 'g'  in hex 
    res2 = te::common::Hex2Char2(hexg);
    std::cout << " Input: " << hexg << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hexZ[] = "5a";  //character 'Z'  in hex   ("5a"  retorna 'z' e ao maiusculo)
    res2 = te::common::Hex2Char2(hexZ);
    std::cout << " Input 5a: " << hexZ << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hexZ1[] = "5A";  //character 'Z'  in hex   (Hex2Char("5a")  retorna 'z' )
    res2 = te::common::Hex2Char2(hexZ1);
    std::cout << " Input 5A: " << hexZ1 << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hexm[] = "6d";  //character 'm'  in hex (it does not accept lower case - return wrong value
    res2 = te::common::Hex2Char2(hexm);
    std::cout << " Input: " << hexm << " Output tcHex2Char2 res2: " << res2  << std::endl;

    char hex_xx[] = "7b";  //character '{'  in hex 
    char res_xx = te::common::Hex2Char2(hex_xx);
    std::cout << " Input {: " << hex_xx << " Output tcHex2Char2 res2: " << res_xx  << std::endl;
  }

  // Final tests - reading from json
  {
    // Reading from json file into ascii_tab property tree and converting to ascii_json vector
    std::vector<std::pair<std::string, std::string>>  ascii_json;
    std::vector<std::pair<std::string, std::string>>::iterator itpair; 
    boost::property_tree::ptree ascii_tab;
    boost::property_tree::read_json("../data/module_common/hexutils/ascii_tab.json", ascii_tab);
    te::common::Convert(ascii_tab.get_child("ascii_t"), ascii_json);

    for(itpair = ascii_json.begin(); itpair < ascii_json.end(); itpair++)
    {
      std::cout << " First: " << (*itpair).first << " Second: " <<(*itpair).second  << std::endl;
      const char* h = ((*itpair).second).c_str();    
      char res_ascii = te::common::Hex2Char2(((*itpair).second).c_str());
      const char cres_ascci = res_ascii;
      const char* itout = ((*itpair).first).c_str();
      char c_itout = *(((*itpair).first).c_str());
      CPPUNIT_ASSERT(res_ascii == c_itout);
      CPPUNIT_ASSERT(te::common::Hex2Char2(((*itpair).second).c_str()) == *(((*itpair).first).c_str()) );
    }
  }
}

void TsHexUtils::tcHex2Binary_1()
{
}
void TsHexUtils::tcHex2Binary_2()
{
}
void TsHexUtils::tcHex2Binary2_1()
{
}
void TsHexUtils::tcHex2Binary2_2()
{
}
void TsHexUtils::tcBinary2Hex_1()
{
}
void TsHexUtils::tcBinary2Hex_2()
{
}