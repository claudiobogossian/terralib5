#include "ProgressExamples.h"

// TerraLib
#include <terralib/common/TerraLib.h>
#include "terralib/common/progress/ProgressManager.h"

// System
#include <windows.h>
#include <iostream>

#define TOTAL_STEPS 15000

void ConsoleSleeper()
{
  te::common::ProgressManager::getInstance().reset();
	te::common::ProgressManager::getInstance().setTotalSteps(TOTAL_STEPS);
	te::common::ProgressManager::getInstance().setMessage("Console Progress Test");
  te::common::ProgressManager::getInstance().useProgressTimer(true);
  
  std::cout << std::endl << "Console progress bar test..." << std::endl;

  te::common::ProgressManager::getInstance().startTimer();

	for(unsigned int i = 0; i < TOTAL_STEPS; ++i)
	{
		if(!te::common::ProgressManager::getInstance().isActive())
		{
			break;
		}

		Sleep(5);

		te::common::ProgressManager::getInstance().pulse();
	}
}