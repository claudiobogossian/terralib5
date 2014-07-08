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
  \file terralib/raster/RasterSynchronizer.cpp

  \brief An access synchronizer to be used in RasterSynchronizer raster instances
*/

// TerraLib
#include "RasterSynchronizer.h"
#include "Band.h"
#include "BandProperty.h"
#include "Exception.h"
#include "../common/Translator.h"

#include <climits>

te::rst::RasterSynchronizer::RasterSynchronizer( Raster& raster,
  const te::common::AccessPolicy policy )
: m_raster( raster )
{
  if( ( raster.getAccessPolicy() & te::common::WAccess ) &&
    ( policy & te::common::WAccess ) )
  {
    m_policy = raster.getAccessPolicy();
  }
  else
  {
    if( raster.getAccessPolicy() & te::common::RAccess ) 
    {
      m_policy = te::common::RAccess;
    }
    else
    {
      m_policy = te::common::NoAccess;
    }
  }
  
  m_blocksUseCounters.resize( raster.getNumberOfBands() );
  
  for( unsigned int bandIdx = 0; bandIdx < m_blocksUseCounters.size() ;  ++bandIdx )
  {
    m_blocksUseCounters[ bandIdx ].resize( raster.getBand( bandIdx )->getProperty()->m_nblocksy );
    
    for( unsigned int blockYIdx = 0 ; blockYIdx < m_blocksUseCounters[ bandIdx ].size() ;
      ++blockYIdx )
    {
      m_blocksUseCounters[ bandIdx ][ blockYIdx ].resize( raster.getBand( bandIdx )->getProperty()->m_nblocksx, 0 );
    }
  }    
}

te::rst::RasterSynchronizer::~RasterSynchronizer()
{
}
        
bool te::rst::RasterSynchronizer::acquireBlock( const unsigned int bandIdx,
  const unsigned int blockXIndex, const unsigned int blockYIndex,
  void* blkDataPtr )
{
  m_mutex.lock();
  
  if( bandIdx >= m_blocksUseCounters.size() )
  {
    m_mutex.unlock();
    throw Exception(TE_TR("Inalid band index") );
  }
  if( blockYIndex >= m_blocksUseCounters[ bandIdx ].size() )
  {
    m_mutex.unlock();
    throw Exception(TE_TR("Inalid block Y index") );
  }
  if( blockXIndex >= m_blocksUseCounters[ bandIdx ][ blockYIndex ].size() )
  {
    m_mutex.unlock();
    throw Exception(TE_TR("Inalid block X index") );
  }  
  
  if( m_policy & te::common::WAccess )
  {
    // Wait the block to be avaliable 
        
    while( m_blocksUseCounters[ bandIdx ][ blockYIndex ][ blockXIndex ] )
    {
      m_condVar.wait( m_mutex );
    }
    
    assert( m_blocksUseCounters[ bandIdx ][ blockYIndex ][ blockXIndex ] == 0 );
    
    m_blocksUseCounters[ bandIdx ][ blockYIndex ][ blockXIndex ] = 1;
    
    m_raster.getBand( bandIdx )->read( blockXIndex, blockYIndex, blkDataPtr );
    
    m_mutex.unlock();  
    
    return true;
  }
  else
  {
    ++( m_blocksUseCounters[ bandIdx ][ blockYIndex ][ blockXIndex ] );
    
    m_raster.getBand( bandIdx )->read( blockXIndex, blockYIndex, blkDataPtr );
    
    m_mutex.unlock();    
    
    return true;
  }
}

bool te::rst::RasterSynchronizer::releaseBlock( const unsigned int bandIdx,
  const unsigned int blockXIndex, const unsigned int blockYIndex,
  void* blkDataPtr )
{
  m_mutex.lock();
  
  if( bandIdx >= m_blocksUseCounters.size() )
  {
    m_mutex.unlock();
    throw Exception(TE_TR("Inalid band index") );
  }
  if( blockYIndex >= m_blocksUseCounters[ bandIdx ].size() )
  {
    m_mutex.unlock();
    throw Exception(TE_TR("Inalid block Y index") );
  }
  if( blockXIndex >= m_blocksUseCounters[ bandIdx ][ blockYIndex ].size() )
  {
    m_mutex.unlock();
    throw Exception(TE_TR("Inalid block X index") );
  }  
  
  if( m_blocksUseCounters[ bandIdx ][ blockYIndex ][ blockXIndex ] )
  {
    if( ( m_policy & te::common::WAccess ) &&
      ( m_raster.getAccessPolicy() & te::common::WAccess ) )
    {
      m_raster.getBand( bandIdx )->write( blockXIndex, blockYIndex, blkDataPtr );
    }
    
    --( m_blocksUseCounters[ bandIdx ][ blockYIndex ][ blockXIndex ] );
  }
  
  m_mutex.unlock();  
  
  m_condVar.notify_all();
  
  return true;
}

