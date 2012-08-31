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
  \file terralib/qt/widgets/se/SymbologyManager.h

  \brief The SymbologyManager is a singleton that can be used to manage all loaded symbols in TerraLib.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLOGYMANAGER_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLOGYMANAGER_H

// TerraLib
#include "../../../common/Singleton.h"
#include "../Config.h"

// STL 
#include <map>
#include <string>
#include <vector>

namespace te
{
// Forward declarations
  namespace se
  {
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class Symbol;

      /*!
        \class SymbologyManager

        \brief The SymbologyManager is a singleton that can be used to manage all loaded symbols in TerraLib.
      
        \sa Singleton
      */
      class TEQTWIDGETSEXPORT SymbologyManager : public te::common::Singleton<SymbologyManager>
      {
        friend class te::common::Singleton<SymbologyManager>;

        public:

          /** @name SymbologyManager Accessor Method
           *  Method used to access the data stored on this manager.
           */
          //@{

          /*!
            \brief It inserts a new symbol that will be managed by SymbologyManager.

            \param symbol The new symbol to be managed by this manager.

            \exception Exception If the symbol doesn't exist it will raise an exception.
          */
          void insert(Symbol* symbol);

          /*!
            \brief It removes the symbol from the manager.

            \param symbol The symbol to be removed.

            \exception Exception If the symbol doesn't exist it will raise an exception.
          */
          void remove(Symbol* symbol);

          /*!
            \brief It returns the symbol identified by a given id or NULL if none is found.

            \param name The id of the symbol we are looking for.

            \return A pointer to a symbol or NULL if none is found.
          */
          Symbol* findById(const std::size_t& id) const;

          /*!
            \brief It returns a pair of iterators over the symbols of this manager.

            \return A pair of iterators over the symbols of this manager where pair.first will be
                      the beginning and pair.second will be the end iterator.
          */
          std::pair<std::map<std::size_t, Symbol*>::const_iterator,
                    std::map<std::size_t, Symbol*>::const_iterator> getIterator() const;

          /*! \brief It unloads all symbols managed by SymbologyManager. */
          void clear();

          //@}

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Destructor. */
          ~SymbologyManager();

          //@}

        protected:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief It initializes the Singleton. */
          SymbologyManager();

          //@}

        private:

          std::map<std::size_t, Symbol*> m_symbolMap; //!< A list of symbols, a map from symbol id to symbol instance.
    };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLOGYMANAGER_H
