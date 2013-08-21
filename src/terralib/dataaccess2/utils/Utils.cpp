/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
#include "../../raster/RasterProperty.h"
#include "../dataset/DataSet.h"
#include "../dataset/DataSetAdapter.h"
#include "../dataset/DataSetType.h"
#include "../dataset/DataSetTypeConverter.h"
#include "../dataset/ObjectId.h"
#include "../dataset/ObjectIdSet.h"
#include "../dataset/PrimaryKey.h"
#include "../dataset/UniqueKey.h"
#include "../datasource/DataSourceInfoManager.h"
#include "../datasource/DataSourceManager.h"
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

  //DataSourcePtr datasource(te::da::DataSourceManager::getInstance().get(datasourceId));
  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    return;

  if(dataset->size() == 0)
  {
    boost::ptr_vector<te::dt::Property> properties = datasource->getProperties(dataset->getName());

    dataset->add(properties);
  }

  //datasource->getCheckConstraints(dataset);

  //cloader->getIndexes(dataset);

  //cloader->getUniqueKeys(dataset);

  //cloader->getPrimaryKey(dataset);
}

//void te::da::LoadFull(te::da::DataSetType* dataset, const std::string& datasourceId)
//{
//  assert(dataset);
//  assert(!datasourceId.empty());
//
//  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));
//
//  if(datasource.get() == 0)
//    return;
//
//  LoadFull(dataset, datasource.get());
//}

//void te::da::LoadFull(DataSetType* dataset, DataSource* datasource)
//{
//  assert(dataset);
//  assert(datasource);
//
//  std::auto_ptr<DataSourceTransactor> transactor(datasource->getTransactor());
//
//  LoadFull(dataset, transactor.get());
//}

void te::da::LoadProperties(te::da::DataSetType* dataset, const std::string& datasourceId)
{
  assert(dataset);
  assert(!datasourceId.empty());

  //DataSourcePtr datasource(te::da::DataSourceManager::getInstance().get(datasourceId));
  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    return;

  boost::ptr_vector<te::dt::Property> properties = datasource->getProperties(dataset->getName());

  dataset->add(properties);
}

te::gm::Envelope* te::da::GetExtent(const std::string& datasetName,
                                    const std::string& propertyName,
                                    const std::string& datasourceId)
{
  assert(!datasourceId.empty());

  //DataSourcePtr datasource(te::da::DataSourceManager::getInstance().get(datasourceId));
  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    throw Exception(TR_DATAACCESS("Could not retrieve data source in order to search for a property extent!"));

  std::auto_ptr<te::gm::Envelope> mbr(datasource->getExtent(datasetName, propertyName));

  return mbr.release();
}

void te::da::GetDataSets(std::vector<std::string>& datasets, const std::string& datasourceId)
{
  assert(!datasourceId.empty());

  //DataSourcePtr datasource(te::da::DataSourceManager::getInstance().get(datasourceId));

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));
  if(datasource.get() == 0)
    return;

  datasets = datasource->getDataSetNames();
}

std::string te::da::GetDataSetCategoryName(int category)
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

  //DataSourcePtr datasource(te::da::DataSourceManager::getInstance().get(datasourceId));
  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));

  if(datasource.get() == 0)
    return false;

  return datasource->hasDataSets();
}

te::da::DataSet* te::da::GetDataSet(const std::string& name, const std::string& datasourceId)
{
  assert(!datasourceId.empty());

  //DataSourcePtr datasource(te::da::DataSourceManager::getInstance().get(datasourceId));

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));
  if(datasource.get() == 0)
    return false;

  std::auto_ptr<DataSet> dataset(datasource->getDataSet(name));

  return dataset.release();
}

