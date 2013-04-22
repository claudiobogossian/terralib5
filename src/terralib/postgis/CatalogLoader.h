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
  \file terralib/postgis/CatalogLoader.h

  \brief It allows to retrieve information about datasets in a PostGIS data source.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_CATALOGLOADER_H
#define __TERRALIB_POSTGIS_INTERNAL_CATALOGLOADER_H

// TerraLib
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "Config.h"

namespace te
{
// Forward declaration
  namespace da { class DataSet; }
  namespace dt { class Property; }
  namespace rst { class RasterProperty; }

  namespace pgis
  {
// Forward declaration
    class Transactor;

    /*!
      \class CatalogLoader

      \brief It allows to retrieve information about datasets in a PostGIS data source.

      \sa te::da::DataSourceCatalogLoader, te::da::DataSetType, Transactor
    */
    class TEPGISEXPORT CatalogLoader : public te::da::DataSourceCatalogLoader
    {
      public:

        CatalogLoader(Transactor* t);

        ~CatalogLoader();

        void getDataSets(boost::ptr_vector<std::string>& datasets);

        te::da::DataSetType* getDataSetType(const std::string& datasetName, const bool full = false);

        void getProperties(te::da::DataSetType* dt);

        te::dt::Property* getProperty(const std::string& datasetName, const std::string& propertyName);

        void getPrimaryKey(te::da::DataSetType* dt);

        void getUniqueKeys(te::da::DataSetType* dt);

        void getForeignKeys(te::da::DataSetType* dt, std::vector<std::string>& fkNames, std::vector<std::string>& rdts);

        te::da::ForeignKey* getForeignKey(const std::string& fkName, te::da::DataSetType* dt, te::da::DataSetType* rdt);

        void getIndexes(te::da::DataSetType* dt);

        void getCheckConstraints(te::da::DataSetType* dt);

        void getSequences(std::vector<std::string*>& sequences);

        te::da::Sequence* getSequence(const std::string& seqName);

        te::gm::Envelope* getExtent(const te::dt::Property* sp);

        void loadCatalog(const bool full = false);

        bool hasDataSets();

        bool datasetExists(const std::string& name);

        bool primarykeyExists(const std::string& name);

        bool uniquekeyExists(const std::string& name);

        bool foreignkeyExists(const std::string& name);

        bool checkConstraintExists(const std::string& name);

        bool indexExists(const std::string& name);

        bool sequenceExists(const std::string& name);

        te::da::DataSourceTransactor* getTransactor() const;

        Transactor* getPGTransactor() const { return m_t; }

        /*!
          \brief It returns the list of tables and views in the database.

          \return A recordset with the following fields:
                  <ul>
                  <li>0 (Oid):  pg_class.oid</li>
                  <li>1 (name): pg_namespace.nspname</li>
                  <li>2 (name): pg_class.relname</li>
                  <li>3 (char): pg_class.relkind</li>
                  </ul>

          \exception Exception It throws an exception if it was not possible to get the information needed.

          \note Information about system tables and system views will not be returned.
          \note The client of this method will take the ownership of the returned DataSet.
          \note PostGIS driver extended method.
        */
        te::da::DataSet* getDataSetTypes();

        /*!
          \brief It looks for the table oid in the PostgreSQL system tables.

          \param tableName  Table name. If it doesn't have the schema prefix name, it will look the table in the default schema.
 
          \exception Exception It throws an exception if it was not possible to set the oid information.

          \note PostGIS driver extended method.
        */
        unsigned int getTableId(const std::string& tableName);

        /*!
          \brief It looks for a table with the given oid in the PostgreSQL system tables.

          \param id The table oid.

          \return The table name.
 
          \exception Exception It throws an exception if it was not possible to retrieve the information.

          \note PostGIS driver extended method.
        */
        std::string getTableName(unsigned int id);
        
        /*!
          \brief It sets the property oid from PostgreSQL system tables.

          \param p  The property whose oid will be set from PostgreSQL system tables.
         
          \exception Exception It throws an exception if it was not possible to set the oid information.

          \note PostGIS driver extended method.
        */
        void getPropertyId(te::dt::Property* p);

