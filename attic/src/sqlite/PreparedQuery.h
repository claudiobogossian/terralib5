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
  \file terralib/sqlite/PreparedQuery.h

  \brief A class that implements a prepared query for the TerraLib SQLite Data Access Driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_PREPAREDQUERY_H
#define __TERRALIB_SQLITE_INTERNAL_PREPAREDQUERY_H

// TerraLib
#include "../dataaccess/datasource/PreparedQuery.h"
#include "Config.h"

// STL
#include <vector>

// Boost
#include <boost/cstdint.hpp>

// Forward declarations
extern "C"
{
  struct sqlite3_stmt;
  typedef struct sqlite3_stmt sqlite3_stmt;
}


namespace te
{
// Forward declarations
  namespace da
  {
    class DataSetType;
  }

  namespace sqlite
  {
// Forward declarations
    class DataSet;
    class DataSourceTransactor;

    /*!
      \class PreparedQuery

      \brief A class that implements a prepared query for the TerraLib SQLite Data Access Driver

      \sa te::da::PreparedQuery
    */
    class PreparedQuery : public te::da::PreparedQuery
    {
      public:

        PreparedQuery(DataSourceTransactor* parent, sqlite3* db);

        ~PreparedQuery();

        std::string getName() const;

        void prepare(const te::da::Query& query, const std::vector<te::dt::Property*>& paramTypes);

        void prepare(const std::string& query, const std::vector<te::dt::Property*>& paramTypes);

        void execute();

        te::da::DataSet* query(te::common::TraverseType travType = te::common::FORWARDONLY, 
                               te::common::AccessPolicy rwRole = te::common::RAccess);

        void bind(int i, char value);

        void bind(int i, unsigned char value);

        void bind(int i, boost::int16_t value);

        void bind(int i, boost::int32_t value);

        void bind(int i, boost::int64_t value);

        void bind(int i, bool value);

        void bind(int i, float value);

        void bind(int i, double value);

        void bindNumeric(int i, const std::string& value);

        void bind(int i, const std::string& value);

        void bind(int i, const te::dt::ByteArray& value);

        void bind(int i, const te::gm::Geometry& value);

        void bind(int i, const te::rst::Raster& value);

        void bind(int i, const te::dt::DateTime& value); 

        void bind(int i, const te::da::DataSet& value);

        void bind(int i, const te::dt::AbstractData& ad);

        te::da::DataSourceTransactor* getTransactor() const;

        void prepare(const std::string& query);

        void bind(const std::vector<std::size_t>& propertiesPos, std::size_t offset, te::da::DataSet* d);

        void bind(const std::vector<std::size_t>& propertiesPos, te::da::DataSet* d);

        void bind(te::da::DataSet* d);

      private:

        DataSourceTransactor* m_t;
        sqlite3_stmt* m_stmt;
        sqlite3* m_db;
    };

  } // end namespace sqlite
}   // end namespace te


#endif  // __TERRALIB_SQLITE_INTERNAL_PREPAREDQUERY_H


