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
  \file Enums.h

  \brief Declaration of enums for the MapTools module.
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
        TIFF = 8    /*!< Tag Image File Format (TIFF) support.            */
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
        Top = 1,      /*!< Top.     */
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

      /*!
        \enum GroupingType

        \brief The grouping type associated to the layer.
      */
      enum GroupingType
      {
        EQUAL_STEPS,       /*!< If the legend was generated using the equal steps algorithm.                            */
        QUANTIL,           /*!< If the legend was generated using the quantil algorithm.                                */
        STD_DEVIATION,     /*!< If the legend was generated using the standard deviation algorithm.                     */
        UNIQUE_VALUE       /*!< If the legend was generated using the unique value algorithm.                           */
      };

      /*!
        \enum ChartType

        \brief The chart types.
      */
      enum ChartType
      {
        Pie, /*!< A pie chart. */
        Bar  /*!< A bar chart. */
      };

      /*!
        \enum CompositionMode

        \brief The composition mode used to render the canvas
      */
      enum CompositionMode
      {
        SourceOver,         /*!< This is the default mode. The alpha of the source is used to blend the pixel on top of the destination.                                                                                  */
        DestinationOver,    /*!< The alpha of the destination is used to blend it on top of the source pixels.                                                                                                            */
        Clear,              /*!< The pixels in the destination are cleared (set to fully transparent) independent of the source.                                                                                          */
        Source,             /*!< The output is the source pixel. (This means a basic copy operation and is identical to SourceOver when the source pixel is opaque).                                                      */
        Destination,        /*!< The output is the destination pixel. This means that the blending has no effect.                                                                                                         */
        SourceIn,           /*!< The output is the source, where the alpha is reduced by that of the destination.                                                                                                         */
        DestinationIn,      /*!< The output is the destination, where the alpha is reduced by that of the source.                                                                                                         */
        SourceOut,          /*!< The output is the source, where the alpha is reduced by the inverse of destination.                                                                                                      */
        DestinationOut,     /*!< The output is the destination, where the alpha is reduced by the inverse of the source.                                                                                                  */
        SourceAtop,         /*!< The source pixel is blended on top of the destination, with the alpha of the source pixel reduced by the alpha of the destination pixel.                                                 */
        DestinationAtop,    /*!< The destination pixel is blended on top of the source, with the alpha of the destination pixel is reduced by the alpha of the destination pixel.                                         */
        Xor,                /*!< The source, whose alpha is reduced with the inverse of the destination alpha, is merged with the destination, whose alpha is reduced by the inverse of the source alpha.                 */
        Plus,               /*!< Both the alpha and color of the source and destination pixels are added together.                                                                                                        */
        Multiply,           /*!< The output is the source color multiplied by the destination. Multiplying a color with white leaves the color unchanged, while multiplying a color with black produces black.            */
        Screen,             /*!< The source and destination colors are inverted and then multiplied. Screening a color with white produces white, whereas screening a color with black leaves the color unchanged.        */
        Overlay,            /*!< Multiplies or screens the colors depending on the destination color. The destination color is mixed with the source color to reflect the lightness or darkness of the destination.       */
        Darken,             /*!< The darker of the source and destination colors is selected.                                                                                                                             */
        Lighten,            /*!< The lighter of the source and destination colors is selected.                                                                                                                            */
        ColorDodge,         /*!< The destination color is brightened to reflect the source color. A black source color leaves the destination color unchanged.                                                            */
        ColorBurn,          /*!< The destination color is darkened to reflect the source color. A white source color leaves the destination color unchanged.                                                              */
        HardLight,          /*!< Multiplies or screens the colors depending on the source color. A light source color will lighten the destination color, whereas a dark source color will darken the destination color.  */
        SoftLight,          /*!< Darkens or lightens the colors depending on the source color. Similar to CompositionMode_HardLight.                                                                                      */
        Difference,         /*!< Subtracts the darker of the colors from the lighter. Painting with white inverts the destination color, whereas painting with black leaves the destination color unchanged.              */
        Exclusion           /*!< Similar to CompositionMode_Difference, but with a lower contrast. Painting with white inverts the destination color, whereas painting with black leaves the destination color unchanged. */
      };

  }   // end namespace map
}     // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ENUMS_H

