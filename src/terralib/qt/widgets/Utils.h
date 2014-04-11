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
#include "../../common/CharEncodingConv.h"
#include "../../color/RGBAColor.h"
#include "../../geometry/Enums.h"
#include "../../maptools/Enums.h"
#include "Config.h"
#include "Globals.h"

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtGui/QPixmap>

// STL
#include <string>

// Forward declarations
class QAction;
class QImage;
class QMenu;
class QMenuBar;
class QTreeWidgetItem;

namespace te
{
  namespace map
  {
    class Canvas;
  }

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

      /*!
        \brief

        \param
        \param

        \return
      */
      TEQTWIDGETSEXPORT QAction* FindAction(const QString& actText, QMenu* mnu);

      /*!
        \brief

        \param
        \param

        \return
      */
      TEQTWIDGETSEXPORT QAction* FindAction(const QString& actText, QMenuBar* mnuBar);

       /*!
        \brief It converts a Qt Color to TerraLib Color.

        \param color The Qt Color that will be converted.
      */
      TEQTWIDGETSEXPORT te::color::RGBAColor Convert2TerraLib(const QColor& color);

      /*!
        \brief It converts a TerraLib Color to Qt Color.

        \param color The TerraLib Color that will be converted.
      */
      TEQTWIDGETSEXPORT QColor Convert2Qt(const te::color::RGBAColor& color);

      /*!
        \brief It configs (i.e. prepares) the given canvas to draw polygons.

        \param canvas The canvas that will be configured. Do not pass NULL!
        \param fillColor The color that will be used to fill the polygons.
        \param contourColor The color that will be used to contour the polygons.
        \param contourWidth The width that will be used to contour the polygons.
      */
      TEQTWIDGETSEXPORT void Config2DrawPolygons(te::map::Canvas* canvas, const QColor& fillColor, const QColor& contourColor, const std::size_t& contourWidth = 1);

      /*!
        \brief It configs (i.e. prepares) the given canvas to draw lines.

        \param canvas The canvas that will be configured. Do not pass NULL!
        \param color The color that will be used to draw the lines.
        \param width The width that will be used to draw the lines.
      */
      TEQTWIDGETSEXPORT void Config2DrawLines(te::map::Canvas* canvas, const QColor& color, const std::size_t& width = 1);

      /*!
        \brief It configs (i.e. prepares) the given canvas to draw points.

        \param canvas The canvas that will be configured. Do not pass NULL!
        \param color The color that will be used to draw the points.
        \param width The width that will be used to draw the points.
      */
      TEQTWIDGETSEXPORT void Config2DrawPoints(te::map::Canvas* canvas, const QColor& color, const std::size_t& width = 1);

       /*!
        \brief It configs (i.e. prepares) the given canvas to draw points.

        \param canvas The canvas that will be configured. Do not pass NULL!
        \param markName The mark name that will be used to draw the points.
        \param size The size that will be used to draw the points.
        \param fillColor The color that will be used to fill the mark.
        \param contourColor The color that will be used to contour the mark.
        \param contourWidth The width that will be used to contour the mark.
      */
      TEQTWIDGETSEXPORT void Config2DrawPoints(te::map::Canvas* canvas, const QString& markName, const std::size_t& size,
                                               const QColor& fillColor, const QColor& contourColor, const std::size_t& contourWidth = 1);

      /*!
        \brief It configs (i.e. prepares) the given canvas to draw a layer selection.

        \param canvas The canvas that will be configured. Do not pass NULL!
        \param selectionColor The color that will be used to draw the selection.
        \param type The geometry type that will be drawn.
      */
      TEQTWIDGETSEXPORT void Config2DrawLayerSelection(te::map::Canvas* canvas, const QColor& selectionColor, const te::gm::GeomType& type);

      /*!
        \brief It creates a pixmap to use as icon (to be used as legend icon).

        \param size         The height and width for the pixmap.
        \param penColor     The color that will be used to set the pen.
        \param brushColor   The color that will be used to set the brush.
        \param contourSize  The countour size.
      */
      TEQTWIDGETSEXPORT QPixmap CreatePixmapIcon(const int& size, const QColor& penColor, const QColor& brushColor, const int& contourSize);

      /*!
        \brief Save last used path in QSettings.

        \param path The path to last used file.

        \param typeFile The type of file opened. Actually we assume three values: "vector", "raster", "database". Any other value may be not used in the system. 

        \note The params are case sensitive. It means that a "Vector" value for the argument \a typeFile is unkowned by application.
      */
      TEQTWIDGETSEXPORT void AddFilePathToSettings(const QString& path, const QString& typeFile);

      /*!
        \brief Returns the value of the last saved file path for the \a typeFile required.

        \param typeFile The type of file opened. Actually we assume three values: "vector", "raster", "database". Any other value may be not used in the system.

        \return The path if encountered or a null QString.
      */
      TEQTWIDGETSEXPORT QString GetFilePathFromSettings(const QString& typeFile);

      /*!
        \brief It converts the given string to an appropriate character encoding.

        \param text      The source string.
        \param encoding  The type of the character encoding of the string to be converted to an appropriate charset.

        \return The converted string.
      */
      TEQTWIDGETSEXPORT QString Convert2Qt(const std::string& text, const te::common::CharEncoding& encoding);

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QTWIDGETS_INTERNAL_UTILS_H
