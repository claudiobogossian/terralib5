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
    std::vector<std::string> m_datasetNames;          //!< The list of dataset names of the data source.

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

std::string te::pgis::DataSource::getType() const
{
  return PGIS_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::pgis::DataSource::getConnectionInfo() const
{
  return m_pImpl->m_connInfo;
}

void te::pgis::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_pImpl->m_connInfo = connInfo;
}

bool te::pgis::DataSource::isTimeAnInteger()
{
  return m_pImpl->m_timeIsInteger;
}

void te::pgis::DataSource::setTimeAsInteger(bool timeIsInteger)
{
  m_pImpl->m_timeIsInteger = timeIsInteger;
}

void te::pgis::DataSource::open()
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

  // Get the dataset names of the data source
  getDataSetNames();
}

void te::pgis::DataSource::close()
{
  if(m_pImpl->m_conn)
    m_pImpl->m_pool->release(m_pImpl->m_conn);

  m_pImpl->m_pool->finalize();

  delete m_pImpl->m_currentSchema;
  m_pImpl->m_currentSchema = 0;
}

bool te::pgis::DataSource::isOpened() const
{
  return m_pImpl->m_pool->isInitialized();
}

bool te::pgis::DataSource::isValid() const
{
  return m_pImpl->m_pool->isValid();
}

const te::da::DataSourceCapabilities& te::pgis::DataSource::getCapabilities() const
{
  return Impl::sm_capabilities;
}

const te::da::SQLDialect* te::pgis::DataSource::getDialect() const
{
  return Impl::sm_dialect;
}

void te::pgis::DataSource::begin()
{
  execute("BEGIN");
  m_pImpl->m_isInTransaction = true;
}

void te::pgis::DataSource::commit()
{
  m_pImpl->m_isInTransaction = false;
  execute("COMMIT");
}

void te::pgis::DataSource::rollBack()
{
  m_pImpl->m_isInTransaction = false;
  execute("ROLLBACK");
}

bool te::pgis::DataSource::isInTransaction() const
{
  return m_pImpl->m_isInTransaction;
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getDataSet(const std::string& name,
                                                                te::common::TraverseType /*travType*/)
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
                                                                te::common::TraverseType travType)
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
                                                                te::common::TraverseType travType)
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
                                                           te::common::TraverseType travType)
{
  std::string sql;

  SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  q.accept(visitor);
 
  return query(sql, travType);
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::query(const std::string& query, 
                                                           te::common::TraverseType travType)
{
  PGresult* result = m_pImpl->m_conn->query(query);

  return std::auto_ptr<te::da::DataSet>(new DataSet(result, m_pImpl->m_conn, new std::string(query)));
}

void te::pgis::DataSource::execute(const te::da::Query& command)
{
  std::string sql;

  SQLVisitor visitor(*(getDialect()), sql, m_pImpl->m_conn->getConn());
  command.accept(visitor);
 
  execute(sql);
}

void te::pgis::DataSource::execute(const std::string& command)
{
  m_pImpl->m_conn->execute(command);
}

void te::pgis::DataSource::cancel()
{
}

boost::int64_t te::pgis::DataSource::getLastGeneratedId()
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

std::string te::pgis::DataSource::escape(const std::string& value)
{
  return value;
}

bool te::pgis::DataSource::isDataSetNameValid(const std::string& /*datasetName*/)
{
  return true;
}

bool te::pgis::DataSource::isPropertyNameValid(const std::string& /*propertyName*/)
{
  return true;
}

std::vector<std::string> te::pgis::DataSource::getDataSetNames()
{
  if(!m_pImpl->m_datasetNames.empty())
    return m_pImpl->m_datasetNames;

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
    m_pImpl->m_datasetNames.push_back(datasetName);
  }

  return m_pImpl->m_datasetNames;
}

