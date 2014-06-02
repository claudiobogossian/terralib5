// TerraLib
#include "../Config.h"

// Examples
#include "DataAccessExamples.h"

// STL
#include <iostream>


void MemoryExample()
{
  te::da::DataSet* ds = 0;
  te::da::DataSetType* dt =0;
  try
  {
    std::auto_ptr<te::da::DataSource> dsMem = te::da::DataSourceFactory::make("MEM");
  
    std::map<std::string, std::string> connInfo;
  
    std::string data_dir = TERRALIB_EXAMPLES_DATA_DIR;
  
    connInfo["SOURCE"] = data_dir + "/mem";
    dsMem->setConnectionInfo(connInfo);

    //It creates a data set type in memory called "SoilMeasures"
    std::string named = "SoilMeasures";
    std::map<std::string, std::string> options;
    te::da::DataSetType* dt1  = CreateDataSetTypeInMemory(named);
    dsMem->createDataSet(dt1,options);
    std::cout << std::endl << "A DATASETTYPE in memory has been created! " << std::endl;    
    std::auto_ptr<te::da::DataSetType> dt1_ret = dsMem->getDataSetType(named);   

    //It creates a data set in memory giving a datasettype (populate)
    te::da::DataSet* datas = CreatingDataSetInMemoryGivingDt(dt1);
    PrintDataSet(named, datas);

    //Adding dataset to dsMem datasource of MEMORY type
    dsMem->add(named,datas,options);    
    std::auto_ptr<te::da::DataSet> ds_ret = dsMem->getDataSet(named);
    PrintDataSet(named, ds_ret.get());

    delete ds;

    //It creates a data set in memory named memory_test
    ds = CreatingDataSetInMemory("memory_test");
    std::cout << std::endl << "A data set in memory has been created! " << std::endl;
    PrintDataSet("memory_test", ds);
    
    //std::map<std::string, std::string> options; //API exije esse param
    dsMem->add("memory_test", ds,options); //cai
    std::auto_ptr<te::da::DataSet> ds_ret1 = dsMem->getDataSet("memory_test");
    PrintDataSet("memory_test", ds_ret1.get());
    delete ds;
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

