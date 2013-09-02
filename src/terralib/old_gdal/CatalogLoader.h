/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/gdal/CatalogLoader.h

  \brief It allows to retrieve information about datasets in a GDAL data source.
*/

#ifndef __TERRALIB_GDAL_INTERNAL_CATALOGLOADER_H
#define __TERRALIB_GDAL_INTERNAL_CATALOGLOADER_H

// TerraLib
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "Config.h"

// Forward declaration
class GDALDataset;

namespace te
{
// Forward declaration
  namespace da { class DataSourceCatalog; }

  namespace gdal
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class CatalogLoader

      \brief It allows to retrieve information about datasets in a GDAL data source.

      \sa te::da::DataSourceCatalogLoader, te::da::DataSetType, DataSourceTransactor
    */
    class TEGDALEXPORT CatalogLoader : public te::da::DataSourceCatalogLoader
    {
      public:

        CatalogLoader(te::gdal::DataSourceTransactor* t);

        ~CatalogLoader();

        void getDataSets(boost::ptr_vector<std::string>& datasets);

        te::da::DataSetType* getDataSetType(const std::string& datasetName, const bool full = false);

        te::dt::Property* getProperty(const std::string& datasetName, const std::string& propertyName);

        void getPrimaryKey(te::da::DataSetType* dt);

        void getUniqueKeys(te::da::DataSetType* dt);

        void getForeignKeys(te::da::DataSetType* dt, std::vector<std::string>& fkNames, std::vector<std::string>& rdts);

        te::da::ForeignKey* getForeignKey(const std::string& fkName, te::da::DataSetType* dt, te::da::DataSetType* rdt);

        void getIndexes(te::da::DataSetType* /*dt*/);

        void getCheckConstraints(te::da::DataSetType* /*dt*/);

        void getSequences(std::vector<std::string*>& /*sequences*/);

        te::da::Sequence* getSequence(const std::string& /*seqName*/);

        te::gm::Envelope* getExtent(const te::dt::Property* sp);

        void loadCatalog(const bool full = false);
      
        bool hasDataSets();
      
        void getProperties(te::da::DataSetType* dt);

        bool datasetExists(const std::string& name);

        bool primarykeyExists(const std::string& /*name*/) { return false; }

        bool uniquekeyExists(const std::string& /*name*/) { return false; }

        bool foreignkeyExists(const std::string& /*name*/) { return false; }

        bool checkConstraintExists(const std::string& /*name*/) { return false; }

        bool indexExists(const std::string& /*name*/) { return false; }

        bool sequenceExists(const std::string& /*name*/) { return false; }

        te::da::DataSourceTransactor* getTransactor() const;

      protected:

        void getDataSetTypeList(GDALDataset* dataset, std::vector<te::da::DataSetType*>& datasets);

        te::da::DataSetType* getDataSetType(GDALDataset* dataset);

        te::da::DataSourceCatalog* initCatalog();

      private:
        
        DataSourceTransactor* m_t;  //!< The associated GDAL transactor.
    };

  } // end namespace gdal
}   // end namespace te

#endif  // __TERRALIB_GDAL_INTERNAL_CATALOGLOADER_H
