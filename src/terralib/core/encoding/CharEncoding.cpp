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

  \brief A module for character enconding/decoding handling.

  \author Matheus Cavassan Zaglia
*/

#include "CharEncoding.h"

//Boost
#include <boost/locale.hpp>
#include <boost/assign/list_of.hpp>

const std::map<te::core::Encoding, std::string> te::core::CharEncoding::EncodingString = boost::assign::map_list_of(te::core::Encoding::UTF8, "UTF8")
                                                                                                  (te::core::Encoding::CP1250, "CP1250")
                                                                                                  (te::core::Encoding::CP1251, "CP1251")
                                                                                                  (te::core::Encoding::CP1252,"CP1252")
                                                                                                  (te::core::Encoding::CP1253,"CP1253")
                                                                                                  (te::core::Encoding::CP1254,"CP1254")
                                                                                                  (te::core::Encoding::CP1257,"CP1257")
                                                                                                  (te::core::Encoding::LATIN1 ,"LATIN1")
                                                                                                  (te::core::Encoding::UNKNOWN_CHAR_ENCODING, "UNKNOWN");

std::string te::core::CharEncoding::encode(const std::string &src, const te::core::Encoding &from)
{
  return boost::locale::conv::to_utf<char>(src, EncodingString.at(from));
}

std::string te::core::CharEncoding::decode(const std::string &src, const te::core::Encoding &to)
{
  return boost::locale::conv::from_utf<char>(src, EncodingString.at(to));
}

std::string te::core::CharEncoding::convert(const std::string &src, const te::core::Encoding &from, const te::core::Encoding &to)
{
  return boost::locale::conv::between(src, EncodingString.at(to), EncodingString.at(from));
}
