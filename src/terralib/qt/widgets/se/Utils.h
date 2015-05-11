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
  \file terralib/qt/widgets/se/Utils.h
   
  \brief This file contains utility functions for dealing with Symbology Enconding and Qt concepts.
 */

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_UTILS_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_UTILS_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

// Forward declarations
class QBrush;
class QPen;

namespace te
{
// Forward declarations
  namespace se
  {
    class Fill;
    class Stroke;
  }

  namespace xml
  {
    class Reader;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declaration
      class Symbol;

      /*!
        \brief It configs the given pen based on Symbology Enconding Stroke element.

        \param pen    The pen that will be configured.
        \param stroke The Symbology Enconding Stroke element that will be used. If NULL, Qt::transparent color will be setted on pen.

        \note Warning: It consider that the parameters of Stroke element is enconded using a Literal OGC Filter expression.
      */
      TEQTWIDGETSEXPORT void Config(QPen& pen, const te::se::Stroke* stroke);

      /*!
        \brief It configs the given brush based on Symbology Enconding Fill element.

        \param brush  The brush that will be configured.
        \param fill   The Symbology Enconding Fill element that will be used. If NULL, Qt::transparent color will be setted on brush.

        \note Warning: It consider that the parameters of Fill element is enconded using a Literal OGC Filter expression.
      */
      TEQTWIDGETSEXPORT void Config(QBrush& brush, const te::se::Fill* fill);

      /*!
        \brief It reads a symbol library file and put the result on SymbolLibraryManager.

        \param path  The path of the symbol library file.
      */
      TEQTWIDGETSEXPORT void ReadSymbolLibrary(const std::string& path);

      /*!
        \brief It reads a symbol using the given reader.

        \return The symbol read.
      */
      TEQTWIDGETSEXPORT Symbol* ReadSymbol(te::xml::Reader& reader);

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_UTILS_H
