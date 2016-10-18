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
  \file main.cpp

  \brief An example of using the TerraLib ADO Driver.
 */

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/memory/DataSet.h>
#include <terralib/plugin.h>

// Example
#include "ADOExamples.h"

// STL
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    // Initialize Terralib support
    TerraLib::getInstance().initialize();

    LoadModules();

    std::unique_ptr<te::da::DataSource> ds = GetADOConnection();

    // Show the information about datasets stored in the data source catalog
    PrintCatalog(ds.get());

    // Retrieve and then print the datasets stored in the data source
    PrintDataSets(ds.get());

    // Copying data from a shapefile
    CopyFromShapeFile(ds.get());
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
