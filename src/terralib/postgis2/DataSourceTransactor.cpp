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
  \file terralib/dataaccess/postgis/DataSourceTransactor.cpp

  \brief A DataSourceTransactor can be viewed as a gateway for reading/writing things into the data source.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/dataset/CheckConstraint.h"
#include "../dataaccess2/dataset/DataSet.h"
#include "../dataaccess2/dataset/ForeignKey.h"
#include "../dataaccess2/dataset/Index.h"
#include "../dataaccess2/dataset/ObjectIdSet.h"
#include "../dataaccess2/dataset/PrimaryKey.h"
#include "../dataaccess2/dataset/Sequence.h"
#include "../dataaccess2/dataset/UniqueKey.h"
#include "../dataaccess2/datasource/DataSourceCatalog.h"
#include "../dataaccess2/datasource/ScopedTransaction.h"
#include "../dataaccess2/query/Select.h"
#include "../dataaccess2/query/SQLDialect.h"
#include "../dataaccess2/utils/Utils.h"
#include "../datatype/Array.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"
#include "../raster/Grid.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterProperty.h"
#include "../geometry/Geometry.h"
#include "Connection.h"
#include "ConnectionPool.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "DataSet.h"
#include "Exception.h"
#include "Globals.h"
#include "PreparedQuery.h"
#include "SQLVisitor.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/format.hpp>

// libpq
#include <libpq-fe.h>

te::pgis::DataSourceTransactor::DataSourceTransactor(DataSource* ds, Connection* conn)
  : m_ds(ds),
    m_conn(conn),
    m_isInTransaction(false)
{
}

te::pgis::DataSourceTransactor::~DataSourceTransactor()
{
  m_ds->getConnPool()->release(m_conn);
}

te::da::DataSource* te::pgis::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}

te::pgis::Connection* te::pgis::DataSourceTransactor::getConnection() const
{
  return m_conn;
}

void te::pgis::DataSourceTransactor::begin()
{
  execute("BEGIN");
  m_isInTransaction = true;
}

void te::pgis::DataSourceTransactor::commit()
{
 m_isInTransaction = false;
  execute("COMMIT");
}

void te::pgis::DataSourceTransactor::rollBack()
{
  m_isInTransaction = false;
  execute("ROLLBACK");
}

bool te::pgis::DataSourceTransactor::isInTransaction() const
{
 return m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSourceTransactor::getDataSet(const std::string& name,
                                                                          te::common::TraverseType travType,
                                                                          bool /*isConnected*/)
{
   std::auto_ptr<std::string> sql(new std::string("SELECT * FROM "));
   *sql += name;

  PGresult* result = m_conn->query(*sql);

  std::vector<int> ptypes;
  Convert2TerraLib(result, m_ds->getGeomTypeId(), m_ds->getRasterTypeId(), ptypes);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, ptypes, m_ds->isTimeAnInteger()));
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSourceTransactor::getDataSet(const std::string& name,
                                                                          const std::string& propertyName,
                                                                          const te::gm::Envelope* e,
                                                                          te::gm::SpatialRelation r,
                                                                          te::common::TraverseType travType,
                                                                          bool /*isConnected*/)
{
  if(e == 0)
    throw Exception(TR_PGIS("The envelope is missing!"));

  std::auto_ptr<te::dt::Property> p = getProperty(name, propertyName);

  const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p.get());

  std::string rel = GetBoxSpatialRelation(r);

  std::string sql("SELECT * FROM ");
  sql += name;
  sql += " WHERE ";
  sql += propertyName;
  sql += rel;

  Convert2PostGIS(e, gp->getSRID(), sql);

  PGresult* result = m_conn->query(sql);

  std::vector<int> ptypes;
  Convert2TerraLib(result, m_ds->getGeomTypeId(), m_ds->getRasterTypeId(), ptypes);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, ptypes, m_ds->isTimeAnInteger()));
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSourceTransactor::getDataSet(const std::string& name,
                                                                          const std::string& propertyName,
                                                                          const te::gm::Geometry* g,
                                                                          te::gm::SpatialRelation r,
                                                                          te::common::TraverseType travType,
                                                                          bool /*isConnected*/)
{
 if(g == 0)
    throw Exception(TR_PGIS("The geometry is missing!"));

  std::string rel = GetSpatialRelation(r);

  std::string sql("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";
              sql += rel;
              sql += "(";

  Convert2PostGIS(m_conn->m_pgconn, g, sql);

  sql += ",";
  sql += propertyName;
  sql += ")";

  PGresult* result = m_conn->query(sql);

  std::vector<int> ptypes;
  Convert2TerraLib(result, m_ds->getGeomTypeId(), m_ds->getRasterTypeId(), ptypes);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, ptypes, m_ds->isTimeAnInteger()));
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSourceTransactor::query(const te::da::Select& q,
                                                                     te::common::TraverseType travType,
                                                                     bool isConnected)
{
  std::string sql;

  SQLVisitor visitor(*(m_ds->getDialect()), sql, m_conn->getConn());
  q.accept(visitor);

  return query(sql, travType, isConnected);
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSourceTransactor::query(const std::string& query,
                                                                     te::common::TraverseType travType,
                                                                     bool isConnected)
{
  PGresult* result = m_conn->query(query);

  std::vector<int> ptypes;
  Convert2TerraLib(result, m_ds->getGeomTypeId(), m_ds->getRasterTypeId(), ptypes);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, ptypes, m_ds->isTimeAnInteger()));
}

void te::pgis::DataSourceTransactor::execute(const te::da::Query& command)
{
  std::string sql;

  SQLVisitor visitor(*(m_ds->getDialect()), sql, m_conn->getConn());
  command.accept(visitor);

  execute(sql);
}

void te::pgis::DataSourceTransactor::execute(const std::string& command)
{
  m_conn->execute(command);
}

std::auto_ptr<te::da::PreparedQuery> te::pgis::DataSourceTransactor::getPrepared(const std::string& qName)
{
  return std::auto_ptr<te::da::PreparedQuery>(new PreparedQuery(this, qName));
}

std::auto_ptr<te::da::BatchExecutor> te::pgis::DataSourceTransactor::getBatchExecutor()
{
  return std::auto_ptr<te::da::BatchExecutor>(0);
}

void te::pgis::DataSourceTransactor::cancel()
{
}

boost::int64_t te::pgis::DataSourceTransactor::getLastGeneratedId()
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::string te::pgis::DataSourceTransactor::escape(const std::string& value)
{
  return value;
}

bool te::pgis::DataSourceTransactor::isDataSetNameValid(const std::string& datasetName)
{
  return true;
}

bool te::pgis::DataSourceTransactor::isPropertyNameValid(const std::string& propertyName)
{
  return true;
}

std::vector<std::string> te::pgis::DataSourceTransactor::getDataSetNames()
{
  std::vector<std::string> datasetNames;

  std::string sql("SELECT pg_class.oid, pg_namespace.nspname, pg_class.relname, pg_class.relkind "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relname !~ '^pg_' "
                  "AND pg_class.relname NOT IN ('spatial_ref_sys', 'geometry_columns', 'geography_columns', 'raster_columns', 'raster_overviews') "
                  "AND pg_class.relkind in ('r','v') "
                  "AND pg_class.relnamespace = pg_namespace.oid "
                  "AND pg_namespace.nspname NOT IN ('information_schema', 'pg_toast', 'pg_temp_1', 'pg_catalog', 'topology')");

  std::auto_ptr<te::da::DataSet> datasetInfo = query(sql);

  while(datasetInfo->moveNext())
  {
    std::string datasetName = std::string(datasetInfo->getString(1) + "." + datasetInfo->getString(2));
    datasetNames.push_back(datasetName);
  }

  return datasetNames;
}

std::size_t te::pgis::DataSourceTransactor::getNumberOfDataSets()
{
  return getDataSetNames().size();
}

te::da::DataSetTypePtr te::pgis::DataSourceTransactor::getDataSetType(const std::string& name)
{
  std::string datasetName = getFullName(name);

  if(!dataSetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % datasetName).str());

  // If the dataset is already in the catalog, get it from there
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();

  if(catalog->datasetExists(datasetName))
    return catalog->getDataSetType(datasetName);

  // Find the dataset id
  unsigned int dtid = getDataSetId(datasetName);
  
  // Create the dataset type
  te::da::DataSetTypePtr dt(new te::da::DataSetType(datasetName, dtid));
  dt->setTitle(datasetName);

  // Get the properties of the dataset and add them to its schema
  boost::ptr_vector<te::dt::Property> properties = getProperties(datasetName);
  for(std::size_t i = 0; i < properties.size(); ++i)
  {
    te::dt::Property* p = properties[i].clone();
    dt->add(p);
  }

  // Get all the constraints of the dataset and load them to its schema
  getConstraints(dt);

  // Get the indexes of the dataset and add them to its schema
  getIndexes(dt);

  // Add the dataset schema to the catalog
  catalog->add(dt);

  return dt;
}

