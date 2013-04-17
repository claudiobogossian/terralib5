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
#include "../../geometry/Enums.h"
#include "../datasource/DataSource.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

// STL
#include <string>
#include <vector>

namespace te
{
// Forward declaration
  namespace gm
  {
    class Envelope;
    class Geometry;
    class GeometryProperty;
  }

  namespace da
  {
// Forward declaration
    class DataSet;
    class DataSetType;
    class DataSourceCatalogLoader;
    class DataSourceTransactor;
    class ObjectIdSet;

    TEDATAACCESSEXPORT void LoadFull(te::da::DataSetType* dataset, const std::string& datasourceId);

    TEDATAACCESSEXPORT void LoadFull(te::da::DataSetType* dataset, te::da::DataSource* datasource);

    TEDATAACCESSEXPORT void LoadFull(te::da::DataSetType* dataset, te::da::DataSourceTransactor* transactor);

    TEDATAACCESSEXPORT void LoadFull(te::da::DataSetType* dataset, te::da::DataSourceCatalogLoader* cloader);

    TEDATAACCESSEXPORT void LoadProperties(te::da::DataSetType* dataset, const std::string& datasourceId);

    TEDATAACCESSEXPORT void LoadProperties(te::da::DataSetType* dataset, te::da::DataSource* datasource);

    TEDATAACCESSEXPORT void LoadProperties(te::da::DataSetType* dataset, te::da::DataSourceTransactor* transactor);

    /*!
      \brief It computes the bounding rectangle for the first spatial property of the given dataset.

      \return The bounding rectangle for the first spatial property of the given dataset.

      \post The caller of this method will take the ownership of the returned box.

      \exception Exception It throws an exception if something goes wrong during MBR search.
    */
    TEDATAACCESSEXPORT te::gm::Envelope* GetExtent(te::da::DataSet* dataset);

    /*!
      \return The data extent considering the informed property. The caller will take the ownership of the returned box.

      \exception Exception It throws an exception if the extent can not be determined.
    */
    TEDATAACCESSEXPORT te::gm::Envelope* GetExtent(te::gm::GeometryProperty* gp, const std::string& datasourceId);

    /*!
      \return The data extent considering the informed property. The caller will take the ownership of the returned box.

      \exception Exception It throws an exception if the extent can not be determined.
    */
    TEDATAACCESSEXPORT te::gm::Envelope* GetExtent(te::gm::GeometryProperty* gp, te::da::DataSource* datasource);

    /*!
      \return The data extent considering the informed property. The caller will take the ownership of the returned box.

      \exception Exception It throws an exception if the extent can not be determined.
    */
    TEDATAACCESSEXPORT te::gm::Envelope* GetExtent(te::gm::GeometryProperty* gp, te::da::DataSourceTransactor* transactor);

    TEDATAACCESSEXPORT void GetDataSets(boost::ptr_vector<std::string>& datasets, const std::string& datasourceId);

    TEDATAACCESSEXPORT void GetDataSets(boost::ptr_vector<std::string>& datasets, te::da::DataSource* datasource);

    TEDATAACCESSEXPORT void GetDataSets(boost::ptr_vector<std::string>& datasets, te::da::DataSourceTransactor* transactor);

    TEDATAACCESSEXPORT std::string GetCategoryName(int category);

    TEDATAACCESSEXPORT bool HasDataSet(const std::string& datasourceId);

    TEDATAACCESSEXPORT bool HasDataSet(te::da::DataSource* datasource);

    TEDATAACCESSEXPORT bool HasDataSet(te::da::DataSourceTransactor* transactor);

    TEDATAACCESSEXPORT DataSet* GetDataSet(const std::string& name, const std::string& datasourceId);

    TEDATAACCESSEXPORT DataSet* GetDataSet(const std::string& name, te::da::DataSource* datasource);

    /*!
      \brief Search for a data source with the informed id in the DataSourceManager.

       If the data source is not found in that manager it will try to get some information
       in the DataSourceInfoManager and will create a new data source instance,
       storing a reference to the new data source in the manager.

       \param datasourceId  The data source to look for.
       \param opened        Automatically open the data source.

       \return A data source with the informed id.

       \exception Exception It throws an exception if the informed data source could not be retrieved.
    */
    TEDATAACCESSEXPORT DataSourcePtr GetDataSource(const std::string& datasourceId, const bool opened = true);

