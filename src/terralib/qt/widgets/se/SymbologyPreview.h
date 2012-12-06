/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/SymbologyPreview.h

  \brief Static methods used to generate preview of Symbology elements.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLOGYPREVIEW_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLOGYPREVIEW_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QPixmap>

// STL
#include <vector>

namespace te
{
// Forward declarations
  namespace gm
  {
    class Geometry;
  }

  namespace se
  {
    class Rule;
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class Symbol;

      namespace SymbologyPreview
      {

        /*!
          \brief Generates the preview of given symbolizer element.

          \param symb A valid symbolizer element.
          \param geom The geometry that will be used to draw the preview.
          \param size The preview size.

          \return A pixmap that represents the result of symbolizer preview.
        */
        TEQTWIDGETSEXPORT QPixmap build(te::se::Symbolizer* symb, te::gm::Geometry* geom, const QSize& size);

        /*!
          \brief Generates the preview of a set of symbolizers elements.

          \param symbs A set of symbolizers elements.
          \param geom The geometry that will be used to draw the preview.
          \param size The preview size.

          \return A pixmap that represents the composite result of symbolizers preview.
        */
        TEQTWIDGETSEXPORT QPixmap build(const std::vector<te::se::Symbolizer*>& symbs, te::gm::Geometry* geom, const QSize& size);

        /*!
          \brief Generates the preview of a set of symbolizers elements included in the given rule.

          \param rule A valid rule element that will be used to get the set of symbolizers.
          \param geom The geometry that will be used to draw the preview.
          \param size The preview size.

          \return A pixmap that represents the composite result of rule's symbolizers preview.
        */
        TEQTWIDGETSEXPORT QPixmap build(te::se::Rule* rule, te::gm::Geometry* geom, const QSize& size);

        /*!
          \brief Generates the preview of a symbol.

          \param symbol A valid symbol.
          \param geom The geometry that will be used to draw the preview.
          \param size The preview size.

          \return A pixmap that represents the symbol preview.
        */
        TEQTWIDGETSEXPORT QPixmap build(Symbol* symbol, te::gm::Geometry* geom, const QSize& size);

      } // end namespace SymbologyPreview
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLOGYPREVIEW_H
