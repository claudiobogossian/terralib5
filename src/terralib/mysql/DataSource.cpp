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
  \file terralib/mysql/DataSource.cpp

  \brief Implements the DataSource class for MySQL data access driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/BatchExecutor.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../plugin/AbstractPlugin.h"
#include "../plugin/PluginInfo.h"
#include "../plugin/PluginManager.h"
#include "ConnectionPool.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Globals.h"

// Boost
#include <boost/filesystem.hpp>

// static's initialization
//const te::da::SQLDialect* te::mysql::DataSource::sm_dialect(0);
te::da::DataSourceCapabilities te::mysql::DataSource::sm_capabilities;

// members implementations
te::mysql::DataSource::DataSource()
  : m_pool(0),
    m_catalog(0)
{
  m_catalog.reset(new te::da::DataSourceCatalog);
  m_catalog->setDataSource(this);
  m_pool.reset(new ConnectionPool(this));
}

te::mysql::DataSource::~DataSource()
{
}

const std::string& te::mysql::DataSource::getType() const
{
  return Globals::sm_driverIdentifier;
}

const std::map<std::string, std::string>& te::mysql::DataSource::getConnectionInfo() const
{
  return m_dsInfo;
}

void te::mysql::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_dsInfo = connInfo;
}


const te::da::DataSourceCapabilities& te::mysql::DataSource::getCapabilities() const
{
  ////if(m_isrw)
  //  capabilities["ACCESS_POLICY"] = "RW_ACCESS";
  ////else
  ////  capabilities["ACCESS_POLICY"] = "R_ACCESS";

  //capabilities["TRANSACTION"] = "TRUE";
  //capabilities["DATASET_PERSISTENCE"] = "TRUE";
  //capabilities["DATASETTYPE_PERSISTENCE"] = "TRUE";
  //capabilities["PREPARED_QUERY"] = "TRUE";
  //capabilities["BATCH_EXECUTOR"] = "TRUE";

  //capabilities["BIT_DT"] = "FALSE";
  //capabilities["CHAR_DT"] = "FALSE";
  //capabilities["UCHAR_DT"] = "FALSE";
  //capabilities["INT16_DT"] = "TRUE";
  //capabilities["UINT16_DT"] = "FALSE";
  //capabilities["INT32_DT"] = "TRUE";
  //capabilities["UINT32_DT"] = "FALSE";
  //capabilities["INT64_DT"] = "TRUE";
  //capabilities["UINT64_DT"] = "FALSE";
  //capabilities["BOOLEAN_DT"] = "FALSE";
  //capabilities["FLOAT_DT"] = "FALSE";
  //capabilities["DOUBLE_DT"] = "TRUE";
  //capabilities["NUMERIC_DT"] = "FALSE";
  //capabilities["STRING_DT"] = "TRUE";
  //capabilities["BYTE_ARRAY_DT"] = "TRUE";
  //capabilities["GEOMETRY_DT"] = "TRUE";
  //capabilities["DATETIME_DT"] = "TRUE";
  //capabilities["ARRAY_DT"] = "FALSE";
  //capabilities["COMPOSITE_DT"] = "FALSE";
  //capabilities["RASTER_DT"] = "TRUE";
  //capabilities["DATASET_DT"] = "FALSE";
  //capabilities["XML_DT"] = "FALSE";

  //capabilities["PRIMARY_KEY"] = "TRUE";
  //capabilities["UNIQUE_KEY"] = "TRUE";
  //capabilities["FOREIGN_KEY"] = "TRUE";
  //capabilities["SEQUENCE"] = "FALSE";
  //capabilities["CHECK_CONSTRAINTS"] = "TRUE";
  //capabilities["INDEX"] = "TRUE";
  //capabilities["RTREE_INDEX"] = "TRUE";
  //capabilities["BTREE_INDEX"] = "TRUE";
  //capabilities["HASH_INDEX"] = "FALSE";
  //capabilities["QUADTREE_INDEX"] = "FALSE";

  //capabilities["SUPPORTS_RANDOM_DATASET"] = "FALSE";
  //capabilities["EFFICIENT_MOVE_PREVIOUS"] = "FALSE";
  //capabilities["EFFICIENT_MOVE_FIRST"] = "TRUE";
  //capabilities["EFFICIENT_MOVE_BEFORE_FIRST"] = "TRUE";
  //capabilities["EFFICIENT_MOVE_LAST"] = "FALSE";
  //capabilities["EFFICIENT_MOVE_AFTER_LAST"] = "FALSE";
  //capabilities["EFFICIENT_MOVE"] = "FALSE";
  //capabilities["EFFICIENT_DATASET_SIZE"] = "FALSE";
  //capabilities["DATASET_INSERTION"] = "FALSE";
  //capabilities["DATASET_UPDATE"] = "FALSE";
  //capabilities["DATASET_DELETION"] = "FALSE";

  //capabilities["SQL_DIALECT"] = "TRUE";
  //capabilities["INSERT_QUERY"] = "TRUE";
  //capabilities["UPDATE_QUERY"] = "TRUE";
  //capabilities["DELETE_QUERY"] = "TRUE";
  //capabilities["CREATE_QUERY"] = "TRUE";
  //capabilities["DROP_QUERY"] = "TRUE";
  //capabilities["ALTER_QUERY"] = "TRUE";
  //capabilities["SELECT_QUERY"] = "TRUE";
  //capabilities["SELECT_INTO_QUERY"] = "TRUE";

  return sm_capabilities;
}

