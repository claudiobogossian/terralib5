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

        std::string getType() const throw();

        const std::map<std::string, std::string>& getConnectionInfo() const throw();

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo) throw();

        void open() throw(Exception);
        
        void close() throw(Exception);

        bool isOpened() const throw();

        bool isValid() const throw();

        const te::da::DataSourceCapabilities& getCapabilities() const throw();

        const te::da::SQLDialect* getDialect() const throw();

        static void setDialect(te::da::SQLDialect* dialect);

        void begin() throw(Exception);

        void commit() throw(Exception);

        void rollBack() throw(Exception);

        bool isInTransaction() const throw();

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                          const std::string& propertyName,
                                          const te::gm::Envelope* e,
                                          te::gm::SpatialRelation r,
                                          te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                          const std::string& propertyName,
                                          const te::gm::Geometry* g,
                                          te::gm::SpatialRelation r,
                                          te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);

        std::auto_ptr<te::da::DataSet> query(const te::da::Select& q, te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);

        std::auto_ptr<te::da::DataSet> query(const std::string& query, 
                                      te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);

        void execute(const te::da::Query& command) throw(Exception);

        void execute(const std::string& command) throw(Exception);

        void cancel() throw(Exception);

        boost::int64_t getLastGeneratedId() throw(Exception);

        std::string escape(const std::string& value) throw(Exception);

        bool isDataSetNameValid(const std::string& datasetName) throw(Exception);

        bool isPropertyNameValid(const std::string& propertyName) throw(Exception);

        std::vector<std::string> getDataSetNames() throw(Exception);

        const te::da::DataSetTypePtr& getDataSetType(const std::string& name) throw(Exception);

        std::size_t getNumberOfProperties(const std::string& datasetName) throw(Exception);

        boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName) throw(Exception);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& propertyName) throw(Exception);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos) throw(Exception);

        std::auto_ptr<te::da::PrimaryKey> getPrimaryKey(const std::string& datasetName) throw(Exception);

        std::vector<std::string> getUniqueKeyNames(const std::string& datasetName) throw(Exception);

        boost::ptr_vector<te::da::UniqueKey> getUniqueKeys(const std::string& datasetName) throw(Exception);

        std::auto_ptr<te::da::UniqueKey> getUniqueKey(const std::string& datasetName, const std::string& name) throw(Exception);

        std::vector<std::string> getForeignKeyNames(const std::string& datasetName) throw(Exception);

        std::auto_ptr<te::da::ForeignKey> getForeignKey(const std::string& datasetName, const std::string& name) throw(Exception);

        std::vector<std::string> getIndexNames(const std::string& datasetName) throw(Exception);

        std::auto_ptr<te::da::Index> getIndex(const std::string& datasetName, const std::string& name) throw(Exception);

        std::vector<std::string> getCheckConstraintNames(const std::string& datasetName) throw(Exception);

        std::auto_ptr<te::da::CheckConstraint> getCheckConstraint(const std::string& datasetName, const std::string& name) throw(Exception);

        std::vector<std::string> getSequenceNames() throw(Exception);

        std::auto_ptr<te::da::Sequence> getSequence(const std::string& name) throw(Exception);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName, const std::string& propertyName) throw(Exception);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName, std::size_t propertyPos) throw(Exception);

        std::size_t getNumberOfItems(const std::string& datasetName) throw(Exception);

        bool hasDataSets() throw(Exception);

        bool datasetExists(const std::string& name) throw(Exception);

        bool primarykeyExists(const std::string& datasetName, const std::string& name) throw(Exception);

        bool uniquekeyExists(const std::string& datasetName, const std::string& name) throw(Exception);

        bool foreignkeyExists(const std::string& datasetName, const std::string& name) throw(Exception);

        bool checkConstraintExists(const std::string& datasetName, const std::string& name) throw(Exception);

        bool indexExists(const std::string& datasetName, const std::string& name) throw(Exception);

        bool sequenceExists(const std::string& name) throw(Exception);

        void createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options) throw(Exception);

        void cloneDataSet(const std::string& name,
                          const std::string& cloneName,
                          const std::map<std::string, std::string>& options) throw(Exception);

        void dropDataSet(const std::string& name) throw(Exception);

        void renameDataSet(const std::string& name, const std::string& newName) throw(Exception);

        void addProperty(const std::string& datasetName, const te::dt::Property* p) throw(Exception);

        void dropProperty(const std::string& datasetName, const std::string& propertyName) throw(Exception);

        void renameProperty(const std::string& datasetName,
                            const std::string& propertyName,
                            const std::string& newPropertyName) throw(Exception);

        void addPrimaryKey(const std::string& datasetName, const te::da::PrimaryKey* pk) throw(Exception);

        void dropPrimaryKey(const std::string& datasetName, const std::string& primaryKeyName) throw(Exception);

        void addUniqueKey(const std::string& datasetName, const te::da::UniqueKey* uk) throw(Exception);

        void dropUniqueKey(const std::string& datasetName, const std::string& uniqueKeyName) throw(Exception);

        void addIndex(const std::string& datasetName, const te::da::Index* idx,
                      const std::map<std::string, std::string>& options) throw(Exception); 

        void dropIndex(const std::string& datasetName, const std::string& idxName) throw(Exception);

        void addForeignKey(const std::string& datasetName, const te::da::ForeignKey* fk) throw(Exception);

        void dropForeignKey(const std::string& datasetName, const std::string& fkName) throw(Exception);

        void addCheckConstraint(const std::string& datasetName, const te::da::CheckConstraint* cc) throw(Exception);

        void dropCheckConstraint(const std::string& datasetName, const std::string& name) throw(Exception);

        void createSequence(const te::da::Sequence* sequence) throw(Exception);

        void dropSequence(const std::string& name) throw(Exception);

        void add(const std::string& datasetName, te::da::DataSet* d,
                const std::map<std::string, std::string>& options,
                std::size_t limit = 0) throw(Exception);


        void remove(const std::string& datasetName, const te::da::ObjectIdSet* oids = 0) throw(Exception);

        void update(const std::string& datasetName,
                            te::da::DataSet* dataset,
                            const std::vector<std::size_t>& properties,
                            const te::da::ObjectIdSet* oids,
                            const std::map<std::string, std::string>& options,
                            std::size_t limit = 0) throw(Exception);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& info) throw(Exception);

      protected:
                            
        void create(const std::map<std::string, std::string>& dsInfo) throw(Exception);

        void drop(const std::map<std::string, std::string>& dsInfo) throw(Exception);

        bool exists(const std::map<std::string, std::string>& dsInfo) throw(Exception);

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
