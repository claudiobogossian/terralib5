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
  \file terralib/qt/widgets/progress/ProgressViewerWidget.h

  \brief A class that defines the interface of a qt widget to group a set of ProgressWidgetItem.
 */

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERWIDGET_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERWIDGET_H

// TerraLib
#include "../../../common/progress/AbstractProgressViewer.h"
#include "../../../common/progress/TaskProgress.h"
#include "../Config.h"

//Qt
#include <QDialog>
#include <QGridLayout>
#include <QScrollArea>

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
      class ProgressWidgetItem;

      /*!
        \class ProgressViewerWidget

        \brief A class that defines the interface of a qt widget to group a set of ProgressWidgetItem.

        This viewer is made using ProgressWidgetItem. Feed back will be generated
        over a custom widget. Multiples tasks will be displayed using individual
        progress widgets

        The proportional value is informed by each task.

        \ingroup widgets

        \sa AbstractProgressViewer, QtProgressBar

        \note In this viewer, tasks can be deleted.
      */
      class TEQTWIDGETSEXPORT ProgressViewerWidget : public QDialog,  public te::common::AbstractProgressViewer
      {
        Q_OBJECT

        public:

          /*! \brief Default constructor. */
          ProgressViewerWidget(QWidget* parent);

          /*! \brief Virtual destructor. */
          virtual ~ProgressViewerWidget();

           /*!
            \brief Insert a new taks to progress viewer container.

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

        protected:

          /*!
            \brief Used to receive custom progress events.

            \param e Event sent to this object.
          */
          void customEvent(QEvent* e);

        public slots:

          /*!
            \brief This slot is connect to all cancel buttons of each progress items

            \param id value used to inform what task was canceled.
          */
          void cancel(int id);

        protected:

          std::map<int, te::common::TaskProgress*> m_tasks;   //!< Task container.
          std::map<int, ProgressWidgetItem*> m_items;         //!< Custom widget progress item  container.
          QGridLayout* m_MainLayout;                          //!< GUI Objects used to build the custom widget.
          QGridLayout* m_widgetLayout;                        //!< GUI Objects used to build the custom widget.
          QScrollArea* m_scroll;                              //!< GUI Objects used to build the custom widget.
          QWidget* m_widget;                                  //!< GUI Objects used to build the custom widget.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif //__TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSVIEWERWIDGET_H

