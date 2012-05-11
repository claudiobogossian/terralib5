// Examples
#include "ProgressExamples.h"

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/common/progress/ConsoleProgress.h>

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

    te::common::ConsoleProgress* progress = new te::common::ConsoleProgress();

    te::common::ProgressManager::getInstance().setProgress(progress);
    
    //console sleeper
    ConsoleSleeper();

    //console sleeper multi thread
    ConsoleSleeperMultiThread();

    //qt sleeper
    QtSleeper(argc, argv);

    //boost thread as functor
    //WorkerThread();

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in Progress examples: " << e.what() << std::endl;

    std::cout << std::endl << "Press Enter to exit..." << std::endl;

    std::cin.get();

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in Progress examples!" << std::endl;

    std::cout << std::endl << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

  std::cout << std::endl << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
