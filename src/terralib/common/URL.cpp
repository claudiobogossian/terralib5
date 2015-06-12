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
  \file terralib/common/URL.cpp

  \brief A Uniform Resource Locator.
*/

// TerraLib
#include "Exception.h"
#include "Translator.h"
#include "URL.h"

// uripp - MIT
#include "URI/authority.h"
#include "URI/domain_name.h"
#include "URI/fragment.h"
#include "URI/ip_address.h"
#include "URI/ipv6_address.h"
#include "URI/path.h"
#include "URI/query.h"
#include "URI/scheme.h"
#include "URI/uri.h"
#include "URI/urisyn.h"
#include "URI/utils.h"

// STL
#include <cstdio>
#include <stdexcept>

te::common::URL::URL()
{
}

te::common::URL::URL(const std::string& s) // encoded string
{
  try
  {
    m_uri = te::common::uri::uri(s);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::URL::~URL()
{
}

std::string te::common::URL::getString()
{
  try
  {
    std::string ss, s, login, password;
    std::string protocol = getProtocol();
    s = protocol + "://";

    te::common::uri::authority& a = m_uri.authority();
    login = a.getLogin();
    password = a.getPassword();
    if(login.empty() == false)
      s += login;
    if(password.empty() == false)
      s += ":" + password;
    if(login.empty() == false || password.empty() == false)
      s += '@';

    s += getHost();
    int port = getHostPort();

    if(port != a.getDefaultPort(protocol))
    {
      s += ":";
      char buf[10];
      sprintf(buf, "%d", port);
      s += buf;
    }

    ss = getPathString();
    if(ss.empty() == false)
        s += ss;
    ss = getQueryString();
    if(ss.empty() == false)
      s += "?" + ss;

    ss = getFragmentString();
    if(ss.empty() == false)
      s += "#" + ss;

    return s;
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

std::string te::common::URL::getEncodedString()
{
  try
  {
    std::string ss, s, login, password;
    std::string protocol = getProtocol();
    s = protocol + "://";

    te::common::uri::authority& a = m_uri.authority();
    login = a.getEncodedLogin();
    password = a.getEncodedPassword();
    if(login.empty() == false)
      s += login;
    if(password.empty() == false)
      s += ":" + password;
    if(login.empty() == false || password.empty() == false)
      s += '@';

    s += getHost();
    int port = getHostPort();

    if(port != a.getDefaultPort(protocol))
    {
      s += ":";
      char buf[10];
      sprintf(buf, "%d", port);
      s += buf;
    }

    ss = getEncodedPathString();
    if(ss.empty() == false)
        s += ss;
    ss = getEncodedQueryString();
    if(ss.empty() == false)
      s += "?" + ss;

    ss = getEncodedFragmentString();
    if(ss.empty() == false)
      s += "#" + ss;

    return s;
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setEncodedString(const std::string& s)
{
  try
  {
    *this = URL(s);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

const std::string& te::common::URL::getProtocol() const
{
  try
  {
    return m_uri.scheme().string();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setProtocol(const std::string& s)
{
  try
  {
    te::common::uri::scheme& scheme = m_uri.scheme();
    scheme = te::common::uri::scheme(s);
    setHostPort(m_uri.authority().getDefaultPort(s));
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

const std::string& te::common::URL::getHost() const
{
  try
  {
    return m_uri.authority().host();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setHost(const std::string& s)
{
  try
  {
    te::common::uri::authority& a = m_uri.authority();
    a = te::common::uri::authority(s);
    std::string protocol = getProtocol();
    if(a.port() == 0)
      a.setDefaultPort(protocol);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::uri::authority::host_type_e te::common::URL::getHostType() const
{
  try
  {
    return m_uri.authority().host_type();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

unsigned short te::common::URL::getHostPort() const
{
  try
  {
    return m_uri.authority().port();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setHostPort(unsigned short p)
{
  try
  {
    te::common::uri::authority& a = m_uri.authority();
    a.port(p);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

std::string te::common::URL::getLogin() const
{
  try
  {
    return m_uri.authority().getLogin();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setLogin(const std::string& s)
{
  try
  {
    te::common::uri::authority& a = m_uri.authority();
    a.setLogin(s);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

std::string te::common::URL::getPassword() const
{
  try
  {
    return m_uri.authority().getPassword();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setPassword(const std::string& s)
{
  try
  {
    te::common::uri::authority& a = m_uri.authority();
    a.setPassword(s);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

std::string te::common::URL::getPathString() const
{
  try
  {
    return m_uri.path().string();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

std::string te::common::URL::getEncodedPathString() const
{
  try
  {
    return m_uri.path().encoding();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setEncodedPathString(const std::string& spath)
{
  try
  {
    te::common::uri::path& path = m_uri.path();
    path = te::common::uri::path(spath);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::clearPath()
{
  try
  {
    te::common::uri::path& path = m_uri.path();
    path.clear();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::addPath(const std::string& p)
{
  try
  {
    te::common::uri::path& path = m_uri.path();
    path += p;
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

bool te::common::URL::isAbsolutePath() const
{
  try
  {
    return m_uri.path().absolute();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setIsAbsolutePath(bool v)
{
  try
  {
    te::common::uri::path& path = m_uri.path();
    path.absolute(v);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

bool te::common::URL::isDirectoryPath() const
{
  try
  {
    return m_uri.path().is_directory();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setIsDirectoryPath(bool v)
{
  try
  {
    te::common::uri::path& path = m_uri.path();
    path.is_directory(v);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

bool te::common::URL::isEmptyPath() const
{
  try
  {
    return m_uri.path().empty();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::URL::const_pathIterator te::common::URL::beginPath() const
{
  try
  {
    const te::common::uri::path& path = m_uri.path();
    return path.begin();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::URL::const_pathIterator te::common::URL::endPath() const
{
  try
  {
    const te::common::uri::path& path = m_uri.path();
    return path.end();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

std::string te::common::URL::getQueryString()
{
  try
  {
    std::string s;
    te::common::URL::const_queryIterator it = beginQuery();
    while(it != endQuery())
    {
      s += it->first;
      s += "=";
      s += it->second;
      it++;
      if(it != endQuery())
        s += "&";
    }
    return s;
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

std::string te::common::URL::getEncodedQueryString() const
{
  try
  {
    return m_uri.query().encoding();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setEncodedQueryString(const std::string& squery)
{
  try
  {
    te::common::uri::query& query = m_uri.query();
    query = te::common::uri::query(squery);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

int te::common::URL::getQuerySize() const
{
  try
  {
    return m_uri.query().size();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::clearQuery()
{
  try
  {
    te::common::uri::query& query = m_uri.query();
    query = te::common::uri::query();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::addQuery(const std::string& key, const std::string& value)
{
  try
  {
    te::common::uri::query& query = m_uri.query();
    query.push_back(std::make_pair(key, value));
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::URL::queryIterator te::common::URL::beginQuery()
{
  try
  {
    te::common::uri::query& query = m_uri.query();
    return query.begin();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::URL::queryIterator te::common::URL::endQuery()
{
  try
  {
    te::common::uri::query& query = m_uri.query();
    return query.end();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::URL::const_queryIterator te::common::URL::beginQuery() const
{
  try
  {
    const te::common::uri::query& query = m_uri.query();
    return query.begin();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::URL::const_queryIterator te::common::URL::endQuery() const
{
  try
  {
    const te::common::uri::query& query = m_uri.query();
    return query.end();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::URL::queryIterator te::common::URL::findQueryKey(const std::string& key)
{
  try
  {
    te::common::uri::query& query = m_uri.query();
    te::common::uri::query::iterator it = query.find(key);
    return it;
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

te::common::URL::const_queryIterator te::common::URL::findQueryKey(const std::string& key) const
{
  try
  {
    const te::common::uri::query& query = m_uri.query();
    te::common::uri::query::const_iterator it = query.find(key);
    return it;
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

std::string te::common::URL::getFragmentString() const
{
  try
  {
    return m_uri.fragment().string();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

std::string te::common::URL::getEncodedFragmentString() const
{
  try
  {
    return m_uri.fragment().encoding();
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setFragmentString(const std::string& f)
{
  try
  {
    std::string s = te::common::uri::urisyn::encode(te::common::uri::urisyn::FRAGMENT_TRAITS, f);
    te::common::uri::fragment ff(s);
    setEncodedFragmentString(s);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}

void te::common::URL::setEncodedFragmentString(const std::string& f)
{
  try
  {
    te::common::uri::fragment& fragment = m_uri.fragment();
    fragment = te::common::uri::fragment(f);
  }
  catch(std::exception& e)
  {
    throw(Exception(e.what()));
  }
}
