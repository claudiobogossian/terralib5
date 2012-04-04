/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/Transactor.h

  \brief Transactor class for PostGIS driver.
 */

#ifndef __TERRALIB_POSTGIS_INTERNAL_TRANSACTOR_H
#define __TERRALIB_POSTGIS_INTERNAL_TRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"

namespace te
{
  namespace pgis
  {
// Forward declarations
    class CatalogLoader;
    class Connection;
    class DataSource;    
    class PreparedQuery;

    struct VersionInfo;

    /*!
      \class Transactor

      \brief Transactor class for PostGIS driver.

      \sa te::da::DataSourceTransactor, DataSource
     */
    class TEPGISEXPORT Transactor : public te::da::DataSourceTransactor
    {
      public:

        /*!
          \brief Constructor.

          \param ds    The PostGIS DataSource from wich this transactor belongs.
          \param conn  An available connection that will be releasead when transactor dies.
         */
        Transactor(DataSource* ds, Connection* conn);

        /*! \brief The destructor will, automatically, release the connection to the pool. */
        ~Transactor();

        void begin();

        void commit();

        void rollBack();

        bool isInTransaction() const;

        te::da::DataSet* getDataSet(const std::string& name,  
                                    te::common::TraverseType travType = te::common::FORWARDONLY, 
                                    te::common::AccessPolicy rwRole= te::common::RAccess);

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
                               te::common::AccessPolicy rwRole = te::common::RAccess);

        void execute(const te::da::Query& command);

        void execute(const std::string& command);

        te::da::PreparedQuery* getPrepared(const std::string& qName = std::string(""));

        te::da::BatchExecutor* getBatchExecutor();

        te::da::DataSourceCatalogLoader* getCatalogLoader();

        te::da::DataSetTypePersistence* getDataSetTypePersistence();

        te::da::DataSetPersistence* getDataSetPersistence();

        void cancel();

        te::da::DataSource* getDataSource() const;

        /*!
          \brief It returns the underlying connection.

          \return The underlying connection.

          \note PostGIS driver extended method.
        */
        Connection* getConnection() const { return m_conn; }

        /*!
          \brief It returns the parent DataSource of the transactor.

          \return The caller of this method will NOT take the ownership of the returned DataSource pointer.

          \note PostGIS driver extended method.
        */
        DataSource* getPGDataSource() const { return m_ds; }

        /*!
          \brief It returns a PostGIS data source catalog loader.

          \return A PostGIS data source catalog loader. The caller will take the ownership of the returned pointer.
        */
        CatalogLoader* getPGCatalogLoader();

        /*!
          \brief It returns a PostGIS prepared query.

          \return A PostGIS prepared query. The caller will take the ownership of the returned pointer.
        */
        PreparedQuery* getPGPrepared(const std::string& qName = std::string(""));

        /*!
          \brief It will check in the database catalog the number that identifies the PostGIS Geometry type.

          \return The oid of the geometry type of 0 if it is not found.

          \exception Exception It may throws an exception.

          \note PostGIS driver extended method.
        */
        unsigned int getGeomTypeId();

        /*!
          \brief It retrieves some information about the database such as the default schema used when no one is provided.

          \param currentSchema A reference to output the current schema.

          \exception Exception It throws an exception if it was not possible to query the database.

          \note PostGIS driver extended method.
        */
        void getDatabaseInfo(std::string& currentSchema);

      private:

        DataSource* m_ds;       //!< The PostGIS data source associated to this transactor.
        Connection* m_conn;     //!< The connection used by this transactor.
        bool m_isInTransaction; //!< Tells if there is a transaction in progress.
    };

  } // end namespace pgis
}   // end namespace te


#endif  // __TERRALIB_POSTGIS_INTERNAL_TRANSACTOR_H