        /*!
          \brief It retrieves a property from a given table or view.

          \param dtid The table id.
          \param pid  The property id.

          \return The property having the given id in the informed table. The caller will take the ownership of the returned property.

          \exception It throws an exception if it can not load the information.

          \note The property will not have specific grometric information.

          \note PostGIS driver extended method.
        */
        te::dt::Property* getProperty(unsigned int dtid, unsigned int pid);

        /*!
          \brief It returns the list of properties for the given table or view.

          \param dtid The dataset to load its information.

          \return A recordset with the following fields:
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

          \pre The informed dataset type must have a valid id.

          \exception Exception It throws an exception if it was not possible to get the information needed.

          \note The client of this method will take the ownership of the returned dataset.
          \note PostGIS driver extended method.
        */
        te::da::DataSet* getProperties(unsigned int dtid);

        ///*!
        //  \brief It loads information about properties of the given dataset type.

        //  Before calling this method, make sure that datasetType doesn't have associated properties.
        //  It must have just a name and a valid ID before calling this method.

        //  \param dt The dataset type to load its properties.

        //  \pre The informed DataSetType must have a valid id and name.

        //  \exception It throws an exception if it can not load the information.

        //  \note This method will not load information about primary keys, foreign keys, sequences, indexes, unique keys and check-constraints.
        //  \note PostGIS driver extended method.
        //*/
        //void getProperties(te::da::DataSetType* dt);

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
        te::da::DataSet* getConstraints(unsigned int dtid, char conType = '\0');

        /*!
          \brief Given a DataSetType it loads its constraints: primary key, unique key, foreign key and check constraints.

          \param datasetType The dataset type we are looking information for.

          \pre The informed DataSetType must have a valid id and name.
          \pre If DataSetType datasetType is associated to a DataSourceCatalog
               be sure that all DataSetTypes referenced by its foreign keys
               are in the catalog, otherwise you will have
               troubles when getting foreign key information.

          \exception Exception It throws an exception if it can not load the information.

          \warning Remember that this method doesn't associate PK with indexs and UK with indexes.

          \note PostGIS driver extended method.
        */
        void getConstraints(te::da::DataSetType* datasetType);

        /*!
          \brief Given a recordset pointing to the constraint row, this method adds a check-constraint to the given dataset type.

          \param ccInfo A recordset that contains information about a check-constraint.
          \param dt     The dataset type to add the constraint.
          
          \return A check-constraint from the given information.

          \pre The recordset must have the following fields:
               <ul>
               <li>0 (Oid): constraint oid (c.oid)</li>
               <li>1 (name): constraint schema name (n.nspname)</li>
               <li>2 (name): constraint name (c.conname)</li>
               <li>3 (char): constraint type (c.contype), with the following value: 'c'</li>
               <li>10 (text): constraint expression (pg_get_constraintdef(c.oid))</li>
               </ul>

           \post After calling this method the constraint will be added to the DataSetType.
                 So, the returned pointer will be already attached to DataSetType.

           \note PostGIS driver extended method.
        */
        static te::da::CheckConstraint* addCheckConstraint(te::da::DataSet* ccInfo, te::da::DataSetType* dt);

        /*!
          \brief Given a recordset pointing to the pk constraint row, this method adds a primary key to the dataset type.

          \param pkInfo  A recordset that contains information about a constraint.
          \param dt      The dataset type to add the pk constraint.
          
          \return The primary key added to the dataset type using the given information.

          \pre The recordset must have the following fields:
               <ul>
               <li>0 (Oid): constraint oid (c.oid)</li>
               <li>1 (name): constraint schema name (n.nspname)</li>
               <li>2 (name): constraint name (c.conname)</li>
               <li>3 (char): constraint type (c.contype), with the following value: 'p'</li>
               <li>8 (int2[]): array of attribute numbers (c.conkey), the list of keys in the primary key</li>
               </ul>

           \post After calling this method the constraint will be added to the dataset type.
                 So, the returned pointer will be already attached to dataset type.

           \note If the dataset type doesn't have all the properties used in the primary key this method will do nothing.

           \note PostGIS driver extended method.
        */
        static te::da::PrimaryKey* addPrimaryKey(te::da::DataSet* pkInfo, te::da::DataSetType* dt);