const te::da::DataSetTypePtr& te::pgis::DataSource::getDataSetType(const std::string& name)
{
  if(!datasetExists(name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % name).str());

  // If the DataSetType exists in the catalog, get it from there.
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

  // Get all the constraints of the dataset and load them to its schema
  if(!getConstraints(dt))
    throw Exception((boost::format(TR_PGIS("The constraints of the dataset \"%1%\" could not be loaded!")) % name).str());

  // Get the indexes of the dataset and add them to its schema
  getIndexes(dt);

  // Add the dataset schema to the catalog
  m_pImpl->m_catalog->add(dt);

  return m_pImpl->m_catalog->getDataSetType(name);
}

std::size_t te::pgis::DataSource::getNumberOfProperties(const std::string& datasetName)
{
  return getProperties(datasetName).size();
}

boost::ptr_vector<te::dt::Property> te::pgis::DataSource::getProperties(const std::string& datasetName)
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
                                                                  const std::string& propertyName)
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
                                                                  std::size_t propertyPos)
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

te::da::PrimaryKey* te::pgis::DataSource::getPrimaryKey(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return dt->getPrimaryKey();
}

bool te::pgis::DataSource::primaryKeyExists(const std::string& datasetName, const std::string& name)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  if(dt->getPrimaryKey()->getName() == name)
    return true;

  return false;
}

void te::pgis::DataSource::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  dt->add(pk);
}

void te::pgis::DataSource::dropPrimaryKey(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  te::da::PrimaryKey* pk = dt->getPrimaryKey();

  dt->remove(pk);
}

std::vector<std::string> te::pgis::DataSource::getForeignKeyNames(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> fkNames;

  std::size_t numFK = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < numFK; ++i)
    fkNames.push_back(dt->getForeignKey(i)->getName());

  return fkNames;
}

bool te::pgis::DataSource::foreignKeyExists(const std::string& datasetName, const std::string& name)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> fkNames = getForeignKeyNames(datasetName);

  if(std::find(fkNames.begin(), fkNames.end(), name) != fkNames.end())
    return true;

  return false;
}

te::da::ForeignKey* te::pgis::DataSource::getForeignKey(const std::string& datasetName, const std::string& name)
{
  if(!foreignKeyExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no foreign key with this name \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return dt->getForeignKey(name);
}

void te::pgis::DataSource::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
  std::string fkName = fk->getName();
  if(foreignKeyExists(datasetName, fkName))
    throw Exception((boost::format(TR_PGIS("The dataset already \"%1%\" has a foreign key with this name \"%2%\"!")) % datasetName % fkName).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  dt->add(fk);
}

void te::pgis::DataSource::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  if(!foreignKeyExists(datasetName, fkName))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no foreign key with this name \"%2%\"!")) % datasetName % fkName).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  te::da::ForeignKey* fk = dt->getForeignKey(fkName);
  dt->remove(fk);
}

std::vector<std::string> te::pgis::DataSource::getUniqueKeyNames(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> ukNames;

  std::size_t numUKs = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < numUKs; ++i)
    ukNames.push_back(dt->getUniqueKey(i)->getName());

  return ukNames;
}

bool te::pgis::DataSource::uniqueKeyExists(const std::string& datasetName, const std::string& name)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> ukNames = getUniqueKeyNames(datasetName);

  if(std::find(ukNames.begin(), ukNames.end(), name) != ukNames.end())
    return true;

  return false;
}

te::da::UniqueKey* te::pgis::DataSource::getUniqueKey(const std::string& datasetName, const std::string& name)
{
  if(!uniqueKeyExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no unique key with this name \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return dt->getUniqueKey(name);
}

void te::pgis::DataSource::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  std::string ukName = uk->getName();
  if(uniqueKeyExists(datasetName, ukName))
    throw Exception((boost::format(TR_PGIS("The dataset already \"%1%\" has a unique key with this name \"%2%\"!")) % datasetName % ukName).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  dt->add(uk);
}

void te::pgis::DataSource::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  if(!uniqueKeyExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no unique key with this name \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  te::da::UniqueKey* uk = dt->getUniqueKey(name);
  dt->remove(uk);
}

std::vector<std::string> te::pgis::DataSource::getCheckConstraintNames(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> ccNames;

  std::size_t numCCs = dt->getNumberOfCheckConstraints();

  for(std::size_t i = 0; i < numCCs; ++i)
    ccNames.push_back(dt->getCheckConstraint(i)->getName());

  return ccNames;
}

bool te::pgis::DataSource::checkConstraintExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> ccNames = getCheckConstraintNames(datasetName);

  if(std::find(ccNames.begin(), ccNames.end(), name) != ccNames.end())
    return true;

  return false;
}

