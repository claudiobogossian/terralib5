/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/plugin.h>
#include <terralib/dataaccess2/dataset/CheckConstraint.h>
#include <terralib/dataaccess2/dataset/PrimaryKey.h>
#include <terralib/dataaccess2/dataset/Index.h>
#include <terralib/datatype/SimpleProperty.h>
#include <terralib/postgis2/DataSource.h>
#include <terralib/postgis2/DataSourceFactory.h>
#include <terralib/postgis2/PreparedQuery.h>

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

void PrintDataSetNames(te::da::DataSource* ds);
void PrintDataSetPropertyNames(te::da::DataSource* ds, const std::string& datasetName);
void PrintDataSetConstraints(te::da::DataSource* ds, const std::string& datasetName);

//const te::da::DataSetTypePtr& PrintSchema(te::da::DataSource* ds, const std::string& datasetName);

void LoadModules();

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    // Initialize the Terralib support
    TerraLib::getInstance().initialize();

    LoadModules();

    std::map<std::string, std::string> connInfo;

    //connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;
    //connInfo["PG_HOST"] = "localhost" ;
    connInfo["PG_PORT"] = "5433" ;
    connInfo["PG_USER"] = "postgres";
    connInfo["PG_PASSWORD"] = "sitim110";
    connInfo["PG_DB_NAME"] = "terralib4";
    //connInfo["PG_DB_NAME"] = "Northwind";
    connInfo["PG_CONNECT_TIMEOUT"] = "4";

    std::string dsType = "POSTGIS";

    PrintDataSourceNames(dsType, connInfo);

    // Creation of a data source
    connInfo["PG_NEWDB_NAME"] = "new_dbname";

    std::auto_ptr<te::da::DataSource> newds = CreateDataSource(dsType, connInfo);

    // Drop a data source
    connInfo["PG_DB_TO_DROP"] = "new_dbname";
    DropDataSource(dsType, connInfo);

    // Check the data source existence
    connInfo["PG_CHECK_DB_EXISTENCE"] = "terralib4";
    bool dsExists = CheckDataSourceExistence(dsType, connInfo);

    if(dsExists)
      std::cout << "\nThe data source \"terralib4\" exists!\n";
    else
      std::cout << "\nThe data source \"terralib4\" doesn't exist!\n";

    // Connection to a data source
    te::da::DataSource* ds = te::da::DataSourceFactory::make("POSTGIS");

    // Open the data source using the connection info above
    ds->setConnectionInfo(connInfo);
    ds->open();

    PrintDataSetNames(ds);

    std::string datasetName = "public.br_munic_2001";

    PrintDataSetPropertyNames(ds, datasetName);

    PrintDataSetConstraints(ds, datasetName);

    const te::da::DataSetTypePtr& dt = ds->getDataSetType(datasetName);

    te::da::PreparedQuery* pq = new te::pgis::PreparedQuery(ds,  "testepq");
    delete pq;

    ds->close();

    delete ds;

    te::plugin::PluginManager::getInstance().unloadAll();

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried: " << e.what() << std::endl;

    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();
    
    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried!" << std::endl;

    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();
    
    return EXIT_FAILURE;
  }

  std::cout << "\nPress Enter to exit..." << std::endl;
  std::cin.get();
  
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

  te::da::PrimaryKey* pk = ds->getPrimaryKey(datasetName);
  std::cout << pk->getName() << std::endl;

  std::cout << "\n===== Property Names of the Primary Key \"" << pk->getName() << "\": ";

  const std::vector<te::dt::Property*> pkProperties = pk->getProperties();
  std::size_t numPkProperties = pkProperties.size();
  for(std::size_t i = 0; i < numPkProperties; ++i)
    std::cout << pkProperties[i]->getName() << std::endl;
}




