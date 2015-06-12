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
  \file terralib/dataaccess/datasource/DataSourceTransactor.h

  \brief A DataSourceTransactor can be viewed as a connection to the data source for reading/writing things into it.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCETRANSACTOR_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCETRANSACTOR_H

// TerraLib
#include "../../common/Enums.h"
#include "../../geometry/Enums.h"
#include "../dataset/CheckConstraint.h"
#include "../dataset/DataSetType.h"
#include "../dataset/DataSet.h"
#include "../dataset/ForeignKey.h"
#include "../dataset/Index.h"
#include "../dataset/PrimaryKey.h"
#include "../dataset/Sequence.h"
#include "../dataset/UniqueKey.h"
#include "../Config.h"
#include "BatchExecutor.h"
#include "PreparedQuery.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <set>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>


namespace te
{
  // Forward declarations
  namespace dt { class Property; }
  namespace gm { class Envelope; class Geometry; }

  namespace da
  {
    // Forward declarations
    class DataSetTypeCapabilities;
    class DataSource;
    class ObjectIdSet;
    class Query;
    class Select;
   
    /*!
      \class DataSourceTransactor

      \brief A DataSourceTransactor can be viewed as a connection to the data source for reading/writing things into it.

      A transactor can be used to create a transaction, a prepared query or a batch command executor.

      If you are planning a multi-thread application, it is better not to share
      the same transactor between threads because its methods are not thread-safe. Instead,
      use one transactor per thread.

      \ingroup dataaccess

      \sa DataSource, DataSet, Query, PreparedQuery, BatchExecutor
  */
    class TEDATAACCESSEXPORT DataSourceTransactor : public boost::noncopyable 
    {
      public:

    /*! \brief Default constructor that can be called by subclasses. */
        DataSourceTransactor();

        /*! \brief Virtual destructor. */
        virtual ~DataSourceTransactor();
    
    /*!
          \brief It returns the parent data source of the transactor.
        */
        virtual DataSource* getDataSource() const = 0;
    
        /** @name Transaction
         *  Methods for dealing with transactions.
         */
        //@{

        /*!
          \brief It starts a new transaction.
          
          \note Not thread-safe!
        */
        virtual void begin() = 0;

        /*!
          \brief It commits the transaction.

          After commiting or rolling back, you can start another transaction.

          \note Not thread-safe!
        */
        virtual void commit() = 0;

        /*!
          \brief It aborts the transaction. Any changes will be rolled-back.

          After commiting or rolling back, you can start another transaction.

          \note Not thread-safe!
        */
        virtual void rollBack() = 0;

        /*!
          \brief It returns true if a transaction is in progress, otherwise, it returns false.

          \return True, if a transaction is in progress, otherwise, false.
        */
        virtual bool isInTransaction() const = 0;
        //@}
    
        /** @name DataSet Retrieval
          *  Methods for retrieving data from the data source.
          */
        //@{

