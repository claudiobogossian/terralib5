// Examples
#include "GeometryExamples.h"

// TerraLib
#include <terralib/common/Exception.h>

// SLT
#include <iostream>
#include <fstream>
#include <string>

void readWkts(const std::string& filePath)
{
  std::cout << "Reading Wkt file..." << std::endl;

  // Open a txt file that contains a set of Wkt geometries
  std::ifstream wktFile(filePath.c_str(), std::ifstream::in);
  std::string wkt;
  while(wktFile.good()) // for each Wkt
  {
    std::getline(wktFile, wkt);

    std::cout << std::endl;
    std::cout << "File Wkt: " << wkt << std::endl;
    
    te::gm::Geometry* g = 0;
    try
    {
      // Reads the Wkt and gets the geometry
      g = te::gm::WKTReader::read(wkt.c_str());
    }
    catch(te::common::Exception& e)
    { 
      std::cout << e.what() << std::endl;
    }
    
    if(g == 0)
      continue;

    std::cout << "Generated Geometry Wkt: " << g->asText() << std::endl;
      
    delete g;
  }

  wktFile.close();
}