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
  \file terralib/dataaccess/utils/Utils.cpp

  \brief Utility functions for the data access module.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/GeometryProperty.h"
#include "../dataset/DataSet.h"
#include "../dataset/DataSetPersistence.h"
#include "../dataset/DataSetType.h"
#include "../dataset/DataSetTypePersistence.h"
#include "../dataset/ObjectId.h"
#include "../dataset/ObjectIdSet.h"
#include "../dataset/PrimaryKey.h"
#include "../dataset/UniqueKey.h"
#include "../datasource/DataSourceInfoManager.h"
#include "../datasource/DataSourceManager.h"
#include "../datasource/DataSourceCatalogLoader.h"
#include "../datasource/DataSourceTransactor.h"
#include "../Enums.h"
#include "../Exception.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

void te::da::LoadFull(te::da::DataSetType* dataset, const std::string& datasourceId)
{
  assert(dataset);
  assert(!datasourceId.empty());

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    return;

  LoadFull(dataset, datasource.get());
}

void te::da::LoadFull(DataSetType* dataset, DataSource* datasource)
{
  assert(dataset);
  assert(datasource);

  std::auto_ptr<DataSourceTransactor> transactor(datasource->getTransactor());

  LoadFull(dataset, transactor.get());
}

void te::da::LoadFull(DataSetType* dataset, DataSourceTransactor* transactor)
{
  assert(dataset);
  assert(transactor);

  std::auto_ptr<DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  LoadFull(dataset, cloader.get());
}

void te::da::LoadFull(DataSetType* dataset, DataSourceCatalogLoader* cloader)
{
  assert(dataset);
  assert(cloader);

  if(dataset->size() == 0)
    cloader->getProperties(dataset);

  cloader->getCheckConstraints(dataset);

  cloader->getIndexes(dataset);

  cloader->getUniqueKeys(dataset);

  cloader->getPrimaryKey(dataset);

  dataset->setFullLoaded(true);
}

void te::da::LoadProperties(te::da::DataSetType* dataset, const std::string& datasourceId)
{
  assert(dataset);
  assert(!datasourceId.empty());

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    return;

  LoadProperties(dataset, datasource.get());
}

void te::da::LoadProperties(te::da::DataSetType* dataset, te::da::DataSource* datasource)
{
  assert(dataset);
  assert(datasource);

  std::auto_ptr<DataSourceTransactor> transactor(datasource->getTransactor());

  LoadProperties(dataset, transactor.get());
}

void te::da::LoadProperties(te::da::DataSetType* dataset, te::da::DataSourceTransactor* transactor)
{
  assert(dataset);
  assert(transactor);

  std::auto_ptr<DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  cloader->getProperties(dataset);
}

te::gm::Envelope* te::da::GetExtent(te::da::DataSet* dataset)
{
  std::size_t pos = GetFirstSpatialPropertyPos(dataset);

  if(pos != std::string::npos)
    return dataset->getExtent(pos);

  return new te::gm::Envelope;
}

te::gm::Envelope* te::da::GetExtent(te::gm::GeometryProperty* gp, const std::string& datasourceId)
{
  assert(gp);
  assert(!datasourceId.empty());

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    throw Exception(TR_DATAACCESS("Could not retrieve data source in order to search for a property extent!"));

  return GetExtent(gp, datasource.get());
}

te::gm::Envelope* te::da::GetExtent(te::gm::GeometryProperty* gp, te::da::DataSource* datasource)
{
  assert(gp);
  assert(datasource);

  std::auto_ptr<DataSourceTransactor> transactor(datasource->getTransactor());

  return GetExtent(gp, transactor.get());
}

te::gm::Envelope* te::da::GetExtent(te::gm::GeometryProperty* gp, te::da::DataSourceTransactor* transactor)
{
  assert(gp);
  assert(transactor);

  std::auto_ptr<DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  te::gm::Envelope* mbr = cloader->getExtent(gp);

  return mbr;
}

void te::da::GetDataSets(boost::ptr_vector<std::string>& datasets, const std::string& datasourceId)
{
  assert(!datasourceId.empty());

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    return;

  GetDataSets(datasets, datasource.get());
}

void te::da::GetDataSets(boost::ptr_vector<std::string>& datasets, te::da::DataSource* datasource)
{
  assert(datasource);

  std::auto_ptr<te::da::DataSourceTransactor> transactor(datasource->getTransactor());

  GetDataSets(datasets, transactor.get());
}

