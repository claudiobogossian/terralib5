#include "../../src/terralib/Defines.h"
#include "ProgressExamples.h"
#include "terralib/common/progress/TaskProgress.h"

// System
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  #include <windows.h>
#endif

// C++ library
#include <iostream>

// OpenMP
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  #include <omp.h>
#endif

#define TOTAL_STEPS 3000
#define PART_1 1000
#define PART_2 2000

#define TOTAL_STEPS_INTERNAL 500

void ConsoleSleeperMultiThread()
{
  te::common::TaskProgress task;
  task.setTotalSteps(TOTAL_STEPS);
  task.setMessage("Console Progress Test");

  task.useMultiThread(true);
  task.useTimer(true);

  #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    omp_set_num_threads( 4 );
    #pragma omp parallel for
  #endif
  for(int i = 0; i < TOTAL_STEPS; ++i)
  {
    if(task.isActive())
    {
      #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        Sleep(5);
      #endif

      task.pulse();
    }

    if(i == PART_1 || i == PART_2)
    {
      te::common::TaskProgress taskInternal;
      taskInternal.setTotalSteps(TOTAL_STEPS_INTERNAL);
      taskInternal.setMessage("Console Progress Internal Task");

      for(int i = 0; i < TOTAL_STEPS_INTERNAL; ++i)
      {
        if(taskInternal.isActive())
        {
          #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
            Sleep(5);
          #endif

          taskInternal.pulse();
        }
      }
    }
  }
}
