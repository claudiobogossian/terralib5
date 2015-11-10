/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/terralib4/Transactor.h

  \brief DataSourceTransactor implementation for TerraLib 4.x API.
*/

#ifndef __TERRALIB_TERRALIB4_INTERNAL_DATASOURCETRANSACTOR_H
#define __TERRALIB_TERRALIB4_INTERNAL_DATASOURCETRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"
#include "DataSource.h"

// STL
#include <memory>
#include <map>
#include <string>

class TeView;
class TeAbstractTheme;
class TeLayer;

namespace te
{
  namespace dt { class Property; }
  namespace gm { class Envelope; class Geometry; }
}

namespace terralib4
{
  class BatchExecutor;
  class DataSet;
  class Connection;
  class ObjectIdSet;
  class PreparedQuery;
  class Query;
  struct ThemeInfo;

  class TETERRALIB4EXPORT Transactor : public te::da::DataSourceTransactor
  {
    public:

      Transactor(DataSource* ds, TeDatabase* db);

      ~Transactor();

      te::da::DataSource* getDataSource() const;

      void begin();

      void commit();

      void rollBack();

      bool isInTransaction() const;

      std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                te::common::TraverseType travType = te::common::FORWARDONLY, 
                                                bool connected = false,
                                                const te::common::AccessPolicy accessPolicy = te::common::RAccess);

