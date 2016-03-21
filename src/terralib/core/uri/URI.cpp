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
  \file te/core/URI.cpp

  \brief URI.

  \author Gilberto Ribeiro de Queiroz
  \author Vinicius Campanha
 */

// TerraLib
#include "../../common/Translator.h"
#include "../Exception.h"
#include "URI.h"

te::core::URI::URI()
  : isValid_(false)
{
}

te::core::URI::URI(string_type uri)
  : uri_(uri), isValid_(false)
{
  try
  {
    parse();
  }
  catch(te::Exception& e)
  {
    std::string messageError = TE_TR("Error in URI! \n\n Details: \n");
    messageError.append(e.what());
    throw URIException() << ErrorDescription(messageError);
  }
}

te::core::URI::URI(const URI& other)
  : uri_(other.uri_)
{
  try
  {
    parse();
  }
  catch(te::Exception& e)
  {
    std::string messageError = TE_TR("Error in URI! \n\n Details: \n");
    messageError.append(e.what());
    throw URIException() << ErrorDescription(messageError);
  }
}

te::core::URI::URI(URI&& other) noexcept
{
  // VINICIUS:
}

te::core::URI& te::core::URI::operator=(const te::core::URI& other)
{
  URI(other).swap(*this);
  return *this;
}

te::core::URI& te::core::URI::operator=(te::core::URI&& other) noexcept
{
  // VINICIUS:
}

void te::core::URI::swap(URI& other) noexcept
{
  boost::swap(uri_, other.uri_);

  try
  {
    other.parse();
  }
  catch(te::Exception& e)
  {
    std::string messageError = TE_TR("Error in URI! \n\n Details: \n");
    messageError.append(e.what());
    throw URIException() << ErrorDescription(messageError);
  }

  boost::swap(isValid_, other.isValid_);
}

te::core::URI::const_iterator te::core::URI::begin() const
{
  return boost::begin(uri_);
}

te::core::URI::const_iterator te::core::URI::end() const
{
  return boost::end(uri_);
}

void te::core::URI::parseScheme(const_iterator& begin_it, const_iterator end_it)
{
  const_iterator it = begin_it;

  if(it == end_it)
    throw URIException() << ErrorDescription("Can't find scheme!");

  // Find scheme
  for(; it <= end_it; it++)
  {
    if(*it == '/' || *it == '.' || *it == '?' || *it == '@')
      break;

    if(*it == ':')
    {
      uriParts_.scheme = boost::iterator_range<const_iterator>(begin(), it);
      begin_it = it;
      break;
    }
  }

  if(!uriParts_.scheme || scheme() == "")
    throw URIException() << ErrorDescription("Can't find scheme!");
}

void te::core::URI::parseUserInfo(const_iterator& begin_it, const_iterator end_it)
{
  const_iterator it = begin_it;

  if(it == end_it)
    return;

  //  Check the two "/"
  it++;
  if(it == end_it || *it != '/')
    return;

  it++;
  if(it == end_it || *it != '/')
    return;

  const_iterator user_info_begin = ++it;

  if(user_info_begin == end_it)
    return;

  // Find user first
  bool user = false;
  for( ; it <= end_it; it++)
  {
    if(*it == '@')
      break;

    if(*it == ':')
    {
      user = true;
      break;
    }
  }

  if(user)
  {
    for( ; it <= end_it; it++)
    {
      if(*it == '@')
      {
        uriParts_.hier_part.user_info = boost::iterator_range<const_iterator>(user_info_begin, it);
        begin_it = it;
        break;
      }
    }
  }
}

void te::core::URI::parseHost(const_iterator& begin_it, const_iterator end_it)
{
  const_iterator it = begin_it;

  if(it == end_it)
    return;

  if(!uriParts_.hier_part.user_info || userInfo() == "")
  {
    //  Check the two "/"
    it++;
    if(it == end_it || *it != '/')
      return;

    it++;
    if(it == end_it || *it != '/')
      return;
  }

  // Find Host
  const_iterator host_begin = ++it;

  if(host_begin == end_it)
    return;

  for( ; it <= end_it; it++)
  {
    if(*it == '?')
      break;

    if(*it == ':' || *it == '/' || it == end_it)
    {
      uriParts_.hier_part.host = boost::iterator_range<const_iterator>(host_begin, it);
      begin_it = it;
      break;
    }
  }
}

