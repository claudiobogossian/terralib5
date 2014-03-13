// Examples
#include "GraphExamples.h"

// TerraLib
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/common/progress/ConsoleProgressViewer.h>
#include <terralib/common/TerraLib.h>
#include <terralib/plugin.h>

// STL
#include <cstdlib>
#include <iostream>
#include <locale>


int main()
{
  setlocale(LC_ALL,"C");

  try
  {
    //startup terralib
    TerraLib::getInstance().initialize();

    //create a progress bar
    std::auto_ptr<te::common::ConsoleProgressViewer> cpv(new te::common::ConsoleProgressViewer());
    int idViewer = te::common::ProgressManager::getInstance().addViewer(cpv.get());

    //load all necessary modules
    LoadModules();

    //-----------------------------------------------------------------------------------------------------
    bool draw = true;

    //exchange spatial weights files
    ExchangeSpatialWeightsFiles(draw);


    //create gpm graph
    CreateGPMAdjacencyGraph(draw);

    CreateGPMDistanceGraph(draw);


    //create ldd graph
    CreateLDDGraph(draw);

    //-----------------------------------------------------------------------------------------------------

    //remove progress bar
    te::common::ProgressManager::getInstance().removeViewer(idViewer);

    //unload modules
    te::plugin::PluginManager::getInstance().unloadAll();

    //finalize terralib
    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in Graph examples: " << e.what() << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in Graph examples!" << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
