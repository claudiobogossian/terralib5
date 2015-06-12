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
  \file terralib/postgis/Connection.h

  \brief A class that implements a connection to a PostgreSQL database.  
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_CONNECTION_H
#define __TERRALIB_POSTGIS_INTERNAL_CONNECTION_H

// TerraLib
//#include "../dataaccess/datasource/Connection.h"
#include "Config.h"

// STL
#include <cstddef>
#include <string>

// Boost
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/noncopyable.hpp>

// Forward declarations for libpq
extern "C"
{
  struct pg_conn;
  typedef struct pg_conn PGconn;

  struct pg_result;
  typedef struct pg_result PGresult;
}

namespace te
{
  namespace pgis
  {
    // Forward declarations
    class ConnectionPool;

    /*!
      \class Connection

      \brief A class that implements a connection to a PostgreSQL database

      This class models a physical connection to a PostgreSQL data source.
      It is designed to work with the connection pool.

      \sa ConnectionPool
    */
    class TEPGISEXPORT Connection : public boost::noncopyable
    {
      public:

        /*!
          \brief It queries the database.

          \param query A SQL Select command.

          \return A resultset. The caller of this method will take its ownership.

          \exception Exception It throws an exception if the query execution fails.
        */
        PGresult* query(const std::string& query);

        /*!
          \brief It executes the given SQL command and throws away the result.

          \param command Any SQL command.

          \exception Exception It throws an exception if the query execution fails.
        */
        void execute(const std::string& command);

        /*!
          \brief It gets the underlying PGconn object.

          \return The underlying PGconn object.
        */
        PGconn* getConn() const
        {
          return m_pgconn;
        }

        /*!
          \brief It gets the associated connection pool.

          \return The associated connection pool.
        */
        ConnectionPool* getPool() const
        {
          return m_pool;
        }
                
        /*! \brief Destructor. */
        ~Connection();

      private:

        /*!
          \brief Constructor.

          \param pool     The associated connection pool.
          \param conninfo A coonection string as undertood by libpq.
          \param cecoding The character encoding used by application.
          \param inuse    A marker that tells if the connection is in use or not.
        */
        Connection(ConnectionPool* pool, const std::string& conninfo, const std::string& cencoding, bool inuse = false);

      public:

        ConnectionPool* m_pool;               //!< The connection pool associated to the connection.
        PGconn* m_pgconn;                     //!< The PostgreSQL real connection handle.
        bool m_inuse;                         //!< Tells if the connection is in use or not.
        boost::posix_time::ptime m_lastuse;   //!< It marks the last time this connection was used.

      friend class ConnectionPool;
    };

  } // end namespace pgis
}   // end namespace te


#endif  // __TERRALIB_POSTGIS_INTERNAL_CONNECTION_H
