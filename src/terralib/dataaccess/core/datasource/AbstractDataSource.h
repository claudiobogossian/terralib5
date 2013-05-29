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

          /** @name DataSetType Persistence Methods
           *  Methods that subclasses must implement in order to support the persistence interface.
           */
          //@{

          /*!
            \brief It creates the DataSetType definition in the target data source.

            The informed DataSetType will be created in the target data source.

            If a dataset type with the same name already exists in the target data source this may
            throws an exception.

            After calling this method, the dataset type will receive a new identification. If
            it is associated to a catalog, the method will automatically adjust its entry in the catalog.

            If the dataset type is not associated to the catalog, the method does NOT add it to the target data source catalog.

            If you want to create a DataSetType in a data source with the same data
            of an existing DataSetType from another data source, first of all,
            you must create a fresh copy of the DataSetType (see the method clone()
            in the DataSetType class).

            \param dt      The DataSetType to be created. It may be changed if the underlying data source
                           creates new dataset type itens or if it doesn't have support for some of them.

            \pre All parameters must be valid pointers.
            \pre The DataSetTypes referenced by foreign keys must be already in the data source catalog.

            \post In some data sources this method may outputs implicit indexes, sequences or constraints.
                  The method, if necessary, will create and adjust the dataset type.

            \exception Exception It throws an exception if the DataSetType cannot be created.
          */
          virtual void create(DataSetType* dt) throw(Exception);

          /*!
            \brief It creates the DataSetType definition in the target data source.

            The informed DataSetType will be created in the target data source.

            If a dataset type with the same name already exists in the target data source this may
            throws an exception.

            After calling this method, the dataset type will receive a new identification. If
            it is associated to a catalog, the method will automatically adjust its entry in the catalog.

            If the dataset type is not associated to the catalog, the method does NOT add it to the target data source catalog.

            If you want to create a DataSetType in a data source with the same data
            of an existing DataSetType from another data source, first of all,
            you must create a fresh copy of the DataSetType (see the method clone()
            in the DataSetType class).

            \param dt      The DataSetType to be created. It may be changed if the underlying data source
                           creates new dataset type itens or if it doesn't have support for some of them.
            \param options A list of optional modifiers. It is driver specific.

            \pre All parameters must be valid pointers.
            \pre The DataSetTypes referenced by foreign keys must be already in the data source catalog.

            \post In some data sources this method may outputs implicit indexes, sequences or constraints.
                  The method, if necessary, will create and adjust the dataset type.

            \exception Exception It throws an exception if the DataSetType cannot be created.
          */
          virtual void create(DataSetType* dt, const std::map<std::string, std::string>& options) throw(Exception) = 0;

          /*!
            \brief It clones the dataset in the data source.

            \param datasetName    The dataset to be cloned.
            \param newDataSetName The name of the cloned dataset.
            \param options        A list of optional modifiers. It is driver specific.
          */
          virtual void clone(const std::string& datasetName,
                             const std::string& newDatasetName,
                             const std::map<std::string, std::string>& options) throw(Exception) = 0;

          /*!
            \brief It drops the DataSetType from the data source.

            The DataSetType will be removed from the data source associated to it.

            If the DataSetType is associated to a DataSourceCatalog,
            this method will automatically remove it from the catalog. In this case,
            this method can propagate changes to the catalog, for example, removing associated 
            sequences and foreign keys. Otherwise, the data source catalog is not updated.

            \param dt The DataSetType to be removed from data source.

            \post If the DataSetType is associated to a data source catalog, it will automatically remove 
                  the given DataSetType from the data source catalog. Otherwise, the data source catalog 
                  is not updated.
            \post The given DataSetType pointer will be invalidated as soon as the DataSetType is
                  removed from its data source. 
            \post If the DataSetType is associated to a data source catalog, this may cause changes 
                  propagation through the data source catalog objects: changing in sequences, foreign
                  keys and other stuffs. 

            \exception Exception It throws an exception if the DataSetType can not be removed.
          */
          virtual void drop(const std::string& datasetName) throw(Exception) = 0;

          /*!
            \brief It renames the DataSetType.

            The DataSetType will be renamed in the data source associated to it.

            If the DataSetType is associated to a data source catalog, this method can cause changes 
            in the catalog.

            \param dt      The DataSetType to be renamed.
            \param newName The new DataSetType name.

            \pre The DataSetType must be a valid pointer.

            \post If the DataSetType is associated to a data source catalog, 
                  it will be renamed in the associate data source catalog. In this case, 
                  this method can cause a cascade propagation in the data source catalog.
                  Otherwise, the data source catalog will be not updated.
            \post The given DataSetType pointer will receive the new name.
          
            \exception Exception It throws an exception if the DataSetType can not be renamed.
          */
          virtual void rename(DataSetType* dt, const std::string& newName) throw(Exception) = 0;
   
          /*!
            \brief It adds a new property to the DataSetType.

            It will create the given property in the DataSetType data source and associate
            it to the DataSetType.          

            If you want to add a property in a DataSetType with the same data
            of an existing one from another DataSetType, first of all,
            you must create a fresh copy of the property (see the method clone()
            in the Property class). After that, the property will be added to the
            DataSetType in memory.

            \param dt The DataSetType where the Property will be added.
            \param p  The new Property to be added.

            \pre The parameters must be valid pointers.

            \post This method can cause a cascade propagation in the data source catalog. 

            \exception Exception It throws an exception if the Property can not add to the DataSetType.
          */
          virtual void add(DataSetType* dt, te::dt::Property* p) throw(Exception) = 0;

          /*!
            \brief It removes a property.

            This method will also remove every object
            associated to the property, such as, unique key, primary
            key and foreign key.

            The property must be associated to a DataSetType before calling this method.

            \param p The property to be removed from the DataSetType.

            \pre The parameters must be valid pointers.

            \post After being removed, the property pointer will be invalidaded.
            \post This method can cause a cascade propagation in the data source catalog and in the DataSetType: changing in constraints and other stuffs.

            \exception Exception It throws an exception if the property can not be removed.
            \exception Exception It throws an exception if the property is not associated to a DataSetType.
          */
          virtual void drop(te::dt::Property* p) throw(Exception) = 0;

          /*!
            \brief It renames the property.

            In order to rename a property, it must belong to a DataSetType.
            If the operation is successful, 
            the property in memory will have its name automatically changed in the dataset type.

            \param p       The property to be renamed.
            \param newName The new property name.

            \pre The property must be a valid pointer. 

            \post This method can cause a cascade propagation in the DataSetType and associate objects: changing in constraints and other stuffs.

            \exception Exception It throws an exception if the property can not be renamed.
            \exception Exception It throws an exception if the property is not associated to a DataSetType.
          */
          virtual void rename(te::dt::Property* p, const std::string& newName) throw(Exception) = 0;

          /*!
            \brief It updates a property.

            It will update an old property based on a new property information.
            Therefore, the old property must belong to a valid DataSetType and 
            its DataSetType must belong to a data source. The old property will become
            invalid and the new one will take its place in the associated DataSetType.

            \param oldP The property to be changed.
            \param newP The property with the new information.

            \exception Exception It throws an exception if the DataSetType cannot be updated.
            \exception Exception It throws an exception if the property is not associated to a DataSetType.

            \note The parameters must be valid pointers. 

            \note The newP must be not associated to another object before calling this method.

            \note If the operation was successful, the new property will be associated to the same DataSetType
                  of the oldP, and the oldP pointer will be invalidated.

            \warning This method can cause a cascade propagation in the data source catalog: changing in constraints and other stuffs.
          */
          virtual void update(te::dt::Property* oldP, te::dt::Property* newP) throw(Exception) = 0;
        
          /*!
            \brief It adds a primary key constraint in the DataSetType.

            The DataSetType must be associated to a data source.
            After adding the given primary key in the data source, 
            it will be associated to the DataSetType in memory.

            \param dt  The DataSetType where the primary key will be added.
            \param pk  The primary key constraint.

            \pre The parameters must be valid pointers.           
            \pre The property that will be updated to primary key must already exist in data source. 
          
            \post This method can cause a cascade propagation in the DataSetType indexes.

            \exception Exception It throws an exception if the primary key cannot be added to the DataSetType.
          */
          virtual void add(DataSetType* dt, PrimaryKey* pk) throw(Exception) = 0;
        
          /*!
            \brief It removes the primary key constraint from the DataSetType which it is associated.

            The given primary key constraint must belong to a DataSetType and its DataSetType must be associated
            to a data souce. After removing the given primary key from the data source, 
            it will be removed from its DataSetType in memory.

            \param pk The primary key constraint that will be removed.

            \pre The primary key must be a valid pointer. 

            \post After being removed, the primary key pointer will be invalidaded.
            \post This method can cause a cascade propagation in the DataSetType indexes.

            \exception Exception It throws an exception if the primary key cannot be removed from its DataSetType.
          */
          virtual void drop(PrimaryKey* pk) throw(Exception) = 0;
        
          /*!
            \brief It adds a unique key constraint to the DataSetType.

            The DataSetType must be associated to a data source.
            After adding the given unique key in the data source, 
            it will be associated to the DataSetType in memory.

            \param dt The DataSetType where the unique key will be added.
            \param uk The unique key constraint.

            \pre The parameters must be valid pointers.

            \post This method can cause a cascade propagation in the DataSetType indexes.

            \exception Exception It throws an exception if the unique key cannot be added to the DataSetType.
          */
          virtual void add(DataSetType* dt, UniqueKey* uk) throw(Exception) = 0; 
        
          /*!
            \brief It removes the unique key constraint from the DataSetType which it is associated.

            The given unique key constraint must belong to a DataSetType and its DataSetType must be associated
            to a data souce. After removing the given unique key from the data source, 
            it will be removed from its DataSetType in memory.

            \param uk The unique key constraint.

            \post After being removed, the unique key pointer will be invalidaded.
            \post This method can cause a cascade propagation in the DataSetType indexes.

            \pre The parameter must be a valid pointer. 

            \exception Exception It throws an exception if the unique key cannot be removed from its DataSetType.
          */
          virtual void drop(UniqueKey* uk) throw(Exception) = 0;

          /*!
            \brief It adds an index in a DataSetType.

            The DataSetType must be associated to a data source, and the given index
            must not belong to any DataSetType. After adding the given index in the data source, 
            it will be associated to the DataSetType in memory.

            \param dt       The DataSetType where the index will be added.
            \param index    The index to be added.

            \pre The parameters must be valid pointers. 

            \post It will automatically attach the index to the DataSetType.

            \exception Exception It throws an exception if the index cannot be added to the DataSetType.
          */
          virtual void add(DataSetType* dt, Index* index) throw(Exception);
        
          /*!
            \brief It adds an index in a DataSetType.

            The DataSetType must be associated to a data source, and the given index
            must not belong to any DataSetType. After adding the given index in the data source, 
            it will be associated to the DataSetType in memory.

            \param dt       The DataSetType where the index will be added.
            \param index    The index to be added.
            \param options  A list of optional modifiers. It is driver specific.

            \pre The parameters must be valid pointers. 

            \post It will automatically attach the index to the DataSetType.

            \exception Exception It throws an exception if the index cannot be added to the DataSetType.
          */
          virtual void add(DataSetType* dt, Index* index, const std::map<std::string, std::string>& options) throw(Exception) = 0; 
        
          /*!
            \brief It removes the index from the DataSetType which it is associated.

            The given index must belong to a DataSetType and its DataSetType must be associated
            to a data souce. After removing the index from the data source, 
            it will be removed from its DataSetType in memory.

            \param index The index associated to a DataSetType.

            \pre The index must be a valid pointer. 

            \post After being removed, the index pointer will be invalidaded.
            \post The changes can propagate to associated primary key or unique key.

            \exception Exception It throws an exception if the index cannot be removed from its DataSetType.
          */
          virtual void drop(Index* index) throw(Exception) = 0;

          /*!
            \brief It adds a foreign key constraint to a DataSetType.

            After adding the given foreign key in the data source, 
            it will be associated to the DataSetType in memory. 

            \param dt  The DataSetType where the foreign key will be added.
            \param fk      The foreign key constraint.

            \pre The parameters must be valid pointers. 

            \post It will automatically attach the fk to the DataSetType.
            \post This may propagate changes to the DataSourceCatalog associated to the persistence.

            \exception Exception It throws an exception if the foreign key cannot be added to the DataSetType.
          */
          virtual void add(DataSetType* dt, ForeignKey* fk) throw(Exception) = 0;

          /*!
            \brief It removes the foreign key constraint from the DataSetType which it is associated.

            The given foreign key constraint must belong to a DataSetType and its DataSetType must be associated
            to a data souce. After removing the foreign key constraint from the data source, 
            it will be removed from its DataSetType in memory.

            \param fk The foreign key constraint that will be removed.

            \pre The parameter must be a valid pointer. 

            \post It will automatically remove the fk from the DataSetType.
            \post After being removed, the foreign key pointer will be invalidaded.
            \post This may propagate changes to the DataSourceCatalog associated to the persistence.

            \exception Exception It throws an exception if the foreign key cannot be removed from its DataSetType.          
          */
          virtual void drop(ForeignKey* fk) throw(Exception) = 0;

          /*!
            \brief It adds a check constraint to the DataSetType.

            The DataSetType must be associated to a data source.
            After adding the given check constraint in the data source, 
            it will be associated to the DataSetType in memory.

            \param dt The DataSetType where the check constraint will be added.
            \param cc The check constraint.

            \pre The parameters must be valid pointers. 

            \post It will automatically attach the cc to the DataSetType.

            \exception Exception It throws an exception if the check constraint cannot be added to the DataSetType.
          */
          virtual void add(DataSetType* dt, CheckConstraint* cc) throw(Exception) = 0;
   
          /*!
            \brief It removes the check constraint from the DataSetType which it is associated..

            The given check constraint must belong to a DataSetType and its DataSetType must be associated
            to a data souce. After removing the check constraint from the data source, 
            it will be removed from its DataSetType in memory.
          
            The given check constraint may belong to a DataSetType. 

            \param cc The check constraint that will be removed.

            \exception Exception It throws an exception if the check constraint cannot be removed from its DataSetType.

            \note The check constraint must be a valid pointer. 

            \note After being removed, the check constraint pointer will be invalidaded.
          */
          virtual void drop(CheckConstraint* cc) throw(Exception) = 0;
   
          /*!
            \brief It creates a new sequence in the data source.

            After calling this method, the given sequence will receive a new identification. If
            it is associated to a data source catalog, the method will automatically 
            adjust its entry in the catalog.
          
            \param sequence The sequence that will be created in the data source.

            \pre The parameters must be valid pointers.

            \exception Exception It throws an exception if the sequence cannot be created to the data source.
          */
          virtual void create(Sequence* sequence) throw(Exception) = 0;
   
          /*!
            \brief It removes the sequence from the data source.
          
            If the sequence is associated to a data source catalog, it will
            be removed from the catalog. 

            \param sequence The sequence that will be removed.

            \pre The parameter must be a valid pointer. 

            \post After being removed, the sequence pointer will be invalidaded.
            \post If it is associated to a data source catalog, this may propagate changes to it.

            \exception Exception It throws an exception if the sequence cannot be removed from the data source.
          */
          virtual void drop(Sequence* sequence) throw(Exception) = 0;

          //@}

          /** @name DataSet Persistence Methods
           *  Methods that subclasses must implement in order to support the persistence interface.
           */
          //@{

          /*!
            \brief It removes all the informed items from the dataset.

            It removes all data collection identified by an 
            object identifier from the data source. If no OIDs are
            informed all data will be removed.

            \param datasetName The dataset name.
            \param oids        A list of object identifiers used to remove data from the datasource or NULL for all.

            \exception Exception It throws an exception if the dataset items can not be removed.
          */
          virtual void remove(const std::string& datasetName, const ObjectIdSet* oids = 0) throw(Exception) = 0;

          /*!
            \brief It adds more data items to the dataset in the data source.

            \param datasetName The target dataset name.
            \param d           The data items to be added to the dataset.
            \param limit       The number of items to be used from the input dataset. If set to 0 (default) all items are used.
          
            \pre All parameters must be valid pointers.

            \exception Exception It throws an exception if the data items can not be added.

            \note It is the caller responsability to release the dataset 'd' pointer.

            \note DataSetPersistence will start reading the dataset 'd' in the
                  current position. So, keep in mind that it is the caller responsability
                  to inform the dataset 'd' in the right position (and a valid one) to start processing it.
          */
          virtual void add(const std::string& datasetName,
                           DataSet* d,
                           std::size_t limit = 0) throw(Exception);

          /*!
            \brief It adds more data items to the dataset in the data source.

            \param datasetName The target dataset name.
            \param d           The data items to be added to the dataset.
            \param options     A list of optional modifiers. It is driver specific.
            \param limit       The number of items to be used from the input dataset. If set to 0 (default) all items are used.

            \pre All parameters must be valid pointers.

            \exception Exception It throws an exception if the data items can not be added.

            \note It is the caller responsability to release the dataset 'd' pointer.

            \note DataSetPersistence will start reading the dataset 'd' in the
                  current position. So, keep in mind that it is the caller responsability
                  to inform the dataset 'd' in the right position (and a valid one) to start processing it.
          */
          virtual void add(const std::string& datasetName,
                           DataSet* d,
                           const std::map<std::string, std::string>& options,
                           std::size_t limit = 0) throw(Exception) = 0;

          /*!
            \brief It updates the dataset items in the data source.

            It updates a dataset in the
            data source based on OID list.

            \param datasetName The target dataset name.
            \param dataset     The list of data items to be updated.
            \param properties  The list of properties of the dataset to be updated.
            \param oids        The list of objects to be updated.
            \param limit       The number of items to be used from the input dataset. If set to 0 (default) all items are used.

            \pre All parameters must be valid pointers.

            \exception Exception It throws an exception if the dataset can not be updated.

            \note It is the caller responsability to release the dataset pointer.

            \note DataSetPersistence will start reading the dataset 'd' in the
                  current position. So, keep in mind that it is the caller responsability
                  to inform the dataset 'd' in the right position (and a valid one) to start processing it.
          */
          virtual void update(const std::string& datasetName,
                              DataSet* dataset,
                              const std::vector<std::size_t>& properties,
                              const ObjectIdSet* oids,
                              const std::map<std::string, std::string>& options,
                              std::size_t limit = 0) throw(Exception) = 0;

          //@}
      };

      typedef boost::shared_ptr<AbstractDataSource> AbstractDataSourcePtr;

    }  // end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ABSTRACTDATASOURCE_H

