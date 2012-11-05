/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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

  \brief The PostGIS driver.  
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../geometry/Geometry.h"
#include "Connection.h"
#include "ConnectionPool.h"
#include "DataSource.h"
#include "DataTypes.h"
#include "Exception.h"
#include "Globals.h"
#include "Transactor.h"
#include "Utils.h"

// STL
#include <cassert>
#include <cstring>
#include <memory>

// libpq
#include <libpq-fe.h>

std::map<std::string, std::string> te::pgis::DataSource::sm_capabilities;
te::da::SQLDialect* te::pgis::DataSource::sm_myDialect(0);

te::pgis::DataSource::DataSource()
  : m_catalog(0),
    m_pool(0),
    m_geomTypeOid(0),
    m_rasterTypeOid(0),
    m_currentSchema(0),
    m_timeIsInteger(true)
{
  m_catalog = new te::da::DataSourceCatalog;
  m_catalog->setDataSource(this);
  m_pool = new ConnectionPool(this);
}

te::pgis::DataSource::~DataSource()
{
  delete m_catalog;
  delete m_pool;
  delete m_currentSchema;
}

const std::string& te::pgis::DataSource::getType() const
{
  return Globals::sm_driverIdentifier;
}

const std::map<std::string, std::string>& te::pgis::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

void te::pgis::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connectionInfo = connInfo;
}

void te::pgis::DataSource::getCapabilities(std::map<std::string, std::string>& capabilities) const
{
  capabilities = sm_capabilities;
}

const te::da::SQLDialect* te::pgis::DataSource::getDialect() const
{
   return sm_myDialect;
}

void te::pgis::DataSource::open()
{
  close();

  m_pool->initialize();

  std::auto_ptr<Transactor> t(getPGTransactor());

// find PostGIS types
  m_geomTypeOid = t->getGeomTypeId();
  m_rasterTypeOid = t->getRasterTypeId();

  if(m_currentSchema == 0)
    m_currentSchema = new std::string;

// find PostgreSQL current schema of the connection
  t->getDatabaseInfo(*m_currentSchema);
}

void te::pgis::DataSource::close()
{
  m_pool->finalize();

  delete m_currentSchema;
  m_currentSchema = 0;
}


bool te::pgis::DataSource::isOpened() const
{
  return m_pool->isInitialized();
}

bool te::pgis::DataSource::isValid() const
{
  return m_pool->isValid();
}

te::da::DataSourceCatalog* te::pgis::DataSource::getCatalog() const
{
  return m_catalog;
}

te::da::DataSourceTransactor* te::pgis::DataSource::getTransactor()
{
  Connection* conn = m_pool->getConnection();

  return new Transactor(this, conn);
}

void te::pgis::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_PGIS("Not implemented yet!"));
}

te::pgis::Transactor* te::pgis::DataSource::getPGTransactor()
{
  Connection* conn = m_pool->getConnection();

  return new Transactor(this, conn);
}

void te::pgis::DataSource::setDialect(te::da::SQLDialect* myDialect)
{
  delete sm_myDialect;
  sm_myDialect = myDialect;
}

void te::pgis::DataSource::setCapabilities(const std::map<std::string, std::string>& capabilities)
{
  sm_capabilities = capabilities;
}

void te::pgis::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
// let's have a connection to the auxiliary database
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

// now, let's create the database creation command
  std::string sql = "CREATE DATABASE ";

  std::map<std::string, std::string>::const_iterator it = dsInfo.find("PG_NEWDB_NAME");
  std::map<std::string, std::string>::const_iterator it_end = dsInfo.end();

  if(it != it_end)
    sql += it->second;
  else
    throw Exception(TR_PGIS("Can not create database due to missing a parameter: PG_NEWDB_NAME."));

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

// let's get a transactor to execute the command
  std::auto_ptr<Transactor> t(ds->getPGTransactor());

  t->execute(sql);

// assure that this datasource is closed
  close();

