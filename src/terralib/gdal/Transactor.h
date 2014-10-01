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
 \file terralib/gdal/Transactor.h

 */

#ifndef __TERRALIB_GDAL_INTERNAL_TRANSACTOR_H
#define __TERRALIB_GDAL_INTERNAL_TRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"

//STL
#include <memory>

// Boost
#include <boost/filesystem.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace dt
  {
    class Property;
  }
  
  namespace gm
  {
    class Envelope;
    class Geometry;
  }
  namespace da
  {
    class DataSet;
    class DataSource;
    class ObjectIdSet;
    typedef boost::shared_ptr<DataSetType> DataSetTypePtr;
  }
  
  namespace gdal
  {
    class DataSource;
    
    /*!
     \class Transactor
     */
    class TEGDALEXPORT Transactor : public te::da::DataSourceTransactor
    {
    public:
      
      Transactor(const std::string& accessInfo);
      
      ~Transactor();
      
      te::da::DataSource* getDataSource() const;
      
      std::vector<std::string> getDataSetNames();
      
      std::size_t getNumberOfDataSets();      
      
      std::auto_ptr<te::da::DataSetType> getDataSetType(const std::string& name);
      
      boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName);
      
      std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& name);
      
      std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos);
      
      std::vector<std::string> getPropertyNames(const std::string& datasetName);

      std::size_t getNumberOfProperties(const std::string& datasetName);

      bool propertyExists(const std::string& datasetName, const std::string& name);
      
      bool propertyExists(const std::string& datasetName, size_t propertyPos);

      void addProperty(const std::string& , te::dt::Property* ) {}

      void dropProperty(const std::string& , const std::string& ) {}

      void renameProperty(const std::string& , const std::string& , const std::string& ) {}
      
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
                                                const te::da::ObjectIdSet* oids, 
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
      
      std::size_t getNumberOfItems(const std::string& datasetName) { return 1; }
      
      bool hasDataSets();
      
      bool dataSetExists(const std::string& name);
      
      void createDataSet(te::da::DataSetType* dt,
                         const std::map<std::string, std::string>& options);
      
      void cloneDataSet(const std::string& name,
                        const std::string& cloneName,
                        const std::map<std::string, std::string>& options) ;
      
      void dropDataSet(const std::string& name);
      
      void renameDataSet(const std::string& name,
                         const std::string& newName);
      
      void add(const std::string& ,
               te::da::DataSet* ,
               const std::map<std::string, std::string>& ,
               std::size_t ) {}
      
      void remove(const std::string& ,
                  const te::da::ObjectIdSet* ){}
      
      void update(const std::string& ,
                  te::da::DataSet* ,
                  const std::vector<std::size_t>& ,
                  const te::da::ObjectIdSet* ,
                  const std::map<std::string, std::string>& ,
                  std::size_t ) {}
      
      std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                const std::string& propertyName);
      
      std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                std::size_t propertyPos);
      
      void begin() {}
      
      void commit() {}
      
      void rollBack() {}
      
      bool isInTransaction() const { return false; }
      
      void execute(const te::da::Query& /*command*/) {}
      
      void execute(const std::string& /*command*/) {}
      
      std::auto_ptr<te::da::PreparedQuery> getPrepared(const std::string& /*qName = std::string("")*/) { return std::auto_ptr<te::da::PreparedQuery>(); }
      
      std::auto_ptr<te::da::BatchExecutor> getBatchExecutor() { return std::auto_ptr<te::da::BatchExecutor>(); }
      
      void cancel() {}
      
      boost::int64_t getLastGeneratedId() { return 0; }
      
      std::string escape(const std::string& /*value*/) { return std::string(""); }
      
      std::auto_ptr<te::da::PrimaryKey> getPrimaryKey(const std::string& ) 
      { return std::auto_ptr<te::da::PrimaryKey>(); }
      
      bool primaryKeyExists(const std::string&, const std::string& ) { return false; }
      
      void addPrimaryKey(const std::string&, te::da::PrimaryKey*) {}
      
      void dropPrimaryKey(const std::string&) {}
      
      std::auto_ptr<te::da::ForeignKey> getForeignKey(const std::string& , const std::string& ) { return std::auto_ptr<te::da::ForeignKey>(); }
      
      std::vector<std::string> getForeignKeyNames(const std::string& ) { return std::vector<std::string>(); }
      
      bool foreignKeyExists(const std::string& , const std::string& ) { return false; }
      
      void addForeignKey(const std::string& , te::da::ForeignKey*) {}
      
      void dropForeignKey(const std::string& , const std::string& ) {}
      
      std::auto_ptr<te::da::UniqueKey> getUniqueKey(const std::string& , const std::string& ) 
      { return std::auto_ptr<te::da::UniqueKey>(); }
      
      std::vector<std::string> getUniqueKeyNames(const std::string& ) 
      { return std::vector<std::string>(); }
      
      bool uniqueKeyExists(const std::string& , const std::string& ) { return false; }
      
      void addUniqueKey(const std::string& , te::da::UniqueKey* ) {}
      
      void dropUniqueKey(const std::string& , const std::string& ) {}
      
      std::auto_ptr<te::da::CheckConstraint> getCheckConstraint(const std::string& , const std::string& ) 
      { return std::auto_ptr<te::da::CheckConstraint>(); }
      
      std::vector<std::string> getCheckConstraintNames(const std::string& ) 
      { return std::vector<std::string>(); }
      
      bool checkConstraintExists(const std::string& , const std::string& ) {return false; }
      
      void addCheckConstraint(const std::string& , te::da::CheckConstraint* ) {}
      
      void dropCheckConstraint(const std::string& , const std::string& ) {}
      
      std::auto_ptr<te::da::Index> getIndex(const std::string& , const std::string& ) 
      { return std::auto_ptr<te::da::Index>(); }
      
      std::vector<std::string> getIndexNames(const std::string& ) 
      { return std::vector<std::string>(); }
      
      bool indexExists(const std::string& , const std::string& )
      { return false; }
      
      void addIndex(const std::string& , te::da::Index* , const std::map<std::string, std::string>& ) {}
      
      void dropIndex(const std::string& , const std::string& ) {}
      
      bool sequenceExists(const std::string& )
      { return false; }
      
      std::auto_ptr<te::da::Sequence> getSequence(const std::string& )
      { return std::auto_ptr<te::da::Sequence>(); }
      
      std::vector<std::string> getSequenceNames()
      { return std::vector<std::string>(); }
      
      void addSequence(te::da::Sequence*) {}
      
      void dropSequence(const std::string&) {}

      void optimize(const std::map<std::string, std::string>& ){}

      te::common::CharEncoding getEncoding();
      
    private:
      
      boost::filesystem::path m_path;
      std::auto_ptr<te::da::DataSetType> getType(const std::string& dsfullname);  
      void getDataSetNames(const boost::filesystem::path& path, std::vector<std::string>& dsnames);
      bool hasDataSets(const boost::filesystem::path& path);
      size_t getNumberOfDataSets(const boost::filesystem::path& path);
      std::auto_ptr<te::da::DataSetType> getDataSetType(const boost::filesystem::path& path, const std::string& name, std::string& uri);
      
    };
  } // end namespace gdal
} // end namespace te

#endif // __TERRALIB_GDAL_INTERNAL_TRANSACTOR_H

