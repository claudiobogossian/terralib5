/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/Exception.h"
#include "Config.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Utils.h"

// STL
#include <cassert>

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

// SQLite
#include <sqlite3.h>

class te::sqlite::DataSource::Impl
{
  public:

    Impl()
      : m_db(0)
    {
    }

    ~Impl()
    {
      int ret = SQLITE_OK;
      ret = sqlite3_close(m_db);
      assert(ret == SQLITE_OK);
    }

    std::map<std::string, std::string> m_connInfo;
    sqlite3* m_db;

    static te::da::SQLDialect* sm_dialect;
    static te::da::DataSourceCapabilities* sm_capabilities;
};

te::da::SQLDialect* te::sqlite::DataSource::Impl::sm_dialect(0);
te::da::DataSourceCapabilities* te::sqlite::DataSource::Impl::sm_capabilities(0);

te::sqlite::DataSource::DataSource()
  : m_pImpl(0)
{
  m_pImpl = new Impl;
}

te::sqlite::DataSource::~DataSource()
{
  delete m_pImpl;
}

std::string te::sqlite::DataSource::getType() const
{
  return TE_SQLITE_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::sqlite::DataSource::getConnectionInfo() const
{
  return m_pImpl->m_connInfo;
}

void te::sqlite::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_pImpl->m_connInfo = connInfo;
}

std::auto_ptr<te::da::DataSourceTransactor> te::sqlite::DataSource::getTransactor()
{
  std::auto_ptr<te::da::DataSourceTransactor> t(new DataSourceTransactor(this, m_pImpl->m_db));
  return t;
}

void te::sqlite::DataSource::open()
{
  close();

  int flags = GetConnectionFlags(m_pImpl->m_connInfo);

  if(flags == 0)
    flags = SQLITE_OPEN_READONLY;

  std::string fileName;

  {
    std::map<std::string, std::string>::const_iterator it = m_pImpl->m_connInfo.find("SQLITE_FILE");

    if(it != m_pImpl->m_connInfo.end())
      fileName = it->second;
  }

  std::string vfs;

  {
    std::map<std::string, std::string>::const_iterator it = m_pImpl->m_connInfo.find("SQLITE_VFS");

    if(it != m_pImpl->m_connInfo.end())
      vfs = it->second;
  }

  int ret = sqlite3_open_v2(fileName.c_str(), &(m_pImpl->m_db), flags, vfs.empty() ? 0 : vfs.c_str());

  if(ret != SQLITE_OK)
  {
    boost::format errmsg(TR_COMMON("Could not open the SQLite data source due to the following error: %1%."));

    if(m_pImpl->m_db)
      errmsg = errmsg % sqlite3_errmsg(m_pImpl->m_db);
    else
      errmsg = errmsg % "unknown error";

    sqlite3_close(m_pImpl->m_db);

    m_pImpl->m_db = 0;

    throw te::da::Exception(errmsg.str());
  }

  std::auto_ptr<te::da::DataSourceTransactor> transactor(getTransactor());

  std::auto_ptr<te::da::DataSet> result(transactor->query("SELECT CheckSpatialMetaData()"));

  if(!result->moveNext())
    throw te::da::Exception(TR_COMMON("Could not check if SQLite database has spatial metadata tables due to unexpected error!"));

  int spatialMetaTableType = result->getInt32(0);

  if(spatialMetaTableType == 0)
    transactor->execute("SELECT InitSpatialMetadata(1)");
}

void te::sqlite::DataSource::close()
{
  if(m_pImpl->m_db == 0)
    return;

  int ret = sqlite3_close(m_pImpl->m_db);

  if(ret != SQLITE_OK)
  {
    boost::format errmsg(TR_COMMON("Could not close the SQLite data source due to the following error: %1%."));

    if(m_pImpl->m_db)
      errmsg = errmsg % sqlite3_errmsg(m_pImpl->m_db);
    else
      errmsg = errmsg % "unknown error";

    throw te::da::Exception(errmsg.str());
  }

  m_pImpl->m_db = 0;
}

bool te::sqlite::DataSource::isOpened() const
{
  return m_pImpl->m_db != 0;
}

bool te::sqlite::DataSource::isValid() const
{
  return m_pImpl->m_db != 0;
}

const te::da::DataSourceCapabilities& te::sqlite::DataSource::getCapabilities() const
{
  return *Impl::sm_capabilities;
}

const te::da::SQLDialect* te::sqlite::DataSource::getDialect() const
{
  return Impl::sm_dialect;
}

void te::sqlite::DataSource::set(te::da::DataSourceCapabilities* capabilities)
{
  delete Impl::sm_capabilities;
  Impl::sm_capabilities = capabilities;
}

void te::sqlite::DataSource::set(te::da::SQLDialect* dialect)
{
  delete Impl::sm_dialect;
  Impl::sm_dialect = dialect;
}

void te::sqlite::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  if(Exists(dsInfo))
    throw te::da::Exception(TR_COMMON("The informed SQLite database already exists!"));

  {
    std::map<std::string, std::string>::const_iterator it = dsInfo.find("SQLITE_CREATE_INTERMEDIATE_DIR");

    if((it != dsInfo.end()) && (boost::to_upper_copy(it->second) == "TRUE"))
    {
      it = dsInfo.find("SQLITE_FILE");

      if(it == dsInfo.end())
        throw te::da::Exception(TR_COMMON("You must at least provide a database file name if you want to create its intermediate directories!"));

      boost::filesystem::path fileName(it->second);

      if(fileName.has_parent_path() &&
         !boost::filesystem::exists(fileName.parent_path()) &&
         !boost::filesystem::create_directories(fileName.parent_path()))
          throw te::da::Exception(TR_COMMON("Could not create directory for SQLite database!"));
    }
  }

  std::map<std::string, std::string> info = dsInfo;

  info["SQLITE_OPEN_READWRITE"] = "true";
  info["SQLITE_OPEN_CREATE"] = "true";

  setConnectionInfo(info);
  open();
  close();
}

void te::sqlite::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  if(!Exists(dsInfo))
    throw te::da::Exception(TR_COMMON("The informed SQLite database doesn't exist!"));

  if((dsInfo.find("SQLITE_OPEN_URI") != dsInfo.end()) &&
     (boost::to_upper_copy(m_pImpl->m_connInfo.find("SQLITE_OPEN_URI")->second) == "TRUE"))
     throw te::da::Exception(TR_COMMON("Sorry! We don't support removing SQLite database through an URI!"));

  std::string fileName;

  {
    std::map<std::string, std::string>::const_iterator it = dsInfo.find("SQLITE_FILE");

    if(it != dsInfo.end())
      fileName = it->second;
    else
      throw te::da::Exception(TR_COMMON("To remove an SQLite database you must inform its file name!"));
  }

  boost::filesystem::remove(fileName);
}

bool te::sqlite::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  std::string fileName;

  std::map<std::string, std::string>::const_iterator it = dsInfo.find("SQLITE_FILE");

  if(it == dsInfo.end())
    throw te::da::Exception(TR_COMMON("In order to check the existence of a SQLite database you must inform its file name"));

  return boost::filesystem::exists(it->second);
}

std::vector<std::string> te::sqlite::DataSource::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  return std::vector<std::string>();
}

std::vector<std::string> te::sqlite::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  std::vector<std::string> encodings;
  encodings.push_back("UTF-8");
  return encodings;
}

