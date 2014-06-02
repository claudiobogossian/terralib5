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
  \file terralib/postgis/Connection.cpp

  \brief A class that implements a connection to a PostgreSQL database.  
*/

// TerraLib
#include "../common/Translator.h"
#include "Connection.h"
#include "Exception.h"

// STL
#include <cassert>
#include <string>

// libpq
#include <libpq-fe.h>

// Boost
#include <boost/format.hpp>

/*! \brief This will just close the PostgreSQL notice processor. */
static void PostGISNoticeProcessor(void* /*arg*/, const char* /*pszMessage*/);

PGresult* te::pgis::Connection::query(const std::string& query)
{
  PGresult* result = PQexecParams(m_pgconn, query.c_str(), 0, 0, 0, 0, 0, 1);

  if(PQresultStatus(result) != PGRES_TUPLES_OK)
  {
    boost::format errmsg(TE_TR("Could not retrieve the dataset due to the following error: %1%."));
                  errmsg = errmsg % PQerrorMessage(m_pgconn);

    PQclear(result);

    throw Exception(errmsg.str());
  }

  return result;
}

void te::pgis::Connection::execute(const std::string& command)
{
  PGresult* result = PQexec(m_pgconn, command.c_str());

  if((PQresultStatus(result) != PGRES_COMMAND_OK) &&
     (PQresultStatus(result) != PGRES_TUPLES_OK))
  {
    boost::format errmsg(TE_TR("Could not execute the sql statement due to the following error: %1%."));
    
    errmsg = errmsg % PQerrorMessage(m_pgconn);

    PQclear(result);

    throw Exception(errmsg.str());
  }

  PQclear(result);
}

te::pgis::Connection::~Connection()
{
  if(m_pgconn)
    PQfinish(m_pgconn);
}

te::pgis::Connection::Connection(ConnectionPool* pool, const std::string& conninfo, const std::string& cencoding, bool inuse)
  : m_pool(pool),
    m_pgconn(0),
    m_inuse(inuse),
    m_lastuse(boost::posix_time::second_clock::local_time())
{
  //if(conninfo.empty())
  //  return;

// it tries to create the connection object  
  m_pgconn = PQconnectdb(conninfo.c_str());

  ConnStatusType status = PQstatus(m_pgconn);

  if(status != CONNECTION_OK)
  {
    boost::format errmsg(TE_TR("It was not possible to create a connection to the given data source due to the following error: %1%."));

    errmsg = errmsg % PQerrorMessage(m_pgconn);

    PQfinish(m_pgconn);

    m_pgconn = 0;

    throw Exception(errmsg.str());
  }

  PQsetNoticeProcessor(m_pgconn, PostGISNoticeProcessor, this);

  if(cencoding.empty())
    return;

  if(PQsetClientEncoding(m_pgconn, cencoding.c_str()) == -1)
  {
    boost::format errmsg(TE_TR("It was not possible to set the client encoding for the PostGIS data source due to the following error: %1%."));

    errmsg = errmsg % PQerrorMessage(m_pgconn);

    PQfinish(m_pgconn);

    m_pgconn = 0;

    throw Exception(errmsg.str());
  }
}

static void PostGISNoticeProcessor(void* /*arg*/, const char* /*pszMessage*/)
{
}
