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
  \file terralib/dataaccess/datasource/DataSource.h

  \brief An abstraction for a data provider (like a DBMS, Web Service or a regular file).
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASOURCE_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASOURCE_H

// TerraLib
#include "../Config.h"

// STL
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace da
  {
// Forward declarations
    class DataSourceCapabilities;
    class DataSourceCatalog;
    class DataSourceTransactor;
    class SQLDialect;

    /*!
      \class DataSource

      \brief An abstraction for a data provider (like a DBMS, Web Service or a regular file).

      A data source is the fundamental class of the data access module and
      it represents a data repository. For example, it may represents a
      PostgreSQL database, an Oracle database, an OGC Web Feature Service, a
      directory of ESRI shape-files or a data stream. Each type of
      data source requires a driver implementation that must register a
      concrete factory for building data source objects
      of that type (DataSourceFactory).
      
      A data source is characterized by a set of parameters that can be used
      to set up an access channel to its underlying repository. This information
      is referred hereinafter as the data source connection information. The
      data source can be viewed as a factory for connections to the underlying
      system. This connection is called in TerraLib a data source
      transactor (DataSourceTransactor). A transactor can be used in order
      to access, both for reading an writing, data in a data source. The
      driver implementation is responsible for implementing or not a
      pooling of transactors.
      
      A data source expouses the data contained in it as a collection of datasets. The information about what data
      are stored in a data source may be available in the data source catalog
      (DataSourceCatalog). For each dataset the catalog keeps information about
      its name and structure (DataSetType).
      
      Besides the descriptive information about the underlying data repository
      each data source also provides information about its requirements and
      capabilities. This information may be used by applications so that they
      can adapt to its requirements and capabilities.

      \sa DataSourceFactory, DataSourceManager, DataSet, DataSetType, DataSourceTransactor, DataSourceCatalog, DataSourceCatalogLoader
    */
    class TEDATAACCESSEXPORT DataSource : public boost::noncopyable 
    {
      public:

        /*!
          \brief An identification value for the data source.

          \return The data source identification.
        */
        const std::string& getId() const;

        /*!
          \brief Sets the data source identification.

          \param id An identification value.
        */
        void setId(const std::string& id);

        /*!
          \brief It returns the data source type name (in UPPER CASE) such as: POSTGIS, SQLITE, WFS, WMS, MYSQL, ORACLE_SPATIAL, SHP, MICROSOFT_ACCESS, OGR, GDAL, FDO.

          \return The data source type name such as: POSTGIS, SQLITE, WFS, WMS, MYSQL, ORACLE_SPATIAL, SHP, MICROSOFT_ACCESS, OGR, GDAL, FDO.

          \note Each data source driver must have a unique name.

          \note Thread-safe!
        */
        virtual const std::string& getType() const = 0;

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
        virtual const std::map<std::string, std::string>& getConnectionInfo() const = 0;

        /*!
          \brief It sets the connection information to be used when connecting to the data source.

          \param connInfo Key-value-pairs (kvp) with the connection information.
        */
        virtual void setConnectionInfo(const std::map<std::string, std::string>& connInfo) = 0;

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
        virtual std::string getConnectionStr() const;

        /*!
          \brief It sets the connection information to be used when connecting to the data source.

          \param connInfo An URL encoded string with key-value-pairs that contains information about the data source.
        */
        virtual void setConnectionStr(const std::string& connInfo);

        /*!
          \brief It returns the known capabilities of the data source.
          
          The returned object has all informatin about what the data source can perform.
          Here you will find if the data source implementation supports primary keys,
          foreign keys, if it can be used in a thread environment and much more information.

          \param capabilities The known capabilities of the data source.

          \note Thread-safe!
        */
        virtual const DataSourceCapabilities& getCapabilities() const = 0;

        /*!
          \brief It returns the data source SQL dialect if one exists.

          \return The data source SQL dialect.
        */
        virtual const te::da::SQLDialect* getDialect() const = 0;

        /*!
          \brief It opens the data source and makes it ready for use.

          If the subclass needs to open a connection to a database server, 
          or to open a file or to get information from a Web Service, this
          method can do this kind of job in order to prepare the data source to be
          in an operational mode. It will use the connection information provided by the methods setConnectionInfo.

          \exception Exception It throws an exception if the data source can not be opened.

          \note This method doesn't load the data source catalog.

          \note Not thread-safe!
        */
        virtual void open() = 0;

        /*!
          \brief It opens the data source and makes it ready for use.

          If the subclass needs to open a connection to a database server, 
          or to open a file or to get information from a Web Service, this
          method can do this kind of job in order to prepare the data source to be
          in an operational mode.

          \param connInfo The set of parameters used to set up the underlying access channel to the repository.

          \exception Exception It throws an exception if the data source can not be opened.

          \note This method doesn't load the data source catalog.

          \note Not thread-safe!
        */
        virtual void open(const std::map<std::string, std::string>& connInfo);

        /*!
          \brief It opens the data source and makes it ready for use.

          If the subclass needs to open a connection to a database server, 
          or to open a file or to get information from a Web Service, this
          method can do this kind of job in order to prepare the data source to be
          in an operational mode.

          \param connInfo The set of parameters used to set up the underlying access channel to the repository (URL encoded).

          \exception Exception It throws an exception if the data source can not be opened.

          \note This method doesn't load the data source catalog.

          \note Not thread-safe!
        */
        virtual void open(const std::string& connInfo);

        /*!
          \brief It closes the data source and clears all the resources used by its internal communication channel.

          This method closes any connection, closes any opened file
          or releases any other resources.

          \exception Exception It throws an exception if the data source can not be closed.

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
          \brief It returns the data source catalog.

          Before using this method, a user must explicitly load
          the data source catalog. This can be done by
          using the method loadCatalog or calling loadDataSetMetadata through
          a data source transactor created by the data source.

          \return A non-NULL pointer to the data source catalog.

          \note The data source has the ownership of the returned pointer. So, don't delete it.

          \note Thread-safe!
        */
        virtual DataSourceCatalog* getCatalog() const = 0;

        /*!
          \brief It returns an object that can execute transactions in the context of a data source.

          Use this method to get an object that allows to retrieve
          dataset, to insert data or to modify dataset types.
          You don't need to cache this kind of object because each driver in TerraLib
          already keeps a pooling. So as soon as you finish using
          the transactor, destroy it.

          \return A pointer to an object that can execute transactions in the context of a data source.

          \exception Exception It throws an exception if it is not possible to get a Transactor, for example, if there is not an available connection.

          \note The caller of this method will take the ownership of the transactor.

          \note Thread-safe!
        */
        virtual DataSourceTransactor* getTransactor() = 0;

        /*!
          \brief For some data access drivers this method will perform some operations to optmize the data storage.

          This is can be a tipical maintenance command in database systems (like vacuum).

          \param opInfo Any information needed by the underlying driver in order to fine tune the optimization.

          \exception Exception It may throws an exception if something goes wrong.

          \note Not thread-safe!

          \note For some systems you must assure that no other thread are executing commands against the data source.

          \note For some drivers this method doesn't have effect.
        */
        virtual void optimize(const std::map<std::string, std::string>& opInfo) = 0;
       
        /*!
          \brief It returns a ecodings names string vector.
          
          \param dsType The data source type name (example: PostGIS, Oracle, WFS).
          \param dsInfo The data source information.

          \return Ecodings names string vector.

          \exception Exception It throws an exception if the data source can not be consulted.
        */
        static std::vector<std::string> getDataSources(const std::string& dsType, const std::map<std::string, std::string>& info);

        /*!
          \brief It returns a DataSources names string vector.

          
          \param dsType The data source type name (example: PostGIS, Oracle, WFS).
          \param dsInfo The data source information.

          \return DataSources names string vector.

          \exception Exception It throws an exception if the data source can not be consulted.
        */
        static std::vector<std::string> getEncodings(const std::string& dsType, const std::map<std::string, std::string>& info);

        /*!
          \brief It creates a new data source with the given information.

          \param dsType The data source type name (example: PostGIS, Oracle, WFS).
          \param dsInfo The information for creating a new data source.

          \return It returns the new data source.

          \exception Exception It throws an exception if the new data source can not be created.

          \note After creation the data source must be in a "closed state". The caller will have to
                decide when to open the data source.

          \note Thread-safe!
        */
        static DataSource* create(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        /*!
          \brief It drops a data source.

          
          \param dsType The data source type name (example: PostGIS, Oracle, WFS).
          \param dsInfo The information for droping the data source.

          \exception Exception It throws an exception if the data source can not be dropped.

          \note No other instance of the data source to be dropped can be opened when calling this method.
        */
        static void drop(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        /*!
          \brief It checks if the informed data source exists.

          
          \param dsType The data source type name (example: PostGIS, Oracle, WFS).
          \param dsInfo The data source information.

          \return True if the data source exists of false otherwise.

          \exception Exception It throws an exception if the data source can not be consulted.

          \note Thread-safe!
        */
        static bool exists(const std::string& dsType, const std::map<std::string, std::string>& dsInfo);

        // todo: metodo para retornar a lista de data sources
        //static void getDataSources(const std::string& dsType,
        //                           const std::map<std::string, std::string>& systemInfo,
        //                           std::vector<std::map<std::string, std::string> >& datasources);

        /*! \brief Virtual destructor. */
        virtual ~DataSource() {}

        bool operator<(const DataSource& rhs) const;

      protected:

        /*! \brief Default constructor just can be called by subclasses. */
        DataSource() {}

        /*!
          \brief A pure virtual method that subclasses must implement in order to create a new data source.

          \param dsInfo The information for creating a new data source.

          \exception Exception Implementations may throw exceptions.

          \note After creation the data source must be in a "closed state". The caller will have to
                decide when to open the data source.

          \note Thread-safe!
        */
        virtual void create(const std::map<std::string, std::string>& dsInfo) = 0;

        /*!
          \brief A pure virtual method that subclasses must implement in order to drop a data source.

          \param dsInfo The information for dropping a data source.

          \exception Exception Implementations may throw exceptions.

          \note Not thread-safe!
        */
        virtual void drop(const std::map<std::string, std::string>& dsInfo) = 0;

        /*!
          \brief A pure virtual method that subclasses must implement in order to check a data source existence.
          
          \param dsInfo The data source information.

          \return True if the data source exists of false otherwise.

          \exception Exception Implementations may throw exceptions.

          \note Thread-safe!
        */
        virtual bool exists(const std::map<std::string, std::string>& dsInfo) = 0;

        /*!
          \brief A pure virtual method that subclasses must implement in order to get the DataSources.
          
          \param dsInfo The data source information.

          \return DataSources names string vector.

          \exception Exception Implementations may throw exceptions.
        */
        virtual std::vector<std::string> getDataSources(const std::map<std::string, std::string>& info) = 0;

        /*!
          \brief A pure virtual method that subclasses must implement in order to get the encodings
          
          \param dsInfo The data source information.

          \return Ecodings names string vector.

          \exception Exception Implementations may throw exceptions.
        */
        virtual std::vector<std::string> getEncodings(const std::map<std::string, std::string>& info) = 0;

      protected:

        std::string m_id;
    };

    typedef boost::shared_ptr<DataSource> DataSourcePtr;

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASOURCE_H

