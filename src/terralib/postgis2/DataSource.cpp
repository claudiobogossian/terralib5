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
  \file terralib/postgis/DataSource.cpp

  \brief Implementation of the data source for the PostGIS driver.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/dataset/CheckConstraint.h"
#include "../dataaccess2/dataset/DataSet.h"
#include "../dataaccess2/dataset/ForeignKey.h"
#include "../dataaccess2/dataset/Index.h"
#include "../dataaccess2/dataset/PrimaryKey.h"
#include "../dataaccess2/dataset/UniqueKey.h"
#include "../dataaccess2/datasource/DataSourceCatalog.h"
#include "../dataaccess2/query/Select.h"
#include "../dataaccess2/query/SQLDialect.h"
#include "../dataaccess2/utils/Utils.h"
#include "../datatype/Array.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../geometry/GeometryProperty.h"
#include "../raster/Grid.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterProperty.h"
#include "../geometry/Geometry.h"
#include "Connection.h"
#include "ConnectionPool.h"
#include "DataSource.h"
#include "DataSet.h"
//#include "DataTypes.h"
#include "Exception.h"
#include "Globals.h"
#include "SQLVisitor.h"
#include "Utils.h"

// STL
#include <cassert>
//#include <cstring>
#include <memory>

// Boost
//#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/thread.hpp>

class te::pgis::DataSource::Impl
{
  public:

    Impl(te::pgis::DataSource* ds)
      : m_ds(ds),
        m_catalog(0),
        m_pool(0),
        m_conn(0),
        m_geomTypeOid(0),
        m_rasterTypeOid(0),
        m_currentSchema(0),
        m_timeIsInteger(true),
        m_isInTransaction(false)
    {
      m_catalog = new te::da::DataSourceCatalog;
      m_catalog->setDataSource(m_ds);
      m_pool = new ConnectionPool(m_ds);
    }

    ~Impl()
    {
      delete m_catalog;
      delete m_pool;
      delete m_currentSchema;
    }

    unsigned int getGeomTypeId()
    {
      std::string sql("SELECT oid FROM pg_type WHERE typname = 'geometry'");

      std::auto_ptr<te::da::DataSet> result(m_ds->query(sql));

      if(result->moveNext() == false)
        return 0;

      unsigned int id = result->getInt32(0);

      return id;
    }

    unsigned int getRasterTypeId()
    {
      std::string sql("SELECT oid FROM pg_type WHERE typname = 'raster'");

      std::auto_ptr<te::da::DataSet> result(m_ds->query(sql));

      if(result->moveNext() == false)
        return 0;

      unsigned int id = result->getInt32(0);

      return id;
    }

    te::pgis::DataSource* m_ds;                       //!< The data source which this class implements.
    te::da::DataSourceCatalog* m_catalog;             //!< The main system catalog.
    std::map<std::string, std::string> m_connInfo;    //!< Connection information.
    ConnectionPool* m_pool;                           //!< The connection pool.
    Connection* m_conn;                               //!< The connection to the PostGIS data source.
    unsigned int m_geomTypeOid;                       //!< PostGIS Geometry type OID.
    unsigned int m_rasterTypeOid;                     //!< PostGIS Raster type OID.
    std::string* m_currentSchema;                     //!< The default schema used when no one is provided.
    bool m_timeIsInteger;                             //!< It indicates if the postgis stores, internally, time and timestamp as an integer. 
    bool m_isInTransaction;                           //!< It indicates if there is a transaction in progress.

    static te::da::DataSourceCapabilities sm_capabilities;  //!< PostGIS capabilities.
    static te::da::SQLDialect* sm_dialect;                  //!< PostGIS SQL dialect.
};


te::da::DataSourceCapabilities te::pgis::DataSource::Impl::sm_capabilities;
te::da::SQLDialect* te::pgis::DataSource::Impl::sm_dialect(0);


te::pgis::DataSource::DataSource()
  : m_pImpl(0)
{
  m_pImpl = new Impl(this);
}

te::pgis::DataSource::~DataSource()
{
  delete m_pImpl;
}

std::string te::pgis::DataSource::getType() const throw()
{
  return PGIS_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::pgis::DataSource::getConnectionInfo() const throw()
{
  return m_pImpl->m_connInfo;
}

void te::pgis::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo) throw()
{
  m_pImpl->m_connInfo = connInfo;
}

bool te::pgis::DataSource::isTimeAnInteger() throw()
{
  return m_pImpl->m_timeIsInteger;
}

void te::pgis::DataSource::setTimeAsInteger(bool timeIsInteger) throw()
{
  m_pImpl->m_timeIsInteger = timeIsInteger;
}

void te::pgis::DataSource::open() throw(te::da::Exception)
{
  // Assure we are in a closed state
  close();

  m_pImpl->m_pool->initialize();
  m_pImpl->m_conn = m_pImpl->m_pool->getConnection();

  // Find PostGIS types
  m_pImpl->m_geomTypeOid = m_pImpl->getGeomTypeId();
  m_pImpl->m_rasterTypeOid = m_pImpl->getRasterTypeId();

  if(m_pImpl->m_currentSchema == 0)
    m_pImpl->m_currentSchema = new std::string;

  // Find PostgreSQL current schema of the connection
  getDatabaseInfo(*m_pImpl->m_currentSchema);
}

void te::pgis::DataSource::close() throw(te::da::Exception)
{
  if(m_pImpl->m_conn)
    m_pImpl->m_pool->release(m_pImpl->m_conn);

  m_pImpl->m_pool->finalize();

  delete m_pImpl->m_currentSchema;
  m_pImpl->m_currentSchema = 0;
}

bool te::pgis::DataSource::isOpened() const throw()
{
  return m_pImpl->m_pool->isInitialized();
}

bool te::pgis::DataSource::isValid() const throw()
{
  return m_pImpl->m_pool->isValid();
}

const te::da::DataSourceCapabilities& te::pgis::DataSource::getCapabilities() const throw()
{
  return Impl::sm_capabilities;
}