    /*
      \brief It generates the set of unique ids for every element of the given dataset.

      This method tries to use the primary key properties to generate the unique ids. If they do not exist it tries
      to use unique keys. If they do not exist then it uses all the properties.
     
      \param dataset A pointer to a valid data set. Do not pass null.

      \return The object id set generated from the given dataset.
    */
    TEDATAACCESSEXPORT ObjectIdSet* GenerateOIDSet(DataSet* dataset);

    /*
      \brief It generates the set of unique ids for every element of the given dataset using a set of attributes.

      \param dataset A pointer to a valid data set.
      \param names   A non empty vector of property names to be used to identify the elements.
     
      \return The object id set generated from the given dataset.
    */
    TEDATAACCESSEXPORT ObjectIdSet* GenerateOIDSet(DataSet* dataset, const std::vector<std::string>& names);

    /*
      \brief It generates the set of unique ids for every element of the given dataset using a set of attributes.

      \param dataset A pointer to a valid data set.
      \param indexes A non empty vector of property indexes to be used to identify the elements.
     
      \return The object id set generated from the given dataset.
    */
    TEDATAACCESSEXPORT ObjectIdSet* GenerateOIDSet(DataSet* dataset, const std::vector<std::size_t>& indexes);

    /*!
      \brief It returns the first dataset spatial property or NULL if none is found.

      \param dataset The dataset to search for a spatial property.

      \return The first dataset spatial property or NULL if none is found.
    */
    TEDATAACCESSEXPORT std::size_t GetFirstSpatialPropertyPos(const te::da::DataSet* dataset);

    TEDATAACCESSEXPORT std::size_t GetFirstPropertyPos(const te::da::DataSet* dataset, int datatype);

    TEDATAACCESSEXPORT std::size_t GetPropertyPos(const DataSet* dataset, const std::string& name);

    //TEDATAACCESSEXPORT te::da::DataSetType* CreateDataSetType(const te::da::DataSet* dataset);

    TEDATAACCESSEXPORT void GetPropertyInfo(const DataSetType* const dt,
                                            std::vector<std::string>& pnames,
                                            std::vector<int>& ptypes);

    TEDATAACCESSEXPORT void GetPropertyInfo(const DataSet* const dataset,
                                            std::vector<std::string>& pnames,
                                            std::vector<int>& ptypes);

    /*!
      \brief It creates the dataset definition in a data source and then fill it with data from the input dataset.

      This function will create the dataset schema definition and will 
      save all the dataset data.

      \param t       The data source transactor.
      \param dt      The source dataset definition.
      \param d       The source dataset data.
      \param limit   The number of items to be used from the input dataset. If set to 0 (default) all items are used.

      \pre All parameters must be valid pointers.

      \post It is the caller responsability to release the dataset 'd' pointer.

      \exception Exception It throws an exception if the dataset can not be created.

      \note DataSetPersistence will start reading the dataset 'd' in the
            current position. So, keep in mind that it is the caller responsability
            to inform the dataset 'd' in the right position (and a valid one) to start processing it.
    */
    TEDATAACCESSEXPORT void Create(DataSourceTransactor* t, DataSetType* dt, DataSet* d, std::size_t limit = 0);

    /*!
      \brief It creates the dataset definition in a data source and then fill it with data from the input dataset.

      This method will create the dataset schema definition and will 
      save all the dataset data.

      \param dt       The source dataset definition.
      \param d        The source dataset data.
      \param options  A list of optional modifiers. It is driver specific.
      \param limit    The number of items to be used from the input dataset. If set to 0 (default) all items are used.

      \pre All parameters must be valid pointers.

      \exception Exception It throws an exception if the dataset can not be created.

      \note It is the caller responsability to release the dataset 'd' pointer.

      \note DataSetPersistence will start reading the dataset 'd' in the
            current position. So, keep in mind that it is the caller responsability
            to inform the dataset 'd' in the right position (and a valid one) to start processing it.
    */
    TEDATAACCESSEXPORT void Create(DataSourceTransactor* t,
                                   DataSetType* dt,
                                   DataSet* d,
                                   const std::map<std::string, std::string>& options,
                                   std::size_t limit = 0);

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_UTILS_H

