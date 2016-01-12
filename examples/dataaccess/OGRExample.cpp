// Examples
#include "../Config.h"
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype/Property.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>

// STL
#include <iostream>

void OGRExampleRead()
{
  try
  {
    std::string data_dir = TERRALIB_DATA_DIR;
    
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = data_dir + "/shp";
  
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
    ds->setConnectionInfo(connInfo);
    ds->open();
    
    // check point: the datasource exists and is opened to be used
    std::cout << "Datasource is opened? " << std::boolalpha << ds->isOpened() << '\n' << '\n';

    // check point: what can be done with this datasource
    PrintDataSourceCapabilities(ds.get());

    // check point: retrieving data from the datasource
    std::cout << "\nDatasource has " << ds->getNumberOfDataSets() << " datasources\n";
    std::vector<std::string> dsets = ds->getDataSetNames();
    for (size_t i=0; i<ds->getNumberOfDataSets(); ++i)
      std::cout << '[' << i << "]: " << dsets[i] << std::endl;
    
    // check point: retrieving the data from a dataset of the datasource
    std::cout << "Select a dataset [from 0 to " << ds->getNumberOfDataSets() << "] to see its data: ";
    int n;
    std::cin >> n;
    if (n<0 && n>= ds->getNumberOfDataSets())
      return;
    
    PrintDataSet(dsets[n], ds->getDataSet(dsets[n]).get());

  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in the OGR Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in the OGR Example!" << std::endl;
  }
}

void ORGExampleWrite()
{
  try
  {
    // create a dataset in memory with some data
    std::string dsName("soilmeasures");
    te::da::DataSetType* dType  = CreateDataSetTypeInMemory(dsName);
    te::da::DataSet* dSet = CreatingDataSetInMemoryGivingDt(dType);
    dSet->moveBeforeFirst();
  
    // create a datasource using OGR
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = std::string(TERRALIB_DATA_DIR) + "/shp";
    ds->setConnectionInfo(connInfo);
    ds->open();

    // check point: the datasource exists and is opened to be used
    std::cout << "Datasource is opened? " << std::boolalpha << ds->isOpened() << '\n' << '\n';
    
    // persist the dataset from memory to OGR
    ds->createDataSet(dType, std::map<std::string, std::string>());
    ds->add(dsName,dSet, std::map<std::string, std::string>());
    ds->close();
    
    // check point: reopening the data source
    std::cout << "Datasource is opened? " << std::boolalpha << ds->isOpened() << '\n' << '\n';
    ds->open();
    std::cout << "Datasource is opened? " << std::boolalpha << ds->isOpened() << '\n' << '\n';
    
    // check point: reading the data inserted
    PrintDataSet(dsName, ds->getDataSet(dsName).get());
    
    delete dSet;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in the OGR Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in the OGR Example!" << std::endl;
  }
}

