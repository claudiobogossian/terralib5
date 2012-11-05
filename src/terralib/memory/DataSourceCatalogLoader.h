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
  \file DataSourceCatalogLoader.h

  \brief DataSourceCatalogLoader class implementation for the TerraLib In-Memory Driver.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_DATASOURCECATALOGLOADER_H
#define __TERRALIB_MEMORY_INTERNAL_DATASOURCECATALOGLOADER_H

// TerraLib
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "Config.h"

namespace te
{
  namespace mem
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class DataSourceCatalogLoader

      \brief DataSourceCatalogLoader class implementation for the TerraLib In-Memory Driver.

      \sa te::da::DataSourceCatalogLoader
    */
    class TEMEMORYEXPORT DataSourceCatalogLoader : public te::da::DataSourceCatalogLoader
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

        te::gm::Envelope* getExtent(const te::gm::GeometryProperty* gp);

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

      private:

        DataSourceTransactor* m_t;  //!< The associated transactor.
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASOURCECATALOGLOADER_H

