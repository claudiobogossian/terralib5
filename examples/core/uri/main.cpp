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
#include <terralib/core/uri/URI.h>

// STL
#include <assert.h>
#include <cstdlib>

int main(int argc, char *argv[])
{
  te::core::URI uri("http://www.dpi.inpe.br/terralib5/wiki/doku.php?id=wiki:documentation:devguide#modules");

  assert(uri.scheme() == "http");
  assert(uri.host() == "www.dpi.inpe.br");
  assert(uri.path() == "/terralib5/wiki/doku.php");
  assert(uri.query() == "id=wiki:documentation:devguide");
  assert(uri.fragment() == "modules");

  if(!uri.isValid())
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