        /*!
          \brief It gets the dataset identified by the given name.
                 A dataset can be connected or disconnected. A connected dataset, after its creation through
                 the data source transactor, continues to depend on the connection given by its associated
                 data source. Differently, a disconnected dataset, after its creation, no more depends of the
                 connection given by the data source, and it continues to live after the connection has been
                 released to the data source.

          \param name      The dataset name.
          \param accessPolicy Access policy.
          \param travType  The traverse type associated to the returned dataset. 
          \param connected A flag to indicate if the returned dataset is connected or not.

          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     </ul>

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> getDataSet(const std::string& name, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY, 
                                                  bool connected = false,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess) = 0;

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the specified property.
                 A dataset can be connected or disconnected. A connected dataset, after its creation through
                 the data source transactor, continues to depend on the connection given by its associated
                 data source. Differently, a disconnected dataset, after its creation, no more depends of the
                 connection given by the data source, and it continues to live after the connection has been
                 released to the data source.

          \param name          The dataset name.
          \param propertyName  The name of the spatial property that will be used to apply the spatial filter.
          \param e             A rectangle to be used as a spatial filter when retrieving datasets.
          \param r             The spatial relation to be used during the filter.
          \param accessPolicy  Access policy.
          \param travType      The traversal type associated to the returned dataset.
          \param connected     A flag to indicate if the returned dataset is connected or not.

          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     </ul>

          \note The envelope coordinates should be in the same coordinate system as the dataset.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Envelope* e,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY, 
                                                  bool connected = false,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess) = 0;

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the given geometric property.
                 A dataset can be connected or disconnected. A connected dataset, after its creation through
                 the data source transactor, continues to depend on the connection given by its associated
                 data source. Differently, a disconnected dataset, after its creation, no more depends of the
                 connection given by the data source, and it continues to live after the connection has been
                 released to the data source.

          \param name          The dataset name.
          \param propertyName  The name of the spatial property that will be used to apply the spatial filter.
          \param g             The geometry that will be used as a spatial filter when retrieving the dataset.
          \param r             The spatial relation that will be used during the filtering.
          \param accessPolicy  Access policy.
          \param travType      The traverse type associated to the returned dataset.
          \param connected     A flag to indicate if the returned dataset is connected or not.

          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     </ul>

          \note The geometry coordinates should be in the same coordinate system as the dataset.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Geometry* g,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY, 
                                                  bool connected = false,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess) = 0;

        /*!
          \brief It gets the dataset identified by the given name using the set of objects identification.
                 A dataset can be connected or disconnected. A connected dataset, after its creation through
                 the data source transactor, continues to depend on the connection given by its associated
                 data source. Differently, a disconnected dataset, after its creation, no more depends of the
                 connection given by the data source, and it continues to live after the connection has been
                 released to the data source.

          \param name       The dataset name of the dataset.
          \param oids       A pointer for the set of objects. Do not pass null, nor an empty set.
          \param accessPolicy  Access policy.
          \param travType   The traverse type associated to the returned dataset.
          \param connected  A flag to indicate if the returned dataset is connected or not.

          \exception Exception It can throw an exception if:
                    <ul>
                    <li>something goes wrong during data retrieval</li>
                    <li>if the data source driver doesn't support the traversal type</li>
                    </ul>
        */
        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                  const ObjectIdSet* oids, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY, 
                                                  bool connected = false,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        /*!
          \brief It executes a query that may return some data using a generic query.
                 A dataset can be connected or disconnected. A connected dataset, after its creation through
                 the data source transactor, continues to depend on the connection given by its associated
                 data source. Differently, a disconnected dataset, after its creation, no more depends of the
                 connection given by the data source, and it continues to live after the connection has been
                 released to the data source.
                 This method is different of the method that accepts a dataset name and
                 a spatial filter, because it allows the retrieving of only a
                 subset of the attributes, since a query can include a property list.

          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     </ul>

          \param q          A valid query object.
          \param travType   The traverse type associated to the returned dataset. 
          \param connected  A flag to indicate if the returned dataset is connected or not.
          \param accessPolicy  Access policy.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> query(const Select& q,
                                             te::common::TraverseType travType = te::common::FORWARDONLY, 
                                             bool connected = false,
                                             const te::common::AccessPolicy accessPolicy = te::common::RAccess) = 0;

        /*!
          \brief It executes a query that may return some data using the data source native language.
                 A dataset can be connected or disconnected. A connected dataset, after its creation through
                 the data source transactor, continues to depend on the connection given by its associated
                 data source. Differently, a disconnected dataset, after its creation, no more depends of the
                 connection given by the data source, and it continues to live after the connection has been
                 released to the data source.

          \param query     A query string in the data source native language.
          \param travType  The traverse type associated to the returned dataset.
          \param connected  A flag to indicate if the returned dataset is connected or not.
          \param accessPolicy  Access policy.

          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     </ul>

          \note Don't use this method, if you want portability for your application.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> query(const std::string& query,
                                             te::common::TraverseType travType = te::common::FORWARDONLY, 
                                             bool connected = false,
                                             const te::common::AccessPolicy accessPolicy = te::common::RAccess) = 0;
        //@}
    
        /** @name Command Execution Methods

         *  Methods for executing commands against the data source.
         */
        //@{

