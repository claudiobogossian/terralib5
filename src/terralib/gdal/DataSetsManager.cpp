/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/gdal/DataSetsManager.cpp

  \brief GDAL data set manager.
*/

// TerraLib
#include "DataSetsManager.h"

te::gdal::DataSetsManager::DataSetsManager()
{
  m_maxUseCounter = 1;  
}

te::gdal::DataSetsManager::~DataSetsManager()
{}

bool te::gdal::DataSetsManager::incrementUseCounter( const std::string& uri )
{
  if( uri.empty() )
  {
    return false;
  }
  else
  {
    m_mutex.lock();
    
    unsigned int currentCount = 0;
    
    if( m_openURIS.find( uri ) != m_openURIS.end() )
    {
      currentCount = m_openURIS[ uri ];
    }
    
    if( currentCount < m_maxUseCounter )
    {
      ++( m_openURIS[ uri ] );
      m_mutex.unlock(); 
      return true;
    }
    else
    {
      m_mutex.unlock(); 
      return false;
    }
  }
}
        
void te::gdal::DataSetsManager::decrementUseCounter( const std::string& uri )
{
  if( ! uri.empty() )
  {
    m_mutex.lock();
    
    if( m_openURIS.find( uri ) != m_openURIS.end() )
    {
      if(  m_openURIS[ uri ] == 1 )
      {
        m_openURIS.erase( uri );
      }
      else
      {
        --( m_openURIS[ uri ] );
      }
    }
    
    m_mutex.unlock();
  }
}
        
unsigned int te::gdal::DataSetsManager::getUseCounter( const std::string& uri ) const
{
  if( uri.empty() )
  {
    return 0;
  }
  else
  {
    m_mutex.lock();
    
    if( m_openURIS.find( uri ) == m_openURIS.end() )
    {
      m_mutex.unlock();
      return 0;
    }
    else
    {
      unsigned int returnValue = m_openURIS[ uri ];
      m_mutex.unlock();
      return returnValue;
    }
  }
}

void te::gdal::DataSetsManager::setMaxUseCounter( const unsigned int counter )
{
  m_mutex.lock();
  m_maxUseCounter = counter;
  m_mutex.unlock();
}

bool te::gdal::DataSetsManager::hasReachedMaxUseCounter( const std::string& uri ) const
{
  m_mutex.lock();
  
  unsigned int currentCount = 0;
  
  if( ( !uri.empty() ) && ( m_openURIS.find( uri ) != m_openURIS.end() ) )
  {
    currentCount = m_openURIS[ uri ];
  }
  
  if( currentCount < m_maxUseCounter )
  {
    m_mutex.unlock(); 
    return false;
  }
  else
  {
    m_mutex.unlock(); 
    return true;
  }  
}
