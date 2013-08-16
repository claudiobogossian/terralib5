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
  namespace gm  { class GeometryProperty; }
  namespace rst { class RasterProperty; }

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

        std::string getType() const;

        const std::map<std::string, std::string>& getConnectionInfo() const;

        void setConnectionInfo(const std::map<std::string, std::string>& connInfo);

        te::da::Connection* getConnection();

        void closeConnection(te::da::Connection* conn);

        bool isTimeAnInteger();

        void setTimeAsInteger(bool timeIsInteger);

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
        void open();

        void close();

        bool isOpened() const;

        bool isValid() const;

        const te::da::DataSourceCapabilities& getCapabilities() const;

        const te::da::SQLDialect* getDialect() const;

        void begin();

        void commit();

        void rollBack();

        bool isInTransaction() const;

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                  te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Envelope* e,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                  const std::string& propertyName,
                                                  const te::gm::Geometry* g,
                                                  te::gm::SpatialRelation r,
                                                  te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<te::da::DataSet> query(const te::da::Select& q,
                                             te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<te::da::DataSet> query(const std::string& query, 
                                             te::common::TraverseType travType = te::common::FORWARDONLY);

        void execute(const te::da::Query& command);

        void execute(const std::string& command);

        void cancel();

        boost::int64_t getLastGeneratedId();

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

        std::string escape(const std::string& value);

        bool isDataSetNameValid(const std::string& datasetName);

        bool isPropertyNameValid(const std::string& propertyName);

        std::vector<std::string> getDataSetNames();

        te::da::DataSetTypePtr getDataSetType(const std::string& name);

       /*!
          \brief It gets the properties for the given dataset.

          \param datasetName The dataset name.

          \note Internally, a record set will be generated containing the following fields:
                <ul>
                <li>0 (int2): attribute number in the table (a.attnum), remember that attribute number is 1 based</li>
                <li>1 (name): attribute name (a.attname)</li>
                <li>2 (Oid): attribute type oid (t.oid)</li>
                <li>3 (bool): 't' if attribute is NOT NULL, otherwise, its value is 'f' (a.attnotnull)</li>
                <li>4 (text): type modifier information, like precision and scale (format_type(a.atttypid, a.atttypmod))</li>
                <li>5 (bool): 't' if attribute is has a default value, otherwise, its value is 'f' (a.atthasdef)</li>
                <li>6 (text): attribute default value if field number 5 is true (pg_get_expr(d.adbin, d.adrelid))</li>
                <li>7 (int4): Number of dimensions, if the column is an array type; otherwise 0 (a.attndims)</li>
                </ul>

          \exception Exception It throws an exception if it was not possible to get the information needed.

          \note PostGIS driver extended method.
        */
        boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName);

        std::vector<std::string> getPropertyNames(const std::string& datasetName);

        std::size_t getNumberOfProperties(const std::string& datasetName);

        bool propertyExists(const std::string& datasetName, const std::string& name);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, const std::string& name);

        std::auto_ptr<te::dt::Property> getProperty(const std::string& datasetName, std::size_t propertyPos);

        void addProperty(const std::string& datasetName, te::dt::Property* p);

        void dropProperty(const std::string& datasetName, const std::string& propertyName);

        void renameProperty(const std::string& datasetName, const std::string& propertyName, const std::string& newPropertyName);

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

        bool checkConstraintExists(const std::string& datasetName, const std::string& name);

        void addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc);

        void dropCheckConstraint(const std::string& datasetName, const std::string& name);

        std::auto_ptr<te::da::Index> getIndex(const std::string& datasetName, const std::string& name);

        std::vector<std::string> getIndexNames(const std::string& datasetName);

        bool indexExists(const std::string& datasetName, const std::string& name);

        void addIndex(const std::string& datasetName, te::da::Index* idx, const std::map<std::string, std::string>& options); 

        void dropIndex(const std::string& datasetName, const std::string& idxName);

        std::auto_ptr<te::da::Sequence> getSequence(const std::string& name);

        std::vector<std::string> getSequenceNames();

        bool sequenceExists(const std::string& name);

        void addSequence(te::da::Sequence* sequence);

        void dropSequence(const std::string& name);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName, const std::string& propertyName);

        std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName, std::size_t propertyPos);

        std::auto_ptr<te::gm::Envelope> getExtent(te::da::DataSet* dataset);

        std::size_t getNumberOfItems(const std::string& datasetName);

        bool hasDataSets();

        bool dataSetExists(const std::string& name);

        void createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options);

        void cloneDataSet(const std::string& name, const std::string& cloneName,
                          const std::map<std::string, std::string>& options);

        void dropDataSet(const std::string& name);

        void renameDataSet(const std::string& name, const std::string& newName);

        void add(const std::string& datasetName, te::da::DataSet* d,
                 const std::map<std::string, std::string>& options, std::size_t limit);

        void remove(const std::string& datasetName, const te::da::ObjectIdSet* oids);

        void update(const std::string& datasetName,
                    te::da::DataSet* dataset,
                    const std::vector<std::size_t>& properties,
                    const te::da::ObjectIdSet* oids,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit);

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
        const std::string& getCurrentSchema();

        /*!
          \brief It loads information about a given geometry column.

          \param datasetName The name of the dataset containing the geometric property.
          \param gp          The geometric columns to load its information.

          \exception It throws an exception if it can not load the information.
        */
        void getGeometryInfo(const std::string& datasetName, te::gm::GeometryProperty* gp);

        /*!
          \brief It loads information about a given raster column.

          \param datasetName The name of the dataset containing the geometric property.
          \param rp          The raster column to load its information.

          \exception It throws an exception if it can not load the information.
        */
        void getRasterInfo(const std::string& datasetName, te::rst::RasterProperty* rp);

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

      protected:

        /*!
          \brief It looks for the dataset id in the PostgreSQL system.

          \param datasetName  Dataset name. If it has no schema prefix name, it will look the dataset id in the default schema.
 
          \exception Exception It throws an exception, if it was not possible to get the dataset id.

          \note PostGIS driver extended method.
        */
        unsigned int getDataSetId(const std::string& datasetName);

        /*!
          \brief It looks for a dataset name with the given id in the PostgreSQL.

          \param id The dataset id.

          \return The dataset name.
 
          \exception Exception It throws an exception if it was not possible to retrieve the information.

          \note PostGIS driver extended method.
        */
        std::string getDataSetName(unsigned int id);

       /*!
          \brief It gets the information about the given dataset.

          \param datasetName The dataset name.

          \return The information about the given dataset.

          \note Internally, a record set will be generated containing the following fields:
                <ul>
                <li>0 (int2): attribute number in the table (a.attnum), remember that attribute number is 1 based</li>
                <li>1 (name): attribute name (a.attname)</li>
                <li>2 (Oid): attribute type oid (t.oid)</li>
                <li>3 (bool): 't' if attribute is NOT NULL, otherwise, its value is 'f' (a.attnotnull)</li>
                <li>4 (text): type modifier information, like precision and scale (format_type(a.atttypid, a.atttypmod))</li>
                <li>5 (bool): 't' if attribute is has a default value, otherwise, its value is 'f' (a.atthasdef)</li>
                <li>6 (text): attribute default value if field number 5 is true (pg_get_expr(d.adbin, d.adrelid))</li>
                <li>7 (int4): Number of dimensions, if the column is an array type; otherwise 0 (a.attndims)</li>
                </ul>

          \exception Exception It throws an exception if it was not possible to get the information needed.

          \note PostGIS driver extended method.
        */
        std::auto_ptr<te::da::DataSet> getPropertiesInfo(const std::string& datasetName);

        /*!
          \brief It sets the property id from the PostgreSQL system.

          \param p  The property whose oid will be set from PostgreSQL system tables.
         
          \exception Exception It throws an exception if it was not possible to set the oid information.

          \note PostGIS driver extended method.
        */
        void getPropertyId(te::dt::Property* p);

        /*!
          \brief It retrieves a property with the given id from the given dataset.

          \param pid         The property id.
          \param datasetName The dataset name.

          \return The property with the given id from the dataset.

          \note PostGIS driver extended method.
        */
        std::auto_ptr<te::dt::Property> getProperty(unsigned int pid, const std::string& datasetName);

        /*!
          \brief It gets the dataset containing information about one of the constraints(primary, foreign or 
                 unique keys, or the check constraints) of the given dataset. If the constraint type
                 is not specified, it gets all the constraints of the given dataset.

          \param datasetName The dataset name.
          \param conType     The type of constraint to be retrieved. If this value is '\0' all the types are retrieved.

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

          \return The dataset containing information about the constraints of the given dataset.
        */
        std::auto_ptr<te::da::DataSet> getConstraints(const std::string& datasetName, char conType = '\0');

        /*!
          \brief It gets the information about all the dataset constraints(primary, foreign and 
                 unique keys, and the check constraints) and loads it on the given dataset schema.

          \param dt The dataset schema.
        */
        void getConstraints(te::da::DataSetTypePtr& dt);

        /*!
          \brief It gets all the indexes of the given dataset and adds them to the dummy schema.

          \param dt The dataset schema.

          \return The dataset indexes.
        */
        void getIndexes(te::da::DataSetTypePtr& dt);

        /*!
          \brief It gets information about all the sequences in the datasource.

          \exception It throws an exception if the sequences cannot be loaded.

          \return The list of sequences in the data source,

          \note Not thread-safe!
          \note PostGIS driver extended method.
        */
        std::vector<te::da::Sequence*> getSequences();

      protected:

        void create(const std::map<std::string, std::string>& dsInfo);

        void drop(const std::map<std::string, std::string>& dsInfo);

        bool exists(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

        std::vector<std::string> getEncodings(const std::map<std::string, std::string>& dsInfo);

      private:

        class Impl;

        Impl* m_pImpl;
    };

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_DATASOURCE_H
