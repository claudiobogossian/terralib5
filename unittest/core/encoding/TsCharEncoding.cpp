/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib - a Framework for building GIS enabled
  applications.

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
  \file terralib/unittest/core/encoding/TsCharEncoding.cpp

  \brief A test suit for the TerraLib Core Encoding Module.

  \author Matheus Cavassan Zaglia.
 */

// STL
#include <string>
#include <fstream>

// TerraLib
#include <terralib/BuildConfig.h>
#include <terralib/core/encoding/CharEncoding.h>
#include <terralib/core/Exception.h>
#include <terralib/Defines.h>

// Boost
#include <boost/test/unit_test.hpp>

struct F
{
  F()
      : text_in_utf8(""),
        text_in_latin1(""),
        text_in_cp1252(""),
        text_in_ascii("")
  {
    std::ifstream utf8_file(TERRALIB_DATA_DIR "/encoding/arq_utf8.txt");
    std::ifstream latin1_file(TERRALIB_DATA_DIR "/encoding/arq_latin1.txt");
    std::ifstream cp1252_file(TERRALIB_DATA_DIR "/encoding/arq_cp1252.txt");
    std::ifstream ascii_file(TERRALIB_DATA_DIR "/encoding/arq_ascii.txt");
    text_in_utf8 = std::string((std::istreambuf_iterator<char>(utf8_file)),
                               std::istreambuf_iterator<char>());
    text_in_latin1 = std::string((std::istreambuf_iterator<char>(latin1_file)),
                                 std::istreambuf_iterator<char>());
    text_in_cp1252 = std::string((std::istreambuf_iterator<char>(cp1252_file)),
                                 std::istreambuf_iterator<char>());
    text_in_ascii = std::string((std::istreambuf_iterator<char>(ascii_file)),
                                std::istreambuf_iterator<char>());
  }
  ~F() {}

  std::string text_in_utf8;
  std::string text_in_latin1;
  std::string text_in_cp1252;
  std::string text_in_ascii;
};

BOOST_FIXTURE_TEST_SUITE(encoding, F)

BOOST_AUTO_TEST_CASE(encoding_test_utils)
{
  BOOST_CHECK(te::core::EncodingType::UTF8 ==
              te::core::CharEncoding::getEncodingType("UTF-8"));

  BOOST_CHECK(te::core::EncodingType::CP1250 ==
              te::core::CharEncoding::getEncodingType("CP1250"));

  BOOST_CHECK(te::core::EncodingType::CP1251 ==
              te::core::CharEncoding::getEncodingType("CP1251"));

  BOOST_CHECK(te::core::EncodingType::CP1252 ==
              te::core::CharEncoding::getEncodingType("CP1252"));

  BOOST_CHECK(te::core::EncodingType::CP1253 ==
              te::core::CharEncoding::getEncodingType("CP1253"));

  BOOST_CHECK(te::core::EncodingType::CP1254 ==
              te::core::CharEncoding::getEncodingType("CP1254"));

  BOOST_CHECK(te::core::EncodingType::CP1257 ==
              te::core::CharEncoding::getEncodingType("CP1257"));

  BOOST_CHECK(te::core::EncodingType::LATIN1 ==
              te::core::CharEncoding::getEncodingType("LATIN1"));

  BOOST_CHECK_THROW(te::core::CharEncoding::getEncodingType("UTF8"),
                    te::core::Exception);

  BOOST_CHECK_NO_THROW(te::core::CharEncoding::getEncodingList());
}

BOOST_AUTO_TEST_CASE(encoding_test_utf8_latin1)
{
  std::string utf8_from_latin1 = te::core::CharEncoding::toUTF8(
      text_in_latin1, te::core::EncodingType::LATIN1);
  std::string latin1_from_utf8 = te::core::CharEncoding::fromUTF8(
      text_in_utf8, te::core::EncodingType::LATIN1);

  BOOST_CHECK(utf8_from_latin1 == text_in_utf8);

  BOOST_CHECK(latin1_from_utf8 == text_in_latin1);
}

BOOST_AUTO_TEST_CASE(encoding_test_utf8_cp1252)
{
  std::string utf8_from_cp1252 = te::core::CharEncoding::toUTF8(
      text_in_cp1252, te::core::EncodingType::CP1252);

  std::string cp1252_from_utf8 = te::core::CharEncoding::fromUTF8(
      text_in_utf8, te::core::EncodingType::CP1252);

  BOOST_CHECK(utf8_from_cp1252 == text_in_utf8);

  BOOST_CHECK(cp1252_from_utf8 == text_in_cp1252);
}

BOOST_AUTO_TEST_CASE(encoding_test_latin_cp1252)
{
  std::string latin1_from_cp1252 = te::core::CharEncoding::convert(
      text_in_cp1252, te::core::EncodingType::CP1252,
      te::core::EncodingType::LATIN1);

  std::string cp1252_from_latin1 = te::core::CharEncoding::convert(
      text_in_latin1, te::core::EncodingType::LATIN1,
      te::core::EncodingType::CP1252);

  BOOST_CHECK(latin1_from_cp1252 == text_in_latin1);

  BOOST_CHECK(cp1252_from_latin1 == text_in_cp1252);
}

BOOST_AUTO_TEST_CASE(encoding_test_utf8_ascii)
{
  std::string ascii_from_utf8 = te::core::CharEncoding::toASCII(text_in_utf8);

  BOOST_CHECK(ascii_from_utf8 == text_in_ascii);
}

BOOST_AUTO_TEST_CASE(encoding_test_locale_utf8)
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  std::string latin1_from_utf8 = te::core::CharEncoding::fromUTF8(text_in_utf8);
  BOOST_CHECK(text_in_latin1 == latin1_from_utf8);

  std::string cp1252_from_utf8 = te::core::CharEncoding::fromUTF8(text_in_utf8);
  BOOST_CHECK(text_in_cp1252 == cp1252_from_utf8);
#endif
}

BOOST_AUTO_TEST_CASE(enconding_test_wrong_type)
{
  // using wrong encoding when converting
  std::string wrong_utf8 = te::core::CharEncoding::toUTF8(
      text_in_utf8, te::core::EncodingType::LATIN1);

  std::string wrong_latin1 = te::core::CharEncoding::fromUTF8(
      text_in_latin1, te::core::EncodingType::LATIN1);

  BOOST_CHECK(text_in_latin1 != wrong_latin1);

  BOOST_CHECK(text_in_utf8 != wrong_utf8);
}

BOOST_AUTO_TEST_SUITE_END()
