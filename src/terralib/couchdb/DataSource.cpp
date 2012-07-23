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
  \file terralib/couchdb/DataSource.cpp

  \brief Implements the DataSource class for CouchDB.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Globals.h"
#include "HttpClient.h"

// STL
#include <algorithm>
#include <cassert>

// Boost
#include <boost/format.hpp>

// JSONCpp
#include <json/json.h>

te::couchdb::DataSource::DataSource()
  : m_isOpened(false)
{
  m_catalog.reset(new te::da::DataSourceCatalog);
  m_catalog->setDataSource(this);
}

te::couchdb::DataSource::~DataSource()
{
}

const std::string& te::couchdb::DataSource::getType() const
{
  return Globals::sm_driverIdentifier;
}

const std::map<std::string, std::string>& te::couchdb::DataSource::getConnectionInfo() const
{
  return m_dsInfo;
}

void te::couchdb::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{
  m_dsInfo = connInfo;
}

void te::couchdb::DataSource::getCapabilities(std::map<std::string, std::string>& /*capabilities*/) const
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

const te::da::SQLDialect* te::couchdb::DataSource::getDialect() const
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

void te::couchdb::DataSource::open()
{
  close();

  std::map<std::string, std::string>::const_iterator it = m_dsInfo.find("COUCHDB_URI");
  std::map<std::string, std::string>::const_iterator itEnd = m_dsInfo.end();

  if(it != itEnd)
    m_serverURI = it->second;
  else
    m_serverURI = "http://127.0.0.1:5984";

  it = m_dsInfo.find("COUCHDB_DB_NAME");

  if(it == itEnd)
    throw Exception(TR_COUCHDB("The COUCHDB_DB_NAME parameter is mandatory!"));

  m_dbName = it->second;

  HttpClient hc;

  std::string response;
  std::string mime;

// curl server-url
// where server-url can be something like: http://127.0.0.1:5984
  hc.get(m_serverURI, response, mime);

  assert((mime == "text/plain; charset=utf-8") || (mime == "application/json; charset=utf-8"));

  Json::Value root;

  Json::Reader reader;

  bool parsingSuccessful = reader.parse(response, root);

  if(!parsingSuccessful)
    throw Exception((boost::format(TR_COUCHDB("Failed to parse a JSON file returned by CouchDB Server. Error: %1%")) % reader.getFormatedErrorMessages()).str());

  const Json::Value& srv = root["couchdb"];

  if(srv.isNull() || !srv.isString() || (srv.asString() != "Welcome"))
    throw Exception((boost::format(TR_COUCHDB("CouchDB server response was not correctly understood: %1%")) % response).str());

  const Json::Value& version = root["version"];

  if(version.isNull() || !version.isString())
    throw Exception((boost::format(TR_COUCHDB("CouchDB server version was not correctly understood: %1%")) % response).str());

  m_srvVersion = version.asString();

  m_isOpened = true;
}

void te::couchdb::DataSource::close()
{
  m_serverURI = "";
  m_dbName = "";
  m_srvVersion = "";
  m_isOpened = false;
  m_catalog->clear();
}

bool te::couchdb::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::couchdb::DataSource::isValid() const
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

te::da::DataSourceCatalog* te::couchdb::DataSource::getCatalog() const
{
  return m_catalog.get();
}

te::da::DataSourceTransactor* te::couchdb::DataSource::getTransactor()
{
  assert(isOpened());

  return new DataSourceTransactor(this);
}

void te::couchdb::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_COUCHDB("Not implemented yet!"));
}

