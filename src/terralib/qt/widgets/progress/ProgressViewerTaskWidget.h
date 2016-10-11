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
  \file terralib/qt/widgets/progress/ProgressViewerTaskWidget.h

  \brief A progress dialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERTASKWIDGET_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERTASKWIDGET_H

// TerraLib
#include "../../../common/progress/AbstractProgressViewer.h"
#include "../../../common/progress/TaskProgress.h"
#include "../Config.h"

// Qt
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QToolButton>

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
        \class ProgressViewerTaskWidget

        \brief A progress viewer for a unique task.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT ProgressViewerTaskWidget : public QWidget, public te::common::AbstractProgressViewer
      {
        Q_OBJECT

        public:

          /*! \brief Default constructor. */
          ProgressViewerTaskWidget(QWidget* parent, bool hideToolBar, std::string message = "");

          /*! \brief Virtual destructor. */
          virtual ~ProgressViewerTaskWidget();

           /*!
            \brief Insert a new taks to progress viewer container

            \param t  Task pointer.
            \param id Task identifier.
          */
          void addTask(te::common::TaskProgress* t, int id);

          /*!
            \brief Removes a task from progress viewer container.

            \param taskId Task identifier.
          */
          void removeTask(int taskId);

          /*!
            \brief Cancel a task.

            \param taskId Task identifier.
          */
          void cancelTask(int taskId);

          /*!
            \brief Set task total steps.

            \param taskId Task identifier.
          */
          void setTotalValues(int taskId);

          /*!
            \brief Update the progress evaluation.

            \param taskId Task identifier.
          */
          void updateValue(int taskId);

          /*!
            \brief Update the progress message.

            \param taskId Task identifier.
          */
          void updateMessage(int taskId);

          void setTask(te::common::TaskProgress* task);

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

          int m_taskId;                                       //!< Task Identifier.
          int m_totalSteps;                                   //!< Attribute used to define the total steps of all tasks.
          int m_currentStep;                                  //!< Attribute used to define the current steps of all task.
          int m_propStep;                                     //!< Attribute used to define the proportional step (0-100).
          std::string m_message;                              //!< Attribute used to define dialog message.
          te::common::TaskProgress* m_task;                   //!< Task object.
          QProgressBar* m_progressBar;                        //!< GUI Objects used to build the custom widget.
          QToolButton* m_button;                              //!< GUI Objects used to build the custom widget.
          QGridLayout* m_mainGridLayout;                      //!< GUI Objects used to build the custom widget.
          QGridLayout* m_frameGridLayout;                     //!< GUI Objects used to build the custom widget.
          QLabel* m_label;                //!< GUI Objects used to build the custom widget.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif //__TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERTASKWIDGET_H
