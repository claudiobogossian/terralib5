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
  \file terralib/annotationtext/Enums.h

  \brief Several enumerations related to Annotation Text.
 */

#ifndef __TERRALIB_ANNOTATIONTEXT_INTERNAL_ENUMS_H
#define __TERRALIB_ANNOTATIONTEXT_INTERNAL_ENUMS_H

namespace te
{
  namespace at
  {
    /*!
      \enum FontStyle

      \brief Font style for drawing text.
     */
    enum FontStyle
    {
      Italic = 1,      /*!< An italic style. */
      NormalStyle = 2, /*!< A normal style.  */
      Oblique = 4      /*!< A oblique style. */
    };

    /*!
      \enum FontWeight

      \brief Font weight for drawing text.
     */
    enum FontWeight
    {
      Light = 100,          /*!< ???? */
      NormalWeight = 200,   /*!< ???? */
      DemiBold = 300,       /*!< ???? */
      Bold = 400,           /*!< ???? */
      ExtraBold = 500,      /*!< ???? */
      UltraBold = 600,      /*!< ???? */
      Black = 700,          /*!< ???? */
      ExtraBlack = 800,     /*!< ???? */
      UltraBlack = 900      /*!< ???? */
    };

    /*!
      \enum TextDecoration

      \brief Text decoration for drawing text.
     */
    enum TextDecoration
    {
      None = 1,           /*!< ???? */
      LineThrought = 2,   /*!< ???? */
      Underline = 4,      /*!< ???? */
      Overline =8         /*!< ???? */
    };

    /*!
      \enum HorizontalAlignment

      \brief Horizontal alignment for drawing text.
     */
    enum HorizontalAlignment
    {
      Start = 1,    /*!< ???? */
      CenterH = 2,  /*!< ???? */
      End = 4       /*!< ???? */
    };

    /*!
      \enum VerticalAlignment

      \brief Vertical alignment for drawing text.
     */
    enum VerticalAlignment
    {
      Top = 1,         /*!< ???? */
      CenterV = 2,     /*!< ???? */
      Baseline = 4,    /*!< ???? */
      Bottom = 8       /*!< ???? */
    };

    /*!
      \enum LineJustification

      \brief Line justification for drawing multi line text.
     */
    enum LineJustification
    {
      Left = 1,     /*!< ???? */
      Center = 2,   /*!< ???? */
      Right = 4     /*!< ???? */
    };
  } // end namespace at
}   // end namespace te

#endif  // __TERRALIB_ANNOTATIONTEXT_INTERNAL_ENUMS_H