boost::ptr_vector<te::dt::Property> te::pgis::DataSourceTransactor::getProperties(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  boost::ptr_vector<te::dt::Property> properties;

  std::auto_ptr<te::da::DataSet> pInfo = getPropertiesInfo(fullDatasetName);

  while(pInfo->moveNext())
  {
    unsigned int attNum = pInfo->getInt16(0);
    std::string attName = pInfo->getString(1);
    unsigned int attType = pInfo->getInt32(2);
    bool attNotNull = pInfo->getBool(3);
    std::string fmt = pInfo->getString(4);
    bool attHasDefault = pInfo->getBool(5);
    std::string attDefValue = pInfo->getString(6);
    int ndims = pInfo->getInt32(7);

    te::dt::Property* p =  Convert2TerraLib(attNum, attName.c_str(), attType, attNotNull,
                                            fmt.c_str(), attHasDefault, attDefValue.c_str(),
                                            ndims, m_ds->getGeomTypeId(), m_ds->getRasterTypeId());

    properties.push_back(p);

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      getGeometryInfo(datasetName, static_cast<te::gm::GeometryProperty*>(p));
    }
    else if(p->getType() == te::dt::RASTER_TYPE)
    {
      getRasterInfo(datasetName, static_cast<te::rst::RasterProperty*>(p));
    }
  }

  return properties;
}

std::auto_ptr<te::dt::Property> te::pgis::DataSourceTransactor::getProperty(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  te::dt::Property* p = 0;

  std::string pName = name;

  // If the property name contains also the dataset name, strip it from the name.
  std::size_t pos = pName.find_last_of(".");
  if(pos != std::string::npos)
    pName = pName.substr(++pos);

  std::auto_ptr<te::da::DataSet> pInfo = getPropertiesInfo(fullDatasetName);
  while(pInfo->moveNext())
  {
    std::string attName = pInfo->getString(1);
    if(attName != pName)
      continue;

    unsigned int attNum = pInfo->getInt16(0);
    unsigned int attType = pInfo->getInt32(2);
    bool attNotNull = pInfo->getBool(3);
    std::string fmt = pInfo->getString(4);
    bool attHasDefault = pInfo->getBool(5);
    std::string attDefValue = pInfo->getString(6);
    int ndims = pInfo->getInt32(7);

    p =  Convert2TerraLib(attNum, attName.c_str(), attType, attNotNull,
                          fmt.c_str(), attHasDefault, attDefValue.c_str(),
                          ndims, m_ds->getGeomTypeId(), m_ds->getRasterTypeId());

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      getGeometryInfo(datasetName, static_cast<te::gm::GeometryProperty*>(p));
    }
    else if(p->getType() == te::dt::RASTER_TYPE)
    {
      getRasterInfo(datasetName, static_cast<te::rst::RasterProperty*>(p));
    }
  }

  return std::auto_ptr<te::dt::Property>(p);
}

std::auto_ptr<te::dt::Property> te::pgis::DataSourceTransactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  std::string fullDatasetName = getFullName(datasetName);

  boost::ptr_vector<te::dt::Property> properties = getProperties(fullDatasetName);

  assert(propertyPos < properties.size());

  return std::auto_ptr<te::dt::Property>(properties[propertyPos].clone());
}

std::vector<std::string> te::pgis::DataSourceTransactor::getPropertyNames(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);

  boost::ptr_vector<te::dt::Property> properties = getProperties(fullDatasetName);

  std::size_t numProperties = properties.size();

  std::vector<std::string> pNames(numProperties);

  for(std::size_t i = 0; i < numProperties; ++i)
    pNames[i] = properties[i].getName();

  return pNames;
}

std::size_t te::pgis::DataSourceTransactor::getNumberOfProperties(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);
  return getProperties(fullDatasetName).size();
}

bool te::pgis::DataSourceTransactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::vector<std::string> pNames = getPropertyNames(fullDatasetName);

  if(std::find(pNames.begin(), pNames.end(), name) != pNames.end())
    return true;

  return false;
}

void te::pgis::DataSourceTransactor::addProperty(const std::string& datasetName, te::dt::Property* p)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::string pName = p->getName();

  if(propertyExists(datasetName, pName))
    throw Exception((boost::format(TR_PGIS("The dataset already \"%1%\" has a property with this name \"%2%\"!")) % fullDatasetName % pName).str());

  // Persist the property in the database
  std::string sql;
 
  if(p->getType() == te::dt::GEOMETRY_TYPE)
  {
    te::gm::GeometryProperty* gp = static_cast<te::gm::GeometryProperty*>(p);

    sql  = "SELECT AddGeometryColumn('";

  // split schema-name.table-name
    std::size_t pos = fullDatasetName.find(".");

    if(pos == std::string::npos)
    {
      sql += te::common::Convert2LCase(fullDatasetName);
      sql += "', '";
    }
    else
    {
      sql += te::common::Convert2LCase(fullDatasetName.substr(0, pos));
      sql += "', '";
      sql += te::common::Convert2LCase(fullDatasetName.substr(pos + 1));
      sql += "', '";
    }

    sql += gp->getName();
    sql += "', ";
    sql += te::common::Convert2String(gp->getSRID());
    sql += ", '";
    sql += GetGeometryName(gp->getGeometryType() == te::gm::UnknownGeometryType ? te::gm::GeometryType : gp->getGeometryType());
    sql += "', ";
    sql += te::common::Convert2String(te::gm::GetCoordDimension(gp->getGeometryType()));
    sql += ")";
  }
  else
  {
    sql = "ALTER TABLE ";
    sql += fullDatasetName;
    sql += " ADD COLUMN ";
    sql += pName;
    sql += " ";

    SetColumnDef(sql, p);
  }

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    dt->add(p);
  }
}

void te::pgis::DataSourceTransactor::dropProperty(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!propertyExists(fullDatasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no property with this name \"%2%\"!")) % fullDatasetName % name).str());

  std::auto_ptr<te::dt::Property> p = getProperty(fullDatasetName, name);

  std::string sql;

  if(p->getType() == te::dt::GEOMETRY_TYPE)
  {
    sql  =  "SELECT DropGeometryColumn('";

    // Split schema-name.table-name, if needed
    std::size_t pos = fullDatasetName.find(".");

    if(pos == std::string::npos)
    {
      sql += m_ds->getCurrentSchema();
      sql += "', '";
      sql += te::common::Convert2LCase(fullDatasetName);
    }
    else
    {
      sql += te::common::Convert2LCase(fullDatasetName.substr(0, pos));
      sql += "', '";
      sql += te::common::Convert2LCase(fullDatasetName.substr(pos + 1));
    }

    sql += "', '";
    sql += te::common::Convert2LCase(name) + "'";
    sql += ")";    
  }
  else
  {
    sql  = " ALTER TABLE ";
    sql += p->getParent()->getName();
    sql += " DROP COLUMN ";
    sql += name;
  }

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    te::dt::Property* p = dt->getProperty(name);
    dt->remove(p);
  }
}

void te::pgis::DataSourceTransactor::renameProperty(const std::string& datasetName,
                                                    const std::string& name,
                                                    const std::string& newName)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!propertyExists(fullDatasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no property with this name \"%2%\"!")) % fullDatasetName % name).str());

  if(propertyExists(fullDatasetName, newName))
    throw Exception((boost::format(TR_PGIS("There dataset \"%1%\" already has a property with this name \"%2%\"!")) % fullDatasetName % newName).str());
  else
  {
    if(!isPropertyNameValid(newName))
      throw Exception((boost::format(TR_PGIS("The name of the new property \"%1%\" is not valid!")) % newName).str());
  }

  std::auto_ptr<te::dt::Property> p = getProperty(fullDatasetName, name);

  std::string sql("ALTER TABLE ");
  sql += fullDatasetName;
  sql += " RENAME COLUMN ";
  sql += name + " TO ";
  sql += newName;

  execute(sql);

  if(p->getType() == te::dt::GEOMETRY_TYPE)
  {
    sql = "UPDATE geometry_columns SET f_geometry_column = '" + newName + "'";
    sql += " WHERE f_table_name = '" ;

    // Split schema-name.table-name
    std::size_t pos = fullDatasetName.find(".");
    
    if(pos == std::string::npos)
    {
      sql += fullDatasetName;
      sql += "' AND f_table_schema ='";
      sql += m_ds->getCurrentSchema();
      sql += "'";
    }
    else
    {
      sql += fullDatasetName.substr(pos + 1);
      sql += "' AND f_table_schema ='";
      sql += fullDatasetName.substr(0, pos);
      sql += "'";
    }

    sql += " AND f_geometry_column = '" + name + "'";

    execute(sql);
  }

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    te::dt::Property* p = dt->getProperty(name);
    p->setName(newName);
  }
}

std::auto_ptr<te::da::PrimaryKey> te::pgis::DataSourceTransactor::getPrimaryKey(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  te::da::PrimaryKey* pk = 0;

  std::auto_ptr<te::da::DataSet> pkInfo = getConstraints(fullDatasetName, 'p');

  while(pkInfo->moveNext())
  {
    unsigned int pkId = pkInfo->getInt32(0);
    std::string pkName  = pkInfo->getString(2);

    pk = new te::da::PrimaryKey(pkName, 0, pkId);

    std::auto_ptr<te::dt::Array> pkCols(pkInfo->getArray(8));
    std::size_t size = pkCols->getDimensionSize(0);

    std::vector<std::size_t> pos;
    pos.push_back(0);
    for(std::size_t i = 0; i < size; ++i)
    {
      pos[0] = i;

      te::dt::AbstractData* pkCol = pkCols->getData(pos);

      std::auto_ptr<te::dt::Property> p = getProperty(static_cast<te::dt::Int16*>(pkCol)->getValue(), fullDatasetName);
      pk->add(p.release());
    }
  }

  // Try to link the pk to an index
  std::vector<std::string> idxNames = getIndexNames(fullDatasetName);

  for(std::size_t i = 0; i < idxNames.size(); ++i)
  {
    if(pk->getName() == idxNames[i])
    {
      pk->setAssociatedIndex(getIndex(fullDatasetName, idxNames[i]).get());
      break;
    }
  }

  return std::auto_ptr<te::da::PrimaryKey>(pk);
}

