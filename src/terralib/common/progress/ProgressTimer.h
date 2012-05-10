/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/ProgressTimer.h
 
  \brief The ProgressTimer is a util class used to calculate the estimated
         time to finish loop job.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_PROGRESSTIMER_H
#define __TERRALIB_COMMON_INTERNAL_PROGRESSTIMER_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

#include <time.h>


namespace te
{
  namespace common
  {
    /*!
    \class ProgressTimer

    \brief The ProgressTimer is a util class used to calculate the estimated
           time to finish loop job.
    \sa 

    \todo 

    */
    class TECOMMONEXPORT ProgressTimer
    {
      
      public:


      /** @name Initializer Methods
      *  Methods related to instantiation and destruction.
      */
      //@{

      /*! \brief It initializes a new AbstractProgress.

      */
      ProgressTimer(int totalSteps);

      /*! \brief Destructor */
      ~ProgressTimer();

      //@}

      
      /*!
      \brief 

      */
      void start();

      
      /*!
      \brief 

      */
      void tick();

      /*!
      \brief 

      */
      void setTotalSteps(const int& totalSteps);


      /** @name ProgressTimer Accessor Method
      *  Methods used to access the attributes stored on this class.
      */
      //@{

      /*!
      \brief 

      \return 

      */
      double getRemainingTimeInMin();

      /*!
      \brief 

      \return 

      */
      double getSpeedTimeInSec();

      /*!
      \brief 

      \return 

      */
      std::string getMessage();
      //@}

		
      private:

      int m_totalSteps;       //!< Total steps
      int m_count;            //!< Internal counter
      time_t m_startTime;     //!< Initial time
      double m_remainingTime;	//!< Remaining time in minutes
      double m_speedTime;	    //!< Speed time in seconds
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_PROGRESSTIMER_H