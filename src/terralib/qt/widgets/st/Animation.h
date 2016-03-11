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
  \file terralib/qt/widgets/st/Animation.h

  \brief This file defines a class for a Trajectory Animation.
*/

#ifndef __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_H
#define __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_H

// TerraLib
#include "../Config.h"
#include "../../../geometry.h"
#include "../../../datatype.h"

// Qt
#include <QtCore/QPropertyAnimation>


namespace te
{
  namespace gm
  {
    class Envelope;
  }
  namespace dt
  {
    class TimePeriod;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class Animation.

        \Brief This class is responsible for making the animation of a trajectory or temporal image. 
        It has controls like loop, forward, backward and pause.
      */
      class TEQTWIDGETSEXPORT Animation : public QPropertyAnimation
      {
      public:
        /*!
          \brief Constructor
          It constructs a Animation.
          \param target The target QObject for this animation.
          \param propertyName The target property name for this animation.
          \param parent Parent of this object.
        */
        Animation(QObject* target, const QByteArray& propertyName, QObject* parent=0);

        /*!
          \brief Destructor
          It destructs a Animation.
        */
        virtual ~Animation();

        /*!
          \brief 
          It adjust animation data for a given period;
          /param period The temporal period.
        */
        void adjustDataToAnimationTemporalExtent(const te::dt::TimePeriod& period);

        /*!
          \brief 
          It It sets the data key values.
        */
        void setDataKeyValues();

        /*!
          \brief
          It It gets the closest animation data index.
          \param trel Normalized time (between 0 and 1).

          \return The animation data index.
        */
        int getClosestAnimationDataIndex(const double& trel);

        /*!
          \brief
          It It gets the next animation data index. it is the end point, between two points of the trajectory.
          \param trel Normalized time (between 0 and 1).

          \return The animation data index.
        */
        int getNextAnimationDataIndex(const double& trel);

      public:
        te::gm::Envelope m_spatialExtent;             //!< Spatial extent.
        te::dt::TimePeriod m_temporalExtent;          //!< Total temporal extent.
        te::dt::TimePeriod m_temporalAnimationExtent; //!< Temporal animation extent. It can be a portion of the total or greater than the total.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_ANIMATION_H

