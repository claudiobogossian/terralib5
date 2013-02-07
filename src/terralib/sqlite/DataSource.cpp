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
  \file terralib/sqlite/DataSource.cpp

  \brief Implements the DataSource class for the SQLite Data Access Driver.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../dataaccess/datasource/ScopedTransaction.h"
#include "../plugin/AbstractPlugin.h"
#include "../plugin/PluginInfo.h"
#include "../plugin/PluginManager.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "FwDataSet.h"
#include "Globals.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

// SQLite
#include <sqlite3.h>

// static's initialization
te::da::DataSourceCapabilities te::sqlite::DataSource::sm_capabilities;

te::sqlite::DataSource::DataSource()
  : m_db(0),
    m_catalog(0),
    m_dialect(0),
    m_useSpatiaLite(true),
    m_isrw(false)
{
  m_catalog = new te::da::DataSourceCatalog;
  m_catalog->setDataSource(this);
}

te::sqlite::DataSource::~DataSource()
{
  int ret = SQLITE_OK;

  ret = sqlite3_close(m_db);

  assert(ret == SQLITE_OK);

  delete m_catalog;
}

const std::string& te::sqlite::DataSource::getType() const
{
  return Globals::sm_driverIdentifier;
}

const std::map<std::string, std::string>& te::sqlite::DataSource::getConnectionInfo() const
{
  return m_connInfo;
}

void te::sqlite::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_connInfo = connInfo;
}

const te::da::DataSourceCapabilities& te::sqlite::DataSource::getCapabilities() const
{
  //if(m_isrw)
  //  capabilities["ACCESS_POLICY"] = "RW_ACCESS";
  //else
  //  capabilities["ACCESS_POLICY"] = "R_ACCESS";

  //capabilities["TRANSACTION"] = "TRUE";
  //capabilities["DATASET_PERSISTENCE"] = "TRUE";
  //capabilities["DATASETTYPE_PERSISTENCE"] = "TRUE";
  //capabilities["PREPARED_QUERY"] = "TRUE";
  //capabilities["BATCH_EXECUTOR"] = "TRUE";

  //capabilities["BIT_DT"] = "FALSE";
  //capabilities["CHAR_DT"] = "FALSE";
  //capabilities["UCHAR_DT"] = "FALSE";
  //capabilities["INT16_DT"] = "FALSE";
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

const te::da::SQLDialect* te::sqlite::DataSource::getDialect() const
{
  return m_dialect;
}

void te::sqlite::DataSource::open()
{
// assure that we are in a closed state
  close();

// retrieve connection flags froom data source information
  int flags = GetConnectionFlags(m_connInfo);

// assure that at least SQLITE_OPEN_READONLY is present!
  if(flags == 0)
    flags = SQLITE_OPEN_READONLY;

// get file name (remember: it may be a null value!)
  std::string fileName;

  {
    std::map<std::string, std::string>::const_iterator it = m_connInfo.find("SQLITE_FILE");

    if(it != m_connInfo.end())
      fileName = it->second;
  }

// get VFS parameter
  std::string vfs;

  {
    std::map<std::string, std::string>::const_iterator it = m_connInfo.find("SQLITE_VFS");

    if(it != m_connInfo.end())
      vfs = it->second;
  }

// try to open the connection
  int ret = sqlite3_open_v2(fileName.c_str(), &m_db, flags, vfs.empty() ? 0 : vfs.c_str());

  if(ret != SQLITE_OK)
  {
    boost::format errmsg(TR_SQLITE("Could not open the SQLite data source due to the following error: %1%."));
    
    if(m_db)
      errmsg = errmsg % sqlite3_errmsg(m_db);
    else
      errmsg = errmsg % "unknown error";

    sqlite3_close(m_db);

    m_db = 0;

    throw Exception(errmsg.str());
  }

// check if spatial metadata table already exist
  std::auto_ptr<DataSourceTransactor> transactor(getSQLiteTransactor());

  std::auto_ptr<te::da::DataSet> result(transactor->query("SELECT CheckSpatialMetaData()"));

  if(!result->moveNext())
    throw Exception(TR_SQLITE("Could not check if SQLite database has spatial metadata tables due to unexpected error!"));

  int spatialMetaTableType = result->getInt32(0);
// CheckSpatialMetaData returns:
// 0 - if no valid SpatialMetaData were found
// 1 - if SpatiaLite-like SpatialMetadata were found
// 2 - if FDO-OGR-like SpatialMetadata were found

// if there isn't spatial metadata tables let's create them
  if(spatialMetaTableType == 0)
  {
    if((m_connInfo.find("SQLITE_CREATE_OGR_METADATA_TABLES") != m_connInfo.end()) &&
       (te::common::Convert2UCase(m_connInfo.find("SQLITE_CREATE_OGR_METADATA_TABLES")->second) == "TRUE"))
    {
      const te::plugin::PluginInfo& plugin = te::plugin::PluginManager::getInstance().getPlugin(TE_SQLITE_PLUGIN_NAME);

      boost::filesystem::path spath = plugin.m_folder;
      
      spath /= "init_ogc.sql";

      te::da::ScopedTransaction t(*transactor);

      ExecuteScript(m_db, spath.string().c_str());

      t.commit();

      spatialMetaTableType = 2;
    }
    else  // assume we will create SpatiaLite meta-tables == SQLITE_CREATE_SPATIALITE_METADATA_TABLES = true
    {
// !!SpatialLite will start a hidden transaction, so we don't need to start one at this point!!
      transactor->execute("SELECT InitSpatialMetadata()");

      spatialMetaTableType = 1;
    }
  }

  m_useSpatiaLite = spatialMetaTableType == 1;

// check if we are allowed to create TerraLib raster tables
  if((m_connInfo.find("SQLITE_CREATE_TERRALIB_RASTER_METADATA_TABLES") != m_connInfo.end()) &&
       (te::common::Convert2UCase(m_connInfo.find("SQLITE_CREATE_TERRALIB_RASTER_METADATA_TABLES")->second) == "TRUE"))
  {
// check if meta-tables are already created
    std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

    if(!cloader->datasetExists("te_tiled_rasters"))
    {
      const te::plugin::PluginInfo& plugin = te::plugin::PluginManager::getInstance().getPlugin(TE_SQLITE_PLUGIN_NAME);

      boost::filesystem::path spath = plugin.m_folder;
      
      spath /= "init_terralib_raster.sql";

      te::da::ScopedTransaction t(*transactor);

      ExecuteScript(m_db, spath.string().c_str());

      t.commit();
    }
  }

// set SQL dialect
  if(m_useSpatiaLite)
    m_dialect = Globals::sm_spatialiteDialect;
  else
    m_dialect = Globals::sm_sqliteDialect;
}

void te::sqlite::DataSource::close()
{
  if(m_db == 0)
    return;

  int ret = sqlite3_close(m_db);

  if(ret != SQLITE_OK)
  {
    boost::format errmsg(TR_SQLITE("Could not close the SQLite data source due to the following error: %1%."));

    if(m_db)
      errmsg = errmsg % sqlite3_errmsg(m_db);
    else
      errmsg = errmsg % "unknown error";

    throw Exception(errmsg.str());
  }

  m_db = 0;

  m_dialect = 0;
}

bool te::sqlite::DataSource::isOpened() const
{
  return m_db != 0;
}

bool te::sqlite::DataSource::isValid() const
{
  return m_db != 0; // in future we should try sqlite3_db_status if there is a constant to this end!
}

te::da::DataSourceCatalog* te::sqlite::DataSource::getCatalog() const
{
  return m_catalog;
}

te::da::DataSourceTransactor* te::sqlite::DataSource::getTransactor()
{
  return new DataSourceTransactor(this);
}

void te::sqlite::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  std::auto_ptr<DataSourceTransactor> t(getSQLiteTransactor());

  t->execute("VACUUM");
}

