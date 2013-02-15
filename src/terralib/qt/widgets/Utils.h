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
#include <QMessageBox>

// Forward declarations
class QImage;
class QMenu;
class QMenuBar;
class QTreeWidgetItem;

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

      /*!
        \brief Finds a menu item in the \a mnu object.

        Use this function to find menus and submenus. There is a sintaxe that is for required sub-menus. Use the names separated by \".\" character,
        e.g to return a menu named \"Layers\" inside another menu named \"Add\" calls this function using "Add.Layers" as the value of the 
        \a mnuText argument.

        \note Pay attention on the translations.

        \param mnuText Name of the QMenu searched.

        \param mnu The menu where the submenu must be searched.

        \return The pointer to the QMenu object or NULL if it do not exists.
      */
      TEQTWIDGETSEXPORT QMenu* FindMenu(const QString& mnuText, QMenu* mnu);

      /*!
        \overload QMenu* findMenu(const QString& mnuText, QMenuBar* bar)

        The difference for the above function is that this will search in all QMenu objects contained in the \a bar.
      */
      TEQTWIDGETSEXPORT QMenu* FindMenu(const QString& mnuText, QMenuBar* bar);

      /*!
        \brief Gets a menu or submenu contained in the \a mnu object.

        This function searches by the menu exactly as the FindMenu function. The difference is that if the menu do not exists,
        this function will create it, in all levels that may be required. Use the syntax described above to the search / create the submenus desired.

        \param mnuText Name of the menu. (This will be used as texts of the menus.)

        \param mnu The menu that will used in search.

        \return A pointer to a QMenu object.
      */
      TEQTWIDGETSEXPORT QMenu* GetMenu(const QString& mnuText, QMenu* mnu);

      /*!
        \overload QMenu* getMenu(const QString& mnuText, QMenuBar* bar)

        The difference for the above function is that it will search in all QMenu objects contained in the \a bar.
      */
      TEQTWIDGETSEXPORT QMenu* GetMenu(const QString& mnuText, QMenuBar* bar);

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QTWIDGETS_INTERNAL_UTILS_H
