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
  \file Enums.h

  \brief Declaration of enums for MapTools module.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ENUMS_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ENUMS_H

namespace te
{
  namespace map
  {
      /*!
        \enum ImageType

        \brief This enum specifies the possible input and output image formats supported by the canvas API.
      */
      enum ImageType
      {
        PNG  = 1,    /*!< Portable Network Graphics (PNG) support.         */
        JPEG = 2,    /*!< Joint Photographic Experts Group (JPEG) support. */
        GIF  = 3,    /*!< Graphics Interchange Format (GIF) support.       */
        BMP  = 4,    /*!< Window Bitmap (BMP) support.                     */
        XPM  = 5,    /*!< X PixMap support.                                */
        XBM  = 6,    /*!< X Bitmap support.                                */
        WBMP = 7,    /*!< Wireless Application Protocol Bitmap.            */
        TIFF = 8,    /*!< Tag Image File Format (TIFF) support.            */
      };

      /*!
        \enum LineDashStyle

        \brief This enum encodes enumerated values telling how lines should be drawn. e.g. as a plain line or dash line, etc.
      */
      enum LineDashStyle
      {
        NoLine          = 0, /*!< No line at all.                                                  */
        SolidLine       = 1, /*!< A plain line.                                                    */
        DashLine        = 2, /*!< Dashes separated by a few pixels.                                */
        DotLine         = 3, /*!< Dots separated by a few pixels.                                  */
        DashDotLine     = 4, /*!< Alternate dots and dashes.                                       */
        DashDotDotLine  = 5, /*!< One dash, two dots, one dash, two dots.                          */
        CustomDashLine  = 6  /*!< A custom dash pattern. i.e. defined by a serie of double values. */
      };

      /*!
        \enum LineCapStyle

        \brief This enum encodes enumerated values telling how line strings should be capped (at the two ends of the line string).
      */
      enum LineCapStyle
      {
        ButtCap   = 0x00,   /*!< A square line end that does not cover the end point of the line. */
        SquareCap = 0x10,   /*!< A square line end that covers the end point and extends beyond it by half the line width. */
        RoundCap  = 0x20    /*!< A rounded line end. */
      };

      /*!
        \enum LineJoinStyle

        \brief This enum encodes enumerated values telling how line strings should be joined (between line segments).
      */
      enum LineJoinStyle
      {
        MiterJoin = 0x00,     /*!< The outer edges of the lines are extended to meet at an angle, and this area is filled. */
        BevelJoin = 0x40,     /*!< The triangular notch between the two lines is filled. */
        RoundJoin = 0x80      /*!< A circular arc between the two lines is filled. */
      };

      /*!
        \enum MarkerType

        \brief This enum specifies ??????.
      */
      enum PtMarkerType
      {
        MarkerNone = 0,                 /*!< ???? */
        MarkerSquare = 1,               /*!< ???? */
        MarkerDiamond = 2,              /*!< ???? */
        MarkerCircle = 3,               /*!< ???? */
        MarkerCrossedCircle = 4,        /*!< ???? */
        MarkerSemiEllipseLeft = 5,      /*!< ???? */
        MarkerSemiEllipseRight = 6,     /*!< ???? */
        MarkerSemiEllipseUp = 7,        /*!< ???? */
        MarkerSemiEllipseDown = 8,      /*!< ???? */
        MarkerTriangleLeft = 9,         /*!< ???? */
        MarkerTriangleRight = 10,       /*!< ???? */
        MarkerTriangleUp = 11,          /*!< ???? */
        MarkerTriangleDown = 12,        /*!< ???? */
        MarkerFourRays = 13,            /*!< ???? */
        MarkerCross = 14,               /*!< ???? */
        MarkerX = 15,                   /*!< ???? */
        MarkerDash = 16,                /*!< ???? */
        MarkerDot = 17,                 /*!< ???? */
        MarkerPixel = 18,               /*!< ???? */
        MarkerPattern = 19              /*!< icon marker */
      };

      /*!
        \enum AlignType

        \brief This enum contains values to control the alignment of components (like Canvas and MapDisplay).
      */
      enum AlignType
      {
        Top    = 1,   /*!< Top.     */
        Center = 2,   /*!< Center.  */
        Bottom = 3,   /*!< Bottom.  */
        Left   = 4,   /*!< Left.    */
        Right  = 5    /*!< Right.   */
      };

      /*!
        \enum Visibility

        \brief Each layer can have three states of visibility.
      */
      enum Visibility
      {
        NOT_VISIBLE,       /*!< If the layer is not visible and if it has children layers and they are not visible too. */
        VISIBLE,           /*!< If the layer is visible or all its children are visible too.                            */
        PARTIALLY_VISIBLE  /*!< If some of the children layer are visible and some are not.                             */
      };

  }   // end namespace map
}     // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ENUMS_H

