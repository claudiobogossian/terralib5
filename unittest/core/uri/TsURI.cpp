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
  \file terralib/unittest/core/uri/TsURI.cpp

  \brief A test suit for the Core URI API.

  \author Vinicius campanha
 */

// STL
#include <iostream>

// TerraLib
#include <terralib/core/uri/URI.h>

// Boost
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_case )

BOOST_AUTO_TEST_CASE( test_full_address )
{
  std::string address("http://www.dpi.inpe.br:80/terralib5/wiki/doku.php?id=wiki:documentation:devguide#modules");
  te::core::URI uri(address);

  //  BOOST_CHECK(uri.is_absolute());
  //  BOOST_CHECK(!uri.is_opaque());
  std::cout << "Scheme: " << uri.scheme() << std::endl;
  std::cout << "Host: " << uri.host() << std::endl;
  std::cout << "Port: " << uri.port() << std::endl;
  std::cout << "Path: " << uri.path() << std::endl;
  std::cout << "Query: " << uri.query() << std::endl;
  std::cout << "Fragment: " << uri.fragment() << std::endl;

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.host() == "www.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "80");
  BOOST_CHECK(uri.path() == "/terralib5/wiki/doku.php");
  BOOST_CHECK(uri.query() == "id=wiki:documentation:devguide");
  BOOST_CHECK(uri.fragment() == "modules");

  return;
}

BOOST_AUTO_TEST_CASE( test_full_address2 )
{
  std::string address("http://sedac.ciesin.columbia.edu/geoserver/wcs?service=WCS&request=GetCapabilities&version=2.0.1");
  te::core::URI uri(address);

  //  BOOST_CHECK(uri.is_absolute());
  //  BOOST_CHECK(!uri.is_opaque());
  std::cout << "Scheme: " << uri.scheme() << std::endl;
  std::cout << "Host: " << uri.host() << std::endl;
  std::cout << "Port: " << uri.port() << std::endl;
  std::cout << "Path: " << uri.path() << std::endl;
  std::cout << "Query: " << uri.query() << std::endl;
  std::cout << "Fragment: " << uri.fragment() << std::endl;

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.host() == "sedac.ciesin.columbia.edu");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "/geoserver/wcs");
  BOOST_CHECK(uri.query() == "service=WCS&request=GetCapabilities&version=2.0.1");
  BOOST_CHECK(uri.fragment() == "");

  return;
}

BOOST_AUTO_TEST_CASE( test_ftp_address )
{
  std::string address("ftp://ftp.ftp.inpe.br:21/path/");
//  std::string address("ftp://queimadas:inpe_2012@ftp.dgi.inpe.br:21/focos_operacao/");
  te::core::URI uri(address);

  //  BOOST_CHECK(uri.is_absolute());
  //  BOOST_CHECK(!uri.is_opaque());
  std::cout << "Scheme: " << uri.scheme() << std::endl;
  std::cout << "Host: " << uri.host() << std::endl;
  std::cout << "Port: " << uri.port() << std::endl;
  std::cout << "Path: " << uri.path() << std::endl;
  std::cout << "Query: " << uri.query() << std::endl;
  std::cout << "Fragment: " << uri.fragment() << std::endl;

  BOOST_CHECK(uri.scheme() == "ftp");
  BOOST_CHECK(uri.host() == "ftp.ftp.inpe.br");
  BOOST_CHECK(uri.port() == "21");
  BOOST_CHECK(uri.path() == "/path/");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");

  return;
}

BOOST_AUTO_TEST_CASE( test_opaque_full_address )
{
  std::string address("xmpp:example-node@example.com?message;subject=Hello%20World");
  te::core::URI uri(address);

  //  BOOST_CHECK(uri.is_absolute());
  //  BOOST_CHECK(!uri.is_opaque());
  std::cout << "Scheme: " << uri.scheme() << std::endl;
  std::cout << "Host: " << uri.host() << std::endl;
  std::cout << "Port: " << uri.port() << std::endl;
  std::cout << "Path: " << uri.path() << std::endl;
  std::cout << "Query: " << uri.query() << std::endl;
  std::cout << "Fragment: " << uri.fragment() << std::endl;

  BOOST_CHECK(uri.scheme() == "xmpp");
  BOOST_CHECK(uri.host() == "");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "example-node@example.com");
  BOOST_CHECK(uri.query() == "message;subject=Hello%20World");
  BOOST_CHECK(uri.fragment() == "");

  return;
}

BOOST_AUTO_TEST_CASE( test_partial_adress )
{
  std::string address("http://www.dpi.inpe.br:80");

  te::core::URI uri(address);

  //  BOOST_CHECK(uri.is_absolute());
  //  BOOST_CHECK(!uri.is_opaque());
  std::cout << "Scheme: " << uri.scheme() << std::endl;
  std::cout << "Host: " << uri.host() << std::endl;
  std::cout << "Port: " << uri.port() << std::endl;

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.host() == "www.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "80");
  BOOST_CHECK(uri.path() == "");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_CASE( test_partial_adress2 )
{
  std::string address("http://www.dpi.inpe.br");

  te::core::URI uri(address);

  //  BOOST_CHECK(uri.is_absolute());
  //  BOOST_CHECK(!uri.is_opaque());
  std::cout << "Scheme: " << uri.scheme() << std::endl;
  std::cout << "Host: " << uri.host() << std::endl;
  std::cout << "Port: " << uri.port() << std::endl;

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.host() == "www.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_SUITE_END()
