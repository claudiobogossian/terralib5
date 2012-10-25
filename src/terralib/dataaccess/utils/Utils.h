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
  \file terralib/dataaccess/utils/Utils.h

  \brief Utility functions for the data access module.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_UTILS_H
#define __TERRALIB_DATAACCESS_INTERNAL_UTILS_H

// TerraLib
#include "../Config.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace gm
  {
    class GeometryProperty;
  }

  namespace da
  {
    class DataSet;
    class DataSetType;
    class DataSource;
    class DataSourceCatalogLoader;
    class DataSourceTransactor;

    TEDATAACCESSEXPORT void LoadFull(te::da::DataSetType* dataset, const std::string& datasourceId);

    TEDATAACCESSEXPORT void LoadFull(te::da::DataSetType* dataset, te::da::DataSource* datasource);

    TEDATAACCESSEXPORT void LoadFull(te::da::DataSetType* dataset, te::da::DataSourceTransactor* transactor);

    TEDATAACCESSEXPORT void LoadFull(te::da::DataSetType* dataset, te::da::DataSourceCatalogLoader* cloader);

    TEDATAACCESSEXPORT void LoadProperties(te::da::DataSetType* dataset, const std::string& datasourceId);

    TEDATAACCESSEXPORT void LoadProperties(te::da::DataSetType* dataset, te::da::DataSource* datasource);

    TEDATAACCESSEXPORT void LoadProperties(te::da::DataSetType* dataset, te::da::DataSourceTransactor* transactor);

    TEDATAACCESSEXPORT void LoadExtent(te::gm::GeometryProperty* gp, const std::string& datasourceId, bool refresh = false);

    TEDATAACCESSEXPORT void LoadExtent(te::gm::GeometryProperty* gp, te::da::DataSource* datasource, bool refresh = false);

    TEDATAACCESSEXPORT void LoadExtent(te::gm::GeometryProperty* gp, te::da::DataSourceTransactor* transactor, bool refresh = false);

    TEDATAACCESSEXPORT void GetDataSets(boost::ptr_vector<std::string>& datasets, const std::string& datasourceId);

    TEDATAACCESSEXPORT void GetDataSets(boost::ptr_vector<std::string>& datasets, te::da::DataSource* datasource);

    TEDATAACCESSEXPORT void GetDataSets(boost::ptr_vector<std::string>& datasets, te::da::DataSourceTransactor* transactor);

    TEDATAACCESSEXPORT bool HasDataSet(const std::string& datasourceId);

    TEDATAACCESSEXPORT bool HasDataSet(te::da::DataSource* datasource);

    TEDATAACCESSEXPORT bool HasDataSet(te::da::DataSourceTransactor* transactor);

    TEDATAACCESSEXPORT DataSet* GetDataSet(const std::string& name, const std::string& datasourceId);

    TEDATAACCESSEXPORT DataSet* GetDataSet(const std::string& name, te::da::DataSource* datasource);

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_UTILS_H
