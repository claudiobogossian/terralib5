// TerraLib
#include <terralib/dataaccess.h>

// Examples
#include "STExamples.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// STL
#include <iostream>

void CreateDataSourceAndUpdateManager(te::da::DataSourceInfo& dsinfo)
{
  try
  {
    //Generates a randon id to the data source
    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);
    dsinfo.setId(id);

    //Create the data source and put it into the manager
    te::da::DataSourceManager::getInstance().open(dsinfo.getId(), dsinfo.getType(), dsinfo.getConnInfo());

  }
  catch(const te::common::Exception& e)
  {
    std::cout << std::endl << "Failed to create a new data source and put it into the manager: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to create a new data source and put it into the manager: unknow exception!" << std::endl;
  }
}

