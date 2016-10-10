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
  \file terralib/examples/core/uri/main.cpp

  \brief Examples for the Terralib URI API

  \author Vinicius Campanha
*/

// TerraLib
#include <terralib/Exception.h>
#include <terralib/core/uri/URI.h>
#include <terralib/core/utils/URI.h>

// STL
#include <cassert>
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
  {
    te::core::URI uri("ppgsql://postgres:123@localhost:5432/TerraView5?max_connections=410&min_connections=1&idle_time=60&conn_timeout=5&hide_metadata=FALSE&hide_raster_tables=FALSE&new_db_name=Axaou-531");

    if (!uri.isValid())
    {
      std::cout << "Uri isn't valid!" << std::endl;
      return EXIT_FAILURE;
    }

    std::string scheme, user, password, host, port, path, query, fragment;

    scheme = uri.scheme();
    user = uri.user();
    password = uri.password();
    host = uri.host();
    port = uri.port();
    path = uri.path();
    query = uri.query();
    fragment = uri.fragment();

    std::map<std::string, std::string> kvp = te::core::expand(query);

    std::string res;
    static const boost::regex regex_maxCon("&max_connections=(?<MAX_CON>[1-9][0-9]{0,5})");
    static const std::string regex_dbname ("&new_db_name=(?<NEW_DB_NAME>[A-zÀ-ú0-9_-]*)");
    boost::regex expression(regex_dbname);
    boost::match_results< std::string::const_iterator > match;

    if (boost::regex_search(query, match, expression, boost::match_default))
      res = match["NEW_DB_NAME"].str();
  }

  {
    te::core::URI uri("file://E:/Projetos/Dados/rasters/cbers_rgb342_crop1.tif");
    //te::core::URI uri("file:///home/Dados/rasters/cbers_rgb342_crop1.tif");
    
    if (!uri.isValid())
    {
      std::cout << "Uri isn't valid!" << std::endl;
      return EXIT_FAILURE;
    }

    std::string scheme, user, password, host, port, path, query, fragment;

    scheme = uri.scheme();
    user = uri.user();
    password = uri.password();
    host = uri.host();
    port = uri.port();
    path = uri.path();
    query = uri.query();
    fragment = uri.fragment();
  }

  {
    te::core::URI uri("memory:");

    if (!uri.isValid())
    {
      std::cout << "Uri isn't valid!" << std::endl;
      return EXIT_FAILURE;
    }

    std::string scheme, user, password, host, port, path, query, fragment;

    scheme = uri.scheme();
    user = uri.user();
    password = uri.password();
    host = uri.host();
    port = uri.port();
    path = uri.path();
    query = uri.query();
    fragment = uri.fragment();
  }

  {
    te::core::URI uri("file://:adopass@E:/Projetos/Workspace/TerraView-520/data/ado/ADODataSource.mdb");

    std::string scheme, user, password, host, port, path, query, fragment;

    scheme = uri.scheme();
    user = uri.user();
    password = uri.password();
    host = uri.host();
    port = uri.port();
    path = uri.path();
    query = uri.query();
    fragment = uri.fragment();
  }

  {
    te::core::URI uri("http://demo.opengeo.org/geoserver/ows?&VERSION=2.0.1&USERDATADIR=C:/Users/andre.oliveira/AppData/Local/terralib_example_ws_ogc_wcs");

    if (!uri.isValid())
    {
      std::cout << "Uri isn't valid!" << std::endl;
      return EXIT_FAILURE;
    }

    std::string scheme, user, password, host, port, path, query, fragment;

    scheme = uri.scheme();
    user = uri.user();
    password = uri.password();
    host = uri.host();
    port = uri.port();
    path = uri.path();
    query = uri.query();
    fragment = uri.fragment();

  }

  {
    te::core::URI uri("http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:documentation:devguide#modules");

    if(!uri.isValid())
    {
      std::cout << "Uri isn't valid!" << std::endl;
      return EXIT_FAILURE;
    }

    assert(uri.scheme() == "http");
    assert(uri.user() == "");
    assert(uri.password() == "");
    assert(uri.host() == "www.dpi.inpe.br");
    assert(uri.port() == "");
    assert(uri.path() == "/terralib5/wiki/doku.php");
    assert(uri.query() == "id=wiki:documentation:devguide");
    assert(uri.fragment() == "modules");
  }

  {
    std::string address("xmpp:example-node@example.com?message;subject=Hello%20World");

    te::core::URI uri(address);

    if(!uri.isValid())
    {
      std::cout << "Uri isn't valid!" << std::endl;
      return EXIT_FAILURE;
    }

    assert(uri.scheme() == "xmpp");
    assert(uri.user() == "");
    assert(uri.password() == "");
    assert(uri.host() == "");
    assert(uri.port() == "");
    assert(uri.path() == "example-node@example.com");
    assert(uri.query() == "message;subject=Hello%20World");
    assert(uri.fragment() == "");
  }

  {
    te::core::URI uri("ftp://user:password@ftp.ftp.inpe.br:21/path/");

    if(!uri.isValid())
    {
      std::cout << "Uri isn't valid!" << std::endl;
      return EXIT_FAILURE;
    }

    assert(uri.scheme() == "ftp");
    assert(uri.user() == "user");
    assert(uri.password() == "password");
    assert(uri.host() == "ftp.ftp.inpe.br");
    assert(uri.port() == "21");
    assert(uri.path() == "/path/");
    assert(uri.query() == "");
    assert(uri.fragment() == "");
  }

  {
    te::core::URI uri("InvalidURI");

    if(uri.isValid())
    {
      std::cout << "Uri isn't valid!" << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
