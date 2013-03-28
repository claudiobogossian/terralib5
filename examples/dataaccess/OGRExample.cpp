// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>

// STL
#include <iostream>

void OGRExampleRead()
{
#if TE_EXAMPLE_USE_OGR
  try
  {
    std::string ogrInfo("connection_string="TE_DATA_EXAMPLE_DIR"/data/shp");

    te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
    ds->setConnectionStr(ogrInfo);
    ds->open();

// retrieve the data source capabilities and print it
    PrintDataSourceCapabilities(ds);

// retrieve the information about the data stored in the data source
    PrintCatalog(ds);

// for each dataset in teh data source print all its data
    PrintDataSets(ds);

// release the data source: you are the owner
    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in OGR Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in OGR Example!" << std::endl;
  }
#endif
}

void ORGExampleWrite()
{
  #if TE_EXAMPLE_USE_OGR
  try
  {
    te::da::DataSet* datas = CreatingDataSetInMemory();  

    //PrintDataSet(datas);

    te::da::DataSetType* newDt = new te::da::DataSetType(*datas->getType());
    std::map<std::string, std::string> ogrInfo;
    ogrInfo["connection_string"] = ""TE_DATA_EXAMPLE_DIR"/data/shp/soilmeasure.shp";
    te::da::DataSource* ds = te::da::DataSource::create("OGR", ogrInfo);
      
    te::da::DataSourceTransactor* trans = ds->getTransactor();
    te::da::DataSetPersistence* dsPers = trans->getDataSetPersistence();
    dsPers->create(newDt,datas);
    delete dsPers;

    delete trans;
    delete datas;
    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in OGR Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in OGR Example!" << std::endl;
  }
#endif
}
