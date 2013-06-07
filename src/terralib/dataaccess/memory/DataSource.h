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
  \file terralib/dataaccess/memory/DataSource.h

  \brief 
*/

#ifndef __TERRALIB_DATAACCESS_MEMORY_INTERNAL_DATASOURCE_H
#define __TERRALIB_DATAACCESS_MEMORY_INTERNAL_DATASOURCE_H

// TerraLib
#include "../core/datasource/DataSource.h"
#include "Config.h"

namespace te
{
  namespace da
  {
    namespace mem
    {
      /*!
        \class DataSource

        \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.

        \sa te::da::core::DataSource
      */
      class TEMEMORYEXPORT DataSource : public te::da::core::DataSource
      {
        public:

          DataSource();

          ~DataSource();

          std::string getType() const throw();

          const std::map<std::string, std::string>& getConnectionInfo() const throw();

          void setConnectionInfo(const std::map<std::string, std::string>& connInfo) throw();

          void open() throw(te::da::core::Exception);

          void close() throw(te::da::core::Exception);

          bool isOpened() const throw();

          bool isValid() const throw();

          const te::da::core::DataSourceCapabilities& getCapabilities() const throw();

          const te::da::core::SQLDialect* getDialect() const throw();

          void begin() throw(te::da::core::Exception);

          void commit() throw(te::da::core::Exception);

          void rollBack() throw(te::da::core::Exception);

          bool isInTransaction() const throw();

          std::auto_ptr<te::da::core::DataSet> getDataSet(const std::string& name, 
                                                          te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::DataSet> getDataSet(const std::string& name,
                                                          const std::string& propertyName,
                                                          const te::gm::Envelope* e,
                                                          te::gm::SpatialRelation r,
                                                          te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::DataSet> getDataSet(const std::string& name,
                                                          const std::string& propertyName,
                                                          const te::gm::Geometry* g,
                                                          te::gm::SpatialRelation r,
                                                          te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::DataSet> query(const te::da::core::Select& q,
                                                     te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::DataSet> query(const std::string& query, 
                                                     te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::core::Exception);

          void execute(const te::da::core::Query& command) throw(te::da::core::Exception);

          void execute(const std::string& command) throw(te::da::core::Exception);

          void cancel() throw(te::da::core::Exception);

          boost::int64_t getLastGeneratedId() throw(te::da::core::Exception);

          std::string escape(const std::string& value) throw(te::da::core::Exception);

          bool isDataSetNameValid(const std::string& datasetName) throw(te::da::core::Exception);

          bool isPropertyNameValid(const std::string& propertyName) throw(te::da::core::Exception);

          std::vector<std::string> getDataSetNames() throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::DataSetType> getDataSetType(const std::string& name) throw(te::da::core::Exception);

          std::size_t getNumberOfProperties(const std::string& datasetName) throw(te::da::core::Exception);

          boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName) throw(te::da::core::Exception);

          std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& propertyName) throw(te::da::core::Exception);

          std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos) throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::PrimaryKey> getPrimaryKey(const std::string& datasetName) throw(te::da::core::Exception);

          std::vector<std::string> getUniqueKeyNames(const std::string& datasetName) throw(te::da::core::Exception);

          boost::ptr_vector<te::da::core::UniqueKey> getUniqueKeys(const std::string& datasetName) throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::UniqueKey> getUniqueKey(const std::string& datasetName,
                                                              const std::string& name) throw(te::da::core::Exception);

          std::vector<std::string> getForeignKeyNames(const std::string& datasetName) throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::ForeignKey> getForeignKey(const std::string& datasetName,
                                                                const std::string& name) throw(te::da::core::Exception);

          std::vector<std::string> getIndexNames(const std::string& datasetName) throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::Index> getIndex(const std::string& datasetName,
                                                      const std::string& name) throw(te::da::core::Exception);

          std::vector<std::string> getCheckConstraintNames(const std::string& datasetName) throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::CheckConstraint> getCheckConstraint(const std::string& datasetName,
                                                                          const std::string& name) throw(te::da::core::Exception);

          std::vector<std::string> getSequenceNames() throw(te::da::core::Exception);

          std::auto_ptr<te::da::core::Sequence> getSequence(const std::string& name) throw(te::da::core::Exception);

          std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                            const std::string& propertyName) throw(te::da::core::Exception);

          std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                    std::size_t propertyPos) throw(te::da::core::Exception);

