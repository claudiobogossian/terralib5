// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>

// STL
#include <iostream>

void MySQLExample()
{
#if TE_EXAMPLE_USE_SQLITE
  try
  {
    std::map<std::string, std::string> dsInfo;
 
    dsInfo["SQLITE_FILE"] = "meudb.sqlite";
 
    te::da::DataSource* ds = te::da::DataSource::create("SQLite", dsInfo);
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in SQLite Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in SQLite Example!" << std::endl;
  }
#endif
}

