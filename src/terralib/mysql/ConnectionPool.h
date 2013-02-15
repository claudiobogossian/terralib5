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
  \file terralib/mysql/ConnectionPool.h

  \brief A class that implements a connection pool for MySQL driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_CONNECTIONPOOL_H
#define __TERRALIB_MYSQL_INTERNAL_CONNECTIONPOOL_H

// TerraLib
#include "../dataaccess/datasource/AbstractConnectionPool.h"
#include "Config.h"

namespace te
{
  namespace mysql
  {
// Forward declarations
    class Connection;
    class DataSource;

    /*!
      \class ConnectionPool

      \brief This class implements a connection pool for the MySQL driver.

      \sa AbstractConnectionPool, ConnectionPoolManager, Connection
    */
    class TEMYSQLEXPORT ConnectionPool : public te::da::AbstractConnectionPool
    {
      public:

        void initialize();

        void finalize();

        void idle();

        bool isValid() const;

        bool isInitialized() const;

        std::size_t getPoolSize() const;

        std::size_t getInitialPoolSize() const;

        void setInitialPoolSize(std::size_t size);

        std::size_t getMinPoolSize() const;

        void setMinPoolSize(std::size_t size);

        std::size_t getMaxPoolSize() const;

        void setMaxPoolSize(std::size_t size);  

        Connection* getConnection();

        void release(Connection* conn);

        unsigned int getMaxIdleTime() const;

        void setMaxIdleTime(unsigned int t);

      //private:

        ConnectionPool(DataSource* ds);

        ~ConnectionPool();

      private:

        class ConnectionPoolImpl;

        ConnectionPoolImpl* m_pImpl;  //!< A pointer to the pool implementation.
    };

  } // end namespace mysql
}   // end namespace te


#endif  // __TERRALIB_MYSQL_INTERNAL_CONNECTIONPOOL_H


