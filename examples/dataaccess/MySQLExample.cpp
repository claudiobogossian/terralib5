// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/memory.h>


// STL
#include <iostream>

#include <time.h>

std::string DATASOURCE_NAME = "";

void createDataSource()
{
  time_t t;
  t = time(NULL);

  DATASOURCE_NAME = "teste"+ boost::lexical_cast<int>(t);
  
  std::map<std::string, std::string> dsInfo;
  dsInfo["MY_HOST_NAME"] = "localhost";
  dsInfo["MY_USER_NAME"] = "root";
  dsInfo["MY_PASSWORD"] = "admin";
  dsInfo["MY_SCHEMA_NAME"] = "test";  
  dsInfo["MY_PORT"] = "3306";
  dsInfo["MY_OPT_CONNECT_TIMEOUT"] = "4";  
  dsInfo["MY_CREATE_OGC_METADATA_TABLES"] = "FALSE";
  dsInfo["MY_CREATE_TERRALIB_RASTER_METADATA_TABLES"] = "FALSE";

  dsInfo["MY_NEW_SCHEMA_NAME"] = DATASOURCE_NAME;
  dsInfo["MY_NEW_SCHEMA_CREATE_OGC_METADATA_TABLES"] = "TRUE";

  std::auto_ptr<te::da::DataSource> ds(te::da::DataSource::create("MYSQL", dsInfo));
}

void copyFromOGR()
{
#if TE_EXAMPLE_USE_OGR
  if(DATASOURCE_NAME == "")
    createDataSource();

  std::string ogrInfo("connection_string="TE_DATA_EXAMPLE_LOCALE"/data/shp");

  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("OGR"));
  ds->setConnectionStr(ogrInfo);
  ds->open();

  std::auto_ptr<te::da::DataSourceTransactor> originTransactor(ds->getTransactor());

  std::auto_ptr<te::da::DataSourceCatalogLoader> originLoader(originTransactor->getCatalogLoader());

  originLoader->loadCatalog();

  std::auto_ptr<te::da::DataSetType> originDt(originLoader->getDataSetType("munic_2001"));

  std::auto_ptr<te::da::DataSet> originDset(originTransactor->getDataSet(originDt->getName()));

  std::map<std::string, std::string> dsInfo;
  dsInfo["MY_HOST_NAME"] = "localhost";
  dsInfo["MY_USER_NAME"] = "root";
  dsInfo["MY_PASSWORD"] = "admin";
  dsInfo["MY_SCHEMA"] = DATASOURCE_NAME;
  dsInfo["MY_PORT"] = "3306";
  dsInfo["MY_OPT_CONNECT_TIMEOUT"] = "4";
  dsInfo["MY_CLIENT_MULTI_RESULTS"] = "TRUE";
  dsInfo["MY_CLIENT_MULTI_STATEMENTS"] = "TRUE";
  dsInfo["MY_OPT_CHARSET_NAME"] = "latin1";

  
  std::auto_ptr<te::da::DataSource> destinationDs(te::da::DataSourceFactory::open("MYSQL", dsInfo));

  std::auto_ptr<te::da::DataSourceTransactor> destinationTransactor(destinationDs->getTransactor());

  std::auto_ptr<te::da::DataSetPersistence> destinationDsp(destinationTransactor->getDataSetPersistence());

  std::map<std::string, std::string> op;
  op["ADD_TYPE"] = "T3";

  originDset->moveNext();

  destinationDsp->create(originDt.get(), originDset.get(), op);
        
#endif
}