const te::da::SQLDialect* te::pgis::DataSource::getDialect() const throw()
{
  return Impl::sm_dialect;
}

void te::pgis::DataSource::begin() throw(te::da::Exception)
{
  execute("BEGIN");
  m_pImpl->m_isInTransaction = true;
}

void te::pgis::DataSource::commit() throw(te::da::Exception)
{
  m_pImpl->m_isInTransaction = false;
  execute("COMMIT");
}

void te::pgis::DataSource::rollBack() throw(te::da::Exception)
{
  m_pImpl->m_isInTransaction = false;
  execute("ROLLBACK");
}

bool te::pgis::DataSource::isInTransaction() const throw()
{
  return m_pImpl->m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getDataSet(const std::string& name,
                                                                te::common::TraverseType /*travType*/) throw(te::da::Exception)
{
   std::auto_ptr<std::string> sql(new std::string("SELECT * FROM "));
                             *sql += name;

  PGresult* result = m_pImpl->m_conn->query(*sql);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, sql.release()));
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getDataSet(const std::string& name,
                                                                const std::string& propertyName,
                                                                const te::gm::Envelope* e,
                                                                te::gm::SpatialRelation r,
                                                                te::common::TraverseType travType) throw(te::da::Exception)
{
  if(e == 0)
    throw Exception(TR_PGIS("The envelope is missing!"));

  std::auto_ptr<te::dt::Property> p = getProperty(name, propertyName);

  //const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p);
  const te::gm::GeometryProperty* gp = static_cast<const te::gm::GeometryProperty*>(p.release());

  std::string rel = GetBoxSpatialRelation(r);

  std::string sql("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";
              sql += propertyName;
              sql += rel;

  Convert2PostGIS(e, gp->getSRID(), sql);

  PGresult* result = m_pImpl->m_conn->query(sql);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, new std::string(sql)));
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getDataSet(const std::string& name,
                                                                const std::string& propertyName,
                                                                const te::gm::Geometry* g,
                                                                te::gm::SpatialRelation r,
                                                                te::common::TraverseType travType) throw(te::da::Exception)
{
 if(g == 0)
    throw Exception(TR_PGIS("The geometry is missing!"));

  std::string rel = GetSpatialRelation(r);

  std::string sql("SELECT * FROM ");
              sql += name;
              sql += " WHERE ";
              sql += rel;
              sql += "(";

  Convert2PostGIS( m_pImpl->m_conn->m_pgconn, g, sql);

  sql += ",";
  sql += propertyName;
  sql += ")";

  PGresult* result = m_pImpl->m_conn->query(sql);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, new std::string(sql)));
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::query(const te::da::Select& q,
                                                           te::common::TraverseType travType) throw(te::da::Exception)
{
  std::string sql;

  SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  q.accept(visitor);
 
  return query(sql, travType);
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::query(const std::string& query, 
                                                           te::common::TraverseType travType) throw(te::da::Exception)
{
  PGresult* result = m_pImpl->m_conn->query(query);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, new std::string(query)));
}

void te::pgis::DataSource::execute(const te::da::Query& command) throw(te::da::Exception)
{
  std::string sql;

  SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  command.accept(visitor);
 
  execute(sql);
}

void te::pgis::DataSource::execute(const std::string& command) throw(te::da::Exception)
{
  m_pImpl->m_conn->execute(command);
}

void te::pgis::DataSource::cancel() throw(te::da::Exception)
{
}

boost::int64_t te::pgis::DataSource::getLastGeneratedId() throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

unsigned int te::pgis::DataSource::getGeomTypeId()
{
  return m_pImpl->m_geomTypeOid;
}

unsigned int te::pgis::DataSource::getRasterTypeId()
{
  return m_pImpl->m_rasterTypeOid;
}

const std::string* te::pgis::DataSource::getCurrentSchema()
{
  return m_pImpl->m_currentSchema;
}

void te::pgis::DataSource::getDatabaseInfo(std::string& currentSchema)
{
 std::string sql("SELECT current_schema()");

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    Exception(TR_PGIS("Could not get information about PostgreSQL database backend!"));

  currentSchema = result->getString(0);
}

std::string te::pgis::DataSource::escape(const std::string& value) throw(te::da::Exception)
{
  return value;
}

bool te::pgis::DataSource::isDataSetNameValid(const std::string& /*datasetName*/) throw(te::da::Exception)
{
  return true;
}

bool te::pgis::DataSource::isPropertyNameValid(const std::string& /*propertyName*/) throw(te::da::Exception)
{
  return true;
}

std::vector<std::string> te::pgis::DataSource::getDataSetNames() throw(te::da::Exception)
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

const te::da::DataSetTypePtr& te::pgis::DataSource::getDataSetType(const std::string& name) throw(te::da::Exception)
{
  if(!datasetExists(name))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % name).str());

  if(m_pImpl->m_catalog->datasetExists(name))
    return m_pImpl->m_catalog->getDataSetType(name);

  // Find the dataset id
  unsigned int dtid = getTableId(name);
  
  // Create the dataset type
  te::da::DataSetTypePtr dt(new te::da::DataSetType(name, dtid));
  dt->setTitle(name);

  // Get the properties of the dataset and add them to its schema
  boost::ptr_vector<te::dt::Property> properties = getProperties(name);
  dt->add(properties);

  // Get the check constraints of the dataset and add them to its schema
  std::vector<std::string> ccNames = getCheckConstraintNames(name);

  std::vector<te::da::CheckConstraint*> ccVec(ccNames.size());
  for(std::size_t i = 0; i < ccNames.size(); ++i)
  {
    ccVec[i] = getCheckConstraint(name, ccNames[i]).release();
    dt->add(ccVec[i]);
  }

  // Get the indexes of the dataset and add them to its schema
  std::vector<std::string> indexNames = getIndexNames(name);

  std::vector<te::da::Index*> indexVec(indexNames.size());
  for(std::size_t i = 0; i < indexNames.size(); ++i)
  {
    indexVec[i] = getIndex(name, indexNames[i]).release();
    dt->add(indexVec[i]);
  }

  // Add the dataset schema to the catalog
  m_pImpl->m_catalog->add(dt);

  return m_pImpl->m_catalog->getDataSetType(name);
}

