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
  \file terralib/mysql/PreparedQuery.h

  \brief This class implements the prepared query for MySQL data access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_PREPAREDQUERY_H
#define __TERRALIB_MYSQL_INTERNAL_PREPAREDQUERY_H

// TerraLib
#include "../dataaccess/datasource/PreparedQuery.h"
#include "Config.h"

// STL
#include <vector>

// Forward declaration
#include <iosfwd>

// Forward declaration
namespace sql { class PreparedStatement; }

namespace te
{
// Forward declaration
  namespace da
  {
    class DataSet;
    class DataSetItem;
    class DataSetType;
  }

  namespace dt { class Property; }

  namespace mysql
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class PreparedQuery

      \brief This class implements the prepared query for MySQL data access driver.

      \sa te::da::PreparedQuery
    */
    class TEMYSQLEXPORT PreparedQuery : public te::da::PreparedQuery
    {
      public:

        PreparedQuery(DataSourceTransactor* t);

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

        te::da::DataSourceTransactor* getTransactor() const;

        /*!
          \note MySQL driver extended method.
        */
        void bind(const std::vector<std::size_t>& propertiesPos, const te::da::DataSetType* dt, te::da::DataSetItem* item);

        /*!
          \note MySQL driver extended method.
        */
        void bind(const te::da::DataSetType* dt, te::da::DataSetItem* item);

        /*!
          \note MySQL driver extended method.
        */
        void bind(const std::vector<std::size_t>& propertiesPos, std::size_t offset, const te::da::DataSetType* dt, te::da::DataSetItem* item);

      private:

        DataSourceTransactor* m_t;            //!< The associated transactor.
        sql::PreparedStatement* m_pstmt;      //!< The associated prepared query.
        std::vector<std::istream*> m_blobs;   //!< The list of blobs binded.
        std::size_t m_nparams;                //!< The number o parameters to be bind.
    };

  } // end namespace mysql
}   // end namespace te

#endif  // __TERRALIB_MYSQL_INTERNAL_PREPAREDQUERY_H

