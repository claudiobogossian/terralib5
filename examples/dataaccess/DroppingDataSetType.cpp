// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/memory.h>

// Examples
#include "DataAccessExamples.h"

// STL
#include <iostream>

void DroppingDataSetType(te::da::DataSetType* dt, te::da::DataSourceTransactor* trans)
{
  try
  {  
    te::da::DataSetTypePersistence* dtPersistence = trans->getDataSetTypePersistence();
    
    dtPersistence->drop(dt);
    
    delete dtPersistence;     
    return;      
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in Dropping DataSetType Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in Dropping DataSetType Example!" << std::endl;
  }
}