void createSpatialTables()
{
  std::map<std::string, std::string> dsInfo;
  dsInfo["MY_HOST_NAME"] = "localhost";
  dsInfo["MY_USER_NAME"] = "root";
  dsInfo["MY_PASSWORD"] = "admin";
  dsInfo["MY_SCHEMA"] = DATASOURCE_NAME;
  dsInfo["MY_PORT"] = "3306"; 
  dsInfo["MY_OPT_CONNECT_TIMEOUT"] = "4";
  
  std::auto_ptr<te::da::DataSource> destinationDs(te::da::DataSourceFactory::open("MYSQL", dsInfo));

  te::da::DataSourceTransactor* t = destinationDs->getTransactor();
  te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();

  te::da::DataSetType* polDt = new te::da::DataSetType("poligonos");

  te::dt::SimpleProperty* polProp1 = new te::dt::SimpleProperty("id", te::dt::INT32_TYPE);
  te::gm::GeometryProperty* polProp2 = new te::gm::GeometryProperty("the_geom", 4326, te::gm::PolygonType, true);

  polDt->add(polProp1);
  polDt->add(polProp2);
  polDt->setDefaultGeomProperty(polProp2);

  t->getDataSetTypePersistence()->create(polDt);

  /////////////////////////////////////////////////////////////////////////////////////

  te::da::DataSetType* pointDt = new te::da::DataSetType("pontos");

  te::dt::SimpleProperty* pointProp1 = new te::dt::SimpleProperty("id", te::dt::INT32_TYPE);
  te::gm::GeometryProperty* pointProp2 = new te::gm::GeometryProperty("the_geom", 4326, te::gm::PointType, true);

  pointDt->add(pointProp1);
  pointDt->add(pointProp2);
  pointDt->setDefaultGeomProperty(pointProp2);

  t->getDataSetTypePersistence()->create(pointDt);

  //////////////////////////////////////////////////////////////////////////////////////

  te::da::DataSetType* lineDt = new te::da::DataSetType("linhas");

  te::dt::SimpleProperty* lineProp1 = new te::dt::SimpleProperty("id", te::dt::INT32_TYPE);
  te::gm::GeometryProperty* lineProp2 = new te::gm::GeometryProperty("the_geom", 4326, te::gm::LineStringType, true);

  lineDt->add(lineProp1);
  lineDt->add(lineProp2);
  lineDt->setDefaultGeomProperty(lineProp2);

  t->getDataSetTypePersistence()->create(lineDt);
}

