// TerraLib
#include <terralib/dataaccess.h>

// Examples
#include "DataAccessExamples.h"

// STL
#include <iostream>


void MemoryExample()
{
  te::da::DataSet* ds = 0;
  try
  {
    //It creates a data set in memory
    ds = CreatingDataSetInMemory("SoilMeasures");
    std::cout << std::endl << "A data set in memory has been created! " << std::endl;
    PrintDataSet("SoilMeasures", ds);
    delete ds;
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