          std::size_t getNumberOfItems(const std::string& datasetName) throw(te::da::core::Exception);

          bool hasDataSets() throw(te::da::core::Exception);

          bool datasetExists(const std::string& name) throw(te::da::core::Exception);

          bool primarykeyExists(const std::string& datasetName,
                                const std::string& name) throw(te::da::core::Exception);

          bool uniquekeyExists(const std::string& datasetName, const std::string& name) throw(te::da::core::Exception);

          bool foreignkeyExists(const std::string& datasetName,
                                const std::string& name) throw(te::da::core::Exception);

          bool checkConstraintExists(const std::string& datasetName,
                                     const std::string& name) throw(te::da::core::Exception);

          bool indexExists(const std::string& datasetName,
                           const std::string& name) throw(te::da::core::Exception);

          bool sequenceExists(const std::string& name) throw(te::da::core::Exception);

          void createDataSet(te::da::core::DataSetType* dt,
                                     const std::map<std::string, std::string>& options) throw(te::da::core::Exception);

          void cloneDataSet(const std::string& name,
                            const std::string& cloneName,
                            const std::map<std::string, std::string>& options) throw(te::da::core::Exception);

          void dropDataSet(const std::string& name) throw(te::da::core::Exception);

          void renameDataSet(const std::string& name,
                             const std::string& newName) throw(te::da::core::Exception);

          void addProperty(const std::string& datasetName,
                           const te::dt::Property* p) throw(te::da::core::Exception);

          void dropProperty(const std::string& datasetName,
                            const std::string& propertyName) throw(te::da::core::Exception);

          void renameProperty(const std::string& datasetName,
                              const std::string& propertyName,
                              const std::string& newPropertyName) throw(te::da::core::Exception);

          void addPrimaryKey(const std::string& datasetName,
                             const te::da::core::PrimaryKey* pk) throw(te::da::core::Exception);

          void dropPrimaryKey(const std::string& datasetName,
                              const std::string& primaryKeyName) throw(te::da::core::Exception);

          void addUniqueKey(const std::string& datasetName,
                            const te::da::core::UniqueKey* uk) throw(te::da::core::Exception);

          void dropUniqueKey(const std::string& datasetName,
                             const std::string& uniqueKeyName) throw(te::da::core::Exception);

          void addIndex(const std::string& datasetName,
                        const te::da::core::Index* idx,
                        const std::map<std::string, std::string>& options) throw(te::da::core::Exception); 

          void dropIndex(const std::string& datasetName,
                         const std::string& idxName) throw(te::da::core::Exception);

          void addForeignKey(const std::string& datasetName,
                             const te::da::core::ForeignKey* fk) throw(te::da::core::Exception);

          void dropForeignKey(const std::string& datasetName,
                              const std::string& fkName) throw(te::da::core::Exception);

          void addCheckConstraint(const std::string& datasetName,
                                  const te::da::core::CheckConstraint* cc) throw(te::da::core::Exception);

          void dropCheckConstraint(const std::string& datasetName,
                                   const std::string& name) throw(te::da::core::Exception);

          void createSequence(const te::da::core::Sequence* sequence) throw(te::da::core::Exception);

          void dropSequence(const std::string& name) throw(te::da::core::Exception);

          void add(const std::string& datasetName,
                   te::da::core::DataSet* d,
                   const std::map<std::string, std::string>& options,
                   std::size_t limit) throw(te::da::core::Exception);

          void remove(const std::string& datasetName,
                      const te::da::core::ObjectIdSet* oids) throw(te::da::core::Exception);

          void update(const std::string& datasetName,
                      te::da::core::DataSet* dataset,
                      const std::vector<std::size_t>& properties,
                      const te::da::core::ObjectIdSet* oids,
                      const std::map<std::string, std::string>& options,
                      std::size_t limit) throw(te::da::core::Exception);

        protected:

          void create(const std::map<std::string, std::string>& dsInfo) throw(te::da::core::Exception);

          void drop(const std::map<std::string, std::string>& dsInfo) throw(te::da::core::Exception);

          bool exists(const std::map<std::string, std::string>& dsInfo) throw(te::da::core::Exception);

          std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& info) throw(te::da::core::Exception);

        private:

          class Impl;

          Impl* m_pImpl;
      };

    } // end namespace mem
  }   // end namespace da
}     // end namespace te

#endif  // __TERRALIB_DATAACCESS_MEMORY_INTERNAL_DATASOURCE_H
