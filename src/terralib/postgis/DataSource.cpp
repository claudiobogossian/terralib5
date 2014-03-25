/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published byF
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
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/utils/Utils.h"
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
#include "DataSet.h"
#include "Exception.h"
#include "Globals.h"
#include "PreparedQuery.h"
#include "SQLVisitor.h"
#include "Transactor.h"
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

// libpq
#include <libpq-fe.h>

te::pgis::DataSource::DataSource()
  : m_pool(0),
    m_geomTypeOid(0),
    m_rasterTypeOid(0),
    m_timeIsInteger(true)
{
  m_pool = new ConnectionPool(this);
}

te::pgis::DataSource::~DataSource()
{
  delete m_pool;
}

std::string te::pgis::DataSource::getType() const
{
  return PGIS_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::pgis::DataSource::getConnectionInfo() const
{
  return m_connInfo;
}

void te::pgis::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connInfo = connInfo;
}

std::auto_ptr<te::da::DataSourceTransactor> te::pgis::DataSource::getTransactor()
{
  Connection* conn = m_pool->getConnection();

  return std::auto_ptr<te::da::DataSourceTransactor>(new te::pgis::Transactor(this, conn));
}

te::pgis::Connection* te::pgis::DataSource::getConnection()
{
  return m_pool->getConnection();
}

 void te::pgis::DataSource::closeConnection(Connection* conn)
{
  m_pool->release(conn);
}

bool te::pgis::DataSource::isTimeAnInteger()
{
  return m_timeIsInteger;
}

void te::pgis::DataSource::setTimeAsInteger(bool timeIsInteger)
{
  m_timeIsInteger = timeIsInteger;
}

void te::pgis::DataSource::open()
{
  // Assure we are in a closed state
  close();

  m_pool->initialize();

  std::auto_ptr<te::da::DataSourceTransactor> t = getTransactor();
  te::pgis::Transactor* pgt = static_cast<te::pgis::Transactor*>(t.get());

  // Find the PostGIS types
  m_geomTypeOid = pgt->getGeomTypeId();
  m_rasterTypeOid = pgt->getRasterTypeId();

  // Find the PostgreSQL current schema of the connection
  pgt->getDatabaseInfo(m_currentSchema);
}

void te::pgis::DataSource::close()
{
  m_pool->finalize();
}

bool te::pgis::DataSource::isOpened() const
{
  return m_pool->isInitialized();
}

bool te::pgis::DataSource::isValid() const
{
  return m_pool->isValid();
}

const te::da::DataSourceCapabilities& te::pgis::DataSource::getCapabilities() const
{
  return *te::pgis::Globals::sm_capabilities;
}

const te::da::SQLDialect* te::pgis::DataSource::getDialect() const
{
  return te::pgis::Globals::sm_queryDialect;
}

unsigned int te::pgis::DataSource::getGeomTypeId() const
{
  return m_geomTypeOid;
}

unsigned int te::pgis::DataSource::getRasterTypeId() const
{
  return m_rasterTypeOid;
}

const std::string& te::pgis::DataSource::getCurrentSchema() const
{
  return m_currentSchema;
}

te::pgis::ConnectionPool* te::pgis::DataSource::getConnPool() const
{
  return m_pool;
}

