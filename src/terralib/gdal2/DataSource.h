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
 \file terralib/gdal/DataSource.h
 
 \brief The implementation of a DataSource that consists of datasets that can be decoded by the GDAL Library.
 */

#ifndef __TERRALIB_GDAL_INTERNAL_DATASOURCE_H
#define __TERRALIB_GDAL_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess2/datasource/DataSource.h"
#include "../dataaccess2/datasource/DataSourceCapabilities.h"
#include "Config.h"
#include "Exception.h"

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/cstdint.hpp>

// STL 
#include <map>
#include <memory>
#include <string>
#include <vector>

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
    class CheckConstraint;
    class DataSet;
    class DataSetType;
    class DataSource;
    class DataSourceCapabilities;
    class ForeignKey;
    class Index;
    class ObjectIdSet;
    class PrimaryKey;
    class Query;
    class Select;
    class Sequence;
    class SQLDialect;
    class UniqueKey;
    
    typedef boost::shared_ptr<DataSetType> DataSetTypePtr;
  }
  
  namespace gdal
  {
    /*!
     \class DataSource
     
     \brief A driver to access raster data using the GDAL library.
     
     This class is an implementation of a DataSource that consists of raster that can be decoded 
     by the GDAL - Geospatial Data Abstraction Library (http://www.gdal.org/).
     */
    class TEGDALEXPORT DataSource : public te::da::DataSource
    {
    public:
      
      DataSource();
      
      ~DataSource();
      
      std::string getType() const throw();
      
      const std::map<std::string, std::string>& getConnectionInfo() const throw();
      
      /*!
       \brief It sets information to be used when connecting to the data source.
       
       Examples of valid connection information strings are:
       <ul>
       <li>"uri=foo\bar\file.ext": refers to the single file "file.ext".</li>
       <li>"uri=foo\bar\": refers to all files located inside the directory "foo\bar".</li>
       <li>source=host='<host>' port:'<port>' dbname='<dbname>' user='<user>' password='<password>' [schema='<schema>'] [table='<raster_table>'] [where='<sql_where>'] [mode='<working_mode>']" : refers to a PostGIS WKT Raster.</li>
       </ul>
       
       \param connInfo Key-value-pairs (kvp) with the connection information.
       */
     void setConnectionInfo(const std::map<std::string, std::string>& connInfo) throw();
      
      void open() throw(Exception);
      
      void close() throw(Exception);  
      
      bool isOpened() const throw();
      
      bool isValid() const throw();
      
      const te::da::DataSourceCapabilities& getCapabilities() const throw();
      
      const te::da::SQLDialect* getDialect() const throw() { return 0; }
      
      void begin() throw(Exception);
      
      void commit() throw(Exception);
      
      void rollBack() throw(Exception);
      
      bool isInTransaction() const throw() { return false; }
      
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
      
      std::auto_ptr<te::da::DataSet> query(const te::da::Select& q,
                                           te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);
      
      std::auto_ptr<te::da::DataSet> query(const std::string& query, 
                                           te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);
      
      void execute(const te::da::Query& command) throw(Exception) ;
      
      void execute(const std::string& command) throw(Exception) ;
      
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
      
      std::auto_ptr<te::da::UniqueKey> getUniqueKey(const std::string& datasetName,
                                            const std::string& name) throw(Exception);
      
      std::vector<std::string> getForeignKeyNames(const std::string& datasetName) throw(Exception);
      
      
      std::auto_ptr<te::da::ForeignKey> getForeignKey(const std::string& datasetName,
                                              const std::string& name) throw(Exception);
      
      std::vector<std::string> getIndexNames(const std::string& datasetName) throw(Exception);
      
      std::auto_ptr<te::da::Index> getIndex(const std::string& datasetName,
                                    const std::string& name) throw(Exception);
      
      std::vector<std::string> getCheckConstraintNames(const std::string& datasetName) throw(Exception);
      
      std::auto_ptr<te::da::CheckConstraint> getCheckConstraint(const std::string& datasetName,
                                                        const std::string& name) throw(Exception);
      
      std::vector<std::string> getSequenceNames() throw(Exception);
      
      std::auto_ptr<te::da::Sequence> getSequence(const std::string& name) throw(Exception);
      
      std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                const std::string& propertyName) throw(Exception);
      
      std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                std::size_t propertyPos) throw(Exception);
      
      std::size_t getNumberOfItems(const std::string& datasetName) throw(Exception);
      
      bool hasDataSets() throw(Exception);
      
      bool datasetExists(const std::string& name)throw(Exception);
      
      bool primarykeyExists(const std::string& /*datasetName*/,
                            const std::string& /*name*/) throw(Exception);
      
      bool uniquekeyExists(const std::string& /*datasetName*/, const std::string& /*name*/) throw(Exception);
      
      bool foreignkeyExists(const std::string& /*datasetName*/,
                            const std::string& /*name*/) throw(Exception);
      
      bool checkConstraintExists(const std::string& /*datasetName*/,
                                 const std::string& /*name*/) throw(Exception);
      
      bool indexExists(const std::string& /*datasetName*/,
                       const std::string& /*name*/) throw(Exception);
      
      bool sequenceExists(const std::string& /*name*/) throw(Exception);
      
      void createDataSet(te::da::DataSetType* dt,
                         const std::map<std::string, std::string>& options) throw(Exception);
      
      void cloneDataSet(const std::string& name,
                        const std::string& cloneName,
                        const std::map<std::string, std::string>& options) throw(Exception) ;
      
      void dropDataSet(const std::string& name) throw(Exception);
      
      void renameDataSet(const std::string& name,
                         const std::string& newName)throw(Exception);
      
      void addProperty(const std::string& datasetName,
                       const te::dt::Property* p) throw(Exception);
      
      void dropProperty(const std::string& datasetName,
                        const std::string& propertyName) throw(Exception);
      
      void renameProperty(const std::string& datasetName,
                          const std::string& propertyName,
                          const std::string& newPropertyName) throw(Exception);
      
      void addPrimaryKey(const std::string& datasetName,
                         const te::da::PrimaryKey* pk) throw(Exception) {}
      
      void dropPrimaryKey(const std::string& datasetName,
                          const std::string& primaryKeyName) throw(Exception) {}
      
      void addUniqueKey(const std::string& datasetName,
                        const te::da::UniqueKey* uk) throw(Exception) {}
      
      void dropUniqueKey(const std::string& datasetName,
                         const std::string& uniqueKeyName) throw(Exception) {}
      
      void addIndex(const std::string& datasetName,
                    const te::da::Index* idx,
                    const std::map<std::string, std::string>& options)  throw(Exception){}
      
      void dropIndex(const std::string& datasetName,
                     const std::string& idxName) throw(Exception) {}
      
      void addForeignKey(const std::string& datasetName,
                         const te::da::ForeignKey* fk) throw(Exception) {}
      
      void dropForeignKey(const std::string& datasetName,
                          const std::string& fkName) throw(Exception) {}
      
      void addCheckConstraint(const std::string& datasetName,
                              const te::da::CheckConstraint* cc) throw(Exception) {}
      
      void dropCheckConstraint(const std::string& datasetName,
                               const std::string& name) throw(Exception) {}
      
      void createSequence(const te::da::Sequence* sequence) throw(Exception) {}
      
      void dropSequence(const std::string& name) throw(Exception){}
      
      void add(const std::string& datasetName,
               te::da::DataSet* d,
               const std::map<std::string, std::string>& options,
               std::size_t limit = 0) throw(Exception);
      
      void remove(const std::string& datasetName,
                  const te::da::ObjectIdSet* oids = 0) throw(Exception);
      
      void update(const std::string& datasetName,
             te::da::DataSet* dataset,
             const std::vector<std::size_t>& properties,
             const te::da::ObjectIdSet* oids,
             const std::map<std::string, std::string>& options,
             std::size_t limit = 0) throw(Exception);
      
      void optimize(const std::map<std::string, std::string>& opInfo);
      
      bool isDirectory() const { return m_isDirectory; }
      
      /*!
       \brief It sets the capabilities document.
       
       \param capabilities The GDAL data source capabilities.
       
       \note GDAL driver extended method.
       */
      static void setCapabilities(const te::da::DataSourceCapabilities& capabilities);

      std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& info) throw(Exception);

    protected:
      
      void create(const std::map<std::string, std::string>& dsInfo) throw(Exception);
      
      void drop(const std::map<std::string, std::string>& dsInfo) throw(Exception);
      
      bool exists(const std::map<std::string, std::string>& dsInfo) throw(Exception);

    private:
      
      std::map<std::string, std::string> m_connectionInfo;  //!< The connection parameters.
      std::map<std::string, te::da::DataSetTypePtr> m_dstypes;
      
      bool m_isOpened;                                      //!< Tells if the data source is opened.
      bool m_isDirectory;                                   //!< Tells if the data source refers to a directory.
      
      static te::da::DataSourceCapabilities sm_capabilities;  //!< GDAL capabilities.

    };
  } // end namespace gdal
} // end namespace te

#endif // __TERRALIB_GDAL_INTERNAL_DATASOURCE_H

