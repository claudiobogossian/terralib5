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
  \file terralib/common/progress/ProgressManager.h
 
  \brief The Progress is a singleton that can be used to define a progress interface instance.
*/

#ifndef __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSMANAGER_H
#define __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSMANAGER_H

// TerraLib
#include "../Config.h"
#include "../Singleton.h"
#include "AbstractProgress.h"

// STL
#include <map>

namespace te
{
  namespace common
  {
    /*!
      \class ProgressManager

      \brief The ProgressManager is a singleton that can be used to define a progress interface instance.

      Use this class to keep a instance of a Progress Bar. This singleton is not owner of none
      progress instance, those instances must be deleted by his owner.
      
      \sa Singleton, AbstractProgress
    */
    class TECOMMONEXPORT ProgressManager : public te::common::Singleton<ProgressManager>
    {
      friend class te::common::Singleton<ProgressManager>;

      public:

        /*!
          \brief Define the main progress interface

          \param pi Instance of a AbstractProgress.
        */
        void setMainProgress(AbstractProgress* pi);

        /*!
          \brief Get the main progress interface.

          \return Instance of a AbstractProgress.
        */
        AbstractProgress* getMainProgress();

        /*!
          \brief Define the progress interface

          \param pi Instance of a AbstractProgress.

          \return Interger value - Progress ID
        */
        int setProgress(AbstractProgress* pi);

        /*!
          \brief Get the progress interface.

          \param id Progress identification

          \return Instance of a AbstractProgress.
        */
        AbstractProgress* getProgress(const int& id);

        /** @name ProgressManager facade Methods for AbstractProgress
        *  Method used to access the instance stored on this singleton.
        */
        //@{

        /*!
          \brief Used to set the progress total steps.

          \param value Integer value.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \note Used in case of a  default range ( 0 - value).
        */
        void setTotalSteps(const int& value, const int& id = -1);

        /*!
          \brief Used to set the current step 

          \param step Integer value

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined

          \note This function updates the proportional progress value, if this value has changed
                the variable m_hasToUpdate is TRUE. This function only works if the attribute
                m_isActive is TRUE.
        */
        void setCurrentStep(const int& step, const int& id = -1);

        /*!
          \brief Gets the current step.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \return Integer value.
        */
        int getCurrentStep(const int& id = -1);

        /*!
          \brief Gets the proportional value from the current step.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \return Integer value.
        */
        int getCurrentProportionalStep(const int& id = -1);
     
        /*!
          \brief Set the progress message.

          \param message String used to define the progress message

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \param message String value.
        */
        void setMessage(const std::string& message, const int& id = -1);

        /*!
          \brief Get the progress message.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \return String value.
        */
        std::string getMessage(const int& id = -1);

        /*!
          \brief Set the title caption.

          \param title String value.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \note This function has a empty implementation here, this function should be reimplemented
                in GUI applications that uses a progress bar.
        */
        void setTitle(const std::string& title, const int& id = -1);

        /*!
          \brief Set the progress status

          \param status Boolean attribute.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.
        */
        void setActive(const bool& status, const int& id = -1);

        /*!
          \brief Get the progress status.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \return Boolean type.

          \exception Exception If the progress instance was not defined.
        */
        bool isActive(const int& id = -1);

        /*!
          \brief Reset all internal attributes and also restart all progress params attributes

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \note The m_isMultiThread is turn to FALSE and m_isActive is turn to TRUE.

          \exception Exception If the progress instance was not defined.
        */
        void reset(const int& id = -1);
        
        /*!
          \brief Function used to indicate if the progress interface has to be updated.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \return True if the progress has to be updated and false in other case.

          \exception Exception If the progress instance was not defined.
        */
        bool hasToUpdate(const int& id = -1);

        /*!
          \brief Auxiliar function used to increment the progress.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \note This function calls the method setCurrentStep(int) using the current step + 1.
        */
        void pulse(const int& id = -1);
        
        /*!
          \brief This function is used to stop a progress.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \note Turn the variable m_isActive to FALSE.
        */
        void cancel(const int& id = -1);

         /*!
          \brief This function is used to set the window modality

          \param flag Status to define the window modality, true is modal.

          \exception Exception If the progress instance was not defined.

        */
        virtual void setModal(const bool& flag, const int& id = -1);

        /*!
          \brief Set if the progress will be used in a multithread enviroment.

          \param flag Boolean value used to set the multithread state.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined

          \note At the end of the process this flag will turn to FALSE.
        */
        void setMultiThreadProgress(const bool& flag, const int& id = -1);
      
        /*!
          \brief Set if progress timer will be used to estimate time.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \note If progressTimer is enabled the time will start at the startTimer() function.
        */
        void useProgressTimer(const bool& flag, const int& id = -1);

        /*!
          \brief Start the ProgressTimer timer.

          \param id Progress Identification, if id is equal -1, the main progress will be used

          \exception Exception If the progress instance was not defined.

          \note Only works if progressTimer is enabled.
        */
        void startTimer(const int& id = -1);

        //@}

      protected:

        /*! \brief It initializes the Singleton. */
        ProgressManager();

        /*! \brief Destructor. */
        ~ProgressManager();

        /*! \brief Generate a new ID. */
        int getProgressId();

      private:

        int m_progressCounter;                          //!< Internal counter used to generate progress Ids

        std::map<int, AbstractProgress*> m_progressMap; //!< Map with all progress instances
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_PROGRESS_INTERNAL_PROGRESSMANAGER_H

