/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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

  \brief The PostGIS driver.  
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H
#define __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "Config.h"

namespace te
{
  namespace pgis
  {
// Forward declaration
    class ConnectionPool;
    class Transactor;
    struct VersionInfo;

    /*!
      \class DataSource

      \brief The PostGIS driver.

      \sa te::da::DataSource, te::da::DataSourceFactory, te::da::DataSourceManager, Transactor, DataSourceFactory
    */
    class TEPGISEXPORT DataSource : public te::da::DataSource
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        DataSource();

        ~DataSource();

        const std::string& getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        /*! \todo No futuro, ler diretamente de um arquivo XML + alguma coisa do proprio BD. */
        const te::da::DataSourceCapabilities& getCapabilities() const;

        /*! \todo No futuro, ler diretamente de um arquivo XML + alguma coisa do proprio BD. */
        const te::da::SQLDialect* getDialect() const;

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

          \note This method doesn't load the data source catalog.
          \note Not thread safe!
        */
        void open();
        
        void close();
       
        bool isOpened() const;
        
        bool isValid() const;

        te::da::DataSourceCatalog* getCatalog() const;

        te::da::DataSourceTransactor* getTransactor();

        void optimize(const std::map<std::string, std::string>& opInfo);

        /*!
          \brief It returns a pointer to the PostGIS transactor.
          
          This transactor has special methods that helps querying the data source.
          Use this method to get an object that allows to retrieve
          dataset, to insert data or to modify dataset types schemas.

          \return A pointer to the PostGIS transactor.

          \exception Exception It throws an exception if it is not possible to get a Transactor, for example, if there is not an available connection.

          \note PostGIS driver extended method.
        */
        Transactor* getPGTransactor();

        /*!
          \brief It returns a pointer to the internal connection pool.

          \return A pointer to the internal connection pool.          

          \note The caller must not delete the connectio pool. It is used by transactor class.

          \note PostGIS driver extended method.
        */
        ConnectionPool* getConnPool() const { return m_pool; }

        /*!
          \brief It returns the type id associated to the PostGIS Geometry type.

          \return The type id associated to the PostGIS Geometry type.

          \note PostGIS driver extended method.
        */
        unsigned int getGeomTypeId() const { return m_geomTypeOid; }

        /*!
          \brief It returns the type id associated to the PostGIS Raster type.

          \return The type id associated to the PostGIS Raster type.

          \note PostGIS driver extended method.
        */
        unsigned int getRasterTypeId() const { return m_rasterTypeOid; }

        /*!
          \brief It returns the current schema associated to the database connection or NULL if none is set.

          \return The current schema associated to the database connection or NULL if none is set.

          \note PostGIS driver extended method.
        */
        const std::string* getCurrentSchema() const { return m_currentSchema; }

        /*!
          \brief It sets the SQL dialect used by the PostGIS driver.

          \param myDialect The PostGIS SQL dialect to be used when translating a query.

          \note The PostGIS data source will take the ownership of the given dialect.

          \note PostGIS driver extended method.
        */
        static void setDialect(te::da::SQLDialect* myDialect);

        /*!
          \brief It sets the capabilities document.
          
          \param capabilities The PostGIS data source capabilities.

          \note The PostGIS data source will take the ownership of the given capabilities object.

          \note PostGIS driver extended method.
        */
        static void setCapabilities(const te::da::DataSourceCapabilities& capabilities);

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

      private:

        std::map<std::string, std::string> m_connectionInfo;    //!< Connection information.
        te::da::DataSourceCatalog* m_catalog;                   //!< The main system catalog.
        ConnectionPool* m_pool;                                 //!< The connection pool.
        unsigned int m_geomTypeOid;                             //!< PostGIS Geometry type OID.
        unsigned int m_rasterTypeOid;                           //!< PostGIS Raster type OID.
        std::string* m_currentSchema;                           //!< The default schema used when no one is provided.
        bool m_timeIsInteger;                                   //!< A flag to indicate if the postgis stores, internally, time and timestamp as integer 

        static te::da::DataSourceCapabilities sm_capabilities;  //!< PostGIS capabilities.
        static te::da::SQLDialect* sm_myDialect;                //!< PostGIS SQL dialect.

        friend class ConnectionPool;
        friend class DataSet;
        friend class DataSetItem;
    };

  } // end namespace pgis
}   // end namespace te


#endif  // __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H


