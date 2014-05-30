#include "ProgressExamples.h"

// TerraLib
#include "../../src/terralib/Defines.h"
#include <terralib/common/TerraLib.h>
#include "terralib/common/progress/TaskProgress.h"

// System
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  #include <windows.h>
#endif

#include <iostream>

#define TOTAL_STEPS 2000
#define TOTAL_STEPS_INTERNAL 1000

void ConsoleSleeper()
{
  te::common::TaskProgress task;
  
  task.setTotalSteps(TOTAL_STEPS);
  task.setMessage("Console Progress Test");

  for(unsigned int i = 0; i < TOTAL_STEPS; ++i)
  {
    if(i == TOTAL_STEPS_INTERNAL)
    {
      te::common::TaskProgress taskInteral;

      taskInteral.setTotalSteps(TOTAL_STEPS_INTERNAL);
      taskInteral.setMessage("Second Task");

      for(unsigned int j = 0; j < TOTAL_STEPS_INTERNAL; ++j)
      {
        if(!taskInteral.isActive())
        {
          break;
        }

        #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
          Sleep(5);
        #endif

        taskInteral.pulse();
      }
    }

    if(!task.isActive())
    {
      break;
    }

    #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      Sleep(5);
    #endif

    task.pulse();
  }
}
