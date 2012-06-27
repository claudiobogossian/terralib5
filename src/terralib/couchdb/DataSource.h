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
  \file terralib/couchdb/DataSource.h

  \brief Implements the DataSource class for CouchDB.
*/

#ifndef __TERRALIB_COUCHDB_INTERNAL_DATASOURCE_H
#define __TERRALIB_COUCHDB_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "Config.h"

// STL
#include <memory>

namespace te
{
  namespace couchdb
  {
    /*!
      \class DataSource

      \brief Implements the DataSource class for CouchDB.

      \sa te::da::DataSource
    */
    class TECOUCHDBEXPORT DataSource : public te::da::DataSource
    {
      public:

        DataSource();

        ~DataSource();

        const std::string& getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        void getCapabilities(std::map<std::string, std::string>& capabilities) const;

        const te::da::SQLDialect* getDialect() const;

        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        te::da::DataSourceCatalog* getCatalog() const;

        te::da::DataSourceTransactor* getTransactor();

        void optimize(const std::map<std::string, std::string>& opInfo);

        /*!
          \brief Returns the CouchDB server base URI.

          \return The URI used to identify the CouchDB server.

          \note CouchDB driver extended method.
        */
        const std::string& getServerURI() const;

        /*!
          \brief Returns the CouchDB database name.

          \return The CouchDB database name.

          \note CouchDB driver extended method.
        */
        const std::string& getDBName() const;

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

      private:

        std::map<std::string, std::string> m_dsInfo;        //!< The base data source information.
        std::string m_serverURI;                            //!< The database server base URI.
        std::string m_dbName;                               //!< The database name.
        std::string m_srvVersion;                           //!< The database server version.
        std::auto_ptr<te::da::DataSourceCatalog> m_catalog; //!< The database catalog
        bool m_isOpened;                                    //!< If true the database was open successfuly
    };

    inline const std::string& DataSource::getServerURI() const
    {
      return m_serverURI;
    }

    inline const std::string& DataSource::getDBName() const
    {
      return m_dbName;
    }

  } // end namespace couchdb
}   // end namespace te

#endif  // __TERRALIB_COUCHDB_INTERNAL_DATASOURCE_H

