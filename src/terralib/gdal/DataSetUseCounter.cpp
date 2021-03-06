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
  \file terralib/gdal/DataSetUseCounter.cpp

  \brief GDAL data set use counter.
*/

// TerraLib
#include "DataSetUseCounter.h"
#include "Exception.h"
#include "../core/translator/Translator.h"

te::gdal::DataSetUseCounter::DataSetUseCounter( const std::string& uri,
  const DataSetsManager::AccessType aType )
  throw( te::gdal::Exception )
  : m_aType( aType ), m_uri( uri )
{
  if( !DataSetsManager::getInstance().incrementUseCounter( m_uri, aType ) )
  {
    throw Exception(TE_TR("Maximum number of concurrent dataset instances reached"), te::common::NO_CONNECTION_AVAILABLE);
  }  
}

te::gdal::DataSetUseCounter::~DataSetUseCounter()
{
  DataSetsManager::getInstance().decrementUseCounter( m_uri );
}

bool te::gdal::DataSetUseCounter::changeAccessType( const DataSetsManager::AccessType aType)
  throw( te::gdal::Exception )
{
  if( m_aType == aType )
  {
    return true;
  }
  else
  {
    DataSetsManager::getInstance().decrementUseCounter( m_uri );
    
    if( DataSetsManager::getInstance().incrementUseCounter( m_uri, aType ) )
    {
      m_aType = aType;
      return true;
    }    
    else
    {
      if( !DataSetsManager::getInstance().incrementUseCounter( m_uri, m_aType ) )
      {
        throw Exception(TE_TR("Maximum number of concurrent dataset instances reached"), te::common::NO_CONNECTION_AVAILABLE);
      }  
      
      return false;
    }
  }
}

