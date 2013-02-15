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
  \file terralib/mysql/Connection.h

  \brief A class that implements a connection to a MySQL database.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_CONNECTION_H
#define __TERRALIB_MYSQL_INTERNAL_CONNECTION_H

// TerraLib
#include "Config.h"

// STL
#include <cstddef>
#include <map>
#include <string>

// Boost
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/noncopyable.hpp>

// Forward delcarations
namespace sql
{
  class Connection;
  class ResultSet;
}

namespace te
{
  namespace mysql
  {
// Forward declarations
    class ConnectionPool;

    /*!
      \class Connection

      \brief A class that implements a connection to a MySQL database

      This class models a physical connection to a PostgreSQL data source.
      It is designed to work with the connection pool.

      \sa ConnectionPool
    */
    class TEMYSQLEXPORT Connection : public boost::noncopyable
    {
      public:

        sql::ResultSet* query(const std::string& query);

        void execute(const std::string& command);

        sql::Connection* getConn() const
        {
          return m_myconn;
        }

        ConnectionPool* getPool() const
        {
          return m_pool;
        }
                
        ~Connection();

      private:

        Connection(ConnectionPool* pool, const std::map<std::string, std::string>& connInfo, bool inuse = false);

      public:

        ConnectionPool* m_pool;               //!< The connection pool associated to the connection.
        sql::Connection* m_myconn;            //!< The MySQL real connection handle.
        bool m_inuse;                         //!< Tells if the connection is in use or not.
        boost::posix_time::ptime m_lastuse;   //!< It marks the last time this connection was used.

      friend class ConnectionPool;
    };

  } // end namespace mysql
}   // end namespace te


#endif  // __TERRALIB_MYSQL_INTERNAL_CONNECTION_H


