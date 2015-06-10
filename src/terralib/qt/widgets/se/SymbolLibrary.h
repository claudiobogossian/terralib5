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
  \file terralib/qt/widgets/se/SymbolLibrary.h

  \brief This class represents a library of symbols.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLLIBRARY_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLLIBRARY_H

// TerraLib
#include "../Config.h"

// STL 
#include <map>
#include <string>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class Symbol;

      /*!
        \class SymbolLibrary

        \brief This class represents a library of symbols.
      
        \sa Symbol, SymbolLibaryManager
      */
      class TEQTWIDGETSEXPORT SymbolLibrary
      {
        public:

          /*! \brief Constructor. */
          SymbolLibrary(const std::string& name);

          /*! \brief Destructor. */
          ~SymbolLibrary();

          /*!
            \brief It inserts a new symbol to this library.

            \param symbol The new symbol that will be added to this library.
          */
          void insert(Symbol* symbol);

          /*!
            \brief It removes the symbol from this library.

            \param symbol The symbol to be removed.

            \exception Exception If the symbol doesn't exist it will raise an exception.
          */
          void remove(Symbol* symbol);

          /*!
            \brief It returns the symbol identified by a given id or NULL if none is found.

            \param name The id of the symbol we are looking for.

            \return A pointer to a symbol or NULL if none is found.
          */
          Symbol* findById(const std::string& id) const;

          /*!
            \brief It returns a pair of iterators over the symbols of this library.

            \return A pair of iterators over the symbols of this library where pair.first will be
                    the beginning and pair.second will be the end iterator.
          */
          std::pair<std::map<std::string, Symbol*>::const_iterator,
                    std::map<std::string, Symbol*>::const_iterator> getIterator() const;

          const std::string& getName() const;

          /*! \brief It removes all symbols from this library. */
          void clear();

        private:

          std::string m_name;                         //!< The name of the symbol library.
          std::map<std::string, Symbol*> m_symbolMap; //!< The set of symbols.
    };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLLIBRARY_H