        /*!
          \brief It executes the specified command using a generic query representation.

          \param command A query like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE.

          \exception Exception It can throw an exception, if the query cannot be performed.

          \note Not thread-safe!
        */
        virtual void execute(const Query& command) = 0;

        /*!
          \brief It executes the specifed command in the data source native language.

          \param command A query string in the data source native language (like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE).

          \exception Exception It can throw an exception, if the query cannot be performed.

          \note Not thread-safe!
        */
        virtual void execute(const std::string& command) = 0;
        //@}

        /** @name Fine Grained Transactor Objects
         *  Methods for retrieving fine-grained transactor objects.
         */
        //@{
        /*!
          \brief It creates a prepared query object that may be used for query commands (select, insert, update and delete) that are used repeatedly.

          \param qName The prepared query name. 

          \exception Exception An exception can be thrown if the prepared query cannot be performed.

          \return A prepared query object owned by the caller.
        */
        virtual std::auto_ptr<PreparedQuery> getPrepared(const std::string& qName = std::string("")) = 0;

        /*!
          \brief It creates a batch command executor.

          \exception Exception An exception can be thrown if the batch command executor cannot be performed.

          \return A batch command executor owned by the caller.
        */
        virtual std::auto_ptr<BatchExecutor> getBatchExecutor() = 0;
        //@}

    /** @name Auxiliary Commands for Commands and Queries
          *  Auxiliary methods for commands and queries.
          */
        //@{
        
    /*!
          \brief It requests that the data source stop the processing of the current command.

          \note Not thread-safe!
        */
        virtual void cancel() = 0;

    /*!
          \brief It returns the last id generated by an insertion command.

          \return The last id generated by an insertion command.

          \note This is the way to deal with auto-increment values.

          \note Not thread-safe!
        */
        virtual boost::int64_t getLastGeneratedId() = 0;

        /*!
          \brief It escapes a string for using in commands and queries.

          \param value Any string.

          \return A valid escaped string.

          \note Not thread-safe!
        */
        virtual std::string escape(const std::string& value) = 0;

        /*!
          \brief It returns true if the given string is a valid dataset name.

          \param datasetName A dataset name whose validity will be checked.

          \return True, if the name is valid according to the data source rules.

          \note Not thread-safe!
        */
        virtual bool isDataSetNameValid(const std::string& datasetName);

        /*!
          \brief It checks if the given property name is valid.

          \param propertyName A property name whose validity will be checked.

          \return True, if the name is valid according to the data source rules.

          \note Not thread-safe!
        */
        virtual bool isPropertyNameValid(const std::string& propertyName);
        //@}
    
        /** @name Dataset Metadata Retrieval
          *  Methods for retrieving metadata about the datasets of the data source.
          */
        //@{

        /*!
          \brief It It gets the dataset names available in the data source.

          \return The dataset names available in the data source.

          \exception Exception An exception can be thrown, if the dataset names could not be retrieved.

          \note Each dataset in the data source must have a unique name. For example, in a DBMS the name
                may contain the schema name before the table name separated by a dot notation (".").

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getDataSetNames() = 0;

    /*!
          \brief It retrieves the number of data sets available in the data source.

          \exception Exception An exception can be thrown, if the number of datasets could not be retrieved.

          \return The number of data sets available in the data source.

          \note Not thread-safe!
        */
        virtual std::size_t getNumberOfDataSets() = 0;
    
