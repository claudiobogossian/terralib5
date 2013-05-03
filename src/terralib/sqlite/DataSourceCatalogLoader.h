/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/DataSourceCatalogLoader.h

  \brief DataSourceCatalogLoader class implementation for the TerraLib SQLite Data Access Driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_DATASOURCECATALOGLOADER_H
#define __TERRALIB_SQLITE_INTERNAL_DATASOURCECATALOGLOADER_H

// TerraLib
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "Config.h"

namespace te
{
// Forward declaration
  namespace da
  {
    class DataSet;
  }

  namespace dt
  {
    class GeometryProperty;
  }

  namespace sqlite
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class DataSourceCatalogLoader

      \brief DataSourceCatalogLoader class implementation for SQLite.

      \sa te::da::DataSourceCatalogLoader
    */
    class TESQLITEEXPORT DataSourceCatalogLoader : public te::da::DataSourceCatalogLoader
    {
      public:

        DataSourceCatalogLoader(DataSourceTransactor* parent);

        ~DataSourceCatalogLoader();

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

        /*!
          \brief It returns the list of tables and views in the database.

          \return A dataset with the following fields:
                  <ul>
                  <li>0 (int): rowid</li>
                  <li>1 (string): name</li>
                  <li>2 (string): tbl_name</li>
                  <li>3 (string): type (table or view)</li>
                  </ul>

          \exception Exception It throws an exception if it was not possible to get the information needed.

          \note Information about system tables and system views will not be returned.

          \note The client of this method will take the ownership of the returned DataSet.

          \note SQLite driver extended method.
        */
        te::da::DataSet* getDataSets();

        /*!
          \brief It retrieves information about a given geometric property.

          \param name The table to find its rowid in sqlite_master table.

          \return The table id in the SQLite catalog.

          \pre gp must reference a property attached to a datase type.

          \pos gp may be updated if there is spatiaa metadata available.

          \note SQLite driver extended method.
        */
        void getGeometryInfo(te::gm::GeometryProperty* gp);

        /*!
          \brief It returns the geometry column information from the geometry_columns table.

          \param tableName   A table with a geometry column.
          \param geomColName The geometry column name or empty if all columns must be returned.

          \return A recordset with the following fields:
                  <ul>
                  <li>0 (string): type</li>
                  <li>1 (string): coord_dimension</li>
                  <li>2 (int)...: srid</li>
                  <li>3 (int)...: spatial_index_enabled</li>
                  </ul>

          \exception Exception It throws an exception if it was not possible to get the information needed.

          \note The client of this method will take the ownership of the returned DataSet.

          \note SQLite driver extended method.
        */
        te::da::DataSet* getGeometryInfo(const std::string& tableName, const std::string& geomColName);

        /*!
          \brief It retrieves the bounding rectangle for a geometry column of a given table/view.

          \param tableName   A table or view with a geometry column.
          \param geomColName The geometry column name.

          \return The geometry property bounding rectangle or NULL if none is found.

          \exception Exception It throws an exception if something goes wrong during MBR search.

          \note The caller of this method will take the ownership of the returned box.

          \note SQLite driver extended method.
        */
        te::gm::Envelope* getExtent(const std::string& tableName, const std::string& geomColName);

        /*!
          \brief It loads information about a given index for the given DataSetType.

          \param dt       The dataset type we want to look information for.
          \param idxId    The index id.
          \param idxName  The name of the index we are looking information for.
          \param isUnique Tells if the indicated index is unique or not.

          \pre The informed DataSetType dt must have a valid id and name.

          \pos If the index is associated to a primary key or it is unique it may change the DataSetType dt
               adding more information.

          \exception Exception It throws an exception if it can not load the information.

          \note SQLite driver extended method.
        */
        void getIndex(te::da::DataSetType* dt,
                      int idxId,
                      const std::string& idxName,
                      bool isUnique);

        /*!
          \brief It loads information about a given unique key for the given DataSetType.

          \param dt       The dataset type we want to look information for.
          \param idxId    The index id.
          \param idxName  The name of the index we are looking information for, it must be a unique index.

          \pre The informed DataSetType dt must have a valid id and name.

          \exception Exception It throws an exception if it can not load the information.

          \note SQLite driver extended method.
        */
        void getUniqueKey(te::da::DataSetType* dt,
                          int idxId,
                          const std::string& idxName);

        /*!
          \brief It retrieves information about spatial indexes for the datasettype.

          \param dt The dataset type to serach for spatial-indexes.
        */
        void getSpatialIndexes(te::da::DataSetType* dt);

      private:

        DataSourceTransactor* m_t;  //!< The associated transactor.
    };

  } // end namespace sqlite
}   // end namespace te

#endif  // __TERRALIB_SQLITE_INTERNAL_DATASOURCECATALOGLOADER_H

