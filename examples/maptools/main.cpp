/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file main.cpp

  \brief A list of examples for the TerraLib Map Tools.
 */

#include "MapToolsExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/dataaccess.h>
#include <terralib/plugin.h>

// STL
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <locale.h>

unsigned int G_ID = 0;

int main(int /*argc*/, char** /*argv*/)
{
  setlocale(LC_ALL,"C");

// initialize Terralib support
  TerraLib::getInstance().initialize();

  try
  {
    // It initializes all the data source drivers (see LoadModule.cpp)
    LoadModules();

    // Show a MapDisplay
    MapDisplay();

    // Creates a OGC Filter Expression and convert it to TerraLib Expression.
    Filter2Query();

    // Creates a set of Layers and Styles.
    //DrawStyledLayers();

    // Creates a set of raster Layers and Styles.
    DrawRasterStyledLayers();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred!" << std::endl;

    return EXIT_FAILURE;
  }
  
  te::plugin::PluginManager::getInstance().unloadAll();
  
  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