      std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                const std::string& propertyName,
                                                const te::gm::Envelope* e,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType = te::common::FORWARDONLY, 
                                                bool connected = false,
                                                const te::common::AccessPolicy accessPolicy = te::common::RAccess);

      std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                const std::string& propertyName,
                                                const te::gm::Geometry* g,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType = te::common::FORWARDONLY, 
                                                bool connected = false,
                                                const te::common::AccessPolicy accessPolicy = te::common::RAccess);

      std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                const ObjectIdSet* oids, 
                                                te::common::TraverseType travType = te::common::FORWARDONLY, 
                                                bool connected = false,
                                                const te::common::AccessPolicy accessPolicy = te::common::RAccess);

      std::auto_ptr<te::da::DataSet> query(const te::da::Select& q,
                                           te::common::TraverseType travType = te::common::FORWARDONLY, 
                                           bool connected = false,
                                           const te::common::AccessPolicy accessPolicy = te::common::RAccess);

      std::auto_ptr<te::da::DataSet> query(const std::string& query,
                                           te::common::TraverseType travType = te::common::FORWARDONLY, 
                                           bool connected = false,
                                           const te::common::AccessPolicy accessPolicy = te::common::RAccess);

      void execute(const te::da::Query& command);
        
      void execute(const std::string& command);

      std::auto_ptr<te::da::PreparedQuery> getPrepared(const std::string& qName = std::string(""));

      std::auto_ptr<te::da::BatchExecutor> getBatchExecutor();

      void cancel();

      boost::int64_t getLastGeneratedId();

      std::string escape(const std::string& value);

      bool isDataSetNameValid(const std::string& datasetName);

      bool isPropertyNameValid(const std::string& propertyName);

      std::vector<std::string> getDataSetNames();

      std::size_t getNumberOfDataSets();

      std::auto_ptr<te::da::DataSetType> getDataSetType(const std::string& name);

      boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName);

      std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& name);

      std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos);

      std::vector<std::string> getPropertyNames(const std::string& datasetName);

      std::size_t getNumberOfProperties(const std::string& datasetName);

      bool propertyExists(const std::string& datasetName, const std::string& name);

      void addProperty(const std::string& datasetName, te::dt::Property* p);

      void dropProperty(const std::string& datasetName, const std::string& name);

      void renameProperty(const std::string& datasetName,
                          const std::string& propertyName,
                          const std::string& newPropertyName);

      std::auto_ptr<te::da::PrimaryKey> getPrimaryKey(const std::string& datasetName);

      bool primaryKeyExists(const std::string& datasetName, const std::string& name);

      void addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk);

      void dropPrimaryKey(const std::string& datasetName);

      std::auto_ptr<te::da::ForeignKey> getForeignKey(const std::string& datasetName, const std::string& name);

      std::vector<std::string> getForeignKeyNames(const std::string& datasetName);

      bool foreignKeyExists(const std::string& datasetName, const std::string& name);

      void addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk);

      void dropForeignKey(const std::string& datasetName, const std::string& fkName);

      std::auto_ptr<te::da::UniqueKey> getUniqueKey(const std::string& datasetName, const std::string& name);

      std::vector<std::string> getUniqueKeyNames(const std::string& datasetName);

      bool uniqueKeyExists(const std::string& datasetName, const std::string& name);

      void addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk);

      void dropUniqueKey(const std::string& datasetName, const std::string& name);

      std::auto_ptr<te::da::CheckConstraint> getCheckConstraint(const std::string& datasetName, const std::string& name);

      std::vector<std::string> getCheckConstraintNames(const std::string& datasetName);

      bool checkConstraintExists(const std::string& datasetName, const std::string& name);

      void addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc);

      void dropCheckConstraint(const std::string& datasetName, const std::string& name);

      std::auto_ptr<te::da::Index> getIndex(const std::string& datasetName, const std::string& name);

      std::vector<std::string> getIndexNames(const std::string& datasetName);

      bool indexExists(const std::string& datasetName, const std::string& name);

      void addIndex(const std::string& datasetName, te::da::Index* idx,
                    const std::map<std::string, std::string>& options); 

      void dropIndex(const std::string& datasetName, const std::string& idxName);

      std::auto_ptr<te::da::Sequence> getSequence(const std::string& name);

      std::vector<std::string> getSequenceNames();

      bool sequenceExists(const std::string& name);

      void addSequence(te::da::Sequence* sequence);

      void dropSequence(const std::string& name);

      std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                const std::string& propertyName);

      std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                std::size_t propertyPos);

      std::size_t getNumberOfItems(const std::string& datasetName);

      bool hasDataSets();

      bool dataSetExists(const std::string& name);

      void createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

      void cloneDataSet(const std::string& name,
                        const std::string& cloneName,
                        const std::map<std::string, std::string>& options);

      void dropDataSet(const std::string& name);

      void renameDataSet(const std::string& name, const std::string& newName);

      void add(const std::string& datasetName,
                te::da::DataSet* d,
                const std::map<std::string, std::string>& options,
                std::size_t limit = 0);

      void remove(const std::string& datasetName, const te::da::ObjectIdSet* oids = 0);

      void update(const std::string& datasetName,
                  te::da::DataSet* dataset,
                  const std::vector<std::size_t>& properties,
                  const te::da::ObjectIdSet* oids,
                  const std::map<std::string, std::string>& options,
                  std::size_t limit = 0);

      void optimize(const std::map<std::string, std::string>& opInfo);

      te::common::CharEncoding getEncoding();

      std::vector<std::string> getTL4Layers();

      std::vector<std::string> getTL4Tables();

      std::vector<std::string> getTL4Rasters();

      std::vector<std::pair<std::string, std::string> > getTL4RasterFiles();

      std::vector<::terralib4::ThemeInfo> getTL4Themes();

      TeTheme* getTL4Theme(const ::terralib4::ThemeInfo& theme);

      int getLayerSRID(const std::string & layerName);

    private:

      DataSource* m_ds;
      TeDatabase* m_db;
      bool m_isInTransaction;

      std::map<int, TeLayer*>& m_layerMap;
      std::map<int, TeView*>& m_viewMap;
      std::map<int, TeAbstractTheme*>& m_themeMap;

  };

}   // end namespace terralib4

#endif  // __TERRALIB_TERRALIB4_INTERNAL_DATASOURCETRANSACTOR_H