bool te::pgis::DataSourceTransactor::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::auto_ptr<te::da::PrimaryKey> pk = getPrimaryKey(fullDatasetName);

  if(pk->getName() == name)
    return true;

  return false;
}

void te::pgis::DataSourceTransactor::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
  std::string fullDatasetName = getFullName(datasetName);

  // Check if there is a primary key in the dataset
  // If true, drop it from the dataset
  if(getPrimaryKey(fullDatasetName).get() != 0)
    dropPrimaryKey(fullDatasetName);

  std::string pkName;

  if(pk->getName().empty())
  {
    pkName = fullDatasetName + "_pk";

    boost::replace_all(pkName, ".", "_");

    pk->setName(pkName);
  }

  pkName = pk->getName();

  std::string sql("ALTER TABLE ");
  sql += fullDatasetName;
  sql += " ADD CONSTRAINT ";
  sql += pkName;
  sql += " PRIMARY KEY (";

  const std::vector<te::dt::Property*>& properties = pk->getProperties();

  std::size_t size = properties.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += properties[i]->getName();
  }

  sql += ")";

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    dt->add(pk);
  }
}

void te::pgis::DataSourceTransactor::dropPrimaryKey(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::auto_ptr<te::da::PrimaryKey> pk = getPrimaryKey(fullDatasetName);

  std::string pkName = pk->getName();

  std::string sql("ALTER TABLE ");
  sql += fullDatasetName;
  sql += " DROP CONSTRAINT " + pkName;

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    dt->remove(pk.release());
  }
}

std::auto_ptr<te::da::ForeignKey> te::pgis::DataSourceTransactor::getForeignKey(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  te::da::ForeignKey* fk = 0;

  std::auto_ptr<te::da::DataSet> fkInfo = getConstraints(fullDatasetName, 'f');

  while(fkInfo->moveNext())
  {
    std::string fkName  = fkInfo->getString(2);

    if(fkName != name)
      continue;

    unsigned int fkId = fkInfo->getInt32(0);
    unsigned int refDatasetId = fkInfo->getInt32(4);
    char onUpdate = fkInfo->getChar(5);
    char onDeletion = fkInfo->getChar(6);

    std::auto_ptr<te::dt::Array> fkCols(fkInfo->getArray(8));
    std::auto_ptr<te::dt::Array> fkRefCols(fkInfo->getArray(9));

    assert(fkCols->getDimension() == 1);
    assert(fkCols->getDimension() == fkRefCols->getDimension());
    assert(fkCols->getDimensionSize(0) == fkRefCols->getDimensionSize(0));

    std::string refName = getDataSetName(refDatasetId);
    const te::da::DataSetTypePtr& refdt = getDataSetType(refName);

    fk = new te::da::ForeignKey(fkName, fkId);
    fk->setOnUpdateAction(GetAction(onUpdate));
    fk->setOnDeleteAction(GetAction(onDeletion));
    fk->setReferencedDataSetType(refdt.get());
  
    std::size_t size = fkCols->getDimensionSize(0);

    std::vector<std::size_t> pos;
    pos.push_back(0);

    for(std::size_t i = 0; i < size; ++i)
    {
      pos[0] = i;

      te::dt::AbstractData* fkRefCol = fkRefCols->getData(pos);
      fk->addRefProperty(refdt->getPropertyById(static_cast<te::dt::Int16*>(fkRefCol)->getValue()));

      te::dt::AbstractData* fkCol = fkCols->getData(pos);
      fk->add(getProperty(static_cast<te::dt::Int16*>(fkCol)->getValue(), datasetName).release());
    }
  }

  return std::auto_ptr<te::da::ForeignKey>(fk);
}

std::vector<std::string> te::pgis::DataSourceTransactor::getForeignKeyNames(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  std::vector<std::string> fkNames;

  std::auto_ptr<te::da::DataSet> fkInfo = getConstraints(fullDatasetName, 'f');

  while(fkInfo->moveNext())
  {
    std::string fkName  = fkInfo->getString(2);
    fkNames.push_back(fkName);
  }

  return fkNames;
}

bool te::pgis::DataSourceTransactor::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::vector<std::string> fkNames = getForeignKeyNames(fullDatasetName);
  if(std::find(fkNames.begin(), fkNames.end(), name) != fkNames.end())
    return true;

  return false;
}

void te::pgis::DataSourceTransactor::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::string fkName = fk->getName();

  if(foreignKeyExists(fullDatasetName, fkName))
    throw Exception((boost::format(TR_PGIS("The dataset already \"%1%\" has a foreign key with this name \"%2%\"!")) % fullDatasetName % fkName).str());

  std::string sql("ALTER TABLE ");
  sql += fullDatasetName;
  sql += " ADD CONSTRAINT ";
  sql += fkName;
  sql += " FOREIGN KEY (";
  
  std::size_t size = fk->getProperties().size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += fk->getProperties()[i]->getName();
  }

  sql += ") REFERENCES " + fk->getReferencedDataSetType()->getName() + " (";

  size = fk->getReferencedProperties().size();

  for(size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += fk->getReferencedProperties()[i]->getName();
  }

  sql += ") ON DELETE ";

  switch(fk->getOnDeleteAction())
  {
    case te::da::NO_ACTION:
      sql += " NO ACTION ";
      break;

    case te::da::RESTRICT:
      sql += " RESTRICT "; 
      break;

    case te::da::CASCADE:
      sql += " CASCADE "; 
      break;

    case te::da::SET_NULL: 
      sql += "SET NULL ";
      break;

    case te::da::SET_DEFAULT:
    default:
      sql += "SET DEFAULT ";
      break;
  }

  sql += " ON UPDATE ";

  switch(fk->getOnUpdateAction())
  {
    case te::da::NO_ACTION:
      sql += " NO ACTION ";
    break;

    case te::da::RESTRICT:
      sql += " RESTRICT "; 
    break;

    case te::da::CASCADE:
      sql += " CASCADE "; 
    break;

    case te::da::SET_NULL: 
      sql += "SET NULL ";
    break;

    case te::da::SET_DEFAULT:
    default:
      sql += "SET DEFAULT ";
    break;
  }

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    dt->add(fk);
  }
}

void te::pgis::DataSourceTransactor::dropForeignKey(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!foreignKeyExists(fullDatasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no foreign key with this name \"%2%\"!")) % fullDatasetName % name).str());

  std::string sql("ALTER TABLE ");
  sql += fullDatasetName;
  sql += " DROP CONSTRAINT ";
  sql += name;

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    te::da::ForeignKey* fk = dt->getForeignKey(name);
    dt->remove(fk);
  }
}

std::auto_ptr<te::da::UniqueKey> te::pgis::DataSourceTransactor::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  te::da::UniqueKey* uk = 0;

  std::auto_ptr<te::da::DataSet> ukInfo = getConstraints(fullDatasetName, 'u');

  while(ukInfo->moveNext())
  {
    std::string ukName  = ukInfo->getString(2);

    if(ukName != name)
      continue;

    unsigned int ukId = ukInfo->getInt32(0);

    std::auto_ptr<te::dt::Array> ukCols(ukInfo->getArray(8));

    uk = new te::da::UniqueKey(ukName, 0, ukId);

    std::size_t size = ukCols->getDimensionSize(0);

    std::vector<std::size_t> pos;
    pos.push_back(0);

    for(std::size_t i = 0; i < size; ++i)
    {
      pos[0] = i;

      te::dt::AbstractData* ukCol = ukCols->getData(pos);

      std::auto_ptr<te::dt::Property> p = getProperty(static_cast<te::dt::Int16*>(ukCol)->getValue(), fullDatasetName);

      uk->add(p.release());
    }
  }

  // Try to link the uk to an index
  std::vector<std::string> idxNames = getIndexNames(fullDatasetName);

  for(std::size_t i = 0; i < idxNames.size(); ++i)
  {
    if(uk->getName() == idxNames[i])
    {
      uk->setAssociatedIndex(getIndex(fullDatasetName, idxNames[i]).get());
      break;
    }
  }

  return std::auto_ptr<te::da::UniqueKey>(uk);
}

std::vector<std::string> te::pgis::DataSourceTransactor::getUniqueKeyNames(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  std::vector<std::string> ukNames;

  std::auto_ptr<te::da::DataSet> ukInfo = getConstraints(fullDatasetName, 'u');

  while(ukInfo->moveNext())
  {
    std::string ukName  = ukInfo->getString(2);
    ukNames.push_back(ukName);
  }

  return ukNames;
}

bool te::pgis::DataSourceTransactor::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::vector<std::string> ukNames = getUniqueKeyNames(fullDatasetName);

  if(std::find(ukNames.begin(), ukNames.end(), name) != ukNames.end())
    return true;

  return false;
}

