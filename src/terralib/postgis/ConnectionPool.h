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
  \file terralib/postgis/ConnectionPool.h

  \brief A class that implements a connection pool for PostGIS.  
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_CONNECTIONPOOL_H
#define __TERRALIB_POSTGIS_INTERNAL_CONNECTIONPOOL_H

// TerraLib
#include "../dataaccess/datasource/AbstractConnectionPool.h"
#include "Config.h"

namespace te
{
  namespace pgis
  {
// Forward declarations
    class Connection;
    class DataSource;

    /*!
      \class ConnectionPool

      \brief This class implements a connection pool for the PostGIS driver.

      \sa AbstractConnectionPool, ConnectionPoolManager, Connection
    */
    class TEPGISEXPORT ConnectionPool : public te::da::AbstractConnectionPool
    {
      public:

        void initialize();

        void finalize();

        void idle();

        bool isValid() const;

        bool isInitialized() const;

        te::pgis::DataSource* getDataSource() const;

       std::size_t getPoolSize() const;

        std::size_t getInitialPoolSize() const;

        void setInitialPoolSize(std::size_t size);

        std::size_t getMinPoolSize() const;

        void setMinPoolSize(std::size_t size);

        std::size_t getMaxPoolSize() const;

        void setMaxPoolSize(std::size_t size);  

        /*!
          \brief It returns a connection from the pool.

          \return A connection if one is available or an exception will be raised.
          
          \exception Exception It throws an exception if it is not possible to get a connection.

          \warning You must call the release() method after using the connection to assure
                   that the connection was returned to the pool. This approach
                   will increase concurrency in the connection.

          \note PostGIS extended method.
        */
        Connection* getConnection();

        /*!
          \brief It brings the informed connection back to the pool.

          \param conn The connection to be put back the pool.

          \note PostGIS extended method.
        */
        void release(Connection* conn);

        /*!
          \brief It returns the maximum idle time in seconds that a connection can be maintained in the pool without being used.

          \return The maximum idle time that a connection can be maintained in the pool without being used.

          \note Thread-safe.

          \note PostGIS extended method.
        */
        unsigned int getMaxIdleTime() const;

        /*!
          \brief It sets the maximum idle time that a connection can be maintained in the pool without being used.

          \param t It specifies the maximum idle time in seconds that a connection
                   can be maintained in the pool without being used.

          \note A zero value indicates that the connections will not be removed from the
                pool when it is not being used for a while.

          \note Thread-safe.

          \note PostGIS extended method.
        */
        void setMaxIdleTime(unsigned int t);

      private:

        /*!
          \brief It creates a new connection pool for the database informed.

          \param ds The data source using this pool.
        */
        ConnectionPool(DataSource* ds);

        /*!
          \brief Destructor.
          
          It will automatically close all connections to a PostgreSQL database.
        */
        ~ConnectionPool();

      private:

        class ConnectionPoolImpl;

        ConnectionPoolImpl* m_pImpl;  //!< A pointer to the pool implementation.

      friend class DataSource;
    };

  } // end namespace pgis
}   // end namespace te


#endif  // __TERRALIB_POSTGIS_INTERNAL_CONNECTIONPOOL_H
