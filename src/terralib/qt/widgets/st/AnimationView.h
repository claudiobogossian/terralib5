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
  \file terralib/qt/widgets/st/Animation.h

  \brief This file defines a class for a Trajectory Animation.
*/

#ifndef __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_VIEW_H
#define __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_VIEW_H

// TerraLib
#include "../Config.h"

// Qt
#include <QGraphicsView>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      /*!
        \brief This class is a animation view.
      */
      class TEQTWIDGETSEXPORT AnimationView : public QGraphicsView
      {
        Q_OBJECT

      public:
        /*!
          \brief Constructor
          It constructs a AnimationView.
          \param parent Parent of this object.
        */
        AnimationView(te::qt::widgets::MapDisplay* md);

        /*!
          \brief Destructor
          It destructs a Animation.
        */
        virtual ~AnimationView();

        void mousePressEvent(QMouseEvent*);
        void mouseMoveEvent(QMouseEvent*);
        void mouseReleaseEvent(QMouseEvent*);
        void mouseDoubleClickEvent(QMouseEvent*);
        void enterEvent(QEvent*);
        void keyPressEvent(QKeyEvent*);
        void keyReleaseEvent(QKeyEvent*);
        void leaveEvent(QEvent*);

        void dragEnterEvent(QDragEnterEvent*);
        void dragLeaveEvent(QDragLeaveEvent*);
        void dragMoveEvent(QDragMoveEvent*);
        void dropEvent(QDropEvent*);

      signals:
        void animationDragEnterEvent(QDragEnterEvent*);
        void animationDropEvent(QDropEvent*);


        //bool eventFilter(QObject*, QEvent*);

      private:
        MapDisplay* m_display;

      public:
        bool m_dragDropAccepted;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_VIEW_H

