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
  \file terralib/mysql/DataSourceTransactor.h

  \brief An implementation of DataSourceTransactor class for MySQL data access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_DATASOURCETRANSACTOR_H
#define __TERRALIB_MYSQL_INTERNAL_DATASOURCETRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"

// Forward declaration
namespace sql { class ResultSet; }

namespace te
{
  namespace mysql
  { 
// Forward declaration
    class Connection;
    class DataSource;
    class DataSourceCatalogLoader;

    /*!
      \class DataSourceTransactor

      \brief An implementation of DataSourceTransactor class for MySQL data access driver.

      \sate::da::DataSourceTransactor
    */
    class TEMYSQLEXPORT DataSourceTransactor : public te::da::DataSourceTransactor
    {
      public:

        DataSourceTransactor(DataSource* parent, Connection* conn);

        ~DataSourceTransactor();

        void begin();

        void commit();

        void rollBack();

        bool isInTransaction() const;

        te::da::DataSet* getDataSet(const std::string& name, 
                                    te::common::TraverseType travType = te::common::FORWARDONLY, 
                                    te::common::AccessPolicy rwRole = te::common::RAccess);

        te::da::DataSet* getDataSet(const std::string& name,
                                    const te::dt::Property* p,
                                    const te::gm::Envelope* e,
                                    te::gm::SpatialRelation r,
                                    te::common::TraverseType travType = te::common::FORWARDONLY, 
                                    te::common::AccessPolicy rwRole = te::common::RAccess);

        te::da::DataSet* getDataSet(const std::string& name,
                                    const te::dt::Property* p,
                                    const te::gm::Geometry* g,
                                    te::gm::SpatialRelation r,
                                    te::common::TraverseType travType = te::common::FORWARDONLY, 
                                    te::common::AccessPolicy rwRole = te::common::RAccess);

        te::da::DataSet* query(const te::da::Select& q, 
                               te::common::TraverseType travType = te::common::FORWARDONLY, 
                               te::common::AccessPolicy rwRole = te::common::RAccess);

        te::da::DataSet* query(const std::string& query, 
                               te::common::TraverseType travType = te::common::FORWARDONLY, 
                               te::common::AccessPolicy rwRole= te::common::RAccess);

        void execute(const te::da::Query& command);

        void execute(const std::string& command);
 
        te::da::PreparedQuery* getPrepared(const std::string& qName = std::string(""));

        te::da::BatchExecutor* getBatchExecutor();

        te::da::DataSourceCatalogLoader* getCatalogLoader();

        te::da::DataSetTypePersistence* getDataSetTypePersistence();

        te::da::DataSetPersistence* getDataSetPersistence();

        void cancel();

        boost::int64_t getLastInsertId();

        te::da::DataSource* getDataSource() const;

        /*!
          \note MySQL driver extended method.
        */
        Connection* getConnection() const;

        /*!
          \note MySQL driver extended method.
        */
        DataSourceCatalogLoader* getMyCatalogLoader();

        /*!
          \note MySQL driver extended method.
        */
        sql::ResultSet* myQuery(const std::string& query);

        /*!
          \note MySQL driver extended method.
        */
        te::da::DataSet* getRasterDataSet(const std::string& name);

        /*!
          \note MySQL driver extended method.
        */
        DataSource* getMyDataSource() const;

      private:

        DataSource* m_ds;       //!< The associated data source.
        Connection* m_conn;     //!< The underlying connection.
        bool m_isInTransaction; //!< Tells if there is a transaction in progress.
    };

  } // end namespace mysql
}   // end namespace te

#endif  // __TERRALIB_MYSQL_INTERNAL_DATASOURCETRANSACTOR_H

