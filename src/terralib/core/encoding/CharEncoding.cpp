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
  \file terralib/core/encoding/CharEncoding.cpp

  \brief A class for handling character enconding/decoding.

  \author Matheus Cavassan Zaglia
*/

// TerraLib
#include "CharEncoding.h"

// Boost
#include <boost/assign/list_of.hpp>
#include <boost/locale.hpp>

const std::map<te::core::EncodingType, std::string>
te::core::CharEncoding::EncodingString = boost::assign::map_list_of(te::core::EncodingType::UTF8, "UTF8")
                                                                   (te::core::EncodingType::CP1250, "CP1250")
                                                                   (te::core::EncodingType::CP1251, "CP1251")
                                                                   (te::core::EncodingType::CP1252,"CP1252")
                                                                   (te::core::EncodingType::CP1253,"CP1253")
                                                                   (te::core::EncodingType::CP1254,"CP1254")
                                                                   (te::core::EncodingType::CP1257,"CP1257")
                                                                   (te::core::EncodingType::LATIN1 ,"LATIN1")
                                                                   (te::core::EncodingType::UNKNOWN, "UNKNOWN");

std::string
te::core::CharEncoding::toUTF8(const std::string &src, EncodingType from)
{
  return boost::locale::conv::to_utf<char>(src, EncodingString.at(from));
}

std::string
te::core::CharEncoding::fromUTF8(const std::string &src, EncodingType to)
{
  return boost::locale::conv::from_utf<char>(src, EncodingString.at(to));
}

std::string
te::core::CharEncoding::convert(const std::string &src,
                                EncodingType from,
                                EncodingType to)
{
  return boost::locale::conv::between(src, EncodingString.at(to), EncodingString.at(from));
}