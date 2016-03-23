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
  \file terralib/unittest/core/encoding/TsCharEncoding.cpp

  \brief A test suit for the TerraLib Core Encoding Module.

  \author Matheus Cavassan Zaglia.
 */

// STL
#include <string>
#include <iostream>

// TerraLib
#include <terralib/core/encoding/CharEncoding.h>

// Boost
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(encoding_test)
{
  std::string src = "\u004c\u00f5\u0072\u00e9\u006d \u0069\u0070\u0073\u0075\u006d \u0064\u006f\u006c\u00f5\u0072 \u0073\u00ed\u0074 \u00e3\u006d\u0065\u0074\u002c \u00e1\u0065\u0071\u0075\u0065 \u0065\u0073\u0073\u0065\u006e\u0074 \u0076\u0069\u0074\u0075\u0070\u0065\u0072\u0061\u0074\u0061 \u006e\u0065 \u0068\u00e2\u0073\u002c \u006d\u00ea\u006c \u0069\u006e \u0069\u0075\u0064\u0069\u0063\u00e3\u0062\u0069\u0074 \u0069\u0072\u00e0\u0063\u0075\u006e\u0064\u0069\u00e0\u002c \u00e7\u0075 \u006d\u0065\u006e\u0061\u006e\u0064\u0072\u0069 \u0064\u0065\u0073\u00e9\u0072\u0075\u0069\u0073\u0073\u0065 \u0071\u0075\u006f\u0021 \u0056\u0065\u006c \u0065\u0074 \u0063\u0069\u0076\u0069\u0062\u0075\u0073 \u00e1\u0072\u0067\u0075\u006d\u0065\u006e\u0074\u0075\u006d\u002e \u0042\u00f5\u006e\u006f\u0072\u0075\u006d \u0073\u0065\u006e\u0073\u00e9\u0072\u0069\u0074 \u00e9\u0066\u0066\u00ed\u0063\u0069\u0065\u006e\u0064\u0069 \u006e\u00e0\u006d \u0063\u00fa\u002e \u0050\u00f5\u0073\u0073\u0065 \u0076\u0069\u0076\u00ea\u006e\u0064\u00fa\u006d \u00e1\u006e \u0076\u0069\u006d\u002c \u0064\u0075\u006f \u0064\u006f\u006c\u00f5\u0072 \u0064\u0065\u0063\u006f\u0072\u0065 \u0064\u0069\u0073\u0070\u00fa\u0074\u00e0\u006e\u0064\u00f4 \u0065\u0074\u002e \u0048\u00ed\u0073 \u0063\u0075 \u00e3\u0067\u00e3\u006d \u0066\u0075\u0067\u0069\u0074 \u0065\u0072\u0069\u0070\u00fa\u0069\u0074\u002c \u0074\u0069\u006d\u0065\u00e0\u006d \u0072\u0061\u0074\u0069\u00f5\u006e\u0069\u0062\u0075\u0073 \u0073\u0069\u0074 \u00e3\u0074\u002c \u00ea\u0074 \u00e9\u0075\u006d \u0070\u0072\u006f\u0064\u0065\u0073\u0073\u0065\u0074 \u00e3\u0062\u0068\u006f\u0072\u0072\u00e9\u0061\u006e\u0074 \u0063\u00f5\u006e\u0073\u0074\u0069\u0074\u00fa\u0061\u006d\u003f \u0045\u0070\u0069\u0063\u0075\u0072\u0069 \u0070\u00e9\u0072\u0074\u0069\u006e\u00e2\u0078 \u0073\u00ea\u0064 \u00e2\u0074\u002c \u00ea\u0078 \u0076\u0069\u0078 \u0074\u00ed\u006e\u0063\u0069\u0064\u0075\u006e\u0074 \u0064\u00ed\u0073\u0073\u0065\u006e\u0074\u00ed\u0061\u0073 \u0076\u006f\u006c\u0075\u0070\u0074\u0061\u0074\u0069\u0062\u0075\u0073\u002e";



  BOOST_CHECK(src != te::core::CharEncoding::fromUTF8(src, te::core::EncodingType::LATIN1));
  BOOST_CHECK(src.size() != te::core::CharEncoding::fromUTF8(src, te::core::EncodingType::LATIN1).size());

  return;
}