// copy database connection parameters to this new data source object.
  it = dsInfo.find("PG_NEWDB_HOST");

  if(it == it_end)
    it = dsInfo.find("PG_HOST");

  if(it != it_end)
    m_connectionInfo["PG_HOST"] = it->second;

  it = dsInfo.find("PG_NEWDB_HOSTADDR");

  if(it == it_end)
    it = dsInfo.find("PG_HOST_ADDR");

  if(it != it_end)
    m_connectionInfo["PG_HOST_ADDR"] = it->second;

  it = dsInfo.find("PG_NEWDB_PORT");

  if(it == it_end)
    it = dsInfo.find("PG_PORT");

  if(it != it_end)
    m_connectionInfo["PG_PORT"] = it->second;

  it = dsInfo.find("PG_NEWDB_NAME");

  if(it != it_end)
    m_connectionInfo["PG_DB_NAME"] = it->second;

  it = dsInfo.find("PG_NEWDB_USER");

  if(it == it_end)
    it = dsInfo.find("PG_USER");

  if(it != it_end)
    m_connectionInfo["PG_USER"] = it->second;

  it = dsInfo.find("PG_NEWDB_PASSWORD");

  if(it == it_end)
    it = dsInfo.find("PG_PASSWORD");

  if(it != it_end)
    m_connectionInfo["PG_PASSWORD"] = it->second;

  it = dsInfo.find("PG_NEWDB_CONNECT_TIMEOUT");

  if(it == it_end)
    it = dsInfo.find("PG_CONNECT_TIMEOUT");

  if(it != it_end)
    m_connectionInfo["PG_CONNECT_TIMEOUT"] = it->second;

  it = dsInfo.find("PG_NEWDB_OPTIONS");

  if(it == it_end)
    it = dsInfo.find("PG_OPTIONS");

  if(it != it_end)
    m_connectionInfo["PG_OPTIONS"] = it->second;

  it = dsInfo.find("PG_NEWDB_SSL_MODE");

  if(it == it_end)
    it = dsInfo.find("PG_SSL_MODE");

  if(it != it_end)
    m_connectionInfo["PG_SSL_MODE"] = it->second;

  it = dsInfo.find("PG_NEWDB_KRBSRVNAME");

  if(it == it_end)
    it = dsInfo.find("PG_KRBSRVNAME");

  if(it != it_end)
    m_connectionInfo["PG_KRBSRVNAME"] = it->second;

  it = dsInfo.find("PG_NEWDB_GSSLIB");

  if(it == it_end)
    it = dsInfo.find("PG_GSSLIB");

  if(it != it_end)
    m_connectionInfo["PG_GSSLIB"] = it->second;

  it = dsInfo.find("PG_NEWDB_INITIAL_POOL_SIZE");

  if(it == it_end)
    it = dsInfo.find("PG_INITIAL_POOL_SIZE");

  if(it != it_end)
    m_connectionInfo["PG_INITIAL_POOL_SIZE"] = it->second;

  it = dsInfo.find("PG_NEWDB_MIN_POOL_SIZE");

  if(it == it_end)
    it = dsInfo.find("PG_MIN_POOL_SIZE");

  if(it != it_end)
    m_connectionInfo["PG_MIN_POOL_SIZE"] = it->second;

  it = dsInfo.find("PG_NEW_DB_MAX_POOL_SIZE");

  if(it == it_end)
    it = dsInfo.find("PG_MAX_POOL_SIZE");

  if(it != it_end)
    m_connectionInfo["PG_MAX_POOL_SIZE"] = it->second;

  it = dsInfo.find("PG_NEWDB_MAX_IDLE_TIME");

  if(it == it_end)
    it = dsInfo.find("PG_MAX_IDLE_TIME");

  if(it != it_end)
    m_connectionInfo["PG_MAX_IDLE_TIME"] = it->second;

  it = dsInfo.find("PG_NEWDB_CLIENT_ENCODING");

  if(it == it_end)
    it = dsInfo.find("PG_CLIENT_ENCODING");

  if(it != it_end)
    m_connectionInfo["PG_CLIENT_ENCODING"] = it->second;
}

void te::pgis::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
// let's have a connection to the auxiliary database
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

// now let's create the drop command
  std::string sql = "DROP DATABASE ";

  std::map<std::string, std::string>::const_iterator it = dsInfo.find("PG_DB_TO_DROP");

  if(it == dsInfo.end())
    throw Exception(TR_PGIS("Can not drop database due to a missing parameter: PG_DB_TO_DROP."));

  if((it->second == "postgres") || (it->second == "template_postgis"))
    throw Exception(TR_PGIS("Not allowed to drop database called postgres or template_postgis!"));

  sql += it->second;

// let's get a transactor to execute the command
  std::auto_ptr<Transactor> t(ds->getPGTransactor());

  t->execute(sql);

// assure that this datasource is closed
  close();
}

bool te::pgis::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  if(dsInfo.count("PG_CHECK_DB_EXISTENCE") == 0)
    throw Exception(TR_PGIS("Could not check PostgreSQL database existence due to missing parameter: PG_CHECK_DB_EXISTENCE!"));

  const std::string& dbName = dsInfo.find("PG_CHECK_DB_EXISTENCE")->second;

// let's have an auxiliary connection
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

// try to check
  std::string sql("SELECT * FROM pg_database WHERE datname = '");
              sql += dbName;
              sql += "'";

  std::auto_ptr<te::da::DataSourceTransactor> transactor(ds->getTransactor());

  std::auto_ptr<te::da::DataSet> database(transactor->query(sql));

  return database->moveNext();
}

