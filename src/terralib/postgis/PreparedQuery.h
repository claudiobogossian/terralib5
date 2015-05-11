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
  \file terralib/postgis/PreparedQuery.h

  \brief A class that implements a prepared query for PostgreSQL data access driver.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_PREPAREDQUERY_H
#define __TERRALIB_POSTGIS_INTERNAL_PREPAREDQUERY_H

// TerraLib
#include "../dataaccess/datasource/PreparedQuery.h"
#include "Config.h"

// Forward delcarations for libpq
extern "C"
{
  struct pg_conn;
  typedef struct pg_conn PGconn;

  struct pg_result;
  typedef struct pg_result PGresult;
}

namespace te
{
// Forward declarations
  namespace da
  {
    class DataSetType;
  }

  namespace pgis
  {
// Forward declarations
    class Transactor;

    /*!
      \class PreparedQuery

      \brief A class that implements a prepared query for PostgreSQL data access driver.

      \note Some general notes:
            - bind doesn't need to set length for fixed-size types.

      \note Review pending bind methods (raster, date, array, ...).

      \note We need a numeric  type by tomorrow!
    */
    class TEPGISEXPORT PreparedQuery : public te::da::PreparedQuery
    {
      public:

        PreparedQuery(Transactor* t, const std::string& pqname);

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

        /*!
          \note PostgeSQL driver extended method.
        */
        void prepare(const std::string& query, const std::vector<int>& paramTypes);

        /*!
          \note PostgeSQL driver extended method.
        */
        void bind(const std::vector<std::size_t>& propertiesPos, std::size_t offset, te::da::DataSet* d);

        /*!
          \note PostgeSQL driver extended method.
        */
        void bind(const std::vector<std::size_t>& propertiesPos, te::da::DataSet* d);

        /*!
          \note PostgeSQL driver extended method.
        */
        void bind(te::da::DataSet* d);

        /*!
          \note PostgeSQL driver extended method.
        */
        void clear();

      private:

        Transactor* m_t;
        PGconn* m_conn;
        PGresult* m_result;
        char** m_paramValues;
        int* m_paramLengths;
        int* m_paramFormats;
        std::size_t m_nparams;
        std::string m_qname;
    };

  } // end namespace pgis
}   // end namespace te


#endif  // __TERRALIB_POSTGIS_INTERNAL_PREPAREDQUERY_H