        /*!
          \brief Given a recordset pointing to the constraint row, this method adds a unique key to the dataset type.

          \param ukInfo  A recordset that contains information about a constraint.
          \param dt      The dataset type to add the constraint.
          
          \return A unique key from the given information.

          \pre The recordset must have the following fields:
               <ul>
               <li>0 (Oid): constraint oid (c.oid)</li>
               <li>1 (name): constraint schema name (n.nspname)</li>
               <li>2 (name): constraint name (c.conname)</li>
               <li>3 (char): constraint type (c.contype), with the following value: 'u'</li>
               <li>8 (int2[]): array of attribute numbers (c.conkey), the list of keys in the unique key</li>
               </ul>

           \post After calling this method the constraint will be added to the DataSetType.
                 So, the returned pointer will be already attached to DataSetType.

           \note If the dataset type doesn't have all the properties used in the unique key this method will do nothing.

           \note PostGIS driver extended method.
        */
        static te::da::UniqueKey* addUniqueKey(te::da::DataSet* ukInfo, te::da::DataSetType* dt);

        /*!
          \brief It returns the list of foreign keys of the given table.

          \param dt      The dataset type we are looking information for.

          \return A recordset with the following fields:
                  <ul>
                  <li>0 (name): foreign key schema name (fs.nspname)</li>
                  <li>1 (name): foreign key name (c.conname)</li>
                  <li>2 (name): table schema name (s.nspname)</li>
                  <li>3 (name): related table name (t.name)</li>
                  </ul>

          \pre The informed DataSetType must have a valid id and name.

          \exception Exception It throws an exception if it can not get the information.

          \note The client of this method will take the ownership of the returned DataSet.
          \note PostGIS driver extended method.
        */
        te::da::DataSet* getFKs(te::da::DataSetType* dt);

        /*!
          \brief It retrieves information about a given foreign key relating the two tables.

          \param fk  The name of the foreign key we are looking information for.
          \param dt  The dataset type we are looking the fk information for.
          \param rdt The table associated to dt.

          \return A recordset with the following fields:
                  <ul>
                  <li>0 (Oid): constraint oid (c.oid)</li>
                  <li>1 (char): OnUpdate action (c.confupdtype)</li>
                  <li>2 (char): OnDeletion action (c.confdeltype)</li>
                  <li>3 (char): ??? (c.confmatchtype)</li>
                  <li>4 (int2[]): array of attribute numbers (c.conkey), the list of keys in a foreign key</li>
                  <li>5 (int2[]): array of attribute numbers in the referenced table (c.confkey)</li>
                  </ul>

          \pre The informed dataset type must have a valid id and name.

          \exception Exception It throws an exception if it can not get the information.

          \note The client of this method will take the ownership of the returned DataSet.
          \note PostGIS driver extended method.
        */
        te::da::DataSet* getFK(const std::string& fk, te::da::DataSetType* dt, te::da::DataSetType* rdt);

        /*!
          \brief Given a recordset pointing to the fk row, this method adds a foreign key to the dataset type.

          \param fkInfo  A recordset that contains information about a foreign key.
          \param dt      The dataset type to add the constraint.
          
          \return A foreign key from the given information.

          \pre The recordset must have the following fields:
               <ul>
               <li>0 (Oid): constraint oid (c.oid)</li>
               <li>1 (name): constraint name (c.conname)</li>
               <li>2 (char): constraint type (c.contype), with the following value: 'f'</li>
               <li>3 (Oid): the referenced table (c.confrelid)</li>
               <li>4 (char): OnUpdate action (c.confupdtype)</li>
               <li>5 (char): OnDeletion action (c.confdeltype)</li>
               <li>6 (char): ??? (c.confmatchtype)</li>
               <li>7 (int2[]): array of attribute numbers (c.conkey), the list of keys in a foreign key</li>
               <li>8 (int2[]): array of attribute numbers in the referenced table (c.confkey)</li>
               </ul>

           \post After calling this method the constraint will be added to the dataset type.
                 So, the returned pointer will be already attached to dataset type.
           \post If the dataset type is not associated to a catalog or there is not the related dataset type,
                 this method returns NULL.

           \note PostGIS driver extended method.
        */
        te::da::ForeignKey* addForeignKey(te::da::DataSet* fkInfo, te::da::DataSetType* dt);