te::sqlite::DataSourceTransactor* te::sqlite::DataSource::getSQLiteTransactor()
{
  return new DataSourceTransactor(this);
}

//void te::sqlite::DataSource::setCapabilities(const std::map<std::string, std::string>& capabilities)
//{
//  m_capabilities = capabilities;
//}

void te::sqlite::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
// if database already exists raise an exception
  if(Exists(dsInfo))
    throw Exception(TR_SQLITE("The informed SQLite database already exists!"));

// if intermediate dir is required we must check if we may create them
  {
    std::map<std::string, std::string>::const_iterator it = dsInfo.find("SQLITE_CREATE_INTERMEDIATE_DIR");

    if((it != dsInfo.end()) && (te::common::Convert2UCase(it->second) == "TRUE"))
    {
      it = dsInfo.find("SQLITE_FILE");

      if(it == dsInfo.end())
        throw Exception(TR_SQLITE("You must at least provide a database file name if you want to create its intermediate directories!"));

      boost::filesystem::path fileName(it->second);

      if(fileName.has_parent_path() &&
         !boost::filesystem::exists(fileName.parent_path()) &&
         !boost::filesystem::create_directories(fileName.parent_path()))
          throw Exception(TR_SQLITE("Could not create directory for SQLite database!"));
    }
  }

// force creational flags
  std::map<std::string, std::string> info = dsInfo;

  info["SQLITE_OPEN_READWRITE"] = "true";
  info["SQLITE_OPEN_CREATE"] = "true";

// open can be used to create the database
  te::da::DataSource::open(info);

// and after all... we close the database
  close();
}

void te::sqlite::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
// if database doesn't exists raise an exception
  if(!Exists(dsInfo))
    throw Exception(TR_SQLITE("The informed SQLite database doesn't exist!"));

// URI is not supported yet!
  if((dsInfo.find("SQLITE_OPEN_URI") != dsInfo.end()) &&
     (te::common::Convert2UCase(m_connInfo.find("SQLITE_OPEN_URI")->second) == "TRUE"))
     throw Exception(TR_SQLITE("Sorry! We don't support removing SQLite database through an URI!"));

// get file name (remember: it can not be a null value!)
  std::string fileName;

  {
    std::map<std::string, std::string>::const_iterator it = dsInfo.find("SQLITE_FILE");

    if(it != dsInfo.end())
      fileName = it->second;
    else
      throw Exception(TR_SQLITE("To remove an SQLite database you must inform its file name!"));
  }

// remove from file system
  // this is an unecessary check because Exists already check the file existence
  //if(!boost::filesystem::is_regular_file(fileName))
  //  throw Exception(TR_SQLITE("Can only remove database files!"));

  boost::filesystem::remove(fileName);
}

bool te::sqlite::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  std::string fileName;

  std::map<std::string, std::string>::const_iterator it = dsInfo.find("SQLITE_FILE");

  if(it == dsInfo.end())
    throw Exception(TR_SQLITE("To check the existence of SQLite databases you must inform at least the file name!"));

  return boost::filesystem::exists(it->second);
}


