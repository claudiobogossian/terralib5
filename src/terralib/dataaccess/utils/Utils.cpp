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
#include "../../common/StringUtils.h"
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
#include "../query/DataSetName.h"
#include "../query/Field.h"
#include "../query/Fields.h"
#include "../query/LiteralEnvelope.h"
#include "../query/LiteralGeom.h"
#include "../query/PropertyName.h"
#include "../query/ST_Contains.h"
#include "../query/ST_Crosses.h"
#include "../query/ST_Disjoint.h"
#include "../query/ST_Equals.h"
#include "../query/ST_Intersects.h"
#include "../query/ST_Overlaps.h"
#include "../query/ST_Touches.h"
#include "../query/ST_Within.h"
#include "../query/Where.h"
#include "../Enums.h"
#include "../Exception.h"
#include "../../maptools/QueryLayer.h"
#include "Utils.h"

// STL
#include <cassert>
#include <algorithm>

//BOOST
#include <boost/algorithm/string.hpp>
#include <boost/math/special_functions/round.hpp>

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
    throw Exception(TE_TR("Could not retrieve data source in order to search for a property extent!"));

  std::auto_ptr<te::gm::Envelope> mbr(datasource->getExtent(datasetName, propertyName));

  return mbr.release();
}

void te::da::GetDataSetNames(std::vector<std::string>& datasetNames, const std::string& datasourceId)
{
  assert(!datasourceId.empty());

  DataSourcePtr ds(te::da::DataSourceManager::getInstance().find(datasourceId));
  if(ds.get() == 0)
    return;

  datasetNames = ds->getDataSetNames();
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

te::da::DataSetType* te::da::GetDataSetType(const std::string& name, const std::string& datasourceId)
{
  assert(!datasourceId.empty());

  DataSourcePtr datasource(te::da::DataSourceManager::getInstance().find(datasourceId));
  if(datasource.get() == 0)
    return false;

  std::auto_ptr<DataSetType> datasettype(datasource->getDataSetType(name));

  return datasettype.release();
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
//      throw Exception(TE_TR("Could not find data source!"));
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
      throw Exception(TE_TR("Could not find data source!"));

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
    if(props[i]->getType() == te::dt::GEOMETRY_TYPE || props[i]->getType() == te::dt::RASTER_TYPE ||
       props[i]->getType() == te::dt::FLOAT_TYPE || props[i]->getType() == te::dt::DOUBLE_TYPE ||
       props[i]->getType() == te::dt::BYTE_ARRAY_TYPE || props[i]->getType() == te::dt::NUMERIC_TYPE)
      continue;

    pnames.push_back(props[i]->getName());
  }
}

void te::da::GetOIDDatasetProps(const DataSetType* type, std::pair<std::string, int>& dsProps)
{
  assert(type);
  std::map<std::string, std::pair<int, int> > dsNames;
  std::map<std::string, std::pair<int, int> >::const_iterator dsNamesIt;

  //Acquiring the primary key of the given DataSetType
  std::vector<te::dt::Property*> props = type->getPrimaryKey()->getProperties();

  //Getting the name and the number of properties of each dataset involved
  for(size_t i = 0; i < props.size(); ++i)
  {
    te::dt::Property* pRef =props[i];
    assert(pRef);
    dsNames[pRef->getDatasetName()].first = dsNames.size();
    dsNames[pRef->getDatasetName()].second++;
  }

  //Getting the name and the number of properties of the base dataset
  for(dsNamesIt = dsNames.begin(); dsNamesIt !=  dsNames.end(); ++dsNamesIt)
  {
    if((*dsNamesIt).second.first == 0)
    {
      dsProps.first = (*dsNamesIt).first;
      dsProps.second = (*dsNamesIt).second.second;
      break;
    }
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
    
    if(pos == std::string::npos)
      throw Exception(TE_TR("Primary Key ") + names[i] + TE_TR(" not found!"));
    
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
    if(!dataset->isNull(names[i]))
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

    if(boost::iequals(pname, name))
      return i;
  }

  return std::string::npos;
}

