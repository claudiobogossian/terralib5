/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ogr/DataSource.h

  \brief A class for data providers of OGR.
*/

#ifndef __TERRALIB_OGR_INTERNAL_DATASOURCE_H
#define __TERRALIB_OGR_INTERNAL_DATASOURCE_H

// TerraLib
#include "Config.h"
#include "../dataaccess2/datasource/DataSource.h"
#include "../dataaccess2/datasource/DataSourceCapabilities.h"
#include "Exception.h"

// Forward declarations
class OGRDataSource;

namespace te
{
  namespace ogr
  {
    /*!
      \class DataSource

      \brief The OGR data source provider.
    */
    class TEOGREXPORT DataSource : public te::da::DataSource
    {
      public:

        /*! \brief Default constructor that can be called by subclasses. */
        DataSource();

        /*! \brief Virtual destructor. */
        ~DataSource();

        std::string getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        void open();
        
        void close();

        bool isOpened() const;

        bool isValid() const;

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;

        static void setDialect(te::da::SQLDialect* dialect);

        void begin();

        void commit();

        void rollBack();

        bool isInTransaction() const;

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Envelope* e,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Geometry* g,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<te::da::DataSet> query(const te::da::Select& q,
                                             te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<te::da::DataSet> query(const std::string& query, 
                                             te::common::TraverseType travType = te::common::FORWARDONLY);

        void execute(const std::string& command);

        std::vector<std::string> getDataSetNames();

        const te::da::DataSetTypePtr& getDataSetType(const std::string& name);

        std::size_t getNumberOfProperties(const std::string& datasetName);

        boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName);

        te::dt::Property* getProperty(const std::string& datasetName, const std::string& name);

        te::dt::Property* getProperty(const std::string& datasetName, std::size_t propertyPos);

        void addProperty(const std::string& datasetName, te::dt::Property* p);

        te::da::PrimaryKey* getPrimaryKey(const std::string& datasetName);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName, const std::string& propertyName);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName, std::size_t propertyPos);

        bool hasDataSets();

        void createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

        void add(const std::string& datasetName, te::da::DataSet* d,
                 const std::map<std::string, std::string>& options,
                 std::size_t limit = 0);
      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

      protected:

        std::map<std::string, std::string> m_connectionInfo;        //!< Connection information.
        OGRDataSource* m_ogrDS;                                     //!< A pointer to OGR Data Source.
        bool m_isValid;                                             //!< True if this is a valid datasource.
        te::da::DataSourceCapabilities m_capabilities;              //!< OGR capabilities.
        bool m_isInTransaction;                                     //!< Tells if there is a transaction in progress.

        static te::da::SQLDialect* sm_myDialect;                    //!< OGR SQL dialect.
    };
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_OGR_INTERNAL_DATASOURCE_H
