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
  \file terralib/dataaccess/datasource/ConnectionPoolManager.cpp

  \brief A singleton for managing the connection pools available in the system.
*/

// TerraLib
#include "AbstractConnectionPool.h"
#include "ConnectionPoolManager.h"

void te::da::ConnectionPoolManager::add(AbstractConnectionPool* pool)
{
  boost::lock_guard<boost::mutex> lock(m_mtx);

  if(pool)
    m_pools.push_back(pool);
}

void te::da::ConnectionPoolManager::remove(AbstractConnectionPool* pool)
{
  boost::lock_guard<boost::mutex> lock(m_mtx);

  std::vector<AbstractConnectionPool*>::iterator it = std::find(m_pools.begin(), m_pools.end(), pool);

  if(it != m_pools.end())
    m_pools.erase(it);
}

void te::da::ConnectionPoolManager::start()
{
  boost::lock_guard<boost::mutex> lock(m_mtx);

// is manager already initialized? 
  if(m_initialized) 
    return;

// start all the pools
  const std::size_t size = m_pools.size();

  for(std::size_t i = 0; i < size; ++i)
    m_pools[i]->initialize();

  m_initialized = true;

// create a monitoring thread
  // calling a static method
  //m_thread = boost::thread(te::da::ConnectionPoolManager::monitore);
  m_thread = boost::thread(&te::da::ConnectionPoolManager::monitore, this);
}

void te::da::ConnectionPoolManager::stop()
{
// ok, let's signal the monitoring thread that it must interrupt and then le't wait for it to finish its job
  m_thread.interrupt();
  m_thread.join();

// let's hold the mutex!
  boost::lock_guard<boost::mutex> lock(m_mtx);

// finalizes all the pools
  const std::size_t size = m_pools.size();

  for(std::size_t i = 0; i < size; ++i)
    m_pools[i]->finalize();

  m_initialized = false;
}

bool te::da::ConnectionPoolManager::isInitialized() const
{
  return m_initialized;
}

te::da::ConnectionPoolManager::ConnectionPoolManager()
  : m_initialized(false)
{
}

te::da::ConnectionPoolManager::~ConnectionPoolManager()
{
  //m_thread.interrupt();
  //m_thread.join();
}

void te::da::ConnectionPoolManager::monitore()
{
  try
  {
    while(true)  
    {
// keep a nested scope to hold the mutex just for a while        
      {
        boost::lock_guard<boost::mutex> lock(getInstance().m_mtx);

        std::vector<AbstractConnectionPool*>& pools = getInstance().m_pools;

        const std::size_t size = pools.size();

        for(std::size_t i = 0; i < size; ++i)
          pools[i]->idle();
      }

      boost::posix_time::seconds sleepTime(TERRALIB_POOL_DEFAULT_MONITORING_TIME);
      boost::this_thread::sleep(sleepTime);
    }
  }
  catch(boost::thread_interrupted&)
  {
    return; // let's finish!
  }
  catch(...)
  {
    return; // let's finish!
  }
}

