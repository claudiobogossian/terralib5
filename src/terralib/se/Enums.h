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
  \file terralib/se/Enums.h

  \brief Enumerations related to Symbology Enconding module.
*/

#ifndef __TERRALIB_SE_INTERNAL_ENUMS_H
#define __TERRALIB_SE_INTERNAL_ENUMS_H

namespace te
{
  namespace se
  {
    /*!
      \enum ColorCompositionType

      \brief Allowed color compositions type.
    */
    enum ColorCompositionType
    {
      RGB_COMPOSITION,      /*!< Color composition with 3 bands                 */
      RED_COMPOSITION,      /*!< Color composition with 1 band, red channel     */
      GREEN_COMPOSITION,    /*!< Color composition with 1 band, green channel   */
      BLUE_COMPOSITION,     /*!< Color composition with 1 band, blue channel    */
      GRAY_COMPOSITION,     /*!< Color composition with 1 band, gray channel    */
      UNKNOWN_COMPOSITION   /*!< Color composition unknown or not defined       */
    };

    /*!
      \enum ColorMapTransformationType

      \brief Allowed color transformations type.
    */
    enum ColorMapTransformationType
    {
      CATEGORIZE_TRANSFORMATION,  /*!< The transformation of continuous values to a number of values (Interpolate function). */
      INTERPOLATE_TRANSFORMATION, /*!< The transformation of continuous values to distinct values (Categorize function)      */
      RECODE_TRANSFORMATION       /*!< Transformation of discrete values to other values.                                    */
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_ENUMS_H