void te::pgis::DataSourceTransactor::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::string ukName = uk->getName();

  if(uniqueKeyExists(fullDatasetName, ukName))
    throw Exception((boost::format(TR_PGIS("The dataset already \"%1%\" has a unique key with the name \"%2%\"!")) % fullDatasetName % ukName).str());

  std::string sql("ALTER TABLE ");
  sql += fullDatasetName;
  sql += " ADD CONSTRAINT ";
  sql += ukName;
  sql += " UNIQUE (";

  const std::vector<te::dt::Property*>& properties = uk->getProperties();

  std::size_t size = properties.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += properties[i]->getName();
  }

  sql += ")";

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    dt->add(uk);
  }
}

void te::pgis::DataSourceTransactor::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!uniqueKeyExists(fullDatasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no unique key with this name \"%2%\"!")) % fullDatasetName % name).str());

  std::string sql("ALTER TABLE ");
  sql += fullDatasetName;
  sql += " DROP CONSTRAINT ";
  sql += name;

  execute(sql);

  // Remove the index associated to the unique key
  if(indexExists(fullDatasetName, name))
    dropIndex(fullDatasetName, name);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    te::da::UniqueKey* uk = dt->getUniqueKey(name);
    dt->remove(uk);
  }
}

std::auto_ptr<te::da::CheckConstraint> te::pgis::DataSourceTransactor::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  te::da::CheckConstraint* cc = 0;

  std::auto_ptr<te::da::DataSet> ccInfo = getConstraints(datasetName, 'c');

  while(ccInfo->moveNext())
  {
    std::string ccName  = ccInfo->getString(2);

    if(ccName != name)
      continue;

    unsigned int ccId = ccInfo->getInt32(0);

    cc = new te::da::CheckConstraint(ccName);
    cc->setId(ccId);
    cc->setExpression(ccInfo->getString(10));
  }

  return std::auto_ptr<te::da::CheckConstraint>(cc);
}

std::vector<std::string> te::pgis::DataSourceTransactor::getCheckConstraintNames(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  std::vector<std::string> ccNames;

  std::auto_ptr<te::da::DataSet> ccInfo = getConstraints(fullDatasetName, 'c');

  while(ccInfo->moveNext())
  {
    std::string ccName = ccInfo->getString(2);

    ccNames.push_back(ccName);
  }

  return ccNames;
}

bool te::pgis::DataSourceTransactor::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::vector<std::string> ccNames = getCheckConstraintNames(fullDatasetName);

  if(std::find(ccNames.begin(), ccNames.end(), name) != ccNames.end())
    return true;

  return false;
}

void te::pgis::DataSourceTransactor::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::string ccName = cc->getName();

  if(checkConstraintExists(fullDatasetName, ccName))
    throw Exception((boost::format(TR_PGIS("The dataset already \"%1%\" has a check constraint key with the name \"%2%\"!")) % fullDatasetName % ccName).str());

  std::string sql("ALTER TABLE ");
  sql += fullDatasetName;
  sql += " ADD CONSTRAINT ";
  sql += ccName;
  sql += " CHECK(";
  sql += cc->getExpression();
  sql += ")";

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    dt->add(cc);
  }
}

void te::pgis::DataSourceTransactor::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!checkConstraintExists(fullDatasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no check constraint with this name \"%2%\"!")) % fullDatasetName % name).str());

  std::string sql("ALTER TABLE ");
  sql += fullDatasetName;
  sql += " DROP CONSTRAINT ";
  sql += name;

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    te::da::CheckConstraint* cc = dt->getCheckConstraint(name);
    dt->remove(cc);
  }
}

std::auto_ptr<te::da::Index> te::pgis::DataSourceTransactor::getIndex(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  te::da::Index* idx = 0;

  unsigned int dtid = getDataSetId(fullDatasetName);

  std::string sql("SELECT idx_table.oid, s.nspname, idx_table.relname, pg_index.indkey, pg_am.amname, pg_index.indisunique, pg_index.indisprimary " 
                  "FROM pg_index, pg_class idx_table, pg_am, pg_namespace s "
                  "WHERE s.oid = idx_table.relnamespace "
                  "AND pg_index.indexrelid = idx_table.oid "
                  "AND idx_table.relam = pg_am.oid "
                  "AND pg_index.indrelid = ");
  sql += te::common::Convert2String(dtid);

  std::auto_ptr<te::da::DataSet> idxInfo = query(sql);

  while(idxInfo->moveNext())
  {
    std::string idxName = idxInfo->getString(2);

    if(idxName != name)
      continue;

    unsigned int idxId = idxInfo->getInt32(0);

    std::auto_ptr<te::dt::Array> idxCols(idxInfo->getArray(3));

    std::string idxType = idxInfo->getString(4);
    bool isUK = idxInfo->getBool(5);
    bool isPK = idxInfo->getBool(6);

    idx = new te::da::Index(idxName, GetIndexType(idxType.c_str()), 0, idxId);

    std::size_t size = idxCols->getDimensionSize(0);

    std::vector<std::size_t> pos;
    pos.push_back(0);

    for(std::size_t i = 0; i < size; ++i)
    {
      pos[0] = i;
      te::dt::AbstractData* idxCol = idxCols->getData(pos);

      std::auto_ptr<te::dt::Property> p = getProperty(static_cast<te::dt::Int16*>(idxCol)->getValue(), fullDatasetName);

      idx->add(p.release());
    }
  }

  return std::auto_ptr<te::da::Index>(idx);
}

std::vector<std::string> te::pgis::DataSourceTransactor::getIndexNames(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!dataSetExists(fullDatasetName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % fullDatasetName).str());

  std::vector<std::string> idxNames;

  unsigned int dtid = getDataSetId(fullDatasetName);

  std::string sql("SELECT idx_table.oid, s.nspname, idx_table.relname, pg_index.indkey, pg_am.amname, pg_index.indisunique, pg_index.indisprimary " 
                  "FROM pg_index, pg_class idx_table, pg_am, pg_namespace s "
                  "WHERE s.oid = idx_table.relnamespace "
                  "AND pg_index.indexrelid = idx_table.oid "
                  "AND idx_table.relam = pg_am.oid "
                  "AND pg_index.indrelid = ");
  sql += te::common::Convert2String(dtid);

  std::auto_ptr<te::da::DataSet> idxInfo = query(sql);

  while(idxInfo->moveNext())
  {
    std::string idxName = idxInfo->getString(2);

    idxNames.push_back(idxName);
  }

  return idxNames;
}

bool te::pgis::DataSourceTransactor::indexExists(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  std::vector<std::string> idxNames = getIndexNames(fullDatasetName);

  if(std::find(idxNames.begin(), idxNames.end(), name) != idxNames.end())
    return true;

  return false;
}

void te::pgis::DataSourceTransactor::addIndex(const std::string& datasetName, te::da::Index* idx,
                                    const std::map<std::string, std::string>& options) 
{
  std::string fullDatasetName = getFullName(datasetName);

  std::string idxName = idx->getName();

  if(indexExists(datasetName, idxName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" already has an index with the name \"%2%\"!")) % fullDatasetName % idxName).str());

  // Check if the index is associated to a UK or PK
  std::auto_ptr<te::da::PrimaryKey> pk = getPrimaryKey(fullDatasetName);

  if(pk.get() && (pk->getAssociatedIndex() == idx))
    return;

  std::vector<std::string> ukNames = getUniqueKeyNames(datasetName);

  for(std::size_t i = 0; i < ukNames.size(); ++i)
    if(getUniqueKey(datasetName, ukNames[i])->getAssociatedIndex() == idx)
      return;

  // If there is not a uk or pk associated, let's create the index!
  std::string sql("CREATE INDEX ");
  sql += idxName;
  sql += " ON ";
  sql += fullDatasetName;
 
  if(idx->getIndexType() == te::da::HASH_TYPE)
    sql += " USING HASH (";
  else if(idx->getIndexType() == te::da::R_TREE_TYPE)
    sql += " USING GIST (";
  else if(idx->getIndexType() == te::da::B_TREE_TYPE)
    sql += " USING BTREE (";
  else
    throw Exception(TR_PGIS("The index type is not supported!"));

  std::size_t numProperties = idx->getProperties().size();

  for(size_t i = 0; i < numProperties; ++i)
  {
    if(i != 0)
      sql += ", ";

    sql += idx->getProperties()[i]->getName();
  }

  sql += ")";

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    dt->add(idx);
  }
}

void te::pgis::DataSourceTransactor::dropIndex(const std::string& datasetName, const std::string& name)
{
  std::string fullDatasetName = getFullName(datasetName);

  if(!indexExists(fullDatasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no index with this name: \"%2%\"!")) % fullDatasetName % name).str());

  std::string sql("DROP INDEX ");
  sql += name;

  execute(sql);

  // If the data source catalog contains the dataset, update it.
  te::da::DataSourceCatalog* catalog = m_ds->getCatalog();
  if(catalog->datasetExists(fullDatasetName))
  {
    te::da::DataSetTypePtr dt = catalog->getDataSetType(fullDatasetName);
    te::da::Index* idx = dt->getIndex(name);
    dt->remove(idx);
  }
}

