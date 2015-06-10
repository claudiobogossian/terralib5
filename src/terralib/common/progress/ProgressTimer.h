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
  \file terralib/common/progress/ProgressTimer.h

  \brief The ProgressTimer is a utility class that can be used to calculate the estimated time to finish a task.
*/

#ifndef __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSTIMER_H
#define __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSTIMER_H

// TerraLib
#include "../Config.h"

// STL
#include <ctime>
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \class ProgressTimer

      \brief The ProgressTimer is a utility class that can be used to calculate the estimated time to finish a task.

      \sa TaskProgress
    */
    class TECOMMONEXPORT ProgressTimer
    {

      public:

        /*! \brief It initializes a ProgressTimer. */
        ProgressTimer(int totalSteps, std::string message);

        /*! \brief Destructor. */
        ~ProgressTimer();

        /*! \brief Start the internal timer. */
        void start();

        /*! 
          \brief Define a new step process evolution.

          \note For each tick, new values for remaining time and speed time are computed.
        */
        void tick();

        /*! \brief Set the total steps. */
        void setTotalSteps(int totalSteps);

        /*! \brief Set the message used by task progress. */
        void setMessage(std::string message);

        /*!
          \brief Function used to get the remaining time to end the process.

          \return Dobule value, the remaingin time in minutes.
        */
        double getRemainingTimeInMin() const;

        /*!
          \brief Function used to get the speed time.

          \return Double value, the spped time in seconds.
        */
        double getSpeedTimeInSec() const;

        /*!
          \brief Get the information about the evolution of the process.

          \return String with the information about remaining and speed time.
        */
        std::string getMessage();

      private:

        int m_totalSteps;         //!< Total steps.
        int m_count;              //!< Internal counter.
        time_t m_startTime;       //!< Initial time.
        double m_remainingTime;   //!< Remaining time in minutes.
        double m_speedTime;       //!< Speed time in seconds.
        std::string m_message;    //!< Original task message
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSTIMER_H