te::da::CheckConstraint* te::pgis::DataSource::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  if(!checkConstraintExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no check constraint with this name \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return dt->getCheckConstraint(name);
}

void te::pgis::DataSource::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
{
  std::string name = cc->getName();
  if(checkConstraintExists(datasetName, name) != 0)
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" already has a check constraint with this name: \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  dt->add(cc);
}

void te::pgis::DataSource::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
  if(!checkConstraintExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no check constraint with this name: \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  te::da::CheckConstraint* cc = dt->getCheckConstraint(name);

  dt->remove(cc);
}

std::vector<std::string> te::pgis::DataSource::getIndexNames(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  std::vector<std::string> idxNames;

  std::size_t numIdxs = dt->getNumberOfIndexes();

  for(std::size_t i = 0; i < numIdxs; ++i)
    idxNames.push_back(dt->getIndex(i)->getName());

  return idxNames;
}

bool te::pgis::DataSource::indexExists(const std::string& datasetName, const std::string& name)
{
  std::vector<std::string> idxNames = getIndexNames(datasetName);

  if(std::find(idxNames.begin(), idxNames.end(), name) != idxNames.end())
    return true;

  return false;
}

te::da::Index* te::pgis::DataSource::getIndex(const std::string& datasetName, const std::string& name)
{
  if(!indexExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no index with this name \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  return dt->getIndex(name);
}

void te::pgis::DataSource::addIndex(const std::string& datasetName, te::da::Index* idx,
                                    const std::map<std::string, std::string>& options) 
{
  std::string name = idx->getName();
  if(indexExists(datasetName, name) != 0)
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" already has a index with this name: \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  dt->add(idx);
}

void te::pgis::DataSource::dropIndex(const std::string& datasetName, const std::string& name)
{
  if(!indexExists(datasetName, name))
    throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" has no index with this name: \"%2%\"!")) % datasetName % name).str());

  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);

  te::da::Index* idx = dt->getIndex(name);

  dt->remove(idx);
}

std::vector<std::string> te::pgis::DataSource::getSequenceNames()
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::da::Sequence> te::pgis::DataSource::getSequence(const std::string& /*name*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::pgis::DataSource::getExtent(const std::string& /*datasetName*/,
                                                                const std::string& /*propertyName*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::auto_ptr<te::gm::Envelope> te::pgis::DataSource::getExtent(const std::string& /*datasetName*/,
                                                                std::size_t /*propertyPos*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

std::size_t te::pgis::DataSource::getNumberOfItems(const std::string& /*datasetName*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::hasDataSets()
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

bool te::pgis::DataSource::datasetExists(const std::string& name)
{
  bool datasetExists = false;

  if(std::find(m_pImpl->m_datasetNames.begin(), m_pImpl->m_datasetNames.end(), name) != m_pImpl->m_datasetNames.end())
    datasetExists = true;

  return datasetExists;
}

bool te::pgis::DataSource::sequenceExists(const std::string& /*name*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::createDataSet(te::da::DataSetType* dt,
                                            const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::cloneDataSet(const std::string& /*name*/,
                                           const std::string& /*cloneName*/,
                                           const std::map<std::string, std::string>& /*options*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropDataSet(const std::string& /*name*/)
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
                                            const std::string& /*newName*/)
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
                                          const te::dt::Property* /*p*/)
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
                                           const std::string& /*propertyName*/)
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
                                             const std::string& /*newPropertyName*/)
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

