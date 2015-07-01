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
  \file terralib/se/SymbologyManager.h

  \brief The SymbologyManager is a singleton that can be used to manage all loaded symbols in TerraLib.
*/

#ifndef __TERRALIB_SE_INTERNAL_SYMBOLOGYMANAGER_H
#define __TERRALIB_SE_INTERNAL_SYMBOLOGYMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "Config.h"

// STL 
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace se
  {
// Forward declarations
    class Symbolizer;

    /*!
      \class SymbologyManager

      \brief The SymbologyManager is a singleton that can be used to manage all loaded symbols in TerraLib.
      
      \sa Singleton

      \todo Sera que nao devia ser uma classe chamada StyleManager? Eu acho que nao pois soh tem serventia p/ os tipos basicos de desenho que a pessoa poderia escolher previamente e neste caso nao sao estilos e sim simbolos!
      \todo Talvez seria interessante criar uma classe StyleManager com estilos completos ou pelo menos que a partir de simbolos ele jah defina tudinho: tracejado-vazado-linhas-amarelas...
    */
    class TESEEXPORT SymbologyManager : public te::common::Singleton<SymbologyManager>
    {
      friend class te::common::Singleton<SymbologyManager>;

      public:

        /** @name ColorManager Accessor Method
         *  Method used to access the data stored on this manager.
         */
        //@{

        /*!
          \brief It inserts a new catalog that will be managed by ColorManager.

          \param c The new catalog to be managed by this manager.

          \exception Exception If the catalog already exists it will raise an exception.

          \note Don't free the resources used by the catalog, the manager will take the ownership of it.
        */
        //void insert(Symbolizer* c);

        /*! \brief It removes the catalog from the system. It will not delete it from disk.

            \param c The catalog to be removed.

            \exception Exception If the catalog doesn't exist it will raise an exception.

            \note The memory pointed by c will be deallocated. Don't reference it anymore.
        */
        //void remove(Symbolizer* c);

        /*! \brief It returns the catalog identified by a given name or NULL if none is found.

            \param name The name of the catalog we are looking for.

            \return A pointer to a catalog (don't free the pointer) or NULL if none is found.
        */
        //Symbolizer* findByName(const std::string& name) const;

        /*! \brief It returns a pair of iterators over the catalogs of this manager.

            \return A pair of iterators over the catalogs of this manager where pair.first will be
                    the beginning and pair.second will be the end iterator.
        */
        //std::pair<std::vector<Symbolizer*>::const_iterator,
        //          std::vector<Symbolizer*>::const_iterator> getIterator() const;

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

        std::map<std::string, Symbolizer*> m_catalogIdxByName;  //!< An index from catalog's name to catalog's instance (note: we can not have duplicated names).
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_SYMBOLOGYMANAGER_H

