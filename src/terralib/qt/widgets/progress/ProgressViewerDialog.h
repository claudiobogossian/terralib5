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
  \file terralib/qt/widgets/progress/ProgressViewerDialog.h

  \brief A class that defines the interface of a qt bar progress viewer.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERDIALOG_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERDIALOG_H

// TerraLib
#include "../../../common/progress/AbstractProgressViewer.h"
#include "../../../common/progress/TaskProgress.h"
#include "../Config.h"

// Qt
#include <QtGui/QProgressDialog>

// STL
#include <map>
#include <string>

namespace te
{
  namespace qt
  {
    namespace widgets
    {

      /*!
        \class ProgressViewerDialog

        \brief  A class that defines the interface of a qt bar progress viewer.

        This widget is a dialog box with progress information and a
        cancel button.
         
        This viewer is made using Qt Progress Dialog Feed back will be generated
        over a dialog window. Multiples tasks will be displayed using ONLY one
        progress.

        The proportional value is calculated using TOTALSTEPS / CURRENTSTEPS.

        TOTALSTEPS - Sum of all total steps of all tasks
        CURRENTSTEPS - Sum of all current steps of all tasks

        The progress message is defined by the task message, if it has more than
        on task the message will be "MULTI TASKS".
      */
      class TEQTWIDGETSEXPORT ProgressViewerDialog : public QObject,  public te::common::AbstractProgressViewer
      {
        Q_OBJECT

        public:

          /*! \brief Default constructor. */
          ProgressViewerDialog(QWidget* parent);

          /*! \brief Virtual destructor. */
          virtual ~ProgressViewerDialog();

           /** @name ProgressViewerDialog Methods
           *  Methods for ProgressViewerDialog access
           */
          //@{

           /*!
            \brief Insert a new taks to progress viewer container

            \param t  Task pointer.
            \param id Task identifier.
          */
          virtual void addTask(te::common::TaskProgress* t, int id);

          /*!
            \brief Removes a task from progress viewer container.

            \param taskId Task identifier.
          */
          virtual void removeTask(int taskId);

          /*!
            \brief Cancel a task.

            \param taskId Task identifier.
          */
          virtual void cancelTask(int taskId);

          /*!
            \brief Set task total steps.

            \param taskId Task identifier.
          */
          virtual void setTotalValues(int taskId);

          /*!
            \brief Update the progress evaluation.

            \param taskId Task identifier.
          */
          virtual void updateValue(int taskId);

          /*!
            \brief Update the progress message.

            \param taskId Task identifier.
          */
          virtual void updateMessage(int taskId);

          //@}

        protected:

          /*!
            \brief Used to receive custom progress events.

            \param e Event sent to this object.
          */
          virtual void customEvent(QEvent* e);

        public slots:

          /*! \brief Get the button clicked and cancel ALL tasks. */
          virtual void cancel();

        protected:

          int m_totalSteps;                                   //!< Attribute used to define the total steps of all tasks.
          int m_currentStep;                                  //!< Attribute used to define the current steps of all task.
          int m_propStep;                                     //!< Attribute used to define the proportional step (0-100).
          std::string m_message;                              //!< Attribute used to define dialog message.
          std::map<int, te::common::TaskProgress*> m_tasks;   //!< Task container.
          QProgressDialog* m_dlgProgress;                     //!< GUI Objects used as progress bar dialog.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif //__TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERDIALOG_H
