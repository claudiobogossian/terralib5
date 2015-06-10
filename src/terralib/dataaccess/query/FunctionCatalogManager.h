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
  \file terralib/dataaccess/query/FunctionCatalogManager.h

  \brief A FunctionCatalogManager is a singleton that can be used to manage function catalogs of data source implementations.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONCATALOGMANAGER_H
#define __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONCATALOGMANAGER_H

// TerraLib
#include "../../common/Singleton.h"
#include "../Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace da
  {
// Forward declarations
    class FunctionCatalog;

    /*!
      \class FunctionCatalogManager
      
      \brief A FunctionCatalogManager is a singleton that can be used to manage function catalogs of data source implementations.

      Each data source implementation must register its function catalog
      in this singleton. This can be achieved by providing the data source
      identification token (a string) and its catalog.

      \sa FunctionCatalog
    */
    class TEDATAACCESSEXPORT FunctionCatalogManager : public te::common::Singleton<FunctionCatalogManager>
    {
      friend class te::common::Singleton<FunctionCatalogManager>;

      public:  

        /*! \brief Destructor. */
        ~FunctionCatalogManager();

        /*!
          \brief It inserts the function catalog associated to the given data source type.

          \param dsType   The type of data source.
          \param fcatalog The function catalog. The manager will take the ownership of the given function catalog.

          \exception Exception It throws an exception if a catalog is already registered for the given data source type.
        */
        void insert(const std::string& dsType, FunctionCatalog* fcatalog);

        /*!
          \brief It finds the function catalog with the given data source type.

          \param dsType The data source type.

          \return The catalog fr the given data source type.

          \note Don't delete the returned pointer, it belongs to the manager!
        */
        FunctionCatalog* find(const std::string& dsType);

      protected:   

        /*! \brief No constructor available for clients. */
        FunctionCatalogManager() {}

      private:

        std::map<std::string, FunctionCatalog*> m_fcatalogMap;  //!< Function map: datasource-type -> FunctionCatalog*.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FUNCTIONCATALOGMANAGER_H