const te::da::SQLDialect* te::mysql::DataSource::getDialect() const
{
  return Globals::sm_queryDialect;
}

void te::mysql::DataSource::open()
{
  close();

  m_pool->initialize();

  createMetadata();

  loadRasters();
}

void te::mysql::DataSource::close()
{
  m_pool->finalize();

  te::common::FreeContents(m_rasters);

  m_rasters.clear();
}

bool te::mysql::DataSource::isOpened() const
{
  return m_pool->isInitialized();
}

bool te::mysql::DataSource::isValid() const
{
  return m_pool->isValid();
}

te::da::DataSourceCatalog* te::mysql::DataSource::getCatalog() const
{
  return m_catalog.get();
}

te::da::DataSourceTransactor* te::mysql::DataSource::getTransactor()
{
  Connection* conn = m_pool->getConnection();

  return new DataSourceTransactor(this, conn);
}

void te::mysql::DataSource::optimize(const std::map<std::string, std::string>& opInfo)
{
  std::auto_ptr<DataSourceTransactor> t(getMyTransactor());

  std::map<std::string, std::string>::const_iterator it = opInfo.find("OPTIMIZE TABLE");

  if(it != opInfo.end())
  {
    std::string optmizationQuery("OPTIMIZE TABLE ");
    optmizationQuery += it->second;

    t->execute(optmizationQuery);
  }

  it = opInfo.find("ANALYZE TABLE");

  if(it != opInfo.end())
  {
    std::string optmizationQuery("ANALYZE TABLE ");
    optmizationQuery += it->second;

    t->execute(optmizationQuery);
  }
}

te::mysql::DataSourceTransactor* te::mysql::DataSource::getMyTransactor()
{
  Connection* conn = m_pool->getConnection();

  return new DataSourceTransactor(this, conn);
}

te::mysql::ConnectionPool* te::mysql::DataSource::getPool() const
{
  return m_pool.get();
}

bool te::mysql::DataSource::isRaster(const std::string& datasetName) const
{
  return m_rasters.find(datasetName) != m_rasters.end();
}

void te::mysql::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
// let's have an auxiliary connection
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  //ds->m_dsInfo["MY_CREATE_OGC_METADATA_TABLES"] = "FALSE";
  //ds->m_dsInfo["MY_CREATE_TERRALIB_RASTER_METADATA_TABLES"] = "FALSE";

  ds->open();

// try to create database
  std::string cmd("CREATE DATABASE IF NOT EXISTS ");

  if(dsInfo.count("MY_NEW_SCHEMA_NAME") > 0)
    cmd += dsInfo.find("MY_NEW_SCHEMA_NAME")->second;
  else
    throw Exception(TR_MYSQL("Could not create a new MySQL database due to missing parameter: MY_NEW_SCHEMA_NAME!"));

// create specification if the parameters was passed 
  if(dsInfo.count("MY_NEW_SCHEMA_CHARSET_NAME") > 0) 
  {
    cmd += " CHARACTER SET = "; 
    cmd += dsInfo.find("MY_NEW_SCHEMA_CHARSET_NAME")->second;
  }

  if(dsInfo.count("MY_NEW_SCHEMA_COLLATE_NAME") > 0)  
  {
    cmd += " COLLATE = "; 
    cmd += dsInfo.find("MY_NEW_SCHEMA_COLLATE_NAME")->second;
  }

  std::auto_ptr<DataSourceTransactor> transactor(ds->getMyTransactor());

  transactor->execute(cmd);

  close();

