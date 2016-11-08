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
  \file examples/ado/CopyDataSet.cpp

  \brief It copy a DataSet
*/

// Example
#include "ADOExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/utils/Utils.h>

// STL
#include <iostream>

std::unique_ptr<te::da::DataSource> GetADOConnection()
{
  // Set the minimum server connection information needed to connect to the database server
  std::string connInfo("file://");
  std::string data_dir = TERRALIB_DATA_DIR;
  std::string user, pass, path;

  std::cout << "Inform the location of your Microsoft Access database (ENTER to accept default \'" << (data_dir + "/ado/ADODataSource.mdb") << "\'): ";
  std::getline(std::cin, path);

  std::cout << "Inform the user to access your Microsoft Access database (ENTER if there is none): ";
  std::getline(std::cin, user);

  std::cout << "Inform the Password to access Microsoft Access database (ENTER if there is none): ";
  std::getline(std::cin, pass);

  connInfo += user.empty() ? "" : user + ":";
  connInfo += pass.empty() ? "" : pass + "@";
  connInfo += path.empty() ? (data_dir + "/ado/ADODataSource.mdb") : path;

#ifdef _M_IX86
  connInfo += "?PROVIDER=Microsoft.Jet.OLEDB.4.0";
#else
  connInfo += "?PROVIDER=Microsoft.ACE.OLEDB.12.0";
#endif

  // Create a data source using the data source factory
  std::unique_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("ADO", connInfo));

  try
  {
    //Open the data source using the connection information given above
    ds->open();
    return std::move(ds);
  }
  catch (const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cout << std::endl << "An unexpected exception has occurred!" << std::endl;
  }
}
