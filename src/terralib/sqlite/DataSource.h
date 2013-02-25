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
  \file terralib/sqlite/DataSource.h

  \brief Implements the DataSource class for the SQLite Data Access Driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_DATASOURCE_H
#define __TERRALIB_SQLITE_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"

// Forward declarations
extern "C"
{
  struct sqlite3;
  typedef struct sqlite3 sqlite3;
}

namespace te
{
  namespace sqlite
  {
// Forward declarations
    class DataSourceTransactor;

    /*!
      \class DataSource

      \brief Implements the DataSource class for SQLite Data Access Driver.

      \sa te::da::DataSource
    */
    class TESQLITEEXPORT DataSource : public te::da::DataSource
    {
      public:

        DataSource();

        ~DataSource();

        const std::string& getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;

        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        te::da::DataSourceCatalog* getCatalog() const;

        te::da::DataSourceTransactor* getTransactor();

        void optimize(const std::map<std::string, std::string>& opInfo);

        /*!
          \brief It returns a pointer to an SQLite transactor.
          
          \return A pointer to the SQLite transactor.

          \exception Exception It throws an exception if it is not possible to get a transactor, for example, if there is not an available connection.

          \note SQLite driver extended method.
        */
        DataSourceTransactor* getSQLiteTransactor();

        /*!
          \brief It returns the internal database pointer.

          \return The internal database pointer.

          \note SQLite driver extended method.
        */
        sqlite3* getDB() const;

        /*!
          \brief It sets the SQLite capabilities.
          
          \param capabilities The SQLite capabilities.

          \note SQLite driver extended method.
        */
        //static void setCapabilities(const std::map<std::string, std::string>& capabilities);

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSources(const std::map<std::string, std::string>& info);

        std::vector<std::string> getEncodings(const std::map<std::string, std::string>& info);

      private:

        std::map<std::string, std::string> m_connInfo;            //!< DataSource information.
        sqlite3* m_db;                                            //!< SQLite db handle.
        te::da::DataSourceCatalog* m_catalog;                     //!< The main system catalog.
        const te::da::SQLDialect* m_dialect;                      //!< The SQL dialect of the data source (note: the data source doesn't have the ownership of this pointer).
        bool m_useSpatiaLite;                                     //!< If true this driver works over SpatiaLite, otherwise, it uses OGR format.
        bool m_isrw;                                              //!< If true indicates that the database is opened for read and write, otherwise, just read access.

        static te::da::DataSourceCapabilities sm_capabilities;    //!< SQLite capabilities.
    };

    inline sqlite3* DataSource::getDB() const
    {
      return m_db;
    }

  } // end namespace sqlite
}   // end namespace te

#endif  // __TERRALIB_SQLITE_INTERNAL_DATASOURCE_H