std::size_t te::pgis::DataSource::getNumberOfProperties(const std::string& datasetName) throw(te::da::Exception)
{
  return getProperties(datasetName).size();
}

boost::ptr_vector<te::dt::Property> te::pgis::DataSource::getProperties(const std::string& datasetName) throw(te::da::Exception)
{
  boost::ptr_vector<te::dt::Property> properties;

  unsigned int dtid = getTableId(datasetName);

  std::auto_ptr<te::da::DataSet> result(getProperties(dtid));

  while(result->moveNext())
  {
    unsigned int attNum = result->getInt16(0);
    std::string attName = result->getString(1);
    unsigned int attType = result->getInt32(2);
    bool attNotNull = result->getBool(3);
    std::string fmt = result->getString(4);
    bool attHasDefault = result->getBool(5);
    std::string attDefValue = result->getString(6);
    int ndims = result->getInt32(7);

    te::dt::Property* p =  Convert2TerraLib(attNum, attName.c_str(), attType, attNotNull,
                                            fmt.c_str(), attHasDefault, attDefValue.c_str(),
                                            ndims, getGeomTypeId(), getRasterTypeId());

    properties.push_back(p);

    if(p->getType() == te::dt::GEOMETRY_TYPE)
      getGeometryInfo(datasetName, static_cast<te::gm::GeometryProperty*>(p));
    else if(p->getType() == te::dt::RASTER_TYPE)
      getRasterInfo(datasetName, static_cast<te::rst::RasterProperty*>(p));
  }

  return properties;
}

std::auto_ptr<te::dt::Property> te::pgis::DataSource:: getProperty(const std::string& datasetName,
                                                                  const std::string& propertyName) throw(te::da::Exception)
{
  unsigned int dtid = getTableId(datasetName);

  std::string sql("SELECT a.attnum, a.attname, t.oid, a.attnotnull, format_type(a.atttypid, a.atttypmod), a.atthasdef, pg_get_expr(d.adbin, d.adrelid), a.attndims "
                  "FROM pg_attribute AS a INNER JOIN pg_type AS t ON (a.atttypid = t.oid) LEFT JOIN pg_attrdef AS d ON (a.attrelid = d.adrelid AND a.attnum = d.adnum) "
                  "WHERE a.attrelid = ");
              sql += te::common::Convert2String(dtid);
              sql += " AND a.attisdropped = false"
                     " AND a.attnum > 0"
                     " AND a.attname = propertyName";

  std::auto_ptr<te::da::DataSet> result(query(sql).release());

  te::dt::Property* p;

  while(result->moveNext())
  {
    unsigned int attNum = result->getInt16(0);
    std::string attName = result->getString(1);
    unsigned int attType = result->getInt32(2);
    bool attNotNull = result->getBool(3);
    std::string fmt = result->getString(4);
    bool attHasDefault = result->getBool(5);
    std::string attDefValue = result->getString(6);
    int ndims = result->getInt32(7);

    p =  Convert2TerraLib(attNum, attName.c_str(), attType, attNotNull,
                          fmt.c_str(), attHasDefault, attDefValue.c_str(),
                          ndims, getGeomTypeId(), getRasterTypeId());

    if(p->getType() == te::dt::GEOMETRY_TYPE)
      getGeometryInfo(datasetName, static_cast<te::gm::GeometryProperty*>(p));
    else if(p->getType() == te::dt::RASTER_TYPE)
      getRasterInfo(datasetName, static_cast<te::rst::RasterProperty*>(p));
  }

  return std::auto_ptr<te::dt::Property>(p);
}

std::auto_ptr<te::dt::Property> te::pgis::DataSource::getProperty(const std::string& datasetName,
                                                                  std::size_t propertyPos) throw(te::da::Exception)
{
  unsigned int dtid = getTableId(datasetName);

  std::string sql("SELECT a.attnum, a.attname, t.oid, a.attnotnull, format_type(a.atttypid, a.atttypmod), a.atthasdef, pg_get_expr(d.adbin, d.adrelid), a.attndims "
                  "FROM pg_attribute AS a INNER JOIN pg_type AS t ON (a.atttypid = t.oid) LEFT JOIN pg_attrdef AS d ON (a.attrelid = d.adrelid AND a.attnum = d.adnum) "
                  "WHERE a.attrelid = ");
              sql += te::common::Convert2String(dtid);
              sql += " AND a.attisdropped = false"
                     " AND a.attnum = propertyPos + 1";

  std::auto_ptr<te::da::DataSet> result(query(sql).release());

  te::dt::Property* p;

  while(result->moveNext())
  {
    unsigned int attNum = result->getInt16(0);
    std::string attName = result->getString(1);
    unsigned int attType = result->getInt32(2);
    bool attNotNull = result->getBool(3);
    std::string fmt = result->getString(4);
    bool attHasDefault = result->getBool(5);
    std::string attDefValue = result->getString(6);
    int ndims = result->getInt32(7);

    p =  Convert2TerraLib(attNum, attName.c_str(), attType, attNotNull,
                          fmt.c_str(), attHasDefault, attDefValue.c_str(),
                          ndims, getGeomTypeId(), getRasterTypeId());

    if(p->getType() == te::dt::GEOMETRY_TYPE)
      getGeometryInfo(datasetName, static_cast<te::gm::GeometryProperty*>(p));
    else if(p->getType() == te::dt::RASTER_TYPE)
      getRasterInfo(datasetName, static_cast<te::rst::RasterProperty*>(p));
  }

  return std::auto_ptr<te::dt::Property>(p);
}

