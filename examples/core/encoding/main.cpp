/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
#include <terralib/BuildConfig.h>

// STL
#include <cstdlib>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
  {
    std::cout << "===> Handling character encoding CP1252 <===" << std::endl;
    std::ifstream input_file(TERRALIB_DATA_DIR "/encoding/arq_cp1252.txt");
  
    std::string text_in_cp1252((std::istreambuf_iterator<char>(input_file)),
                               std::istreambuf_iterator<char>());
  
    std::cout << "Original text size in CP1252: " << text_in_cp1252.size() << std::endl;
    std::cout << "Original text in CP1252:\n" << text_in_cp1252 << std::endl << std::endl;
    
    std::string text_in_utf8 = te::core::CharEncoding::toUTF8(text_in_cp1252, te::core::EncodingType::CP1252);
    
    std::cout << "Text size when converted from CP1252 to UTF-8: " << text_in_utf8.size() << std::endl;
    std::cout << "Text converted from CP1252 to UTF-8:\n" << text_in_utf8 << std::endl << std::endl ;
  }

  {
    std::cout << "===> Handling character encoding LATIN1 <===" << std::endl;
    
    std::ifstream input_file(TERRALIB_DATA_DIR "/encoding/arq_latin1.txt");
    
    std::string text_in_latin1((std::istreambuf_iterator<char>(input_file)),
                               std::istreambuf_iterator<char>());
    
    std::cout << "Original text size in LATIN1: " << text_in_latin1.size() << std::endl;
    std::cout << "Original text in LATIN1:\n" << text_in_latin1 << std::endl << std::endl;
    
    std::string text_in_utf8 = te::core::CharEncoding::toUTF8(text_in_latin1, te::core::EncodingType::LATIN1);
    
    std::cout << "Text size when converted from LATIN1 to UTF-8: " << text_in_utf8.size() << std::endl;
    std::cout << "Text converted from LATIN1 to UTF-8:\n" << text_in_utf8 << std::endl << std::endl ;
  }
  
  {
    std::cout << "===> // Handling character encoding UTF8 <===" << std::endl;
    
    std::ifstream input_file(TERRALIB_DATA_DIR "/encoding/arq_utf8.txt");
    
    std::string text_in_utf8((std::istreambuf_iterator<char>(input_file)),
                               std::istreambuf_iterator<char>());
    
    std::cout << "Original text size in UTF8: " << text_in_utf8.size() << std::endl;
    std::cout << "Original text in UTF8:\n" << text_in_utf8 << std::endl << std::endl;
    
    std::string text_in_latin1 = te::core::CharEncoding::fromUTF8(text_in_utf8, te::core::EncodingType::LATIN1);
    
    std::cout << "Text size when converted from UTF8 to LATIN1: " << text_in_latin1.size() << std::endl;
    std::cout << "Text converted from UTF8 to LATIN1:\n" << text_in_latin1 << std::endl << std::endl;
    
    std::string text_in_cp1252 = te::core::CharEncoding::fromUTF8(text_in_utf8, te::core::EncodingType::CP1252);
    
    std::cout << "Text size when converted from UTF8 to CP1252: " << text_in_cp1252.size() << std::endl;
    std::cout << "Text converted from UTF8 to CP1252:\n" << text_in_cp1252 << std::endl ;
  }
  
  return EXIT_SUCCESS;
}
