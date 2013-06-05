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
  \file terralib/dataaccess/core/datasource/ScopedTransaction.h

  \brief An utility class to coordinate transactions.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_DATASOURCE_INTERNAL_SCOPEDTRANSACTION_H
#define __TERRALIB_DATAACCESS_CORE_DATASOURCE_INTERNAL_SCOPEDTRANSACTION_H

// TerraLib
#include "../../Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace da
  {
    namespace core
    {
  // Forward declarations
      class DataSource;

      /*!
        \class ScopedTransaction

        \brief An utitily class to coordinate transactions.

        This class can be used to create an object that
        automatically rollback a transaction when it goes out of scope.

        When the client has finished performing operations
        it must explicitly call the commit method. This will
        inform ScopedTransaction to not rollback when it goes out of scope.

        \sa DataSourceTransactor
      */
      class TEDATAACCESSEXPORT ScopedTransaction : public boost::noncopyable
      {
        public:

          /*!
            \brief Constructor.

            \param transactor The transactor that will be used to initiate a transaction.
          */
          ScopedTransaction(DataSource& ds);

          /*! \brief Destructor. */
          ~ScopedTransaction();

          /*!
            \brief It commits the transaction.

            \exception It throws an exception if something goes wrong during transaction commitment.
          */
          void commit();

        private:

          DataSource& m_ds;    //!< A reference to the underlying data source.
          bool m_rollback;     //!< A flag that indicates if the transaction may be aborted at the end.
      };

    }  // end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_DATASOURCE_INTERNAL_SCOPEDTRANSACTION_H
