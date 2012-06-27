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
  \file ConnectionPool.cpp

  \brief A class that implements a connection pool for MySQL driver.  
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/ConnectionPoolManager.h"
#include "Connection.h"
#include "ConnectionPool.h"
#include "DataSource.h"
#include "Exception.h"

// STL
#include <list>

// BOOST
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

// MySQL C++ connector
#include <driver/mysql_public_iface.h>

class te::mysql::ConnectionPool::ConnectionPoolImpl
{
  public:

    ConnectionPoolImpl(te::mysql::DataSource* ds)
      : m_ds(ds),
        m_initialPoolSize(MYSQL_DEFAULT_INITIAL_POOL_SIZE),
        m_minPoolSize(MYSQL_DEFAULT_MIN_POOL_SIZE),
        m_maxPoolSize(MYSQL_DEFAULT_MAX_POOL_SIZE),
        m_poolSize(0),
        m_maxIdleTime(MYSQL_DEFAULT_MAX_IDLE_TIME),
        m_initialized(false)
    {
    }

    ~ConnectionPoolImpl()
    {
      te::common::FreeContents(m_connections);
    }

    te::mysql::DataSource* m_ds;                        //!< The data source using the pool.
    std::size_t m_initialPoolSize;                     //!< It indicates the initial number of connections opened by the pool.
    std::size_t m_minPoolSize;                         //!< It indicates the minimum number of connections in the pool.
    std::size_t m_maxPoolSize;                         //!< It indicates the maximum number of connections in the pool.
    std::size_t m_poolSize;                            //!< It indicates the maximum number of connections in the pool.
    unsigned int m_maxIdleTime;                        //!< The number of seconds waiting for releasing connections.
    std::list<te::mysql::Connection*> m_connections;    //!< The list of available connections ready to be used.
    boost::mutex m_mtx;                                //!< A mutex for preventing reading/writing in the connection list.
    bool m_initialized;                                //!< A flag that indicates if the pool was initialized or not.
};

void te::mysql::ConnectionPool::initialize()
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  if(isInitialized())
    throw Exception(TR_MYSQL("The connection pool is already iniatialized!"));

// check for pool parameters...
  const std::map<std::string, std::string>& connInfo = m_pImpl->m_ds->getConnectionInfo();

  std::map<std::string, std::string>::const_iterator it_end = connInfo.end();

  {
    std::map<std::string, std::string>::const_iterator it = connInfo.find("MY_INITIAL_POOL_SIZE");
    m_pImpl->m_initialPoolSize = (it != connInfo.end() && !it->second.empty()) ? atoi(it->second.c_str()) : MYSQL_DEFAULT_INITIAL_POOL_SIZE;
  }

  {
    std::map<std::string, std::string>::const_iterator it = connInfo.find("MY_MIN_POOL_SIZE");
    m_pImpl->m_minPoolSize = (it != connInfo.end() && !it->second.empty()) ? atoi(it->second.c_str()) : MYSQL_DEFAULT_MIN_POOL_SIZE;
  }

  {
    std::map<std::string, std::string>::const_iterator it = connInfo.find("MY_MAX_POOL_SIZE");
    m_pImpl->m_maxPoolSize = (it != connInfo.end() && !it->second.empty()) ? atoi(it->second.c_str()) : MYSQL_DEFAULT_MAX_POOL_SIZE;
  }

// check pool size
  if(m_pImpl->m_minPoolSize > m_pImpl->m_maxPoolSize)
    m_pImpl->m_minPoolSize = m_pImpl->m_maxPoolSize;

  if(m_pImpl->m_initialPoolSize > m_pImpl->m_maxPoolSize)
    m_pImpl->m_initialPoolSize = m_pImpl->m_maxPoolSize;
  else if(m_pImpl->m_initialPoolSize < m_pImpl->m_minPoolSize)
    m_pImpl->m_initialPoolSize = m_pImpl->m_minPoolSize;

  {
    std::map<std::string, std::string>::const_iterator it = connInfo.find("MAX_IDLE_TIME");
    m_pImpl->m_maxIdleTime = (it != connInfo.end() && !it->second.empty()) ? atoi(it->second.c_str()) : MYSQL_DEFAULT_MAX_IDLE_TIME;
  }

