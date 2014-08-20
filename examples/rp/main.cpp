// Examples
#include "RPExamples.h"

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/plugin.h>

// STL
#include <cstdlib>
#include <iostream>

int main()
{
  try
  {
    TerraLib::getInstance().initialize();

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

    te::plugin::PluginManager::getInstance().unloadAll();

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
