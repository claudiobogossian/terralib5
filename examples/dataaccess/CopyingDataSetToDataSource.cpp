// TerraLib
#include <terralib/dataaccess.h>

// Examples
#include "DataAccessExamples.h"

// STL
#include <iostream>

void CopyingDataSetToDataSource(te::da::DataSet* dataSet, const std::string& dsName, const std::string& dsInfo)
{
  try
  {  
    //Open the destination data source
    te::da::DataSource* dsDestination = te::da::DataSourceFactory::make(dsName);
    dsDestination->open(dsInfo);
    
    //Get the destinatino data source persistence
    te::da::DataSourceTransactor* tDestination = dsDestination->getTransactor();
    te::da::DataSetPersistence* pDestination = tDestination->getDataSetPersistence();
    
    std::cout << std::endl << "start copying..." << std::endl;
    pDestination->create(dataSet);  
    std::cout << std::endl << "copying finished..." << std::endl;  
    
    // release datasets and data sources
    delete pDestination;
    delete tDestination;   
    delete dsDestination;   //verificar se apaga os transactors associados a ele... 
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Copying DataSet to DataSource Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried DataSet to DataSource Example!" << std::endl;
  }
}
