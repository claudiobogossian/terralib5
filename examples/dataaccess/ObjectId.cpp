// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// STL
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

void ObjectId2()
{
  // let's give the minimal server connection information needed to connect to the database server
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "";
  connInfo["PG_DB_NAME"] = "terralib5";
  connInfo["PG_CONNECT_TIMEOUT"] = "4";
  
  // create a data source using the data source factory
  te::da::DataSource* ds = te::da::DataSourceFactory::make("POSTGIS");
  
  // as we are going to use the data source, let's open it using the connection info above!
  ds->open(connInfo);
  
  // get a transactor to interact to the data source in the next examples
  te::da::DataSourceTransactor* transactor = ds->getTransactor();
  
  // now we will retrieve all cities that intersects a given box
  te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);
  
  te::da::DataSet* dataset = transactor->getDataSet("munic_2001", &box, te::gm::INTERSECTS);
  assert(dataset);
  std::cout << "DataSet size: " << dataset->size() << std::endl;
  
  // let's generate the oid's
  te::da::ObjectIdSet* oids = 0/*te::da::GenerateOIDSet(dataset)*/;
  assert(oids);
  
  // let's retrieve the identified elements via transactor
  te::da::DataSet* identified = 0/*transactor->getDataSet(oids)*/;
  
  assert(identified);
  assert(dataset->size() == identified->size());
  
  std::cout << "== DataSet Retrieved From Box == " << std::endl;
  PrintDataSet("munic_2001", dataset);
  
  std::cout << "== DataSet Retrieved From ObjectIdSet == " << std::endl;
  PrintDataSet("munic_2001_identified",identified);
  
  // Cleaning All!
  delete identified;
  delete oids;
  delete dataset;
  delete transactor;
  delete ds;
}

void ObjectId()
{
// let's give the minimal server connection information needed to connect to the database server
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "";
  connInfo["PG_DB_NAME"] = "terralib5";
  connInfo["PG_CONNECT_TIMEOUT"] = "4";

// create a data source using the data source factory
  te::da::DataSource* ds = te::da::DataSourceFactory::make("POSTGIS");

// as we are going to use the data source, let's open it using the connection info above!
  ds->open(connInfo);

// get a transactor to interact to the data source in the next examples
  te::da::DataSourceTransactor* transactor = ds->getTransactor();

// now we will retrieve all cities that intersects a given box
  te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);

  te::da::DataSet* dataset = transactor->getDataSet("munic_2001", &box, te::gm::INTERSECTS);
  assert(dataset);
  std::cout << "DataSet size: " << dataset->size() << std::endl;

// let's generate the oid's
  te::da::ObjectIdSet* oids = 0/*te::da::GenerateOIDSet(dataset)*/;
  assert(oids);
  std::cout << "OIDs size: " << oids->size() << std::endl;

  dataset->moveBeforeFirst();

  assert(dataset->size() == oids->size());
  //assert(oids->isFromPrimaryKey());

// let's get the Select from the generated OIDS
  te::da::Select* select = 0/*oids->getQuery()*/;
  assert(select);

// let's print the SQL
  std::string sql;
  te::da::SQLVisitor sqlConverter(*ds->getDialect(), sql);
  select->accept(sqlConverter);

  std::cout << "The SQL is: " << sql << std::endl;

// let's retrieve the identified elements
  te::da::DataSet* identified = transactor->query(sql);

  assert(identified);
  assert(dataset->size() == identified->size());

  std::cout << "== DataSet Retrieved From Box == " << std::endl;
  PrintDataSet("munic_2001", dataset);

  std::cout << "== DataSet Retrieved From ObjectIdSet == " << std::endl;
  PrintDataSet("munic_2001_identified", identified);

  // Cleaning All!
  delete identified;
  delete select;
  delete oids;
  delete dataset;
  delete transactor;
  delete ds;

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();
}
