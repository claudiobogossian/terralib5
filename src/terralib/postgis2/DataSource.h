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
  \file terralib/postgis/DataSource.h

  \brief Implementation of the data source for the PostGIS driver.  
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H
#define __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess2/datasource/DataSource.h"
#include "../dataaccess2/datasource/DataSourceCapabilities.h"
#include "Config.h"

namespace te
{
  namespace pgis
  {
// Forward declaration
    class ConnectionPool;
    struct VersionInfo;

    /*!
      \class DataSource

      \brief The PostGIS driver.

      \sa te::da::DataSource, te::da::DataSourceFactory, te::da::DataSourceManager, DataSourceFactory
    */
    class TEPGISEXPORT DataSource : public te::da::DataSource
    {
      public:

        /** @name Initializer Methods
         *  Methods related to the instantiation and destruction.
         */
        //@{

        DataSource();

        ~DataSource();

        std::string getType() const throw();

        const std::map<std::string, std::string>& getConnectionInfo() const throw();

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo) throw();

        bool isTimeAnInteger() throw();

        void setTimeAsInteger(bool timeIsInteger) throw();

        /*!
          \brief It opens the connection(s) to the PostgreSQL database server.

          This method will:
          <ul>
          <li>start the connection pool;</li>
          <li>register the data type maps and get PostGIS geometry type OID;</li>
          <li>get PostgreSQL version information;</li>
          <li>get PostGIS version information;</li>
          <li>get default schema name;</li>
          <li>register user defined types (udts);</li>
          </ul>

          \exception Exception It throws an exception if the data source can not be opened.

          \note This method doesn't load the data source catalog.
          \note Not thread safe!
        */
        void open() throw(te::da::Exception);

        void close() throw(te::da::Exception);

        bool isOpened() const throw();

        bool isValid() const throw();

        const te::da::DataSourceCapabilities& getCapabilities() const throw();

        const te::da::SQLDialect* getDialect() const throw();

        void begin() throw(te::da::Exception);

        void commit() throw(te::da::Exception);

        void rollBack() throw(te::da::Exception);

