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
  \file Connection.cpp

  \brief A class that implements a connection to a MySQL database.  
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "Connection.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

// MySQL C++ connector
#include <driver/mysql_public_iface.h>

// Boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

void te::mysql::Connection::execute(const std::string& command)
{
  try
  {
    std::auto_ptr<sql::Statement> stmt(m_myconn->createStatement());

    stmt->execute(command);
  }
  catch (sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not execute the SQL command against the MySQL database due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown error executing a command in a MySQL database!"));
  }
}

te::mysql::Connection::~Connection()
{
  m_myconn->close();

  delete m_myconn;
}

te::mysql::Connection::Connection(ConnectionPool* pool, const std::map<std::string, std::string>& connInfo, bool inuse)
  : m_pool(pool),
    m_myconn(0),
    m_inuse(inuse),
    m_lastuse(boost::posix_time::second_clock::local_time())
{
  sql::Driver* driver = sql::mysql::get_driver_instance();

  sql::ConnectOptionsMap connProperties;

  std::map<std::string, std::string>::const_iterator it_end = connInfo.end();

  std::map<std::string, std::string>::const_iterator it = connInfo.find("MY_HOST_NAME");
  connProperties["hostName"] = it != it_end ? it->second : std::string(MYSQL_DEFAULT_HOST);

  it = connInfo.find("MY_PORT");
  connProperties["port"] = it != connInfo.end() ? boost::lexical_cast<int>(it->second) : MYSQL_DEFAULT_PORT;

  it = connInfo.find("MY_USER_NAME");
  connProperties["userName"] = it != connInfo.end() ? it->second : std::string("");

  it = connInfo.find("MY_PASSWORD");
  connProperties["password"] = it != connInfo.end() ? it->second : std::string("");

  it = connInfo.find("MY_SCHEMA");
  connProperties["schema"] = it != connInfo.end() ? it->second : std::string("");

  it = connInfo.find("MY_CLIENT_COMPRESS");

  if(it != connInfo.end())
    connProperties["CLIENT_COMPRESS"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_CLIENT_FOUND_ROWS");

  if(it != connInfo.end())
    connProperties["CLIENT_FOUND_ROWS"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_CLIENT_IGNORE_SIGPIPE");

  if(it != connInfo.end())
    connProperties["CLIENT_IGNORE_SIGPIPE"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_CLIENT_IGNORE_SPACE");

  if(it != connInfo.end())
    connProperties["CLIENT_IGNORE_SPACE"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_CLIENT_INTERACTIVE");

  if(it != connInfo.end())
    connProperties["CLIENT_INTERACTIVE"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_CLIENT_LOCAL_FILES");

  if(it != connInfo.end())
    connProperties["CLIENT_LOCAL_FILES"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_CLIENT_MULTI_RESULTS");

  if(it != connInfo.end())
    connProperties["CLIENT_MULTI_RESULTS"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_CLIENT_MULTI_STATEMENTS");

  if(it != connInfo.end())
    connProperties["CLIENT_MULTI_STATEMENTS"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_CLIENT_NO_SCHEMA");

  if(it != connInfo.end())
    connProperties["MY_CLIENT_NO_SCHEMA"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_OPT_CHARSET_NAME");

  if(it != connInfo.end())
    connProperties["OPT_CHARSET_NAME"] = it->second;

  it = connInfo.find("MY_SOCKET");

  if(it != connInfo.end())
    connProperties["socket"] = it->second;

  it = connInfo.find("MY_SSL_KEY");

  if(it != connInfo.end())
    connProperties["sslKey"] = it->second;

  it = connInfo.find("MY_SSL_CERT");

  if(it != connInfo.end())
    connProperties["sslCert"] = it->second;

  it = connInfo.find("MY_SSL_CA");

  if(it != connInfo.end())
    connProperties["sslCA"] = it->second;

  it = connInfo.find("MY_SSL_CA_PATH");

  if(it != connInfo.end())
    connProperties["sslCAPath"] = it->second;

  it = connInfo.find("MY_SSL_CIPHER");

  if(it != connInfo.end())
    connProperties["sslCipher"] = it->second;

  it = connInfo.find("MY_PIPE");

  if(it != connInfo.end())
    connProperties["pipe"] = it->second;

  it = connInfo.find("MY_OPT_CONNECT_TIMEOUT");

  if(it != connInfo.end())
    connProperties["OPT_CONNECT_TIMEOUT"] = boost::lexical_cast<int>(it->second);
  else
    connProperties["OPT_CONNECT_TIMEOUT"] = MYSQL_DEFAULT_CONNECT_TIMEOUT;

  it = connInfo.find("MY_OPT_READ_TIMEOUT");

  if(it != connInfo.end())
    connProperties["OPT_READ_TIMEOUT"] = boost::lexical_cast<int>(it->second);

  it = connInfo.find("MY_OPT_WRITE_TIMEOUT");

  if(it != connInfo.end())
    connProperties["OPT_WRITE_TIMEOUT"] = boost::lexical_cast<int>(it->second);

  it = connInfo.find("MY_OPT_RECONNECT");

  if(it != connInfo.end())
    connProperties["OPT_RECONNECT"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_OPT_REPORT_DATA_TRUNCATION");

  if(it != connInfo.end())
    connProperties["OPT_REPORT_DATA_TRUNCATION"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;

  it = connInfo.find("MY_CHARACTERSET_RESULTS");

  if(it != connInfo.end())
    connProperties["characterSetResults"] = it->second;

  it = connInfo.find("MY_DEFAULT_STATEMENT_RESULT_TYPE");

  if(it != connInfo.end())
    connProperties["defaultStatementResultType"] = boost::lexical_cast<int>(it->second);

  it = connInfo.find("MY_DEFAULT_PREPARED_STATEMENT_RESULT_TYPE");

  if(it != connInfo.end())
    connProperties["defaultPreparedStatementResultType"] = boost::lexical_cast<int>(it->second);

  it = connInfo.find("MY_METADATA_USE_INFO_SCHEMA");

  if(it != connInfo.end())
    connProperties["metadataUseInfoSchema"] = te::common::Convert2UCase(it->second) == "TRUE" ? true : false;
  
  try
  {
    m_myconn = driver->connect(connProperties);
  }
  catch(const sql::SQLException &e)
  {
    throw Exception((boost::format(TR_MYSQL("Could not open connection to MySQL database due to the following error: %1%")) % e.what()).str());
  }
  catch(...)
  {
    throw Exception(TR_MYSQL("Unknown error opening a connection to a MySQL database!"));
  }
}

