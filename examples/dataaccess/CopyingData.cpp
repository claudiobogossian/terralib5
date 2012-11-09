// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// STL
#include <iostream>

void CopyingData()
{
  try
  {
// let's take the input dataset
  std::string ogrInfo("connection_string="TE_DATA_EXAMPLE_LOCALE"/data/shp/munic_2001.shp");

  te::da::DataSource* dsOrigin = te::da::DataSourceFactory::make("OGR");
  dsOrigin->setConnectionStr(ogrInfo);
  dsOrigin->open();

// get a transactor to interact to the data source origin
  te::da::DataSourceTransactor* tOrigin = dsOrigin->getTransactor();

// in order to access the datasource catalog you need a DataSourceCatalogLoader
  te::da::DataSourceCatalogLoader* clOrigin = tOrigin->getCatalogLoader();
    
// let's find out the information in the data source!
  clOrigin->loadCatalog(); //only the schemas

  boost::ptr_vector<std::string> datasets;

  clOrigin->getDataSets(datasets);

  te::da::DataSet* datasetOrigin = tOrigin->getDataSet(datasets[0]);
  te::da::DataSetType* dtOrigin = datasetOrigin->getType();

// let's open the destination datasource
  std::map<std::string, std::string> pgisInfo;
  pgisInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
  pgisInfo["PG_USER"] = "postgres";
  pgisInfo["PG_PASSWORD"] = "sitim110";
  pgisInfo["PG_DB_NAME"] = "terralib4";
  pgisInfo["PG_CONNECT_TIMEOUT"] = "4"; 

  te::da::DataSource* dsDestination = te::da::DataSourceFactory::make("POSTGIS");
  dsDestination->open(pgisInfo);

// get a transactor to interact to the data source
  te::da::DataSourceTransactor* tDestination = dsDestination->getTransactor();

// create and save dataset in the datasource destination
  te::da::DataSetType* newDataSet = static_cast<te::da::DataSetType*>(dtOrigin->clone());
  newDataSet->setName("public.new_tab_munic_2001_gribis1");
  newDataSet->getDefaultGeomProperty()->setSRID(4326);
  newDataSet->getDefaultGeomProperty()->setGeometryType(te::gm::GeometryType);

  std::cout << std::endl << "starting copy..." << std::endl;
  tDestination->begin();
  tDestination->getDataSetPersistence()->create(newDataSet, datasetOrigin);
  tDestination->commit();
  std::cout << std::endl << "copy finished..." << std::endl;

// release datasets and data sources
  delete datasetOrigin;
  delete tOrigin;
  delete dsOrigin;

  delete tDestination;
  delete dsDestination;    
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Copy Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Copy Example!" << std::endl;
  }
}
