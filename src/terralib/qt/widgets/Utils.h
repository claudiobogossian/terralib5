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
  \file QtUtils.h
   
  \brief This file contains utility functions for dealing with Qt controls.
 */

#ifndef __TERRALIB_QTWIDGETS_INTERNAL_UTILS_H
#define __TERRALIB_QTWIDGETS_INTERNAL_UTILS_H

// TerraLib
#include "../../maptools/Canvas.h"
#include "Config.h"
#include "Globals.h"

// Qt
#include <QtCore/QObject>

// Forward declarations
class QTreeWidgetItem;
class QImage;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \brief It sets the check state for the children of a item.
      */
      TEQTWIDGETSEXPORT void SetChildrenCheckState(QTreeWidgetItem* item, int column, Qt::CheckState state);

      /*!
        \brief It returns the file format as a NULL terminated string.

        \return The file format as a NULL terminated string.
      */
      inline const char* GetFormat(te::map::ImageType t)
      {
        switch(t)
        {
          case te::map::PNG:
            return Globals::sm_pngFmt;

          case te::map::JPEG:
            return Globals::sm_jpegFmt;

          case te::map::GIF:
            return Globals::sm_gifFmt;

          case te::map::BMP:
            return Globals::sm_bmpFmt;

          case te::map::XPM:
            return Globals::sm_xpmFmt;

          case te::map::XBM:
            return Globals::sm_xbmFmt;

          default:
            return Globals::sm_unknownFmt;
        };
      }

      /*!
        \brief It creates a QImage from an RGBA color array.

        \param img    Image - a square matrix of int's where each position corresponds to a RGBA color.
        \param width  The image width.
        \param height The image height.

        \return A pointer to a QImage.

        \note The caller of this method will take the ownership of the returned pointer.
      */
      TEQTWIDGETSEXPORT QImage* GetImage(te::color::RGBAColor** img, int width, int height);

      /*!
        \brief It creates a RGBA color array from a QImage.

        \param img QImage that will be converted.

        \return Image - a square matrix of int's where each position corresponds to a RGBA color.

        \note The caller of this method will take the ownership of the returned pointer.
      */
      TEQTWIDGETSEXPORT te::color::RGBAColor** GetImage(QImage* img);

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QTWIDGETS_INTERNAL_UTILS_H
