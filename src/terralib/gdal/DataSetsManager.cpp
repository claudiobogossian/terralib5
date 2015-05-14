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
  \file terralib/gdal/DataSetsManager.cpp

  \brief GDAL data set manager.
*/

// TerraLib
#include "DataSetsManager.h"

te::gdal::DataSetsManager::DataSetsManager()
{
}

te::gdal::DataSetsManager::~DataSetsManager()
{}

bool te::gdal::DataSetsManager::incrementUseCounter( const std::string& uri,
  const AccessType aType  )
{
  if( uri.empty() )
  {
    return false;
  }
  else
  {
    m_mutex.lock();
    
    UrisInfoT::iterator it = m_openURIS.find( uri );
    
    if( it == m_openURIS.end() )
    {
      m_openURIS[ uri ] = std::pair< AccessType, unsigned long int >( aType, 1 ) ;
      m_mutex.unlock(); 
      return true;
    }
    else
    {
      if( aType == SingleAccessType )
      {
        m_mutex.unlock();
        return false;
      }
      else
      {
        if( it->second.first == SingleAccessType )
        {
          m_mutex.unlock(); 
          return false;
        }
        else
        {
          ++( it->second.second );
          m_mutex.unlock(); 
          return true;
        }
      }
    }
  }
}
        
void te::gdal::DataSetsManager::decrementUseCounter( const std::string& uri )
{
  if( ! uri.empty() )
  {
    m_mutex.lock();
    
    UrisInfoT::iterator it = m_openURIS.find( uri );
    
    if( it != m_openURIS.end() )
    {
      if(  it->second.second == 1 )
      {
        m_openURIS.erase( uri );
      }
      else
      {
        --( it->second.second );
      }
    }
    
    m_mutex.unlock();
  }
}

