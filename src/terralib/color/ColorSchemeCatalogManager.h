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
  \file terralib/color/ColorSchemeCatalogManager.h
 
  \brief The ColorSchemeCatalogManager is a singleton that can be used to manage all loaded color scheme catalogs.
*/

#ifndef __TERRALIB_COLOR_INTERNAL_COLORSCHEMECATALOGMANAGER_H
#define __TERRALIB_COLOR_INTERNAL_COLORSCHEMECATALOGMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "Config.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace color
  {
// Forward declaration
    class ColorSchemeCatalog;

    /*!
      \class ColorSchemeCatalogManager

      \brief The ColorSchemeCatalogManager is a singleton that can be used to manage all loaded color scheme catalogs.
      
      Use this class to keep the loaded catalogs and to be a point of entry for plugins that
      need to known what are the opened catalogs of your application. When the singleton finishes
      its life time it automatically clears all loaded catalogs stored on it.
      You can also use it only inside a given window, where a list of catalogs will be displayed
      and then when the window gets closed, you clear the catalogs to save memory.

      \note You can save memory not loading all catalogs and keeping then opened during the entire life of your application.

      \ingroup color

      \sa Singleton, ColorScheme, ColorSchemeGroup, ColorSchemeCatalog

      \todo Em breve, seria melhor transformar as classes ColorScheme, ColorSchemeCatalog
            em pImpl ou shared pointers, para que as pessoas nao tenham que se preocupar
            com concorrencia em nivel de thread no que diz respeito a quem pegou o ponteiro,
            quem apagou e assim por diante!

      \todo Criar uma classe chamada ApplicationPreferences que guarda qual o catalogo
            default de um dado usuario. Depois cria um classe XML que salva as preferencias num arquivo XML.
            Exemplo de pereferncias: tipo de fonte padrao, cor padrao, catalogo de cores padrao, numero de cores padrao, tipo de agrupament padrao, driver de fonte de dados padrao,
            nome usuario e senha por driver, localizacao do mapa de layout (quickview ou coisa assim),
            estilo de ponto padrao, estilo de linha padrao, estilo de poligono padrao,
            local de salvar o projeto, local de exportar dados,
            local de importar dados.... e varias outras preferencias do usuario
    */
    class TECOLOREXPORT ColorSchemeCatalogManager : public te::common::Singleton<ColorSchemeCatalogManager>
    {
      friend class te::common::Singleton<ColorSchemeCatalogManager>;

      public:

        /** @name ColorSchemeCatalogManager Accessor Method
         *  Method used to access the data stored on this manager.
         */
        //@{

        /*!
         \brief Inializes the manager from a JSON file containing instances of color schemes
       
         This methods reads the file "TE_JSON_FILES_LOCATION/color_ramps.json" for color schemes definitions and insert them on the manager if it is empty.
         \exception te::srs::Exception if the JSON file is not well formed.
         */
        void init();

        /*!
          \brief It inserts a new catalog that will be managed by ColorSchemeCatalogManager.

          \param c The new catalog to be managed by this manager.

          \exception Exception If the catalog already exists it will raise an exception.

          \note Don't free the resources used by the catalog, the manager will take the ownership of it.
         */
        void insert(ColorSchemeCatalog* c);

        /*!
          \brief It removes the internal reference to the catalog.

          \param c The catalog to be removed.

          \exception Exception If the catalog doesn't exist it will raise an exception.

          \note The caller of this method will take the ownership of the catalog.
         */
        void disconnect(ColorSchemeCatalog* c);

        /*!
          \brief It removes the catalog from the manager and clears it resources.

          \param c The catalog to be removed.

          \exception Exception If the catalog doesn't exist it will raise an exception.

          \note The memory pointed by c will be deallocated. Don't reference it anymore.
         */
        void erase(ColorSchemeCatalog* c);

        /*!
          \brief It returns the catalog identified by a given name or NULL if none is found.

          \param name The name of the catalog we are looking for.

          \return A pointer to a catalog (don't free the pointer) or NULL if none is found.
         */
        ColorSchemeCatalog* findByName(const std::string& name) const;

        /*!
          \brief It returns a pair of iterators over the catalogs of this manager.

          \return A pair of iterators over the catalogs of this manager where pair.first will be
                  the beginning and pair.second will be the end iterator.
         */
        std::pair<std::vector<ColorSchemeCatalog*>::const_iterator,
                  std::vector<ColorSchemeCatalog*>::const_iterator> getIterator() const;

        /*!
          \brief It returns the list of catalogs available in the system.

          \return The list of catalogs available in the system.
         */
        const std::vector<ColorSchemeCatalog*>& getCatalogs() const;

        /*!
          \brief It returns true if the manager contains at least one catalog. If no catalog exists, it returns false.

          \return True if the manager contains at least one catalog. If no catalog exists, it returns false.
         */
        bool isEmpty() const;

        /*! \brief It unloads all catalogs managed by ColorSchemeCatalogManager. */
        void clear();

        //@}

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Destructor. */
        ~ColorSchemeCatalogManager();

        //@}

      protected:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes the Singleton. */
        ColorSchemeCatalogManager();

        //@}

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        ColorSchemeCatalogManager(const ColorSchemeCatalogManager& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        ColorSchemeCatalogManager& operator=(const ColorSchemeCatalogManager& rhs);

        //@}

      private:

        std::vector<ColorSchemeCatalog*> m_catalogs;                    //!< This is the list of all system's loaded catalogs.
        std::map<std::string, ColorSchemeCatalog*> m_catalogIdxByName;  //!< An index from catalog's name to catalog's instance (note: we can not have duplicated names).
    };

  } // end namespace color
}   // end namespace te

#endif  // __TERRALIB_COLOR_INTERNAL_COLORSCHEMECATALOGMANAGER_H

