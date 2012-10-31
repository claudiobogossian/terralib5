/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/progress/TaskProgress.h

  \brief This class is the implementation of a progress evalution of a task.
*/

#ifndef __TERRALIB_COMMON_PROGRESS_INTERNAL_TASKPROGRESS_H
#define __TERRALIB_COMMON_PROGRESS_INTERNAL_TASKPROGRESS_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace common
  {
// Forward declaration
    class ProgressTimer;

    /*!
      \class TaskProgress

      \brief This class is the implementation of a progress evalution of a task.

      \sa ProgressTimer

      \note In constructor the task auto register in progress manager and in
            detructor the task is removed from progress manager.

      \todo ProgressTimer is NOT working if TaskProgress is in multithread mode.
    */
    class TECOMMONEXPORT TaskProgress
    {
      public:

        /*! \brief Default constructor. */
        TaskProgress(const std::string& message = "");

        /*! \brief Destructor. */
        ~TaskProgress();

        /** @name TaskProgress Methods
         *  Methods for task access
         */
        //@{

        /*!
          \brief Get the task identification.

          \return Task id as integer value.
        */
        int getId();

        /*!
          \brief Get the task total stepes.

          \return Interger value with total steps.
        */
        int getTotalSteps();

        /*!
          \brief Set the task total stepes.

          \param value Interger value with total steps.
        */
        void setTotalSteps(int value);

        /*!
          \brief Get the proportional value (value between 0-100).

          \return Proportional value as integer.
        */
        int getProportionalValue();

        /*!
          \brief Get the task current step.

          \return Current step as integer value.
        */
        int getCurrentStep();

        /*!
          \brief Set the task current step.

          \param value Current value.
        */
        void setCurrentStep(int value);

        /*! \brief Calls setCurrentStep() function using getCurrentStep() + 1. */
        void pulse();

        /*!
          \brief Get the task message.

          \return String value with task message.
        */
        std::string getMessage();

        /*!
          \brief Set the task message.

          \param message String value with task message.
        */
        void setMessage(std::string message);

        /*!
          \brief Verify if the task is active.

          \return True if task is active and false in other case.
        */
        bool isActive();

        /*! \brief Cancel task, set the task active FALSE. */
        void cancel();

        /*!
          \brief Used to define if task is running in thread mode.

          \param flag Boolean value used to set the thread option.
        */
        void useMultiThread(bool flag);

        /*!
          \brief Used to define if task use progress timer information.

          \param flag  Boolean value used to set the timer option.
        */
        void useTimer(bool flag);

        /*!
          \brief This function is used when proportional value has changed.

          \return True if progress has to be update and false in other case.
        */
        bool hasToUpdate();

        //@}

      protected:

        int m_id;                   //!< Task identification.
        int m_totalSteps;           //!< Task total steps.
        int m_currentStep;          //!< Task current step.
        int m_currentPropStep;      //!< Current proportinal step.
        std::string m_message;      //!< Task message.
        bool m_hasToUpdate;         //!< Flag used to indicate the update status.
        bool m_isActive;            //!< Flag used to indicate the task status.
        bool m_isMultiThread;       //!< Flag used to indicate the thread mode.
        bool m_useTimer;            //!< Flag used to indicate the timer status.
        ProgressTimer* m_timer;     //!< Progress timer instance.
    };

  } // end namespace common
}   // end namespace te

#endif //__TERRALIB_COMMON_PROGRESS_INTERNAL_TASKPROGRESS_H
