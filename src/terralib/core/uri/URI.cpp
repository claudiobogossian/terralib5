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
  \file terralib/core/URI.cpp

  \brief URI.

  \author Gilberto Ribeiro de Queiroz
  \author Vinicius Campanha
 */

// TerraLib
#include "URI.h"
#include "../../common/Translator.h"
#include "../Exception.h"


te::core::URI::URI()
  : isValid_(false)
{
}

te::core::URI::URI(string_type uri)
  : uri_(uri), isValid_(false)
{
  parse();
}

te::core::URI::URI(const URI& other)
  : uri_(other.uri_)
{
  parse();
}


te::core::URI& te::core::URI::operator=(const te::core::URI& other)
{
  URI(other).swap(*this);
  return *this;
}

void te::core::URI::swap(URI& other) noexcept
{
  boost::swap(uri_, other.uri_);
  other.parse();
  boost::swap(isValid_, other.isValid_);
}

void te::core::URI::parse()
{
  isValid_ = false;
  boost::match_results< const_iterator > match;
  match_ = match;

  encode();

  // It's important to verify the groups sequence after updating the search regex.

  // Contains group: 1
  string_type regex_scheme = "([^\\]\\[:/?#@!$&'()*+,;=]+?):";

  // Contains group: 2, 3, 4
  string_type regex_userInfo = "((.+):(.+)@)?";

  // Contains group: 5
  string_type regex_host = "([^\\]\\[:/?#@!$&'()*+,;=]+)";

  // Contains group: 6, 7
  string_type regex_port = "(:([0-9]{1,5}))?";

  // Contains group: 8
  string_type regex_path = "([^\\]\\[:?#!$&'()*+,;=]+)?";

  // Contains group: 9, 10
  string_type regex_query = "(\\?([^\\]\\[/?#@!$'()*+,]+))?";

  // Contains group: 11, 12
  string_type regex_fragment = "(#(.+))?";

  //  URI with host
  boost::regex expression(regex_scheme + "//" + regex_userInfo + regex_host + regex_port + regex_path + regex_query + regex_fragment);

  if(!boost::regex_match(uri_, match_, expression, boost::match_default))
  {
    // URI without host
    // It's important to verify the groups sequence after updating the search regex.
    // Has empty groups to compensate the non used user, password and host groups
    boost::regex expression(regex_scheme + "/{0,2}()()()()()()" + regex_path + regex_query + regex_fragment);

    if(!boost::regex_match(uri_, match_, expression, boost::match_default))
      throw URIException() << ErrorDescription("Invalid URI!");
  }

  isValid_ = true;
}

te::core::URI::string_type te::core::URI::uri() const
{
  return match_[0].str();
}

te::core::URI::string_type te::core::URI::scheme() const
{
  return match_[1].str();
}

te::core::URI::string_type te::core::URI::user() const
{
  return match_[3].str();
}

te::core::URI::string_type te::core::URI::password() const
{
  return match_[4].str();
}

te::core::URI::string_type te::core::URI::host() const
{
  return match_[5].str();
}

te::core::URI::string_type te::core::URI::port() const
{
  return match_[7].str();
}

te::core::URI::string_type te::core::URI::path() const
{
  return match_[8].str();
}

te::core::URI::string_type te::core::URI::query() const
{
  return match_[10].str();
}

te::core::URI::string_type te::core::URI::fragment() const
{
  return match_[12].str();
}

bool te::core::URI::isValid() const
{
  return isValid_;
}

void te::core::URI::encode()
{
  string_type temp;

  const_iterator it = boost::begin(uri_);

  while(it != boost::end(uri_))
  {
    int ASCIIvalue = *it;

    if(ASCIIvalue < 33 || ASCIIvalue > 126)
    {
      temp += '%';
      temp += hex_to_letter((*it >> 4) & 0x0f);
      temp += hex_to_letter(*it & 0x0f);
    }
    else
      temp += *it;

    it++;
  }

  uri_ = temp;
}


te::core::URI::string_type te::core::URI::hex_to_letter(int i)
{
  int ASCIIvalue = i;
  te::core::URI::string_type value;

  switch (ASCIIvalue) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    {
      value = ASCIIvalue + '0';
      return value;
    }
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    default:
    {
      value = ASCIIvalue - 10 + 'A';
      return value;
    }
  }
  return te::core::URI::string_type();
}
