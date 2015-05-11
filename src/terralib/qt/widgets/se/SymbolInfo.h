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
  \file terralib/qt/widgets/se/SymbolInfo.h
  
  \brief Information about a given Symbol.
 */

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLINFO_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLINFO_H

// STL
#include <string>

namespace te
{
  namespace qt
  {
    namespace widgets
    {

    /*!
      \struct SymbolType
      
      \brief Enumeration that indicates the symbol type.

      \sa Symbol
    */
    enum SymbolType
    {
      PolygonSymbol, /*!< Polygon symbol type  */
      LineSymbol,    /*!< Line symbol type     */
      PointSymbol    /*!< Point symbol type    */
    };

    /*!
      \struct SymbolInfo
      
      \brief Information about a given Symbol.

      \sa Symbol
    */
    struct SymbolInfo
    {
      std::string m_id;          /*!< Symbol identifier            */
      std::string m_name;        /*!< Symbol name                  */
      std::string m_author;      /*!< Author of the symbol         */
      std::string m_tags;        /*!< Keywords for the symbol      */
      std::string m_description; /*!< Description about the symbol */
      // more... - type?
    };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_PLUGININFO_H
