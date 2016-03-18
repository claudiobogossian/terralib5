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
  \file terralib/examples/core/encoding/main.cpp

  \brief Examples for the Terralib Encoding Module

  \author Matheus Cavassan Zaglia
*/

// TerraLib
#include <terralib/core/encoding/CharEncoding.h>

// STL
#include <cstdlib>
#include <iostream>
#include <fstream>
int main(int argc, char *argv[])
{

  std::ifstream cp1252("/home/mzaglia/shared/arq_cp1252.txt");
  std::ifstream latin1("/home/mzaglia/shared/arq_latin1.txt");
  std::ifstream utf8("/home/mzaglia/shared/arq_utf8.txt");

  std::string cp1252_s((std::istreambuf_iterator<char>(cp1252)), std::istreambuf_iterator<char>());
  std::string latin1_s((std::istreambuf_iterator<char>(latin1)), std::istreambuf_iterator<char>());
  std::string utf8_s((std::istreambuf_iterator<char>(utf8)), std::istreambuf_iterator<char>());

  std::cout << "\n\n===== BEFORE CONVERSION =====\n\n" << std::endl;
  std::cout << "CP1252:\n" << cp1252_s << std::endl;
  std::cout << "LATIN1:\n" << latin1_s << std::endl;
  std::cout << "UTF8:\n" << utf8_s << std::endl;

  #ifdef __linux__

  cp1252_s = te::core::CharEncoding::encode(cp1252_s, te::core::CP1252);
  latin1_s = te::core::CharEncoding::encode(latin1_s, te::core::LATIN1);
  utf8_s = te::core::CharEncoding::decode(utf8_s, te::core::LATIN1);

  std::cout << "\n\n===== AFTER CONVERSION  =====\n\n" << std::endl;
  std::cout << "CP1252 to UTF-8:\n" << cp1252_s << std::endl;
  std::cout << "LATIN1 to UTF-8:\n" << latin1_s << std::endl;
  std::cout << "UTF-8 to LATIN1:\n" << utf8_s << std::endl;

  #elif _WIN32

  #endif


  return EXIT_SUCCESS;
}
