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
  \file terralib/ado2/ConnectionPool.cpp

  \brief A class that implements a connection pool for ADO.  
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/datasource/ConnectionPoolManager.h"
#include "Connection.h"
#include "ConnectionPool.h"
#include "DataSource.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <list>

// BOOST
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

class te::ado::ConnectionPool::ConnectionPoolImpl
{
  public:

    ConnectionPoolImpl(te::ado::DataSource* ds)
      : m_ds(ds),
        m_initialPoolSize(ADO_DEFAULT_INITIAL_POOL_SIZE),
        m_minPoolSize(ADO_DEFAULT_MIN_POOL_SIZE),
        m_maxPoolSize(ADO_DEFAULT_MAX_POOL_SIZE),
        m_poolSize(0),
        m_maxIdleTime(ADO_DEFAULT_MAX_IDLE_TIME),
        m_initialized(false)
    {
    }

    ~ConnectionPoolImpl()
    {
      te::common::FreeContents(m_connections);
    }

    std::string m_conninfo;                            //!< The connection info.
    std::string m_cencoding;                           //!< Client encoding.
    te::ado::DataSource* m_ds;                        //!< The data source using the pool.
    std::size_t m_initialPoolSize;                     //!< It indicates the initial number of connections opened by the pool.
    std::size_t m_minPoolSize;                         //!< It indicates the minimum number of connections in the pool.
    std::size_t m_maxPoolSize;                         //!< It indicates the maximum number of connections in the pool.
    std::size_t m_poolSize;                            //!< It indicates the maximum number of connections in the pool.
    unsigned int m_maxIdleTime;                        //!< The number of seconds waiting for releasing connections.
    std::list<te::ado::Connection*> m_connections;    //!< The list of available connections ready to be used.
    boost::mutex m_mtx;                                //!< A mutex for preventing reading/writing in the connection list.
    bool m_initialized;                                //!< A flag that indicates if the pool was initialized or not.
};

void te::ado::ConnectionPool::initialize()
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  if(isInitialized())
    throw Exception(TR_ADO("The connection pool is already initialized!"));

// check for pool parameters...
  const std::map<std::string, std::string>& connInfo = m_pImpl->m_ds->getConnectionInfo();

  std::map<std::string, std::string>::const_iterator itend = connInfo.end();

  std::map<std::string, std::string>::const_iterator it = connInfo.find("ADO_INITIAL_POOL_SIZE");
  m_pImpl->m_initialPoolSize = (it != itend && !it->second.empty()) ? atoi(it->second.c_str()) : ADO_DEFAULT_INITIAL_POOL_SIZE;
  
  it = connInfo.find("ADO_MIN_POOL_SIZE");
  m_pImpl->m_minPoolSize = (it != itend && !it->second.empty()) ? atoi(it->second.c_str()) : ADO_DEFAULT_MIN_POOL_SIZE;
  
  it = connInfo.find("ADO_MAX_POOL_SIZE");
  m_pImpl->m_maxPoolSize = (it != itend && !it->second.empty()) ? atoi(it->second.c_str()) : ADO_DEFAULT_MAX_POOL_SIZE;

// assure valid values for pool parameters
  if(m_pImpl->m_minPoolSize > m_pImpl->m_maxPoolSize)
    m_pImpl->m_minPoolSize = m_pImpl->m_maxPoolSize;

  if(m_pImpl->m_initialPoolSize > m_pImpl->m_maxPoolSize)
    m_pImpl->m_initialPoolSize = m_pImpl->m_maxPoolSize;
  else if(m_pImpl->m_initialPoolSize < m_pImpl->m_minPoolSize)
    m_pImpl->m_initialPoolSize = m_pImpl->m_minPoolSize;

  it = connInfo.find("ADO_MAX_IDLE_TIME");
  m_pImpl->m_maxIdleTime = (it != itend && !it->second.empty()) ? atoi(it->second.c_str()) : ADO_DEFAULT_MAX_IDLE_TIME;

  it = connInfo.find("ADO_CLIENT_ENCODING");
  m_pImpl->m_cencoding = (it != itend ? it->second : std::string(""));

// make connection info
  m_pImpl->m_conninfo = MakeConnectionStr(connInfo);

// try to open the connections
  for(std::size_t i = 0; i < m_pImpl->m_initialPoolSize; ++i)
  {
    Connection* conn = new Connection(this, m_pImpl->m_conninfo, m_pImpl->m_cencoding, false);
    m_pImpl->m_connections.push_back(conn);
    ++(m_pImpl->m_poolSize);
  }

// verify the internal date time storage
  //if(!m_pImpl->m_connections.empty())
  //{
  //  std::string off = "off";
  //  
  //  std::string answer = PQparameterStatus(m_pImpl->m_connections.front()->m_pgconn, "integer_datetimes");

  //  if(answer == off)
  //    m_pImpl->m_ds->setTimeAsInteger(false);
  //  else
  //    m_pImpl->m_ds->setTimeAsInteger(true);
  //}

  m_pImpl->m_initialized = true;
}

void te::ado::ConnectionPool::finalize()
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

// if there is a connection in use it will raise an exception!
  std::list<te::ado::Connection*>::iterator it = m_pImpl->m_connections.begin();
  std::list<te::ado::Connection*>::iterator itend = m_pImpl->m_connections.end();

  while(it != itend)
  {
    if((*it)->m_inuse)
      throw Exception(TR_ADO("There are opened connections. Please, close all connections before finalizing the connection pool."));

    ++it;
  }

