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
  \file terralib/dataaccess/datasource/AbstractConnectionPool.h

  \brief This class defines the basic interface for a connection pool.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ABSTRACTCONNECTIONPOOL_H
#define __TERRALIB_DATAACCESS_INTERNAL_ABSTRACTCONNECTIONPOOL_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

// Boost
#include <boost/utility.hpp>

namespace te
{
  namespace da
  {
    /*!
      \class AbstractConnectionPool

      \brief This class defines the basic interface for a connection pool.

      A connection pool keeps a cache of physical connections to a data source.
      These connections can be reused by the application when it receives new
      requests for a connection.

      A connection pool is a way to improve performance and scalability when
      multiple clients can share a smaller number of physical database connections.
      It manages a set of connections to be used by the applications. In TerraLib,
      each data source can have its own connection pool.

      This class takes part of the data source connection framework and it
      is optionally used in a data source driver implementation.

      Notice that a given data source object can have its own connection pool.
      So each data source can control the number of simultaneous opened connections.

      \sa ConnectionPoolManager
    */
    class TEDATAACCESSEXPORT AbstractConnectionPool : public boost::noncopyable
    {
      public:

        /** @name Pure Virtual Methods
         *  Methods that subclasses must implement.
         */
        //@{

        /*!
          \brief It initializes the connections to be managed by the pool.

          \exception Exception It throws an exception if it is not possible to initialize the pool.

          \note Successive calls won't have effect.

          \note Thread-safe.
        */
        virtual void initialize() = 0;

        /*!
          \brief It closes all connections and clears all resources managed by the pool.

          \exception Exception It may throws an exception if there is a connection in use or if it is not possible to stop the pool.

          \note Successive calls won't have effect.

          \note Thread-safe.
        */
        virtual void finalize() = 0;

        /*!
          \brief It releases the connections that are not in use for a long time.

          This method will try to keep the pool with the minimum number
          of opened connections. It will be invoked by the
          connection pool manager when its thread wakeup.
          It must check the idle connections and must destroy them.

          \note Thread-safe.
        */
        virtual void idle() = 0;

        /*!
          \brief It checks if all the connections in the pool are valid (the communication channel is ok).

          \return It returns true if all the connections are valid (the communication channel is ok), otherwise it returns false.

          \note Thread-safe.
        */
        virtual bool isValid() const = 0;

        /*!
          \brief It returns true if the connection pool is initialized, otherwise it returns false.

          \return True if the connection pool is initialized, otherwise it returns false.

          \note Thread-safe.
        */
        virtual bool isInitialized() const = 0;

        /*!
          \brief It returns the number of connections in the pool.

          \return The number of the connections in the pool.

          \note Thread-safe.
        */
        virtual std::size_t getPoolSize() const = 0;

        /*!
          \brief It returns the initial number of connections opened by the pool at its startup.

          \return The initial number of connections opened by the pool at its startup.

          \note Thread-safe.
        */
        virtual std::size_t getInitialPoolSize() const = 0;

        /*!
          \brief It sets the initial number of connections opened by the pool at its startup.

          \param size It specifies the initial number of connections that
                      must be opened by the pool at its startup.

          \note The pool must be reset before the changes take effect.

          \note Thread-safe.
        */
        virtual void setInitialPoolSize(std::size_t size) = 0;

        /*!
          \brief It returns the minimum number of connections managed by the pool.

          \return The minimum number of connections managed by the pool.

          \note Thread-safe.
        */
        virtual std::size_t getMinPoolSize() const = 0;

        /*!
          \brief It sets the minimum number of connections managed by the pool.

          \param size The minimum number of connections that the pool must manage.

          \note The pool must be reset before the changes take effect.

          \note Thread-safe.
        */
        virtual void setMinPoolSize(std::size_t size) = 0;

        /*!
          \brief It returns the maximum number of connections managed by the pool.

          \return The maximum number of connections managed by the pool.

          \note Thread-safe.
        */
        virtual std::size_t getMaxPoolSize() const = 0;

        /*!
          \brief It sets the maximum number of connections managed by the pool.

          \param size The maximum number of connections that the pool must manage.

          \note The pool must be reset before the changes take effect.

          \note Thread-safe.
        */
        virtual void setMaxPoolSize(std::size_t size) = 0;  

        //@}

      protected:

        /*! \brief Default constructor. */
        AbstractConnectionPool() { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractConnectionPool() { }
    };

  } // end namespace da
}   // end namespace te


#endif  // __TERRALIB_DATAACCESS_INTERNAL_ABSTRACTCONNECTIONPOOL_H