std::auto_ptr<te::da::PrimaryKey> te::pgis::DataSource::getPrimaryKey(const std::string& datasetName) throw(te::da::Exception)
{
  unsigned int dtid = getTableId(datasetName);

  std::auto_ptr<te::da::DataSet> pkInfo(getConstraints(dtid, 'p'));

  unsigned int id = pkInfo->getInt32(0);

  std::string pkName  = pkInfo->getString(2);

  std::auto_ptr<te::dt::Array> pkCols(pkInfo->getArray(8));

  std::auto_ptr<te::da::PrimaryKey> pk(new te::da::PrimaryKey(pkName, 0, id));

  std::size_t size = pkCols->getDimensionSize(0);

  std::vector<std::size_t> pos;
  pos.push_back(0);

  for(std::size_t i = 0; i < size; ++i)
  {
    pos[0] = i;

    te::dt::AbstractData* pkCol = pkCols->getData(pos);

    //te::dt::Property* p = dt->getPropertyById(static_cast<te::dt::Int16*>(pkCol)->getValue());
    te::dt::Property* p = getProperty(dtid, static_cast<te::dt::Int16*>(pkCol)->getValue());

    if(p == 0) // property not found in the dataset type list
      return std::auto_ptr<te::da::PrimaryKey>(0);

    pk->add(p);
  }

  // Try to link the pk to an index
  std::vector<std::string> indexNames = getIndexNames(datasetName);

  for(std::size_t i = 0; i < indexNames.size(); ++i)
  {
    if(pk->getName() == indexNames[i])
    {
      pk->setAssociatedIndex(getIndex(datasetName, indexNames[i]).release());
      break;
    }
  }

  return std::auto_ptr<te::da::PrimaryKey>(pk);
}

std::vector<std::string> te::pgis::DataSource::getUniqueKeyNames(const std::string& datasetName) throw(te::da::Exception)
{
  std::vector<std::string> ukNames;

  unsigned int dtid = getTableId(datasetName);

  std::auto_ptr<te::da::DataSet> ukInfo(getConstraints(dtid, 'u'));

  while(ukInfo->moveNext())
  {
    std::string ukName  = ukInfo->getString(2);
    ukNames.push_back(ukName);
  }

  return ukNames;
}

boost::ptr_vector<te::da::UniqueKey> te::pgis::DataSource::getUniqueKeys(const std::string& datasetName) throw(te::da::Exception)
{
  boost::ptr_vector<te::da::UniqueKey> uks;

  unsigned int dtid = getTableId(datasetName);

  std::auto_ptr<te::da::DataSet> ukInfo(getConstraints(dtid, 'u'));

  while(ukInfo->moveNext())
  {
    unsigned int id = ukInfo->getInt32(0);

    std::string ukName  = ukInfo->getString(2);

    std::auto_ptr<te::dt::Array> ukCols(ukInfo->getArray(8));

    std::auto_ptr<te::da::UniqueKey> uk(new te::da::UniqueKey(ukName, 0, id));

    std::size_t size = ukCols->getDimensionSize(0);

    std::vector<std::size_t> pos;
    pos.push_back(0);

    for(std::size_t i = 0; i < size; ++i)
    {
      pos[0] = i;

      te::dt::AbstractData* ukCol = ukCols->getData(pos);

      te::dt::Property* p = getProperty(dtid, static_cast<te::dt::Int16*>(ukCol)->getValue());

      if(p == 0) // property not found
        return boost::ptr_vector<te::da::UniqueKey>(0);

      uk->add(p);
    }

    // Try to link the uk to an index
    std::vector<std::string> indexNames = getIndexNames(datasetName);

    for(std::size_t i = 0; i < indexNames.size(); ++i)
    {
      if(uk->getName() == indexNames[i])
      {
        uk->setAssociatedIndex(getIndex(datasetName, indexNames[i]).release());
        break;
      }
    }

    uks.push_back(uk);
  }

  return uks;
}