        /*!
          \brief It returns the list of indexes for the given table or view.

          \param dtOid      The oid of the table.
          \param indexName  If you want to load a particular index, the index name.

          \return A recordset with the following fields:
                  <ul>
                  <li>0 (Oid): index oid (idx_table.oid)</li>
                  <li>1 (name): index namespace name (s.nspname)</li>
                  <li>2 (name): index name (idx_table.relname)</li>
                  <li>3 (int2[]): array of attribute number, they take part of the index (pg_index.indkey)</li>
                  <li>4 (name): index type (pg_am.amname)</li>
                  <li>5 (bool): if 't' this index is unique (pg_index.indisunique)</li>
                  <li>6 (bool): if 't' this index is associated to the primary key (pg_index.indisprimary)</li>
                  </ul>

          \exception Exception It throws an exception if it was not possible to get the information needed.

          \note The client of this method will take the ownership of the returned DataSet.
          \note PostGIS driver extended method.
        */
        te::da::DataSet* getIndexes(unsigned int dtOid, const std::string& indexName = "");

        /*!
          \brief Given a recordset pointing to a row with index information, this method adds an index to the dataset type.

          \param idxInfo A recordset that contains information about an index.
          \param dt      The DataSetType to add the index.
          
          \return An index from the given information.

          \pre The recordset must have the following fields:
               <ul>
               <li>0: index oid (idx_table.oid)</li>
               <li>1 (name): index namespace name (s.nspname)</li>
               <li>2: index name (idx_table.relname)</li>
               <li>3: array of attribute number, they take part of the index (pg_index.indkey)</li>
               <li>4: index type (pg_am.amname)</li>
               <li>5: if 't' this index is unique (pg_index.indisunique)</li>
               <li>6: if 't' this index is associated to the primary key (pg_index.indisprimary)</li>
               </ul>

           \post After calling this method the index will be added to the DataSetType.
                 So, the returned pointer will be already attached to DataSetType.
           \post If the index is unique or is a primary key it will try to associate the index to a uk or to a pk.
                 This association is done by comparing the index name and the constraint (pk name or uk name).

           \note PostGIS driver extended method.
        */
        static te::da::Index* addIndex(te::da::DataSet* idxInfo, te::da::DataSetType* dt);

        /*!
          \brief It returns the list of sequences in the database.

          \return A recordset with the following fields:
                  <ul>
                  <li>0 (Oid): sequences oid (c.oid)</li>
                  <li>1 (name): schema name (n.nspname)</li>
                  <li>2 (name): sequence name (c.relname)</li>
                  <li>3 (char): object type = 'S' (c.relkind)</li>
                  </ul>

          \exception Exception It throws an exception if it was not possible to get the information needed.

          \note The client of this method will take the ownership of the returned DataSet.
          \note PostGIS driver extended method.
        */
        te::da::DataSet* getSequences();

        /*!
          \brief It loads information about all sequences in the database.

          \exception It throws an exception if it can not load the information.

          \note Not thread-safe!
          \note PostGIS driver extended method.
        */
        void loadSequences();

        /*!
          \brief It tells if a constraint with the given name already exists in the database.

          \param name    The constraint name fully qualified (schema.constraint-name).
          \param conType The type of constraint to be checked. If this value is '\0' (the default) all types are checked.

          \return True if a constraint with the given name already exists otherwise false.

          \exception Exception It throws an exception if something goes wrong.
        */
        bool constraintExists(const std::string& name, char conType = '\0');

      private:

        Transactor* m_t;  //!< The associated PostGIS transactor.
    };

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_CATALOGLOADER_H
