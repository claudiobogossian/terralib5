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
  \file terralib/qt/widgets/st/TrajectoryItem.h

  \brief This file defines a class for a Trajectory Item.
*/

#ifndef __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_TRAJECTORY_ITEM_H
#define __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_TRAJECTORY_ITEM_H

// TerraLib
#include "AnimationItem.h"
#include "../Config.h"
#include "../../../geometry.h"
#include "../../../datatype.h"

// Qt
#include <QtCore/QAbstractAnimation>

namespace te
{
  namespace dt
  {
    class DateTimeInstant;
    class TimePeriod;
 }

  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class Animation;

      /*!
      \class This class lets to make trajectory animation on the map display.
      */
      class TEQTWIDGETSEXPORT TrajectoryItem : public AnimationItem
      {
        //Q_OBJECT
        //Q_PROPERTY(QPointF pos READ pos WRITE setPos)

      public:

        /*!
          \brief Constructor
          It constructs a Trajectory Icon Item.
          \param title The icon item title.
          \param display Where the icon item is displayed.
          \param file The icon file.
          \param size icon size.
        */
        TrajectoryItem(const QString& title, te::qt::widgets::MapDisplay* display, const QString& file, const QSize& size);

        /*!
          \brief Destructor
          It destructs a Trajectory Icon Item.
        */
        virtual ~TrajectoryItem();

        /*!
          \brief Paint a piece of trajectory trail.
        */
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

        /*!
        \brief It transform coodinates to map display projection coodinates.
        \param vec Coordinates in native projection.
        */
        void transformToDisplayProjection(QVector<QPointF>& vec);

        /*!
          \brief Draw the trajectory long trail. 
          It draws the beginning until the current time.
          The beginning depends on the direction (forward or backward).
        */
        void draw();

        /*!
          \brief Draw a piece of tracktrajectory trail.
          It draws from the previous time to the current time.

          \paran curTime The trajectory current time.
        */
        void drawForward(const unsigned int& curTime);

        /*!
          \brief erase a piece of tracktrajectory trail.

          \paran curTime The trajectory current time.
        */
        void erase(const unsigned int& curTime);

      public:
        QString m_iconFile;       //!< file that generated the pixmap
        QSize m_iconSize;         //!< icon size
        bool m_drawTrail;         //!< flag to draw trail
        QColor m_forwardColor;    //!< The forward trail color.
        QColor m_backwardColor;   //!< The backward trail color.
        int m_lineWidth;          //!< The route/trail line width in pixels.
        bool m_erasePerfectly;    //!< flag to erase trajectory piece perfectly (default = false).
        QPointF m_posOld;         //!< Auxiliar point
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_TRAJECTORY_ITEM_H
