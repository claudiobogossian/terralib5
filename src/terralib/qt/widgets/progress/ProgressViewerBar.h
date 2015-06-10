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
  \file terralib/qt/widgets/progress/ProgressViewerBar.h

  \brief A class that defines the interface of a qt bar progress viewer.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERBAR_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERBAR_H

// TerraLib
#include "../../../common/progress/AbstractProgressViewer.h"
#include "../../../common/progress/TaskProgress.h"
#include "../Config.h"

//Qt
#include <QGridLayout>
#include <QProgressBar>
#include <QPushButton>

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
        \class ProgressViewerBar

        \brief A class that defines the interface of a qt bar progress viewer.

        It can be use inside a status bar.

        This viewer is made using QtProgressBar object. Feed back will be generated
        over a custom widget. Multiples tasks will be displayed using ONLY one
        progress.

        The proportional value is calculated using TOTALSTEPS / CURRENTSTEPS.

        TOTALSTEPS - Sum of all total steps of all tasks
        CURRENTSTEPS - Sum of all current steps of all tasks

        This viwer does NOT have message information. A Qt Button objected has located
        at the right side of the bar. The button clicked can be accessed getting 
        clicked() signal.

        \ingroup widgets

        \sa AbstractProgressViewer, QtProgressBar

        \note In this viewer, tasks can NOT be deleted.
      */
      class TEQTWIDGETSEXPORT ProgressViewerBar : public QWidget,  public te::common::AbstractProgressViewer
      {
        Q_OBJECT

        public:

          /*! \brief Default constructor. */
          ProgressViewerBar(QWidget* parent);

          /*! \brief Virtual destructor. */
          ~ProgressViewerBar();

           /*!
            \brief Insert a new taks to progress viewer container

            \param t  Task pointer
            \param id Task identifier
          */
          void addTask(te::common::TaskProgress* t, int id);

          /*!
            \brief Removes a task from progress viewer container

            \param taskId Task identifier
          */
          void removeTask(int taskId);

          /*!
            \brief Cancel a task

            \param taskId Task identifier
          */
          void cancelTask(int taskId);

          /*!
            \brief Set task total steps

            \param taskId Task identifier
          */
          void setTotalValues(int taskId);

          /*!
            \brief Update the progress evaluation

            \param taskId Task identifier
          */
          void updateValue(int taskId);

          /*!
            \brief Update the progress message

            \param taskId Task identifier
          */
          void updateMessage(int taskId);

          void setButtonText(const std::string& value);

        protected:

          /*!
            \brief Used to receive custom progress events.

            \param e Event sent to this object.
          */
          virtual void customEvent(QEvent* e);

        public slots:

          /*! \brief Used to get the button clicked (internal function). */
          virtual void onReleased();

        signals:

          /*! \brief Emit a signal if the button was clicked. */
          void clicked();

        protected:

          int m_totalSteps;                                   //!< Attribute used to define the total steps of all tasks.
          int m_currentStep;                                  //!< Attribute used to define the current steps of all task.
          int m_propStep;                                     //!< Attribute used to define the proportional step (0-100).
          std::map<int, te::common::TaskProgress*> m_tasks;   //!< Task container.
          QGridLayout* m_layout;                              //!< GUI Objects used to build the custom widget.
          QProgressBar* m_progressBar;                        //!< GUI Objects used to build the custom widget.
          QPushButton* m_button;                              //!< GUI Objects used to build the custom widget.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif //__TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERBAR_H
