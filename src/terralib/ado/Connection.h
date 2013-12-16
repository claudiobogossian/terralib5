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
  \file terralib/ado/Connection.h

  \brief A class that implements a connection to a ADO database.  
*/

#ifndef __TERRALIB_ADO_INTERNAL_CONNECTION_H
#define __TERRALIB_ADO_INTERNAL_CONNECTION_H

// TerraLib
#include "Config.h"

// STL
#include <cstddef>
#include <string>

// Boost
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/noncopyable.hpp>

// ADO
#import "msado15.dll" \
  no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

namespace te
{
  namespace ado
  {

    /*!
      \class Connection

      \brief A class that implements a connection to a ADO database

      This class models a physical connection to a ADO data source.
      It is designed to work with the connection pool.

      \sa ConnectionPool
    */
    class TEADOEXPORT Connection : public boost::noncopyable
    {
      public:

        /*!
          \brief Constructor.

          \param conninfo A coonection string as undertood by ADO.
          \param inuse    A marker that tells if the connection is in use or not.
        */
        Connection(const std::string& conninfo);

        /*! \brief Destructor. */
        ~Connection();

        /*!
          \brief It queries the database.

          \param query A SQL Select command.

          \return A resultset. The caller of this method will take its ownership.

          \exception Exception It throws an exception if the query execution fails.
        */
        _RecordsetPtr query(const std::string& query, bool connected = false);

        /*!
          \brief It executes the given SQL command and throws away the result.

          \param command Any SQL command.

          \exception Exception It throws an exception if the query execution fails.
        */
        void execute(const std::string& command);

        bool isValid();

        /*!
          \brief It gets the ADO Connection object.

          \return The ADO Connection object.
        */
        _ConnectionPtr getConn() const
        {
          return m_conn;
        }

      public:

        _ConnectionPtr m_conn;                //!< The ADO real connection handle.

      friend class ConnectionPool;
    };

  } // end namespace ado
}   // end namespace te


#endif  // __TERRALIB_ADO_INTERNAL_CONNECTION_H


