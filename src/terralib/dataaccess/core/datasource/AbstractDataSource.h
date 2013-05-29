/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/datasource/AbstractDataSource.h

  \brief An abstract class for data providers like a DBMS, Web Services or a regular file.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ABSTRACTDATASOURCE_H
#define __TERRALIB_DATAACCESS_INTERNAL_ABSTRACTDATASOURCE_H

// TerraLib
#include "../../../common/Enums.h"
#include "../../../geometry/Enums.h"
#include "../../Config.h"
#include "../../Exception.h"

// STL
#include <map>
#include <string>
#include <vector>

// Boost
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
    namespace core
    {
      class BatchExecutor;
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

      /*!
        \class AbstractDataSource

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

        A data source can be used in order to access data in a data source
        and it requires a driver implementation that must register a
        concrete factory for building data source objects
        of that type (DataSourceFactory).

        Each data source is characterized by a set of parameters that can be used
        to set up an access channel to its underlying repository. This information
        is referred as the data source connection information.

        A data source exposes the data contained in it as a collection of datasets.
        The information about data stored in a data source may be retrieved.

        Besides the descriptive information about the underlying data repository
        each data source also provides information about their capabilities.

        \sa DataSourceManager, DataSet, DataSetType
      */
      class TEDATAACCESSEXPORT AbstractDataSource : public boost::noncopyable
      {
        public:

          /*! \brief Default constructor just can be called by subclasses. */
          AbstractDataSource() { }

          /*! \brief Virtual destructor. */
          virtual ~AbstractDataSource() { }

          /** @name Basic Methods of a Data Source
           *  Basic Methods for dealing with a data source.
           */
          //@{

          /*!
            \brief It returns the data source type name (in UPPER CASE). Ex: POSTGIS, SQLITE, WFS, WMS, or MYSQL.

            \return The data source type name. Ex: POSTGIS, SQLITE, WFS, WMS, or MYSQL.

            \note Each data source driver must have a unique name.

            \note Thread-safe!
          */
          virtual const std::string& getType() const throw() = 0;

          /*!
            \brief It returns the set of parameters used to set up the access channel to the underlying repository.

            This is the connection information used by a data source in order to enter in an operational mode
            when the open method is called.

            The key-value-pairs (kvp) may contain information about: maximum number of accepted connections,
            user name and password required for establishing a connection, the url of a service or any other
            information needed by the data source to operate. This information is dependent on the data source driver so, please,
            check the driver documentation for any additional information on the kvp.

            \return An associative conteiner (key-value-pair) with information about the data source.

            \note Thread-safe!
          */
          virtual const std::map<std::string, std::string>& getConnectionInfo() const throw() = 0;

          /*!
            \brief It sets the connection information to be used when connecting to the data source.

            \param connInfo Key-value-pairs (kvp) with the connection information.
          */
          virtual void setConnectionInfo(const std::map<std::string, std::string>& connInfo) throw() = 0;

          /*!
            \brief It returns the set of parameters used to set up the access channel to the underlying repository in a string format.

            This is the connection information used by a data source in order to enter in an operational mode
            when the open method is called.

            The string may contain key-value-pairs (kvp) with information about: maximum number of accepted connections,
            user name and password required for establishing a connection, the url of a service or any other
            information needed by the data source to operate. This information is dependent on the data source driver so, please,
            check the driver documentation for any additional information on the kvp.

            The key-value pairs are separated by an equal sign and each pair
            is separated by the ampersand ('&'). The values present in a kvp must
            be URL encoded.  

            \return An URL encoded string with key-value-pairs that contains information about the data source.

            \note See the following sites for more information on the syntax:
                  <ul>
                  <li>http://www.terralib.org/datasourcedrivers</li>
                  <li>http://en.wikipedia.org/wiki/Query_string</li>
                  <li>http://www.ietf.org/rfc/rfc3986.txt</li>
                  <li>http://www.ietf.org/rfc/rfc1738.txt</li>
                  </ul>

            \note Thread-safe!
          */
          virtual std::string getConnectionStr() const throw();

          /*!
            \brief It sets the connection information to be used when connecting to the data source.

            \param connInfo An URL encoded string with key-value-pairs that contains information about the data source.
          */
          virtual void setConnectionStr(const std::string& connInfo) throw();

          /*!
            \brief It opens the data source and makes it ready for use.

            If the subclass needs to open a connection to a database server, 
            or to open a file or to get information from a Web Service, this
            method can do this kind of job in order to prepare the data source to be
            in an operational mode. It will use the connection information provided by the methods setConnectionInfo.

            \note This method doesn't load the data source catalog.

            \note Not thread-safe!
          */
          virtual void open() throw(Exception) = 0;

          /*!
            \brief It opens the data source and makes it ready for use.

            If the subclass needs to open a connection to a database server, 
            or to open a file or to get information from a Web Service, this
            method can do this kind of job in order to prepare the data source to be
            in an operational mode.

            \param connInfo The set of parameters used to set up the underlying access channel to the repository.

            \note This method doesn't load the data source catalog.

            \note Not thread-safe!
          */
          virtual void open(const std::map<std::string, std::string>& connInfo) throw(Exception);

          /*!
            \brief It opens the data source and makes it ready for use.

            If the subclass needs to open a connection to a database server, 
            or to open a file or to get information from a Web Service, this
            method can do this kind of job in order to prepare the data source to be
            in an operational mode.

            \param connInfo The set of parameters used to set up the underlying access channel to the repository (URL encoded).

            \note This method doesn't load the data source catalog.

            \note Not thread-safe!
          */
          virtual void open(const std::string& connInfo) throw(Exception);

          /*!
            \brief It closes the data source and clears all the resources used by its internal communication channel.

            This method closes any connection, closes any opened file
            or releases any other resources.

            \note This method doesn't clear the data source catalog.

            \note Not thread-safe!
          */
          virtual void close() throw(Exception) = 0;

          /*!
            \brief It returns true if the data source is opened, otherwise it returns false.

            This method will not check if the data source is available for use,
            it will just answer if the data source has been already opened.
            If you want to know if the data source is available for use,
            check the isValid() method.

            \return It returns true if the data source is opened, otherwise it returns false.

            \note Not thread-safe!
          */
          virtual bool isOpened() const throw() = 0;

          /*!
            \brief It checks if the data source is valid (available for use).

            For a DBMS, this will check the opened connections.
            For a WFS client, this will check if the server is reachable.
            For a file, this will check if it can be read.

            \return It returns true if the data source is available, otherwise it returns false.

            \note Not thread-safe!
          */
          virtual bool isValid() const throw() = 0;

          /*!
            \brief It returns the known capabilities of the data source.
          
            The returned object has all informatin about what the data source can perform.
            Here you will find if the data source implementation supports primary keys,
            foreign keys, if it can be used in a thread environment and much more information.

            \param capabilities The known capabilities of the data source.

            \note Thread-safe!
          */
          virtual const DataSourceCapabilities& getCapabilities() const throw() = 0;

          /*!
            \brief It returns the data source SQL dialect if one exists.

            \return The data source SQL dialect.
          */
          virtual const SQLDialect* getDialect() const throw() = 0;

          /*!
            \brief For some data access drivers this method will perform some operations to optmize the data storage.

            This is can be a tipical maintenance command in database systems (like vacuum).

            \param opInfo Any information needed by the underlying driver in order to fine tune the optimization.

            \note Not thread-safe!

            \note For some systems you must assure that no other thread are executing commands against the data source.

            \note For some drivers this method doesn't have effect.
          */
          virtual void optimize(const std::map<std::string, std::string>& opInfo) throw(Exception) = 0;

          /*!
            \brief It returns a Encodings names string vector.
          
            \param dsType The data source type name (example: PostGIS, Oracle, WFS).
            \param dsInfo The data source information.

            \return Encodings names string vector.

            \exception Exception It throws an exception if the data source can not be consulted.
          */
          virtual std::vector<std::string> getEncodings() throw(Exception) = 0;

          //@}

          /** @name Transaction
           *  Methods for dealing with transactions.
           */
          //@{

          /*!
            \brief It starts a new transaction.

            \note Not thread-safe!
          */
          virtual void begin() throw(Exception) = 0;

          /*!
            \brief It commits the transaction.

            After commiting or rolling back, you can start another transaction.

            \note Not thread-safe!
          */
          virtual void commit() throw(Exception) = 0;

          /*!
            \brief It aborts the transaction. Any changes will be rolled-back.

            After commiting or rolling back, you can start another transaction.

            \note Not thread-safe!
          */
          virtual void rollBack() throw(Exception) = 0;

          /*!
            \brief It returns true if a transaction is in progress, otherwise, false.

            \return True if a transaction is in progress, otherwise, false.
          */
          virtual bool isInTransaction() const throw() = 0;

          //@}

          /** @name DataSet Retrieval
           *  Methods for retrieving data from the DataSource.
           */
          //@{

          /*!
            \brief It gets the dataset identified by the given name.

            \param name     The name of the dataset. It must be the same name as the DataSetType name in the DataSource catalog.
            \param travType The traverse type associated to the returned dataset. 
            \param rwRole   The read and write permission associated to the returned dataset. 

            \return The caller of this method will take the ownership of the returned dataset.

            \note Not thread-safe!
          */
          virtual DataSet* getDataSet(const std::string& name, 
                                      te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception) = 0;

          /*!
            \brief It gets the dataset identified by the given name using a spatial filter over the default geometric property.

            \param name     The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
            \param e        A rectangle to be used as a spatial filter when retrieving datasets.
            \param r        The spatial relation to be used during the filter.
            \param travType The traverse type associated to the returned dataset. 
            \param rwRole   The read and write permission associated to the returned dataset. 

            \return The caller of this method will take the ownership of the returned DataSet.

            \note Transactor will not take the ownership of the given envelope.

            \note The envelope coordinates should be in the same coordinate system as the dataset.

            \note This first attempt of this method will be to retrieve the default geometric column in
                  the data source catalog. If it can not retrieve that information it may implicitly
                  use the data source catalog loader to query the data source and retrieve the geometric
                  property information.

            \note Not thread-safe!
          */
          virtual DataSet* getDataSet(const std::string& name,
                                      const te::gm::Envelope* e,
                                      te::gm::SpatialRelation r,
                                      te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);

          /*!
            \brief It gets the dataset identified by the given name using a spatial filter over the specified property.

            \param name     The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
            \param p        The geometric property in order to apply the spatial filter.
            \param e        A rectangle to be used as a spatial filter when retrieving datasets.
            \param r        The spatial relation to be used during the filter.
            \param travType The traverse type associated to the returned dataset. 
            \param rwRole   The read and write permission associated to the returned dataset. 

            \return The caller of this method will take the ownership of the returned DataSet.

            \note Transactor will not take the ownership of the given envelope.

            \note The envelope coordinates should be in the same coordinate system as the dataset.

            \note Not thread-safe!
          */
          virtual DataSet* getDataSet(const std::string& name,
                                      const te::dt::Property* p,
                                      const te::gm::Envelope* e,
                                      te::gm::SpatialRelation r,
                                      te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception) = 0;

          /*!
            \brief It gets the dataset identified by the given name using a spatial filter over the default geometric property.

            \param name     The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
            \param g        A geometry to be used as a spatial filter when retrieving datasets.
            \param r        The spatial relation to be used during the filter.
            \param travType The traverse type associated to the returned dataset. 
            \param rwRole   The read and write permission associated to the returned dataset. 

            \return The caller of this method will take the ownership of the returned DataSet.

            \note Transactor will not take the ownership of the given geometry.

            \note The geometry coordinates should be in the same coordinate system as the dataset.

            \note This first attempt of this method will be to retrieve the default geometric column in
                  the data source catalog. If it can not retrieve that information it may implicitly
                  use the data source catalog loader to query the data source and retrieve the geometric
                  property information.

            \note Not thread-safe!
          */
          virtual DataSet* getDataSet(const std::string& name,
                                      const te::gm::Geometry* g,
                                      te::gm::SpatialRelation r,
                                      te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);

          /*!
            \brief It gets the dataset identified by the given name using a spatial filter over the given geometric property.

            \param name     The name of the DataSetType. It must be the same name as the DataSetType name in the DataSource catalog.
            \param p        The geometric property in order to apply the spatial filter.
            \param g        A geometry to be used as a spatial filter when retrieving datasets.
            \param r        The spatial relation to be used during the filter.
            \param travType The traverse type associated to the returned dataset. 
            \param rwRole   The read and write permission associated to the returned dataset.

            \return The caller of this method will take the ownership of the returned DataSet.

            \note Transactor will not take the ownership of the given geometry.

            \note The geometry coordinates should be in the same coordinate system as the dataset.
          
            \note Not thread-safe!
          */
          virtual DataSet* getDataSet(const std::string& name,
                                      const te::dt::Property* p,
                                      const te::gm::Geometry* g,
                                      te::gm::SpatialRelation r,
                                      te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception) = 0;

          /*
           \brief It gets the dataset identified by the given name using the set of objects identification.

           \param name     The name of the dataset. It must be the same name as the DataSetType name in the DataSource catalog.
           \param oids     A pointer to a set of objects identification. Do not pass null. Do not pass set empty.
           \param travType The traverse type associated to the returned dataset.
           \param rwRole   The read and write permission associated to the returned dataset.

           \return The caller of this method will take the ownership of the returned DataSet.
          */
          virtual DataSet* getDataSet(const std::string& name,
                                      const ObjectIdSet* oids, 
                                      te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception);

          /*!
            \brief It executes a query that may return some data using a generic query.

            Different from the method that accepts a dataset name and
            a spatial filter, this method allows for retrieving only a
            subset of the attributes since a query can include
            a property list.

            \param q        A valid query object.
            \param travType The traverse type associated to the returned dataset. 
            \param rwRole   The read and write permission associated to the returned dataset. 

            \return The caller of this method will take the ownership of the returned dataset.

            \note Not thread-safe!
          */
          virtual DataSet* query(const Select& q,
                                 te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception) = 0;

          /*!
            \brief It executes a query that may returns some data using the data source native language.

            \param query    A query string in the data source native language.
            \param travType The traverse type associated to the returned dataset. 
            \param rwRole   The read and write permission associated to the returned dataset. 

            \return The caller of this method will take the ownership of the returned data set.

            \note Avoid using this method if you want portability for your application.

            \note Not thread-safe!
          */
          virtual DataSet* query(const std::string& query, 
                                 te::common::TraverseType travType = te::common::FORWARDONLY) throw(Exception) = 0;
          //@}

          /** @name Execution Methods
           *  Methods for executing operations against the data source.
           */
          //@{

          /*!
            \brief It executes the specifed command using a generic query representation.

            \param command A query like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE.

            \exception Exception It throws an exception if the query can not be performed.

            \note Not thread-safe!
          */
          virtual void execute(const Query& command) throw(Exception) = 0;

          /*!
            \brief It executes the specifed command in the data source native language.

            \param command A query string in the data source native language (like: CREATE, DROP, ALTER, INSERT, UPDATE, DELETE).

            \exception Exception It throws an exception if the query can not be performed.

            \note Not thread-safe!
          */
          virtual void execute(const std::string& command) throw(Exception) = 0;

          /*!
            \brief It requests that the data source stop the processing of the current command.

            \exception Exception It may throw an exception if it is not possible to cancel the command execution.
          */
          virtual void cancel() throw(Exception) = 0;

          /*!
            \brief It returns the last id generated by an insertion command.

            \return The last id generated by an insertion command.

            \note This is the way to deal with auto-increment values.
          */
          virtual boost::int64_t getLastGeneratedId() throw(Exception) = 0;

          //@}

          /** @name DataSet Metadata Retrieval
           *  Methods for retrieving metadata from the data source.
           */
          //@{

          /*!
            \brief It searches for the list of datasets available in the data source.

            \param datasets A vector of string pointers to output the names of the datasets available in the data source.

            \note Each dataset in the data source must have a unique name. For example, in a DBMS the name
                  may contains the schema name before the table name separated by a dot notation (".").

            \note The caller of this method will take the ownership of the strings in the output vector.
          */
          virtual std::vector<std::string> getDataSetNames() throw(Exception) = 0;

          /*!
            \brief It searches for information about a given dataset in the data source.

            Using this method you can get more information about a dataset:
            <ul>
            <li>the list of properties, including: name, data type, size, if the value is required or not, if it is an autoincrement</li>
            <li>primary key</li>
            <li>foreign keys</li>
            <li>unique keys</li>
            <li>check constraints</li>
            <li>indexes</li>
            </ul>

            \param datasetName The name of the dataset we are looking information for.

            \return The dataset schema. The caller of this smethod will take the ownership of the returned pointer.

            \note No sequence information is retrieved for autoincrement properties.
            \note Even if parameter <i>full</i> is true the bounding box of the dataset will not be loaded and
                  if you want that information you should call getExtent method.
          */
          virtual DataSetType* getDataSetType(const std::string& name) throw(Exception) = 0;

          virtual std::size_t getNumberOfProperties(const std::string& datasetName) throw(Exception) = 0;

          /*!
            \brief It retrieves the properties of the dataset.

            \param dt The dataset we are looking for the properties.

            \post The dataset will contain information only about properties (no pk, no uk nor check-constraints).
          */
          virtual std::vector<te::dt::Property*> getProperties(const std::string& datasetName) throw(Exception) = 0;

          /*!
            \brief It retrieves a property from a dataset.

            \param datasetName  The name of a dataset.
            \param propertyName The name of the property.

            \return The property information. The caller will take the property ownership.
          */
          virtual te::dt::Property* getProperty(const std::string& datasetName, const std::string& propertyName) throw(Exception) = 0;

          virtual te::dt::Property* getProperty(const std::string& datasetName, std::size_t propertyPos) throw(Exception) = 0;

          /*!
            \brief It retrieves the dataset's primary key.

            \param dt The dataset we are looking for the primary key information.

            \exception Exception It throws an exception if something goes wrong during data source search.

            \post The primary key may not have an associated index.
            \post The found primary key is stored in the dataset type.
          */
          virtual PrimaryKey* getPrimaryKey(const std::string& datasetName) throw(Exception) = 0;

          /*!
            \brief It searches in the data source for unique keys associated to the given dataset.

            \param dt  The dataset we are looking unique keys information for.

            \exception Exception It throws an exception if something goes wrong during data source search.

            \pre The dataset must be retrieved by getDataSetType or loadDataSetType before calling this method.

            \post Unique keys may not have an associated index.
            \post Any found unique keys will be added to the dataset type.
          */
          virtual std::vector<std::string> getUniqueKeyNames(const std::string& datasetName) throw(Exception) = 0;

          /*!
            \brief It searches in the data source for unique keys associated to the given dataset.

            \param dt  The dataset we are looking unique keys information for.

            \exception Exception It throws an exception if something goes wrong during data source search.

            \pre The dataset must be retrieved by getDataSetType or loadDataSetType before calling this method.

            \post Unique keys may not have an associated index.
            \post Any found unique keys will be added to the dataset type.
          */
          virtual std::vector<UniqueKey*> getUniqueKeys(const std::string& datasetName) throw(Exception) = 0;

          virtual UniqueKey* getUniqueKey(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It searches for foreign keys defined over the informed dataset dt.

            \param dt      The dataset we are looking foreign keys information for.
            \param fkNames The list of foreign key names associated to dt.
            \param rdts    The list of related tables to dt. 

            \exception Exception It throws an exception if something goes wrong during data source search.
          */
          virtual std::vector<std::string> getForeignKeyNames(const std::string& datasetName) throw(Exception) = 0;

          /*!
            \brief It retrieves the foreign key defined over dt that is related to rdt.

            \param fkName The foreign key name relating dt to rdt.
            \param dt     The dataset that has the fk.
            \param rdt    The dataset related to dt (probably the one with a primary key).

            \return The foreign key relating the two datasets or NULL if none exists. The caller will take the ownership of the returned fk.

            \exception Exception It throws an exception if something goes wrong during data source search.
          */
          virtual ForeignKey* getForeignKey(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It searches in the data source for indexes associated to the given dataset.

            \param dt  The dataset we are looking indexes information for.

            \exception Exception It throws an exception if something goes wrong during data source search.

            \post The indexes will have a dataset owner.
            \post Any found indexes will be added to the dataset type.
          */
          virtual std::vector<std::string> getIndexNames(const std::string& datasetName) throw(Exception) = 0;

          virtual Index* getIndex(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It searches in the data source for check constraints associated to the given dataset.

            \param dt  The dataset we are looking checking-constraints information for.

            \exception Exception It throws an exception if something goes wrong during data source search.
           */
          virtual std::vector<std::string> getCheckConstraintNames(const std::string& datasetName) throw(Exception) = 0;

          virtual CheckConstraint* getCheckConstraint(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It searches for the list of sequences available in the data source.

            \param sequences A vector of string pointers to output the names of the sequences available in the data source.

            \exception Exception It throws an exception if something goes wrong during data source search.

            \note Each sequence in the data source must have a unique name. For example, in a DBMS the name
                  may contains the schema name before the sequnce name separated by a dot notation (".").

            \note The caller of this method will take the ownership of the strings in the output vector.
          */
          virtual std::vector<std::string> getSequenceNames() throw(Exception) = 0;

          /*!
            \brief It searches information about the given sequence in the data source.

            \param seqName  The sequence we are looking information for.

            \return The sequence with the given name. The caller will take its ownership.

            \exception Exception It throws an exception if something goes wrong during data source search.
          */
          virtual Sequence* getSequence(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It retrieves the bounding rectangle for the given spatial property.

            \param sp The spatial property.

            \return The spatial property bounding rectangle or NULL if none is found.

            \pre The spatial property must be associated to a dataset type.

            \post The caller of this method will take the ownership of the returned box.

            \exception Exception It throws an exception if something goes wrong during MBR search.
          */
          virtual te::gm::Envelope* getExtent(const std::string& datasetName, const std::string& propertyName) throw(Exception) = 0;

          virtual te::gm::Envelope* getExtent(const std::string& datasetName, std::size_t propertyPos) throw(Exception) = 0;

          virtual std::size_t getNumberOfItems(const std::string& datasetName) throw(Exception) = 0;

          /*!
            \brief It return true if the data source has any dataset.

            \return True if the data source has datasets otherwise, false.
          */
          virtual bool hasDataSets() throw(Exception) = 0;

          /*!
            \brief It checks if a dataset with the given name already exists in the data source.

            \param name The name of the dataset to check.

            \return True if the dataset exists in the data source otherwise false.

            \exception Exception It throws an exception if something goes wrong.
           */
          virtual bool datasetExists(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It checks if a primary key with the given name already exists in the data source.

            \param name The name of the primary key to check.

            \return True if the primary key exists in the data source otherwise false.

            \exception Exception It throws an exception if something goes wrong.
          */
          virtual bool primarykeyExists(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It checks if a unique key with the given name already exists in the data source.

            \param name The name of the primary key to check.

            \return True if the unique key exists in the data source otherwise false.

            \exception Exception It throws an exception if something goes wrong.
          */
          virtual bool uniquekeyExists(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It checks if a foreign key with the given name already exists in the data source.

            \param name The name of the foreign key to check.

            \return True if the foreign key exists in the data source otherwise false.

            \exception Exception It throws an exception if something goes wrong.
          */
          virtual bool foreignkeyExists(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It checks if a check-constraint with the given name already exists in the data source.

            \param name The name of the check-constraint to check.

            \return True if the check-constraint exists in the data source otherwise false.

            \exception Exception It throws an exception if something goes wrong.
          */
          virtual bool checkConstraintExists(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It checks if an index with the given name already exists in the data source.

            \param name The name of the index to check.

            \return True if the index exists in the data source otherwise false.

            \exception Exception It throws an exception if something goes wrong.
          */
          virtual bool indexExists(const std::string& name) throw(Exception) = 0;

          /*!
            \brief It checks if a sequence with the given name already exists in the data source.

            \param name The name of the sequence to check.

            \return True if the sequence exists in the data source otherwise false.

            \exception Exception It throws an exception if something goes wrong.
          */
          virtual bool sequenceExists(const std::string& name) throw(Exception) = 0;

          //@}
      };

      typedef boost::shared_ptr<AbstractDataSource> AbstractDataSourcePtr;

    }  // end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ABSTRACTDATASOURCE_H