//te::da::DataSourcePtr te::da::GetDataSource(const std::string& datasourceId)
//{
//  assert(!datasourceId.empty());
//
//  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().get(datasourceId));
//
//  if(datasource.get() == 0)
//  {
//    DataSourceInfoPtr dsinfo = te::da::DataSourceInfoManager::getInstance().get(datasourceId);
//
//    if(dsinfo.get() == 0)
//      throw Exception(TR_DATAACCESS("Could not find data source!"));
//
//    te::da::DataSourceManager::getInstance().open(datasourceId, dsinfo->getAccessDriver(), dsinfo->getConnInfo());
//
//    datasource = te::da::DataSourceManager::getInstance().get(datasourceId);
//  }
//
//  return datasource;
//}

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

void te::da::GetEmptyOIDSet(const te::da::DataSetType* type, te::da::ObjectIdSet*& set)
{
  assert (type);

  std::vector<size_t> ppos;
  std::vector<int> ptypes;
  std::vector<std::string> pnames;
  std::vector<size_t>::iterator it;
  set = new ObjectIdSet();

  GetOIDPropertyPos(type, ppos);
  
  for(it=ppos.begin(); it!=ppos.end(); ++it)
  {
    te::dt::Property* prop = type->getProperty(*it); 
    ptypes.push_back(prop->getType());
    pnames.push_back(prop->getName());
  }

  for(size_t i=0; i<ppos.size(); i++)
    set->addProperty(pnames[i], ppos[i], ptypes[i]);
}

void te::da::GetOIDPropertyNames(const te::da::DataSetType* type, std::vector<std::string>& pnames)
{
  assert(type);

  // Looking for the primary key properties
  PrimaryKey* pk = type->getPrimaryKey();
  if(pk != 0)
  {
    const std::vector<te::dt::Property*>& pkProperties = pk->getProperties();

    for(std::size_t i = 0; i < pkProperties.size(); ++i)
      pnames.push_back(pkProperties[i]->getName());

    return;
  }

  // Looking for the unique key properties
  if(type->getNumberOfUniqueKeys() > 0)
  {
    for(std::size_t i = 0; i < type->getNumberOfUniqueKeys(); ++i)
    {
      UniqueKey* uk = type->getUniqueKey(i);

      const std::vector<te::dt::Property*>& ukProperties = uk->getProperties();

      for(std::size_t j = 0; j < ukProperties.size(); ++j)
        pnames.push_back(ukProperties[j]->getName());
    }

    return;
  }
  
  // Here, the data set type do not have primary key properties or unique key properties.
  // So, use all the non geometric properties.
  const std::vector<te::dt::Property*>& props = type->getProperties();
  for(std::size_t i = 0; i < props.size(); ++i)
  {
    if(props[i]->getType() == te::dt::GEOMETRY_TYPE || props[i]->getType() == te::dt::RASTER_TYPE)
      continue;

    pnames.push_back(props[i]->getName());
  }
}

void te::da::GetOIDPropertyPos(const te::da::DataSetType* type, std::vector<std::size_t>& ppos)
{
  assert(type);

  std::vector<std::string> oidprops;
  GetOIDPropertyNames(type, oidprops);

  for(std::size_t i = 0; i < oidprops.size(); ++i)
    ppos.push_back(type->getPropertyPosition(oidprops[i]));
}

te::da::ObjectIdSet* te::da::GenerateOIDSet(te::da::DataSet* dataset, const te::da::DataSetType* type)
{
  assert(dataset);
  assert(type);

  std::vector<std::string> oidprops;
  GetOIDPropertyNames(type, oidprops);

  return te::da::GenerateOIDSet(dataset, oidprops);
}

te::da::ObjectIdSet* te::da::GenerateOIDSet(te::da::DataSet* dataset, const std::vector<std::string>& names)
{
  assert(dataset);
  assert(!names.empty());

  ObjectIdSet* oids = new ObjectIdSet;

  for(std::size_t i = 0; i < names.size(); ++i)
  {
    std::size_t pos = GetPropertyPos(dataset, names[i]);
    assert(pos != std::string::npos);
    oids->addProperty(names[i], pos, dataset->getPropertyDataType(pos));
  }

  while(dataset->moveNext())
    oids->add(GenerateOID(dataset, names));

  return oids;
}