std::auto_ptr<te::da::Sequence> te::pgis::DataSourceTransactor::getSequence(const std::string& name)
{
  te::da::Sequence* seq = 0;

  // Query the data source for the sequences
  std::vector<std::string> seqNames;

  std::string sql("SELECT c.oid, n.nspname, c.relname, c.relkind "
                  "FROM pg_class c, pg_namespace n "
                  "WHERE c.relname !~ '^pg_' "
                  "AND c.relkind = 'S' "
                  "AND c.relnamespace = n.oid "
                  "AND n.nspname NOT IN ('information_schema', 'pg_toast', 'pg_temp_1', 'pg_catalog')");

  std::auto_ptr<te::da::DataSet> seqNamesInfo = query(sql);

  while(seqNamesInfo->moveNext())
  {
    std::string seqName(seqNamesInfo->getString(2));

    if(seqName != name)
      continue;

    std::string sql("SELECT * FROM ");
    sql += seqName;

    std::auto_ptr<te::da::DataSet> result(query(sql));

    if(result->moveNext())
    {
      unsigned int seqId = getDataSetId(seqName);

      seq = new te::da::Sequence(seqName, 0, 0, 0, seqId);

      // Check if the sequence is cycled
      if(result->getBool(8))
        seq->setAsCycle();
      else
        seq->setAsNoCycle();

      seq->setCachedValues(result->getInt64(6)); // "cache_value"
      seq->setIncrement(result->getInt64(3));    // "increment_by";
      seq->setMaxValue(result->getInt64(4));     // "max_value";
      seq->setMinValue(result->getInt64(5));     // "min_value";
    }
  }

  return std::auto_ptr<te::da::Sequence>(seq);
}

std::vector<std::string> te::pgis::DataSourceTransactor::getSequenceNames()
{
  std::vector<std::string> seqNames;

  std::string sql("SELECT c.oid, n.nspname, c.relname, c.relkind "
                  "FROM pg_class c, pg_namespace n "
                  "WHERE c.relname !~ '^pg_' "
                  "AND c.relkind = 'S' "
                  "AND c.relnamespace = n.oid "
                  "AND n.nspname NOT IN ('information_schema', 'pg_toast', 'pg_temp_1', 'pg_catalog')");

  std::auto_ptr<te::da::DataSet> seqNamesInfo = query(sql);

  while(seqNamesInfo->moveNext())
  {
    std::string seqName(seqNamesInfo->getString(2));
    seqNames.push_back(seqName);
  }

  return seqNames;
}

bool te::pgis::DataSourceTransactor::sequenceExists(const std::string& name)
{
  std::vector<std::string> seqNames = getSequenceNames();

  if(std::find(seqNames.begin(), seqNames.end(), name) != seqNames.end())
    return true;

  return false;
}

void te::pgis::DataSourceTransactor::addSequence(te::da::Sequence* sequence)
{
  std::string seqName = sequence->getName();

  if(sequenceExists(seqName))
    throw Exception((boost::format(TR_PGIS("The datasource already has a sequence with this name (\"%1%\")!")) % seqName).str());

  std::string sql("CREATE SEQUENCE ");
  sql += seqName;
  sql += " INCREMENT BY ";
  sql += te::common::Convert2String(sequence->getIncrement());
  sql += " MINVALUE ";
  sql += te::common::Convert2String(sequence->getMinValue());
  sql += " MAXVALUE ";
  sql += te::common::Convert2String(sequence->getMaxValue());
  sql += " START WITH ";
  sql += te::common::Convert2String(sequence->getStartValue());
  sql += " CACHE ";
  sql += te::common::Convert2String(sequence->getCachedValues());

  if(sequence->isCycled() == false)
    sql += " NO";

  sql += " CYCLE ";

  if(sequence->getOwner())
  {
    sql += " OWNED BY ";
    sql += sequence->getOwner()->getParent()->getName();
    sql += ".";
    sql += sequence->getOwner()->getName();
  }

  execute(sql);

  unsigned int seqId = getDataSetId(seqName);

  sequence->setId(seqId);
}

void te::pgis::DataSourceTransactor::dropSequence(const std::string& name)
{
  std::auto_ptr<te::da::Sequence> seq = getSequence(name);

  if(!sequenceExists(name))
    throw Exception((boost::format(TR_PGIS("There is no sequence with this name: (\"%1%\")!")) % name).str());

  std::string sql("DROP SEQUENCE ");
  sql += name;

  execute(sql);
}

std::auto_ptr<te::gm::Envelope> te::pgis::DataSourceTransactor::getExtent(const std::string& datasetName, const std::string& propertyName)
{
  std::auto_ptr<te::dt::Property> p = getProperty(datasetName, propertyName);

  std::string sql("SELECT ST_Extent(");
  sql += propertyName;
  sql += ") FROM ";
  sql += p->getParent()->getName();

  PGresult* result = PQexec(m_conn->getConn(), sql.c_str());

  if(PQresultStatus(result) != PGRES_TUPLES_OK)
  {
    std::string errmsg(TR_PGIS("Could not find the envelope for the given geometry property due to the following error: "));
    errmsg += PQerrorMessage(m_conn->getConn());

    PQclear(result);

    throw Exception(errmsg);
  }

  const char* boxStr = PQgetvalue(result, 0, 0);
  
  te::gm::Envelope* mbr = 0;

  if(*boxStr != '\0')
    mbr = GetEnvelope(boxStr);

  PQclear(result);
  return std::auto_ptr<te::gm::Envelope>(mbr);
}

std::auto_ptr<te::gm::Envelope> te::pgis::DataSourceTransactor::getExtent(const std::string& datasetName, std::size_t propertyPos)
{
  std::auto_ptr<te::dt::Property> p = getProperty(datasetName, propertyPos);

  std::string sql("SELECT ST_Extent(");
  sql += p->getName();
  sql += ") FROM ";
  sql += p->getParent()->getName();

  PGresult* result = PQexec(m_conn->getConn(), sql.c_str());

  if(PQresultStatus(result) != PGRES_TUPLES_OK)
  {
    std::string errmsg(TR_PGIS("Could not find the envelope for the given geometry property due to the following error: "));
    errmsg += PQerrorMessage(m_conn->getConn());

    PQclear(result);

    throw Exception(errmsg);
  }

  const char* boxStr = PQgetvalue(result, 0, 0);
  
  te::gm::Envelope* mbr = 0;

  if(*boxStr != '\0')
    mbr = GetEnvelope(boxStr);

  PQclear(result);
  return std::auto_ptr<te::gm::Envelope>(mbr);
}

std::size_t te::pgis::DataSourceTransactor::getNumberOfItems(const std::string& datasetName)
{
  std::auto_ptr<te::da::DataSet> result = getDataSet(datasetName);
  return result->size();
}

bool te::pgis::DataSourceTransactor::hasDataSets()
{
  std::vector<std::string> datasetNames = getDataSetNames();

  if(datasetNames.empty())
    return false;

  return true;
}

bool te::pgis::DataSourceTransactor::dataSetExists(const std::string& name)
{
  std::string datasetName = getFullName(name);

  std::vector<std::string> datasetNames = getDataSetNames();

  if(std::find(datasetNames.begin(), datasetNames.end(), datasetName) != datasetNames.end())
    return true;

  return false;
}

void te::pgis::DataSourceTransactor::createDataSet(te::da::DataSetType* dt, const std::map<std::string, std::string>& options)
{
  std::string datasetName = dt->getName();
  datasetName = getFullName(datasetName);

  if(!dataSetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The datasource already has a dataset with this name (\"%1%\")!")) % datasetName).str());

  std::string sql  = "CREATE TABLE ";
  sql += datasetName;
  sql += "()";

  execute(sql);

  // Find the table id
  unsigned int dtid = getDataSetId(datasetName);

  dt->setId(dtid);

  // Add the properties
  std::size_t nCols = dt->size();
  for(std::size_t i = 0; i < nCols; ++i)
    addProperty(datasetName, dt->getProperty(i));

  // Add the primary key
  if(dt->getPrimaryKey())
    addPrimaryKey(datasetName, dt->getPrimaryKey());

  // Add the unique keys
  std::size_t nUKs = dt->getNumberOfUniqueKeys();
  for(std::size_t i = 0; i < nUKs; ++i)
    addUniqueKey(datasetName, dt->getUniqueKey(i));

  // Add the indexes, just if no primary key or unique key with the same name exists!
  std::size_t nIdxs = dt->getNumberOfIndexes();
  for(std::size_t i = 0; i < nIdxs; ++i)
      addIndex(datasetName, dt->getIndex(i), options);

  // Add the foreign keys
  std::size_t nFKs = dt->getNumberOfForeignKeys();
  for(std::size_t i = 0; i < nFKs; ++i)
    addForeignKey(datasetName, dt->getForeignKey(i));

  // Add the the check constraints
  std::size_t nCCs = dt->getNumberOfCheckConstraints();

  for(std::size_t i = 0; i < nCCs; ++i)
    addCheckConstraint(datasetName, dt->getCheckConstraint(i));

  // Try to link the primary key to an index
  std::vector<std::string> indexNames = getIndexNames(datasetName);

  te::da::PrimaryKey* pk = dt->getPrimaryKey();
  if(pk)
  {
    for(std::size_t i = 0; i < indexNames.size(); ++i)
    {
      if(pk->getName() == indexNames[i])
      {
        pk->setAssociatedIndex(dt->getIndex(indexNames[i]));
        break;
      }
    }
  }

  // Try to link the unique keys to an index
  std::size_t numUKs = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < numUKs; ++i)
  {
    te::da::UniqueKey* uk = dt->getUniqueKey(i);

    for(std::size_t j = 0; j < indexNames.size(); ++j)
    {
      if(uk->getName() == indexNames[j])
      {
        uk->setAssociatedIndex(dt->getIndex(indexNames[j]));
        break;
      }
    }
  }
}

