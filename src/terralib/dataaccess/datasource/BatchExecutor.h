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
  \file terralib/dataaccess/datasource/BatchExecutor.h

  \brief A class that models an object that submits commands in batch to the data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_BATCHEXECUTOR_H
#define __TERRALIB_DATAACCESS_INTERNAL_BATCHEXECUTOR_H

// TerraLib
#include "../Config.h"

// STL
#include <iosfwd>
#include <map>
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace da
  {
// Forward declarations
    class DataSourceTransactor;
    class Query;

    /*!
      \class BatchExecutor

      \brief A class that models an object that submits commands in batch to the data source.

      \sa DataSourceTransactor, Query
    */
    class TEDATAACCESSEXPORT BatchExecutor : public boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        BatchExecutor() {}

        /*! \brief Virtual destructor. */
        virtual ~BatchExecutor() {}

        /*!
          \brief It adds the given statement to the list of commands to be executed.

          \param query The query command to be added to the list of commands to be executed.
          
          \note The batch executor will take the query ownership.
        */
        virtual void add(Query* q) = 0;

        /*
          \brief It executes a batch of previously loaded statements. 

          \exception Excpetion It throws na exception if something goes wrong.

          \pos The internal buffer of statements are dropped.
        */
        virtual void execute() = 0;

        /*!
          \brief It executes a series of statements in the respource identified by the URI.

          \param uri     An URI with a resource with query statements.
          \param options Optional parameters that can be used to refine the execution.

          \exception Excpetion It throws na exception if something goes wrong.
        */
        virtual void execute(const std::string& uri, const std::map<std::string, std::string>& options) = 0;

        /*!
          \brief It executes a series of statements from the input stream.

          \param istr    An input stream with query statements.
          \param options Optional parameters that can be used to refine the execution.

          \exception Excpetion It throws na exception if something goes wrong.
        */
        virtual void execute(std::istream& istr, const std::map<std::string, std::string>& options) = 0;

        /*!
          \brief It returns a pointer to the underlying data source transactor.

          \return A pointer to the underlying data source transactor.
        */
        virtual DataSourceTransactor* getTransactor() const = 0;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_BATCHEXECUTOR_H

