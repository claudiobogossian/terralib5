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
  \file terralib/dataaccess/datasource/DataSource.h

  \brief An abstract class for data providers like a DBMS, Web Services or a regular file.
*/

#ifndef __TERRALIB_DATAACCESS_DATASOURCE_INTERNAL_DATASOURCE_H
#define __TERRALIB_DATAACCESS_DATASOURCE_INTERNAL_DATASOURCE_H

// TerraLib
#include "../../common/Enums.h"
#include "../dataset/DataSet.h"
#include "../dataset/DataSetType.h"
#include "../../geometry/Enums.h"
#include "../Config.h"
#include "../Exception.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
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
    class CheckConstraint;
    class Connection;
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

    /*!
      \class DataSource

      \brief An abstract class for data providers like a DBMS, Web Services or a regular file.

      A data source is the fundamental class of the data access module and
      it may represent a data repository. Among other things, a repository can be:
      <ul>
      <li>a PostgreSQL database</li>
      <li>an Oracle database</li>
      <li>an OGC Web Feature Service</li>
      <li>a directory of ESRI shape-files</li>
      <li>a data stream</li>
      <li>

      A data source can be used to access data in a repository
      and it requires a driver implementation that must register a
      concrete factory for building data source objects
      of the type of the driver (DataSourceFactory).

      Each data source is characterized by a set of parameters that can be used
      to set up an access channel to its underlying repository. This information
      is referred as the data source connection information.

      A data source exposes the data contained in it as a collection of datasets.
      The information about the data stored in a data source may be retrieved.

      Besides the descriptive information about the underlying data repository,
      each data source also provides information about their capabilities.

      \sa DataSourceManager, DataSourceFactory, DataSet, DataSetType
    */
    class TEDATAACCESSEXPORT DataSource : public boost::noncopyable
    {
      public:

        /*! \brief Default constructor that can be called by subclasses. */
        DataSource();

        /*! \brief Virtual destructor. */
        virtual ~DataSource();

        /** @name Basic Methods of a Data Source
          *  Basic Methods for operating a data source.
          */
        //@{

        /*!
          \brief It returns the data source type name (in UPPER CASE). Ex: POSTGIS, SQLITE, WFS, WMS, or MYSQL.

          \return The data source type name. Ex: POSTGIS, SQLITE, WFS, WMS, or MYSQL.

          \note Each data source driver must have a unique name.

          \note Thread-safe!
        */
        virtual std::string getType() const = 0;

        /*!
          \brief It returns the set of parameters used to set up the access channel to the underlying repository.

          This is the connection information used by a data source in order to enter in an operational mode,
          when the open method is called.

          The key-value-pairs (kvp) may contain information about: maximum number of accepted connections,
          user name and password required for establishing a connection, the url of a service, or any other
          information needed by the data source to operate. This information is dependent on the data source
          driver, so check the driver documentation for any additional information about the kvp.

          \return An associative container (key-value-pair) with information about the data source.

          \note Not thread-safe!
        */
        virtual const std::map<std::string, std::string>& getConnectionInfo() const = 0;

        /*!
          \brief It sets the connection information to be used when connecting to the data source.

          \param connInfo Key-value-pairs (kvp) with the connection information.
        */
        virtual void setConnectionInfo(const std::map<std::string, std::string>& connInfo) = 0;

        /*!
          \brief It returns an object that can execute transactions in the context of a data source.

          Use this method to get an object that allows to retrieve dataset, to insert data,
          or to modify dataset types. You don't need to cache this kind of object because
          each driver in TerraLib already keeps a pooling. So, as soon as you have finished using it,
          release the connection to the pool.

          \return A pointer to an object that can execute transactions in the context of a data source.

          \exception Exception It throws an exception, if it is not possible to get a connection,
                               for example, if there is not an available connection.

          \note Thread-safe!
        */
        virtual Connection* getConnection();

        /*!
          \brief It releases the connection given returning it to the connections pool. Use this method
                 when you have finished to use the connection to make operations on a dataset.

          \param conn The connection to be released.

          \note Thread-safe!
        */
        virtual void closeConnection(te::da::Connection* conn);

        /*!
          \brief It opens the data source and makes it ready for use.

          If the subclass needs to open a connection to a database server, 
          to open a file, or to get information from a Web Service, this
          method can do this kind of job to prepare the data source to be
          in an operational mode. It will use the connection information provided by the setConnectionInfo methods.

          \note This method doesn't load the data source catalog.

          \note Not thread-safe!
        */
        virtual void open() = 0;

        /*!
          \brief It closes the data source and clears all the resources used by its internal communication channel.

          This method closes any connection, any opened file,
          and releases any other resources.

          \note This method doesn't clear the data source catalog.

          \note Not thread-safe!
        */
        virtual void close() = 0;

        /*!
          \brief It returns true if the data source is opened, otherwise it returns false.

          This method will not check if the data source is available for use,
          it will just answer if the data source has been already opened.
          If you want to know if the data source is available for use,
          check the isValid() method.

          \return It returns true if the data source is opened, otherwise it returns false.

          \note Not thread-safe!
        */
        virtual bool isOpened() const = 0;

        /*!
          \brief It checks if the data source is valid (available for use).

          For a DBMS, this will check the opened connections.
          For a WFS client, this will check if the server is reachable.
          For a file, this will check if it can be read.

          \return It returns true if the data source is available, otherwise it returns false.

          \note Not thread-safe!
        */
        virtual bool isValid() const = 0;

        /*!
          \brief It returns the known capabilities of the data source.
          
          The returned object has all the information about what the data source can perform.
          Here you will find if the data source implementation supports primary keys,
          foreign keys, if it can be used in a thread environment and much more information.

          \param capabilities The known capabilities of the data source.

          \note Thread-safe!
        */
        virtual const DataSourceCapabilities& getCapabilities() const = 0;

        /*!
          \brief It returns the data source SQL dialect, if one exists.

          \return The data source SQL dialect.

          \note Thread-safe!
        */
        virtual const SQLDialect* getDialect() const = 0;

        //@}

        /** @name Transactional Methods
          *  Methods for controlling transactions.
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
          \brief It returns true if a transaction is in progress, otherwise, false.

          \return True if a transaction is in progress, otherwise, false.

          \note Not thread-safe!
        */
        virtual bool isInTransaction() const = 0;

        //@}

        /** @name DataSet Retrieval
          *  Methods for retrieving data from the data source.
          */
        //@{

        /*!
          \brief It gets the dataset identified by the given name.

          \param name     The name of the dataset. It must be the same name as the DataSetType name in the DataSource catalog.
          \param travType The traverse type associated to the returned dataset. 

          \return The caller of this method will take the ownership of the returned data set.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> getDataSet(const std::string& name, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY) = 0;

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the specified property.

          \param name          The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
          \param propertyName  The name of a spatial property in order to apply the spatial filter.
          \param e             A rectangle to be used as a spatial filter when retrieving datasets.
          \param r             The spatial relation to be used during the filter.
          \param travType      The traversal type associated to the returned dataset.

          \return The caller of this method will take the ownership of the returned data set.

          \note The envelope coordinates should be in the same coordinate system as the dataset.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Envelope* e,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY) = 0;

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the given geometric property.

          \param name          The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
          \param propertyName  The name of a spatial property in order to apply the spatial filter.
          \param g             A geometry to be used as a spatial filter when retrieving datasets.
          \param r             The spatial relation to be used during the filter.
          \param travType      The traverse type associated to the returned dataset.

          \return The caller of this method will take the ownership of the returned data set.

          \note The geometry coordinates should be in the same coordinate system as the dataset.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Geometry* g,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY) = 0;

        /*
         \brief It gets the dataset identified by the given name using the set of objects identification.

         \param name     The name of the dataset. It must be the same name as the DataSetType name in the DataSource catalog.
         \param oids     A pointer to a set of objects identification. Do not pass null. Do not pass set empty.
         \param travType The traverse type associated to the returned dataset.
         
         \return The caller of this method will take the ownership of the returned DataSet.
         
         \exception Exception It can throws an exception if:
                    <ul>
                    <li>something goes wrong during data retrieval</li>
                    <li>if the data source driver doesn't support the traversal type</li>
                    <li>if the data source driver doesn't support the access policy</li>
                    </ul>
        */
        virtual std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                          const ObjectIdSet* oids, 
                                                          te::common::TraverseType travType = te::common::FORWARDONLY);

        /*!
          \brief It executes a query that may return some data using a generic query.

          This method is different of the method that accepts a dataset name and
          a spatial filter; this method allows the retrieving of only a
          subset of the attributes, since a query can include a property list.

          \param q        A valid query object.
          \param travType The traverse type associated to the returned dataset. 

          \return The caller of this method will take the ownership of the returned data set.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> query(const Select& q,
                                             te::common::TraverseType travType = te::common::FORWARDONLY) = 0;

        /*!
          \brief It executes a query that may return some data using the data source native language.

          \param query    A query string in the data source native language.
          \param travType The traverse type associated to the returned dataset.

          \return The caller of this method will take the ownership of the returned data set.

          \note Don't use this method, if you want portability for your application.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<DataSet> query(const std::string& query, 
                                             te::common::TraverseType travType = te::common::FORWARDONLY) = 0;
        //@}

        /** @name Command Execution Methods
          *  Methods for executing commands against the data source.
          */
        //@{

        /*!
          \brief It executes the specifed command using a generic query representation.

          \param command A query like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE.

          \note Not thread-safe!
        */
        virtual void execute(const Query& command) = 0;

        /*!
          \brief It executes the specifed command in the data source native language.

          \param command A query string in the data source native language (like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE).

          \note Not thread-safe!
        */
        virtual void execute(const std::string& command) = 0;

        //@}

        /** @name Command Cancelation Method
          *  Method for stopping the execution of a query or command against the data source.
          */
        //@{

        /*!
          \brief It requests that the data source stop the processing of the current command.

          \note Thread-safe!
        */
        virtual void cancel() = 0;

        //@}

        /** @name Auxiliary Commands
          *  Auxiliary commands.
          */
        //@{

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

          \param datasetName A dataset name to check its validity.

          \return True if the name is valid according to the data source rules.

          \note Not thread-safe!
        */
        virtual bool isDataSetNameValid(const std::string& datasetName) = 0;

        /*!
          \brief It returns true if the given string is a valid property name.

          \param propertyName A property name to check its validity.

          \return True if the name is valid according to the data source rules.

          \note Not thread-safe!
        */
        virtual bool isPropertyNameValid(const std::string& propertyName) = 0;

        //@}

        /** @name Dataset Metadata Retrieval
          *  Methods for retrieving metadata about datasets from the data source.
          */
        //@{

        /*!
          \brief It searches for the list of datasets available in the data source.

          \return A vector of strings containing the names of the datasets available in the data source.

          \note Each dataset in the data source must have a unique name. For example, in a DBMS the name
                may contain the schema name before the table name separated by a dot notation (".").

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getDataSetNames() = 0;

        /*!
          \brief It searches for the information about a given dataset in the data source.

          When you use this method you can get more information about a dataset:
          <ul>
          <li>the list of properties, including: name, data type, size, if the value is required or not, if it is an autoincrement</li>
          <li>primary key</li>
          <li>foreign keys</li>
          <li>unique keys</li>
          <li>check constraints</li>
          <li>indexes</li>
          </ul>

          \param name The name of the dataset we are looking information for.

          \return The dataset schema.

          \note Not thread-safe!
        */
        virtual const te::da::DataSetTypePtr& getDataSetType(const std::string& name) = 0;

        /*!
          \brief It searches for the list of property names of the given dataset.

          \param datasetName The dataset name.

          \return The property names of the given dataset.

          \note Each dataset in the data source must have a unique name. For example, in a DBMS the name
                may contain the schema name before the table name separated by a dot notation (".").

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getPropertyNames(const std::string& datasetName) = 0;

        /*!
          \brief It gets the number of properties of the given dataset.

          \param datasetName The dataset name.

          \return The number of properties of the given dataset.

          \note Not thread-safe!
        */
        virtual std::size_t getNumberOfProperties(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a property with the given name exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The property name.

          \return True, if the property exists in the dataset; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool propertyExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It retrieves the properties of the dataset.

          \param datasetName The dataset name.

          \return The properties of the dataset.

          \note Not thread-safe!
        */
        virtual boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName) = 0;

        /*!
          \brief It retrieves a property with the given name from the dataset.

          \param datasetName  The dataset name.
          \param propertyName The property name.

          \return The property with the given name from the dataset.

          \post The caller of this method will take the ownership of the returned property,
                because it is a clone of the one in the schema.

          \note Not thread-safe!
        */
        virtual te::dt::Property* getProperty(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It retrieves a property from the dataset.

          \param datasetName  The dataset name.
          \param propertyPos  The property position.

          \return The property in the given position.

          \post The caller of this method will take the ownership of the returned property,
                because it is a clone of the one in the schema.

          \note Not thread-safe!
        */
        virtual te::dt::Property* getProperty(const std::string& datasetName, std::size_t propertyPos) = 0;

        /*!
          \brief It adds a new property to the dataset schema.

          \param datasetName The dataset where the property will be added.
          \param p           The new property to be added.

          \note Don't delete the given property, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addProperty(const std::string& datasetName, te::dt::Property* p) = 0;

        /*!
          \brief It removes a property from the given dataset.

          \param datasetName  The dataset from where the given property will be removed.
          \param name         The property to be removed from the dataset.

          \note The client of this method must take care of the changes needed by a DataSetType or data source catalog.

          \note Not thread-safe!
        */
        virtual void dropProperty(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It renames a property of the given dataset.

          \param datasetName     The dataset containig the property to be renamed.
          \param propertyName    The property to be renamed from the dataset.
          \param newPropertyName The new property name.

          \note The client of this method must take care of the changes needed by a DataSetType or data source catalog.

          \note Not thread-safe!
        */
        virtual void renameProperty(const std::string& datasetName,
                                    const std::string& propertyName,
                                    const std::string& newPropertyName) = 0;

        /*!
          \brief It retrieves the primary key of the dataset.

          \param datasetName  The dataset name.

          \return The primary key of the dataset.

          \post The caller of this method will take the ownership of the returned primary key,
                because it is a clone of the one in the schema.

          \note Not thread-safe!
        */
        virtual PrimaryKey* getPrimaryKey(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a primary key with the given name exists in the data source.

          \param datasetName  The dataset name.
          \param name         The primary key name.

          \return True, if the primary key exists in the data source; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool primaryKeyExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds a primary key constraint to the dataset schema.

          \param datasetName  The name of the dataset to be added the primary key.
          \param pk           The primary key constraint.

          \note Don't delete the given primary key, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addPrimaryKey(const std::string& datasetName, PrimaryKey* pk) = 0;

        /*!
          \brief It removes the primary key constraint from the dataset schema.

          \param datasetName    The dataset from where the primary key wil be removed.

          \note The client of this method must take care of the changes needed by a DataSetType or data source catalog.

          \note Not thread-safe!
        */
        virtual void dropPrimaryKey(const std::string& datasetName) = 0;

        /*!
          \brief It searches for the foreign key names of the given dataset.

          \param datasetName The dataset name.

          \return The foreign key names of the given dataset.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getForeignKeyNames(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a foreign key with the given name exists in the data source.

          \param datasetName  The dataset name.
          \param name         The foreign key name.

          \return True, if the foreign key exists in the data source; otherwise, it returns false.
        */
        virtual bool foreignKeyExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It retrieves the foreign key with the given name.

          \param name The foreign key name.

          \return The foreign key with the given name in the dataset.

          \post The caller of this method will take the ownership of the returned foreign key,
                because it is a clone of the one in the schema.

          \note Not thread-safe!
        */
        virtual ForeignKey* getForeignKey(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds a foreign key constraint to a dataset.

          \param datasetName  The dataset where the foreign key constraint will be added.
          \param fk           The foreign key constraint.

          \note Don't delete the given foreign key, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addForeignKey(const std::string& datasetName, ForeignKey* fk) = 0;

        /*!
          \brief It removes the foreign key constraint from the dataset schema.

          \param datasetName  The dataset where the foreign key will be removed.
          \param fkName       The foreign key to be removed.

          \note The client of this method must take care of the changes needed by a DataSetType or data source catalog.

          \note Not thread-safe!
        */
        virtual void dropForeignKey(const std::string& datasetName, const std::string& fkName) = 0;

        /*!
          \brief It searches in the data source for the unique key names associated to the given dataset.

          \param datasetName The dataset name.

          \return The names of the unique keys of the dataset.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getUniqueKeyNames(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a unique key with the given name exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The unique key name.

          \return True, if the unique key exists in the data source; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool uniqueKeyExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It gets the unique key in the dataset with the given name.

          \param datasetName  The dataset name.
          \param name         The unique key name.

          \post The caller of this method will take the ownership of the returned property,
                because it is a clone of the one in the schema.

          \return The unique key with the given name in the dataset.

          \note Not thread-safe!
        */
        virtual te::da::UniqueKey* getUniqueKey(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds a unique key constraint to the dataset.

          \param datasetName  The dataset where the unique key will be added.
          \param uk           The unique key constraint.

          \note Don't delete the given unique key, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addUniqueKey(const std::string& datasetName, UniqueKey* uk) = 0;

        /*!
          \brief It removes the unique key constraint from the dataset.

          \param datasetName  The name of the dataset to be removed the unique key.
          \param name         The unique key constraint name.

          \note Not thread-safe!
        */
        virtual void dropUniqueKey(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It searches in the data source for check constraints associated to the given dataset.

          \param datasetName  The dataset name.

          \return The names of the check constraints of the dataset.

          \note Not thread-safe!
          */
        virtual std::vector<std::string> getCheckConstraintNames(const std::string& datasetName) = 0;

        /*!
          \brief It checks if a check-constraint with the given name exists in the data source.

          \param datasetName  The dataset name.
          \param name         The check-constraint name.

          \return True, if the check-constraint exists in the data source; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool checkConstraintExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It gets the check constraint with the given name.

          \param datasetName  The dataset name.
          \param name         The check constraint name.

          \return The check constraint with the given name.

          \post The caller of this method will take the ownership of the returned check constraint,
                because it is a clone of the one in the schema.

          \note Not thread-safe!
        */
        virtual te::da::CheckConstraint* getCheckConstraint(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds a check constraint to the dataset.

          \param datasetName  The dataset where the constraint will be added.
          \param cc           The check constraint.

          \note Don't delete the given check constraint, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addCheckConstraint(const std::string& datasetName, CheckConstraint* cc) = 0;
   
        /*!
          \brief It removes the check constraint from the dataset.

          \note The client of this method must take care of the changes needed by a DataSetType or data source catalog.

          \note Not thread-safe!
        */
        virtual void dropCheckConstraint(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It searches in the data source for the index names associated to the given dataset.

          \param datasetName  The dataset name.

          \return The index names of the given dataset.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getIndexNames(const std::string& datasetName) = 0;

        /*!
          \brief It checks if an index with the given name exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The index name.

          \return True, if the index exists in the data source; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool indexExists(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It gets the index with the given name. in the dataset.

          \param datasetName  The dataset name.
          \param name         The index name.

          \return The index with the given name.

          \post The caller of this method will take the ownership of the returned index,
                because it is a clone of the one in the schema.

          \note Not thread-safe!
        */
        virtual Index* getIndex(const std::string& datasetName, const std::string& name) = 0;

        /*!
          \brief It adds an index to the dataset.

          \param datasetName  The dataset where the index will be added.
          \param idx          The index to be added.
          \param options      A list of optional modifiers (driver specific).

          \note Don't delete the given index, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addIndex(const std::string& datasetName, Index* idx,
                              const std::map<std::string, std::string>& options) = 0; 

        /*!
          \brief It removes the index from the dataset schema.

          \param datasetName  The dataset where the index will be added.
          \param idxName      The index to be removed.

          \note The client of this method must take care of the changes needed by a DataSetType or data source catalog.

          \note Not thread-safe!
        */
        virtual void dropIndex(const std::string& datasetName, const std::string& idxName) = 0;

        /*!
          \brief It searches for the list of sequence names available in the data source.

          \note Each sequence in the data source must have a unique name. For example, in a DBMS the name
                may contain the schema name before the sequence name separated by a dot notation (".").

          \return The sequence names of the data source.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getSequenceNames() = 0;

        /*!
          \brief It checks if a sequence with the given name exists in the data source.

          \param name The sequence name.

          \return True, if the sequence exists in the data source; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool sequenceExists(const std::string& name) = 0;

        /*!
          \brief It gets the sequence with the given name

          \param name  The sequence name.

          \return The sequence with the given name.

          \post The caller of this method will take the ownership of the returned sequence,
                because it is a clone of the one in the catalog.

          \note Not thread-safe!
        */
        virtual Sequence* getSequence(const std::string& name) = 0;

        /*!
          \brief It creates a new sequence in the data source.

          \note Don't delete the given sequence, because the schema will take the ownership of it.
          \note Not thread-safe!
        */
        virtual void addSequence(Sequence* sequence) = 0;
   
        /*!
          \brief It removes the sequence from the data source.

          \param name The sequence that will be removed.

          \note The client of this method must take care of the changes needed by a DataSetType or data source catalog.

          \note Not thread-safe!
        */
        virtual void dropSequence(const std::string& name) = 0;

        /*!
          \brief It retrieves the bounding rectangle for the given dataset and spatial property.

          \param datasetName  The dataset name.
          \param propertyName The spatial property name.

          \return The spatial property bounding rectangle, or NULL if none can be retrieved.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                          const std::string& propertyName) = 0;

        /*!
          \brief It retrieves the bounding rectangle for the given dataset and spatial property position.

          \param datasetName  The dataset name.
          \param propertyPos  The spatial property position.

          \return The spatial property bounding rectangle, or NULL if none can be retrieved.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                          std::size_t propertyPos) = 0;

        /*!
          \brief It retrieves the number of items of the given dataset.

          \param datasetName  The dataset name.

          \return The number of items of the given dataset.

          \note Not thread-safe!
        */
        virtual std::size_t getNumberOfItems(const std::string& datasetName) = 0;

        /*!
          \brief It returns true if the data source has any dataset.

          \return True if the data source has datasets; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool hasDataSets() = 0;

        /*!
          \brief It checks if a dataset with the given name exists in the data source.

          \param name The dataset name.

          \return True, if the dataset exists in the data source; otherwise, it returns false.

          \note Not thread-safe!
          */
        virtual bool dataSetExists(const std::string& name) = 0;

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

          \note If you want to create a new schema based on an already existant one
                you must create a fresh copy of the DataSetType with the clone() method.

          \note Not thread-safe!
        */
        virtual void createDataSet(DataSetType* dt, const std::map<std::string, std::string>& options) = 0;

        /*!
          \brief It clones the dataset in the data source.

          \param name       The dataset to be cloned.
          \param cloneName  The name of the cloned dataset.
          \param options    A list of optional modifiers. It is driver specific.

          \note Not thread-safe!
        */
        virtual void cloneDataSet(const std::string& name,
                                  const std::string& cloneName,
                                  const std::map<std::string, std::string>& options) = 0;

        /*!
          \brief It drops the dataset schema from the data source.

          \param name The dataset name whose schema will be removed from the data source.

          \note The client of this method must take care of the changes needed by a data source catalog.
                Some attention is needed: changing in sequences, foreign keys and other stuffs.

          \note Not thread-safe!
        */
        virtual void dropDataSet(const std::string& name) = 0;

        /*!
          \brief It renames a dataset.

          \param name    The name of the dataset to be renamed.
          \param newName The new dataset name.

          \note The client of this method must take care of the changes needed by a data source catalog.

          \note Not thread-safe!
        */
        virtual void renameDataSet(const std::string& name,
                                    const std::string& newName) = 0;

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
          \param limit       The number of items to be used from the input dataset. If set to 0 (default) all items are used.

          \note The dataset reading will start in the current position.
                So, keep in mind that it is the caller responsability
                to inform the dataset 'd' in the right position (and a valid one) to start processing it.

          \note Not thread-safe!
        */
        virtual void add(const std::string& datasetName,
                         DataSet* d,
                         const std::map<std::string, std::string>& options,
                         std::size_t limit = 0) = 0;

        /*!
          \brief It removes all the informed items from the dataset.

          It removes all data collection identified by an 
          object identifier from the data source. If no OIDs are
          informed all data will be removed.

          \param datasetName The dataset name.
          \param oids        A list of object identifiers used to remove data from the datasource, or NULL for all.

          \note Not thread-safe!
        */
        virtual void remove(const std::string& datasetName,
                            const ObjectIdSet* oids = 0) = 0;

        /*!
          \brief It updates the dataset items in the data source based on OID list.

          \param datasetName The target dataset name.
          \param dataset     The list of data items to be updated.
          \param properties  The list of properties of the dataset to be updated.
          \param oids        The list of objects to be updated.
          \param options     A list of optional modifiers. It is driver specific.
          \param limit       The number of items to be used from the input dataset. If set to 0 (default) all items are used.

          \note The dataset reading will start in the
                current position. So, keep in mind that it is the caller responsability
                to inform the dataset 'd' in the right position (and a valid one) to start processing it.

          \note Not thread-safe!
        */
        virtual void update(const std::string& datasetName,
                            DataSet* dataset,
                            const std::vector<std::size_t>& properties,
                            const ObjectIdSet* oids,
                            const std::map<std::string, std::string>& options,
                            std::size_t limit = 0) = 0;

        //@}

        /** @name ????
          *  ????????.
          */
        //@{

        /*!
          \brief Create a new repository of a data source.

          \param dsType The type of data source to be created (example: POSTGIS, ORACLE, SQLITE).
          \param dsInfo The information for creating a new data source.

          \return A data source from the new data repository.

          \note Not thread-safe!
        */
        static std::auto_ptr<DataSource> create(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        /*!
          \brief Drop a repository of a data source.

          \param dsType The data source type name (example: POSTGIS, ORACLE, SQLITE).
          \param dsInfo The information for droping the data source.

          \note No other instance of the data source to be dropped can be opened when calling this method.

          \note Not thread-safe!
        */
        static void drop(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        /*!
          \brief It checks if the informed data source exists.

          \param dsType The data source type name (example: POSTGIS, ORACLE, SQLITE).
          \param dsInfo The data source information.

          \return True if the data source exists of false otherwise.

          \note Not thread-safe!
        */
        static bool exists(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        /*!
          \brief  Retrieve the list of repository names for data sources.

          \param dsType The data source type name (example: POSTGIS, ORACLE, SQLITE).
          \param dsInfo The data source information.

          \return The list of repository names for data sources.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& info) = 0;

        //@}

      protected:

        /** @name ????
          *  ????????.
          */
        //@{

        /*!
          \brief Create a new repository of a data source.

          \param dsInfo The information for creating a new data source.

          \note Not thread-safe!
        */
        virtual void create(const std::map<std::string, std::string>& dsInfo) = 0;

        /*!
          \brief Drop the repository of a data source.

          \param dsInfo The information for dropping a data source.

          \note Not thread-safe!
        */
        virtual void drop(const std::map<std::string, std::string>& dsInfo) = 0;

        /*!
          \brief Check the existence f a repository of a data source.

          \param dsInfo The data source information.

          \return True if the data source exists of false otherwise.

          \note Thread-safe!
        */
        virtual bool exists(const std::map<std::string, std::string>& dsInfo) = 0;

        //@}
    };

    typedef boost::shared_ptr<DataSource> DataSourcePtr;

  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_DATASOURCE_INTERNAL_DATASOURCE_H
