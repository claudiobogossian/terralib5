/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/progress/WidgetProgressItem.h

  \brief A class that defines the interface of a qt bar progress viewer.
         Can be use inside a status bar.
 */

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_BARPROGRESSVIEWER_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_BARPROGRESSVIEWER_H

// TerraLib
#include "../Config.h"
#include "terralib/common/progress/AbstractProgressViewer.h"
#include "terralib/common/progress/TaskProgress.h"

//Qt
#include <QtGui/QGridLayout>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>

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
        \class BarProgressViewer

        \brief A class that defines the interface of a qt bar progress viewer.
               Can be use inside a status bar.

               This viewer is made using QtProgressBar object. Feed back will be generated
                over a custom widget. Multiples tasks will be displayed using ONLY one
                progress.

                The proportional value is calculated using TOTALSTEPS / CURRENTSTEPS.

                TOTALSTEPS - Sum of all total steps of all tasks
                CURRENTSTEPS - Sum of all current steps of all tasks

                This viwer does NOT have message information. A Qt Button objected has located
                at the right side of the bar. The button clicked can be accessed getting 
                clicked() signal.

        \sa AbstractProgressViewer, QtProgressBar

        \note In this viewer, tasks can NOT be deleted.
      */
      class TEQTWIDGETSEXPORT BarProgressViewer : public QWidget,  public te::common::AbstractProgressViewer
      {
        Q_OBJECT

        public:

          /*! \brief Default constructor. */
          BarProgressViewer(QWidget* parent);

          /*! \brief Virtual destructor. */
          virtual ~BarProgressViewer();


          /** @name BarProgressViewer Methods
           *  Methods for BarProgressViewer access
           */
          //@{

           /*!
            \brief Insert a new taks to progress viewer container

            \param t Task pointer

            \param id Task identifier
          */
          virtual void addTask(te::common::TaskProgress* t, int id);

          /*!
            \brief Removes a task from progress viewer container

            \param taskId Task identifier
          */
          virtual void removeTask(int taskId);

          /*!
            \brief Cancel a task

            \param taskId Task identifier
          */
          virtual void cancelTask(int taskId);

          /*!
            \brief Set task total steps

            \param taskId Task identifier
          */
          virtual void setTotalValues(int taskId);

          /*!
            \brief Update the progress evaluation

            \param taskId Task identifier
          */
          virtual void updateValue(int taskId);

          /*!
            \brief Update the progress message

            \param taskId Task identifier
          */
          virtual void updateMessage(int taskId);

          //@}

        protected:

          /*!
            \brief Used to receive internal events

            \param obj Object sender

            \param event Event sended by object

            \return Always return true
          */
          virtual bool eventFilter(QObject* obj, QEvent* event);

        public slots:

          /*!
            \brief Used to get the button clicked (internal function)
          */
          virtual void on_Released();

        signals:

          /*!
            \brief Emit a signal if the button was clicked
          */
          void clicked();

        protected:

          int m_totalSteps;                                   //!< Attribute used to define the total steps of all tasks
          int m_currentStep;                                  //!< Attribute used to define the current steps of all task
          int m_propStep;                                     //!< Attribute used to define the proportional step (0-100)
          std::map<int, te::common::TaskProgress*> m_tasks;   //!< Task container.
          QGridLayout* m_layout;                              //!< GUI Objects used to build the custom widget
          QProgressBar* m_barProgress;                        //!< GUI Objects used to build the custom widget
          QPushButton* m_button;                              //!< GUI Objects used to build the custom widget
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif //__TERRALIB_QT_WIDGETS_PROGRESS_BARPROGRESSVIEWER_H
