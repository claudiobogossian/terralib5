// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/st.h>
#include <terralib/stloader.h>

// Examples
#include "STExamples.h"

// STL
#include <iostream>

void MovingObjectsFromKMLAndMetadata(std::vector<te::st::MovingObject*>& output, 
                                     const std::string& xmlFile)
{
  try
  { 

    te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
    te::stloader::DataLoader::loadMovingObjects(ds, xmlFile, output);
    delete ds;    
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in MovingObjectsFromKML: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in MovingObjectsFromKML!" << std::endl;
  }
}