void te::da::GetDataSets(boost::ptr_vector<std::string>& datasets, te::da::DataSourceTransactor* transactor)
{
  assert(transactor);

  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  cloader->getDataSets(datasets);
}

std::string te::da::GetCategoryName(int category)
{
  switch(category)
  {
    case te::da::UNKNOWN_DATASET_TYPE:
      return "Unknown";

    case te::da::TABLE_TYPE:
      return "Table";

    case te::da::SYSTEM_TABLE_TYPE:
      return "System";

    case te::da::VIEW_TYPE:
      return "View";

    case te::da::QUERY_TYPE:
      return "Query";

    case te::da::INDEX_TYPE:
      return "Index";

    case te::da::SEQUENCE_TYPE:
      return "Sequence";

    case te::da::TRIGGER_TYPE:
      return "Trigger";

    case te::da::REGULAR_FILE_TYPE:
      return "Regular File";

    default:
      return "";
  }
}

bool te::da::HasDataSet(const std::string& datasourceId)
{
  assert(!datasourceId.empty());

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    return false;

  return HasDataSet(datasource.get());
}

bool te::da::HasDataSet(te::da::DataSource* datasource)
{
  assert(datasource);

  std::auto_ptr<te::da::DataSourceTransactor> transactor(datasource->getTransactor());

  return HasDataSet(transactor.get());
}

bool te::da::HasDataSet(te::da::DataSourceTransactor* transactor)
{
  assert(transactor);

  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  return cloader->hasDataSets();
}

te::da::DataSet* te::da::GetDataSet(const std::string& name, const std::string& datasourceId)
{
  assert(!datasourceId.empty());

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    return false;

  return GetDataSet(name, datasource.get());
}

te::da::DataSet* te::da::GetDataSet(const std::string& name, te::da::DataSource* datasource)
{
  assert(datasource);

  std::auto_ptr<te::da::DataSourceTransactor> transactor(datasource->getTransactor());

  return transactor->getDataSet(name);
}

te::da::DataSourcePtr te::da::GetDataSource(const std::string& datasourceId, const bool opened)
{
  assert(!datasourceId.empty());

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
  {
    DataSourceInfoPtr dsinfo = te::da::DataSourceInfoManager::getInstance().get(datasourceId);

    if(dsinfo.get() == 0)
      throw Exception(TR_DATAACCESS("Could not find data source!"));

    datasource = te::da::DataSourceManager::getInstance().make(datasourceId, dsinfo->getAccessDriver());

    datasource->setConnectionInfo(dsinfo->getConnInfo());
  }

  if(opened && !datasource->isOpened())
    datasource->open();

  return datasource;
}

te::da::ObjectIdSet* te::da::GenerateOIDSet(te::da::DataSet* dataset)
{
  //assert(dataset);

  //const DataSetType* type = dataset->getType();
  //assert(type);

  //if(!type->isFullLoaded())
  //  dataset->loadTypeInfo();

  //// A vector with the property indexes that will be used to generate the unique ids
  //std::vector<std::size_t> oidprops;

  //// Try to use the primary key properties
  //PrimaryKey* pk = type->getPrimaryKey();
  //if(pk != 0)
  //{
  //  const std::vector<te::dt::Property*>& pkProperties = pk->getProperties();

  //  for(std::size_t i = 0; i < pkProperties.size(); ++i)
  //    oidprops.push_back(type->getPropertyPosition(pkProperties[i]->getName()));

  //  return te::da::GenerateOIDSet(dataset, oidprops);
  //}

  //// Try to use the unique key properties
  //if(type->getNumberOfUniqueKeys() > 0)
  //{
  //  for(std::size_t i = 0; i < type->getNumberOfUniqueKeys(); ++i)
  //  {
  //    UniqueKey* uk = type->getUniqueKey(i);

  //    const std::vector<te::dt::Property*>& ukProperties = uk->getProperties();

  //    for(std::size_t j = 0; j < ukProperties.size(); ++j)
  //      oidprops.push_back(type->getPropertyPosition(ukProperties[j]->getName()));
  //  }
  //  
  //  return te::da::GenerateOIDSet(dataset, oidprops);
  //}
  //
  //// Here, the data set do not have primary key properties or unique key properties. 
  //// So, use all the non geometric properties.
  //const std::vector<te::dt::Property*>& props = type->getProperties();
  //for(std::size_t i = 0; i < props.size(); ++i)
  //{
  //  if (props[i]->getType() == te::dt::GEOMETRY_TYPE ||
  //      props[i]->getType() == te::dt::RASTER_TYPE)
  //    continue;
  //  oidprops.push_back(i);
  //}

  //return te::da::GenerateOIDSet(dataset, oidprops);
  return 0;
}

