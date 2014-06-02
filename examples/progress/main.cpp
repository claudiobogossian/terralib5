// Examples
#include "ProgressExamples.h"

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/common/progress/ConsoleProgressViewer.h>

// STL
#include <cstdlib>
#include <iostream>
#include <time.h>


int main(int argc, char *argv[])
{
  try
  {
  //init components
    TerraLib::getInstance().initialize();

    te::common::ConsoleProgressViewer* cpv = new te::common::ConsoleProgressViewer();

    int idViewer = te::common::ProgressManager::getInstance().addViewer(cpv);
    
    //console sleeper
    std::cout << std::endl << "Console progress Test using multi tasks... " << std::endl;
    ConsoleSleeper();

    //console sleeper multi thread
    std::cout << std::endl << "Console progress Test using multi thread... " << std::endl;
    ConsoleSleeperMultiThread();

    //qt sleeper
    QtSleeper(argc, argv);

    te::common::ProgressManager::getInstance().removeViewer(idViewer);

    delete cpv;

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in the Progress examples: " << e.what() << std::endl;

    std::cout << std::endl << "Press Enter to exit..." << std::endl;

    std::cin.get();

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in Progress examples!" << std::endl;

    std::cout << std::endl << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

  std::cout << std::endl << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
