// Examples
#include "DataAccessExamples.h"

// TerraLib
#include "../Config.h"
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// STL
#include <iostream>

void CopyingData()
{
  try
  {
// let's take the input dataset from a shape file
  std::string data_dir = TERRALIB_EXAMPLES_DATA_DIR;
  
  std::string ogrInfo("connection_string=" + data_dir + "/shp/munic_2001.shp");
 
  std::map<std::string, std::string> connInfo;
  std::auto_ptr<te::da::DataSource> dsOrigin = te::da::DataSourceFactory::make("OGR");
  
  connInfo["URI"] = data_dir + "/shp/munic_2001.shp";

  dsOrigin->setConnectionInfo(connInfo);
  dsOrigin->open();

// get a transactor to interact to the data source origin
  std::auto_ptr<te::da::DataSourceTransactor> tOrigin = dsOrigin->getTransactor();

  std::vector<std::string> datasets = tOrigin->getDataSetNames();

  std::auto_ptr<te::da::DataSet> datasetOrigin = tOrigin->getDataSet(datasets[0]);
  std::auto_ptr<te::da::DataSetType> dtOrigin = tOrigin->getDataSetType(datasets[0]);

// let's open the destination datasource
  std::map<std::string, std::string> pgisInfo;
  pgisInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
  pgisInfo["PG_PORT"] = "5433" ;
  pgisInfo["PG_USER"] = "postgres";
  pgisInfo["PG_PASSWORD"] = "postgres";
  pgisInfo["PG_DB_NAME"] = "terralib4";
  pgisInfo["PG_CONNECT_TIMEOUT"] = "4";
  pgisInfo["PG_CONNECT_TIMEOUT"] = "4";
  pgisInfo["PG_CLIENT_ENCODING"] = "WIN1252";

  std::auto_ptr<te::da::DataSource> dsDestination = te::da::DataSourceFactory::make("POSTGIS");
  dsDestination->setConnectionInfo(pgisInfo);
  dsDestination->open();

// get a transactor to interact to the data source
  std::auto_ptr<te::da::DataSourceTransactor> tDestination = dsDestination->getTransactor();

// create and save datasettype in the datasource destination
  te::da::DataSetType* newDataSet = static_cast<te::da::DataSetType*>(dtOrigin->clone());
  newDataSet->setName("public.new_tab_munic_2001_from_shp");
  GetFirstGeomProperty(newDataSet)->setSRID(4326);
  GetFirstGeomProperty(newDataSet)->setGeometryType(te::gm::GeometryType);

  std::cout << std::endl << "starting copy..." << std::endl;
  std::map<std::string, std::string> options;

  tDestination->begin();
  tDestination->createDataSet(newDataSet,options);
  tDestination->add(newDataSet->getName(), datasetOrigin.get(),options);
  tDestination->commit();
  std::cout << std::endl << "C opy finished..." << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in the Copy Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in the Copy Example!" << std::endl;
  }
}
