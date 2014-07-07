/*  Copyright (C) 2001-20013 National Institute For Space Research (INPE) - Brazil.

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

    //CopyFromShapeFile();

    // Set the minimum server connection information needed to connect to the database server
    std::map<std::string, std::string> connInfo;
    connInfo["PROVIDER"] = "Microsoft.Jet.OLEDB.4.0";
    connInfo["HOST"] = "localhost";
    connInfo["USER_NAME"] = "";
    connInfo["PASSWORD"] = "";

    std::string data_dir = TERRALIB_DATA_DIR;
    connInfo["DB_NAME"] = data_dir + "/mdb/ADODataSource.mdb";
   
    // Create a data source using the data source factory
    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("ADO"));

    // Open the data source using the connection information given above
    ds->setConnectionInfo(connInfo);
    ds->open();


    // Show the information about datasets stored in the data source catalog
    PrintCatalog(ds.get());

    // Retrieve and then print the datasets stored in the data source
    PrintDataSets(ds.get());
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
