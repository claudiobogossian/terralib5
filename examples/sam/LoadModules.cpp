// Examples
#include "SAMExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  try
  {
  }
  catch(const te::common::Exception& e)
  {
    std::cout << std::endl << "Failed to load data source drivers: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to load data source drivers: unknow exception!" << std::endl;
  }
}