//int main(int /*argc*/, char** /*argv*/)
//{
//  try
//  {
//    // Initialize the Terralib support
//    TerraLib::getInstance().initialize();
//
//    LoadModules();
//
//    std::map<std::string, std::string> connInfo;
//
//    connInfo["PG_HOST"] = "localhost" ;
//    connInfo["PG_PORT"] = "5433" ;
//    connInfo["PG_USER"] = "postgres";
//    connInfo["PG_PASSWORD"] = "sitim110";
//    //connInfo["PG_DB_NAME"] = "Northwind";
//    //connInfo["PG_DB_NAME"] = "nyc";
//    connInfo["PG_DB_NAME"] = "testepgis";
//    connInfo["PG_CONNECT_TIMEOUT"] = "4";
//
//    // Create a data source
//    //te::da::DataSource* ds = new te::pgis::DataSource;
//
//    te::da::DataSource* ds = te::da::DataSourceFactory::make("POSTGIS");
//
//    // As we are going to use the data source, let's open it using the connection info above!
//    ds->setConnectionInfo(connInfo);
//    ds->open();
//
//    // Get the data source names available
//    PrintDataSourceNames(ds, connInfo);
//
//    // Get the dataset names of the data source nyc
//    PrintDataSetNames(ds);
//
//    //std::string datasetName = "public.categorias";
//    //std::string datasetName = "public.nyc_census_blocks";
//    std::string datasetName = "public.func";
//    //std::string datasetName = "public.detalhesdopedido";
//    //std::string datasetName = "public.testcc";
//    const te::da::DataSetTypePtr& dt = PrintSchema(ds, datasetName);
//
//    //te::da::CheckConstraint* cc = ds->getCheckConstraint(datasetName, "testcc_check");
//
//    te::da::CheckConstraint* newCC = new te::da::CheckConstraint("newCheck1");
//    newCC->setExpression("CHECK a > b");
//    ds->addCheckConstraint(datasetName, newCC);
//
//    ds->dropCheckConstraint(datasetName, "newCheck1");
//
//    std::cout << "===== Names of the sequences available: \n";
//    std::vector<std::string> seqNames = ds->getSequenceNames();
//    for(std::size_t i = 0; i < seqNames.size(); ++i)
//      std::cout << seqNames[i] << std::endl;
//
//    te::da::PreparedQuery* pq = new te::pgis::PreparedQuery(ds,  "testepq");
//    //std::auto_ptr<te::pgis::PreparedQuery> pq(new te::pgis::PreparedQuery(ds, "testepq"));
//
//    delete pq;
//
//    //te::dt::Property* p = new te::dt::SimpleProperty("idade", te::dt::INT16_TYPE);
//    //ds->addProperty(datasetName, p);
//
//    te::da::PrimaryKey* pk = new te::da::PrimaryKey("func_id2");
//
//    std::vector<te::dt::Property*> pVec;
//    pVec.push_back(ds->getProperty("public.func2", 0));
//    pk->setProperties(pVec);
//
//    ds->addPrimaryKey("public.func2", pk);
//
//    ds->close();
//
//    delete ds;
//
//    TerraLib::getInstance().finalize();
//
//    return 0;
//  }
//  catch(const std::exception& e)
//  {
//    std::cout << std::endl << "An exception has occurried: " << e.what() << std::endl;
//  }
//  catch(...)
//  {
//    std::cout << std::endl << "An unexpected exception has occurried!" << std::endl;
//  }
//}
//

//
//const te::da::DataSetTypePtr& PrintSchema(te::da::DataSource* ds, const std::string& datasetName)
//{
//  const te::da::DataSetTypePtr& dt = ds->getDataSetType(datasetName);
//
//  std::cout << "\n===== Names of the dataset properties: \n";
//  std::vector<te::dt::Property*> properties = dt->getProperties();
//  for(std::size_t i = 0; i < properties.size(); ++i)
//    std::cout << properties[i]->getName() << std::endl;
//
//  std::cout << "\n===== Names and expressions of the dataset check constraints: \n";
//  std::size_t numCC = dt->getNumberOfCheckConstraints();
//  for(std::size_t i = 0; i < numCC; ++i)
//  {
//    te::da::CheckConstraint* cc = dt->getCheckConstraint(i);
//    std::cout << cc->getName() << ": " << cc->getExpression() << std::endl;
//  }
//
//  //te::da::CheckConstraint* cc = ds->getCheckConstraint("public.testcc", "testcc_check");
//
//  std::cout << "\n===== Name of the dataset primary key: ";
//  te::da::PrimaryKey* pk = ds->getPrimaryKey(datasetName);
//  std::cout <<pk->getName() << std::endl;
//
//  std::cout << "\n===== Name of the properties of the primary key: \n";
//  const std::vector<te::dt::Property*> pkProperties = pk->getProperties();
//  std::size_t numPkProperties = pkProperties.size();
//  for(std::size_t i = 0; i < numPkProperties; ++i)
//    std::cout << pkProperties[i]->getName() << std::endl;
//
//  std::cout << "\n===== Names of the dataset indexes: \n";
//  std::size_t numIndexes = dt->getNumberOfIndexes();
//  for(std::size_t i = 0; i < numIndexes; ++i)
//    std::cout << dt->getIndex(i)->getName() << std::endl;
//
//  return dt;
//}