void te::pgis::DataSource::createSequence(const te::da::Sequence* /*sequence*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(ds->getCatalog()->getSequence(sequence->getName()))
  //  throw Exception((boost::format(TR_PGIS("The datasource already has a Sequence with this name (\"%1%\")!")) % sequence->getName()).str());
  //   
  //te::da::Sequence* newSeq = new te::da::Sequence(*sequence);
  //newSeq->setCatalog(ds->getCatalog());

  ////getting the datasettype by the name of the sequence datasettype property involved
  //const te::da::DataSetTypePtr& dsDt = ds->getCatalog()->getDataSetType(static_cast<te::da::DataSetType*>(sequence->getOwner()->getParent())->getName());

  //newSeq->setOwner(dsDt->getProperty(sequence->getOwner()->getName()));
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::dropSequence(const std::string& /*name*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //if(ds->getCatalog()->getSequence(sequence->getName()))
  //  throw Exception((boost::format(TR_PGIS("The datasource has no Sequence with this name (\"%1%\")!")) % sequence->getName()).str());

  //ds->getCatalog()->remove(ds->getCatalog()->getSequence(sequence->getName()));
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::add(const std::string& /*datasetName*/,
                                  te::da::DataSet* /*d*/,
                                  const std::map<std::string, std::string>& /*options*/,
                                  std::size_t /*limit*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //te::pgis::DataSet* idataset = ds->getDataSetRef(datasetName);

  //if(idataset == 0)
  //  throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % datasetName).str());

  //idataset->copy(*d, limit);
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::remove(const std::string& /*datasetName*/,
                                     const te::da::ObjectIdSet* /*oids*/)
{
  //DataSource* ds = m_t->getMemDataSource();

  //DataSource::LockWrite l(ds);

  //DataSet* dataset = ds->getDataSetRef(datasetName);

  //if(dataset == 0)
  //  throw Exception((boost::format(TR_PGIS("The dataset \"%1%\" doesn't exist!")) % datasetName).str());

  //dataset->clear();
  throw Exception(TR_PGIS("Not implemented yet!"));
}

