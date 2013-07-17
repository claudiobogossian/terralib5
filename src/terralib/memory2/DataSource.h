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
  \file terralib/memory/DataSource.h

  \brief 
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H
#define __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess2/datasource/DataSource.h"
#include "Config.h"

namespace te
{
  namespace mem
  {
    /*!
      \class DataSource

      \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.

      \sa te::da::DataSource
    */
    class TEMEMORYEXPORT DataSource : public te::da::DataSource
    {
      public:

        DataSource();

        ~DataSource();

        std::string getType() const throw();

        const std::map<std::string, std::string>& getConnectionInfo() const throw();

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo) throw();

        void open() throw(te::da::Exception);

        void close() throw(te::da::Exception);

        bool isOpened() const throw();

        bool isValid() const throw();

        const te::da::DataSourceCapabilities& getCapabilities() const throw();

        const te::da::SQLDialect* getDialect() const throw();

        void begin() throw(te::da::Exception);

        void commit() throw(te::da::Exception);

        void rollBack() throw(te::da::Exception);

        bool isInTransaction() const throw();

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                        te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                        const std::string& propertyName,
                                                        const te::gm::Envelope* e,
                                                        te::gm::SpatialRelation r,
                                                        te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                        const std::string& propertyName,
                                                        const te::gm::Geometry* g,
                                                        te::gm::SpatialRelation r,
                                                        te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        std::auto_ptr<te::da::DataSet> query(const te::da::Select& q,
                                                    te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        std::auto_ptr<te::da::DataSet> query(const std::string& query, 
                                                    te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        void execute(const te::da::Query& command) throw(te::da::Exception);

        void execute(const std::string& command) throw(te::da::Exception);

        void cancel() throw(te::da::Exception);

        boost::int64_t getLastGeneratedId() throw(te::da::Exception);

        std::string escape(const std::string& value) throw(te::da::Exception);

        bool isDataSetNameValid(const std::string& datasetName) throw(te::da::Exception);

        bool isPropertyNameValid(const std::string& propertyName) throw(te::da::Exception);

        std::vector<std::string> getDataSetNames() throw(te::da::Exception);

        const te::da::DataSetTypePtr& getDataSetType(const std::string& name) throw(te::da::Exception);

        std::size_t getNumberOfProperties(const std::string& datasetName) throw(te::da::Exception);

        boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName) throw(te::da::Exception);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& propertyName) throw(te::da::Exception);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos) throw(te::da::Exception);

        std::auto_ptr<te::da::PrimaryKey> getPrimaryKey(const std::string& datasetName) throw(te::da::Exception);

        std::vector<std::string> getUniqueKeyNames(const std::string& datasetName) throw(te::da::Exception);

        boost::ptr_vector<te::da::UniqueKey> getUniqueKeys(const std::string& datasetName) throw(te::da::Exception);

        std::auto_ptr<te::da::UniqueKey> getUniqueKey(const std::string& datasetName,
                                                            const std::string& name) throw(te::da::Exception);

        std::vector<std::string> getForeignKeyNames(const std::string& datasetName) throw(te::da::Exception);

        std::auto_ptr<te::da::ForeignKey> getForeignKey(const std::string& datasetName,
                                                              const std::string& name) throw(te::da::Exception);

        std::vector<std::string> getIndexNames(const std::string& datasetName) throw(te::da::Exception);

        std::auto_ptr<te::da::Index> getIndex(const std::string& datasetName,
                                                    const std::string& name) throw(te::da::Exception);

        std::vector<std::string> getCheckConstraintNames(const std::string& datasetName) throw(te::da::Exception);

        te::da::CheckConstraint* getCheckConstraint(const std::string& datasetName,
                                                                        const std::string& name) throw(te::da::Exception);

        std::vector<std::string> getSequenceNames() throw(te::da::Exception);

