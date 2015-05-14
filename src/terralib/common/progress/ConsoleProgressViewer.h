/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/common/progress/ConsoleProgressViewer.h

  \brief A progress viewer implementation for the console.
*/

#ifndef __TERRALIB_COMMON_PROGRESS_INTERNAL_CONSOLEPROGRESSVIEWER_H
#define __TERRALIB_COMMON_PROGRESS_INTERNAL_CONSOLEPROGRESSVIEWER_H

// TerraLib
#include "../Config.h"
#include "AbstractProgressViewer.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace common
  {
// Forward declarations
    class TaskProgress;

    /*!
      \class ConsoleProgressViewer

      \briefA progress viewer implementation for the console.

      This viewer is based on iostream. All the feedback will be generated
      over a console window. Multiples tasks will be displayed using a single
      report.

      The proportional value is calculated using TOTAL_STEPS / CURRENT_STEPS. Where:
      <ul>
      <li>TOTAL_STEPS: Sum of all total steps of all tasks</li>
      <li>CURRENTSTEPS: Sum of all current steps of all tasks</li>
      </ul>

      The progress message is defined by the task message, if it has more than
      one task the message will be set to: "MULTI TASKS".

      \sa AbstractProgressViewer, TaskProgress
    */
    class TECOMMONEXPORT ConsoleProgressViewer : public AbstractProgressViewer
    {
      public:

        /*! \brief Default constructor. */
        ConsoleProgressViewer();

        /*! \brief Virtual destructor. */
        ~ConsoleProgressViewer();

        void addTask(TaskProgress* t, int id);

        void removeTask(int taskId);

        void cancelTask(int taskId);

        void setTotalValues(int taskId);

        void updateValue(int taskId);

        void updateMessage(int taskId);

      protected:

        int m_totalSteps;                       //!< Attribute used to define the total steps of all tasks.
        int m_currentStep;                      //!< Attribute used to define the current steps of all task.
        int m_propStep;                         //!< Attribute used to define the proportional step (0-100).
        std::string m_message;                  //!< Attribute used to define the progress message.
        std::map<int, TaskProgress*> m_tasks;   //!< Task container.
    };

  } // end namespace common
}   // end namespace te

#endif //__TERRALIB_COMMON_PROGRESS_INTERNAL_CONSOLEPROGRESSVIEWER_H

