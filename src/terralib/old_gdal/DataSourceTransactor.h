/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/gdal/DataSourceTransactor.h

  \brief Transactor class for GDAL data provider.
*/

#ifndef __TERRALIB_GDAL_INTERNAL_DATASOURCETRANSACTOR_H
#define __TERRALIB_GDAL_INTERNAL_DATASOURCETRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"

namespace te
{
  namespace gdal
  {
// Forward declaration
    class DataSource;

    /*!
      \class DataSourceTransactor

      \brief Transactor class for GDAL data provider.
    */
    class TEGDALEXPORT DataSourceTransactor : public te::da::DataSourceTransactor
    {
      public:
      
        /*!
          \brief Default constructor.
          
          \param ds the Data source where this transactor operates.
      
          \note Do not pass a NULL pointer.

          \note The class doesn't take the ownership of the DataSource.
        */
        DataSourceTransactor(te::gdal::DataSource* ds);
      
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
                               te::common::AccessPolicy rwRole = te::common::RAccess);

        void                        execute(const te::da::Query& command);
        void                        execute(const std::string& command);
        te::da::BatchExecutor*      getBatchExecutor();
        te::da::PreparedQuery*  getPrepared(const std::string& /*qName = std::string("")*/);

        void cancel();

        boost::int64_t getLastInsertId();

        te::da::DataSourceCatalogLoader* getCatalogLoader();
        te::da::DataSetTypePersistence*  getDataSetTypePersistence();
        te::da::DataSetPersistence*      getDataSetPersistence();
        te::da::DataSource*              getDataSource() const;

        DataSource* getGDALDataSource() const;

    private:
        
      te::gdal::DataSource* m_ds; //!< my data source pointer.
    };

  } // end namespace gdal
}   // end namespace te

#endif  // __TERRALIB_GDAL_INTERNAL_DATASOURCETRANSACTOR_H


