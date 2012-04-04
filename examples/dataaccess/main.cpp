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

  \brief A list of examples for the TerraLib DataAccess Module.
 */

// TerraLib
#include <terralib/common/TerraLib.h>

// Examples
#include "DataAccessExamples.h"

// STL
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
// It initializes all the data source drivers (see LoadModule.cpp)
  try
  {
// initialize Terralib support
    TerraLib::getInstance().initialize();

    LoadModules();

    OGRExampleRead();

    ORGExampleWrite();

    PostGISExample();     

//    QueryExample(); //??? tem codigo comentado - cai...

    CopyingData();
	
    GDALExample();

    MemoryExample();

//    PersistenceExample(); //??? Está caindo - em ======Adds a new property and its values  

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried: " << e.what() << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried!" << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

// shutdown Terralib support
  //te::plugin::Platform::finalize();
  //te::da::Platform::finalize();
  //te::gm::Platform::finalize();
  //te::common::Platform::finalize();

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}

