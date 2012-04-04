// Examples
#include "RasterExamples.h"

// TerraLib
#include <terralib/common/TerraLib.h>

// STL
#include <cstdlib>
#include <iostream>

int main()
{
  try
  {
    TerraLib::getInstance().initialize();

    LoadModules();

    ExemplifyDataAccess();
    ArithmeticWithRaster();
    MakeRasterBlock();
    RasterizePolygonSet();
    ResampleRaster();
    SplitComplexImage();
    GribExample();
    ReprojectRasterUsingDriver();
    ReprojectRasterUsingFunction();
    RasterInMemory();
    ExemplifyIterators();
    GribPolygonExample();

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in Raster examples: " << e.what() << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in Raster examples!" << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
