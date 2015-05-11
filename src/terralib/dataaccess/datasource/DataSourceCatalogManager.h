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
  \file terralib/dataaccess/datasource/DataSourceCatalogManager.h

  \brief This is a singleton for managing all the data source catalog instances available in the system.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOGMANAGER_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOGMANAGER_H

// TerraLib
#include "../../common/Comparators.h"
#include "../../common/Singleton.h"
#include "../../common/ThreadingPolicies.h"
#include "DataSource.h"
#include "DataSourceCatalog.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace da
  {
    /*!
      \class DataSourceCatalogManager

      \brief This is a singleton for managing all the data source catalog instances available in the system.

      \ingroup dataaccess

      \sa DataSource, DataSourceCatalog, te::common::Singleton
    */
    class TEDATAACCESSEXPORT DataSourceCatalogManager : public te::common::ObjectLevelLockable<DataSourceCatalogManager,
                                                                                               ::boost::recursive_mutex,
                                                                                               ::boost::lock_guard< ::boost::recursive_mutex>,
                                                                                               ::boost::lock_guard< ::boost::recursive_mutex> >,
                                                 public te::common::Singleton<DataSourceCatalogManager>
    {
      friend class te::common::Singleton<DataSourceCatalogManager>;

      public:

        typedef std::map<DataSourcePtr, DataSourceCatalogPtr>::const_iterator const_iterator;
        typedef std::map<DataSourcePtr, DataSourceCatalogPtr>::iterator iterator;

        /*!
          \brief It creates a catalog for the given data source, loads their sequences and the information
                 about the schemas of the datasets contained in the data source. The catalog created is
                 registered into the manager and it is returned to the caller.

          \param ds The data source whose catalog will be created.

          \return A pointer to the new data source catalog.

          \exception Exception It throws an exception if there is already a catalog to the given data source.

          \note Thread-safe!
        */
        DataSourceCatalogPtr create(DataSourcePtr ds);

        /*!
          \brief It gets the catalog associated to the given data source. If there is no catalog registered
                 in the manager, a catalog is created and registered in the manager.

          \param ds The data source whose catalog is to be retrieved.

          \return The catalog associated to the data source.

          \note Thread-safe!
        */
        DataSourceCatalogPtr get(DataSourcePtr ds);

        /*!
          \brief It tries to find the catalog associated to the given data source.

          \param ds The data source whose catalog is to be found.

          \return True, if the catalog associated to the data source is found; otherwise, it returns false.

          \note Thread-safe!
        */
        bool find(const DataSourcePtr& ds) const;

        /*!
          \brief It inserts the data source catalog into the manager.
                 The catalog must have a data source in order to be inserted.

          \param catalog The catalog to be inserted into the manager.

          \note The manager will take the ownership of the catalog.

          \exception Exception It throws an exception, if already exists a catalog for the data source of the given catalog.

          \note Thread-safe!
        */
        void insert(const DataSourceCatalogPtr& catalog);

        /*!
          \brief It changes the ownership of the data source catalog to the caller.

                 The memory used by the given data source catalog will NOT BE released.
                 In other words, you will take the ownership of the data source catalog.

          \param ds The data source catalog to be detached.

          \note Thread-safe!
        */
        void detach(DataSourceCatalogPtr catalog);

        /*!
          \brief It changes the ownership of the data source catalog associated to the given data source.

          \param ds The data source.

          \return The catalog associated to the given data source. The caller will take the ownership of
                  this catalog.

          \note Thread-safe!
        */
        DataSourceCatalogPtr detach(const DataSourcePtr& ds);

        /*!
          \brief All the catalogs whose data sources are of the specified type are detached from the manager.

          \note Thread-safe!
        */
        void detachAll(const std::string& dsType);

        /*!
          \brief All the catalogs are detached from the manager.

          \note Thread-safe!
        */
        void detachAll();

        /*!
          \brief It returns the number of data source catalogs that the manager are keeping track of.
 
          \return The number of tracked data source catalogs.
        */
        std::size_t size() const;

        /*!
          \brief It returns an iterator to the beginning of the container.

          \return A constant iterator to the beginning of the container.
        */
        const_iterator begin() const;

        /*!
          \brief It returns an iterator to the beginning of the container.

          \return An iterator to the beginning of the container.
        */
        iterator begin();

        /*!
          \brief It returns an iterator to the end of the container.

          \return An iterator to the beginning of the container.
        */
        const_iterator end() const;

        /*!
          \brief It returns an iterator to the end of the container.

          \return An iterator to the beginning of the container.
        */
        iterator end();

      protected:

        /*! \brief It initializes the singleton instance of the data source catalog manager. */
        DataSourceCatalogManager();

        /*! \brief Singleton destructor. */
        ~DataSourceCatalogManager();

      private:
        
        std::map<DataSourcePtr, DataSourceCatalogPtr> m_catalogs;   //!< The catalogs registered in the manager.
    };

    inline std::size_t DataSourceCatalogManager::size() const
    {
      return m_catalogs.size();
    }

    inline DataSourceCatalogManager::const_iterator DataSourceCatalogManager::begin() const
    {
      return m_catalogs.begin();
    }

    inline DataSourceCatalogManager::iterator DataSourceCatalogManager::begin()
    {
      return m_catalogs.begin();
    }

    inline DataSourceCatalogManager::const_iterator DataSourceCatalogManager::end() const
    {
      return m_catalogs.end();
    }

    inline DataSourceCatalogManager::iterator DataSourceCatalogManager::end()
    {
      return m_catalogs.end();
    }

  } // end namespace da
}   // end namespace te


#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCECATALOGMANAGER_H
