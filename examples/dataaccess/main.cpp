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
  \file examples/dataaccess/main.cpp

  \brief A list of examples for the TerraLib DataAccess Module.
*/
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/plugin.h>
#include <terralib/dataaccess/dataset/CheckConstraint.h>
#include <terralib/dataaccess/dataset/PrimaryKey.h>
#include <terralib/dataaccess/dataset/Index.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/datatype/SimpleProperty.h>
#include <terralib/memory/DataSource.h>
#include <terralib/memory/DataSourceFactory.h>
#include <terralib/postgis/DataSource.h>
#include <terralib/postgis/DataSourceFactory.h>
#include <terralib/postgis/PreparedQuery.h>

// Examples

// STL
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>

void PrintDataSourceNames(const std::string& dsType, const std::map<std::string, std::string>& info);
std::auto_ptr<te::da::DataSource> CreateDataSource(const std::string& dsType, const std::map<std::string, std::string>& info);
void DropDataSource(const std::string& dsType, const std::map<std::string, std::string>& info);
bool CheckDataSourceExistence(const std::string& dsType, const std::map<std::string, std::string>& info);
void PrintDataSourceEncodings(const std::string& dsType, const std::map<std::string, std::string>& info);

void PrintDataSetNames(te::da::DataSource* ds);
void PrintDataSetPropertyNames(te::da::DataSource* ds, const std::string& datasetName);
void PrintDataSetConstraints(te::da::DataSource* ds, const std::string& datasetName);

//const te::da::DataSetTypePtr& PrintSchema(te::da::DataSource* ds, const std::string& datasetName);
void ExportingOGR();
void LoadModules();

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    // Initialize the Terralib support
    TerraLib::getInstance().initialize();

    LoadModules();

//=== Initial example about datasource: create, drop, check exist, dataset names, encodings, capabilities =====
    std::map<std::string, std::string> connInfo;

    connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;
    connInfo["PG_PORT"] = "5433" ;
    connInfo["PG_USER"] = "postgres";
    connInfo["PG_DB_NAME"] = "terralib4";
    connInfo["PG_CONNECT_TIMEOUT"] = "4";
    connInfo["PG_CLIENT_ENCODING"] = "UTF-8";   //"WIN1252";  //"UTF-8"; 
 
    std::string dsType = "POSTGIS";

    PrintDataSourceNames(dsType, connInfo);

    // Creation of a data source
    connInfo["PG_NEWDB_NAME"] = "new_db";

    std::auto_ptr<te::da::DataSource> newds = CreateDataSource(dsType, connInfo);

    delete newds.release();

    // Drop a data source
    connInfo["PG_DB_TO_DROP"] = "new_db";
    DropDataSource(dsType, connInfo);

    // Check the data source existence
    connInfo["PG_CHECK_DB_EXISTENCE"] = "terralib4";
    bool dsExists = CheckDataSourceExistence(dsType, connInfo);

    if(dsExists)
      std::cout << "\nThe data source \"terralib4\" exists!\n";
    else
      std::cout << "\nThe data source \"terralib4\" doesn't exist!\n";

    PrintDataSourceEncodings(dsType, connInfo);

    // Connection to a data source
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS");

    // Open the data source using the connection info above
    ds->setConnectionInfo(connInfo);
    ds->open();

    PrintDataSetNames(ds.get());

    std::string datasetName = "public.br_munic_2001";

    PrintDataSetPropertyNames(ds.get(), datasetName);

    PrintDataSetConstraints(ds.get(), datasetName);
    PrintDataSourceCapabilities(ds.get());

    ds->close();

    delete ds.release();

//=========== end of Initial example ===========

    PostGISExample();  //ok

    GDALExample();  //ok

    ObjectId(); //ok

    ObjectId_query(); //using builselector and query

    QueryExample_2(); //ok

    MemoryExample(); //Ok

    ExportingOGR();  //ok

    OGRExampleRead(); //ok

    ORGExampleWrite(); //ok

    CopyingData(); //ok

    ///////QueryExample(); //its almost all inside QueryExample_2()

    ///////QueryInsertExample();

    ////////DataSetAdapter();

    //////MySQLExample();

    //////SQLiteExample();

    te::plugin::PluginManager::getInstance().unloadAll();

    TerraLib::getInstance().finalize();
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

  return EXIT_SUCCESS;
}

void PrintDataSourceNames(const std::string& dsType, const std::map<std::string, std::string>& info)
{
  std::cout << "===== Data Source Names available: \n";

  std::vector<std::string> dataSourceNames = te::da::DataSource::getDataSourceNames(dsType, info);
  for(std::size_t i = 0; i < dataSourceNames.size(); ++i)
    std::cout << dataSourceNames[i] << std::endl;
}

std::auto_ptr<te::da::DataSource> CreateDataSource(const std::string& dsType, const std::map<std::string, std::string>& info)
{
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSource::create(dsType, info);

  return ds;
}

void DropDataSource(const std::string& dsType, const std::map<std::string, std::string>& info)
{
  te::da::DataSource::drop(dsType, info);
}

bool CheckDataSourceExistence(const std::string& dsType, const std::map<std::string, std::string>& info)
{
  return te::da::DataSource::exists(dsType, info);
}

void PrintDataSourceEncodings(const std::string& dsType, const std::map<std::string, std::string>& info)
{
  std::cout << "\n===== Encodings for the data source \"" << "terralib4" << "\":\n";

  std::vector<te::common::CharEncoding> encs = te::da::DataSource::getEncodings(dsType, info);
  for(std::size_t i = 0; i < encs.size(); ++i)
    std::cout << encs[i] << std::endl;
}

void PrintDataSetNames(te::da::DataSource* ds)
{
  // Get the database name
  const std::map<std::string, std::string>& connInfo = ds->getConnectionInfo();
  std::string dbName = connInfo.find("PG_DB_NAME")->second;

  std::cout << "\n===== Dataset Names in the data source \"" << dbName << "\":\n";

  std::vector<std::string> datasetNames = ds->getDataSetNames();
  for(std::size_t i = 0; i < datasetNames.size(); ++i)
    std::cout << datasetNames[i] << std::endl;
}

void PrintDataSetPropertyNames(te::da::DataSource* ds, const std::string& datasetName)
{
  std::cout << "\n===== Property Names of the dataset \"" << datasetName << "\":\n";

  std::vector<std::string> pNames = ds->getPropertyNames(datasetName);
  for(std::size_t i = 0; i < pNames.size(); ++i)
    std::cout << pNames[i] << std::endl;
}

void PrintDataSetConstraints(te::da::DataSource* ds, const std::string& datasetName)
{
  std::cout << "\n===== Primary Key Name of the dataset \"" << datasetName << "\": ";

  std::auto_ptr<te::da::PrimaryKey> pk = ds->getPrimaryKey(datasetName);
  std::cout << pk->getName() << std::endl;

  std::cout << "\n===== Property Names of the Primary Key \"" << pk->getName() << "\": ";

  const std::vector<te::dt::Property*> pkProperties = pk->getProperties();
  std::size_t numPkProperties = pkProperties.size();
  for(std::size_t i = 0; i < numPkProperties; ++i)
    std::cout << pkProperties[i]->getName() << std::endl;
}

void UnloadModules()
{
  te::plugin::PluginManager::getInstance().unloadAll();

  TerraLib::getInstance().finalize();
}