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
  \file Utils.h

  \brief Utility functions for Symbology Enconding module.
*/

#ifndef __TERRALIB_SE_INTERNAL_UTILS_H
#define __TERRALIB_SE_INTERNAL_UTILS_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace se
  {
// Forward declarations
    class Fill;
    class Graphic;
    class LineSymbolizer;
    class Mark;
    class PointSymbolizer;
    class PolygonSymbolizer;
    class Stroke;

    /*!
      \brief Creates a stroke.

      \param color The stroke color enconded hexadecimal i.e "#FF0000".
      \param width The stroke width.

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(const std::string& color, const std::string& width);

    /*!
      \brief Creates a stroke.

      \param color   The stroke color enconded hexadecimal i.e "#FF0000".
      \param width   The stroke width.
      \param opacity The stroke opacity [0,1].

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(const std::string& color, const std::string& width, const std::string& opacity);

    /*!
      \brief Creates a stroke.

      \param color     The stroke color enconded hexadecimal i.e "#FF0000".
      \param width     The stroke width.
      \param opacity   The stroke opacity [0,1].
      \param dasharray The stroke dasharray.

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(const std::string& color, const std::string& width,
                                    const std::string& opacity, const std::string& dasharray);

    /*!
      \brief Creates a stroke.

      \param color     The stroke color enconded hexadecimal i.e "#FF0000".
      \param width     The stroke width.
      \param opacity   The stroke opacity [0,1].
      \param dasharray The stroke dasharray.
      \param linecap   The stroke linecap {butt, square, round}.
      \param linejoin  The stroke linejoin {miter, bevel, round}.

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(const std::string& color, const std::string& width,
                                    const std::string& opacity, const std::string& dasharray,
                                    const std::string& linecap, const std::string& linejoin);

    /*!
      \brief Creates a stroke.

      \param graphic   The stroke graphic fill.
      \param color     The stroke color enconded hexadecimal i.e "#FF0000".
      \param width     The stroke width.
      \param opacity   The stroke opacity [0,1].
      \param dasharray The stroke dasharray.
      \param linecap   The stroke linecap {butt, square, round}.
      \param linejoin  The stroke linejoin {miter, bevel, round}.

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(Graphic* graphicFill,
                                    const std::string& color, const std::string& width,
                                    const std::string& opacity, const std::string& dasharray,
                                    const std::string& linecap, const std::string& linejoin);

    /*!
      \brief Creates a fill.

      \param color   The fill color enconded hexadecimal i.e "#FF0000".
      \param opacity The fill opacity.

      \return The Symbology Enconding Fill element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Fill* CreateFill(const std::string& color, const std::string& opacity);

    /*!
      \brief Creates a fill.

      \param graphic The graphic fill.

      \return The Symbology Enconding Fill element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Fill* CreateFill(Graphic* graphicFill);

    /*!
      \brief Creates a mark.

      \param wellKnownName The mark well-known name.
      \param stroke        The mark stroke.
      \param fill          The mark fill.

      \return The Symbology Enconding Mark element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Mark* CreateMark(const std::string& wellKnownName, Stroke* stroke, Fill* fill);

    /*!
      \brief Creates a graphic.

      \param mark     The graphic mark.
      \param size     The graphic size.
      \param rotation The graphic rotation.
      \param opacity  The graphic opacity.

      \return The Symbology Enconding Graphic element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Graphic* CreateGraphic(Mark* mark, const std::string& size, const std::string& rotation, const std::string& opacity);

    /*!
      \brief Creates a polygon symbolizer.

      \param stroke The polygon symbolizer stroke.
      \param fill   The polygon symbolizer fill.

      \return The Symbology Enconding PolygonSymbolizer element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT PolygonSymbolizer* CreatePolygonSymbolizer(Stroke* stroke, Fill* fill);

    /*!
      \brief Creates a line symbolizer.

      \param stroke The line symbolizer stroke.

      \return The Symbology Enconding LineSymbolizer element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT LineSymbolizer* CreateLineSymbolizer(Stroke* stroke);

    /*!
      \brief Creates a point symbolizer.

      \param graphic The point symbolizer graphic.

      \return The Symbology Enconding PointSymbolizer element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT PointSymbolizer* CreatePointSymbolizer(Graphic* graphic);

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_UTILS_H