void te::pgis::DataSource::update(const std::string& /*datasetName*/,
                                     te::da::DataSet* /*dataset*/,
                                     const std::vector<std::size_t>& /*properties*/,
                                     const te::da::ObjectIdSet* /*oids*/,
                                     const std::map<std::string, std::string>& /*options*/,
                                     std::size_t /*limit*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
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

std::vector<std::string> te::pgis::DataSource::getDataSourceNames(const std::map<std::string, std::string>& info)
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

std::vector<std::string> te::pgis::DataSource::getEncodings(const std::map<std::string, std::string>& info)
{
  std::vector<std::string> encodings;

  // Let's have an auxiliary connection
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(info);

  ds->open();

  // Try to check
  std::string sql("SELECT DISTINCT pg_catalog.pg_encoding_to_char(conforencoding) FROM pg_catalog.pg_conversion ORDER BY pg_catalog.pg_encoding_to_char(conforencoding)");

  std::auto_ptr<te::da::DataSet> encs(query(sql));

  while(encs->moveNext())
    encodings.push_back(encs->getString(0));

  ds->close();

  return encodings;
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

unsigned int te::pgis::DataSource::getTableId(const std::string& tableName)
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

bool te::pgis::DataSource::getConstraints(te::da::DataSetTypePtr& dt)
{
  std::string datasetName = dt->getName();
  unsigned int dtid = dt->getId();

  te::da::PrimaryKey* pk = 0;                 // the dataset primary key
  te::da::ForeignKey* fk = 0;                 // the dataset foreign key
  std::vector<te::da::UniqueKey*> uks;        // the dataset unique keys
  std::vector<te::da::CheckConstraint*> ccs;  // the dataset check constraints

  std::string sql("SELECT c.oid, n.nspname, c.conname, c.contype, c.confrelid, c.confupdtype, c.confdeltype, c.confmatchtype, c.conkey, c.confkey, pg_get_constraintdef(c.oid) "
                  "FROM pg_constraint c, pg_namespace n "
                  "WHERE c.connamespace = n.oid "
                  "AND c.conrelid = ");
  sql += te::common::Convert2String(dtid);

  std::auto_ptr<te::da::DataSet> cInfo = query(sql);

  while(cInfo->moveNext())
  {
    char cType = cInfo->getChar(3);

    // begin of the dataset primary key
    if(cType == 'p')
    {
      unsigned int pkId = cInfo->getInt32(0);
      std::string pkName  = cInfo->getString(2);

      pk = new te::da::PrimaryKey(pkName, 0, pkId);

      std::auto_ptr<te::dt::Array> pkCols(cInfo->getArray(8));
      std::size_t size = pkCols->getDimensionSize(0);

      std::vector<std::size_t> pos;
      pos.push_back(0);
      for(std::size_t i = 0; i < size; ++i)
      {
        pos[0] = i;

        te::dt::AbstractData* pkCol = pkCols->getData(pos);

        te::dt::Property* p = dt->getPropertyById(static_cast<te::dt::Int16*>(pkCol)->getValue());
        if(p == 0) // property not found in the dataset type list
          return false;

        pk->add(p);
      }

      // Try to link the pk to an index
      std::vector<std::string> indexNames = getIndexNames(datasetName);

      for(std::size_t i = 0; i < indexNames.size(); ++i)
      {
        if(pk->getName() == indexNames[i])
        {
          pk->setAssociatedIndex(getIndex(datasetName, indexNames[i]));
          break;
        }
      }
    }  // end of the dataset primary key
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

      std::string refName = getTableName(refDatasetId);

      const te::da::DataSetTypePtr& refDatasetType = m_pImpl->m_catalog->getDataSetType(refName);

      if(refDatasetType.get() == 0)
        return false;

      std::string fkName  = cInfo->getString(1);
      fkName += ".";
      fkName += cInfo->getString(2);

      fk = new te::da::ForeignKey(fkName, fkId);
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

        te::dt::Property* p = getProperty(dtid, static_cast<te::dt::Int16*>(ukCol)->getValue());

        if(p == 0) // property not found
          return false;

        uk->add(p);
      }

      // Try to link the uk to an index
      std::vector<std::string> indexNames = getIndexNames(datasetName);

      for(std::size_t i = 0; i < indexNames.size(); ++i)
      {
        if(uk->getName() == indexNames[i])
        {
          uk->setAssociatedIndex(getIndex(datasetName, indexNames[i]));
          break;
        }
      }

      uks.push_back(uk);

    }  // end of unique key constraint
    else if(cType == 'c')
    {
      // begin of check constraint
      std::string ccName  = cInfo->getString(2);
      unsigned int ccId = cInfo->getInt32(0);

      te::da::CheckConstraint* cc = new te::da::CheckConstraint(ccName);
      cc->setId(ccId);
      cc->setExpression(cInfo->getString(10));

      ccs.push_back(cc);
    }  // end of check constraint
  }    // end of moveNext

  // Load the constraints of the dataset to its schema
  dt->add(pk);
  dt->add(fk);
  dt->add(uks);
  dt->add(ccs);

  return true;
}

void te::pgis::DataSource::getIndexes(te::da::DataSetTypePtr& dt)
{
  std::string datasetName = dt->getName();
  unsigned int dtid = dt->getId();

  std::vector<te::da::Index*> idxs;

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
    std::string idxName = idxInfo->getString(1) + ".";
    idxName += idxInfo->getString(2);

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

    idxs.push_back(idx);
  }

  dt->add(idxs);
}

std::auto_ptr<te::da::DataSet> te::pgis::DataSource::getConstraints(unsigned int dtid)
{
  std::string sql("SELECT c.oid, n.nspname, c.conname, c.contype, c.confrelid, c.confupdtype, c.confdeltype, c.confmatchtype, c.conkey, c.confkey, pg_get_constraintdef(c.oid) "
                  "FROM pg_constraint c, pg_namespace n "
                  "WHERE c.connamespace = n.oid "
                  "AND c.conrelid = ");
              sql += te::common::Convert2String(dtid);

  return query(sql);
}

void te::pgis::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  m_pImpl->m_connInfo = dsInfo;

  open();

  close();
}

void te::pgis::DataSource::drop(const std::map<std::string, std::string>& /*dsInfo*/)
{
}

bool te::pgis::DataSource::exists(const std::map<std::string, std::string>& /*dsInfo*/)
{
  return false;
}
