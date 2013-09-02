// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess2/dataset/CheckConstraint.h>
#include <terralib/dataaccess2/dataset/CheckConstraint.h>
#include <terralib/dataaccess2/dataset/PrimaryKey.h>
#include <terralib/dataaccess2/dataset/Index.h>
#include <terralib/dataaccess2/datasource/DataSourceFactory.h>
#include <terralib/postgis2/DataSource.h>
#include <terralib/postgis2/DataSourceFactory.h>
#include <terralib/postgis2/PreparedQuery.h>

//
// STL
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

void ObjectId3()
{
  // let's give the minimal server connection information needed to connect to the database server
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "terralib4";
  connInfo["PG_CONNECT_TIMEOUT"] = "4";
  
  // create a data source using the data source factory and set connection info
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS");
  ds->setConnectionInfo(connInfo);

  // as we are going to use the data source, let's open it using the connection info above!
  ds->open();
  
  // get a transactor to interact to the data source in the next examples
  std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();
  assert(transactor.get());
  // now we will retrieve all cities that intersects a given box
  te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);
  
  //std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet("public.br_munic_2001", &box, te::gm::INTERSECTS);
  std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet("public.br_munic_2001", "geom", &box, te::gm::INTERSECTS);
 
  assert(dataset.get());
  std::cout << "DataSet size: " << dataset->size() << std::endl;

  // let´s generate the oids using the primary key and then 
  // test the getDataSet("public.br_munic_2001",oids) using the oids
  std::vector<std::string> pk_name;
  pk_name.push_back("gid");
  te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataset.get(), pk_name);
  
  assert(oids);
  
  // let's retrieve the identified elements via transactor
  std::auto_ptr<te::da::DataSet> identified = transactor->getDataSet("public.br_munic_2001",oids);
  //transactor->getDataSetNames();
  assert(identified.get());
  assert(dataset->size() == identified->size());
  bool ini = identified->moveBeforeFirst();
  bool ini1 = dataset->moveBeforeFirst(); //otherwise if will be at the end of the dataset and nothing will be printed
 
  std::cout << "== DataSet Retrieved From Box == " << std::endl;
  PrintDataSet("public.br_munic_2001", dataset.get());
  
  std::cout << "== DataSet Retrieved From ObjectIdSet knowing pk == " << std::endl;
  PrintDataSet("munic_2001_identified",identified.get());

  // Another way to get oids 
  // get a catalogloader and load the dataSetType information in order to find out the pk, uk etc...
  //std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  //cl.reset(transactor-> getCatalogLoader());
  //cl.get();

  bool ini2 = dataset->moveBeforeFirst();
  te::da::DataSetType* dt1 =  (transactor->getDataSetType("public.br_munic_2001")).get();
  te::da::ObjectIdSet* oids1 = te::da::GenerateOIDSet( dataset.get(), dt1);
  std::auto_ptr<te::da::DataSet> identified1 = transactor->getDataSet("public.br_munic_2001",oids1);

  
  std::cout << "== DataSet Retrieved From ObjectIdSet using dataSetType to discover pk == " << std::endl;
  PrintDataSet("munic_2001_identified",identified1.get());


// Another test: Not knowing primary key - do not use this way...all property names will be used in oids including geom  
  bool ini3 = dataset->moveBeforeFirst();

  std::vector<std::string> pnames;
  std::vector<int> ptypes;
  te::da::GetPropertyInfo(dataset.get(), pnames,ptypes); //here I do not have pk, uk, etc information.
  //te::da::ObjectIdSet* oids2 = te::da::GenerateOIDSet(dataset.get(), pnames);

  //std::auto_ptr<te::da::DataSet> identified2 = transactor->getDataSet("public.br_munic_2001",oids2);
  std::cout << "== DataSet Retrieved From ObjectIdSet == " << std::endl;
//CAI ao chamar std::auto_ptr<te::da::DataSet> te::pgis::DataSourceTransactor::query(const std::string& query, te::common::TraverseType travType, bool isConnected)
  //PrintDataSet("munic_2001_identified",identified2.get());
  
  // Cleaning All! - Unecessary...they are all auto_ptr
  //delete identified;
  //delete oids;
  //delete dataset;
  //delete transactor;
  //delete ds;
}