// try to open the connections
  for(std::size_t i = 0; i < m_pImpl->m_initialPoolSize; ++i)
  {
    Connection* conn = new Connection(this, connInfo, false);
    m_pImpl->m_connections.push_back(conn);
    ++(m_pImpl->m_poolSize);
  }

  m_pImpl->m_initialized = true;
}

void te::mysql::ConnectionPool::finalize()
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

// if there is a connection in use it will raise an exception!
  std::list<te::mysql::Connection*>::iterator it = m_pImpl->m_connections.begin();
  std::list<te::mysql::Connection*>::iterator itend = m_pImpl->m_connections.end();

  while(it != itend)
  {
    if((*it)->m_inuse)
      throw Exception(TR_MYSQL("There are opened connections. Please, close all connections before finalizing the MySQL connection pool."));

    ++it;
  }

// release all connections
  te::common::FreeContents(m_pImpl->m_connections);

  m_pImpl->m_connections.clear();
  m_pImpl->m_poolSize = 0;

  m_pImpl->m_initialized = false;
}

void te::mysql::ConnectionPool::idle()
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
  std::list<te::mysql::Connection*>::iterator it = m_pImpl->m_connections.begin();
  std::list<te::mysql::Connection*>::iterator itend = m_pImpl->m_connections.end();

  while(it != itend)
  {
    if((*it)->m_inuse == false)
    {
      boost::posix_time::time_duration d = now - (*it)->m_lastuse;

      if(d.seconds() > static_cast<long>(m_pImpl->m_maxIdleTime))
      {
        std::list<te::mysql::Connection*>::iterator itaux = it;
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

bool te::mysql::ConnectionPool::isValid() const
{
  return true;
}

bool te::mysql::ConnectionPool::isInitialized() const
{
  return m_pImpl->m_initialized;
}

std::size_t te::mysql::ConnectionPool::getPoolSize() const
{
  return m_pImpl->m_poolSize;
}

std::size_t te::mysql::ConnectionPool::getInitialPoolSize() const
{
  return m_pImpl->m_initialPoolSize;
}

void te::mysql::ConnectionPool::setInitialPoolSize(std::size_t size)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  m_pImpl->m_initialPoolSize = size;
}

std::size_t te::mysql::ConnectionPool::getMinPoolSize() const
{
  return m_pImpl->m_minPoolSize;
}

void te::mysql::ConnectionPool::setMinPoolSize(std::size_t size)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  m_pImpl->m_minPoolSize = size;
}

std::size_t te::mysql::ConnectionPool::getMaxPoolSize() const
{
  return m_pImpl->m_maxPoolSize;
}

void te::mysql::ConnectionPool::setMaxPoolSize(std::size_t size)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  m_pImpl->m_maxPoolSize = size;
}

te::mysql::Connection* te::mysql::ConnectionPool::getConnection()
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

// search for available connections: we are optmistic - just try to open a new connection if none exist!
  std::list<te::mysql::Connection*>::iterator it = m_pImpl->m_connections.begin();
  std::list<te::mysql::Connection*>::iterator itend = m_pImpl->m_connections.end();

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
    Connection* newConn =  new Connection(this, m_pImpl->m_ds->getConnectionInfo(), true);
    m_pImpl->m_connections.push_back(newConn);
    ++(m_pImpl->m_poolSize);

    return newConn;
  }

  throw Exception(TR_MYSQL("The MySQL connection pool has reached its maximum size!"));
}

void te::mysql::ConnectionPool::release(Connection* conn)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  conn->m_inuse = false;
  conn->m_lastuse = boost::posix_time::second_clock::local_time();
}

unsigned int te::mysql::ConnectionPool::getMaxIdleTime() const
{
  return m_pImpl->m_maxIdleTime;
}

void te::mysql::ConnectionPool::setMaxIdleTime(unsigned int t)
{
// acquire lock
  boost::lock_guard<boost::mutex> lock(m_pImpl->m_mtx);

  m_pImpl->m_maxIdleTime = t;
}

te::mysql::ConnectionPool::ConnectionPool(DataSource* ds)
  : m_pImpl(0)
{
  m_pImpl = new ConnectionPoolImpl(ds);

  te::da::ConnectionPoolManager::getInstance().add(this);
}

te::mysql::ConnectionPool::~ConnectionPool()
{
  te::da::ConnectionPoolManager::getInstance().remove(this);

  delete m_pImpl;
}