te::da::ObjectId* te::da::GenerateOID(te::da::DataSet* dataset, const std::vector<std::string>& names)
{
  assert(dataset);
  assert(!names.empty());

  ObjectId* oid = new ObjectId;

  for(std::size_t i = 0; i < names.size(); ++i)
  {
    if(!dataset->isNull(i))
      oid->addValue(dataset->getValue(names[i]).release());
  }

  return oid;
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

te::dt::Property* te::da::GetFirstSpatialProperty(const DataSetType* dt)
{
  assert(dt);

  const std::size_t np = dt->size();

  for(std::size_t i = 0; i != np; ++i)
  {
    te::dt::Property* p = dt->getProperty(i);

    assert(p);

    int pdt = p->getType();

    if(pdt == te::dt::GEOMETRY_TYPE || pdt == te::dt::RASTER_TYPE)
    {
      return p;
    }
  }

  return 0;
}

te::gm::GeometryProperty* te::da::GetFirstGeomProperty(const DataSetType* dt)
{
  te::dt::Property* p = dt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);

  assert(p);

  return static_cast<te::gm::GeometryProperty*>(p);
}

te::rst::RasterProperty* te::da::GetFirstRasterProperty(const DataSetType* dt)
{
  te::dt::Property* p = dt->findFirstPropertyOfType(te::dt::RASTER_TYPE);

  assert(p);

  return static_cast<te::rst::RasterProperty*>(p);
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

void te::da::Create(DataSource* ds, DataSetType* dt, DataSet* d, std::size_t limit)
{
  std::map<std::string, std::string> options;

  Create(ds, dt, d, options, limit);
}

void te::da::Create(DataSource* ds,
                    DataSetType* dt,
                    DataSet* d,
                    const std::map<std::string, std::string>& options,
                    std::size_t limit)
{
  ds->createDataSet(dt, options);

  ds->add(dt->getName(), d, options, limit);
}

te::da::DataSetAdapter* te::da::CreateAdapter(DataSet* ds, DataSetTypeConverter* converter, bool isOwner)
{
  assert(ds);
  assert(converter);

  DataSetType* type = converter->getResult();
  assert(type);

  const std::vector<std::vector<std::size_t> >& indexes = converter->getConvertedPropertyIndexes();
  const std::vector<AttributeConverter>& funcs = converter->getConverters();
  
  assert((type->size() == indexes.size()) && (type->size() == funcs.size()));

  DataSetAdapter* adapter = new DataSetAdapter(ds, isOwner);

  for(std::size_t i = 0; i < type->size(); ++i)
  {
    te::dt::Property* p = type->getProperty(i);
    assert(p);

    adapter->add(p->getName(), p->getType(), indexes[i], funcs[i]);
  }

  return adapter;
}

std::string te::da::GetSQLValueNames(const te::da::DataSetType* dt)
{
  std::string valueNames("(");

  const std::size_t np = dt->size();

  for(std::size_t i = 0; i != np; ++i)
  {
    if(i != 0)
      valueNames += ",";

    valueNames += dt->getProperty(i)->getName();
  }

  valueNames += ")";

  return valueNames;
}

std::string te::da::GetSQLValueNames(const te::da::DataSet* dataset)
{
  std::string valueNames("(");

  const std::size_t np = dataset->getNumProperties();

  for(std::size_t i = 0; i != np; ++i)
  {
    if(i != 0)
      valueNames += ",";

    valueNames += dataset->getPropertyName(i);
  }

  valueNames += ")";

  return valueNames;
}

std::vector<int> te::da::GetPropertyDataTypes(const te::da::DataSet* dataset)
{
  std::vector<int> properties;

  const std::size_t np = dataset->getNumProperties();

  for(std::size_t i = 0; i != np; ++i)
  {
    properties.push_back(dataset->getPropertyDataType(i));
  }

  return properties;
}