// release all connections
  te::common::FreeContents(m_pImpl->m_connections);

  m_pImpl->m_connections.clear();
  m_pImpl->m_poolSize = 0;

  m_pImpl->m_initialized = false;
}

void te::ado::ConnectionPool::idle()
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

// must we check the pool?
// no: we don't care about monitoring
// no: we weren't initialized
// no: if the pool has a fixed size we don't need to care about it!
// no: we have just the minimum pool connections
  if((m_pImpl->m_maxIdleTime == 0) ||
    !isInitialized() ||
    (m_pImpl->m_maxPoolSize == m_pImpl->m_minPoolSize) ||
    (m_pImpl->m_poolSize == m_pImpl->m_minPoolSize))
    return;

// what time is it?
  boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

// release all connections not in use whose time has expired
  std::list<te::ado::Connection*>::iterator it = m_pImpl->m_connections.begin();
  std::list<te::ado::Connection*>::iterator itend = m_pImpl->m_connections.end();

  while(it != itend)
  {
    if((*it)->m_inuse == false)
    {
      boost::posix_time::time_duration d = now - (*it)->m_lastuse;

      if(d.seconds() > static_cast<long>(m_pImpl->m_maxIdleTime))
      {
        std::list<te::ado::Connection*>::iterator itaux = it;
        ++it; // advance current iterator... we will release the connection pointed by the auxiliar iterator
        delete *itaux;
        m_pImpl->m_connections.erase(itaux);
        --(m_pImpl->m_poolSize);

        if(m_pImpl->m_poolSize == m_pImpl->m_minPoolSize) // have we reached the minimum pool size?
          break;

        continue; // we have already go on... (++it)! 
      }
    }

    ++it;
  }
}

bool te::ado::ConnectionPool::isValid() const
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  std::list<te::ado::Connection*>::iterator it = m_pImpl->m_connections.begin();
  std::list<te::ado::Connection*>::iterator itend = m_pImpl->m_connections.end();

  while(it != itend)
  {
    long status = (*it)->m_conn->GetState();

    if(status != adStateOpen)
      return false;

    ++it;
  }

  return true;
}

bool te::ado::ConnectionPool::isInitialized() const
{
  return m_pImpl->m_initialized;
}

te::ado::DataSource* te::ado::ConnectionPool::getDataSource() const
{
  return m_pImpl->m_ds;
}

std::size_t te::ado::ConnectionPool::getPoolSize() const
{
  return m_pImpl->m_poolSize;
}

std::size_t te::ado::ConnectionPool::getInitialPoolSize() const
{
  return m_pImpl->m_initialPoolSize;
}

void te::ado::ConnectionPool::setInitialPoolSize(std::size_t size)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  m_pImpl->m_initialPoolSize = size;
}

std::size_t te::ado::ConnectionPool::getMinPoolSize() const
{
  return m_pImpl->m_minPoolSize;
}

void te::ado::ConnectionPool::setMinPoolSize(std::size_t size)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  m_pImpl->m_minPoolSize = size;
}

std::size_t te::ado::ConnectionPool::getMaxPoolSize() const
{
  return m_pImpl->m_maxPoolSize;
}

void te::ado::ConnectionPool::setMaxPoolSize(std::size_t size)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  m_pImpl->m_maxPoolSize = size;
}

te::ado::Connection* te::ado::ConnectionPool::getConnection()
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

// search for available connections: we are optmistic - just try to open a new connection if none exist!
  std::list<te::ado::Connection*>::iterator it = m_pImpl->m_connections.begin();
  std::list<te::ado::Connection*>::iterator itend = m_pImpl->m_connections.end();

  while(it != itend)
  {
    if((*it)->m_inuse == false)
    {
      (*it)->m_inuse = true;
      return (*it);
    }

    ++it;
  }

// if we couldn't find an opened connection maybe we may create one and return it
  if(m_pImpl->m_poolSize < m_pImpl->m_maxPoolSize)
  {
    Connection* newConn = new Connection(this, m_pImpl->m_conninfo.c_str(), m_pImpl->m_cencoding.c_str(), true);
    m_pImpl->m_connections.push_back(newConn);
    ++(m_pImpl->m_poolSize);

    return newConn;
  }

  throw Exception(TR_ADO("The connection pool has reached its maximum size!"));
}

void te::ado::ConnectionPool::release(Connection* conn)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  conn->m_inuse = false;
  conn->m_lastuse = boost::posix_time::second_clock::local_time();
}

unsigned int te::ado::ConnectionPool::getMaxIdleTime() const
{
  return m_pImpl->m_maxIdleTime;
}

void te::ado::ConnectionPool::setMaxIdleTime(unsigned int t)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  m_pImpl->m_maxIdleTime = t;
}

te::ado::ConnectionPool::ConnectionPool(DataSource* ds)
  : m_pImpl(0)
{
  m_pImpl = new ConnectionPoolImpl(ds);

  te::da::ConnectionPoolManager::getInstance().add(this);
}

te::ado::ConnectionPool::~ConnectionPool()
{
  te::da::ConnectionPoolManager::getInstance().remove(this);

  delete m_pImpl;
}
