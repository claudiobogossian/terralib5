// TerraLib
#include "../Config.h"

// Examples
#include "DataAccessExamples.h"

// STL
#include <iostream>


void MemoryExample()
{
  try
  {
    // a folder that contains some auxiliary data
    std::string data_dir = TERRALIB_DATA_DIR;
    
    // creates a datasource in memory
    std::auto_ptr<te::da::DataSource> dsMem = te::da::DataSourceFactory::make("MEM");

    // creates a datasettype, named "SoilMeasures", in the memory datasource
    std::string dsName("SoilMeasures");
    te::da::DataSetType* dType  = CreateDataSetTypeInMemory(dsName);
    dsMem->createDataSet(dType,std::map<std::string, std::string>());                       // no specific options
    
    // check point: datasettype was created and can be retrieved from datasource
    std::auto_ptr<te::da::DataSetType> dType_ret = dsMem->getDataSetType(dsName);
    std::cout << std::endl << "DATASETTYPE in memory has been created" << std::endl;

    // creates a dataset in memory that follows the previous create
    te::da::DataSet* dSet = CreatingDataSetInMemoryGivingDt(dType);
    
    // check point: dataset is correct and populated
    std::cout << std::endl << "DATASET " << dsName << " in memory has been created:" << std::endl;
    PrintDataSet(dsName, dSet);

    // adds the dataset to datasource
    dsMem->add(dsName,dSet,std::map<std::string, std::string>()); // no special options
    
    // check point: retrieve the dataset from datasource
    std::auto_ptr<te::da::DataSet> ds_ret = dsMem->getDataSet(dsName);
    std::cout << std::endl << "DATASET " << dsName << " was inserted in retrieved from datasource:" << std::endl;
    PrintDataSet(dsName, ds_ret.get());
    
    delete dSet;  // releases the dataset created
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in Memory Example: " << e.what() << std::endl;
    return;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in Memory Example!" << std::endl;
    return;
  }  
}

