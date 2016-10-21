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
  \file terralib/dataaccess/datasource/ConnectionPoolManager.h

  \brief A singleton for managing the connection pools available in the system.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_CONNECTIONPOOLMANAGER_H
#define __TERRALIB_DATAACCESS_INTERNAL_CONNECTIONPOOLMANAGER_H

// TerraLib
#include "../../common/Singleton.h"
#include "../Config.h"

// STL
#include <vector>

// Boost
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace te
{
  namespace da
  {
    class AbstractConnectionPool;

    /*!
      \class ConnectionPoolManager

      \brief A singleton for managing the connection pools available in the system.

      \sa AbstractConnectionPool
     */
    class TEDATAACCESSEXPORT ConnectionPoolManager : public te::common::Singleton<ConnectionPoolManager>
    {
      friend class te::common::Singleton<ConnectionPoolManager>;

      public:

        /*!
          \brief It adds the given pool to the list of monitored pools.

          \param pool The pool to be monitored.
         */
        void add(AbstractConnectionPool* pool);

        /*!
          \brief It removes (un-register) the pool from the monitoring list.

          \param pool The pool to be removed from the monitoring list.
         */
        void remove(AbstractConnectionPool* pool);

        /*! \brief It starts a new thread of execution that will monitore all the connection pools. */
        void start();

        /*! \brief It stops the connection pools monitoring thread. */
        void stop();

        /*!
          \brief It returns true if the manager was already started.

          \return True if the manager was already started and are monitoring the pools.
         */
        bool isInitialized() const;

      protected:

        /*! \brief Singleton constructor is protected. */
        ConnectionPoolManager();

        /*! \brief Singleton destructor is protected. */
        ~ConnectionPoolManager();

      public:

        /*!
          \brief This static method encapsulates the execution thread that monitores all the pools.

          \note This method will be excuted every x seconds.
         */
        void monitore();

      private:

        std::vector<AbstractConnectionPool*> m_pools; //!< The list of monitored pools.
        boost::mutex m_mtx;                           //!< A mutex to lock the manager access.
        boost::thread m_thread;                       //!< The monitoring thread.
        bool m_initialized;                           //!< A flag that indicates if the manager is monitoring the pools.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_CONNECTIONPOOLMANAGER_H


