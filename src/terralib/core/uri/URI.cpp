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

  \author Vinicius Campanha
  \author Gilberto Ribeiro de Queiroz
 */

// TerraLib
#include "URI.h"

static const std::string regex_scheme = "(?<SCHEME_TYPE>[^\\]\\[:/?#@!$&'()*+,;=]+?):";

static const std::string regex_userInfo = "((?<USER_TYPE>.+):(?<PASSWORD_TYPE>.+)@)?";

static const std::string regex_host = "(?<HOST_TYPE>[^\\]\\[:/?#@!$&'()*+,;=]+)";

static const std::string regex_port = "(:(?<PORT_TYPE>[0-9]{1,5}))?";

static const std::string regex_path = "(?<PATH_TYPE>[^\\]\\[:?#!$&'()*+,;=]+)?";

static const std::string regex_query = "(\\?(?<QUERY_TYPE>[^\\]\\[/?#@!$'()*+,]+))?";

static const std::string regex_fragment = "(#(?<FRAGMENT_TYPE>.+))?";

te::core::URI::URI()
  : isValid_(false)
{
}

te::core::URI::URI(const std::string& uri)
  : uri_(uri), isValid_(false)
{
  parse();
}

te::core::URI::URI(const URI& other)
  : uri_(other.uri_)
{
  parse();
}

te::core::URI&
te::core::URI::operator=(const URI& other)
{
  URI(other).swap(*this);
  return *this;
}

const std::string&
te::core::URI::uri() const
{
  return uri_;
}

std::string te::core::URI::scheme() const
{
  return match_["SCHEME_TYPE"].str();
}

std::string te::core::URI::user() const
{
  return match_["USER_TYPE"].str();
}

std::string te::core::URI::password() const
{
  return match_["PASSWORD_TYPE"].str();
}

std::string te::core::URI::host() const
{
  return match_["HOST_TYPE"].str();
}

std::string te::core::URI::port() const
{
  return match_["PORT_TYPE"].str();
}

std::string te::core::URI::path() const
{
  return match_["PATH_TYPE"].str();
}

std::string te::core::URI::query() const
{
  return match_["QUERY_TYPE"].str();
}

std::string te::core::URI::fragment() const
{
  return match_["FRAGMENT_TYPE"].str();
}

bool te::core::URI::isValid() const
{
  return isValid_;
}

void
te::core::URI::swap(URI& other)
{
  std::swap(uri_, other.uri_);
  std::swap(match_, other.match_);
  std::swap(isValid_, other.isValid_);
}

void
te::core::URI::parse()
{
  isValid_ = false;

  encode();

// first, let's try to match an URI with host
  boost::regex expression(regex_scheme + "//" + regex_userInfo + regex_host + regex_port + regex_path + regex_query + regex_fragment);

  boost::match_results< std::string::const_iterator > match;

// clear match results
  match_ = match;

// is it a valid URI with host?
  if(boost::regex_match(uri_, match, expression, boost::match_default))
  {
    isValid_ = true;
  }
  else
  {
// let's try to macth an URI without host!

    // Has empty groups to compensate the non used user, password and host groups
    boost::regex expression(regex_scheme + "/{0,2}((?<USER_TYPE>)(?<PASSWORD_TYPE>))(?<HOST_TYPE>)(?<PORT_TYPE>())" + regex_path + regex_query + regex_fragment);

    if(boost::regex_match(uri_, match, expression, boost::match_default))
      isValid_ = true;
  }

  match_ = match;
}

void
te::core::URI::encode()
{
  std::string temp;

  std::string::const_iterator it = std::begin(uri_);

  while(it != std::end(uri_))
  {
    int ASCIIvalue = *it;

    if(ASCIIvalue < 33 || ASCIIvalue > 126)
    {
      temp += '%';
      temp += hexToLetter((*it >> 4) & 0x0f);
      temp += hexToLetter(*it & 0x0f);
    }
    else
      temp += *it;

    it++;
  }

  uri_ = temp;
}


std::string
te::core::URI::hexToLetter(int i)
{
  int ASCIIvalue = i;
  std::string value;

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
  return std::string();
}
