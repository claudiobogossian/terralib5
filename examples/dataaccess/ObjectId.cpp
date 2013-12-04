// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess/dataset/CheckConstraint.h>
#include <terralib/dataaccess/dataset/CheckConstraint.h>
#include <terralib/dataaccess/dataset/PrimaryKey.h>
#include <terralib/dataaccess/dataset/Index.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/postgis/DataSource.h>
#include <terralib/postgis/DataSourceFactory.h>
#include <terralib/postgis/PreparedQuery.h>

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
  
  std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet("public.br_munic_2001", "geom", &box, te::gm::INTERSECTS);
 
  assert(dataset.get());
  std::cout << "DataSet size: " << dataset->size() << std::endl;

// let�s generate the oids using the primary key and then 
// test the getDataSet("public.br_munic_2001",oids) using the oids
  std::vector<std::string> pk_name;
  pk_name.push_back("gid");
  te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataset.get(), pk_name);
  
  assert(oids);
  
// let's retrieve the identified elements via transactor
  std::auto_ptr<te::da::DataSet> identified = transactor->getDataSet("public.br_munic_2001",oids);
  assert(identified.get());
  assert(dataset->size() == identified->size());
  bool ini = identified->moveBeforeFirst();
  bool ini1 = dataset->moveBeforeFirst(); //otherwise if will be at the end of the dataset and nothing will be printed
 
  std::cout << "== DataSet Retrieved From Box == " << std::endl;
  PrintDataSet("public.br_munic_2001", dataset.get());
  
  std::cout << "== DataSet Retrieved From ObjectIdSet knowing pk == " << std::endl;
  PrintDataSet("munic_2001_identified",identified.get());

// Another way to get oids using DataSetType
  bool ini2 = dataset->moveBeforeFirst();
  std::auto_ptr<te::da::DataSetType> dt1 =  transactor->getDataSetType("public.br_munic_2001");
  te::da::ObjectIdSet* oids1 = te::da::GenerateOIDSet( dataset.get(), dt1.get());
  std::auto_ptr<te::da::DataSet> identified1 = transactor->getDataSet("public.br_munic_2001",oids1);
  
  std::cout << "== DataSet Retrieved From ObjectIdSet using dataSetType to discover pk == " << std::endl;
  PrintDataSet("munic_2001_identified",identified1.get());

// Another test: Not knowing primary key - using only property names ...all property names will be used in oids2 including geom com valores na clausula IN 

  bool ini3 = dataset->moveBeforeFirst();

  std::vector<std::string> pnames;
  std::vector<int> ptypes;
  te::da::GetPropertyInfo(dataset.get(), pnames,ptypes); //here I do not have pk, uk, etc information.
  te::da::ObjectIdSet* oids2 = te::da::GenerateOIDSet(dataset.get(), pnames);

//Exception ao chamar std::auto_ptr<te::da::DataSet> te::pgis::DataSourceTransactor::query(const std::string& query, te::common::TraverseType travType, bool isConnected)
//  +		query	"SELECT * FROM public.br_munic_2001 WHERE ((((((gid IN(224, 2261, 2815, 2992, 3009, 3957, 4062, 4151, 4470, 4617, 4777, 4861, 4871, 5151, 5370, 5490) and 
//                      geocodigo IN('3160900', '3106408', '3115409', '3118007', '3118304', '3131901', '3133907', '3135407', '3140001', '3142304', '3144805', '3145901', '3146107', '3150802', '3154804', '3157203')) 
//                      and nome IN('São Brás do Suaçuí', 'Belo Vale', 'Catas Altas da Noruega', 'Congonhas', 'Conselheiro Lafaiete', 'Itabirito', 'Itaverava', 'Jeceaba', 'Mariana', 'Moeda', 'Nova Lima', 'Ouro Branco', 'Ouro Preto', 'Piranga', 'Rio Acima', 'Santa Bárbara')) 
//                      and latitudese IN(-20.625, -20.408, -20.69, -20.5, -20.66, -20.253, -20.678, -20.535, -20.378, -20.333, -19.986, -20.521, -20.288, -20.685, -20.088, -19.959)) and longitudes IN(-43.949, -44.024, -43.498, -43.858, -43.786, -43.801, -43.61, -43.983, -43.416, -44.053, -43.847, -43.692, -43.508, -43.3, -43.789, -43.415)) and area_tot_g IN(110.442, 365.437, 143.366, 305.579, 369.544, 543.007, 282.642, 235.603, 1193.29, 154.228, 428.449, 260.766, 1245.11, 657.484, 230.143, 684.21)) and 
//                      geom IN(multipolygon(((-44.043626070683963 -20.625518962953148,-44.035956543992654 -20.6137272695028,-44.040894627202682 -20.610935304129885,-44.030423157019662 -20.607555592080438,-44.034594565956759 -20.603834458277216,-44.03028174271676 -20.597518888787956,-44.027262002099242 -20.598618621571813,-44.027984497034382 -20.594464438056299,-44.020931656130152 -20.588743876647605,-44.020427470109311 -20.582229409297053,-44.012005997033306 -20.571766055554225,-44.00812180401423 -20.57207662062044,-44.006533166711385 -20.565855212215798,-43.998957282003516 -20.566473008582363,-43.990245578310493 -20.576270563470167,-43.984456651636727 -20.577353913264371,-43.977009688319214 -20.590992312925078,-43.971534922546525 -20.588803705538183,-43.965311843722667 -20.597116249593959,-43.953031192532841 -20.593062927766542,-43.948946724344459 -20.595035721618,-43.94190444468957 -20.591647668727145,-43.940091092263039 -20.589002385091472,-43.931863545185"	const std::basic_string<char,std::char_traits<char>,std::allocator<char> > &

  //std::auto_ptr<te::da::DataSet> identified2 = transactor->getDataSet("public.br_munic_2001",oids2);
  //std::cout << "== DataSet Retrieved From ObjectIdSet == " << std::endl;
  //PrintDataSet("munic_2001_identified",identified2.get());
  
  // Cleaning All
  delete oids1;
  delete oids2; 
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
