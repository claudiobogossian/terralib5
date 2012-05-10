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
  \file terralib/common/ProgressManager.h
 
  \brief The Progress is a singleton that can be used to define a progress interface instance.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_PROGRESSMANAGER_H
#define __TERRALIB_COMMON_INTERNAL_PROGRESSMANAGER_H

// TerraLib
#include "AbstractProgress.h"
#include "../Singleton.h"
#include "../Config.h"


namespace te
{
  namespace common
  {
    /*!
    \class ProgressManager

    \brief The ProgressManager is a singleton that can be used to define a progress interface instance.

    Use this class to keep a instance of a Progress Bar. When the singleton finishes
    its life time it automatically delete the current progress interface instance.
      
    \sa Singleton, AbstractProgress

    \todo 

    */
    class TECOMMONEXPORT ProgressManager : public te::common::Singleton<ProgressManager>
    {
      friend class te::common::Singleton<ProgressManager>;

      public:

        /** @name Initializer Methods
        *  Methods related to instantiation and destruction.
        */
        //@{

        /*! \brief It initializes the Singleton. */
        ProgressManager();

        /*! \brief Destructor. */
        ~ProgressManager();

        //@}

        /*!
        \brief Define the progress interface

        \param pi Instance of a AbstractProgress

        */
        void setProgress(AbstractProgress* pi);

        /*!
        \brief Get the progress interface

        \return Instance of a AbstractProgress

        */
        AbstractProgress* getProgress();


        /** @name ProgressManager facade Methods for AbstractProgress
        *  Method used to access the instance stored on this singleton.
        */
        //@{

        /*!
        \brief Used to set the progress total steps

        \param value Integer value

        \exception Exception If the progress instance was not defined

        \note Used in case of a  default range ( 0 - value)
        */
        void setTotalSteps(const int& value);

        /*!
        \brief Used to set the current step 

        \param step Integer value

        \exception Exception If the progress instance was not defined

        \note This function updates the proportional progress value, if this value has changed
              the variable m_hasToUpdate is TRUE. This function only works if the attribute
              m_isActive is TRUE.
        */
        void setCurrentStep(const int& step);

        /*!
        \brief Gets the current step

        \exception Exception If the progress instance was not defined

        \return Integer value

        */
        int getCurrentStep();

        /*!
        \brief Gets the proportional value from the current step

        \exception Exception If the progress instance was not defined

        \return Integer value

        */
        int getCurrentProportionalStep();
     
        /*!
        \brief Set the progress message

        \exception Exception If the progress instance was not defined

        \param message String value

        */
        void setMessage(const std::string& message);

        /*!
        \brief Get the progress message

        \exception Exception If the progress instance was not defined

        \return String value

        */
        std::string getMessage();

        /*!
        \brief Set the title caption.

        \param title String value

        \exception Exception If the progress instance was not defined

        \note This function has a empty implementation here, this function should be reimplemented
              in GUI applications that uses a progress bar.
        */
        void setTitle(const std::string& title);

        /*!
        \brief Set the progress status

        \param status Boolean attribute.

        \exception Exception If the progress instance was not defined

        */
        void setActive(const bool& status);

        /*!
        \brief Get the progress status

        \return Boolean type.

        \exception Exception If the progress instance was not defined

        */
        bool isActive();

        /*!
        \brief Reset all internal attributes and also restart all progress params attributes

        \note The m_isMultiThread is turn to FALSE and m_isActive is turn to TRUE.

        \exception Exception If the progress instance was not defined

        */
        void reset();
        
        /*!
        \brief Function used to indicate if the progress interface has to be updated.

        \return True if the progress has to be updated and false in other case.

        \exception Exception If the progress instance was not defined

        */
        bool hasToUpdate();

        /*!
        \brief Auxiliar function used to increment the progress.

        \exception Exception If the progress instance was not defined

        \note This function calls the method setCurrentStep(int) using the current step + 1

        */
        void pulse();
        
        /*!
        \brief This function is used to stop a progress.

        \exception Exception If the progress instance was not defined

        \note Turn the variable m_isActive to FALSE.

        */
        void cancel();

        /*!
        \brief Set if the progress will be used in a multithread enviroment.

        \param flag Boolean value used to set the multithread state.
        
        \exception Exception If the progress instance was not defined

        \note At the end of the process this flag will turn to FALSE.

        */
        void setMultiThreadProgress(const bool& flag);
      
        /*!
        \brief Set if progress timer will be used to estimate time.

        \exception Exception If the progress instance was not defined             

        \note If progressTimer is enabled the time will start at the startTimer() function

        */
        void useProgressTimer(const bool& flag);

        /*!
        \brief Start the ProgressTimer timer.

        \exception Exception If the progress instance was not defined

        \note Only works if progressTimer is enabled
        */
        void startTimer();

        //@}
 
      private:

        /** @name Copy Constructor and Assignment Operator
        *  Copy constructor and assignment operator not allowed.
        */
        //@{

        /*!
        \brief Copy constructor not allowed.

        \param rhs The right-hand-side copy that would be used to copy from.
        */
        ProgressManager(const ProgressManager& rhs);

        /*!
        \brief Assignment operator not allowed.

        \param rhs The right-hand-side copy that would be used to copy from.

        \return A reference to this object.
        */
        ProgressManager& operator=(const ProgressManager& rhs);

        //@}

      private:

        AbstractProgress* m_progressInterface;  //!< A progress interface instance.
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_PROGRESSMANAGER_H