// copy database connection parameters to this new data source object.  
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("MY_NEW_SCHEMA_HOST");
  std::map<std::string, std::string>::const_iterator it_end = dsInfo.end();

  if(it == it_end)
    it = dsInfo.find("MY_HOST_NAME");

  if(it != it_end)
    m_dsInfo["MY_HOST_NAME"] = it->second;

  it = dsInfo.find("MY_NEW_SCHEMA_USER_NAME");

  if(it == it_end)
    it = dsInfo.find("MY_USER_NAME");

  if(it != it_end)
    m_dsInfo["MY_USER_NAME"] = it->second;

  it = dsInfo.find("MY_NEW_SCHEMA_PASSWORD");

  if(it == it_end)
    it = dsInfo.find("MY_PASSWORD");

  if(it != it_end)
    m_dsInfo["MY_PASSWORD"] = it->second;

  it = dsInfo.find("MY_NEW_SCHEMA_NAME");

  if(it != it_end)
    m_dsInfo["MY_SCHEMA"] = it->second;

  it = dsInfo.find("MY_NEW_SCHEMA_PORT");

  if(it == it_end)
    it = dsInfo.find("MY_PORT");

  if(it != it_end)
    m_dsInfo["MY_PORT"] = it->second;

  it = dsInfo.find("MY_NEW_SCHEMA_CHARSET_NAME");

  if(it == it_end)
    it = dsInfo.find("CHARSET_NAME");

  if(it != it_end)
    m_dsInfo["CHARSET_NAME"] = it->second;

  it = dsInfo.find("MY_NEW_SCHEMA_OPT_CONNECT_TIMEOUT");

  if(it == it_end)
    it = dsInfo.find("MY_OPT_CONNECT_TIMEOUT");

  if(it != it_end)
    m_dsInfo["MY_OPT_CONNECT_TIMEOUT"] = it->second;

  it = dsInfo.find("MY_NEW_SCHEMA_CREATE_OGC_METADATA_TABLES");

  if(it != it_end)
    m_dsInfo["MY_CREATE_OGC_METADATA_TABLES"] = it->second;

  it = dsInfo.find("MY_NEW_SCHEMA_CREATE_TERRALIB_RASTER_METADATA_TABLES");

  if(it != it_end)
    m_dsInfo["MY_CREATE_TERRALIB_RASTER_METADATA_TABLES"] = it->second;

// just to create the metadata tables
  open();

  close();
}

void te::mysql::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
// let's have an auxiliary connection
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

// try to drop
  std::string cmd("DROP DATABASE IF EXISTS ");

  if(dsInfo.count("MY_DB_TO_DROP") > 0)
    cmd += dsInfo.find("MY_DB_TO_DROP")->second;
  else
    throw Exception(TR_MYSQL("Could not drop a MySQL database due to missing parameter: MY_DB_TO_DROP!"));

  std::auto_ptr<DataSourceTransactor> transactor(ds->getMyTransactor());

  transactor->execute(cmd);

  close();
}

bool te::mysql::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  if(dsInfo.count("MY_CHECK_DB_EXISTENCE") == 0)
    throw Exception(TR_MYSQL("Could not check MySQL database existence due to missing parameter: MY_CHECK_DB_EXISTENCE!"));

  const std::string& dbName = dsInfo.find("MY_CHECK_DB_EXISTENCE")->second;

// let's have an auxiliary connection
  std::auto_ptr<DataSource> ds(new DataSource());

  ds->setConnectionInfo(dsInfo);

  ds->open();

// try to check
  std::string sql("SHOW DATABASES");

  std::auto_ptr<DataSourceTransactor> transactor(ds->getMyTransactor());

  std::auto_ptr<te::da::DataSet> databases(transactor->query(sql));

  while(databases->moveNext())
  {
    if(databases->getString(0) == dbName)
      return true;
  }

  return false;
}

void te::mysql::DataSource::createMetadata()
{
// see if we need to create metadata tables
  std::auto_ptr<te::da::DataSourceTransactor> transactor(getTransactor());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  std::auto_ptr<te::da::BatchExecutor> bexecutor(transactor->getBatchExecutor());

  std::map<std::string, std::string>::const_iterator it = m_dsInfo.find("MY_CREATE_OGC_METADATA_TABLES");
  std::map<std::string, std::string>::const_iterator itend = m_dsInfo.end();

  if((it != itend) && (te::common::Convert2UCase(it->second) == "TRUE") && (!cloader->datasetExists("geometry_columns")))
  {
    const te::plugin::PluginInfo& plugin = te::plugin::PluginManager::getInstance().getPlugin(TE_MYSQL_PLUGIN_NAME);

    boost::filesystem::path spath = plugin.m_folder;

    spath /= "init_ogc.sql";

    std::map<std::string, std::string> options;
    options["EXEC_SCRIPT"] = "TRUE";

    //te::da::ScopedTransaction t(*transactor);

    bexecutor->execute(spath.string(), options);

    //t.commit();
  }

  it = m_dsInfo.find("MY_CREATE_TERRALIB_RASTER_METADATA_TABLES");

  if((it != itend) && (te::common::Convert2UCase(it->second) == "TRUE") && (!cloader->datasetExists("te_tiled_rasters")))
  {
    const te::plugin::PluginInfo& plugin = te::plugin::PluginManager::getInstance().getPlugin(TE_MYSQL_PLUGIN_NAME);

    boost::filesystem::path spath = plugin.m_folder;

    spath /= "init_terralib_raster.sql";

    std::map<std::string, std::string> options;
    options["EXEC_SCRIPT"] = "TRUE";

    //te::da::ScopedTransaction t(*transactor);

    bexecutor->execute(spath.string(), options);

    //t.commit();
  }
}

void te::mysql::DataSource::loadRasters()
{
  assert(m_rasters.empty());

  std::auto_ptr<te::da::DataSourceTransactor> transactor(getTransactor());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

  if(!cloader->datasetExists("te_tiled_rasters"))
    return;

  std::auto_ptr<te::da::DataSet> rdatasets(transactor->query("SELECT r_name FROM te_tiled_rasters"));

  while(rdatasets->moveNext())
    m_rasters[rdatasets->getString(0)] = 0;
}



