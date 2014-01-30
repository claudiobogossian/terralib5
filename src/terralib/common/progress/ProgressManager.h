/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/progress/ProgressManager.h

  \brief A singleton class used to manage tasks progresses and their viewers.
*/

#ifndef __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSMANAGER_H
#define __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSMANAGER_H

// TerraLib
#include "../Config.h"
#include "../Singleton.h"
#include "../ThreadingPolicies.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace common
  {
// Forward declarations
    class AbstractProgressViewer;
    class TaskProgress;

    /*!
      \class ProgressManager

      \brief A singleton class used to manage tasks progresses and their viewers.

      This singleton is used to store all tasks created and their
      progress viewers. If one or more viewers is attached,
      all instances will be used to display the tasks progresses.

      \ingroup common

      \sa AbstractProgressViewer, TaskProgress
    */
    class TECOMMONEXPORT ProgressManager : public te::common::ObjectLevelLockable<ProgressManager,
                                                                                  ::boost::recursive_mutex,
                                                                                  ::boost::lock_guard< ::boost::recursive_mutex>,
                                                                                  ::boost::lock_guard< ::boost::recursive_mutex> >,
                                           public te::common::Singleton<ProgressManager>
    {
      friend class te::common::Singleton<ProgressManager>;

      public:

        /*!
          \brief Attach a progress viewer

          \param apv Progress viewer instance

          \return Progress viewer identifier

          \note 
        */
        int addViewer(AbstractProgressViewer* apv);

        /*!
          \brief Dettach a progress viewer

          \param viewerId Progress viewer identifier
        */
        void removeViewer(int viewerId);

        /*!
          \brief Used in TaskProgress constructor, register this task generating a task id.

          \param tp TaskProgress instance

          \return  Task identifier

          \note Thread-safe!
        */
        int addTask(TaskProgress* tp);

        /*!
          \brief Used in TaskProgress destructor, remove task from singleton.

          \param taskId Task identifier.

          \note Thread-safe!
        */
        void removeTask(int taskId);

        /*!
          \brief Inform all viewers that a task was canceled.

          \param taskId Task identifier.
        */
        void cancelTask(int taskId);

        /*!
          \brief Cancels the task with the given task type and inform all viewers that a task was canceled.

          \param type The task type.
        */
        void cancelTasks(unsigned int type);

        /*!
          \brief Inform all viewers that a task set the total values.

          \param taskId Task identifier.
        */
        void setTotalValues(int taskId);

        /*!
          \brief Inform all viewers that a task set the current step.

          \param taskId Task identifier.
        */
        void updateValue(int taskId);

        /*!
          \brief Inform all viewers that a task set the message.

          \param taskId Task identifier.
        */
        void updateMessage(int taskId);

        /*!
          \brief Removes references for Viewers and tasks.
        */
        void clearAll();

      protected:

        /*! \brief Default constructor. */
        ProgressManager();

        /*! \brief Destructor. */
        ~ProgressManager();

        /*!
          \brief Used to generate a new viewer id (use internal counter).

          \return Viewer identification as integer value.
        */
        int generateViewerId();

        /*!
          \brief Used to generate a new task id (use internal counter).

          \return Task identification as integer value.
        */
        int generateTaskId();

      protected:

        int m_taskCounter;                                //!< Counter used to generate a task id.
        int m_viewerCounter;                              //!< Counter used to generate a viewer id.
        std::map<int, TaskProgress*> m_tasks;             //!< Container with tasks.
        std::map<int, AbstractProgressViewer*> m_viewers; //!< Container with viewers.
    };

  } // end namespace common
}   // end namespace te

#endif //__TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSMANAGER_H
