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
  \file terralib/qt/widgets/canvas/AnimationScene.h

  \brief This file defines a class for a Animation Scene.
*/

#ifndef __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_SCENE_H
#define __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_SCENE_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QGraphicsScene>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class AnimationItem;

      /*!
        \class AnimationScene

        \brief This class is a dialog for the Trajectory Scene.
      */
      class TEQTWIDGETSEXPORT AnimationScene : public QGraphicsScene
      {
      public:

        /*!
          \brief Constructor
          It constructs a Trajectory Scene
        */
        AnimationScene(te::qt::widgets::MapDisplay* display, QObject* parent=0);

        /*!
          \brief Destructor
          It destructs a Trajectory Scene
        */
        virtual ~AnimationScene();

        /*!
          \brief Create a new clear pixmap.
        */
        void createNewPixmap();

        /*!
          \brief Add item.
          \param item The graphics item.
        */
        void addItem(AnimationItem* item);

        /*!
          \brief Remove item.
          \param item The graphics item.
        */
        void removeItem(AnimationItem* item);

        /*!
          \brief Clear scene.
        */
        void clear();

        /*!
          \brief It sets the transformation matrix.
        */
        void setMatrix();

        /*!
          \brief It sets the trajectory duration.
          \param duration The animation duration in miliseconds
        */
        void setDuration(const int& duration);

        /*!
          \brief It draws the animation item.
          \param curtime The normalized current time (between 0 and 1).
        */
        void draw(const int& curTime);

        /*!
          \brief It configures automatic pan over a trajectory.
          \param title The trajectory title.
        */
        void setAutomaticPan(const QString& title);

      public:
        te::qt::widgets::MapDisplay* m_display; //!< Indicates where the scene is displayed.
        QPixmap* m_trajectoryPixmap;            //!< QPixmap where all the trajectory item are drawn.
        int m_numberOfTrajectories;             //!< Number of trajectory items.
        int m_numberOfPixmaps;                  //!< Number of pixmap items.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_SCENE_H