void te::couchdb::DataSource::create(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("COUCHDB_URI");
  std::map<std::string, std::string>::const_iterator itEnd = dsInfo.end();

  std::string serverURI = (it != itEnd) ? it->second : std::string("http://127.0.0.1:5984");

  it = dsInfo.find("COUCHDB_DB_NAME");

  if(it == itEnd)
    throw Exception(TR_COUCHDB("The COUCHDB_DB_NAME parameter is mandatory!"));

  const std::string& newdb = it->second;

// curl -X PUT <server-url>/baseball
  std::string newdburi = serverURI + "/" + newdb;

  HttpClient hc;

  std::string response;
  std::string mime;

  hc.put(newdburi, response, mime);

  assert(mime == "text/plain; charset=utf-8");

  Json::Value root;

  Json::Reader reader;

  bool parsingSuccessful = reader.parse(response, root);

  if(!parsingSuccessful)
    throw Exception((boost::format(TR_COUCHDB("Failed to parse a JSON file returned by CouchDB Server. Error: %1%")) % reader.getFormatedErrorMessages()).str());

  const Json::Value& value = root["ok"];

  if(value.isNull() || !value.isBool())
    throw Exception((boost::format(TR_COUCHDB("Could not understand the returned response of CouchDB Server: %1%")) % response).str());

  if(value.asBool() == false)
    throw Exception((boost::format(TR_COUCHDB("Could not create CouchDB database %1%: %2%")) % newdb % response).str());
}

void te::couchdb::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("COUCHDB_URI");
  std::map<std::string, std::string>::const_iterator itEnd = dsInfo.end();

  std::string serverURI = (it != itEnd) ? it->second : std::string("http://127.0.0.1:5984");

  it = dsInfo.find("COUCHDB_DB_NAME");

  if(it == itEnd)
    throw Exception(TR_COUCHDB("The COUCHDB_DB_NAME parameter is mandatory!"));

  const std::string& dropdb = it->second;

// curl -X DELETE <server-url>/baseball
  std::string newdburi = serverURI + "/" + dropdb;

  HttpClient hc;

  std::string response;
  std::string mime;

  hc.del(newdburi, response, mime);

  assert(mime == "text/plain; charset=utf-8");

  Json::Value root;

  Json::Reader reader;

  bool parsingSuccessful = reader.parse(response, root);

  if(!parsingSuccessful)
    throw Exception((boost::format(TR_COUCHDB("Failed to parse a JSON file returned by CouchDB Server. Error: %1%")) % reader.getFormatedErrorMessages()).str());

  const Json::Value& value = root["ok"];

  if(value.isNull() || !value.isBool())
    throw Exception((boost::format(TR_COUCHDB("Could not understand the returned response of CouchDB Server: %1%")) % response).str());

  if(value.asBool() == false)
    throw Exception((boost::format(TR_COUCHDB("Could not drop CouchDB database %1%: %2%")) % dropdb % response).str());
}

bool te::couchdb::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("COUCHDB_URI");
  std::map<std::string, std::string>::const_iterator itEnd = dsInfo.end();

  std::string serverURI = (it != itEnd) ? it->second : std::string("http://127.0.0.1:5984");

  it = dsInfo.find("COUCHDB_DB_NAME");

  if(it == itEnd)
    throw Exception(TR_COUCHDB("The COUCHDB_DB_NAME parameter is mandatory!"));

  const std::string& searchedDB = it->second;

// curl -X GET server-url/_all_dbs
  std::string dburi = serverURI + "/_all_dbs";

  HttpClient hc;

  std::string response;
  std::string mime;

  hc.get(dburi, response, mime);

  assert(mime == "text/plain; charset=utf-8");

  Json::Value root;

  Json::Reader reader;

  bool parsingSuccessful = reader.parse(response, root);

  if(!parsingSuccessful)
    throw Exception((boost::format(TR_COUCHDB("Failed to parse a JSON file returned by CouchDB Server. Error: %1%")) % reader.getFormatedErrorMessages()).str());

  if(root.isNull() || !root.isArray())
    throw Exception((boost::format(TR_COUCHDB("The CouchDB server database list was not correctly understood: %1%")) % response).str());

  Json::Value::UInt ndbs = root.size();

  for(Json::Value::UInt i = 0; i != ndbs; ++i)
  {
    if(root[i].asString() == searchedDB)
      return true; 
  }

  return false;
}

