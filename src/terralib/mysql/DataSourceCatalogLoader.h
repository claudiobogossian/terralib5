/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mysql/DataSourceCatalogLoader.h

  \brief DataSourceCatalogLoader class implementation for MySQL data access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_DATASOURCECATALOGLOADER_H
#define __TERRALIB_MYSQL_INTERNAL_DATASOURCECATALOGLOADER_H

// TerraLib
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "Config.h"

// Forward declaration
namespace sql
{
  class DatabaseMetaData;
  class ResultSet;
}

namespace te
{
  namespace mysql
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class DataSourceCatalogLoader

      \brief DataSourceCatalogLoader class implementation for MySQL data access driver.

      \sa te::da::DataSourceCatalogLoader
    */
    class TEMYSQLEXPORT DataSourceCatalogLoader : public te::da::DataSourceCatalogLoader
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
          \brief It returns the database engine used by the table.

          \param name The table name.

          \return The database engine used by the table.

          \exception Exception It throws an exception if it is not possible to retrieve the engine name.

          \note MySQL driver extended method.
        */
        std::string getTableEngine(const std::string& name);

        /*!
          \note MySQL driver extended method.
        */
        te::da::DataSetType* getRasterDataSet(const std::string& name);

      protected:

        /*!
          \note MySQL driver extended method.
        */
        //void addPrimaryKey(sql::ResultSet* pkInfo, te::da::DataSetType* dt);

        /*!
          \note MySQL driver extended method.
        */
        void getDataSets(std::string& sql);

      private:

        DataSourceTransactor* m_t;    //!< The associated transactor.
        sql::DatabaseMetaData* m_md;  //!< The MySQL metadata object.
        std::string m_catalogName;    //!< ????
        std::string m_schemaName;     //!< database name.
    };

  } // end namespace mysql
}   // end namespace te

#endif  // __TERRALIB_MYSQL_INTERNAL_DATASOURCECATALOGLOADER_H

