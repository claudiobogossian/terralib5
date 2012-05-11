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
  \file terralib/qt/widgets/progress/QtProgress.h
 
  \brief The QtProgress is a qt implementation of a progress class used to define the main progress functions.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_QTPROGRESS_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_QTPROGRESS_H

//TerraLib
#include "../../common/progress/AbstractProgress.h"
#include "../Config.h"

//Qt
#include <QtGui/QProgressDialog>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class QtProgress

        \brief The QtProgress is a progress bar for qt applications.

        \sa ProgressManager, AbstractProgress, ProgressParams
      */
      class TEQTWIDGETSEXPORT QtProgress : public QProgressDialog, public te::common::AbstractProgress
      {
        Q_OBJECT

        public:

        /** @name Initializer Methods
        *  Methods related to instantiation and destruction.
        */
        //@{

        /*!
          \brief It initializes a new AbstractProgress.
        */
        QtProgress(QWidget* parent);

        /*! \brief Destructor */
        ~QtProgress();

        //@}

        /** @name AbstractProgress Virtual Methods
        *  Methods used to manage the progress interface.
        */
        //@{

        /*!
          \brief Used to set the current step.

          \param step Integer value.

          \note This function call the abstract setCurrentStep implementation and after
                updates the qt progress information.
        */
        virtual void setCurrentStep(const int& step);

        /*!
          \brief Set the progress message.

          \param message String value.
        */
        virtual void setMessage(const std::string& message);

        /*!
          \brief Set the title caption.

          \param title String value.
        */
        virtual void setTitle(const std::string& title);

        /*!
          \brief Reset all internal attributes and also restart all progress params attributes.

          \note The m_isMultiThread is turn to FALSE and m_isActive is turn to TRUE.
        */
        virtual void reset();

      public slots:

        /*!
          \brief This function is used to stop a progress.

          \note Turn the variable m_isActive to FALSE.
        */
        virtual void cancel();

      protected:

        /*!
          \brief This function is used to receive the custom events sended by this class.

          \note Is oly used if m_isMultiThread attribute is TRUE.
        */
        virtual bool eventFilter(QObject* obj, QEvent* event);
    
        //@}
      };
    }  // end namespace widgets
  }    // end namespace qt
}      // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROGRESS_QTPROGRESS_H