void populateSpatialTables()
{

  std::map<std::string, std::string> op;
  op["ADD_TYPE"] = "T3";

  std::map<std::string, std::string> dsInfo;
  dsInfo["MY_HOST_NAME"] = "localhost";
  dsInfo["MY_USER_NAME"] = "root";
  dsInfo["MY_PASSWORD"] = "admin";
  dsInfo["MY_SCHEMA"] = DATASOURCE_NAME;
  dsInfo["MY_PORT"] = "3306"; 
  dsInfo["MY_OPT_CONNECT_TIMEOUT"] = "4";
  
  std::auto_ptr<te::da::DataSource> destinationDs(te::da::DataSourceFactory::open("MYSQL", dsInfo));

  te::da::DataSourceTransactor* t = destinationDs->getTransactor();
  te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();  

  ////////////////////////////////////////////////////////////////////////////////////////

  te::gm::LinearRing* s = new te::gm::LinearRing(5, te::gm::LineStringType);
  s->setPoint(0, -51, -15.5);
  s->setPoint(1, -49.5, -15.5);
  s->setPoint(2, -49.5, -15);
  s->setPoint(3, -51, -15);
  s->setPoint(4, -51, -15.5);

  te::gm::LinearRing* s2 = new te::gm::LinearRing(5, te::gm::LineStringType);
  s2->setPoint(0, -41, -5.5);
  s2->setPoint(1, -39.5, -5.5);
  s2->setPoint(2, -39.5, -5);
  s2->setPoint(3, -41, -5);
  s2->setPoint(4, -41, -5.5);

  te::gm::Polygon* pol = new te::gm::Polygon(0, te::gm::PolygonType);
  pol->push_back(s);
  pol->setSRID(4326); //srid =  4326

  te::gm::Polygon* pol2 = new te::gm::Polygon(0, te::gm::PolygonType);
  pol2->push_back(s2);
  pol2->setSRID(4326); //srid =  4326  

  te::da::DataSetType* polDt = loader->getDataSetType("poligonos");

  te::mem::DataSet* polDs = new te::mem::DataSet(polDt);

  te::mem::DataSetItem* polItem1 = new te::mem::DataSetItem(polDs);
  polItem1->setInt32("id", 1);
  polItem1->setGeometry("the_geom", *pol);

  te::mem::DataSetItem* polItem2 = new te::mem::DataSetItem(polDs);
  polItem2->setInt32("id", 2);
  polItem2->setGeometry("the_geom", *pol2);

  polDs->add(polItem1);
  polDs->add(polItem2);

  polDs->moveNext();

  t->getDataSetPersistence()->add(polDt, polDs, op);

  ///////////////////////////////////////////////////////////////////////////////////////

  te::gm::Point* p1 = new te::gm::Point(-51, -15.5, 4326);
  te::gm::Point* p2 = new te::gm::Point(-49.5, -15, 4326);
  
  te::da::DataSetType* pointDt = loader->getDataSetType("pontos");

  te::mem::DataSet* pointDs = new te::mem::DataSet(pointDt);

  te::mem::DataSetItem* pointItem1 = new te::mem::DataSetItem(pointDs);
  pointItem1->setInt32("id", 1);
  pointItem1->setGeometry("the_geom", *p1);

  te::mem::DataSetItem* pointItem2 = new te::mem::DataSetItem(pointDs);
  pointItem2->setInt32("id", 2);
  pointItem2->setGeometry("the_geom", *p2);

  pointDs->add(pointItem1);
  pointDs->add(pointItem2);

  pointDs->moveNext();

  t->getDataSetPersistence()->add(pointDt, pointDs, op);

  //////////////////////////////////////////////////////////////

  te::gm::LineString* l1 = new te::gm::LineString(3, te::gm::LineStringType);
  l1->setPoint(0, -51, -15.5);
  l1->setPoint(1, -49.5, -15.5);
  l1->setPoint(2, -49.5, -15);

  te::gm::LineString* l2 = new te::gm::LineString(3, te::gm::LineStringType);
  l2->setPoint(0, -41, -5.5);
  l2->setPoint(1, -39.5, -5.5);
  l2->setPoint(2, -39.5, -5);
  
  te::da::DataSetType* lineDt = loader->getDataSetType("linhas");

  te::mem::DataSet* lineDs = new te::mem::DataSet(lineDt);

  te::mem::DataSetItem* lineItem1 = new te::mem::DataSetItem(lineDs);
  lineItem1->setInt32("id", 1);
  lineItem1->setGeometry("the_geom", *l1);

  te::mem::DataSetItem* lineItem2 = new te::mem::DataSetItem(lineDs);
  lineItem2->setInt32("id", 2);
  lineItem2->setGeometry("the_geom", *l2);

  lineDs->add(lineItem1);
  lineDs->add(lineItem2);

  lineDs->moveNext();

  t->getDataSetPersistence()->add(lineDt, lineDs, op);
}

void createInsertExample()
{
  if(DATASOURCE_NAME == "")
    createDataSource();

  createSpatialTables();
  populateSpatialTables();
}

void readExample()
{
  std::map<std::string, std::string> dsInfo;
  dsInfo["MY_HOST_NAME"] = "localhost";
  dsInfo["MY_USER_NAME"] = "root";
  dsInfo["MY_PASSWORD"] = "admin";
  dsInfo["MY_SCHEMA"] = DATASOURCE_NAME;
  dsInfo["MY_PORT"] = "3306"; 
  dsInfo["MY_OPT_CONNECT_TIMEOUT"] = "4";
  dsInfo["MY_MAX_POOL_SIZE"] = "10";
  
  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::open("MYSQL", dsInfo));

  te::da::DataSourceTransactor* t = ds->getTransactor();
  te::da::DataSourceCatalogLoader* loader = t->getCatalogLoader();  

  loader->loadCatalog();

  PrintCatalog(ds.get());

  PrintDataSets(ds.get());

  PrintDataSourceCapabilities(ds.get());
  
}

void MySQLExample()
{
#if TE_EXAMPLE_USE_MYSQL
  try
  {   
    //createInsertExample();
    //readExample();
    copyFromOGR();
    

  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in MySQL Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in MySQL Example!" << std::endl;
  }
#endif
}