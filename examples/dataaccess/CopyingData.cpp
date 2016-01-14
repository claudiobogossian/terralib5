// Examples
#include "DataAccessExamples.h"

// TerraLib
#include "../Config.h"
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// STL
#include <iostream>

std::auto_ptr<te::da::DataSource> GetPostGISConnection();

void CopyingData()
{
  try
  {
    // let's take the input dataset from a shape file
    std::string data_dir = TERRALIB_DATA_DIR;
    std::map<std::string, std::string> connInfo;
    
    std::string aux("");
    std::cout << "Inform the location of your shapefile (ENTER to accept default \'" << (data_dir + "/shp/munic_2001.shp") << "\'): ";
    std::getline (std::cin, aux);
    if (!aux.empty())
      connInfo["URI"] = aux;
    else
      connInfo["URI"] = data_dir + "/shp/munic_2001.shp";
    
    std::auto_ptr<te::da::DataSource> dsOrigin = te::da::DataSourceFactory::make("OGR");
    dsOrigin->setConnectionInfo(connInfo);
    dsOrigin->open();
    
    if (!dsOrigin->isValid())
    {
      std::cout << "Can not access the shapefile.\n";
      return;
    }
    
    // get a transactor to interact to the data source origin
    std::auto_ptr<te::da::DataSourceTransactor> tOrigin = dsOrigin->getTransactor();
    
    std::vector<std::string> datasets = tOrigin->getDataSetNames();
    std::auto_ptr<te::da::DataSet> datasetOrigin = tOrigin->getDataSet(datasets[0]);
    std::auto_ptr<te::da::DataSetType> dtOrigin = tOrigin->getDataSetType(datasets[0]);
    
    std::auto_ptr<te::da::DataSource> dsDestination = GetPostGISConnection();
    if (!dsDestination.get())
      return;
      
    dsDestination->open();

    // get a transactor to interact to the data source
    std::auto_ptr<te::da::DataSourceTransactor> tDestination = dsDestination->getTransactor();
    
    // create and save datasettype in the datasource destination
    te::da::DataSetType* newDataSet = static_cast<te::da::DataSetType*>(dtOrigin->clone());
    newDataSet->setName("public.munic_2001_from_shp_to_pgis");
    GetFirstGeomProperty(newDataSet)->setSRID(4326);
    GetFirstGeomProperty(newDataSet)->setGeometryType(te::gm::GeometryType);
    
    std::cout << std::endl << "starting copy..." << std::endl;
    std::map<std::string, std::string> options;
    if (tDestination->dataSetExists("public.munic_2001_from_shp_to_pgis"))
    {
      tDestination->dropDataSet("public.munic_2001_from_shp_to_pgis");
    }
    tDestination->begin();
    tDestination->createDataSet(newDataSet,options);
    tDestination->add(newDataSet->getName(), datasetOrigin.get(),options);
    tDestination->commit();
    std::cout << std::endl << "Copy finished..." << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in the Copy Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in the Copy Example!" << std::endl;
  }
}
