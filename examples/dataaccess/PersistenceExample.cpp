// TerraLib
#include <terralib/dataaccess.h>

// Examples
#include "DataAccessExamples.h"

// STL
#include <iostream>


void PersistenceExample()
{
 // te::da::DataSet* ds = 0;
 // try
 // {
 //   //============= Creates a new data set    
 //   //It creates a data set in memory
 //   ds = CreatingDataSetInMemory();
 //   std::cout << std::endl << "A data set in memory has been created! " << std::endl;
 //   
 //   //It creates the data set 'ds' in a data source
 //   std::string dsInfo("DataSource=POSTGIS&PG_HOST=atlas.dpi.inpe.br&PG_PORT=5432&PG_DB_NAME=terralib4&PG_USER=postgres&PG_PASSWORD=sitim110&PG_CONNECT_TIMEOUT=5");
 //   CopyingDataSetToDataSource(ds, "POSTGIS", dsInfo);
 // 
 //   //============= Adds a new property and its values     
 //   //Get the created data set
 //   te::da::DataSource* dataSource = te::da::DataSourceFactory::make("POSTGIS");
 //   dataSource->open(dsInfo);
 //   te::da::DataSourceTransactor* transactor = dataSource->getTransactor();
 //   te::da::DataSet* newDataSet = transactor->getDataSet(ds->getType()->getName());
 //           
 //   //============= It adds a new property and its values in the new data set
 //   AddingDataSetPropertyValues(newDataSet);  
 // 
 //   //============= It removes a property
 ////   te::dt::Property* prop = newDataSet->getType()->getProperty(5);
 ////   DroppingDataSetTypeProperty(prop, transactor);
 // 
 //   //============= It drops the created dataset
 //   //DroppingDataSetType(newDataSet->getType(), transactor);
 // 
 //   //Delete the old data set in memory
 //   delete ds; 

 //   //Delete data source
 //   delete transactor;
 //   delete newDataSet;
 //   delete dataSource;
 // }
 // catch(const std::exception& e)
 // {
 //   std::cout << std::endl << "An exception has occurried in Persistence Example: " << e.what() << std::endl;
 //   return;
 // }
 // catch(...)
 // {
 //   std::cout << std::endl << "An unexpected exception has occurried in Persistence Example!" << std::endl;
 //   return;
 // }  
}

