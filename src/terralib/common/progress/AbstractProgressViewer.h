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
  \file terralib/common/progress/AbstractProgressViewer.h

  \brief A class that defines the interface of an abstract progress viewer.
*/

#ifndef __TERRALIB_COMMON_PROGRESS_INTERNAL_ABSTRACTPROGRESSVIEWER_H
#define __TERRALIB_COMMON_PROGRESS_INTERNAL_ABSTRACTPROGRESSVIEWER_H

// TerraLib
#include "../Config.h"

namespace te
{
  namespace common
  {
// Forward declarations
    class TaskProgress;

    /*!
      \class AbstractProgressViewer

      \brief A class that defines the interface of an abstract progress viewer.

      \ingroup common

      \sa TaskProgress, ProgressManager
    */
    class AbstractProgressViewer
    {
      public:

        /*! \brief Default constructor. */
        AbstractProgressViewer() { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractProgressViewer() { }

        /*!
          \brief Insert a new task in the progress viewer.

          \param t  Task pointer.
          \param id Task identifier.

          \note The Progress Viewer will NOT take the ownership of TaskProgress (t).
        */
        virtual void addTask(TaskProgress* t, int id) = 0;

        /*!
          \brief Removes a task from progress viewer container.

          \param taskId Task identifier.
        */
        virtual void removeTask(int taskId) = 0;

        /*!
          \brief Cancel a task.

          \param taskId Task identifier.
        */
        virtual void cancelTask(int taskId) = 0;

        /*!
          \brief Set task total steps.

          \param taskId Task identifier.
        */
        virtual void setTotalValues(int taskId) = 0;

        /*!
          \brief Update the progress evaluation.

          \param taskId Task identifier.
        */
        virtual void updateValue(int taskId) = 0;

        /*!
          \brief Update the progress message.

          \param taskId Task identifier.
        */
        virtual void updateMessage(int taskId) = 0;
    };

  } // end namespace common
}   // end namespace te

#endif //__TERRALIB_COMMON_PROGRESS_INTERNAL_ABSTRACTPROGRESSVIEWER_H