void te::pgis::DataSourceTransactor::cloneDataSet(const std::string& /*name*/,
                                                  const std::string& /*cloneName*/,
                                                  const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSourceTransactor::dropDataSet(const std::string& /*name*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));

  //std::string sql;

  //if(dt->hasGeom())
  //{
  //  std::string tSchema, tName;
  //  SplitTableName(dt->getName(), m_t->getPGDataSource()->getCurrentSchema(), tSchema, tName);

  //  sql = "SELECT DropGeometryTable('";
  //  sql += te::common::Convert2LCase(tSchema);
  //  sql += "', '";
  //  sql += te::common::Convert2LCase(tName);
  //  sql += "')";
  //}
  //else
  //{
  //  sql += "DROP TABLE ";
  //  sql += dt->getName();
  //}

  //m_t->execute(sql);
 
  //if(dt->getCatalog())
  //{
  //  te::da::DataSourceCatalog* catalog = dt->getCatalog();
  //  catalog->remove(dt, true);
  //}
  //else
  //  delete dt;
}

void te::pgis::DataSourceTransactor::renameDataSet(const std::string& name, const std::string& newName)
{
  if(!dataSetExists(name))
    throw Exception((boost::format(TR_PGIS("The datasource already has a dataset with this name (\"%1%\")!")) % name).str());

  if(!isDataSetNameValid(newName))
    throw Exception((boost::format(TR_PGIS("The new dataset name (\"%1%\") is not valid!")) % newName).str());

  std::string newTableName, newTableSchema, oldTableName, oldTableSchema;

  std::string sql("ALTER TABLE ");
  sql += name;
  sql += " RENAME TO ";

  SplitTableName(newName, &(m_ds->getCurrentSchema()), newTableSchema, newTableName);

  sql += newTableName;

  execute(sql);

  // If the table has a geometry column, we need to propagate changes to the geometry columns table
  te::da::DataSetTypePtr dt = getDataSetType(name);

  if(dt->hasGeom())
  {
    SplitTableName(name, &(m_ds->getCurrentSchema()), oldTableSchema, oldTableName);

    sql  = "UPDATE geometry_columns SET f_table_name = '";
    sql += newTableName;
    sql += "' WHERE f_table_name = '";
    sql += oldTableName;
    sql += "' AND f_table_schema ='";
    sql += oldTableSchema;
    sql += "'";

    execute(sql);
  }
}

void te::pgis::DataSourceTransactor::add(const std::string& datasetName,
                                         te::da::DataSet* d,
                                         const std::map<std::string, std::string>& options,
                                         std::size_t limit)
{
  if(limit == 0)
    limit = std::string::npos;

// create a prepared statement
  std::string sql  = "INSERT INTO ";
              sql += datasetName;
              sql += te::da::GetSQLValueNames(d);
              sql += " VALUES";
              sql += GetSQLBindValues(d->getNumProperties());

  std::auto_ptr<PreparedQuery> pq(new PreparedQuery(this, "a" + boost::lexical_cast<std::string>((intptr_t)(this))));

  te::da::ScopedTransaction st(*this);

  std::vector<int> paramTypes = te::da::GetPropertyDataTypes(d);

  std::size_t nProcessedRows = 0;

  pq->prepare(sql, paramTypes);

  do
  {
    pq->bind(d);
    pq->execute();

    ++nProcessedRows;

  }while(d->moveNext() && (nProcessedRows != limit));

  st.commit();
}


void te::pgis::DataSourceTransactor::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
  //std::string sql("DELETE FROM ");
  //          sql += datasetName);

  //m_t->execute(sql);

//  const std::vector<te::dt::Property*>* keyProperties = 0;
// 
//  if(dt->getPrimaryKey())
//  {
//    keyProperties = &(dt->getPrimaryKey()->getProperties());
//  }
//  else if(dt->getNumberOfUniqueKeys() > 0)
//  {
//    keyProperties = &(dt->getUniqueKey(0)->getProperties());
//  }
//  else
//  {
//    throw Exception(TR_PGIS("Can not remove dataset items because dataset doesn't have a primary key or unique key!")); 
//  }
//
//// create a prepared statement
//  std::string sql  = "DELETE FROM ";
//              sql += dt->getName();
//              sql += " WHERE ";
//              sql += GetBindableWhereSQL(*keyProperties);
//
//  std::vector<std::size_t> propertiesPos;
//  
//  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  te::da::ScopedTransaction st(*m_t);
//
//  pq->prepare(sql, *keyProperties);
//
//  do
//  {
//    pq->bind(propertiesPos, dt, d);
//    pq->execute();
//
//  }while(d->moveNext());
//
//  st.commit();

//  const std::vector<te::dt::Property*>* keyProperties = 0;
// 
//  if(dt->getPrimaryKey())
//  {
//    keyProperties = &(dt->getPrimaryKey()->getProperties());
//  }
//  else if(dt->getNumberOfUniqueKeys() > 0)
//  {
//    keyProperties = &(dt->getUniqueKey(0)->getProperties());
//  }
//  else
//  {
//    throw Exception(TR_PGIS("Can not remove dataset item because dataset type doesn't have a primary key or unique key!")); 
//  }
//
//// create a prepared statement
//  std::string sql  = "DELETE FROM ";
//              sql += dt->getName();
//              sql += " WHERE ";
//              sql += GetBindableWhereSQL(*keyProperties);
//
//  std::vector<std::size_t> propertiesPos;
//  
//  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  pq->prepare(sql, *keyProperties);
//  pq->bind(propertiesPos, dt, item);
//  pq->execute();
}

void te::pgis::DataSourceTransactor::update(const std::string& /*datasetName*/,
                                            te::da::DataSet* /*dataset*/,
                                            const std::vector<std::size_t>& /*properties*/,
                                            const te::da::ObjectIdSet* /*oids*/,
                                            const std::map<std::string, std::string>& /*options*/,
                                            std::size_t /*limit*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));

//  const std::vector<te::dt::Property*>* keyProperties = 0;
// 
//  if(dt->getPrimaryKey())
//  {
//    keyProperties = &(dt->getPrimaryKey()->getProperties());
//  }
//  else if(dt->getNumberOfUniqueKeys() > 0)
//  {
//    keyProperties = &(dt->getUniqueKey(0)->getProperties());
//  }
//  else
//  {
//    throw Exception(TR_PGIS("Can not update dataset item(s) because dataset doesn't have a primary key or unique key!")); 
//  }
//
//// create a prepared statement
//  std::string sql  = "UPDATE ";
//              sql += dt->getName();
//              sql += " SET ";
//              sql += GetBindableUpdateSQL(properties);
//              sql += " WHERE ";
//              sql += GetBindableWhereSQL(*keyProperties, properties.size());
//
//  std::vector<std::size_t> propertiesPos;
//
//  te::dt::GetPropertiesPosition(properties, dt, propertiesPos);
//
//  te::dt::GetPropertiesPosition(*keyProperties, dt, propertiesPos);
//
//  std::auto_ptr<PreparedQuery> pq(m_t->getPGPrepared("a" + boost::lexical_cast<std::string>((boost::int64_t)(this))));
//
//  std::vector<te::dt::Property*> allprops(properties);
//
//  std::copy(keyProperties->begin(), keyProperties->end(), allprops.end());
//
//  te::da::ScopedTransaction st(*m_t);
//
//  pq->prepare(sql, allprops);
//  
//  do
//  {
//    pq->bind(propertiesPos, dt, dataset);
//    pq->execute();
//
//  }while(dataset->moveNext());
//
//  st.commit();
}

void te::pgis::DataSourceTransactor::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

unsigned int te::pgis::DataSourceTransactor::getGeomTypeId()
{
  std::string sql("SELECT oid FROM pg_type WHERE typname = 'geometry'");

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    return 0;

  unsigned int id = result->getInt32(0);

  return id;
}

unsigned int te::pgis::DataSourceTransactor::getRasterTypeId()
{
  std::string sql("SELECT oid FROM pg_type WHERE typname = 'raster'");

  std::auto_ptr<te::da::DataSet> result(m_ds->query(sql));

  if(result->moveNext() == false)
    return 0;

  unsigned int id = result->getInt32(0);

  return id;
}

void te::pgis::DataSourceTransactor::getDatabaseInfo(std::string& currentSchema)
{
  std::string sql("SELECT current_schema()");

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    Exception(TR_PGIS("Could not get information about PostgreSQL database backend!"));

  currentSchema = result->getString(0);
}

void te::pgis::DataSourceTransactor::getGeometryInfo(const std::string& datasetName, te::gm::GeometryProperty* gp)
{
  std::string sql = "SELECT g.coord_dimension, g.srid, g.type "
                    "FROM geometry_columns g "
                    "WHERE g.f_table_name = '";

  std::string tname, sname;
  
  SplitTableName(datasetName, &(m_ds->getCurrentSchema()), sname, tname);

  sql += tname;
  sql += "' AND g.f_table_schema = '";
  sql += sname;
  sql += "' AND f_geometry_column = '";
  sql += gp->getName();
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext())
  {
    //int cdim = result->getInt(0);
    int srid = result->getInt32(1);
    te::gm::GeomType t = te::gm::Geometry::getGeomTypeId(result->getString(2));
    //gp->setCoordDimension(cdim);
    gp->setSRID(srid);
    gp->setGeometryType(t);
  }
  else
  {
// Don't throw: someone can create a geometry column without using AddGeometryColumn function!!
    gp->setSRID(-1);
    gp->setGeometryType(te::gm::GeometryType);
  }
}

void te::pgis::DataSourceTransactor::getRasterInfo(const std::string& datasetName, te::rst::RasterProperty* rp)
{
  std::string sql = "SELECT * FROM raster_columns as r WHERE r.r_table_name = '";

  std::string tname, sname;

  SplitTableName(datasetName, &(m_ds->getCurrentSchema()), sname, tname);

  sql += tname;
  sql += "' AND r.r_table_schema = '";
  sql += sname;
  sql += "' AND r_raster_column = '";
  sql += rp->getName();
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext())
  {
    int srid = result->getInt32("srid");

    double scale_x = result->getDouble("scale_x");

    double scale_y = result->getDouble("scale_y");

    int blocksize_x = result->getInt32("blocksize_x");

    int blocksize_y = result->getInt32("blocksize_y");

    //bool regular_blocking = result->getBool("regular_blocking");

    int nbands = result->getInt32("num_bands");

    std::auto_ptr<te::dt::Array> pixel_types(result->getArray("pixel_types"));

    std::auto_ptr<te::dt::Array> nodata_values(result->getArray("nodata_values"));

    std::auto_ptr<te::gm::Geometry> g(result->getGeometry("extent"));

    const te::gm::Envelope* e = g->getMBR();

    std::auto_ptr<te::rst::Grid> grid(new te::rst::Grid(scale_x, scale_y, new te::gm::Envelope(*e), srid));

    rp->set(grid.release());

    for(int i = 0; i != nbands; ++i)
    {
      std::vector<std::size_t> pos(1, i);

      std::string st = pixel_types->getData(pos)->toString();

      int t = te::dt::UNKNOWN_TYPE;

      if(st == "8BI")
        t = te::dt::CHAR_TYPE;
      else if(st == "8BUI")
        t = te::dt::UCHAR_TYPE;
      else if(st == "16BI")
        t = te::dt::INT16_TYPE;
      else if(st == "16BUI")
        t = te::dt::UINT16_TYPE;
      else if(st == "32BI")
        t = te::dt::INT32_TYPE;
      else if(st == "32BUI")
        t = te::dt::UINT32_TYPE;
      else if(st == "32BF")
        t = te::dt::FLOAT_TYPE;
      else if(st == "64BF")
        t = te::dt::DOUBLE_TYPE;
      else
        throw Exception(TR_PGIS("Band data type not supported by PostGIS driver!"));

      te::rst::BandProperty* bp = new te::rst::BandProperty(i, t);

      bp->m_blkh = blocksize_y;

      bp->m_blkw = blocksize_x;

      te::dt::AbstractData* ab = nodata_values->getData(pos);

      if(ab)
        bp->m_noDataValue = static_cast<te::dt::Double*>(ab)->getValue();

      rp->add(bp);
    }
  }
  else
  {
    //throw Exception(TR_PGIS("We must add support for rasters that don't have constraints!"));
  }
}

std::string te::pgis::DataSourceTransactor::getFullName(const std::string& name)
{
  std::string fullName = name;

  if(fullName.find(".") == std::string::npos)
    fullName = m_ds->getCurrentSchema() + "." + name;

  return fullName;
}

/////////// Protected methods

unsigned int te::pgis::DataSourceTransactor::getDataSetId(const std::string& tableName)
{
 std::string tname, sname;
  
  SplitTableName(tableName, &(m_ds->getCurrentSchema()), sname, tname);

  std::string sql("SELECT pg_class.oid "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND lower(pg_class.relname) = '");

  sql += te::common::Convert2LCase(tname);
  sql += "' AND lower(pg_namespace.nspname) = '";
  sql += te::common::Convert2LCase(sname);
  sql += "'";

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find the table oid!"));

  unsigned int tableid = result->getInt32(0);

  return tableid;
}

std::string te::pgis::DataSourceTransactor::getDataSetName(unsigned int id)
{
  std::string sql("SELECT pg_namespace.nspname, pg_class.relname "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND pg_class.oid = ");

  sql += te::common::Convert2String(id);

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find the dataset name!"));

  std::string tname  = result->getString(0);
              tname += ".";
              tname += result->getString(1);

  return tname;
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSourceTransactor::getPropertiesInfo(const std::string& datasetName)
{
  std::string fullDatasetName = getFullName(datasetName);

  unsigned int dtid = getDataSetId(fullDatasetName);

  std::string sql("SELECT a.attnum, a.attname, t.oid, a.attnotnull, format_type(a.atttypid, a.atttypmod), a.atthasdef, pg_get_expr(d.adbin, d.adrelid), a.attndims "
                  "FROM pg_attribute AS a INNER JOIN pg_type AS t ON (a.atttypid = t.oid) LEFT JOIN pg_attrdef AS d ON (a.attrelid = d.adrelid AND a.attnum = d.adnum) "
                  "WHERE a.attrelid = ");
  sql += te::common::Convert2String(dtid);
  sql += " AND a.attisdropped = false"
         " AND a.attnum > 0"
         " ORDER BY a.attnum";

  return query(sql);
}

void te::pgis::DataSourceTransactor::getPropertyId(te::dt::Property* p)
{
  if((p->getParent()==0) || (p->getParent()->getType() != te::dt::DATASET_TYPE))
    throw Exception(TR_PGIS("The informed property is not valid!"));

  std::string sql("SELECT attnum "
                  "FROM pg_attribute WHERE attrelid = ");
  sql += te::common::Convert2String(p->getParent()->getId());
  sql += " AND attisdropped = false "
         "AND attname = '";
  sql += te::common::Convert2LCase(p->getName());
  sql += "' ";

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find the property ID!"));
  
  unsigned int id = result->getInt32(0);
  p->setId(id);
}

std::auto_ptr<te::dt::Property> te::pgis::DataSourceTransactor::getProperty(unsigned int pid, const std::string& datasetName)
{
  unsigned int dtid = getDataSetId(datasetName);

  std::string sql("SELECT a.attnum, a.attname, t.oid, a.attnotnull, format_type(a.atttypid, a.atttypmod), a.atthasdef, pg_get_expr(d.adbin, d.adrelid), a.attndims "
                  "FROM pg_attribute AS a INNER JOIN pg_type AS t ON (a.atttypid = t.oid) LEFT JOIN pg_attrdef AS d ON (a.attrelid = d.adrelid AND a.attnum = d.adnum) "
                  "WHERE a.attrelid = ");
              sql += te::common::Convert2String(dtid);
              sql += " AND a.attnum = ";
              sql += te::common::Convert2String(pid);
              sql += " AND a.attisdropped = false"
                     " AND a.attnum > 0";

  std::auto_ptr<te::da::DataSet> result(query(sql));

  std::auto_ptr<te::dt::Property> p(0);

  if(result->moveNext())
  {
    unsigned int attNum = result->getInt16(0);
    std::string attName = result->getString(1);
    unsigned int attType = result->getInt32(2);
    bool attNotNull = result->getBool(3);
    std::string fmt = result->getString(4);
    bool attHasDefault = result->getBool(5);
    std::string attDefValue = result->getString(6);
    int ndims = result->getInt32(7);

    p.reset(Convert2TerraLib(attNum, attName.c_str(), attType, attNotNull, fmt.c_str(), attHasDefault,
                             attDefValue.c_str(),ndims, m_ds->getGeomTypeId(), m_ds->getRasterTypeId()));

    if(p->getType() == te::dt::GEOMETRY_TYPE)
      getGeometryInfo(datasetName, static_cast<te::gm::GeometryProperty*>(p.get()));
  }

  return p;
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSourceTransactor::getConstraints(const std::string& datasetName, char conType)
{
  unsigned int dtid = getDataSetId(datasetName);

  std::string sql("SELECT c.oid, n.nspname, c.conname, c.contype, c.confrelid, c.confupdtype, c.confdeltype, c.confmatchtype, c.conkey, c.confkey, pg_get_constraintdef(c.oid) "
                  "FROM pg_constraint c, pg_namespace n "
                  "WHERE c.connamespace = n.oid "
                  "AND c.conrelid = ");
  sql += te::common::Convert2String(dtid);

  if(conType != '\0')
  {
    sql += " AND c.contype = '";
    sql += conType;
    sql += "'";
  }

  return query(sql);
}

void te::pgis::DataSourceTransactor::getConstraints(te::da::DataSetTypePtr& dt)
{
  std::string datasetName = dt->getName();
  unsigned int dtid = dt->getId();

  std::string sql("SELECT c.oid, n.nspname, c.conname, c.contype, c.confrelid, c.confupdtype, c.confdeltype, c.confmatchtype, c.conkey, c.confkey, pg_get_constraintdef(c.oid) "
                  "FROM pg_constraint c, pg_namespace n "
                  "WHERE c.connamespace = n.oid "
                  "AND c.conrelid = ");
  sql += te::common::Convert2String(dtid);

  std::auto_ptr<te::da::DataSet> cInfo = query(sql);

  while(cInfo->moveNext())
  {
    char cType = cInfo->getChar(3);
    if(cType == 'p')
    {
      // begin of the dataset primary key
      unsigned int pkId = cInfo->getInt32(0);
      std::string pkName  = cInfo->getString(2);

      te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, 0, pkId);

      std::auto_ptr<te::dt::Array> pkCols(cInfo->getArray(8));
      std::size_t size = pkCols->getDimensionSize(0);

      std::vector<std::size_t> pos;
      pos.push_back(0);
      for(std::size_t i = 0; i < size; ++i)
      {
        pos[0] = i;

        te::dt::AbstractData* pkCol = pkCols->getData(pos);

        te::dt::Property* p = dt->getPropertyById(static_cast<te::dt::Int16*>(pkCol)->getValue());
        pk->add(p);
      }

      // Try to link the pk to an index
      std::vector<std::string> idxNames = getIndexNames(datasetName);

      for(std::size_t i = 0; i < idxNames.size(); ++i)
      {
        if(pk->getName() == idxNames[i])
        {
          pk->setAssociatedIndex(getIndex(datasetName, idxNames[i]).get());
          break;
        }
      }

      // Add the primary key to the schema
      dt->add(pk);

    } // end of the dataset primary key
    else if (cType == 'f')
    {
      // begin of foreign key constraint
      unsigned int fkId = cInfo->getInt32(0);
      unsigned int refDatasetId = cInfo->getInt32(4);
      char onUpdate = cInfo->getChar(5);
      char onDeletion = cInfo->getChar(6);

      std::auto_ptr<te::dt::Array> fkCols(cInfo->getArray(8));
      std::auto_ptr<te::dt::Array> fkRefCols(cInfo->getArray(9));

      assert(fkCols->getDimension() == 1);
      assert(fkCols->getDimension() == fkRefCols->getDimension());
      assert(fkCols->getDimensionSize(0) == fkRefCols->getDimensionSize(0));

      std::string refName = getDataSetName(refDatasetId);

      const te::da::DataSetTypePtr& refDatasetType = getDataSetType(refName);

      std::string fkName  = cInfo->getString(2);

      te::da::ForeignKey* fk = new te::da::ForeignKey(fkName, fkId);
      fk->setOnUpdateAction(GetAction(onUpdate));
      fk->setOnDeleteAction(GetAction(onDeletion));
      fk->setReferencedDataSetType(refDatasetType.get());
  
      std::size_t size = fkCols->getDimensionSize(0);

      std::vector<std::size_t> pos;
      pos.push_back(0);

      for(std::size_t i = 0; i < size; ++i)
      {
        pos[0] = i;

        te::dt::AbstractData* fkRefCol = fkRefCols->getData(pos);
        fk->addRefProperty(refDatasetType->getPropertyById(static_cast<te::dt::Int16*>(fkRefCol)->getValue()));

        te::dt::AbstractData* fkCol = fkCols->getData(pos);
        fk->add(getProperty(static_cast<te::dt::Int16*>(fkCol)->getValue(), datasetName).release());
      }

      // Add the foreign key to the schema
      dt->add(fk);

    }  // end of foreign key constraint
    else if(cType == 'u')
    {
      // begin of unique key constraint
      unsigned int ukId = cInfo->getInt32(0);
      std::string ukName  = cInfo->getString(2);

      std::auto_ptr<te::dt::Array> ukCols(cInfo->getArray(8));

      te::da::UniqueKey* uk = new te::da::UniqueKey(ukName, 0, ukId);

      std::size_t size = ukCols->getDimensionSize(0);

      std::vector<std::size_t> pos;
      pos.push_back(0);

      for(std::size_t i = 0; i < size; ++i)
      {
        pos[0] = i;

        te::dt::AbstractData* ukCol = ukCols->getData(pos);

        std::auto_ptr<te::dt::Property> p = getProperty(static_cast<te::dt::Int16*>(ukCol)->getValue(), datasetName);
        uk->add(p.release());
      }

      // Try to link the uk to an index
      std::vector<std::string> idxNames = getIndexNames(datasetName);

      for(std::size_t i = 0; i < idxNames.size(); ++i)
      {
        if(uk->getName() == idxNames[i])
        {
          uk->setAssociatedIndex(getIndex(datasetName, idxNames[i]).get());
          break;
        }
      }

      // Add the unique key to the schema
      dt->add(uk);

    }  // end of the unique key constraint
    else if(cType == 'c')
    {
      // begin of check constraint
      std::string ccName = cInfo->getString(2);

      unsigned int ccId = cInfo->getInt32(0);

      te::da::CheckConstraint* cc = new te::da::CheckConstraint(ccName);
      cc->setId(ccId);
      cc->setExpression(cInfo->getString(10));

      // Add the check constraint to the schema
      dt->add(cc);

    }  // end of check constraint
  }    // end of moveNext
}

void te::pgis::DataSourceTransactor::getIndexes(te::da::DataSetTypePtr& dt)
{
  std::string datasetName = dt->getName();
  unsigned int dtid = dt->getId();

  std::string sql("SELECT idx_table.oid, s.nspname, idx_table.relname, pg_index.indkey, pg_am.amname, pg_index.indisunique, pg_index.indisprimary " 
                  "FROM pg_index, pg_class idx_table, pg_am, pg_namespace s "
                  "WHERE s.oid = idx_table.relnamespace "
                  "AND pg_index.indexrelid = idx_table.oid "
                  "AND idx_table.relam = pg_am.oid "
                  "AND pg_index.indrelid = ");
  sql += te::common::Convert2String(dtid);

  std::auto_ptr<te::da::DataSet> idxInfo = query(sql);

  while(idxInfo->moveNext())
  {
    unsigned int idxId = idxInfo->getInt32(0);
    std::string idxName = idxInfo->getString(2);

    std::auto_ptr<te::dt::Array> idxCols(idxInfo->getArray(3));

    std::string idxType = idxInfo->getString(4);
    bool isUK = idxInfo->getBool(5);
    bool isPK = idxInfo->getBool(6);

    te::da::Index* idx = new te::da::Index(idxName, GetIndexType(idxType.c_str()), dt.get(), idxId);

    std::size_t size = idxCols->getDimensionSize(0);

    std::vector<std::size_t> pos;
    pos.push_back(0);

    for(std::size_t i = 0; i < size; ++i)
    {
      pos[0] = i;
      te::dt::AbstractData* idxCol = idxCols->getData(pos);

      idx->add(dt->getPropertyById(static_cast<te::dt::Int16*>(idxCol)->getValue()));
    }

    // Look if there is an association with a pk and uk
    idxName = idxInfo->getString(2);
    if(isPK && dt->getPrimaryKey() && (dt->getPrimaryKey()->getName() == idxName))
    {
      dt->getPrimaryKey()->setAssociatedIndex(idx);
    }
    else if(isUK)
    {
      te::da::UniqueKey* uk = dt->getUniqueKey(idxName);

      if(uk)
        uk->setAssociatedIndex(idx);
    }
  }
}

std::vector<te::da::Sequence*> te::pgis::DataSourceTransactor::getSequences()
{
  std::vector<te::da::Sequence*> seqs;

  // Query the data source for the sequences
  std::vector<std::string> seqNames;

  std::string sql("SELECT c.oid, n.nspname, c.relname, c.relkind "
                  "FROM pg_class c, pg_namespace n "
                  "WHERE c.relname !~ '^pg_' "
                  "AND c.relkind = 'S' "
                  "AND c.relnamespace = n.oid "
                  "AND n.nspname NOT IN ('information_schema', 'pg_toast', 'pg_temp_1', 'pg_catalog')");

  std::auto_ptr<te::da::DataSet> seqNamesInfo = query(sql);

  while(seqNamesInfo->moveNext())
  {
    std::string seqName(seqNamesInfo->getString(2));
    seqNames.push_back(seqName);
  }

  for(std::size_t i = 0; i < seqNames.size(); ++i)
  {
    std::string seqName = seqNames[i];

    std::string sql("SELECT * FROM ");
    sql += seqName;

    std::auto_ptr<te::da::DataSet> result(query(sql));

    if(result->moveNext() == false)
      throw Exception((boost::format(TR_PGIS("There is no information about the sequence \"%1%\"!")) % seqName).str());

    unsigned int seqId = getDataSetId(seqName);

    te::da::Sequence* seq = new te::da::Sequence(seqName, 0, 0, 0, seqId);

    // Check if the sequence is cycled
    if(result->getBool(8))
      seq->setAsCycle();
    else
      seq->setAsNoCycle();

    seq->setCachedValues(result->getInt64(6)); // "cache_value"
    seq->setIncrement(result->getInt64(3));    // "increment_by";
    seq->setMaxValue(result->getInt64(4));     // "max_value";
    seq->setMinValue(result->getInt64(5));     // "min_value";

    seqs.push_back(seq);
  }

  return seqs;
}
