/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/progress/ProgressWidgetItem.h

  \brief Custom widget used to represent a single task.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSWIDGETITEM_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSWIDGETITEM_H

//TerraLib
#include "../../widgets/Config.h"

//Qt
#include <QCommandLinkButton>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {

      /*!
        \class ProgressWidgetItem

        \brief Custom widget used to represent a single task.

        \ingroup widgets

        \sa ProgressViewerWidget
      */
      class TEQTWIDGETSEXPORT ProgressWidgetItem : public QWidget
      {
        Q_OBJECT

        public:

          /*! \brief Default constructor. */
          ProgressWidgetItem(QWidget* parent, int taskId, int totalSteps);

          /*! \brief Virtual destructor. */
          ~ProgressWidgetItem();

          /*!
            \brief Set the current value in progress bar.

            \param step Step value as integer.
          */
          virtual void setValue(int step);

          /*!
            \brief Set the progress label information.
            
            \param message String with the label information.
          */
          virtual void setLabel(const std::string& message);

          /*! \brief Reset the progress bar. */
          virtual void reset();

        public slots:

          /*! \brief Get the button clicked and cancel ALL tasks. */
          virtual void cancel();

        signals:

          /*!
            \brief Inform that a task was canceled.

            \param id taskId.
          */
          void taskCanceled(int id);

        protected:

          /*!
            \brief Used to receive custom progress events.

            \param e Event sent to this object.
          */
          virtual void customEvent(QEvent* e);

        protected:

          int m_taskId;                   //!< Task Identifier.
          QLabel* m_label;                //!< GUI Objects used to build the custom widget.
          QProgressBar* m_progressBar;    //!< GUI Objects used to build the custom widget.
          QCommandLinkButton* m_button;   //!< GUI Objects used to build the custom widget.
          QFrame* m_frame;                //!< GUI Objects used to build the custom widget.
          QGridLayout* m_mainGridLayout;  //!< GUI Objects used to build the custom widget.
          QGridLayout* m_frameGridLayout; //!< GUI Objects used to build the custom widget.
      };

    } // end namespace widgets
  }   // end namespave qt
}     // end namespave te

#endif  // __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSWIDGETITEM_H

