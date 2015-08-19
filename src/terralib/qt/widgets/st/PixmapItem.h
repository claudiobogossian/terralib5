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
  \file terralib/qt/widgets/st/PixmapItem.h

  \brief This file defines a class for a Pixmap Item.
*/

#ifndef __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_PIXMAP_ITEM_H
#define __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_PIXMAP_ITEM_H

// TerraLib
#include "AnimationItem.h"
#include "../Config.h"
#include "../../../geometry.h"
#include "../../../datatype.h"

// Qt
#include <QtCore/QAbstractAnimation>
#include <QtCore/QDir>

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
        \class This class is used to make animation of temporal image on map display.
        
        Note: It does not use QPixmap. It uses an array of images already 
        processed to make the animation. Therefore, the pixmap() method 
        returns null. Anyway is an artifice used to animate temporal image 
        instead trajectory.
      */
      class TEQTWIDGETSEXPORT PixmapItem : public AnimationItem
      {
        //Q_OBJECT
        //Q_PROPERTY(QPointF pos READ pos WRITE setPos)

      public:

        /*!
          \brief Empty Constructor.
        */
        PixmapItem();

        /*!
          \brief Constructor
          It constructs a Pixmap Item.
          \param title The pixmap item title.
          \param file The absolute file path name.
          \param display Where the pixmap item is displayed.
        */
        PixmapItem(const QString& title, const QString& file, te::qt::widgets::MapDisplay* display);

       /*!
          \brief Destructor
          It destructs a Pixmap Item.
        */
        virtual ~PixmapItem();

        /*!
          \brief Paint a pixmap item.
        */
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

        /*!
          \brief Calculate current file.

          \param curTime current time
        */
        void calculateCurrentFile(const unsigned int& curTime); // remover

        /*!
          \brief Draw the current pixmap item.
        */
        void draw();

        /*!
          \brief Adjust data for a given time animation period.
        */
        void adjustDataToAnimationTemporalExtent();

        void setImagePosition(const QPointF& p, const QRectF& dr);

        /*!
          \brief It gets image rect in device coordinate.
        */
        QRect getRect();

        /*!
          \brief Draw current pixmap item.
          \paran curTime The animation current time.
        */
        void drawForward(const unsigned int& curTime);

        /*!
          \brief Draw current pixmap item.
          \paran curTime The animation current time.
        */
        void erase(const unsigned int& curTime);

        /*!
          \brief Sets the LUT.
          \paran tab The LUT information.
        */
        void setLUT(const std::vector<std::pair<int, QColor> >& tab);

      public:
        QDir m_dir;                           //!< The image data directory.
        size_t  m_nlines;                     //!< number of lines.
        size_t  m_ncols;                      //!< number of colunms.
        float m_undef;                        //!< undef value.
        QVector<QString> m_files;             //!< The input files.
        QVector<QString> m_animationFiles;    //!< The png files in display projection. It contains only the portions to be animated.
        QString          m_currentImageFile;  //!< Image to be displayed on paint event animation.
        QString          m_baseFile;          //!< Base file name.
        QString          m_suffix;            //!< File suffix.
        QRectF           m_imaRect;           //!< Image location.
        std::map<uchar, QColor> m_lut;        //!< LUT
        QPointF          m_imageCenterPos;    //!< Center of the image.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SLIDER_INTERNAL_PIXMAP_ITEM_H
