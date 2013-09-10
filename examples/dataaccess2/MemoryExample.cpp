// TerraLib
//#include <terralib/dataaccess.h>

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
    connInfo["SOURCE"] = ""TE_DATA_EXAMPLE_DIR"/data/mem";
    dsMem->setConnectionInfo(connInfo);

    //It creates a data set type in memory
    std::map<std::string, std::string> options;
    te::da::DataSetType* dt1  = CreateDataSetTypeInMemory("memory_test");
    dsMem->createDataSet(dt1,options);
    std::cout << std::endl << "A data set type in memory has been created! " << std::endl;
    PrintDataSet("memory_test", ds);

    te::da::DataSet* datas = CreatingDataSetInMemoryGivingDt(dt1);
    dsMem->add("memory_test",datas,options);

    std::auto_ptr<te::da::DataSet> ds_ret = dsMem->getDataSet("memory_test");
    delete ds;

    ////It creates a data set in memory
    //ds = CreatingDataSetInMemory("memory_test");
    //std::cout << std::endl << "A data set in memory has been created! " << std::endl;
    //PrintDataSet("memory_test", ds);
    //
    //std::map<std::string, std::string> options; //API exije esse param
    //dsMem->add("memory_test", ds,options); //cai
    //std::auto_ptr<te::da::DataSet> ds_ret = dsMem->getDataSet("marisa_test");
    //delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in Memory Example: " << e.what() << std::endl;
    return;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in Memory Example!" << std::endl;
    return;
  }  
}

