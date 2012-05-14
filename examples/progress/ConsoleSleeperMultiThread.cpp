#include "ProgressExamples.h"
#include "terralib/common/progress/ProgressManager.h"

// System
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  #include <windows.h>
#endif

#include <iostream>

// OpenMP
#include <omp.h>

#define TOTAL_STEPS 15000

void ConsoleSleeperMultiThread()
{
  te::common::ProgressManager::getInstance().reset();
  te::common::ProgressManager::getInstance().setTotalSteps(TOTAL_STEPS);
  te::common::ProgressManager::getInstance().setMessage("Console Progress Test");
  te::common::ProgressManager::getInstance().useProgressTimer(false);


  std::cout << std::endl << "Console progress bar test..." << std::endl;

  omp_set_num_threads( 4 );

  #pragma omp parallel for
  for(int i = 0; i < TOTAL_STEPS; ++i)
  {
    if(te::common::ProgressManager::getInstance().isActive())
    {
      #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        Sleep(5);
      #endif

      te::common::ProgressManager::getInstance().pulse();
    }
  }

  te::common::ProgressManager::getInstance().reset();
}
