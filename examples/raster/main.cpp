// Examples
#include "RasterExamples.h"

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/core/utils/Platform.h>
#include <terralib/core/plugin/PluginManager.h>
#include <terralib/core/plugin/Serializers.h>
#include <terralib/core/plugin/Utils.h>

// STL
#include <cstdlib>
#include <iostream>


int main()
{
  try
  {
    TerraLib::getInstance().initialize();
    te::core::plugin::InitializePluginSystem();

    LoadModules();

    ExemplifyDataAccess();
    ArithmeticWithRaster();
    MakeRasterCrop();
    RasterizePolygonSet();
    ResampleRaster();
    SplitComplexImage();
    // GribExample();
    ReprojectRasterUsingDriver();
    ReprojectRasterUsingFunction();
    RasterInMemory();
    ExemplifyIterators();
    // GribPolygonExample();
    Raster1Bit();
    VectorizeRaster();

    te::core::PluginManager::instance().clear();
    te::core::plugin::FinalizePluginSystem();

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in Raster examples: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in Raster examples!" << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
