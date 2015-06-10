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
  \file terralib/qt/widgets/st/AnimationItem.h

  \brief This file defines a class for a Animation Item.
*/

#ifndef __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_ITEM_H
#define __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_ITEM_H

// TerraLib
#include "../Config.h"
#include "../../../geometry.h"
#include "../../../datatype.h"
// Qt
#include <QGraphicsPixmapItem>
#include <QAbstractAnimation>

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
        \class AnimationItem

        \brief This class is a dialog for the Pixmap Item.
      */
      class TEQTWIDGETSEXPORT AnimationItem : public QObject, public QGraphicsPixmapItem
      {
        Q_OBJECT
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)

      public:

        /*!
          \brief Constructor
          It constructs a Animation Item.
          \param title The Animation Item title.
          \param display Where the Animation Item is displayed.
        */
        AnimationItem(const QString& title, te::qt::widgets::MapDisplay* display);

        /*!
          \brief Destructor
          It destructs a Animation Item.
        */
        virtual ~AnimationItem();

        /*!
          \brief It sets the internal matrix.
        */
        void setMatrix();

        /*!
          \brief It returns the Animation Item position in device coordinate.
          \return Position in device coordinate.
        */
        QPoint getPosInDeviceCoordinate();

        /*!
          \brief Paint a Animation Item.
        */
        virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) = 0;

        /*!
          \brief Sets the Animation Item duration.
          \param duration The Animation Item duration.
        */
        void setDuration(const unsigned int& duration);

        /*!
          \brief Gets the Animation Item duration.
          \return The Animation Item duration.
        */
        unsigned int getDuration();

        /*!
          \brief Sets the Animation Item direction.
          \param direction The Animation Item direction.
        */
        void setDirection(const QAbstractAnimation::Direction& direction);

        /*!
          \brief Gets the Animation Item direction.
          \return The Animation Item direction.
        */
        QAbstractAnimation::Direction getDirection();

        /*!
          \brief If draw item. 
        */
        virtual void draw() = 0;

        /*!
          \brief Create Animation Item making reprojection if necessary.
        */
        virtual void createAnimationDataInDisplayProjection();

        /*!
          \brief It draws a piece of tracktrajectory trail or draws the current pixmap item.

          \paran curTime The Animation Item current time.
        */
        virtual void drawForward(const unsigned int& curTime) = 0;

        /*!
          \brief It erases a piece of tracktrajectory trail or draws the current pixmap item.

          \paran curTime The Animation Item current time.
        */
        virtual void erase(const unsigned int& curTime) = 0;

      public:
        QString m_title;                                    //!< The icon item title.
        te::qt::widgets::MapDisplay* m_display;             //!< Indicates where the icon item is displayed.
        int m_SRID;                                         //!< The input route srid.
        QPointF m_pos;                                      //!< The icon position.
        QVector<te::dt::TimeInstant> m_time;                //!< The input time.
        QVector<te::dt::TimeInstant> m_animationTime;       //!< The animation time. It contains only the portions to be animated.
        te::gm::LineString* m_route;                        //!< The input path in world coordenates.
        QVector<QPointF> m_animationRoute;                  //!< The path coordenates in map display projection. It contains only the portions to be animated.
        QMatrix m_matrix;                                   //!< The display transformation matrix.
        bool m_automaticPan;                                //!< True if automatic pan over this item.
        double m_panFactor;                                 //!< the range is between 0.05 and 0.5 
        unsigned int m_curTimeDuration;                     //!< Current time (ms). Its value is between 0 and the total duration.
        double m_norInitialTime;                            //!< Normalized initial time (between 0 and 1).
        double m_norFinalTime;                              //!< Normalized final time (between 0 and 1).
        Animation* m_animation;                             //!< The animation this item.
        unsigned int m_duration;                            //!< The duration this item.
        QAbstractAnimation::Direction m_direction;          //!< The direction
        int m_opacity;                                      //!< Opacity.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_ITEM_H
