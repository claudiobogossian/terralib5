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
#include <terralib/Exception.h>

// Boost
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( uri_test_case )

BOOST_AUTO_TEST_CASE( test_copy_constructor )
{
  std::string address("http://www.dpi.inpe.br:80/terralib5/wiki/doku.php?id=wiki:documentation:devguide#modules");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  te::core::URI copy(uri);

  BOOST_CHECK(copy.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "www.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "80");
  BOOST_CHECK(uri.path() == "/terralib5/wiki/doku.php");
  BOOST_CHECK(uri.query() == "id=wiki:documentation:devguide");
  BOOST_CHECK(uri.fragment() == "modules");

  BOOST_CHECK(uri.scheme() == copy.scheme());
  BOOST_CHECK(uri.user() == copy.user());
  BOOST_CHECK(uri.password() == copy.password());
  BOOST_CHECK(uri.host() == copy.host());
  BOOST_CHECK(uri.port() == copy.port());
  BOOST_CHECK(uri.path() == copy.path());
  BOOST_CHECK(uri.query() == copy.query());
  BOOST_CHECK(uri.fragment() == copy.fragment());

  return;
}

BOOST_AUTO_TEST_CASE( test_copy_assignment )
{
  std::string address("http://www.dpi.inpe.br:80/terralib5/wiki/doku.php?id=wiki:documentation:devguide#modules");
  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  std::string address2("scheme2://user:password@sedac.ciesin.columbia.edu:70/geoserver/wcs?service=WCS&request=GetCapabilities#fragment");
  te::core::URI uri2(address2);

  BOOST_CHECK(uri2.isValid());

  uri2 = uri;

  BOOST_CHECK(uri2.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "www.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "80");
  BOOST_CHECK(uri.path() == "/terralib5/wiki/doku.php");
  BOOST_CHECK(uri.query() == "id=wiki:documentation:devguide");
  BOOST_CHECK(uri.fragment() == "modules");

  BOOST_CHECK(uri.scheme() == uri2.scheme());
  BOOST_CHECK(uri.user() == uri2.user());
  BOOST_CHECK(uri.password() == uri2.password());
  BOOST_CHECK(uri.host() == uri2.host());
  BOOST_CHECK(uri.port() == uri2.port());
  BOOST_CHECK(uri.path() == uri2.path());
  BOOST_CHECK(uri.query() == uri2.query());
  BOOST_CHECK(uri.fragment() == uri2.fragment());

  std::string address3("xmpp:example-node@example.com?message;subject=Hello%20World");
  te::core::URI uri3(address3);

  BOOST_CHECK(uri3.isValid());

  uri = uri3;

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == uri3.scheme());
  BOOST_CHECK(uri.user() == uri3.user());
  BOOST_CHECK(uri.password() == uri3.password());
  BOOST_CHECK(uri.host() == uri3.host());
  BOOST_CHECK(uri.port() == uri3.port());
  BOOST_CHECK(uri.path() == uri3.path());
  BOOST_CHECK(uri.query() == uri3.query());
  BOOST_CHECK(uri.fragment() == uri3.fragment());

  return;
}

BOOST_AUTO_TEST_CASE( test_encode )
{
  std::string address("scheme://host/ ãõáé");
  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  std::string encoded("scheme://host/%20%C3%A3%C3%B5%C3%A1%C3%A9");

  BOOST_CHECK_EQUAL(uri.uri(), encoded);

  return;
}

BOOST_AUTO_TEST_CASE( test_full_http_uri )
{
  std::string address("http://www.dpi.inpe.br:80/terralib5/wiki/doku.php?id=wiki:documentation:devguide#modules");
  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "www.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "80");
  BOOST_CHECK(uri.path() == "/terralib5/wiki/doku.php");
  BOOST_CHECK(uri.query() == "id=wiki:documentation:devguide");
  BOOST_CHECK(uri.fragment() == "modules");

  return;
}

BOOST_AUTO_TEST_CASE( test_full_http_uri_2 )
{
  std::string address("http://sedac.ciesin.columbia.edu/geoserver/wcs?service=WCS&request=GetCapabilities&version=2.0.1");
  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "sedac.ciesin.columbia.edu");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "/geoserver/wcs");
  BOOST_CHECK(uri.query() == "service=WCS&request=GetCapabilities&version=2.0.1");
  BOOST_CHECK(uri.fragment() == "");

  return;
}

BOOST_AUTO_TEST_CASE( test_ftp_uri )
{
  std::string address("ftp://ftp.ftp.inpe.br:21/path/");
  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "ftp");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "ftp.ftp.inpe.br");
  BOOST_CHECK(uri.port() == "21");
  BOOST_CHECK(uri.path() == "/path/");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");

  return;
}

BOOST_AUTO_TEST_CASE( test_ftp_authority_address_uri )
{
  std::string address("ftp://user:password@ftp.ftp.inpe.br:21/path/");
  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "ftp");
  BOOST_CHECK(uri.user() == "user");
  BOOST_CHECK(uri.password() == "password");
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

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "xmpp");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "example-node@example.com");
  BOOST_CHECK(uri.query() == "message;subject=Hello%20World");
  BOOST_CHECK(uri.fragment() == "");

  return;
}

BOOST_AUTO_TEST_CASE( test_http_partial_adress )
{
  std::string address("http://www.dpi.inpe.br:80");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "www.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "80");
  BOOST_CHECK(uri.path() == "");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_CASE( test_http_partial_adress_2 )
{
  std::string address("http://www.dpi.inpe.br");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "www.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_CASE( test_pgsql_uri )
{
  std::string address("pgsql://user:password@atlas.dpi.inpe.br:5433/bdqueimadas?client_encoding=UTF8&max_connections=20");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "pgsql");
  BOOST_CHECK(uri.user() == "user");
  BOOST_CHECK(uri.password() == "password");
  BOOST_CHECK(uri.host() == "atlas.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "5433");
  BOOST_CHECK(uri.path() == "/bdqueimadas");
  BOOST_CHECK(uri.query() == "client_encoding=UTF8&max_connections=20");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_CASE( test_http_uri_3 )
{
  std::string address("http://chronos.dpi.inpe.br/wfs");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "chronos.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "/wfs");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_CASE( test_http_uri_4 )
{
  std::string address("http://chronos.dpi.inpe.br/wms");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "chronos.dpi.inpe.br");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "/wms");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_CASE( test_http_uri_5 )
{
  std::string address("http://xxx.xxx.xx/yyy/yyy");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "xxx.xxx.xx");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "/yyy/yyy");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_CASE( test_http_uri_6 )
{
  std::string address("http://xxx.xxx.xx/yyy/yyy?");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "http");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "xxx.xxx.xx");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "/yyy/yyy");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_CASE( test_file_uri_1 )
{
  std::string address("file:///home/gribeiro/data");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "file");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "/home/gribeiro/data");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}

BOOST_AUTO_TEST_CASE( test_file_uri_2 )
{
  std::string address("file:///home/gribeiro/data/arq.shp");

  te::core::URI uri(address);

  BOOST_CHECK(uri.isValid());

  BOOST_CHECK(uri.scheme() == "file");
  BOOST_CHECK(uri.user() == "");
  BOOST_CHECK(uri.password() == "");
  BOOST_CHECK(uri.host() == "");
  BOOST_CHECK(uri.port() == "");
  BOOST_CHECK(uri.path() == "/home/gribeiro/data/arq.shp");
  BOOST_CHECK(uri.query() == "");
  BOOST_CHECK(uri.fragment() == "");
}


BOOST_AUTO_TEST_SUITE_END()
