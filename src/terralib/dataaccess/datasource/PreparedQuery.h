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
  \file terralib/dataaccess/datasource/PreparedQuery.h

  \brief A class that models a prepared query.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_PREPAREDQUERY_H
#define __TERRALIB_DATAACCESS_INTERNAL_PREPAREDQUERY_H

// TerraLib
#include "../../common/Enums.h"
#include "../Config.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declarations
  namespace dt
  {
    class AbstractData;
    class ByteArray;
    class DateTime;
    class Property;
  }

  namespace gm { class Geometry; }

  namespace rst { class Raster; }

  namespace da
  {
    class DataSet;
    class DataSourceTransactor;
    class Query;

    /*!
      \class PreparedQuery

      \brief A class that model a prepared query.

      \sa DataSourceTransactor, DataSet, Query, Literal
    */
    class TEDATAACCESSEXPORT PreparedQuery : public boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        PreparedQuery() {}
        
        /*! \brief Virtual destructor. */
        virtual ~PreparedQuery() {}

        /*!
          \brief It returns the prepared query name.

          \param return The prepared query name.
        */
        virtual std::string getName() const = 0;

        /*!
          \brief It prepares the query that may be used for commands that are used mutiple times (select, insert, update and delete).

          This command will submit a request to create a prepared query with the given parameters.

          \param query      The query to be prepared.
          \param paramTypes The list of parameters data type.

          \exception Exception It throws an exception if the prepared query can not be created. For example
                               if another prepared query with the same name already exists, this may throws an exception.
        */
        virtual void prepare(const Query& query, const std::vector<te::dt::Property*>& paramTypes) = 0;

        /*!
          \brief It prepares the query using native dialect.

          This command may be used for commands that are used mutiple times (select, insert, update and delete).

          This command will submit a request to create a prepared query with the given parameters.

          \param query      The query to be prepared.
          \param paramTypes The list of parameters data type.

          \exception Exception It throws an exception if the prepared query can not be created. For example
                               if another prepared query with the same name already exists, this may throws an exception.
        */
        virtual void prepare(const std::string& query, const std::vector<te::dt::Property*>& paramTypes) = 0;

        /*
          \brief It executes a prepared query with parameters informed by bind methods.

          \exception Excpetion It throws na exception if something goes wrong.
        */
        virtual void execute() = 0;

        /*
          \brief It executes a retrieval prepared query with parameters informed by bind methods.
          
          \param travType    The traverse type associated to the returned dataset (if any is returned). 
          \param rwRole      The read and write permission associated to the returned dataset (if any is returned). 

          \return A dataset with the result of the given query. The caller will take its ownership.

          \exception Excpetion It throws na exception if something goes wrong.
        */
        virtual DataSet* query(te::common::TraverseType travType = te::common::FORWARDONLY, 
                               te::common::AccessPolicy rwRole = te::common::RAccess) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, char value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, unsigned char value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, boost::int16_t value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, boost::int32_t value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, boost::int64_t value) = 0;

       /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, bool value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, float value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, double value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.

          \todo The name of this method will change when we have a numeric type!
        */
        virtual void bindNumeric(int i, const std::string& value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, const std::string& value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, const te::dt::ByteArray& value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, const te::gm::Geometry& value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, const te::rst::Raster& value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, const te::dt::DateTime& value) = 0; 

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, const DataSet& value) = 0;

        /*!
          \brief Binds the i-th parameter of the query to a value.

          \param i     The parameter index.
          \param value The parameter value.
        */
        virtual void bind(int i, const te::dt::AbstractData& ad);

        /*!
          \brief It returns a pointer to the underlying data source transactor.

          \return A pointer to the underlying data source transactor.
        */
        virtual DataSourceTransactor* getTransactor() const = 0;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_PREPAREDQUERY_H

