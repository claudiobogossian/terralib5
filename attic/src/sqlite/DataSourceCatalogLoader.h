/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

  \brief DataSourceCatalogLoader manages metadata information for the TerraLib SQLite Data Access Driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_DATASOURCECATALOGLOADER_H
#define __TERRALIB_SQLITE_INTERNAL_DATASOURCECATALOGLOADER_H

// STL
#include <memory>
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace da
  {
    class DataSet;
    class DataSetType;
  }

  namespace sqlite
  {
    class DataSourceTransactor;

    class DataSourceCatalogLoader : public boost::noncopyable
    {
      public:

        DataSourceCatalogLoader(DataSourceTransactor* parent);

        ~DataSourceCatalogLoader();

        std::vector<std::string> getDataSetNames();

        std::auto_ptr<te::da::DataSet> getDataSets();

        bool hasDataSets();

        bool datasetExists(const std::string& name);

        te::da::DataSetType* getDataSetType(const std::string& datasetName);

        void getGeometryInfo(const std::string& datasetName, te::gm::GeometryProperty* gp);

        std::auto_ptr<te::da::DataSet> getGeometryInfo(const std::string& tableName, const std::string& geomColName);

        boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName);

        void getPrimaryKey(te::da::DataSetType* dt);

        void getUniqueKeys(te::da::DataSetType* dt);

        void getUniqueKey(te::da::DataSetType* dt,
                          int idxId,
                          const std::string& idxName);

        void getIndexes(te::da::DataSetType* dt);

        void getSpatialIndexes(te::da::DataSetType* dt);

        void getIndex(te::da::DataSetType* dt,
                      int idxId,
                      const std::string& idxName,
                      bool isUnique);

        te::gm::Envelope* getExtent(const std::string& tableName, const std::string& geomColName);

      private:

        class Impl;

        Impl* m_pImpl;
    };
  }
}

#endif  // __TERRALIB_SQLITE_INTERNAL_DATASOURCECATALOGLOADER_H