std::auto_ptr<te::da::UniqueKey> te::pgis::DataSource::getUniqueKey(const std::string& datasetName,
                                                                    const std::string& name) throw(te::da::Exception)
{
  // Check if there is a dataset with this name
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  // Check if there is an unique key with this name
  if(!uniquekeyExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The unique key %1% doesn't exist!")) % name).str());

  std::auto_ptr<te::da::UniqueKey> uk;

  unsigned int dtid = getTableId(datasetName);

  std::auto_ptr<te::da::DataSet> ukInfo(getConstraints(dtid, 'u'));

  while(ukInfo->moveNext())
  {
    unsigned int id = ukInfo->getInt32(0);

    std::string ukName  = ukInfo->getString(2);
    if(ukName == name)
    {
      std::auto_ptr<te::dt::Array> ukCols(ukInfo->getArray(8));

      uk.reset(new te::da::UniqueKey(ukName, 0, id));

      std::size_t size = ukCols->getDimensionSize(0);

      std::vector<std::size_t> pos;
      pos.push_back(0);

      for(std::size_t i = 0; i < size; ++i)
      {
        pos[0] = i;

        te::dt::AbstractData* ukCol = ukCols->getData(pos);

        te::dt::Property* p = getProperty(dtid, static_cast<te::dt::Int16*>(ukCol)->getValue());

        if(p == 0) // property not found
          return std::auto_ptr<te::da::UniqueKey>(0);

        uk->add(p);
      }

      // Try to link the uk to an index
      std::vector<std::string> indexNames = getIndexNames(datasetName);

      for(std::size_t i = 0; i < indexNames.size(); ++i)
      {
        if(uk->getName() == indexNames[i])
        {
          uk->setAssociatedIndex(getIndex(datasetName, indexNames[i]).release());
          break;
        }
      }

      break;
    }
  }

  return uk;
}

std::vector<std::string> te::pgis::DataSource::getForeignKeyNames(const std::string& datasetName) throw(te::da::Exception)
{
  // Check if there is a dataset with this name
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  std::vector<std::string> fkNames;

  unsigned int dtid = getTableId(datasetName);

  std::auto_ptr<te::da::DataSet> fkInfo(getConstraints(dtid, 'f'));

  while(fkInfo->moveNext())
  {
    std::string fkName  = fkInfo->getString(1);
    fkName += ".";
    fkName += fkInfo->getString(2);
    fkNames.push_back(fkName);
  }

  return fkNames;
}

std::auto_ptr<te::da::ForeignKey> te::pgis::DataSource::getForeignKey(const std::string& datasetName,
                                                                      const std::string& name) throw(te::da::Exception)
{
  // Check if there is a dataset with this name
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  // Check if there is an unique key with this name
  if(!foreignkeyExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The foreign key %1% doesn't exist!")) % name).str());

  unsigned int dtid = getTableId(datasetName);

  std::auto_ptr<te::da::DataSet> fkInfo(getConstraints(dtid, 'f'));

  unsigned int id = fkInfo->getInt32(0);
  unsigned int refDatasetId = fkInfo->getInt32(4);
  char onUpdate = fkInfo->getChar(5);
  char onDeletion = fkInfo->getChar(6);

  std::auto_ptr<te::dt::Array> fkCols(fkInfo->getArray(8));
  std::auto_ptr<te::dt::Array> fkRefCols(fkInfo->getArray(9));

  assert(fkCols->getDimension() == 1);
  assert(fkCols->getDimension() == fkRefCols->getDimension());
  assert(fkCols->getDimensionSize(0) == fkRefCols->getDimensionSize(0));

  std::string refName = getTableName(refDatasetId);

  const te::da::DataSetTypePtr& refDatasetType = m_pImpl->m_catalog->getDataSetType(refName);

  if(refDatasetType.get() == 0)
    return std::auto_ptr<te::da::ForeignKey>(0);

  std::string fkName  = fkInfo->getString(1);
  fkName += ".";
  fkName += fkInfo->getString(2);

  te::da::ForeignKey* fk = new te::da::ForeignKey(fkName, id);
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
    fk->add(getProperty(dtid, static_cast<te::dt::Int16*>(fkCol)->getValue()));
  }

  return std::auto_ptr<te::da::ForeignKey>(fk);
}

std::vector<std::string> te::pgis::DataSource::getIndexNames(const std::string& datasetName) throw(te::da::Exception)
{
  // Check if there is a dataset with this name
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  unsigned int dtid = getTableId(datasetName);

  std::string sql("SELECT idx_table.relname " 
                  "FROM pg_index, pg_class idx_table, pg_am, pg_namespace s "
                  "WHERE s.oid = idx_table.relnamespace "
                  "AND pg_index.indexrelid = idx_table.oid "
                  "AND idx_table.relam = pg_am.oid "
                  "AND pg_index.indrelid = ");
  sql += te::common::Convert2String(dtid);

  std::auto_ptr<te::da::DataSet> indexInfo = query(sql);

  std::vector<std::string> indexNames;

  while(indexInfo->moveNext())
  {
    std::string name = indexInfo->getString(0);
    indexNames.push_back(name);
  }

  return indexNames;
}

std::auto_ptr<te::da::Index> te::pgis::DataSource::getIndex(const std::string& datasetName,
                                                            const std::string& name) throw(te::da::Exception)
{
  // Check if there is a dataset with this name
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  // Check if there is an unique key with this name
  if(!indexExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The index %1% doesn't exist!")) % name).str());

  unsigned int dtid = getTableId(datasetName);

  std::string sql("SELECT idx_table.oid, s.nspname, idx_table.relname, pg_index.indkey, pg_am.amname " 
                  "FROM pg_index, pg_class idx_table, pg_am, pg_namespace s "
                  "WHERE s.oid = idx_table.relnamespace "
                  "AND pg_index.indexrelid = idx_table.oid "
                  "AND idx_table.relam = pg_am.oid ");
  sql += "AND idx_table.relname = ";
  sql += "'" + name + "'";
  sql += " AND pg_index.indrelid = ";
  sql += te::common::Convert2String(dtid);

  std::auto_ptr<te::da::DataSet> idxInfo = query(sql);

  if(!idxInfo->moveNext())
    return std::auto_ptr<te::da::Index>(0);

  unsigned int id = idxInfo->getInt32(0);

  std::auto_ptr<te::dt::Array> idxCols(idxInfo->getArray(3));

  std::string idxType = idxInfo->getString(4);

  te::da::Index* idx = new te::da::Index(name, GetIndexType(idxType.c_str()));
  idx->setId(id);

  std::size_t size = idxCols->getDimensionSize(0);

  std::vector<std::size_t> pos;
  pos.push_back(0);

  for(std::size_t i = 0; i < size; ++i)
  {
    pos[0] = i;
    te::dt::AbstractData* idxCol = idxCols->getData(pos);

    idx->add(getProperty(dtid, static_cast<te::dt::Int16*>(idxCol)->getValue()));
  }

  return std::auto_ptr<te::da::Index>(idx);
}

std::vector<std::string> te::pgis::DataSource::getCheckConstraintNames(const std::string& datasetName) throw(te::da::Exception)
{
  // Check if there is a dataset with this name
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist in the data source!")) % datasetName).str());

  std::vector<std::string> ccNames;

  if(m_pImpl->m_catalog->datasetExists(datasetName))
  {
    // The dataset exists in the catalog
    const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

    std::size_t numCC = dt->getNumberOfCheckConstraints();

    for(std::size_t i = 0; i < numCC; ++i)
      ccNames.push_back(dt->getCheckConstraint(i)->getName());
  }
  else
  {
    // The dataset doesn't exist in the catalog
    unsigned int dtid = getTableId(datasetName);

    std::auto_ptr<te::da::DataSet> ccInfo = getConstraints(dtid, 'c');
    while(ccInfo->moveNext())
    {
      std::string ccName  = ccInfo->getString(2);
      ccNames.push_back(ccName);
    }
  }

  return ccNames;
}

std::auto_ptr<te::da::CheckConstraint> te::pgis::DataSource::getCheckConstraint(const std::string& datasetName,
                                                                                const std::string& name) throw(te::da::Exception)
{
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  if(!checkConstraintExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset %1% has no check constraint with this name (%2%)!")) % datasetName % name).str());

  
  te::da::CheckConstraint* cc;

  unsigned int dtid = getTableId(datasetName);

  std::auto_ptr<te::da::DataSet> ccInfo = getConstraints(dtid, 'c');
  while(ccInfo->moveNext())
  {
    std::string ccName  = ccInfo->getString(2);
    if(ccName == name)
    {
      unsigned int id = ccInfo->getInt32(0);
      cc = new te::da::CheckConstraint(ccName);
      cc->setId(id);
      cc->setExpression(ccInfo->getString(10));
    }
  }

  return std::auto_ptr<te::da::CheckConstraint>(cc);
}

bool te::pgis::DataSource::checkConstraintExists(const std::string& datasetName,
                                                 const std::string& name) throw(te::da::Exception)
{
  std::vector<std::string> ccNames = getCheckConstraintNames(datasetName);

  bool ccExists = false;
  for(std::size_t i = 0; i < ccNames.size(); ++i)
  {
    if(ccNames[i] == name)
    {
      ccExists = true;
      break;
    }
  }

  return ccExists;
}

void te::pgis::DataSource::addCheckConstraint(const std::string& datasetName,
                                              const te::da::CheckConstraint* cc) throw(te::da::Exception)
{
  // This method must be removed from the class
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  if(getCheckConstraint(datasetName, cc->getName()).get() != 0)
    throw Exception((boost::format(TR_PGIS("The dataset %1% already has a check constraint with this name: %2%!")) % datasetName % cc->getName()).str());

  te::da::DataSetType* datasetType = getDataSetType(datasetName).get();

  te::da::CheckConstraint* newCC = new te::da::CheckConstraint(cc->getName(), datasetType);
  newCC->setExpression(cc->getExpression());
}

void te::pgis::DataSource::dropCheckConstraint(const std::string& datasetName,
                                               const std::string& name) throw(te::da::Exception)
{
  if(!datasetExists(datasetName))
    throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  // Check if there is a check constraint with this name
  te::da::CheckConstraint* cc = dt->getCheckConstraint(name);
  if(!cc)
    throw Exception((boost::format(TR_PGIS("The dataset %1% has no check constraint with this name: %2%!")) % datasetName % name).str());

  // Remove the check constraint from the dataset schema
  dt->remove(cc);
}

std::vector<std::string> te::pgis::DataSource::getSequenceNames() throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::da::Sequence> te::pgis::DataSource::getSequence(const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::pgis::DataSource::getExtent(const std::string& /*datasetName*/,
                                                                const std::string& /*propertyName*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::pgis::DataSource::getExtent(const std::string& /*datasetName*/,
                                                                std::size_t /*propertyPos*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::size_t te::pgis::DataSource::getNumberOfItems(const std::string& /*datasetName*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::hasDataSets() throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::datasetExists(const std::string& name) throw(te::da::Exception)
{
  bool datasetExists = false;

  std::vector<std::string> datasetNames = getDataSetNames();
  for(std::size_t i = 0; i < datasetNames.size(); ++i)
  {
    if(datasetNames[i] == name)
    {
      datasetExists = true;
      break;
    }
  }

  return datasetExists;
}

bool te::pgis::DataSource::primarykeyExists(const std::string& /*datasetName*/,
                                               const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::uniquekeyExists(const std::string& /*datasetName*/,
                                              const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::foreignkeyExists(const std::string& /*datasetName*/,
                                               const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::indexExists(const std::string& datasetName,
                                       const std::string& name) throw(te::da::Exception)
{
  std::vector<std::string> idxNames = getIndexNames(datasetName);

  bool idxExists = false;
  for(std::size_t i = 0; i < idxNames.size(); ++i)
  {
    if(idxNames[i] == name)
    {
      idxExists = true;
      break;
    }
  }

  return idxExists;
}

bool te::pgis::DataSource::sequenceExists(const std::string& /*name*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::createDataSet(te::da::DataSetType* dt,
                                            const std::map<std::string, std::string>& /*options*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::cloneDataSet(const std::string& /*name*/,
                                           const std::string& /*cloneName*/,
                                           const std::map<std::string, std::string>& /*options*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropDataSet(const std::string& /*name*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(datasetName))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  //ds->remove(datasetName);

  //if(ds->getCatalog() && ds->getCatalog()->datasetExists(datasetName))
  //{
  //  te::da::DataSetTypePtr dt = ds->getCatalog()->getDataSetType(datasetName);

  //  ds->getCatalog()->remove(dt.get());
  //}
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::renameDataSet(const std::string& /*name*/,
                                            const std::string& /*newName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //std::string oldName = dt->getName();

  //if(dt->getCatalog())
  //  ds->getCatalog()->rename(dt, newName);

  //ds->rename(oldName, newName);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addProperty(const std::string& /*datasetName*/,
                                          const te::dt::Property* /*p*/) throw(te::da::Exception)
{
//  DataSource* ds = m_t->getMemDataSource();
//
//  DataSource::LockWrite l(ds);
//
//  if(dt->getProperty(p->getName()))
//    throw Exception((boost::format(TR_PGIS("A property with the same name (%1%) already exists!")) % p->getName()).str());
//
//  if(!ds->datasetExists(dt->getName()))
//    throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str()); 
//
//  dt->add(p);
//
//// adjust the dataset properties
//  DataSet* dataset = ds->getDataSet(dt->getName());
//
//  dataset->add(p->getName(), p->getType());
//
//// adjust dataset schema
//  te::da::DataSetType* dschema = ds->getDataSetType(dt->getName());
//
//  dschema->add(p->clone());
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropProperty(const std::string& /*datasetName*/,
                                           const std::string& /*propertyName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = dynamic_cast<te::da::DataSetType*>(p->getParent());

  //if(dt == 0)
  //  throw Exception((boost::format(TR_PGIS("Dataset for property %1% is unknown!")) % p->getName()).str());

  //DataSource::LockWrite l(ds);

  //DataSet* dataset = ds->getDataSet(dt->getName());

  //te::da::DataSetType* dschema = ds->getDataSetType(dt->getName());

  //std::size_t pos = te::da::GetPropertyPos(dataset, p->getName());

  //dataset->drop(pos);

  //te::dt::Property* pp = dschema->getProperty(p->getName());

  //dschema->remove(pp);

  //dt->remove(p);

  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::renameProperty(const std::string& /*datasetName*/,
                                             const std::string& /*propertyName*/,
                                             const std::string& /*newPropertyName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = dynamic_cast<te::da::DataSetType*>(p->getParent());

  //if(dt == 0)
  //  throw Exception((boost::format(TR_PGIS("Dataset for property %1% is unknown!")) % p->getName()).str());

  //DataSource::LockWrite l(ds);

  //te::da::DataSetType* dataset = ds->getDataSetType(p->getName());

  //dataset->getProperty(p->getName())->setName(newName);

  //p->setName(newName);

  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addPrimaryKey(const std::string& /*datasetName*/,
                                            const te::da::PrimaryKey* /*pk*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));

  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getPrimaryKey())
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% already has a Primary Key!")) % dt->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::PrimaryKey* newPk = new te::da::PrimaryKey(pk->getName(), dsDt);

  //for(std::size_t i = 0; i < pk->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = pk->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newPk->add(dsP);
  //}

  //dt->setPrimaryKey(pk);
}

void te::pgis::DataSource::dropPrimaryKey(const std::string& /*datasetName*/,
                                             const std::string& /*primaryKeyName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = pk->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getPrimaryKey())
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% has not a Primary Key!")) % dsDt->getName()).str());

  //if(dsDt->getPrimaryKey()->getName() != pk->getName())
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% has not a Primary Key named %2%!")) % dsDt->getName() % pk->getName()).str());
  //  
  //if(dsDt != dt)
  //  dsDt->setPrimaryKey(0);

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->setPrimaryKey(0);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addUniqueKey(const std::string& /*datasetName*/,
                                           const te::da::UniqueKey* /*uk*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getUniqueKey(uk->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% already has a Unique Key with this name (%2%)!")) % dt->getName() % uk->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::UniqueKey* newUk = new te::da::UniqueKey(uk->getName(), dsDt);

  //for(std::size_t i = 0; i < uk->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = uk->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newUk->add(dsP);
  //}

  //dt->add(uk);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropUniqueKey(const std::string& /*datasetName*/,
                                            const std::string& /*uniqueKeyName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = uk->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getUniqueKey(uk->getName()))
  //  throw Exception((boost::format(TR_PGIS("The Unique Key %1% not exists!")) % uk->getName()).str());
  //  
  //if(dsDt != dt)
  //  dsDt->remove(dsDt->getUniqueKey(uk->getName()));

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsDt->getUniqueKey(uk->getName()));
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addIndex(const std::string& /*datasetName*/,
                                       const te::da::Index* /*idx*/,
                                       const std::map<std::string, std::string>& /*options*/) throw(te::da::Exception) 
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //if(ds->getDataSet(dt->getName())->getType()->getIndex(index->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% already has a Index with this name (%2%)!")) % dt->getName() % index->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::Index* newIndex = new te::da::Index(index->getName(), index->getIndexType(), dsDt);

  //for(std::size_t i = 0; i < index->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = index->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newIndex->add(dsP);
  //}

  //dt->add(index);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropIndex(const std::string& /*datasetName*/,
                                        const std::string& /*idxName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = index->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //if(!dsDt->getIndex(index->getName()))
  //  throw Exception((boost::format(TR_PGIS("The Index %1% not exists!")) % index->getName()).str());

  //dsDt->remove(dsDt->getIndex(index->getName()));

  //if(dsDt != dt)
  //  dt->remove(index);

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(index);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::addForeignKey(const std::string& /*datasetName*/,
                                            const te::da::ForeignKey* /*fk*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());

  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::ForeignKey* newFk = new te::da::ForeignKey(fk->getName()); 

  //for(std::size_t i = 0; i < fk->getProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = fk->getProperties()[i];

  //   te::dt::Property* dsP = dsDt->getProperty(p->getName());

  //   newFk->add(dsP);
  //}

  //for(std::size_t i = 0; i < fk->getReferencedProperties().size(); ++i)
  //{
  //   const te::dt::Property* p = fk->getProperties()[i];

  //   te::da::DataSetType* currDt = static_cast<te::da::DataSetType*>(p->getParent());

  //   if(!ds->datasetExists(currDt->getName()))
  //     throw Exception((boost::format(TR_PGIS("The dataset %1% referenced not exists!")) % currDt->getName()).str());

  //   te::da::DataSetType* currRefDt = ds->getDataSet(currDt->getName())->getType();

  //   te::dt::Property* refP = currRefDt->getProperty(fk->getReferencedProperties()[i]->getName());

  //   newFk->addRefProperty(refP);
  //}

  //dsDt->add(newFk);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropForeignKey(const std::string& /*datasetName*/,
                                             const std::string& /*fkName*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //te::da::DataSetType* dt = fk->getDataSetType();

  //DataSource::LockWrite l(ds);

  //if(!ds->datasetExists(dt->getName()))
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% not exists!")) % dt->getName()).str());
  //
  //te::da::DataSetType* dsDt = ds->getDataSet(dt->getName())->getType();

  //te::da::ForeignKey* dsFk = dsDt->getForeignKey(fk->getName());
  //
  //if(!dsFk)
  //  throw Exception((boost::format(TR_PGIS("The Foreign Key %1% not exists!")) % fk->getName()).str());
  //  
  //if(dsDt != dt)
  //  dsDt->remove(dsFk);

  //if(ds->getCatalog()->getDataSetType(dsDt->getName()))
  //  ds->getCatalog()->getDataSetType(dsDt->getName())->remove(dsFk);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::createSequence(const te::da::Sequence* /*sequence*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(ds->getCatalog()->getSequence(sequence->getName()))
  //  throw Exception((boost::format(TR_PGIS("The datasource already has a Sequence with this name (%1%)!")) % sequence->getName()).str());
  //   
  //te::da::Sequence* newSeq = new te::da::Sequence(*sequence);
  //newSeq->setCatalog(ds->getCatalog());

  ////getting the datasettype by the name of the sequence datasettype property involved
  //const te::da::DataSetTypePtr& dsDt = ds->getCatalog()->getDataSetType(static_cast<te::da::DataSetType*>(sequence->getOwner()->getParent())->getName());

  //newSeq->setOwner(dsDt->getProperty(sequence->getOwner()->getName()));
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropSequence(const std::string& /*name*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(ds->getCatalog()->getSequence(sequence->getName()))
  //  throw Exception((boost::format(TR_PGIS("The datasource has no Sequence with this name (%1%)!")) % sequence->getName()).str());

  //ds->getCatalog()->remove(ds->getCatalog()->getSequence(sequence->getName()));
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::add(const std::string& /*datasetName*/,
                                  te::da::DataSet* /*d*/,
                                  const std::map<std::string, std::string>& /*options*/,
                                  std::size_t /*limit*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //te::pgis::DataSet* idataset = ds->getDataSetRef(datasetName);

  //if(idataset == 0)
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  //idataset->copy(*d, limit);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::remove(const std::string& /*datasetName*/,
                                     const te::da::ObjectIdSet* /*oids*/) throw(te::da::Exception)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //DataSet* dataset = ds->getDataSetRef(datasetName);

  //if(dataset == 0)
  //  throw Exception((boost::format(TR_PGIS("The dataset %1% doesn't exist!")) % datasetName).str());

  //dataset->clear();
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::update(const std::string& /*datasetName*/,
                                     te::da::DataSet* /*dataset*/,
                                     const std::vector<std::size_t>& /*properties*/,
                                     const te::da::ObjectIdSet* /*oids*/,
                                     const std::map<std::string, std::string>& /*options*/,
                                     std::size_t /*limit*/) throw(te::da::Exception)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

unsigned int te::pgis::DataSource::getTableId(const std::string& tableName) throw(te::da::Exception)
{
 std::string tname, sname;
  
  SplitTableName(tableName, m_pImpl->m_currentSchema, sname, tname);

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

std::string te::pgis::DataSource::getTableName(unsigned int id)
{
  std::string sql("SELECT pg_namespace.nspname, pg_class.relname "
                  "FROM pg_class, pg_namespace "
                  "WHERE pg_class.relnamespace = pg_namespace.oid " 
                  "AND pg_class.oid = ");

  sql += te::common::Convert2String(id);

  std::auto_ptr<te::da::DataSet> result(query(sql));

  if(result->moveNext() == false)
    throw Exception(TR_PGIS("Could not find the table name!"));

  std::string tname  = result->getString(0);
              tname += ".";
              tname += result->getString(1);

  return tname;
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getConstraints(unsigned int dtid, char conType)
{
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

te::da::DataSet* te::pgis::DataSource::getProperties(unsigned int dtid)
{
  std::string sql("SELECT a.attnum, a.attname, t.oid, a.attnotnull, format_type(a.atttypid, a.atttypmod), a.atthasdef, pg_get_expr(d.adbin, d.adrelid), a.attndims "
                  "FROM pg_attribute AS a INNER JOIN pg_type AS t ON (a.atttypid = t.oid) LEFT JOIN pg_attrdef AS d ON (a.attrelid = d.adrelid AND a.attnum = d.adnum) "
                  "WHERE a.attrelid = ");
              sql += te::common::Convert2String(dtid);
              sql += " AND a.attisdropped = false"
                     " AND a.attnum > 0"
                     " ORDER BY a.attnum";

  return query(sql).release();
}

void te::pgis::DataSource::getGeometryInfo(const std::string& datasetName, te::gm::GeometryProperty* gp)
{
  std::string sql = "SELECT g.coord_dimension, g.srid, g.type "
                    "FROM geometry_columns g "
                    "WHERE g.f_table_name = '";

  std::string tname, sname;
  
  SplitTableName(datasetName, getCurrentSchema(), sname, tname);

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

void te::pgis::DataSource::getRasterInfo(const std::string& datasetName, te::rst::RasterProperty* rp)
{
  std::string sql = "SELECT * FROM raster_columns as r WHERE r.r_table_name = '";

  std::string tname, sname;

  SplitTableName(datasetName, getCurrentSchema(), sname, tname);

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

te::dt::Property* te::pgis::DataSource::getProperty(unsigned int dtid, unsigned int pid)
{
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

    p.reset(Convert2TerraLib(attNum, attName.c_str(), attType, attNotNull, fmt.c_str(), attHasDefault, attDefValue.c_str(), ndims,getGeomTypeId(), getRasterTypeId()));

    if(p->getType() == te::dt::GEOMETRY_TYPE)
    {
      std::string datasetName = getTableName(dtid);

      getGeometryInfo(datasetName, static_cast<te::gm::GeometryProperty*>(p.get()));
    }
  }

  return p.release();
}

void te::pgis::DataSource::setDialect(te::da::SQLDialect* myDialect)
{
  DataSource::Impl::sm_dialect = myDialect;
}

void te::pgis::DataSource::setCapabilities(const te::da::DataSourceCapabilities& capabilities)
{
  DataSource::Impl::sm_capabilities = capabilities;
}

void te::pgis::DataSource::create(const std::map<std::string, std::string>& dsInfo) throw(te::da::Exception)
{
  m_pImpl->m_connInfo = dsInfo;

  open();

  close();
}

void te::pgis::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/) throw(te::da::Exception)
{
}

bool te::pgis::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/) throw(te::da::Exception)
{
  return false;
}

std::vector<std::string> te::pgis::DataSource::getDataSourceNames(const std::map<std::string, std::string>& info) throw(te::da::Exception)
{
  // Get an auxiliary connection
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(m_pImpl->m_connInfo);

  ds->open();

  std::string sql("SELECT datname FROM pg_database");

  std::auto_ptr<te::da::DataSet> dataset(query(sql));

  std::vector<std::string> dataSourceNames;

  while(dataset->moveNext())
    dataSourceNames.push_back(dataset->getString(0));

  ds->close();

  return dataSourceNames;
}