        bool isInTransaction() const throw();

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Envelope* e,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Geometry* g,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        std::auto_ptr<te::da::DataSet> query(const te::da::Select& q,
                                             te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        std::auto_ptr<te::da::DataSet> query(const std::string& query, 
                                             te::common::TraverseType travType = te::common::FORWARDONLY) throw(te::da::Exception);

        void execute(const te::da::Query& command) throw(te::da::Exception);

        void execute(const std::string& command) throw(te::da::Exception);

        void cancel() throw(te::da::Exception);

        boost::int64_t getLastGeneratedId() throw(te::da::Exception);

        /*!
          \brief It will check in the database catalog the number that identifies the PostGIS Geometry type.

          \return The oid of the geometry type or 0 if it is not found.

          \exception Exception It may throws an exception.

          \note PostGIS driver extended method.
        */
        unsigned int getGeomTypeId();

        /*!
          \brief It will check in the database catalog the number that identifies the PostGIS Raster type.

          \return The oid of the raster type or 0 if it is not found.

          \exception Exception It may throws an exception.

          \note PostGIS driver extended method.
        */
        unsigned int getRasterTypeId();

        /*!
          \brief It retrieves some information about the database such as the default schema used when no one is provided.

          \param currentSchema A reference to output the current schema.

          \exception Exception It throws an exception if it was not possible to query the database.

          \note PostGIS driver extended method.
        */
        void getDatabaseInfo(std::string& currentSchema);

        std::string escape(const std::string& value) throw(te::da::Exception);

        bool isDataSetNameValid(const std::string& datasetName) throw(te::da::Exception);

        bool isPropertyNameValid(const std::string& propertyName) throw(te::da::Exception);

        std::vector<std::string> getDataSetNames() throw(te::da::Exception);

        std::auto_ptr<te::da::DataSetType> getDataSetType(const std::string& name) throw(te::da::Exception);

        std::size_t getNumberOfProperties(const std::string& datasetName) throw(te::da::Exception);

        boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName) throw(te::da::Exception);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& propertyName) throw(te::da::Exception);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos) throw(te::da::Exception);

        std::auto_ptr<te::da::PrimaryKey> getPrimaryKey(const std::string& datasetName) throw(te::da::Exception);

        std::vector<std::string> getUniqueKeyNames(const std::string& datasetName) throw(te::da::Exception);

        boost::ptr_vector<te::da::UniqueKey> getUniqueKeys(const std::string& datasetName) throw(te::da::Exception);

        std::auto_ptr<te::da::UniqueKey> getUniqueKey(const std::string& datasetName,
                                                            const std::string& name) throw(te::da::Exception);

        std::vector<std::string> getForeignKeyNames(const std::string& datasetName) throw(te::da::Exception);

        std::auto_ptr<te::da::ForeignKey> getForeignKey(const std::string& datasetName,
                                                              const std::string& name) throw(te::da::Exception);

        std::vector<std::string> getIndexNames(const std::string& datasetName) throw(te::da::Exception);

        std::auto_ptr<te::da::Index> getIndex(const std::string& datasetName,
                                                    const std::string& name) throw(te::da::Exception);

        std::vector<std::string> getCheckConstraintNames(const std::string& datasetName) throw(te::da::Exception);

        std::auto_ptr<te::da::CheckConstraint> getCheckConstraint(const std::string& datasetName,
                                                                        const std::string& name) throw(te::da::Exception);

        std::vector<std::string> getSequenceNames() throw(te::da::Exception);

        std::auto_ptr<te::da::Sequence> getSequence(const std::string& name) throw(te::da::Exception);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                          const std::string& propertyName) throw(te::da::Exception);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                  std::size_t propertyPos) throw(te::da::Exception);

        std::size_t getNumberOfItems(const std::string& datasetName) throw(te::da::Exception);

        bool hasDataSets() throw(te::da::Exception);

        bool datasetExists(const std::string& name) throw(te::da::Exception);

        bool primarykeyExists(const std::string& datasetName,
                              const std::string& name) throw(te::da::Exception);

        bool uniquekeyExists(const std::string& datasetName, const std::string& name) throw(te::da::Exception);

        bool foreignkeyExists(const std::string& datasetName,
                              const std::string& name) throw(te::da::Exception);

        bool checkConstraintExists(const std::string& datasetName,
                                    const std::string& name) throw(te::da::Exception);

        bool indexExists(const std::string& datasetName,
                          const std::string& name) throw(te::da::Exception);

        bool sequenceExists(const std::string& name) throw(te::da::Exception);

        void createDataSet(te::da::DataSetType* dt,
                                    const std::map<std::string, std::string>& options) throw(te::da::Exception);

        void cloneDataSet(const std::string& name,
                          const std::string& cloneName,
                          const std::map<std::string, std::string>& options) throw(te::da::Exception);

        void dropDataSet(const std::string& name) throw(te::da::Exception);

        void renameDataSet(const std::string& name,
                            const std::string& newName) throw(te::da::Exception);

        void addProperty(const std::string& datasetName,
                          const te::dt::Property* p) throw(te::da::Exception);

        void dropProperty(const std::string& datasetName,
                          const std::string& propertyName) throw(te::da::Exception);

        void renameProperty(const std::string& datasetName,
                            const std::string& propertyName,
                            const std::string& newPropertyName) throw(te::da::Exception);

        void addPrimaryKey(const std::string& datasetName,
                            const te::da::PrimaryKey* pk) throw(te::da::Exception);

        void dropPrimaryKey(const std::string& datasetName,
                            const std::string& primaryKeyName) throw(te::da::Exception);

        void addUniqueKey(const std::string& datasetName,
                          const te::da::UniqueKey* uk) throw(te::da::Exception);

        void dropUniqueKey(const std::string& datasetName,
                            const std::string& uniqueKeyName) throw(te::da::Exception);

        void addIndex(const std::string& datasetName,
                      const te::da::Index* idx,
                      const std::map<std::string, std::string>& options) throw(te::da::Exception); 

        void dropIndex(const std::string& datasetName,
                        const std::string& idxName) throw(te::da::Exception);

        void addForeignKey(const std::string& datasetName,
                           const te::da::ForeignKey* fk) throw(te::da::Exception);

        void dropForeignKey(const std::string& datasetName,
                            const std::string& fkName) throw(te::da::Exception);

        void addCheckConstraint(const std::string& datasetName,
                                const te::da::CheckConstraint* cc) throw(te::da::Exception);

        void dropCheckConstraint(const std::string& datasetName,
                                  const std::string& name) throw(te::da::Exception);

        void createSequence(const te::da::Sequence* sequence) throw(te::da::Exception);

        void dropSequence(const std::string& name) throw(te::da::Exception);

        void add(const std::string& datasetName,
                  te::da::DataSet* d,
                  const std::map<std::string, std::string>& options,
                  std::size_t limit) throw(te::da::Exception);

        void remove(const std::string& datasetName,
                    const te::da::ObjectIdSet* oids) throw(te::da::Exception);

        void update(const std::string& datasetName,
                    te::da::DataSet* dataset,
                    const std::vector<std::size_t>& properties,
                    const te::da::ObjectIdSet* oids,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit) throw(te::da::Exception);

        void optimize(const std::map<std::string, std::string>& opInfo);

       /*!
          \brief It returns a pointer to the internal connection pool.

          \return A pointer to the internal connection pool.          

          \note The caller must not delete the connectio pool. It is used by transactor class.

          \note PostGIS driver extended method.
        */
        ConnectionPool* getConnPool() const;

        /*!
          \brief It returns the type id associated to the PostGIS Geometry type.

          \return The type id associated to the PostGIS Geometry type.

          \note PostGIS driver extended method.
        */
        unsigned int getGeomTypeId() const;

        /*!
          \brief It returns the type id associated to the PostGIS Raster type.

          \return The type id associated to the PostGIS Raster type.

          \note PostGIS driver extended method.
        */
        unsigned int getRasterTypeId() const;

        /*!
          \brief It returns the current schema associated to the database connection, or NULL, if none is set.

          \return The current schema associated to the database connection, or NULL, if none is set.

          \note PostGIS driver extended method.
        */
        const std::string* getCurrentSchema();

        /*!
          \brief It looks for the table oid in the PostgreSQL system tables.

          \param tableName  Table name. If it doesn't have the schema prefix name, it will look the table in the default schema.
 
          \exception Exception It throws an exception if it was not possible to set the oid information.

          \note PostGIS driver extended method.
        */
        unsigned int getTableId(const std::string& tableName) throw(te::da::Exception);

        /*!
          \brief It looks for a table with the given oid in the PostgreSQL system tables.

          \param id The table oid.

          \return The table name.
 
          \exception Exception It throws an exception if it was not possible to retrieve the information.

          \note PostGIS driver extended method.
        */
        std::string getTableName(unsigned int id);

        /*!
          \brief It returns the list of constraints (primary key, unique key, foreign key and check constraints) of the given table or view.

          \param dtid    The dataset type we are looking information for.
          \param conType The type of constraint to be retrieved. If this value is '\0' (the default) all types are retrieved.

          \return A recordset with the following fields:
                  <ul>
                  <li>0 (Oid): constraint oid (c.oid)</li>
                  <li>1 (name): constraint schema name (n.nspname)</li>
                  <li>2 (name): constraint name (c.conname)</li>
                  <li>3 (char): constraint type (c.contype), one of the following values: 'c', 'f', 'p', 'u'</li>
                  <li>4 (Oid): the referenced table (c.confrelid)</li>
                  <li>5 (char): OnUpdate action (c.confupdtype)</li>
                  <li>6 (char): OnDeletion action (c.confdeltype)</li>
                  <li>7 (char): ??? (c.confmatchtype)</li>
                  <li>8 (int2[]): array of attribute numbers (c.conkey), the list of keys in a foreign key, primary key or unique key</li>
                  <li>9 (int2[]): array of attribute numbers in the referenced table (c.confkey)</li>
                  <li>10 (text): constraint expression (pg_get_constraintdef(c.oid))</li>
                  </ul>

          \exception Exception It throws an exception if it can not get the information.

          \note The client of this method will take the ownership of the returned DataSet.
          \note PostGIS driver extended method.
        */
        std::auto_ptr<te::da::DataSet> getConstraints(unsigned int dtid, char conType = '\0') throw(te::da::Exception);

        /*!
          \brief It sets the SQL dialect used by the PostGIS driver.

          \param myDialect The PostGIS SQL dialect to be used when translating a query.

          \note The PostGIS data source will take the ownership of the given dialect.

          \note PostGIS driver extended method.
        */
        static void setDialect(te::da::SQLDialect* myDialect);

        /*!
          \brief It sets the capabilities document.
          
          \param capabilities The PostGIS data source capabilities.

          \note The PostGIS data source will take the ownership of the given capabilities object.

          \note PostGIS driver extended method.
        */
        static void setCapabilities(const te::da::DataSourceCapabilities& capabilities);

        static std::vector<std::string> getDataSourceNames(const std::string& dsType, const std::map<std::string, std::string>& info);
       
        static std::vector<std::string> getEncodings(const std::string& dsType, const std::map<std::string, std::string>& info);

      protected:

        void create(const std::map<std::string, std::string>& dsInfo) throw(te::da::Exception);

        void drop(const std::map<std::string, std::string>& dsInfo) throw(te::da::Exception);

        bool exists(const std::map<std::string, std::string>& dsInfo) throw(te::da::Exception);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& info) throw(te::da::Exception);

        std::vector<std::string> getEncodings(const std::map<std::string, std::string>& info);

      private:

        class Impl;

        Impl* m_pImpl;
    };

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H
