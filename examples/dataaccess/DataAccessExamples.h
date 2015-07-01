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
  \file DataAccessExamples.h

  \brief Examples on how to access/manipulate a specified DatasOURCE.
 */

#ifndef __TERRALIB_EXAMPLES_DATAACCESS_INTERNAL_DATAACCESSEXAMPLES_H
#define __TERRALIB_EXAMPLES_DATAACCESS_INTERNAL_DATAACCESSEXAMPLES_H

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype_fw.h>
#include <terralib/dataaccess_fw.h>

//#include <terralib/dataaccess/datasource/datasourceTransactor.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/geometry.h>



//#include "Config.h"

/*
  \brief It creates a DataSetType called 'our_country' using the schema 'public' in the given data source.

  \param transactor The transactor to be used to get the DataSetTypePersistence and persist the modifications

  \return A pointer to the created dataset type.

  \exception Exception It throws an exception if something goes wrong.
 */
te::da::DataSetType* CreateDataSetType(const std::string& datasetname,te::da::DataSetType*dt, te::da::DataSourceTransactor* transactor);

te::da::DataSetType* CreateDataSetTypeInMemory(const std::string& datasettypename);

/*
  \brief It adds a Primary Key to the given dataset type.

  \param datasetname The target dataset name.
  \param transactor  The transactor to be used to persist the modifications

  \return A pointer to the created primary key.

  \pre The dataset type DOES NOT need to be associated to a data source catalog in memory.
  \pre The dataset type must have an attribute called 'gid'.
  \pre The dataset type must not have a primary key.

  \post The dataset type will have an associated primary key.

  \exception Exception It throws an exception if something goes wrong.
 */
te::da::PrimaryKey* AddPrimaryKey(const std::string& datasetname,
                                             te::da::DataSourceTransactor* transactor);

/*
  \brief It adds an Unique Key to the given dataset type.

  \param datasetType The target dataset type.
  \param transactor  The transactor to be used to persist the modifications

  \return A pointer to the created unique key.

  \pre The dataset type DOES NOT need to be associated to a data source catalog in memory.
  \pre The dataset type must have an attribute called 'country_name'.
  \pre The dataset type must have an attribute called 'city_name'.

  \post The dataset type will have an associated unique key.

  \exception Exception It throws an exception if something goes wrong.
 */
te::da::UniqueKey* AddUniqueKey(const std::string& datasetname,
                                           te::da::DataSourceTransactor* transactor);

/*
  \brief It adds a spatial index to the given dataset type.

  \param datasetType The target dataset type.
  \param transactor  The transactor to be used to persist the modifications

  \return A pointer to the created index.

  \pre The dataset type DOES NOT need to be associated to a data source catalog in memory.
  \pre The dataset type must have an attribute called 'spatial_data'.

  \post The dataset type will have an associated index.

  \exception Exception It throws an exception if something goes wrong.
 */
te::da::Index* AddSpatialIndex(const std::string& datasetname,
                                          te::da::DataSourceTransactor* transactor);

/*
  \brief It adds an integer property called 'population' to the given dataset type.

  \param datasetType The target dataset type.
  \param transactor  The transactor to be used to persist the modifications

  \return A pointer to the created property.

  \pre The dataset type DOES NOT need to be associated to a data source catalog in memory.
  \pre The dataset type must be associated to a data source catalog.
  \pre The dataset type must not have an attribute called 'population'.

  \post The dataset type will have an associated property called 'population'.

  \exception Exception It throws an exception if something goes wrong.
 */
te::dt::SimpleProperty* AddProperty(const std::string& datasetname,
                                  te::da::DataSourceTransactor* transactor);

/*!
  \brief This example will print to standard output all information about the datasets stored in the data source.

  \param ds The data source used to retrieve information about its datasets.
 */
void PrintCatalog(te::da::DataSource* ds);

/*!
  \brief It prints datasets in a given data source.

  \param ds The data source you want to print its data.
 */
