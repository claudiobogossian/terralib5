/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/DataSource.h

  \brief Implementation of the data source for the PostGIS driver.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H
#define __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"


namespace te
{
  namespace gm  { class GeometryProperty; }
  namespace rst { class RasterProperty; }

  namespace pgis
  {
    // Forward declaration
    class Connection;
    class ConnectionPool;
    struct VersionInfo;

    /*!
      \class DataSource

      \brief The PostGIS driver.

      \sa te::da::DataSource, te::da::DataSourceFactory, te::da::DataSourceManager, DataSourceFactory
    */
    class TEPGISEXPORT DataSource : public te::da::DataSource
    {
      public:

        /** @name Initializer Methods
         *  Methods related to the instantiation and destruction.
         */
        //@{

        DataSource();

        ~DataSource();

        std::string getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        std::auto_ptr<te::da::DataSourceTransactor> getTransactor();

        te::pgis::Connection* getConnection();

        void closeConnection(Connection* conn);

        bool isTimeAnInteger();

        void setTimeAsInteger(bool timeIsInteger);

        /*!
          \brief It opens the connection(s) to the PostgreSQL database server.

          This method will:
          <ul>
          <li>start the connection pool;</li>
          <li>register the data type maps and get PostGIS geometry type OID;</li>
          <li>get PostgreSQL version information;</li>
          <li>get PostGIS version information;</li>
          <li>get default schema name;</li>
          <li>register user defined types (udts);</li>
          </ul>

          \exception Exception It throws an exception if the data source can not be opened.

          \note Not thread safe!
        */
        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;

        /*!
          \brief It returns the type id associated to the PostGIS Geometry type.

          \return The type id associated to the PostGIS Geometry type.

          \note PostGIS driver extended method.
        */
        unsigned int getGeomTypeId() const;

        /*!
          \brief It returns the type id associated to the PostGIS Raster type.

          \return The type id associated to the PostGIS Raster type.

          \note PostGIS driver extended method.
        */
        unsigned int getRasterTypeId() const;

        /*!
          \brief It returns the current schema associated to the database connection, or NULL, if none is set.

          \return The current schema associated to the database connection, or NULL, if none is set.

          \note PostGIS driver extended method.
        */
        const std::string& getCurrentSchema() const;
        
        /*!
          \brief It returns a pointer to the internal connection pool.

          \return A pointer to the internal connection pool.

          \note The caller must not delete the connectio pool. It is used by transactor class.

          \note PostGIS driver extended method.
        */
        ConnectionPool* getConnPool() const;

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

        std::vector<te::common::CharEncoding> getEncodings(const std::map<std::string, std::string>& dsInfo);

      private:

        std::map<std::string, std::string> m_connInfo;    //!< Connection information.
        ConnectionPool* m_pool;                           //!< The connection pool.
        unsigned int m_geomTypeOid;                       //!< PostGIS Geometry type OID.
        unsigned int m_rasterTypeOid;                     //!< PostGIS Raster type OID.
        std::string m_currentSchema;                      //!< The default schema used when no one is provided.
        bool m_timeIsInteger;                             //!< It indicates if the postgis stores, internally, time and timestamp as an integer. 

        static te::da::DataSourceCapabilities sm_capabilities;  //!< PostGIS capabilities.
        static te::da::SQLDialect* sm_dialect;                  //!< PostGIS SQL dialect.
    };

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H