////void ObjectId2()
////{
////  // let's give the minimal server connection information needed to connect to the database server
////  std::map<std::string, std::string> connInfo;
////  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;
////  connInfo["PG_USER"] = "postgres";
////  connInfo["PG_PASSWORD"] = "sitim110";
////  connInfo["PG_DB_NAME"] = "terralib4";
////  connInfo["PG_CONNECT_TIMEOUT"] = "4";
////  
////  // create a data source using the data source factory
////  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS");
////  ds->setConnectionInfo(connInfo);
////  // as we are going to use the data source, let's open it using the connection info above!
////  ds->open();
////  
////  // get a transactor to interact to the data source in the next examples
////  std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();
////  
////  // now we will retrieve all cities that intersects a given box
////  te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);
////  
////  te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001", &box, te::gm::INTERSECTS);
////  assert(dataset);
////  std::cout << "DataSet size: " << dataset->size() << std::endl;
////
////  // get a catalogloader and use  getdataSetType information using "true" parameters to get pk, uk,etc
////  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
////  cl.reset(transactor->getCatalogLoader());
////  cl.get();
////
////  std::vector<std::string> pnames;
////  std::vector<int> ptypes;
////
////  te::da::DataSetType *dt =  cl->getDataSetType("public.br_munic_2001", true);
////  te::da::GetPropertyInfo(dt,pnames,ptypes);  //cl must be used to get the information about pk, uk, etc...
////  te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataset, dt);
////
////  //te::da::GetPropertyInfo(dataset, pnames,ptypes); //here I do not have pk, uk, etc information.
////  
////  std::vector<std::string> vgids;
////  vgids.push_back("gid");
////
////  // let's generate the oid's
////  //te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataset, pnames);
////
////  //te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataset, vgids);
////  assert(oids);
////  
////  // let's retrieve the identified elements via transactor
////  te::da::DataSet* identified = transactor->getDataSet("public.br_munic_2001",oids);
////  
////  assert(identified);
////  assert(dataset->size() == identified->size());
////  bool ini = identified->moveBeforeFirst();
////  bool ini1 = dataset->moveBeforeFirst(); //otherwise if will be at the end of the dataset and nothing will be printed
////
////  
////  std::cout << "== DataSet Retrieved From Box == " << std::endl;
////  PrintDataSet("public.br_munic_2001", dataset);
////  
////  std::cout << "== DataSet Retrieved From ObjectIdSet == " << std::endl;
////  PrintDataSet("munic_2001_identified",identified);
////  
////  // Cleaning All!
////  delete identified;
////  delete oids;
////  delete dataset;
////  delete transactor;
////  delete ds;
////}
////
////void ObjectId()
////{
////// let's give the minimal server connection information needed to connect to the database server
////  std::map<std::string, std::string> connInfo;
////  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;
////  connInfo["PG_USER"] = "postgres";
////  connInfo["PG_PASSWORD"] = "sitim110";
////  connInfo["PG_DB_NAME"] = "terralib4";
////  connInfo["PG_CONNECT_TIMEOUT"] = "4";
////
////// create a data source using the data source factory
////  te::da::DataSource* ds = te::da::DataSourceFactory::make("POSTGIS");
////
////// as we are going to use the data source, let's open it using the connection info above!
////  ds->open(connInfo);
////
////// get a transactor to interact to the data source in the next examples
////  te::da::DataSourceTransactor* transactor = ds->getTransactor();
////
////// now we will retrieve all cities that intersects a given box
////  te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);
////
////  te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001", &box, te::gm::INTERSECTS);
////  assert(dataset);
////  std::cout << "DataSet size: " << dataset->size() << std::endl;
////  
////  std::vector<std::string> pnames;
////  std::vector<int> ptypes;
////  te::da::GetPropertyInfo(dataset, pnames,ptypes);
////
////// let's generate the oid's
////  te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataset, pnames);
////  assert(oids);
////  std::cout << "OIDs size: " << oids->size() << std::endl;
////
////  dataset->moveBeforeFirst();
////
////  assert(dataset->size() == oids->size());
////  //assert(oids->isFromPrimaryKey());
////
////// let's get the Select from the generated OIDS  //?? get expression 
////  te::da::Expression* select = oids->getExpression();
////  assert(select);
////  //te::da::Select* select = oids->getQuery();
////  //assert(select);
////
////// let's print the SQL
////  std::string sql;
////  te::da::SQLVisitor sqlConverter(*ds->getDialect(), sql);
////  select->accept(sqlConverter);
////
////  std::cout << "The SQL is: " << sql << std::endl;
////
////// let's retrieve the identified elements
////  te::da::DataSet* identified = transactor->query(sql);
////
////  assert(identified);
////  assert(dataset->size() == identified->size());
////
////  std::cout << "== DataSet Retrieved From Box == " << std::endl;
////  PrintDataSet("br_munic_2001", dataset);
////
////  std::cout << "== DataSet Retrieved From ObjectIdSet == " << std::endl;
////  PrintDataSet("br_munic_2001_identified", identified);
////
////  // Cleaning All!
////  delete identified;
////  delete select;
////  delete oids;
////  delete dataset;
////  delete transactor;
////  delete ds;
////
////  std::cout << "Press Enter to exit..." << std::endl;
////  std::cin.get();
////}
