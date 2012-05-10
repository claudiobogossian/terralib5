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
  \file terralib/common/AbstractProgress.h
 
  \brief The AbstractProgress is an abstract class used to define the main progress functions.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_ABSTRACTPROGRESS_H
#define __TERRALIB_COMMON_INTERNAL_ABSTRACTPROGRESS_H

// TerraLib
#include "../Config.h"
#include "ProgressParams.h"

// STL
#include <string>


namespace te
{
  namespace common
  {
    class ProgressTimer;

    /*!
    \class AbstractProgress

    \brief The AbstractProgress is an abstract class used to define the main progress functions.

    This class has an instance of  a struct Progress Params that have all progress attributes. 
    Also is possible to define if the progress will be used in a multithread enviroment.
      
    \sa ProgressManager, ProgressParams

    \todo ProgressTimer is NOT working if multithread is enabled. Must used some kind of mutex
          to fix this problem.

    */
    class TECOMMONEXPORT AbstractProgress
    {
      public:

        /** @name Initializer Methods
        *  Methods related to instantiation and destruction.
        */
        //@{

        /*! \brief It initializes a new AbstractProgress.

        */
        AbstractProgress();

        /*! \brief Destructor */
        virtual ~AbstractProgress();

        //@}
        
        
        /*!
        \brief Used to set the progress total steps

        \param value Integer value

        \note Used in case of a  default range ( 0 - value)
        */
        virtual void setTotalSteps(const int& value);

        /*!
        \brief Used to set the current step 

        \param step Integer value

        \note This function updates the proportional progress value, if this value has changed
              the variable m_hasToUpdate is TRUE. This function only works if the attribute
              m_isActive is TRUE.
        */
        virtual void setCurrentStep(const int& step);

        /*!
        \brief Gets the current step

        \return Integer value

        */
        virtual int getCurrentStep();

        /*!
        \brief Gets the proportional value from the current step

        \return Integer value

        */
        virtual int getCurrentProportionalStep();
     
        /*!
        \brief Set the progress message

        \param message String value

        */
        virtual void setMessage(const std::string& message);

        /*!
        \brief Get the progress message

        \return String value

        */
        virtual std::string getMessage();

        /*!
        \brief Set the title caption.

        \param title String value

        \note This function has a empty implementation here, this function should be reimplemented
              in GUI applications that uses a progress bar.
        */
        virtual void setTitle(const std::string& title);

        /*!
        \brief Set the progress status

        \param status Boolean attribute.

        */
        virtual void setActive(const bool& status);

        /*!
        \brief Get the progress status

        \return Boolean type.

        */
        virtual bool isActive();

        /*!
        \brief Reset all internal attributes and also restart all progress params attributes

        \note The m_isMultiThread is turn to FALSE and m_isActive is turn to TRUE.

        */
        virtual void reset();
        
        /*!
        \brief Function used to indicate if the progress interface has to be updated.

        \return True if the progress has to be updated and false in other case.

        */
        virtual bool hasToUpdate();

        /*!
        \brief Auxiliar function used to increment the progress.

        \note This function calls the method setCurrentStep(int) using the current step + 1

        */
        virtual void pulse();
        
        /*!
        \brief This function is used to stop a progress.

        \note Turn the variable m_isActive to FALSE.

        */
        virtual void cancel();

        /*!
        \brief Set if the progress will be used in a multithread enviroment.

        \param flag Boolean value used to set the multithread state.

        \note At the end of the process this flag will turn to FALSE.
        */
        void setMultiThreadProgress(const bool& flag);
      
        /*!
        \brief Set if progress timer will be used to estimate time.

        \note If progressTimer is enabled the time will start at the startTimer() function
        */
        void useProgressTimer(const bool& flag);

        /*!
        \brief Start the ProgressTimer timer.

        \note Only works if progressTimer is enabled
        */
        void startTimer();

	
      protected:

        ProgressParams m_progressParams;    //!< A progress params struct.
        ProgressTimer* m_timer;             //!< Class used to set estimated time.

        bool m_isMultiThread;               //!< Flag used to indicate the multithread state.
        bool m_isActive;			              //!< Progress status, turn to FALSE if progress was canceled
        bool m_hasToUpdate;			            //!< Attribute used to indicate if the progress bar has to be uptedated
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_ABSTRACTPROGRESS_H