std::size_t te::da::GetPropertyPos(const DataSetType* dt, const std::string& name)
{
  assert(dt);

  const std::size_t np = dt->getProperties().size();

  for(std::size_t i = 0; i != np; ++i)
  {
    std::string pname = dt->getProperty(i)->getName();

    if(boost::iequals(pname, name))
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

  if(p)
  {
    return static_cast<te::gm::GeometryProperty*>(p);
  }
  else
  {
    return 0;
  }
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

std::auto_ptr<te::da::Expression> te::da::BuildSpatialOp(Expression* e1,
                                                         Expression* e2,
                                                         te::gm::SpatialRelation r)

{
  std::auto_ptr<te::da::Expression> op;
  switch(r)
  {
    case te::gm::INTERSECTS:
      op.reset(new ST_Intersects(e1, e2));
    break;

    case te::gm::DISJOINT:
      op.reset(new ST_Disjoint(e1, e2));
    break;

    case te::gm::TOUCHES:
      op.reset(new ST_Touches(e1, e2));
    break;

    case te::gm::OVERLAPS:
      op.reset(new ST_Overlaps(e1, e2));
    break;

    case te::gm::CROSSES:
      op.reset(new ST_Crosses(e1, e2));
    break;

    case te::gm::WITHIN:
      op.reset(new ST_Within(e1, e2));
    break;

    case te::gm::CONTAINS:
      op.reset(new ST_Contains(e1, e2));
    break;

    case te::gm::EQUALS:
      op.reset(new ST_Equals(e1, e2));
    break;

    default:
      throw;
  }

  return op;
}

std::auto_ptr<te::da::Fields> te::da::BuildFields(const std::vector<std::string>& properties)
{
  std::auto_ptr<Fields> fields(new Fields);

  for(std::size_t i = 0; i < properties.size(); ++i)
    fields->push_back(new te::da::Field(properties[i]));

  return fields;
}

std::auto_ptr<te::da::Select> te::da::BuildSelect(const std::string& dsname)
{
  return BuildSelect(dsname, "*");
}

std::auto_ptr<te::da::Select> te::da::BuildSelect(const std::string& dsname, const std::string& propertyName)
{
  std::vector<std::string> p;
  p.push_back(propertyName);

  return BuildSelect(dsname, p);
}

std::auto_ptr<te::da::Select> te::da::BuildSelect(const std::string& dsname, const std::vector<std::string>& properties)
{
  // Fields
  std::auto_ptr<Fields> fields = BuildFields(properties);

  // From
  FromItem* fi = new DataSetName(dsname);
  From* from = new From;
  from->push_back(fi);

  // Select
  std::auto_ptr<Select> select(new Select(fields.release(), from));

  return select;
}

std::auto_ptr<te::da::Select> te::da::BuildSelect(const std::string& dsname,
                                                  const std::vector<std::string>& properties,
                                                  const std::string& geometryProperty,
                                                  const te::gm::Envelope* e,
                                                  int srid,
                                                  te::gm::SpatialRelation r)
{
  // Fields
  std::auto_ptr<Fields> fields = BuildFields(properties);

  // Adding the geometry property
  fields->push_back(new Field(geometryProperty));

  // From
  FromItem* fi = new DataSetName(dsname);
  From* from = new From;
  from->push_back(fi);

  PropertyName* geomPropertyName = new PropertyName(geometryProperty);
  LiteralEnvelope* lenv = new LiteralEnvelope(*e, srid);

  // The spatial restriction
  std::auto_ptr<Expression> spatialOp = BuildSpatialOp(geomPropertyName, lenv, r);

  // Where
  te::da::Where* filter = new Where(spatialOp.release());

  // Select
  std::auto_ptr<Select> select(new Select(fields.release(), from, filter));

  return select;
}

std::auto_ptr<te::da::Select> te::da::BuildSelect(const std::string& dsname,
                                                  const std::vector<std::string>& properties,
                                                  const std::string& geometryProperty,
                                                  te::gm::Geometry* g,
                                                  te::gm::SpatialRelation r)
{
  // Fields
  std::auto_ptr<Fields> fields = BuildFields(properties);

  // Adding the geometry property
  fields->push_back(new Field(geometryProperty));

  // From
  FromItem* fi = new DataSetName(dsname);
  From* from = new From;
  from->push_back(fi);

  PropertyName* geomPropertyName = new PropertyName(geometryProperty);
  LiteralGeom* lgeom = new LiteralGeom(g);

  // The spatial restriction
  std::auto_ptr<Expression> spatialOp = BuildSpatialOp(geomPropertyName, lgeom, r);

  // Where
  te::da::Where* filter = new Where(spatialOp.release());

  // Select
  std::auto_ptr<Select> select(new Select(fields.release(), from, filter));

  return select;
}

std::auto_ptr<te::da::Select> te::da::BuildSelect(const std::string& dsname,
                                                  const std::vector<std::string>& properties,
                                                  const ObjectIdSet* oids)
{
  // OIDS restriction
  Expression* exp = oids->getExpression();
  assert(exp);

  // Where
  Where* filter = new Where(exp);

  // Fields
  std::auto_ptr<Fields> fields = BuildFields(properties);

  // Adding the oids properties case not included
  const std::vector<std::string>& oidsProperties = oids->getPropertyNames();
  for(std::size_t i = 0; i < oidsProperties.size(); ++i)
  {
    const std::string& oidPropertyName = oidsProperties[i];

    bool alreadyIncluded = false;

    for(std::size_t j = 0; j < properties.size(); ++j)
    {
      if(oidPropertyName == properties[j])
      {
        alreadyIncluded = true;
        break;
      }
    }

    if(!alreadyIncluded)
      fields->push_back(new Field(oidPropertyName));
  }

  // From
  FromItem* fromItem = new DataSetName(dsname);
  From* from = new From;
  from->push_back(fromItem);

  // Select
  std::auto_ptr<Select> select(new Select(fields.release(), from, filter));

  return select;
}

int te::da::GetPropertyIndex(te::da::DataSet* dataSet, const std::string propName)
{
  int index = 0;
  
  for(std::size_t i = 0; i < dataSet->getNumProperties(); ++i)
  {
    if(propName == dataSet->getPropertyName(i))
    {
      index = i;
      return index;
    }
  }
  return -1;
}

bool te::da::IsValidName(const std::string& name, std::string& invalidChar)
{
  if(name.empty())
  {
    return false;
  }

  if(name[0] >= 0x30 && name[0] <= 0x39)
  {
    invalidChar = "begin with a numeric character\n";
    return false;
  }
  if(name[0] == '_')
  {
    invalidChar += "begin with a invalid character: underscore _\n";
    return false;
  }

  int ff = name.find(" ");
  if(ff >= 0)
  {
    invalidChar += "invalid character: blank space\n";
    return false;
  }

  ff = name.find(".");
  if(ff >= 0)
  {
    invalidChar += "invalid character: dot '.'\n";
    return false;
  }

  ff = name.find("*");
  if(ff >= 0)
  {
    invalidChar += "invalid character: mathematical symbol '*'\n";
    return false;
  }

  ff = name.find("/");
  if(ff >= 0)
  {
    invalidChar += "invalid character: mathematical symbol '/'\n";
    return false;
  }

  ff = name.find("(");
  if(ff >= 0)
  {
    invalidChar += "invalid character: parentheses '('\n";
    return false;
  }

  ff = name.find(")");
  if(ff >= 0)
  {
    invalidChar += "invalid character: parentheses ')'\n";
    return false;
  }

  ff = name.find("-");
  if(ff >= 0)
  {
    invalidChar += "invalid character: mathematical symbol '-'\n";
    return false;
  }

  ff = name.find("+");
  if(ff >= 0)
  {
    invalidChar += "invalid character: mathematical symbol '+'\n";
    return false;
  }

  ff = name.find("%");
  if(ff >= 0)
  {
    invalidChar += "invalid character: mathematical symbol '%'\n";
    return false;
  }

  ff = name.find(">");
  if(ff >= 0)
  {
    invalidChar += "invalid character: mathematical symbol '>'\n";
    return false;
  }

  ff = name.find("<");
  if(ff >= 0)
  {
    invalidChar += "invalid character: mathematical symbol '<'\n";
    return false;
  }

  ff = name.find("&");
  if(ff >= 0)
  {
    invalidChar += "invalid character: mathematical symbol '&'\n";
    return false;
  }

  ff = name.find("$");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '$'\n";
    return false;
  }

  ff = name.find(";");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: ';'\n";
    return false;
  }

  ff = name.find("=");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '='\n";
    return false;
  }

  ff = name.find("!");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '!'\n";
    return false;
  }

  ff = name.find("?");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '?'\n";
    return false;
  }

  ff = name.find("#");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '#'\n";
    return false;
  }

  ff = name.find("¨");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '¨'\n";
    return false;
  }

  ff = name.find(",");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: ','\n";
    return false;
  }

  ff = name.find("/");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '/'\n";
    return false;
  }

  ff = name.find("@");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '@'\n";
    return false;
  }

  ff = name.find("{");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '{'\n";
    return false;
  }

  ff = name.find("}");
  if(ff >= 0)
  {
    invalidChar += "invalid symbol: '}'\n";
    return false;
  }

  std::vector<std::string> vecInvalidChars;
  vecInvalidChars.push_back("ª");
  vecInvalidChars.push_back("º");
  vecInvalidChars.push_back("¹");
  vecInvalidChars.push_back("²");
  vecInvalidChars.push_back("³");

  for(unsigned int i = 0; i < vecInvalidChars.size(); ++i)
  {
    std::string invalidItem = vecInvalidChars[i];

    ff = name.find(invalidItem);
    if(ff >= 0)
    {
      invalidChar += "invalid symbol: '" + invalidItem + "'\n";
      return false;
    }
  }
  
  for(unsigned int i = 0; i < name.size(); ++i)
  { 
    char value = name[i];
    if(value < 0)
    {
      invalidChar += "invalid symbol\n";
      return false;
    }
  }

  std::string u = te::common::Convert2UCase(name);
  if(u=="OR" || u=="AND" || u=="NOT" || u=="LIKE" ||
     u=="SELECT" || u=="FROM" || u=="UPDATE" || u=="DELETE" ||u=="BY" || u=="GROUP" || u=="ORDER" ||
     u=="DROP" || u=="INTO" || u=="VALUE" || u=="IN" || u=="ASC" || u=="DESC"|| u=="COUNT" || u=="JOIN" ||
     u=="LEFT" || u=="RIGHT" || u=="INNER" || u=="UNION" || u=="IS" || u=="NULL" || u=="WHERE" ||
     u=="BETWEEN" || u=="DISTINCT" || u=="TRY" || u=="IT" || u=="INSERT" || u=="ALIASES" || u=="CREATE" ||
     u=="ALTER" || u=="TABLE" || u=="INDEX" || u=="ALL" || u=="HAVING" || u=="EXEC" || u== "SET" ||
     u == "AVG" || u == "MAX" || u == "MIN" || u == "SUM" || u == "FILTER" || u == "OFFSET"  || u == "LENGHT" )
  {
    invalidChar += "invalid name: using reserved word " + u + "\n";  
    return false;
  }

  std::string n = te::common::Convert2LCase(name); 
  // reserved words
  if( (n=="zone") || (n=="comp") || (n=="no") || (n=="local") ||
    (n=="level") || (n=="long"))
  {
    invalidChar += "invalid name: using reserved word " + n + "\n";  
    return false;
  }

  return true;
}