void te::pgis::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  // Get an auxiliary data source
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

  // Create a database based on the connection information
  std::string sql = "CREATE DATABASE ";

  std::map<std::string, std::string>::const_iterator it = dsInfo.find("PG_NEWDB_NAME");
  std::map<std::string, std::string>::const_iterator it_end = dsInfo.end();

  if(it != it_end)
    sql += it->second;
  else
    throw Exception(TE_TR("The database could not be created due the missing parameter: PG_NEWDB_NAME!"));

  it = dsInfo.find("PG_NEWDB_TEMPLATE");

  if(it != it_end)
    sql += " TEMPLATE = " + it->second;

  it = dsInfo.find("PG_NEWDB_OWNER");

  if(it != it_end)
    sql += " OWNER = " + it->second;

  it = dsInfo.find("PG_NEWDB_ENCODING");

  if(it != it_end)
    sql += " ENCODING = " + it->second;

  it = dsInfo.find("PG_NEWDB_TABLESPACE");

  if(it != it_end)
    sql += " TABLESPACE = " + it->second;

  it = dsInfo.find("PG_NEWDB_CONN_LIMIT");

  if(it != it_end)
    sql += " CONNECTION LIMIT = " + it->second;

  ds->execute(sql);

  ds->close();

  // Copy the database connection parameters to this new data source object.
  it = dsInfo.find("PG_NEWDB_HOST");

  if(it == it_end)
    it = dsInfo.find("PG_HOST");

  if(it != it_end)
    m_connInfo["PG_HOST"] = it->second;

  it = dsInfo.find("PG_NEWDB_HOSTADDR");

  if(it == it_end)
    it = dsInfo.find("PG_HOST_ADDR");

  if(it != it_end)
    m_connInfo["PG_HOST_ADDR"] = it->second;

  it = dsInfo.find("PG_NEWDB_PORT");

  if(it == it_end)
    it = dsInfo.find("PG_PORT");

  if(it != it_end)
    m_connInfo["PG_PORT"] = it->second;

  it = dsInfo.find("PG_NEWDB_NAME");

  if(it != it_end)
    m_connInfo["PG_DB_NAME"] = it->second;

  it = dsInfo.find("PG_NEWDB_USER");

  if(it == it_end)
    it = dsInfo.find("PG_USER");

  if(it != it_end)
    m_connInfo["PG_USER"] = it->second;

  it = dsInfo.find("PG_NEWDB_PASSWORD");

  if(it == it_end)
    it = dsInfo.find("PG_PASSWORD");

  if(it != it_end)
    m_connInfo["PG_PASSWORD"] = it->second;

  it = dsInfo.find("PG_NEWDB_CONNECT_TIMEOUT");

  if(it == it_end)
    it = dsInfo.find("PG_CONNECT_TIMEOUT");

  if(it != it_end)
    m_connInfo["PG_CONNECT_TIMEOUT"] = it->second;

  it = dsInfo.find("PG_NEWDB_OPTIONS");

  if(it == it_end)
    it = dsInfo.find("PG_OPTIONS");

  if(it != it_end)
    m_connInfo["PG_OPTIONS"] = it->second;

  it = dsInfo.find("PG_NEWDB_SSL_MODE");

  if(it == it_end)
    it = dsInfo.find("PG_SSL_MODE");

  if(it != it_end)
    m_connInfo["PG_SSL_MODE"] = it->second;

  it = dsInfo.find("PG_NEWDB_KRBSRVNAME");

  if(it == it_end)
    it = dsInfo.find("PG_KRBSRVNAME");

  if(it != it_end)
    m_connInfo["PG_KRBSRVNAME"] = it->second;

  it = dsInfo.find("PG_NEWDB_GSSLIB");

  if(it == it_end)
    it = dsInfo.find("PG_GSSLIB");

  if(it != it_end)
    m_connInfo["PG_GSSLIB"] = it->second;

  it = dsInfo.find("PG_NEWDB_INITIAL_POOL_SIZE");

  if(it == it_end)
    it = dsInfo.find("PG_INITIAL_POOL_SIZE");

  if(it != it_end)
    m_connInfo["PG_INITIAL_POOL_SIZE"] = it->second;

  it = dsInfo.find("PG_NEWDB_MIN_POOL_SIZE");

  if(it == it_end)
    it = dsInfo.find("PG_MIN_POOL_SIZE");

  if(it != it_end)
    m_connInfo["PG_MIN_POOL_SIZE"] = it->second;

  it = dsInfo.find("PG_NEW_DB_MAX_POOL_SIZE");

  if(it == it_end)
    it = dsInfo.find("PG_MAX_POOL_SIZE");

  if(it != it_end)
    m_connInfo["PG_MAX_POOL_SIZE"] = it->second;

  it = dsInfo.find("PG_NEWDB_MAX_IDLE_TIME");

  if(it == it_end)
    it = dsInfo.find("PG_MAX_IDLE_TIME");

  if(it != it_end)
    m_connInfo["PG_MAX_IDLE_TIME"] = it->second;

  it = dsInfo.find("PG_NEWDB_CLIENT_ENCODING");

  if(it == it_end)
    it = dsInfo.find("PG_CLIENT_ENCODING");

  if(it != it_end)
    m_connInfo["PG_CLIENT_ENCODING"] = it->second;
}

void te::pgis::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  // Get an auxiliary data source
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

  // Drop the database
  std::string sql = "DROP DATABASE ";

  std::map<std::string, std::string>::const_iterator it = dsInfo.find("PG_DB_TO_DROP");

  if(it == dsInfo.end())
    throw Exception(TE_TR("Could not drop the database due the missing parameter: PG_DB_TO_DROP!"));

  if((it->second == "postgres") || (it->second == "template_postgis"))
    throw Exception(TE_TR("The database postgres or template_postgis is not allowed to be dropped!"));

  sql += it->second;

  ds->execute(sql);

  ds->close();
}

bool te::pgis::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  if(dsInfo.count("PG_CHECK_DB_EXISTENCE") == 0)
    throw Exception(TE_TR("Could not check the PostgreSQL database existence due the missing parameter: PG_CHECK_DB_EXISTENCE!"));

  const std::string& dbName = dsInfo.find("PG_CHECK_DB_EXISTENCE")->second;

  // Get an auxiliary data source
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

  std::string sql("SELECT * FROM pg_database WHERE datname = '");
  sql += dbName;
  sql += "'";

  std::auto_ptr<te::da::DataSet> database(ds->query(sql));

  ds->close();

  return database->moveNext();
}

std::vector<std::string> te::pgis::DataSource::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  // Get an auxiliary data source
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

  std::string sql("SELECT datname FROM pg_database");

  std::auto_ptr<te::da::DataSet> dataset(ds->query(sql));

  std::vector<std::string> dataSourceNames;

  while(dataset->moveNext())
    dataSourceNames.push_back(dataset->getString(0));

  ds->close();

  return dataSourceNames;
}


std::vector<std::string> te::pgis::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  std::vector<std::string> encodings;

  // Get an auxiliary data source
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

  std::string sql("SELECT DISTINCT pg_catalog.pg_encoding_to_char(conforencoding) FROM pg_catalog.pg_conversion ORDER BY pg_catalog.pg_encoding_to_char(conforencoding)");

  std::auto_ptr<te::da::DataSet> encs(ds->query(sql));

  while(encs->moveNext())
    encodings.push_back(encs->getString(0));

  ds->close();

  return encodings;
}