        /*!
          \brief It gets information about the given dataset.

          This method can provide the following information about a dataset:
          <ul>
          <li>the list of properties, including: name, data type, size, if the value is required or not, if it is an autoincrement</li>
          <li>primary key</li>
          <li>foreign keys</li>
          <li>unique keys</li>
          <li>check constraints</li>
          <li>indexes</li>
          </ul>

          \param name The name of the dataset we are looking information for.

          \exception Exception An exception can be thrown, if the information about the dataset could not be retrieved.

          \return The dataset schema.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSetType> getDataSetType(const std::string& name) = 0;

        /*!
        \brief It gets capabilities about a data set.

        \param name Name of the dataset.

        \return The capabilities of the data set.

        \note By default it returns a fully support capabilities. If a driver does not support any capabilty, it may reimplement this function.
       */
        virtual std::auto_ptr<te::da::DataSetTypeCapabilities> getCapabilities(const std::string& name);

        /*!
          \brief It retrieves the properties of the dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the dataset properties could not be retrieved.

          \return The properties of the dataset.

          \note Not thread-safe!
        */
        virtual boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName) = 0;

        /*!
          \brief It retrieves the property with the given name from the dataset.

          \param datasetName  The dataset name.
          \param propertyName The property name.

          \exception Exception An exception can be thrown, if the dataset property could not be retrieved.

          \return The property with the given name from the dataset.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It retrieves the property lying in the given position from the dataset.

          \param datasetName  The dataset name.
          \param propertyPos  The property position.

          \exception Exception An exception can be thrown, if the property lying in the given position could not be retrieved.

          \return The property in the given position.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos) = 0;

        /*!
          \brief It gets the property names of the given dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the property names of the dataset could not be retrieved.

          \return The property names of the dataset.

          \note Each dataset in the data source must have a unique name. For example, in a DBMS the name
                may contain the schema name before the table name separated by a dot notation (".").

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getPropertyNames(const std::string& datasetName) = 0;

        /*!
          \brief It gets the number of properties of the given dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the number of dataset properties could not be retrieved.

          \return The number of properties of the given dataset.

          \note Not thread-safe!
        */
        virtual std::size_t getNumberOfProperties(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a property with the given name exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The property name.

          \exception Exception An exception can be thrown, if the existence of the dataset property could not be obtained.

          \return True, if the property exists in the dataset; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool propertyExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds a new property to the dataset schema.

          \param datasetName The dataset where the property will be added.
          \param p           The new property to be added.

          \exception Exception An exception can be thrown, if the property could not be added to the dataset schema.

          \note Don't delete the given property, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addProperty(const std::string& datasetName, te::dt::Property* p) = 0;

        /*!
          \brief It removes a property from the given dataset.

          \param datasetName  The dataset from where the given property will be removed.
          \param name         The property to be removed from the dataset.

          \exception Exception An exception can be thrown, if the dataset property could not be removed.

          \note Not thread-safe!
        */
        virtual void dropProperty(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It renames a property of the given dataset.

          \param datasetName     The dataset containig the property to be renamed.
          \param propertyName    The property to be renamed from the dataset.
          \param newPropertyName The new property name.

          \exception Exception An exception can be thrown, if the dataset property could not be renamed.

          \note Not thread-safe!
        */
        virtual void renameProperty(const std::string& datasetName,
                                    const std::string& propertyName,
                                    const std::string& newPropertyName) = 0;

        virtual void changePropertyDefinition(const std::string& datasetName, const std::string& propName, te::dt::Property* newProp);

        /*!
          \brief It retrieves the primary key of the dataset.

          \param datasetName  The dataset name.

          \exception Exception An exception can be thrown, if the primary key could not be retrieved.

          \return If a primary key exists in the dataset, it is returned; otherwise, a NULL is returned.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::PrimaryKey> getPrimaryKey(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a primary key exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The primary key name.

          \exception Exception An exception can be thrown, if the existence of the primary key could not be determined.

          \return True, if a primary key exists in the dataset; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool primaryKeyExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds a primary key constraint to the dataset schema.

          \param datasetName  The name of the dataset where the primary key will be added.
          \param pk           The primary key constraint.

          \exception Exception An exception can be thrown, if the primary key could not be added to the dataset schema.

          \note Don't delete the given primary key, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addPrimaryKey(const std::string& datasetName, PrimaryKey* pk) = 0;

        /*!
          \brief It removes the primary key constraint from the dataset schema.

          \param datasetName  The dataset from where the primary key will be removed.

          \exception Exception An exception can be thrown, if the primary key could not be dropped from the dataset schema.

          \note Not thread-safe!
        */
        virtual void dropPrimaryKey(const std::string& datasetName) = 0;

        /*!
          \brief It retrieves the foreign key from the given dataset.

          \param datasetName The dataset name.
          \param name        The foreign key name.

          \exception Exception An exception can be thrown, if the foreign key could not be retrieved.

          \return If the foreign key exists in the dataset, it is returned; otherwise, a NULL is returned.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<ForeignKey> getForeignKey(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It gets the foreign key names of the given dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the foreign key names could not be retrieved.

          \return The foreign key names of the given dataset.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getForeignKeyNames(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a foreign key with the given name exists in the data source.

          \param datasetName  The dataset name.
          \param name         The foreign key name.

          \exception Exception An exception can be thrown, if the existence of the foreign key could not be obtained.

          \return True, if the foreign key exists in the dataset; otherwise, it returns false.
        */
        virtual bool foreignKeyExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds a foreign key constraint to a dataset.

          \param datasetName  The dataset where the foreign key constraint will be added.
          \param fk           The foreign key constraint.

          \exception Exception An exception can be thrown, if the foreign key could not be added to the dataset schema.

          \note Don't delete the given foreign key, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addForeignKey(const std::string& datasetName, ForeignKey* fk) = 0;

        /*!
          \brief It removes the foreign key constraint from the dataset schema.

          \param datasetName  The dataset where the foreign key will be removed.
          \param fkName       The foreign key to be removed.

          \exception Exception An exception can be thrown, if the foreign key could not be removed from the dataset schema.

          \note Not thread-safe!
        */
        virtual void dropForeignKey(const std::string& datasetName, const std::string& fkName) = 0;

        /*!
          \brief It gets the unique key in the dataset with the given name.

          \param datasetName  The dataset name.
          \param name         The unique key name.

          \exception Exception An exception can be thrown, if the unique key could not be retrieved.

          \return The unique key with the given name in the dataset.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::UniqueKey> getUniqueKey(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It gets the unique key names of the given dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the unique key names could not be obtained.

          \return The unique key names of the dataset.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getUniqueKeyNames(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a unique key with the given name exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The unique key name.

          \exception Exception An exception can be thrown, if the existence of the unique key could not be determined.

          \return True, if the unique key exists in the data source; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool uniqueKeyExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds a unique key constraint to the dataset.

          \param datasetName  The dataset where the unique key will be added.
          \param uk           The unique key constraint.

          \exception Exception An exception can be thrown, if the unique key could not be added to the dataset schema.

          \note Don't delete the given unique key, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addUniqueKey(const std::string& datasetName, UniqueKey* uk) = 0;

        /*!
          \brief It removes the unique key constraint from the dataset.

          \param datasetName  The dataset from where the unique key will be removed.
          \param name         The unique key constraint name.

          \exception Exception An exception can be thrown, if the unique key could not be removed from the dataset schema.

          \note Not thread-safe!
        */
        virtual void dropUniqueKey(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It gets the check constraint of the dataset with the given name.

          \param datasetName  The dataset name.
          \param name         The check constraint name.

          \exception Exception An exception can be thrown, if the check constraint could not be retrieved.

          \return The check constraint with the given name.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::CheckConstraint> getCheckConstraint(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It gets the check constraint names of the given dataset.

          \param datasetName  The dataset name.

          \exception Exception An exception can be thrown, if the check constraint names could not be retrieved.

          \return The check constraint names of the dataset.

          \note Not thread-safe!
          */
        virtual std::vector<std::string> getCheckConstraintNames(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a check-constraint with the given name exists in the data source.

          \param datasetName  The dataset name.
          \param name         The check-constraint name.

          \exception Exception An exception can be thrown, if the existence of the check constraint could not be determined.

          \return True, if the check-constraint exists in the dataset; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool checkConstraintExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds a check constraint to the dataset.

          \param datasetName  The dataset where the constraint will be added.
          \param cc           The check constraint.

          \exception Exception An exception can be thrown, if the check constraint could not be added to the dataset schema.

          \note Don't delete the given check constraint, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addCheckConstraint(const std::string& datasetName, CheckConstraint* cc) = 0;
   
        /*!
          \brief It removes the check constraint from the dataset.

          \param datasetName The dataset from where the check constraint will be removed.
          \param name        The check constraint to be removed.

          \exception Exception An exception can be thrown, if the check constraint could not be removed from the dataset schema.

          \note Not thread-safe!
        */
        virtual void dropCheckConstraint(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It gets the index with the given name from the dataset.

          \param datasetName  The dataset name.
          \param name         The index name.

          \exception Exception An exception can be thrown, if the index could not be retrieved.

          \return The index with the given name.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::Index> getIndex(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It gets the index names of the given dataset.

          \param datasetName  The dataset name.

          \exception Exception An exception can be thrown, if the index names could not be retrieved.

          \return The index names of the given dataset.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getIndexNames(const std::string& datasetName) = 0;

        /*!
          \brief It checks if an index with the given name exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The index name.

          \exception Exception An exception can be thrown, if the index existence could not be determined.

          \return True, if the index exists in the dataset; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool indexExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds an index to the dataset.

          \param datasetName  The dataset where the index will be added.
          \param idx          The index to be added.
          \param options      A list of optional modifiers (driver specific).

          \exception Exception An exception can be thrown, if the index could not be added to the dataset schema.

          \note Don't delete the given index, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addIndex(const std::string& datasetName, Index* idx,
                              const std::map<std::string, std::string>& options) = 0; 

        /*!
          \brief It removes the index from the dataset schema.

          \param datasetName  The dataset where the index will be removed.
          \param idxName      The index to be removed.

          \exception Exception An exception can be thrown, if the index could not be removed from the dataset schema.

          \note Not thread-safe!
        */
        virtual void dropIndex(const std::string& datasetName, const std::string& idxName) = 0;

        /*!
          \brief It gets the sequence with the given name in the data source.

          \param name  The sequence name.

          \exception Exception An exception can be thrown, if the sequence could not be retrieved from the data source.

          \return The sequence with the given name.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<Sequence> getSequence(const std::string& name) = 0;

        /*!
          \brief It gets the sequence names available in the data source.

          \note Each sequence in the data source must have a unique name. For example, in a DBMS the name
                may contain the schema name before the sequence name separated by a dot notation (".").

          \exception Exception An exception can be thrown, if the sequence names could not be retrieved.

          \return The sequence names of the data source.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getSequenceNames() = 0;

        /*!
          \brief It checks if a sequence with the given name exists in the data source.

          \param name The sequence name.

          \exception Exception An exception can be thrown, if the index existence could not be determined.

          \return True, if the sequence exists in the data source; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool sequenceExists(const std::string& name) = 0;

        /*!
          \brief It creates a new sequence in the data source.

          \exception Exception An exception can be thrown, if the sequence could not be added to the data source.

          \note Don't delete the given sequence, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addSequence(Sequence* sequence) = 0;
   
        /*!
          \brief It removes the sequence from the data source.

          \param name The sequence that will be removed.

          \exception Exception An exception can be thrown, if the sequence could not be removed from the data source.

          \note Not thread-safe!
        */
        virtual void dropSequence(const std::string& name) = 0;

        /*!
          \brief It retrieves the bounding rectangle of the spatial property for the given dataset.

          \param datasetName  The dataset name.
          \param propertyName The spatial property name.

          \exception Exception An exception can be thrown, if the extent of the geometry property could not be retrieved.

          \return The spatial property bounding rectangle, or NULL, if none can be retrieved.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                          const std::string& propertyName) = 0;

        /*!
          \brief It retrieves the bounding rectangle for the spatial property lying in the given position in the dataset.

          \param datasetName  The dataset name.
          \param propertyPos  The spatial property position.

          \exception Exception An exception can be thrown, if the extent of the geometry property lying in the given position could not be retrieved.

          \return The spatial property bounding rectangle, or NULL if none can be retrieved.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                          std::size_t propertyPos) = 0;

        /*!
          \brief It retrieves the number of items of the given dataset.

          \param datasetName  The dataset name.

          \exception Exception An exception can be thrown, if the number of items of the dataset could not be retrieved.

          \return The number of items of the given dataset.

          \note Not thread-safe!
        */
        virtual std::size_t getNumberOfItems(const std::string& datasetName) = 0;

        /*!
          \brief It checks if the data source has any dataset.

          \exception Exception An exception can be thrown, if it is not possible to check if the data source has datasets .

          \return True, if the data source has datasets; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool hasDataSets() = 0;

        /*!
          \brief It checks if a dataset with the given name exists in the data source.

          \param name The dataset name.

          \exception Exception An exception can be thrown, if the existence of a dataset in the data source could not be determined.

          \return True, if the dataset exists in the data source; otherwise, it returns false.

          \note Not thread-safe!
          */
        virtual bool dataSetExists(const std::string& name) = 0;

        /*!
          \brief It return the DataSource current encoding.

          \return The DataSource current encoding.
          */
        virtual te::common::CharEncoding getEncoding() = 0;
        //@}
    
    /** @name Dataset Schema Persistence Methods
          *  Methods for dealing with datasource and dataset schema changes.
          */
        //@{

        /*!
          \brief It creates the dataset schema definition in the target data source.

          If a dataset schema with the same name already exists in the target data source,
          this may throw an exception.

          After calling this method, the dataset schema may be updated.

          \param dt       The dataset schema to be created. It may be changed during the operation.
          \param options  A list of optional modifiers (driver specific).

          \pre The schema of a related dataset in a foreign key must be already in the data source.

          \post In some data sources, this method may output implicit indexes, sequences or constraints.
                The method, if necessary, will create and adjust the dataset schema.

          \post The caller of this method will take the ownership of the given schema.

          \note If you want to create a new schema based on an already existing one,
                you must create a fresh copy of the DataSetType with the clone() method.

          \note Not thread-safe!
        */
        virtual void createDataSet(DataSetType* dt, const std::map<std::string, std::string>& options) = 0;

        /*!
          \brief It clones the dataset in the data source.

          \param name       The dataset to be cloned.
          \param cloneName  The name of the cloned dataset.
          \param options    A list of optional modifiers. It is driver specific.

          \exception Exception An exception can be thrown, if the dataset schema could not be cloned.
          \note Not thread-safe!
        */
        virtual void cloneDataSet(const std::string& name,
                                  const std::string& cloneName,
                                  const std::map<std::string, std::string>& options) = 0;

        /*!
          \brief It removes the dataset schema from the data source.

          \param name The dataset name whose schema will be removed from the data source.

          \exception Exception An exception can be thrown, if the dataset could not be removed from the data source.

          \note Not thread-safe!
        */
        virtual void dropDataSet(const std::string& name) = 0;

        /*!
          \brief It renames a dataset.

          \param name    The name of the dataset to be renamed.
          \param newName The new dataset name.

          \exception Exception An exception can be thrown, if the dataset could not be renamed.

          \note Not thread-safe!
        */
        virtual void renameDataSet(const std::string& name, const std::string& newName) = 0;
        //@}

        /** @name Dataset Persistence Methods
          *  Methods for dealing with the persistence of data in a data source.
          */
        //@{

        /*!
          \brief It adds data items to the dataset in the data source.

          \param datasetName The target dataset name.
          \param d           The data items to be added to the dataset.
          \param options     A list of optional modifiers (driver specific).
          \param limit       The number of items to be used from the input dataset. If set to 0 (default), all items are used.

          \exception Exception An exception can be thrown, if the input dataset items could not be added to the given dataset.

          \note The dataset reading will start in the current position.
                So, keep in mind that it is the caller responsability
                to inform the right position(and a valid one) in the dataset 'd', to start the inserting process.

          \note Not thread-safe!
        */
        virtual void add(const std::string& datasetName,
                         DataSet* d,
                         const std::map<std::string, std::string>& options,
                         std::size_t limit = 0) = 0;

        /*!
          \brief It removes all the informed items from the dataset.

          It removes all the data items from a dataset which are identified by 
          a set of object identifiers. If this set is not informed, all items will be removed.

          \param datasetName The dataset name.
          \param oids        A list of object identifiers used to remove data from the datasource, or NULL for all.

          \exception Exception An exception can be thrown, if the data items could not be removed.

          \note Not thread-safe!
        */
        virtual void remove(const std::string& datasetName, const ObjectIdSet* oids = 0) = 0;

        /*!
          \brief It updates the contents of a dataset for the set of data items.

          \param datasetName The target dataset name.
          \param dataset     The list of data items to be updated.
          \param properties  The list of properties of the dataset to be updated.
          \param oids        The list of objects to be updated.
          \param options     A list of optional modifiers. It is driver specific.
          \param limit       The number of items to be used from the input dataset. If set to 0 (default) all items are used.

          \exception Exception An exception can be thrown, if the dataset could not be updated.

          \note The dataset reading will start in the
                current position. So, keep in mind that it is the caller responsability
                to inform the right position(and a valid one) for the dataset 'd', to start the updating process.

          \note Not thread-safe!
        */
        virtual void update(const std::string& datasetName,
                            DataSet* dataset,
                            const std::vector<std::size_t>& properties,
                            const ObjectIdSet* oids,
                            const std::map<std::string, std::string>& options,
                            std::size_t limit = 0) = 0;

        /*!
         \brief It updates the contents of a dataset.

         All rows are edited. The third parameter tells wich columns are edited for each row.

         \param datasetName Name pf the dataset.
         \param dataset Dataset with editions.
         \param properties Columns edited for each row. Note that the size of \a properties must be the same of the \a dataset.
         \param ids List of positions of the columns that identifies rows.

         \exception te::da::Exception An exception can be thrown, if the dataset could not be updated.
         */
        virtual void update(const std::string& datasetName,
                            DataSet* dataset,
                            const std::vector< std::set<int> >& properties,
                            const std::vector<size_t>& ids);

        /*!
          \brief For some data access drivers, this method will perform some operations to optimize the data storage.

          This is can be a typical maintenance command in database systems (like vacuum).

          \param opInfo Any information needed by the underlying driver in order to fine tune the optimization.

          \exception Exception It may throw an exception if something goes wrong.

          \note Not thread-safe!

          \note For some systems you must assure that no other thread are executing commands against the data source.

          \note For some drivers, this method has no effect.
        */
        virtual void optimize(const std::map<std::string, std::string>& opInfo) = 0;
        //@}
    };

    typedef boost::shared_ptr<DataSourceTransactor> DataSourceTransactorPtr;

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCETRANSACTOR_H