void PrintDataSets(te::da::DataSource* ds);

/*!
  \brief It prints the data in a given dataset.

  \param dataset Any dataset.
 */
void PrintDataSet(const std::string& datasetName, te::da::DataSet* dataset);

/*!
  \brief An example showing how to retrieve data using a spatial filter.

  \param transactor The transactor to be used to retrieve the data.
 */
void RetrieveUsingSpatialFilter(te::da::DataSource* ds);

/*!
  \brief An example showing how to retrieve the extent of a DataSet.

  \param transactor The transactor to be used to retrieve the data.
 */
void DataSetGetExtent(te::da::DataSourceTransactor* transactor);

/*! \brief It loads the data source drivers. */
void LoadModules();

/*! \brief An example using OGR data source driver to retrieve data from a datafile */
void OGRExampleRead();

/*! \brief An example using OGR data source driver to persist data to a datafile */
void ORGExampleWrite();

/*! \brief An example using PostGIS data source driver. */
void PostGISExample();

/*! \brief An example using GDAL data source driver. */
void GDALExample();

/*! \brief Quering a dataset. */
void QueryExample();

/*! \brief Quering a dataset. */
void QueryExample_2();

/*! \brief Quering Insert clause. */
void QueryInsertExample();

/*! \brief This example shows how to copy a given dataset from one data source to another one. */
void CopyingData();

/*! \brief This example shows how to create DataSet and DataSetType in memory. */
void MemoryExample();

/*! \brief An example using MySQL data source driver. */
void MySQLExample();

/*! \brief An example using SQLite data source driver. */
void SQLiteExample();

/*! 
  \brief It creates a data set in memory.
  
  \return It returns a data set in memory.
  
  \note The caller is responsible for deleting the returned data set.
*/
te::da::DataSet* CreatingDataSetInMemory(const std::string& datasetName);


te::da::DataSet* CreatingDataSetInMemoryGivingDt(te::da::DataSetType* dt);

/*! \brief This example shows how to use the TerraLib Persistence. */
void PersistenceExample();

/*! 
  \brief It copies a data set to a data source.
  
  \param dataSet the dataSet that will be copied to data source.
  \param dsName  the data source name 
  \param dsInfo  the data source info   
*/
void CopyingDataSetToDataSource(te::da::DataSet* dataSet, const std::string& dsName, std::map<std::string, std::string>& dsInfo); 

/*! 
  \brief It adds a new data set property and its values.
  
  \param dataSet the dataSet that will be added.
*/
void AddingDataSetPropertyValues(te::da::DataSet* ds);

/*! 
  \brief It removes a property from the data set type and from the associated data source.
  
  \param prop   the property that will be removed. 
  \param trans  the transactor to be used in the removing. 
*/
//void DroppingDataSetTypeProperty(te::dt::Property* prop, te::da::DataSourceTransactor* trans);
void DroppingDataSetTypeProperty(const std::string& datasetname, const std::string &propname, te::da::DataSourceTransactor* trans);

/*! 
  \brief It removes a data set type from the associated data source.
  
  \param dt     the dataSetType that will be removed from data source.
  \param trans  the transactor to be used in the removing. 
*/
//void DroppingDataSetType(te::da::DataSetType* dt, te::da::DataSourceTransactor* trans);
void DroppingDataSetType(const std::string& datasetname, te::da::DataSourceTransactor* trans);

/*!
  \brief It retrieves and prints the capabilities of a given data source.

  \param ds A valid data source.
*/
void PrintDataSourceCapabilities(te::da::DataSource* ds);

/*! \brief DataSet Adapter example. */
void DataSetAdapter();

/*! \brief ObjectId example. */
void ObjectId();

void ObjectId_query();

void ObjectId3();

#endif  // __TERRALIB_EXAMPLES_DATAACCESS_INTERNAL_DATAACCESSEXAMPLES_H
