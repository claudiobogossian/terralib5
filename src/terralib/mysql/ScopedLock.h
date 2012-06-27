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
  \file terralib/mysql/ScopedLock.h

  \brief An utitily class to coordinate table locks.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_SCOPEDLOCK_H
#define __TERRALIB_MYSQL_INTERNAL_SCOPEDLOCK_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace mysql
  {
// Forward declarations
    class DataSourceTransactor;

    /*!
      \class ScopedLock

      \brief An utitily class to coordinate table locks.
    */
    class TEMYSQLEXPORT ScopedLock : public boost::noncopyable
    {
      public:

        /*!
          \brief Constructor.

          \param transactor The transactor that will be used to initiate the lock.
          \param tbls       The list of tables to be locked.
        */
        ScopedLock(te::da::DataSourceTransactor& transactor, const std::string& tbls);

        /*! \brief Destructor. */
        ~ScopedLock();

      private:

        te::da::DataSourceTransactor& m_t;    //!< A reference to the underlying transactor.
    };

  } // end namespace mysql
}   // end namespace te

#endif  // __TERRALIB_MYSQL_INTERNAL_SCOPEDLOCK_H

