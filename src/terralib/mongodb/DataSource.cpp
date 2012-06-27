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
  \file terralib/mongodb/DataSource.cpp

  \brief Implements the DataSource class for MyDriver.
*/

// TerraLib
#include "../common/Translator.h"
#include "DataSource.h"
#include "Exception.h"

// Boost
#include <boost/format.hpp>

// MongoDB
#include <mongo/client/dbclient.h>

te::mongodb::DataSource::DataSource()
  : m_conn(0)
{
}

te::mongodb::DataSource::~DataSource()
{
  close();
}

const std::string& te::mongodb::DataSource::getType() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

const std::map<std::string, std::string>& te::mongodb::DataSource::getConnectionInfo() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_dsInfo = connInfo;
}

void te::mongodb::DataSource::getCapabilities(std::map<std::string, std::string>& /*capabilities*/) const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

const te::da::SQLDialect* te::mongodb::DataSource::getDialect() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSource::open()
{
// assure we are closed!
  close();

// get db parameters
  std::map<std::string, std::string>::const_iterator it = m_dsInfo.find("MONGODB_URI");

  std::string uri = (it != m_dsInfo.end()) ? it->second : std::string(MONGODB_DEFAULT_URI);

  it = m_dsInfo.find("MONGODB_DB_NAME");

  if(it == m_dsInfo.end())
    throw Exception(TR_MONGODB("Missing database name parameter!"));

  uri = uri + "/" + it->second;

// open connection
  try
  {
    m_conn = new mongo::DBClientConnection;
    m_conn->connect(uri);
  }
  catch(const mongo::DBException& e)
  {
    throw Exception(e.what());
  }
  catch(...)
  {
    throw Exception(TR_MONGODB("Unknown exception opening MongoDB connection!"));
  }
}

void te::mongodb::DataSource::close()
{
  if(m_conn == 0)
    return;

  delete m_conn;
  m_conn = 0;
}

bool te::mongodb::DataSource::isOpened() const
{
  return !m_conn;
}

bool te::mongodb::DataSource::isValid() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSourceCatalog* te::mongodb::DataSource::getCatalog() const
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::mongodb::DataSource::getTransactor()
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_MONGODB("Not implemented yet!"));
}

void te::mongodb::DataSource::create(const std::map<std::string, std::string>& /*dsInfo*/)
{
// don't apply to mongodb!
}

void te::mongodb::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("MONGODB_URI");

  std::string uri = (it != dsInfo.end()) ? it->second : std::string(MONGODB_DEFAULT_URI);

  it = dsInfo.find("MONGODB_DB_NAME");

  if(it == dsInfo.end())
    throw Exception(TR_MONGODB("Missing database name parameter!"));

  const std::string& db_name = it->second;

  try
  {
    mongo::DBClientConnection m_conn;

    m_conn.connect(uri);

    mongo::BSONObj info;

    m_conn.dropDatabase(db_name, &info);

    if(!info["ok"].boolean())
      throw Exception((boost::format(TR_MONGODB("Erro dropping MongoDB database %1%: %2%")) % db_name % info.toString()).str());
  }
  catch(const mongo::DBException& e)
  {
    throw Exception(e.what());
  }
  catch(...)
  {
    throw Exception(TR_MONGODB("Unknown exception opening MongoDB connection!"));
  }
}

bool te::mongodb::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("MONGODB_URI");

  std::string uri = (it != dsInfo.end()) ? it->second : std::string(MONGODB_DEFAULT_URI);

  it = dsInfo.find("MONGODB_DB_NAME");

  if(it == dsInfo.end())
    throw Exception(TR_MONGODB("Missing database name parameter!"));

  const std::string& db_name = it->second;

  try
  {
    mongo::DBClientConnection m_conn;
    m_conn.connect(uri);
    return m_conn.exists(db_name);
  }
  catch(const mongo::DBException& e)
  {
    throw Exception(e.what());
  }
  catch(...)
  {
    throw Exception(TR_MONGODB("Unknown exception opening MongoDB connection!"));
  }
}

