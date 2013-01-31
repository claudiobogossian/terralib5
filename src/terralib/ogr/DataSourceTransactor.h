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
  \file terralib/ogr/Transactor.h

  \brief Transactor class for OGR data provider.  
 */

#ifndef __TERRALIB_OGR_INTERNAL_OGRTRANSACTOR_H
#define __TERRALIB_OGR_INTERNAL_OGRTRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"

// Forward declarations
class OGRDataSource;

namespace te
{
  namespace ogr
  {
    // Forward declarations
    class DataSource;
    
    /*!
      \class Transactor

      \brief Transactor class for OGR data provider.

      \sa DataSourceTransactor
     */
    class TEOGREXPORT DataSourceTransactor : public te::da::DataSourceTransactor
    {
      public:

        /** @name Constructor/Destructor
         *  Initilizer methods.
         */
        //@{

        /*! \brief Constructor. */
        DataSourceTransactor(DataSource* ds, OGRDataSource* ogrDS);

        /*! \brief Destructor. */
        ~DataSourceTransactor();

        //@}
   
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

        /** @name OGR Transactor Specific Methods
         *  Specific methods of a OGR Transactor.
         */
        //@{

        /*!
          \brief It retrieves the OGR DataSource associate to this transactor.

          \return The OGR Datasource.
         */
        OGRDataSource* getOGRDataSource() const;

        //@}

      private:

        DataSource*    m_ds;    //!< The DataSource associated to this transactor.
        OGRDataSource* m_ogrDS; //!< The OGR DataSource associated to this transactor.
        bool m_isInTransaction; //!< Tells if there is a transaction in progress.
    };

  } // end namespace ogr
}   // end namespace te


#endif  // __TERRALIB_OGR_INTERNAL_OGRTRANSACTOR_H