        std::auto_ptr<te::da::Sequence> getSequence(const std::string& name) throw(te::da::Exception);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                          const std::string& propertyName) throw(te::da::Exception);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                  std::size_t propertyPos) throw(te::da::Exception);

        std::size_t getNumberOfItems(const std::string& datasetName) throw(te::da::Exception);

        bool hasDataSets() throw(te::da::Exception);

        bool datasetExists(const std::string& name) throw(te::da::Exception);

        bool primarykeyExists(const std::string& datasetName,
                              const std::string& name) throw(te::da::Exception);

        bool uniquekeyExists(const std::string& datasetName, const std::string& name) throw(te::da::Exception);

        bool foreignkeyExists(const std::string& datasetName,
                              const std::string& name) throw(te::da::Exception);

        bool checkConstraintExists(const std::string& datasetName,
                                    const std::string& name) throw(te::da::Exception);

        bool indexExists(const std::string& datasetName,
                          const std::string& name) throw(te::da::Exception);

        bool sequenceExists(const std::string& name) throw(te::da::Exception);

        void createDataSet(te::da::DataSetType* dt,
                                    const std::map<std::string, std::string>& options) throw(te::da::Exception);

        void cloneDataSet(const std::string& name,
                          const std::string& cloneName,
                          const std::map<std::string, std::string>& options) throw(te::da::Exception);

        void dropDataSet(const std::string& name) throw(te::da::Exception);

        void renameDataSet(const std::string& name,
                            const std::string& newName) throw(te::da::Exception);

        void addProperty(const std::string& datasetName,
                          const te::dt::Property* p) throw(te::da::Exception);

        void dropProperty(const std::string& datasetName,
                          const std::string& propertyName) throw(te::da::Exception);

        void renameProperty(const std::string& datasetName,
                            const std::string& propertyName,
                            const std::string& newPropertyName) throw(te::da::Exception);

        void addPrimaryKey(const std::string& datasetName,
                            const te::da::PrimaryKey* pk) throw(te::da::Exception);

        void dropPrimaryKey(const std::string& datasetName,
                            const std::string& primaryKeyName) throw(te::da::Exception);

        void addUniqueKey(const std::string& datasetName,
                          const te::da::UniqueKey* uk) throw(te::da::Exception);

        void dropUniqueKey(const std::string& datasetName,
                            const std::string& uniqueKeyName) throw(te::da::Exception);

        void addIndex(const std::string& datasetName,
                      const te::da::Index* idx,
                      const std::map<std::string, std::string>& options) throw(te::da::Exception); 

        void dropIndex(const std::string& datasetName,
                        const std::string& idxName) throw(te::da::Exception);

        void addForeignKey(const std::string& datasetName,
                            const te::da::ForeignKey* fk) throw(te::da::Exception);

        void dropForeignKey(const std::string& datasetName,
                            const std::string& fkName) throw(te::da::Exception);

        void addCheckConstraint(const std::string& datasetName,
                                const te::da::CheckConstraint* cc) throw(te::da::Exception);

        void dropCheckConstraint(const std::string& datasetName,
                                  const std::string& name) throw(te::da::Exception);

        void createSequence(const te::da::Sequence* sequence) throw(te::da::Exception);

        void dropSequence(const std::string& name) throw(te::da::Exception);

        void add(const std::string& datasetName,
                  te::da::DataSet* d,
                  const std::map<std::string, std::string>& options,
                  std::size_t limit) throw(te::da::Exception);

        void remove(const std::string& datasetName,
                    const te::da::ObjectIdSet* oids) throw(te::da::Exception);

        void update(const std::string& datasetName,
                    te::da::DataSet* dataset,
                    const std::vector<std::size_t>& properties,
                    const te::da::ObjectIdSet* oids,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit) throw(te::da::Exception);

      protected:

        void create(const std::map<std::string, std::string>& dsInfo) throw(te::da::Exception);

        void drop(const std::map<std::string, std::string>& dsInfo) throw(te::da::Exception);

        bool exists(const std::map<std::string, std::string>& dsInfo) throw(te::da::Exception);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& info) throw(te::da::Exception);

      private:

        class Impl;

        Impl* m_pImpl;
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H