void te::core::URI::parsePort(const_iterator& begin_it, const_iterator end_it)
{
  const_iterator it = begin_it;

  if(it == end_it)
    return;
  // Find port

  if(*it == ':')
  {
    const_iterator port_begin = ++it;

    if(port_begin == end_it)
      return;

    for( ; it <= end_it; it++)
    {
      if(*it == '.' || *it == '?' || *it == '@')
        break;

      if(*it == '/' || it == end_it)
      {
        uriParts_.hier_part.port = boost::iterator_range<const_iterator>(port_begin, it);
        begin_it = it;
        break;
      }
    }
  }
}

void te::core::URI::parsePath(const_iterator& begin_it, const_iterator end_it)
{
  const_iterator it = begin_it;

  if(it == end_it)
    return;

  const_iterator path_begin;

  if(*it == '/')
    path_begin = it;
  else
    path_begin = ++it;

  if(path_begin == end_it)
    return;

  for( ; it <= end_it; it++)
  {
    if(*it == '=' || *it == '#')
      break;

    if(*it == '?' || it == end_it)
    {
      uriParts_.hier_part.path = boost::iterator_range<const_iterator>(path_begin, it);
      begin_it = it;
      break;
    }
  }
}

void te::core::URI::parseQuery(const_iterator& begin_it, const_iterator end_it)
{
  const_iterator it = begin_it;

  if(it == end_it)
    return;

  if(*it == '?')
  {
    const_iterator query_begin = ++it;

    if(query_begin == end_it)
      return;

    for( ; it <= end_it; it++)
    {
      if(*it == '#' || it == end_it)
      {
        uriParts_.query = boost::iterator_range<const_iterator>(query_begin, it);
        begin_it = it;
        break;
      }
    }
  }
}

void te::core::URI::parseFragment(const_iterator& begin_it, const_iterator end_it)
{
  const_iterator it = begin_it;

  if(it == end_it)
    return;

  if(*it == '#')
  {
    const_iterator fragment_begin = ++it;

    if(fragment_begin == end_it)
      return;

    for( ; it <= end_it; it++)
    {
      if(it == end_it)
      {
        uriParts_.fragment = boost::iterator_range<const_iterator>(fragment_begin, it);
        begin_it = it;
        break;
      }
    }
  }
}

void te::core::URI::parse()
{
  isValid_ = false;
  URIParts<const_iterator> parts;
  uriParts_ = parts;

  // Get uri_ begin
  const_iterator it = begin();

  try
  {
    // Try to find scheme until the end of uri_
    parseScheme(it, end());
    parseUserInfo(it, end());
    parseHost(it, end());
    parsePort(it, end());
    parsePath(it, end());
    parseQuery(it, end());
    parseFragment(it, end());
  }
  catch(te::Exception& e)
  {
    throw;
  }
  catch(...)
  {
    throw URIException() << ErrorDescription("Unknow error on te::core::URI!");
  }

  if(!uriParts_.hier_part.host && !uriParts_.hier_part.path)
    throw URIException() << ErrorDescription("Invalid URI!");

  if(host() == "" && path() == "")
    throw URIException() << ErrorDescription("Invalid URI!");

  uriParts_.update();

  isValid_ = true;
}

te::core::URI::string_type te::core::URI::uri() const
{
  return uri_;
}

te::core::URI::string_type te::core::URI::scheme() const
{
  const_range_type range = scheme_range();
  return range ? string_type(boost::begin(range), boost::end(range))
               : string_type();
}

te::core::URI::string_type te::core::URI::userInfo() const
{
  const_range_type range = userInfo_range();
  return range ? string_type(boost::begin(range), boost::end(range))
               : string_type();
}

te::core::URI::string_type te::core::URI::host() const
{
  const_range_type range = host_range();
  return range ? string_type(boost::begin(range), boost::end(range))
               : string_type();
}

te::core::URI::string_type te::core::URI::port() const
{
  const_range_type range = port_range();
  return range ? string_type(boost::begin(range), boost::end(range))
               : string_type();
}

te::core::URI::string_type te::core::URI::path() const
{
  const_range_type range = path_range();
  return range ? string_type(boost::begin(range), boost::end(range))
               : string_type();
}

te::core::URI::string_type te::core::URI::query() const
{
  const_range_type range = query_range();
  return range ? string_type(boost::begin(range), boost::end(range))
               : string_type();
}

te::core::URI::string_type te::core::URI::fragment() const
{
  const_range_type range = fragment_range();
  return range ? string_type(boost::begin(range), boost::end(range))
               : string_type();
}

bool te::core::URI::isValid()
{
  return isValid_;
}
