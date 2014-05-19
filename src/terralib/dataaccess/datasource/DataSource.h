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
#include "../../geometry/Enums.h"
#include "../dataset/CheckConstraint.h"
#include "../dataset/DataSet.h"
#include "../dataset/DataSetType.h"
#include "../dataset/ForeignKey.h"
#include "../dataset/Index.h"
#include "../dataset/PrimaryKey.h"
#include "../dataset/Sequence.h"
#include "../dataset/UniqueKey.h"
#include "../Config.h"

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
    class DataSetTypeCapabilities;
    class DataSourceCapabilities;
    class DataSourceTransactor;
    class ObjectIdSet;
    class Query;
    class Select;
    class SQLDialect;
    class CheckConstraint;
    class ForeignKey;
    class Index;
    class PrimaryKey;
    class Sequence;
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

      \ingroup dataaccess

      \sa DataSourceManager, DataSourceFactory, DataSet, DataSetType
    */
    class TEDATAACCESSEXPORT DataSource : public boost::noncopyable
    {
      public:

        /*! \brief Default constructor that can be called by subclasses. */
        DataSource();

        /*! \brief Virtual destructor. */
        virtual ~DataSource();

        /*!
          \brief An identification value for the data source.

          \return The data source identification.
        */
        const std::string& getId() const;

        /*!
          \brief It sets the data source identification.

          \param id An identification value.
        */
        void setId(const std::string& id);

        /** @name Data Source Basic Methods
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

          \note Thread-safe!
        */
        virtual const std::map<std::string, std::string>& getConnectionInfo() const = 0;

        /*!
          \brief It sets the connection information to be used when connecting to the data source.

          \param connInfo Key-value-pairs (kvp) with the connection information.
        */
        virtual void setConnectionInfo(const std::map<std::string, std::string>& connInfo) = 0;

        /*!
          \brief It returns an object that can execute transactions in the context of a data source.

          Use this method to get an object that allows to retrieve a dataset, to insert or update data,
          or to modify dataset types(schemas). You don't need to cache this kind of object because each
          driver in TerraLib already keeps a connection pooling. So, as soon as you finish using
          the transactor, destroy it.

          \return A pointer to an object that can execute transactions in the context of a data source.

          \exception Exception An exception can be thrown, if it is not possible to get a transactor;
                               for example, if there is no available connection.

          \note Thread-safe!
        */
        virtual std::auto_ptr<DataSourceTransactor> getTransactor() = 0;

    /*!
          \brief It opens the data source and makes it ready for using.

          If the subclass needs to open a connection to a database server, 
          to open a file, or to get information from a Web Service, this
          method can do this kind of job to prepare the data source to be
          in an operational mode. It will use the connection information
          provided by the setConnectionInfo methods.

          \exception Exception An exception can be thrown, if the data source cannot be opened.

          \note Not thread-safe!
        */
        virtual void open() = 0;

        /*!
          \brief It closes the data source and clears all the resources used by its internal communication channel.

          This method closes any connection, any opened file, and releases any other resources.

          \exception Exception An exception can be thrown, if the data source cannot be closed.

          \note Not thread-safe!
        */
        virtual void close() = 0;

        /*!
          \brief It returns true if the data source is opened, otherwise it returns false.

          This method will not check if the data source is available for using;
          it will just answer if the data source has already been opened.
          If you want to know if the data source is available for using,
          check the isValid() method.

          \return It returns true if the data source is opened; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool isOpened() const = 0;

        /*!
          \brief It checks if the data source is valid (available for using).

          For a DBMS, it will check the opened connections.
          For a WFS client, it will check if the server is reachable.
          For a file, it will check if it can be read.

          \return It returns true if the data source is available; otherwise, it returns false.

          \note Not thread-safe!
        */
        virtual bool isValid() const = 0;

        /*!
          \brief It returns the known capabilities of the data source.
          
          The returned object has all the information about the operations the data source can perform.
          Here you will find if the data source implementation supports primary keys,
          foreign keys, if it can be used in a thread environment and much more information.

          \param capabilities The known capabilities of the data source.

          \note Thread-safe!
        */
        virtual const DataSourceCapabilities& getCapabilities() const = 0;

        /*!
          \brief It returns the data source SQL dialect, if there is one.

          \return The data source SQL dialect.

          \note Thread-safe!
        */
        virtual const SQLDialect* getDialect() const = 0;
        //@}

        /** @name Data Retrieval
          *  Methods for retrieving data from the data source.
          */
        //@{

        /*!
          \brief It gets the dataset identified by the given name.
                 This method always returns a disconnected dataset, that is, a dataset that no more depends
                 of the data source that provided the connection for its creation. Therefore, the disconnected
                 dataset continues to live after the connection given by the data source has been released.

          \param name     The dataset name.
          \param accessPolicy Access policy.
          \param travType The traverse type associated to the returned dataset. 

          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     </ul>

        \note Thread-safe!
        */
        virtual std::auto_ptr<DataSet> getDataSet(const std::string& name, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the specified property.
                 This method always returns a disconnected dataset, that is, a dataset that no more depends
                 of the data source that provided the connection for its creation. Therefore, the disconnected
                 dataset continues to live after the connection given by the data source has been released.

          \param name          The dataset name.
          \param propertyName  The name of the spatial property used to apply the spatial filter.
          \param e             A rectangle used as a spatial filter when retrieving the dataset.
          \param r             The spatial relation used during the filtering.
          \param accessPolicy  Access policy.
          \param travType      The traversal type associated to the returned dataset.

          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     </ul>

          \note The envelope coordinates should be in the same coordinate system as the dataset.

          \note Thread-safe!
        */
        virtual std::auto_ptr<DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Envelope* e,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        /*!
          \brief It gets the dataset identified by the given name using a spatial filter over the given geometric property.
                 This method always returns a disconnected dataset, that is, a dataset that no more depends
                 of the data source that provided the connection for its creation. Therefore, the disconnected
                 dataset continues to live after the connection given by the data source has been released.

          \param name          The dataset name.
          \param propertyName  The name of the spatial property used to apply the spatial filter.
          \param g             The geometry used as a spatial filter when retrieving the dataset.
          \param r             The spatial relation used during the filtering.
          \param accessPolicy  Access policy.
          \param travType      The traverse type associated to the returned dataset.

          \note The geometry coordinates should be in the same coordinate system as the dataset.

          \note Thread-safe!
        */
        virtual std::auto_ptr<DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Geometry* g,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        /*!
          \brief It gets the dataset identified by the given name using the identification of the objects.
                 This method always returns a disconnected dataset, that is, a dataset that no more depends
                 of the data source that provided the connection for its creation. Therefore, the disconnected
                 dataset continues to live after the connection given by the data source has been released.

          \param name     The dataset name.
          \param oids     A pointer to the set of objects. Do not pass a null pointer nor an empty set.
          \param accessPolicy Access policy.
          \param travType The traverse type associated to the returned dataset.

          \exception Exception It can throw an exception if:
                    <ul>
                    <li>something goes wrong during data retrieval</li>
                    <li>if the data source driver doesn't support the traversal type</li>
                    </ul>
          
          \note Thread-safe!
        */
        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                  const te::da::ObjectIdSet* oids, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY,
                                                  const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        /*!
          \brief It executes a query that may return some data using a generic query.
                 This method always returns a disconnected dataset, that is, a dataset that no more depends
                 of the data source that provided the connection for its creation. Therefore, the disconnected
                 dataset continues to live after the connection given by the data source has been released.

                 This method is different of the method that accepts a dataset name and
                 a spatial filter; this method allows the retrieving of only a
                 subset of the attributes, since a query can include a property list.

          \param q         A valid query object.
          \param travType  The traverse type associated to the returned dataset. 
          \param accessPolicy Access policy.

          \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     </ul>

          \note Thread-safe!
        */
        virtual std::auto_ptr<DataSet> query(const Select& q,
                                             te::common::TraverseType travType = te::common::FORWARDONLY,
                                             const te::common::AccessPolicy accessPolicy = te::common::RAccess);

        /*!
          \brief It executes a query that may return some data using the data source native language.
                 This method always returns a disconnected dataset, that is, a dataset that is no more dependent
                 of the data source that provided the connection for its creation. Therefore, the disconnected
                 dataset continues to live after the connection given by the data source has been released.

          \param query    A query string in the data source native language.
          \param travType The traverse type associated to the returned dataset.
          \param accessPolicy Access policy.

         \exception Exception It can throw an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     </ul>
                     
          \note Don't use this method, if you want portability for your application.
          \note Thread-safe!
        */
        virtual std::auto_ptr<DataSet> query(const std::string& query, 
                                             te::common::TraverseType travType = te::common::FORWARDONLY,
                                             const te::common::AccessPolicy accessPolicy = te::common::RAccess);
        //@}

        /** @name Command Execution Methods
          *  Methods for executing commands against the data source.
          */
        //@{
        /*!
          \brief It executes the specified command using a generic query representation.

          \param command A query like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE.

          \exception Exception An exception can be thrown if the query cannot be performed.

          \note Thread-safe!
        */
        virtual void execute(const Query& command);

        /*!
          \brief It executes the specified command in the data source native language.

          \param command A query string in the data source native language (like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE).

          \exception Exception An exception can be thrown if the query can not be performed.

          \note Thread-safe!
        */
        virtual void execute(const std::string& command);
        //@}

        /** @name Auxiliary Methods for Commands and Queries
          *  Auxiliary methods for commands and queries.
          */
        //@{        
        /*!
          \brief It escapes a string for using in commands and queries.

          \param value Any string.

          \return A valid escaped string.

          \note Thread-safe!
        */
        virtual std::string escape(const std::string& value);

        /*!
          \brief It checks if the given dataset name is valid.

          \param datasetName A dataset name whose validity will be checked.

          \return True, if the name is valid according to the data source rules.

          \note Thread-safe!
        */
        virtual bool isDataSetNameValid(const std::string& datasetName);

        /*!
          \brief It checks if the given property name is valid.

          \param propertyName A property name whose validity will be checked.

          \return True, if the name is valid according to the data source rules.

          \note Thread-safe!
        */
        virtual bool isPropertyNameValid(const std::string& propertyName);
        //@}

        /** @name Dataset Metadata Retrieval
          *  Methods for retrieving metadata about the datasets of the data source.
          */
        //@{
        /*!
          \brief It gets the dataset names available in the data source.

          \return The dataset names available in the data source.

          \exception Exception An exception can be thrown, if the dataset names could not be retrieved.

          \note Each dataset in the data source must have a unique name. For example, in a DBMS the name
                may contain the schema name before the table name separated by a dot notation (".").

          \note Thread-safe!
        */
        virtual std::vector<std::string> getDataSetNames();
    
        /*!
          \brief It retrieves the number of data sets available in the data source.

          \exception Exception An exception can be thrown, if the number of datasets could not be retrieved.

          \return The number of data sets available in the data source.

          \note Thread-safe!
        */
        virtual std::size_t getNumberOfDataSets();

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

          \note Thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSetType> getDataSetType(const std::string& name);

        /*!
         \brief It gets capabilities about a data set.

         \param name Name of the dataset.

         \return The capabilities of the data set.
        */
        std::auto_ptr<te::da::DataSetTypeCapabilities> getCapabilities(const std::string& name);

        /*!
          \brief It retrieves the properties of the dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the dataset properties could not be retrieved.

          \return The dataset properties.

          \note Thread-safe!
        */
        virtual boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName);

        /*!
          \brief It retrieves the property with the given name from the dataset.

          \param datasetName  The dataset name.
          \param propertyName The property name.

          \exception Exception An exception can be thrown, if the dataset property could not be retrieved.

          \return The property with the given name from the dataset.

          \note Thread-safe!
        */
        virtual std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& name);

        /*!
          \brief It retrieves the property lying in the given position from the dataset.

          \param datasetName  The dataset name.
          \param propertyPos  The property position.

          \exception Exception An exception can be thrown, if the property lying in the given position could not be retrieved.

          \return The property in the given position.

          \note Thread-safe!
        */
        virtual std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos);

        /*!
          \brief It gets the property names of the given dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the property names of the dataset could not be retrieved.

          \return The property names of the dataset.

          \note Each dataset in the data source must have a unique name. For example, in a DBMS the name
                may contain the schema name before the table name separated by a dot notation (".").

          \note Thread-safe!
        */
        virtual std::vector<std::string> getPropertyNames(const std::string& datasetName);

        /*!
          \brief It gets the number of properties of the given dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the number of dataset properties could not be retrieved.

          \return The number of dataset properties.

          \note Thread-safe!
        */
        virtual std::size_t getNumberOfProperties(const std::string& datasetName);

        /*!
          \brief It checks if a property with the given name exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The property name.

          \exception Exception An exception can be thrown, if the existence of the dataset property could not be obtained.

          \return True, if the property exists in the dataset; otherwise, it returns false.

          \note Thread-safe!
        */
        virtual bool propertyExists(const std::string& datasetName, const std::string& name);

        /*!
          \brief It adds a new property to the dataset schema.

          \param datasetName The dataset where the property will be added.
          \param p           The new property to be added.

          \exception Exception An exception can be thrown, if the property could not be added to the dataset schema.

          \note Don't delete the given property, because the schema will take the ownership of it.
          \note Thread-safe!
        */
        virtual void addProperty(const std::string& datasetName, te::dt::Property* p);

        /*!
          \brief It removes a property from the given dataset.

          \param datasetName  The dataset from where the given property will be removed.
          \param name         The property to be removed from the dataset.

          \exception Exception An exception can be thrown, if the dataset property could not be removed.

          \note Thread-safe!
        */
        virtual void dropProperty(const std::string& datasetName, const std::string& name);

        /*!
          \brief It renames a property of the given dataset.

          \param datasetName     The dataset containing the property to be renamed.
          \param propertyName    The property to be renamed from the dataset.
          \param newPropertyName The new property name.

          \exception Exception An exception can be thrown, if the dataset property could not be renamed.

          \note Thread-safe!
        */
        virtual void renameProperty(const std::string& datasetName,
                                    const std::string& propertyName,
                                    const std::string& newPropertyName);

        virtual void changePropertyDefinition(const std::string& datasetName, const std::string& propName, te::dt::Property* newProp);

        virtual void changePropertiesDefinitions(const std::string& datasetName, const std::vector<std::string>& propsNames, const std::vector<te::dt::Property*> newProps);

        /*!
          \brief It retrieves the primary key of the dataset.

          \param datasetName  The dataset name.

          \exception Exception An exception can be thrown, if the primary key could not be retrieved.

          \return If a primary key exists in the dataset, it is returned; otherwise, a NULL is returned.

          \note Thread-safe!
        */
        virtual std::auto_ptr<te::da::PrimaryKey> getPrimaryKey(const std::string& datasetName);

        /*!
          \brief It checks if a primary key exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The primary key name.

          \exception Exception An exception can be thrown, if the existence of the primary key could not be determined.

          \return True, if a primary key exists in the dataset; otherwise, it returns false.

          \note Thread-safe!
        */
        virtual bool primaryKeyExists(const std::string& datasetName, const std::string& name);

        /*!
          \brief It adds a primary key constraint to the dataset schema.

          \param datasetName  The name of the dataset where the primary key will be added.
          \param pk           The primary key constraint.

          \exception Exception An exception can be thrown, if the primary key could not be added to the dataset schema.

          \note Don't delete the given primary key, because the schema will take the ownership of it.
          \note Thread-safe!
        */
        virtual void addPrimaryKey(const std::string& datasetName, PrimaryKey* pk);

        /*!
          \brief It removes the primary key constraint from the dataset schema.

          \param datasetName  The dataset from where the primary key will be removed.

          \exception Exception An exception can be thrown, if the primary key could not be dropped from the dataset schema.

          \note Thread-safe!
        */
        virtual void dropPrimaryKey(const std::string& datasetName);

        /*!
          \brief It retrieves the foreign key from the given dataset.

          \param datasetName The dataset name.
          \param name        The foreign key name.

          \exception Exception An exception can be thrown, if the foreign key could not be retrieved.

          \return If the foreign key exists in the dataset, it is returned; otherwise, a NULL is returned.

          \note Thread-safe!
        */
        virtual std::auto_ptr<ForeignKey> getForeignKey(const std::string& datasetName, const std::string& name);

        /*!
          \brief It gets the foreign key names of the given dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the foreign key names could not be retrieved.

          \return The foreign key names of the dataset.

          \note Thread-safe!
        */
        virtual std::vector<std::string> getForeignKeyNames(const std::string& datasetName);

        /*!
          \brief It checks if a foreign key with the given name exists in the data source.

          \param datasetName  The dataset name.
          \param name         The foreign key name.

          \exception Exception An exception can be thrown, if the existence of the foreign key could not be obtained.

          \return True, if the foreign key exists in the dataset; otherwise, it returns false.
        */
        virtual bool foreignKeyExists(const std::string& datasetName, const std::string& name);

        /*!
          \brief It adds a foreign key constraint to a dataset.

          \param datasetName  The dataset where the foreign key constraint will be added.
          \param fk           The foreign key constraint.

          \exception Exception An exception can be thrown, if the foreign key could not be added to the dataset schema.

          \note Don't delete the given foreign key, because the schema will take the ownership of it.
          \note Thread-safe!
        */
        virtual void addForeignKey(const std::string& datasetName, ForeignKey* fk);

        /*!
          \brief It removes the foreign key constraint from the dataset schema.

          \param datasetName  The dataset where the foreign key will be removed.
          \param fkName       The foreign key to be removed.

          \exception Exception An exception can be thrown, if the foreign key could not be removed from the dataset schema.

          \note Thread-safe!
        */
        virtual void dropForeignKey(const std::string& datasetName, const std::string& fkName);

        /*!
          \brief It gets the unique key in the dataset with the given name.

          \param datasetName  The dataset name.
          \param name         The unique key name.

          \exception Exception An exception can be thrown, if the unique key could not be retrieved.

          \return The unique key with the given name in the dataset.

          \note Thread-safe!
        */
        virtual std::auto_ptr<te::da::UniqueKey> getUniqueKey(const std::string& datasetName, const std::string& name);

        /*!
          \brief It gets the unique key names of the given dataset.

          \param datasetName The dataset name.

          \exception Exception An exception can be thrown, if the unique key names could not be obtained.

          \return The unique key names of the dataset.

          \note Thread-safe!
        */
        virtual std::vector<std::string> getUniqueKeyNames(const std::string& datasetName);

        /*!
          \brief It checks if a unique key with the given name exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The unique key name.

          \exception Exception An exception can be thrown, if the existence of the unique key could not be determined.

          \return True, if the unique key exists in the dataset; otherwise, it returns false.

          \note Thread-safe!
        */
        virtual bool uniqueKeyExists(const std::string& datasetName, const std::string& name);

        /*!
          \brief It adds a unique key constraint to the dataset.

          \param datasetName  The dataset where the unique key will be added.
          \param uk           The unique key constraint.

          \exception Exception An exception can be thrown, if the unique key could not be added to the dataset schema.

          \note Don't delete the given unique key, because the schema will take the ownership of it.
          \note Thread-safe!
        */
        virtual void addUniqueKey(const std::string& datasetName, UniqueKey* uk);

        /*!
          \brief It removes the unique key constraint from the dataset.

          \param datasetName  The dataset from where the unique key will be removed.
          \param name         The unique key constraint name.

          \exception Exception An exception can be thrown, if the unique key could not be removed from the dataset schema.

          \note Thread-safe!
        */
        virtual void dropUniqueKey(const std::string& datasetName, const std::string& name);

        /*!
          \brief It gets the check constraint of the dataset with the given name.

          \param datasetName  The dataset name.
          \param name         The check constraint name.

          \exception Exception An exception can be thrown, if the check constraint could not be retrieved.

          \return The check constraint with the given name.

          \note Thread-safe!
        */
        virtual std::auto_ptr<te::da::CheckConstraint> getCheckConstraint(const std::string& datasetName, const std::string& name);

        /*!
          \brief It gets the check constraint names of the given dataset.

          \param datasetName  The dataset name.

          \exception Exception An exception can be thrown, if the check constraint names could not be retrieved.

          \return The check constraint names of the dataset.

          \note Thread-safe!
        */
        virtual std::vector<std::string> getCheckConstraintNames(const std::string& datasetName);

        /*!
          \brief It checks if a check-constraint with the given name exists in the data source.

          \param datasetName  The dataset name.
          \param name         The check-constraint name.

          \exception Exception An exception can be thrown, if the existence of the check constraint could not be determined.

          \return True, if the check-constraint exists in the dataset; otherwise, it returns false.

          \note Thread-safe!
        */
        virtual bool checkConstraintExists(const std::string& datasetName, const std::string& name);

        /*!
          \brief It adds a check constraint to the dataset.

          \param datasetName  The dataset where the constraint will be added.
          \param cc           The check constraint to be added.

          \exception Exception An exception can be thrown, if the check constraint could not be added to the dataset schema.

          \note Don't delete the given check constraint, because the schema will take the ownership of it.
          \note Thread-safe!
        */
        virtual void addCheckConstraint(const std::string& datasetName, CheckConstraint* cc);
   
        /*!
          \brief It removes the check constraint from the dataset.

          \param datasetName The dataset from where the check constraint will be removed.
          \param name        The check constraint to be removed.

          \exception Exception An exception can be thrown, if the check constraint could not be removed from the dataset schema.

          \note Thread-safe!
        */
        virtual void dropCheckConstraint(const std::string& datasetName, const std::string& name);

        /*!
          \brief It gets the index with the given name from the dataset.

          \param datasetName  The dataset name.
          \param name         The index name.

          \exception Exception An exception can be thrown, if the index could not be retrieved.

          \return The index from the given dataset.

          \note Thread-safe!
        */
        virtual std::auto_ptr<te::da::Index> getIndex(const std::string& datasetName, const std::string& name);

        /*!
          \brief It gets the index names of the given dataset.

          \param datasetName  The dataset name.

          \exception Exception An exception can be thrown, if the index names could not be retrieved.

          \return The index names of the given dataset.

          \note Thread-safe!
        */
        virtual std::vector<std::string> getIndexNames(const std::string& datasetName);

        /*!
          \brief It checks if an index with the given name exists in the dataset.

          \param datasetName  The dataset name.
          \param name         The index name.

          \exception Exception An exception can be thrown, if the index existence could not be determined.

          \return True, if the index exists in the dataset; otherwise, it returns false.

          \note Thread-safe!
        */
        virtual bool indexExists(const std::string& datasetName, const std::string& name);

        /*!
          \brief It adds an index to the dataset.

          \param datasetName  The dataset where the index will be added.
          \param idx          The index to be added.
          \param options      A list of optional modifiers (driver specific).

          \exception Exception An exception can be thrown, if the index could not be added to the dataset schema.

          \note Don't delete the given index, because the schema will take the ownership of it.
          \note Thread-safe!
        */
        virtual void addIndex(const std::string& datasetName, Index* idx,
                              const std::map<std::string, std::string>& options); 

        /*!
          \brief It removes the index from the given dataset.

          \param datasetName  The dataset where the index will be removed.
          \param idxName      The index to be removed.

          \exception Exception An exception can be thrown, if the index could not be removed from the dataset schema.

          \note Thread-safe!
        */
        virtual void dropIndex(const std::string& datasetName, const std::string& idxName);

        /*!
          \brief It gets the sequence with the given name in the data source.

          \param name  The sequence name.

          \exception Exception An exception can be thrown, if the sequence could not be retrieved from the data source.

          \return The sequence with the given name.

          \note Thread-safe!
        */
        virtual std::auto_ptr<Sequence> getSequence(const std::string& name);

        /*!
          \brief It gets the sequence names available in the data source.

          \note Each sequence in the data source must have a unique name. For example, in a DBMS the name
                may contain the schema name before the sequence name separated by a dot notation (".").

          \exception Exception An exception can be thrown, if the sequence names could not be retrieved.

          \return The sequence names of the data source.

          \note Thread-safe!
        */
        virtual std::vector<std::string> getSequenceNames();

        /*!
          \brief It checks if a sequence with the given name exists in the data source.

          \param name The sequence name.

          \exception Exception An exception can be thrown, if the index existence could not be determined.

          \return True, if the sequence exists in the data source; otherwise, it returns false.

          \note Thread-safe!
        */
        virtual bool sequenceExists(const std::string& name);

        /*!
          \brief It adds a new sequence in the data source.

          \exception Exception An exception can be thrown, if the sequence could not be added to the data source.

          \note Don't delete the given sequence, because the data source will take the ownership of it.
          \note Thread-safe!
        */
        virtual void addSequence(Sequence* sequence);
   
        /*!
          \brief It removes the sequence from the data source.

          \param name The sequence that will be removed.

          \exception Exception An exception can be thrown, if the sequence could not be removed from the data source.

          \note Thread-safe!
        */
        virtual void dropSequence(const std::string& name);

        /*!
          \brief It retrieves the bounding rectangle of the spatial property for the given dataset.

          \param datasetName  The dataset name.
          \param propertyName The spatial property name.

          \exception Exception An exception can be thrown, if the extent of the geometry property could not be retrieved.

          \return The spatial property bounding rectangle, or NULL, if none can be retrieved.

          \note Thread-safe!
        */
        virtual std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                          const std::string& propertyName);

        /*!
          \brief It retrieves the bounding rectangle for the spatial property lying in the given position in the dataset.

          \param datasetName  The dataset name.
          \param propertyPos  The spatial property position.

          \exception Exception An exception can be thrown, if the extent of the geometry property lying in the given position could not be retrieved.

          \return The spatial property bounding rectangle, or NULL, if none can be retrieved.

          \note Thread-safe!
        */
        virtual std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName, std::size_t propertyPos);

        /*!
          \brief It retrieves the number of items of the given dataset.

          \param datasetName  The dataset name.

          \exception Exception An exception can be thrown, if the number of items of the dataset could not be retrieved.

          \return The number of items of the given dataset.

          \note Thread-safe!
        */
        virtual std::size_t getNumberOfItems(const std::string& datasetName);

        /*!
          \brief It checks if the data source has any dataset.

          \exception Exception An exception can be thrown, if it is not possible to check if the data source has datasets .

          \return True, if the data source has datasets; otherwise, it returns false.

          \note Thread-safe!
        */
        virtual bool hasDataSets();

        /*!
          \brief It checks if a dataset with the given name exists in the data source.

          \param name The dataset name.

          \exception Exception An exception can be thrown, if the existence of a dataset in the data source could not be determined.

          \return True, if the dataset exists in the data source; otherwise, it returns false.

          \note Thread-safe!
        */
        virtual bool dataSetExists(const std::string& name);
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

          \note Thread-safe!
        */
        virtual void createDataSet(DataSetType* dt, const std::map<std::string, std::string>& options);

        /*!
          \brief It clones the dataset in the data source.

          \param name       The dataset to be cloned.
          \param cloneName  The name of the cloned dataset.
          \param options    A list of optional modifiers. It is driver specific.

          \exception Exception An exception can be thrown, if the dataset schema could not be cloned.

          \note Thread-safe!
        */
        virtual void cloneDataSet(const std::string& name,
                                  const std::string& cloneName,
                                  const std::map<std::string, std::string>& options);

        /*!
          \brief It removes the dataset schema from the data source.

          \param name The dataset name whose schema will be removed from the data source.

          \exception Exception An exception can be thrown, if the dataset could not be removed from the data source.

          \note Thread-safe!
        */
        virtual void dropDataSet(const std::string& name);

        /*!
          \brief It renames a dataset.

          \param name    The name of the dataset to be renamed.
          \param newName The new dataset name.

          \exception Exception An exception can be thrown, if the dataset could not be renamed.

          \note Thread-safe!
        */
        virtual void renameDataSet(const std::string& name, const std::string& newName);
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

          \note Thread-safe!
        */
        virtual void add(const std::string& datasetName,
                         DataSet* d,
                         const std::map<std::string, std::string>& options,
                         std::size_t limit = 0);

        /*!
          \brief It removes all the informed items from the dataset.

          It removes all the data items from a dataset which are identified by 
          a set of object identifiers. If this set is not informed, all items will be removed.

          \param datasetName The dataset name.
          \param oids        A set of object identifiers used used to remove data from the dataset, or NULL, for removing all.

          \exception Exception An exception can be thrown, if the data items could not be removed.

          \note Thread-safe!
        */
        virtual void remove(const std::string& datasetName, const te::da::ObjectIdSet* oids = 0);

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

          \note Thread-safe!
        */
        virtual void update(const std::string& datasetName,
                            DataSet* dataset,
                            const std::vector<std::size_t>& properties,
                            const te::da::ObjectIdSet* oids,
                            const std::map<std::string, std::string>& options,
                            std::size_t limit = 0);

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

    //@}

        /** @name Data Source static methods
          *  Data Source static methods
        */
        //@{

        /*!
          \brief It creates a new repository for a data source.

          \param dsType The type of data source to be created (example: POSTGIS, ORACLE, SQLITE).
          \param dsInfo The information for creating a new data source.

          \exception Exception An exception can be thrown, if the data source could not be created.

          \return A data source for the new data repository.

          \note Thread-safe!
        */
        static std::auto_ptr<DataSource> create(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        /*!
          \brief It removes a data source identified by its connection information and the driver type.

          \param dsType The data source type name (example: POSTGIS, ORACLE, SQLITE).
          \param dsInfo The connection information for removing the data source.

          \exception Exception An exception can be thrown, if the data source could not be removed.

          \note No other instance of the data source to be removed can be opened, when calling this method.

          \note Thread-safe!
        */
        static void drop(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        /*!
          \brief It checks if the data source exists with the connection information and the driver type.

          \param dsType The data source type name (example: POSTGIS, ORACLE, SQLITE).
          \param dsInfo The data source information.

          \exception Exception An exception can be thrown, if the data source exists in the driver.

          \return True, if the data source exists; otherwise, it returns false.

          \note Thread-safe!
        */
        static bool exists(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        /*!
          \brief It returns the data source names available in the driver.
         
          \param dsType The type name of the data source(example: PostGIS, Oracle, WFS).
          \param dsInfo The information about the data sources.

          \exception Exception An exception can be thrown, if the data source names could not be retrieved.

          \return The data source names available.

          \exception Exception An exception can be thrown, if the list of data source names could not be retrieved.
        */
        static std::vector<std::string> getDataSourceNames(const std::string& dsType, const std::map<std::string, std::string>& info);

        /*!
          \brief It gets the encoding names of the data source.
          
          \param dsType The data source type name (example: PostGIS, Oracle, WFS).
          \param dsInfo The data source information.

          \exception Exception An exception can be thrown, if the encoding names could not be retrieved.

          \return The encoding types of the data source.
        */
        static std::vector<te::common::CharEncoding> getEncodings(const std::string& dsType, const std::map<std::string, std::string>& info);
        //@}

      protected:

        /** @name Protected Data Source Methods????
          * The protected methods of the data source
          */
        //@{

        /*!
          \brief It creates a new data source.

          \param dsInfo The information for creating a new data source.

          \exception Exception An exception can be thrown, if the data source could not be created.

          \note Not thread-safe!
        */
        virtual void create(const std::map<std::string, std::string>& dsInfo) = 0;

        /*!
          \brief It removes the data source with the connection information from a driver.

          \param dsInfo The information for removing a data source from a driver.

          \exception Exception An exception can be thrown, if the data source could not be removed.

          \note Not thread-safe!
        */
        virtual void drop(const std::map<std::string, std::string>& dsInfo) = 0;

        /*!
          \brief Check the existence of a data source in a driver.

          \param dsInfo The data source information.

          \exception Exception An exception can be thrown, if the existence of a data source in a driver could not be determined.

          \return True, if the data source exists, or false, otherwise.

          \note Thread-safe!
        */
        virtual bool exists(const std::map<std::string, std::string>& dsInfo) = 0;

        /*!
          \brief  It gets the data source names available in a driver.

          \param dsInfo The data source information.

          \exception Exception An exception can be thrown, if the data source names could not be retrieved.

          \return The data source names available in the driver.

          \note Not thread-safe!
        */
        virtual std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo) = 0;

        /*!
          \brief It gets the encodings for the data source.
          
          \param dsInfo The data source information.

          \exception Exception An exception can be thrown, if the encoding names could not be retrieved.

          \return The encoding types for the data source.

        */
        virtual std::vector<te::common::CharEncoding> getEncodings(const std::map<std::string, std::string>& dsInfo) = 0;
        //@}

      protected:

        std::string m_id;  //!< The data source identification.
    };

    typedef boost::shared_ptr<DataSource> DataSourcePtr;

  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_DATASOURCE_INTERNAL_DATASOURCE_H
