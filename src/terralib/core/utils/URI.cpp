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
  \file terralib/core/utils/URI.h

  \brief  This file contains utility functions used to manipulate data from a URI.

  \author Andre Gomes de Oliveira
  \author Gilberto Ribeiro de Queiroz
*/

//Terralib
#include "../Exception.h"
#include "../translator/Translator.h"
#include "URI.h"

// STL
#include <iterator>

// Boost
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>

std::map<std::string, std::string>
te::core::Expand(const std::string& query_str)
{
  std::map<std::string, std::string> result;

  boost::char_separator<char> kvp_sep("&");

  boost::tokenizer<boost::char_separator<char> > key_value_pairs(query_str, kvp_sep);

  boost::tokenizer<boost::char_separator<char> >::iterator ikv_pair = key_value_pairs.begin();

  while(ikv_pair != key_value_pairs.end())
  {
    std::string kv = *ikv_pair;

    boost::char_separator<char> kv_sep("=");

    boost::tokenizer<boost::char_separator<char> > key_value_pair(kv, kv_sep);

    boost::tokenizer<boost::char_separator<char> >::iterator ikv = key_value_pair.begin();

    if(ikv == key_value_pair.end())
    {
      boost::format err_msg(TE_TR("Invalid key-value pair format in query string!"));
      throw Exception() << te::ErrorDescription((err_msg).str());
    }

    std::string k = *ikv;

    ++ikv;

    if (ikv == key_value_pair.end())
    {
      boost::format err_msg(TE_TR("Invalid key-value pair format in query string!"));
      throw Exception() << te::ErrorDescription((err_msg).str());
    }

    std::string v = *ikv;

    //std::string decoded_k;
    //std::string decoded_v;

    //decode(k.begin(), k.end(), std::back_inserter(decoded_k));
    //decode(v.begin(), v.end(), std::back_inserter(decoded_v));

    //result[decoded_k] = decoded_v;
    result[k] = v;

    ++ikv_pair;
  }

  return result;
}
