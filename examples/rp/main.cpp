// Examples
#include "RPExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/core/plugin.h>

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

    Segmenter();
    Contrast();
    Classifier();
    MixtureModel();
    Mosaic();
    Register();
    IHSFusion();
    Filter();
    ArithmeticOperations();
    Texture();

    te::core::PluginManager::instance().clear();
    te::core::plugin::FinalizePluginSystem();

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in RP examples: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in RP examples!" << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
