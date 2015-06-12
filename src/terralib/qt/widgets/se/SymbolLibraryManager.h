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
  \file terralib/qt/widgets/se/SymbolLibraryManager.h

  \brief The SymbolLibraryManager is a singleton that can be used to manage all loaded symbol libraries in TerraLib.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLLIBRARYMANAGER_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLLIBRARYMANAGER_H

// TerraLib
#include "../../../common/Singleton.h"
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
      class SymbolLibrary;

      /*!
        \class SymbolLibraryManager

        \brief The SymbolLibraryManager is a singleton that can be used to manage all loaded symbol libraries in TerraLib.
      
        \sa Symbol, SymbolLibrary, Singleton
      */
      class TEQTWIDGETSEXPORT SymbolLibraryManager : public te::common::Singleton<SymbolLibraryManager>
      {
        friend class te::common::Singleton<SymbolLibraryManager>;

        public:

          /** @name SymbolLibraryManager Accessor Method
           *  Method used to access the data stored on this manager.
           */
          //@{

          /*!
            \brief It inserts a new symbol library that will be managed by SymbolLibraryManager.

            \param library The new symbol library to be managed by this manager.
          */
          void insert(SymbolLibrary* library);

          /*!
            \brief It removes the symbol library from the manager.

            \param library The symbol library to be removed.

            \exception Exception If the symbol library doesn't exist it will raise an exception.
          */
          void remove(SymbolLibrary* library);

          /*!
            \brief It returns the symbol library identified by a given name or NULL if none is found.

            \param name The name of the symbol library we are looking for.

            \return A pointer to a symbol library or NULL if none is found.
          */
          SymbolLibrary* findByName(const std::string& name) const;

          /*!
            \brief It returns a pair of iterators over the symbol libraries of this manager.

            \return A pair of iterators over the symbol libraries of this manager where pair.first will be
                    the beginning and pair.second will be the end iterator.
          */
          std::pair<std::map<std::string, SymbolLibrary*>::const_iterator,
                    std::map<std::string, SymbolLibrary*>::const_iterator> getIterator() const;

          /*! \brief It unloads all symbol libraries managed by SymbolLibraryManager. */
          void clear();

          //@}

          /*! \brief Destructor. */
          ~SymbolLibraryManager();

        protected:

          /*! \brief It initializes the Singleton. */
          SymbolLibraryManager();

        private:

          std::map<std::string, SymbolLibrary*> m_symbolLibraryMap; //!< The set of symbol libraries.
    };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLLIBRARYMANAGER_H
