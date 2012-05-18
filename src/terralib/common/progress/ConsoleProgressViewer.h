/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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

  \brief A class that defines the interface of an console progress viewer.
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

      \brief A class that defines the interface of an console progress viewer.

          This viewer is made using iostream functions. Feed back will be generated
          over a console window. Multiples tasks will be displayed using ONLY one
          progress.

          The proportional value is calculated using TOTALSTEPS / CURRENTSTEPS.

          TOTALSTEPS - Sum of all total steps of all tasks
          CURRENTSTEPS - Sum of all current steps of all tasks

          The progress message is defined by the task message, if it has more than
          on task the message will be "MULTI TASKS".

      \sa 

      \note In this viewer, tasks can NOT be deleted.
    */

    class TECOMMONEXPORT ConsoleProgressViewer : public AbstractProgressViewer
    {
      public:

        /*! \brief Default constructor. */
        ConsoleProgressViewer();

        /*! \brief Virtual destructor. */
        virtual ~ConsoleProgressViewer();

        /** @name ConsoleProgressViewer Methods
         *  Methods for ConsoleProgressViewer access
         */
        //@{

        /*!
          \brief Insert a new taks to progress viewer container

          \param t Task pointer

          \param id Task identifier
        */
        virtual void addTask(TaskProgress* t, int id);

        /*!
          \brief Removes a task from progress viewer container

          \param taskId Task identifier
        */
        virtual void removeTask(int taskId);

        /*!
          \brief Cancel a task

          \param taskId Task identifier
        */
        virtual void cancelTask(int taskId);

        /*!
          \brief Set task total steps

          \param taskId Task identifier
        */
        virtual void setTotalValues(int taskId);

        /*!
          \brief Update the progress evaluation

          \param taskId Task identifier
        */
        virtual void updateValue(int taskId);

        /*!
          \brief Update the progress message

          \param taskId Task identifier
        */
        virtual void updateMessage(int taskId);

        //@}

      protected:

        int m_totalSteps;                       //!< Attribute used to define the total steps of all tasks
        int m_currentStep;                      //!< Attribute used to define the current steps of all task
        int m_propStep;                         //!< Attribute used to define the proportional step (0-100)
        std::string m_message;                  //!< Attribute used to define the progress message.
        std::map<int, TaskProgress*> m_tasks;   //!< Task container.

    };
  } // end namespace common
}   // end namespace te

#endif //__TERRALIB_COMMON_PROGRESS_INTERNAL_CONSOLEPROGRESSVIEWER_H
