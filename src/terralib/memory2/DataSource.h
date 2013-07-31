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
#include "../dataaccess2/dataset/DataSet.h"
#include "../dataaccess2/datasource/DataSource.h"
#include "Config.h"

// Boost
#include <boost/thread.hpp>

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

        std::string getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;

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

        void execute(const te::da::Query& command);

        void execute(const std::string& command);

        void cancel();

        boost::int64_t getLastGeneratedId();

        std::string escape(const std::string& value);

        bool isDataSetNameValid(const std::string& datasetName);

        bool isPropertyNameValid(const std::string& propertyName);

        std::vector<std::string> getDataSetNames();

        const te::da::DataSetTypePtr& getDataSetType(const std::string& datasetName);

        std::vector<std::string> getPropertyNames(const std::string& datasetName);

        std::size_t getNumberOfProperties(const std::string& datasetName);

        bool propertyExists(const std::string& datasetName, const std::string& name);

        boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName);

        te::dt::Property* getProperty(const std::string& datasetName, const std::string& name);

        te::dt::Property* getProperty(const std::string& datasetName, std::size_t propertyPos);

        void addProperty(const std::string& datasetName, te::dt::Property* p);

        void dropProperty(const std::string& datasetName, const std::string& propertyName);

        void renameProperty(const std::string& datasetName, const std::string& name, const std::string& newName);

        te::da::PrimaryKey* getPrimaryKey(const std::string& datasetName);

        bool primaryKeyExists(const std::string& datasetName, const std::string& name);

        void addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk);

        void dropPrimaryKey(const std::string& datasetName);

        std::vector<std::string> getForeignKeyNames(const std::string& datasetName);

        bool foreignKeyExists(const std::string& datasetName, const std::string& name);

        te::da::ForeignKey* getForeignKey(const std::string& datasetName, const std::string& name);

        void addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk);

        void dropForeignKey(const std::string& datasetName, const std::string& fkName);

        std::vector<std::string> getUniqueKeyNames(const std::string& datasetName);

        bool uniqueKeyExists(const std::string& datasetName, const std::string& name);

        te::da::UniqueKey* getUniqueKey(const std::string& datasetName, const std::string& name);

        void addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk);

        void dropUniqueKey(const std::string& datasetName, const std::string& uniqueKeyName);

        std::vector<std::string> getCheckConstraintNames(const std::string& datasetName);

        bool checkConstraintExists(const std::string& datasetName, const std::string& name);

        te::da::CheckConstraint* getCheckConstraint(const std::string& datasetName, const std::string& name);

        void addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc);

        void dropCheckConstraint(const std::string& datasetName, const std::string& name);

        std::vector<std::string> getIndexNames(const std::string& datasetName);

        bool indexExists(const std::string& datasetName, const std::string& name);

        te::da::Index* getIndex(const std::string& datasetName, const std::string& name);

        void addIndex(const std::string& datasetName, te::da::Index* idx, const std::map<std::string, std::string>& options); 

        void dropIndex(const std::string& datasetName, const std::string& idxName);

        std::vector<std::string> getSequenceNames();

        bool sequenceExists(const std::string& name);

        te::da::Sequence* getSequence(const std::string& name);

        void addSequence(te::da::Sequence* sequence);

        void dropSequence(const std::string& name);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName, const std::string& propertyName);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName, std::size_t propertyPos);

        std::size_t getNumberOfItems(const std::string& datasetName);

        bool hasDataSets();

        bool dataSetExists(const std::string& name);

        void createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

        void cloneDataSet(const std::string& name, const std::string& cloneName,
                          const std::map<std::string, std::string>& options);

        void dropDataSet(const std::string& name);

        void renameDataSet(const std::string& name, const std::string& newName);

        void add(const std::string& datasetName, te::da::DataSet* d,
                 const std::map<std::string, std::string>& options, std::size_t limit);

        void remove(const std::string& datasetName, const te::da::ObjectIdSet* oids);

        void update(const std::string& datasetName,
                    te::da::DataSet* dataset,
                    const std::vector<std::size_t>& properties,
                    const te::da::ObjectIdSet* oids,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit);

      protected:

        void create(const std::map<std::string, std::string>& info);

        void drop(const std::map<std::string, std::string>& info);

        bool exists(const std::map<std::string, std::string>& info);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& info);

        std::vector<std::string> getEncodings(const std::map<std::string, std::string>& info);

      private:

        std::map<std::string, std::string> m_connInfo;              //!< DataSource information.
        std::map<std::string, te::da::DataSetPtr> m_datasets;       //!< The set of datasets stored in memory.
        std::map<std::string, te::da::DataSetTypePtr> m_schemas;    //!< The set of dataset schemas.
        mutable boost::recursive_mutex m_mtx;                       //!< The internal mutex.
        std::size_t m_numDatasets;                                  //!< The number of datasets kept in the data source.
        std::size_t m_maxNumDatasets;                               //!< The maximum number of datasets to be handled by the data source.
        bool m_isOpened;                                            //!< A flag to control the state of the data source.
        bool m_deepCopy;                                            //!< If true, each dataset is cloned in the getDataSet method.

        static te::da::DataSourceCapabilities sm_capabilities;      //!< The Memory data source capabilities.
        static const te::da::SQLDialect sm_dialect;                 //!< A dummy dialect.
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASOURCE_H
