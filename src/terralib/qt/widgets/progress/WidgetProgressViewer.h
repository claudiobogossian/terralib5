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
  \file terralib/qt/widgets/progress/WidgetProgressViewer.h

  \brief A class that defines the interface of a qt widget to group
         a set of WidgetProgressItem.
 */

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_WIDGETPROGRESSVIEWER_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_WIDGETPROGRESSVIEWER_H

// TerraLib
#include "../Config.h"
#include "terralib/common/progress/AbstractProgressViewer.h"
#include "terralib/common/progress/TaskProgress.h"

//Qt
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QScrollArea>

// STL
#include <map>
#include <string>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class WidgetProgressItem;

      /*!
        \class WidgetProgressViewer

        \brief A class that defines the interface of a qt widget to group
               a set of WidgetProgressItem.

              This viewer is made using WidgetProgressItem. Feed back will be generated
              over a custom widget. Multiples tasks will be displayed using individual
              progress widgets

              The proportional value is informed by each task.

        \sa AbstractProgressViewer, QtProgressBar

        \note In this viewer, tasks can be deleted.
      */
      class TEQTWIDGETSEXPORT WidgetProgressViewer : public QDialog,  public te::common::AbstractProgressViewer
      {
        Q_OBJECT

        public:

          /*! \brief Default constructor. */
          WidgetProgressViewer(QWidget* parent);

          /*! \brief Virtual destructor. */
          virtual ~WidgetProgressViewer();

          /** @name WidgetProgressViewer Methods
           *  Methods for WidgetProgressViewer access
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

        public slots:

          /*!
            \brief This slot is connect to all cancel buttons of each progress itens

            \param Integer value used to inform what task was canceled.
          */
          virtual void cancel(int);

        protected:

          std::map<int, te::common::TaskProgress*> m_tasks;   //!< Task container
          std::map<int, WidgetProgressItem*> m_itens;         //!< Custom widget progress item  container
          QGridLayout* m_MainLayout;                          //!< GUI Objects used to build the custom widget
          QGridLayout* m_widgetLayout;                        //!< GUI Objects used to build the custom widget
          QScrollArea* m_scroll;                              //!< GUI Objects used to build the custom widget
          QWidget* m_widget;                                  //!< GUI Objects used to build the custom widget
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif //__TERRALIB_QT_WIDGETS_PROGRESS_WIDGETPROGRESSVIEWER_H