te::da::ObjectIdSet* te::da::GenerateOIDSet(te::da::DataSet* dataset, const std::vector<std::string>& names)
{
  //assert(dataset);
  //assert(!names.empty());
  //
  //const DataSetType* type = dataset->getType();
  //assert(type);

  //std::vector<std::size_t> indexes;

  //for(std::size_t i = 0; i < names.size(); ++i)
  //  indexes.push_back(type->getPropertyPosition(names[i]));

  //return te::da::GenerateOIDSet(dataset, indexes);
  return 0;
}

te::da::ObjectIdSet* te::da::GenerateOIDSet(te::da::DataSet* dataset, const std::vector<std::size_t>& indexes)
{
  //assert(dataset);
  //assert(!indexes.empty());
  //
  //const DataSetType* type = dataset->getType();
  //assert(type);
  //
  //ObjectIdSet* oids = new ObjectIdSet(type);
  //oids->setProperties(indexes);

  //while(dataset->moveNext())
  //{
  //  ObjectId* oid = new ObjectId;

  //  for(std::size_t i = 0; i < indexes.size(); ++i)
  //    oid->addValue(dataset->getValue(indexes[i]));

  //  oids->add(oid);
  //}

  //return oids;
  return 0;
}

std::size_t te::da::GetFirstSpatialPropertyPos(const te::da::DataSet* dataset)
{
  assert(dataset);

  const std::size_t np = dataset->getNumProperties();

  for(std::size_t i = 0; i != np; ++i)
  {
    int pdt = dataset->getPropertyDataType(i);

    if(pdt == te::dt::GEOMETRY_TYPE || pdt == te::dt::RASTER_TYPE)
    {
      return i;
    }
  }

  return std::string::npos;
}

std::size_t te::da::GetFirstPropertyPos(const te::da::DataSet* dataset, int datatype)
{
  assert(dataset);

  const std::size_t np = dataset->getNumProperties();

  for(std::size_t i = 0; i != np; ++i)
  {
    int pdt = dataset->getPropertyDataType(i);

    if(pdt == datatype)
    {
      return i;
    }
  }

  return std::string::npos;
}

std::size_t te::da::GetPropertyPos(const DataSet* dataset, const std::string& name)
{
  assert(dataset);

  const std::size_t np = dataset->getNumProperties();

  for(std::size_t i = 0; i != np; ++i)
  {
    std::string pname = dataset->getPropertyName(i);

    if(pname == name)
      return i;
  }

  return std::string::npos;
}

//te::da::DataSetType* te::da::CreateDataSetType(const te::da::DataSet* dataset)
//{
//  assert(dataset);
//
//  te::da::DataSetType* dt = new DataSetType("");
//
//  const std::size_t np = dataset->getNumProperties();
//
//  for(std::size_t i = 0; i != np; ++i)
//  {
//    const te::dt::Property* p = dataset->getProperty(i);
//
//    dt->add(p->clone());
//  }
//
//  return dt;
//}

void te::da::GetPropertyInfo(const DataSetType* const dt,
                             std::vector<std::string>& pnames,
                             std::vector<int>& ptypes)
{
  assert(dt);

  for(std::size_t i = 0; i != dt->size(); ++i)
  {
    const te::dt::Property* p = dt->getProperty(i);

    pnames.push_back(p->getName());
    ptypes.push_back(p->getType());
  }
}

void te::da::GetPropertyInfo(const DataSet* const dataset,
                             std::vector<std::string>& pnames,
                             std::vector<int>& ptypes)
{
  assert(dataset);

  for(std::size_t i = 0; i != dataset->getNumProperties(); ++i)
  {
    pnames.push_back(dataset->getPropertyName(i));
    ptypes.push_back(dataset->getPropertyDataType(i));
  }
}

void te::da::Create(DataSourceTransactor* t, DataSetType* dt, DataSet* d, std::size_t limit)
{
  std::map<std::string, std::string> options;

  Create(t, dt, d, options, limit);
}

void te::da::Create(DataSourceTransactor* t,
                    DataSetType* dt,
                    DataSet* d,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit)
{
  std::auto_ptr<te::da::DataSetTypePersistence> dtp(t->getDataSetTypePersistence());

  dtp->create(dt);

  std::auto_ptr<te::da::DataSetPersistence> dp(t->getDataSetPersistence());

  dp->add(dt->getName(), d, options, limit);
}

