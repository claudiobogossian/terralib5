#include "RasterExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>

// STL
#include <iostream>
#include <string>

void VectorizeRaster()
{
  try
  {
    std::cout << "This example shows how to use the vectorization method." << std::endl << std::endl;

// define raster info and load
    std::map<std::string, std::string> rinfo;
    rinfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters/pattern1.tif";
    te::rst::Raster* inraster = te::rst::RasterFactory::open(rinfo);

    std::vector<te::gm::Geometry*> polygons;
    inraster->vectorize(polygons, 0);

    std::cout << "vectorizer created " << polygons.size() << " polygons" << std::endl;
    for (unsigned int i = 0; i < polygons.size(); i++)
      std::cout << "  polygon " << i << ": " << polygons[i]->toString() << std::endl;

// clean up
    delete inraster;
    polygons.clear();

    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in VectorizeRaster(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in VectorizeRaster()!" << std::endl;
  }
}
