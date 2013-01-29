// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>

// STL
#include <iostream>

void MySQLExample()
{
#if TE_EXAMPLE_USE_MYSQL
  try
  {
    std::map<std::string, std::string> dsInfo;
 
    dsInfo["MY_HOST_NAME"] = "localhost";
    dsInfo["MY_USER_NAME"] = "root";
    dsInfo["MY_PASSWORD"] = "admin";
    dsInfo["MY_SCHEMA"] = "test";
    dsInfo["MY_PORT"] = "3306";
    dsInfo["MY_OPT_CONNECT_TIMEOUT"] = "4";
 
    te::da::DataSource* ds = te::da::DataSourceFactory::open("MYSQL", dsInfo);
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in MySQL Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in MySQL Example!" << std::endl;
  }
#endif
}