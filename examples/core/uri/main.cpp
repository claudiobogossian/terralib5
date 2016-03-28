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

// STL
#include <assert.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
  try
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
  catch(te::Exception& e)
  {
    std::cout << boost::get_error_info<te::ErrorDescription >(e)->c_str() << std::endl;
    return EXIT_FAILURE;
  }

  try
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
  catch(te::Exception& e)
  {
    std::cout << boost::get_error_info< te::ErrorDescription >(e)->c_str() << std::endl;
    return EXIT_FAILURE;
  }

  try
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
  catch(te::Exception& e)
  {
    std::cout << boost::get_error_info< te::ErrorDescription >(e)->c_str() << std::endl;
    return EXIT_FAILURE;
  }

  try
  {
    te::core::URI uri("InvalidURI");

    if(uri.isValid())
    {
      std::cout << "Uri isn't valid!" << std::endl;
      return EXIT_FAILURE;
    }
  }
  catch(te::Exception& e)
  {
    //std::cout << boost::get_error_info< te::ErrorDescription >(e)->c_str() << std::endl;
  }

  return EXIT_SUCCESS;
}
