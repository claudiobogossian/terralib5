// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>

// STL
#include <iostream>

void SQLiteExample()
{
#if TE_EXAMPLE_USE_SQLITE
  try
  {
    std::map<std::string, std::string> dsInfo;
 
    dsInfo["SQLITE_FILE"] = "meudb.sqlite";
    dsInfo["SQLITE_OPEN_READWRITE"] = "TRUE";
    dsInfo["SQLITE_OPEN_CREATE"] = "TRUE";
 
    te::da::DataSource* ds = te::da::DataSource::create("SQLite", dsInfo);

    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in SQLite Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in SQLite Example!" << std::endl;
  }
#endif
}

