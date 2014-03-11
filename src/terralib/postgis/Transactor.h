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
  \file terralib/postgis/Transactor.h

  \brief A Transactor can be viewed as a connection to the data source for reading/writing things into it.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_TRANSACTOR_H
#define __TERRALIB_POSTGIS_INTERNAL_TRANSACTOR_H

// TerraLib
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "Config.h"
#include "DataSource.h"

// STL
#include <memory>
#include <map>
#include <string>

namespace te
{
// Forward declarations
  namespace dt { class Property; }
  namespace gm { class Envelope; class Geometry; }

  namespace pgis
  {
    // Forward declarations
    class BatchExecutor;
    class DataSet;
    class Connection;
    class ObjectIdSet;
    class PreparedQuery;
    class Query;

    /*!
      \class Transactor

      \brief The transactor class for the PostGIS driver.

      \sa te::da::DataSourceTransactor, DataSource
    */
    class TEPGISEXPORT Transactor : public te::da::DataSourceTransactor
    {
      public:

        /*!
          \brief Constructor.

          \param ds    The PostGIS data source associated to this transactor.
          \param conn  An available connection that will be released when the transactor is deleted
        */
        Transactor(DataSource* ds, Connection* conn);

        /*! \brief The destructor will automatically release the connection to the pool. */
        ~Transactor();

        te::da::DataSource* getDataSource() const;

        /*!
          \brief It returns the underlying connection.

          \return The underlying connection.

          \note PostGIS driver extended method.
        */
        Connection* getConnection() const;

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


        void changePropertyDefinition(const std::string& datasetName, const std::string& propName, te::dt::Property* newProp);


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
                 std::size_t limit = 0);

        void remove(const std::string& datasetName, const te::da::ObjectIdSet* oids = 0);

        void update(const std::string& datasetName,
                    te::da::DataSet* dataset,
                    const std::vector<std::size_t>& properties,
                    const te::da::ObjectIdSet* oids,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit = 0);

        void optimize(const std::map<std::string, std::string>& opInfo);

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
          \brief It gets the full name of the given name including the schema name.

          \param name The dataset name.

          \return  The dataset full name.
        */
        std::string getFullName(const std::string& name);

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
        void getConstraints(te::da::DataSetType* dt);

        /*!
          \brief It gets all the indexes of the given dataset and adds them to the dummy schema.

          \param dt The dataset schema.

          \return The dataset indexes.
        */
        void getIndexes(te::da::DataSetType* dt);

        /*!
          \brief It gets information about all the sequences in the datasource.

          \exception It throws an exception if the sequences cannot be loaded.

          \return The list of sequences in the data source,

          \note Not thread-safe!
          \note PostGIS driver extended method.
        */
        std::vector<te::da::Sequence*> getSequences();

      private:

        DataSource* m_ds;       //!< The PostGIS data source associated to this transactor.
        Connection* m_conn;     //!< The connection used by this transactor.
        bool m_isInTransaction; //!< Tells if there is a transaction in progress.
    };

    typedef boost::shared_ptr<Transactor> TransactorPtr;

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_TRANSACTOR_H
