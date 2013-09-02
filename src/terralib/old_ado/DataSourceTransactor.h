/*  Copyright (C) 2001-20013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ado/DataSourceTransactor.h

  \brief Implementation of the DataSourceTransactor class for the TerraLib ADO Data Access driver.
*/

#ifndef __TERRALIB_ADO_INTERNAL_DATASOURCETRANSACTOR_H
#define __TERRALIB_ADO_INTERNAL_DATASOURCETRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")

#include <oledb.h>
#include <stdio.h>
#include <conio.h>
#include "icrsint.h"

namespace te
{
  namespace ado
  { 
// Forward declaration
    class DataSource;

    /*!
      \class DataSourceTransactor

      \brief Implementation of the DataSourceTransactor class for the TerraLib ADO Data Access driver.

      \sa te::da::DataSourceTransactor
    */
    class TEADOEXPORT DataSourceTransactor : public te::da::DataSourceTransactor
    {
      public:

        DataSourceTransactor(DataSource* ds, _ConnectionPtr conn);

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

        _ConnectionPtr getADOConnection();

      private:

        DataSource* m_ds;           //!< The data source associated to this transactor.
        _ConnectionPtr m_conn;      //!< The ADO connection associated to this transactor.
        bool m_isInTransaction;     //!< Flag indicating if a transaction is in progress.
    };

  } // end namespace ado
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_DATASOURCETRANSACTOR_H
