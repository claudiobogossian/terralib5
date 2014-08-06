/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/stmemory/Transactor.h

  \brief  An implementation of DataSourceTransactor class for the ST In-memory driver.
*/

#ifndef __TERRALIB_STMEMORY_INTERNAL_TRANSACTOR_H
#define __TERRALIB_STMEMORY_INTERNAL_TRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"

namespace te { namespace dt { class DateTimePeriod; } }  

namespace te
{
  namespace stmem
  { 
    // Forward declaration
    class DataSource;
	
    /*!
      \class Transactor

      \brief An implementation of DataSourceTransactor class for the ST in-memory driver. 

      \sa te::da::DataSourceTransactor
    */
    class TESTMEMORYEXPORT Transactor : public te::da::DataSourceTransactor
    {
      public:

        /*!
          \brief It constructs a Transactor.
        */
        Transactor(DataSource* parent);

        /*!
          \brief It constructs a Transactor.
        */
        ~Transactor();

        /** @name inherited methods
         *  
         */
        //@{

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
         
         bool checkConstraintExists(const std::string& datasetName, const std::string& name) ;
         
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
                         std::size_t limit = 0) ;
         
         void remove(const std::string& datasetName, const te::da::ObjectIdSet* oids = 0);
         
         void update(const std::string& datasetName,
                            te::da::DataSet* dataset,
                            const std::vector<std::size_t>& properties,
                            const te::da::ObjectIdSet* oids,
                            const std::map<std::string, std::string>& options,
                            std::size_t limit = 0);
         
         void optimize(const std::map<std::string, std::string>& opInfo);

         te::common::CharEncoding getEncoding();
         //@}

         /*!
          \brief It returns its associated ST In-Memory DataSource
         */
         DataSource* getSTMemDataSource() const;

         /*!
          \brief It adds the DataSet and its type associated to a given name.

          \param name The DataSet name.
          \param t    The DataSet Type.
          \param d    The DataSet.

          \note It will take the ownership of the given pointers.
         */
         void add(const std::string& name, te::da::DataSetType* t, DataSet* d);

         /*!
          \brief It returns a data set with observations whose phenomenon times satisfy a
                 given temporal relation. 

          The possible temporal relations are: 1. AFTER; 
          2. AFTER | EQUALS; 3. BEFORE;  4. BEFORE | EQUALS; 5. DURING; 6. EQUALS 

          \param name The DataSet name.
          \param  dt  A given datetime.
          \param  r   A given temporal relation.
          \param travType The traverse type associated to the returned dataset. 
          \param connected If the returned DataSet is connecet or not.
          
          \return A pointer to a new DataSet that shares the intenal content.

          \note The caller will take the ownership of the returned pointer.
          \note When the temporal relation is DURING, dt must be a time period.
          \note It can throw an Exception when internal errors occur.
        */
         std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                   const te::dt::DateTime* dt, te::dt::TemporalRelation r = te::dt::DURING,
                                                   te::common::TraverseType travType = te::common::FORWARDONLY, 
												                           bool connected = false,
                                                   const te::common::AccessPolicy accessPolicy = te::common::RAccess);

         /*!
          \brief It returns a data set with observations whose observed geometries satisfy a
                 given spatial relation and phenomenon times satisfy a given temporal relation. 

          The possible spatial relations are: INTERSECTS
          The possible temporal relations are: 1. AFTER; 
          2. AFTER | EQUALS; 3. BEFORE;  4. BEFORE | EQUALS; 5. DURING; 6. EQUALS

          \param name         The DataSet name.
          \param geom         A given geometry.
          \param sr           A given spatial relation.
          \param dt           A given datetime.
          \param tr           A given temporal relation.
          \param travType     The traverse type associated to the returned dataset. 
          \param connected    If the returned DataSet is connecet or not.
          
          \return A pointer to a new DataSet that shares the intenal content.

          \note The caller will take the ownership of the returned pointer.
          \note It will NOT take the ownership of the given pointers.
          \note When the temporal relation is DURING, dt must be a time period.
          \note It can throw an Exception when internal errors occur.
        */
        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                  const te::gm::Geometry* geom, te::gm::SpatialRelation sr,
                                                  const te::dt::DateTime* dt, te::dt::TemporalRelation tr = te::dt::DURING,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY, 
												                          bool connected = false,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        /*!
          \brief It returns a data set with observations whose observed geometries satisfy a
                 given spatial relation and phenomenon times satisfy a given temporal relation. 

          The possible spatial relations are: INTERSECTS
          The possible temporal relations are: 1. AFTER; 
          2. AFTER | EQUALS; 3. BEFORE;  4. BEFORE | EQUALS; 5. DURING; 6. EQUALS

          \param name         The DataSet name.
          \param e            A given envelope.
          \param sr           A given spatial relation.
          \param dt           A given datetime.
          \param tr           A given temporal relation.
          \param travType     The traverse type associated to the returned dataset. 
          \param connected    If the returned DataSet is connecet or not.
          
          \return A pointer to a new DataSet that shares the intenal content.

          \note The caller will take the ownership of the returned pointer.
          \note It will NOT take the ownership of the given pointers.
          \note When the temporal relation is DURING, dt must be a time period.
          \note It can throw an Exception when internal errors occur.
        */
        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                  const te::gm::Envelope* e, te::gm::SpatialRelation sr,
                                                  const te::dt::DateTime* dt, te::dt::TemporalRelation tr = te::dt::DURING,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY, 
												                          bool connected = false,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);
         /*!
          \brief It returns the temporal extent associated to a DataSet.

          \param name         The DataSet name.
          
          \return A pointer to its temporal extent.

          \note The caller will take the ownership of the returned pointer.          
          \note It can throw an Exception when internal errors occur.
        */
        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent(const std::string& name);

      protected:

        /*!
          \brief It returns the DataSetType associated to a given name.
        */
        te::da::DataSetType* getType(const std::string& datasetName);

        /*!
          \brief It returns the DataSet associated to a given name.
        */
        DataSet* getData(const std::string& datasetName);
		
      private:

         DataSource*        m_ds;          //!< The associated data source.
    };

  } // end namespace stmem
}   // end namespace te

#endif  // __TERRALIB_STMEMORY_INTERNAL_TRANSACTOR_H