bool te::da::HasLinkedTable(te::map::AbstractLayer* layer)
{
  if(layer->getType() == "QUERYLAYER")
  {
    std::auto_ptr<te::da::DataSetType> schema = layer->getSchema();
    std::vector<te::dt::Property*> props = schema->getPrimaryKey()->getProperties();
    if(props.size() > 1)
    {
      size_t pksize = 0;
      while(++pksize < props.size())
      {
        if(props[pksize-1]->getDatasetName() != props[pksize]->getDatasetName())
          return true;
      }
    }
  }
  return false;
}

double te::da::GetSummarizedValue(std::vector<double>& values, const std::string& sumary)
{
  double size = values.size();
  if(size == 0)
    return 0;

  double d, v;
  std::vector<double>::const_iterator it;

  if(sumary == "MIN")
  {
    it = values.begin();
    v = *it;

    while(it != values.end())
    {
      d = *it++;
      v = std::min(v, d);    
    }
  }
  else if(sumary == "MAX")
  {
    it = values.begin();
    v = *it;

    while(it != values.end())
    {
      d = *it++;
      v = std::max(v, d);    
    }
  }
  else if(sumary == "SUM")
  {
    v = 0;
    for(it = values.begin(); it != values.end(); ++it)
      v += *it;
  }
  else if(sumary == "AVERAGE")
  {
    v = 0;
    for(it = values.begin(); it != values.end(); ++it)
      v += *it;
    v /= size;
  }
  else if(sumary == "STDDEV")
  {
    double m = 0;
    v = 0;
    if(size > 1)
    {
      for(it = values.begin(); it != values.end(); ++it)
      {
        d = *it;
        m += d;
        v += (d * d);
      }
      m /= size;
      v = (v - m) / (size - 1);
      v = sqrt(v);
    }
  }
  else if(sumary == "VARIANCE")
  {
    double m = 0;
    v = 0;
    if(size > 1)
    {
      for(it = values.begin(); it != values.end(); ++it)
      {
        d = *it;
        m += d;
        v += (d * d);
      }
      m /= size;
      v = (v - m) / (size - 1);
    }
  }
  else if(sumary == "MEDIAN")
  {
    if(size == 1)
      v = *it;
    else
    {
      std::stable_sort(values.begin(), values.end());
      size_t meio = (size_t)size / 2;
      v = values[meio];

      if((size_t)size%2 == 0)
        v += values[meio+1] / 2.;
    }
  }
  else if(sumary == "MODE")  // nao dá porque pode gerar nenhum ou vários valores
  {
  }

  return v;
}

std::string te::da::GetSummarizedValue(const std::vector<std::string>& values, const std::string& sumary)
{
  double size = values.size();
  if(size == 0)
    return 0;

  std::string v, d;
  std::vector<std::string>::const_iterator it;

  if(sumary == "MIN")
  {
    it = values.begin();
    v = *it;

    while(it != values.end())
    {
      d = *it++;
      v = std::min(v, d);    
    }
  }
  else if(sumary == "MAX")
  {
    it = values.begin();
    v = *it;

    while(it != values.end())
    {
      d = *it++;
      v = std::max(v, d);    
    }
  }

  return v;
}
double te::da::Round(const double& value, const size_t& precision)
{
  double v = pow(10., (int)precision);
  double ret = boost::math::round(value * v);
  ret /= v;
  return ret;
}
