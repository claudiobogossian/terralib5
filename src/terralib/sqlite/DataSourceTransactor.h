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
  \file terralib/sqlite/DataSourceTransactor.h

  \brief An implementation of DataSourceTransactor class for the TerraLib SQLite Data Access Driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_DATASOURCETRANSACTOR_H
#define __TERRALIB_SQLITE_INTERNAL_DATASOURCETRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"

// Forward declarations
extern "C"
{
  struct sqlite3_stmt;
  typedef struct sqlite3_stmt sqlite3_stmt;
}

namespace te
{
  namespace sqlite
  { 
// Forward declaration
    class BatchExecutor;
    class DataSource;
    class DataSourceCatalogLoader;
    class PreparedQuery;

    /*!
      \class DataSourceTransactor

      \brief An implementation of DataSourceTransactor class for the TerraLib SQLite Data Access Driver.

      \sate::da::DataSourceTransactor
    */
    class TESQLITEEXPORT DataSourceTransactor : public te::da::DataSourceTransactor
    {
      public:

        /*!
          \brief Constructor.

          \param ds The associated SQLite data source.
        */
        DataSourceTransactor(DataSource* ds);

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
          \brief It returns the SQLite data source associated to this transactor.

          \return The SQLite data source associated to this transactor.

          \note SQLite driver extended method.
        */
        DataSource* getLiteDataSource() const;

        PreparedQuery* getLitePrepared();

        BatchExecutor* getLiteBatchExecutor();

        /*!
          \brief It queries the database.

          \param query A SQL select command.

          \return The prepared statement. The caller of this method will take its ownership.
        */
        sqlite3_stmt* queryLite(const std::string& query);

        DataSourceCatalogLoader* getSQLiteCatalogLoader();

      private:

        DataSource* m_ds;       //!< The SQLite data source associated to this transactor.
        bool m_isInTransaction; //!< Tells if there is a transaction in progress.
    };

    inline DataSource* DataSourceTransactor::getLiteDataSource() const
    {
      return m_ds;
    }

  } // end namespace sqlite
}   // end namespace te

#endif  // __TERRALIB_SQLITE_INTERNAL_DATASOURCETRANSACTOR_H

