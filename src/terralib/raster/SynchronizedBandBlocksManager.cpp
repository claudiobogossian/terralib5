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
  \file terralib/raster/SynchronizedBandBlocksManager.cpp
 
  \brief Synchronized raster raster band blocks manager.
*/

// TerraLib

#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "SynchronizedBandBlocksManager.h"
#include "Exception.h"
#include "../common/PlatformUtils.h"
#include "../common/Translator.h"

// STL
#include <algorithm>
#include <climits>

void te::rst::SynchronizedBandBlocksManager::initState()
{
  m_syncPtr = 0;
  m_globalBlocksNumberX = 0;
  m_globalBlocksNumberY = 0;
  m_globalBlockSizeBytes = 0;
  m_maxNumberOfCacheBlocks = 0;
  m_blocksFifoNextSwapBlockIndex = 0;
  m_getBlockPointer_BlkPtr = 0;
}

te::rst::SynchronizedBandBlocksManager::SynchronizedBandBlocksManager()
{
  initState();
}

te::rst::SynchronizedBandBlocksManager::~SynchronizedBandBlocksManager()
{
  free();
}

bool te::rst::SynchronizedBandBlocksManager::initialize( 
  RasterSynchronizer& sync, const unsigned char maxMemPercentUsed )
{
  free();
  
  // Finding the global block dimensions
  
  unsigned int maxBlockSizeBytes = 0;
  unsigned int maxNumberOfCacheBlocks = 1;
  
  if( !( sync.m_policy & te::common::WAccess ) )
  {
    sync.m_mutex.lock();
    for( unsigned int bandIdx = 0 ; bandIdx < m_syncPtr->m_raster.getNumberOfBands() ;
      ++bandIdx )
    {
      if( maxBlockSizeBytes < (unsigned int)m_syncPtr->m_raster.getBand( bandIdx )->getBlockSize() )
        maxBlockSizeBytes = (unsigned int)m_syncPtr->m_raster.getBand( bandIdx )->getBlockSize();
    }
    sync.m_mutex.unlock();
    
    const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
    const double usedVMem = (double)te::common::GetUsedVirtualMemory();
    const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() );
    const double freeVMem = ( ((double)maxMemPercentUsed) / 100.0 ) *
      std::min( totalPhysMem, ( totalVMem - usedVMem ) );  
    maxNumberOfCacheBlocks = (unsigned int)
      std::max( 1.0, std::ceil( freeVMem / ((double)maxBlockSizeBytes) ) );
  }
    
  return initialize( maxNumberOfCacheBlocks, sync );
}

bool te::rst::SynchronizedBandBlocksManager::initialize( 
  const unsigned int maxNumberOfCacheBlocks, 
  RasterSynchronizer& sync )
{
  free();
  
  sync.m_mutex.lock();
  
  m_syncPtr = &sync;
  
  // Finding the global block dimensions
  
  unsigned int numberOfRasterBlocks = 0;
  
  for( unsigned int bandIdx = 0 ; bandIdx < m_syncPtr->m_raster.getNumberOfBands() ;
    ++bandIdx )
  {
    if( m_globalBlocksNumberX < (unsigned int)m_syncPtr->m_raster.getBand( bandIdx )->getProperty()->m_nblocksx )
      m_globalBlocksNumberX = (unsigned int)m_syncPtr->m_raster.getBand( bandIdx )->getProperty()->m_nblocksx;
      
    if( m_globalBlocksNumberY < (unsigned int)m_syncPtr->m_raster.getBand( bandIdx )->getProperty()->m_nblocksy )
      m_globalBlocksNumberY = (unsigned int)m_syncPtr->m_raster.getBand( bandIdx )->getProperty()->m_nblocksy;
      
    if( m_globalBlockSizeBytes < (unsigned int)m_syncPtr->m_raster.getBand( bandIdx )->getBlockSize() )
      m_globalBlockSizeBytes = (unsigned int)m_syncPtr->m_raster.getBand( bandIdx )->getBlockSize();
    
    numberOfRasterBlocks +=
      ( m_syncPtr->m_raster.getBand( bandIdx )->getProperty()->m_nblocksx *
      m_syncPtr->m_raster.getBand( bandIdx )->getProperty()->m_nblocksy );    
  }
    
  // Allocating the internal structures
  
  if( sync.m_policy & te::common::WAccess )
  {
    m_maxNumberOfCacheBlocks = std::min( numberOfRasterBlocks, 1u );
  }
  else
  {
    m_maxNumberOfCacheBlocks = std::min( maxNumberOfCacheBlocks, numberOfRasterBlocks );
    m_maxNumberOfCacheBlocks = std::max( m_maxNumberOfCacheBlocks, 1u );
  }
  
  unsigned int blockBIdx = 0;  
  m_blocksPointers.resize( m_syncPtr->m_raster.getNumberOfBands() );
  
  for( blockBIdx = 0 ; blockBIdx < m_blocksPointers.size() ;  ++blockBIdx )
  {
    m_blocksPointers[ blockBIdx ].resize( m_globalBlocksNumberY );
    
    for( unsigned int blockYIdx = 0 ; blockYIdx < m_globalBlocksNumberY ;
      ++blockYIdx )
    {
      m_blocksPointers[ blockBIdx ][ blockYIdx ].resize( m_globalBlocksNumberX, 0 );
    }
  }
  
  sync.m_mutex.unlock();
  
  return true;
}

void te::rst::SynchronizedBandBlocksManager::free()
{
  // flushing the ram data
  
  if( ( m_syncPtr != 0 ) && ( m_syncPtr->m_policy & te::common::WAccess ) )
  {
    unsigned int blockBIdx = 0;
    unsigned int blockYIdx = 0;
    unsigned int blockXIdx = 0;
    void* blockPtr = 0;
    
    for( blockBIdx = 0 ; blockBIdx < m_blocksPointers.size() ;  ++blockBIdx )
    {
      for( blockYIdx = 0 ; blockYIdx < m_globalBlocksNumberY ;
        ++blockYIdx )
      {
        for( blockXIdx = 0 ; blockXIdx < m_globalBlocksNumberX ;
          ++blockXIdx )
        {
          blockPtr = m_blocksPointers[ blockBIdx ][ blockYIdx ][ blockXIdx ];
          
          if( blockPtr )
          {
            if( !m_syncPtr->releaseBlock( blockBIdx, blockXIdx, blockYIdx, blockPtr ) )
            {
              throw Exception(TE_TR("Block release error") );
            }
          }
        }
      }
    }
  }
  
  m_blocksPointers.clear();
  
  unsigned char* blockPtr = 0;
  for( std::vector< unsigned char* >::size_type blocksHandlerIdx = 0 ; 
    blocksHandlerIdx < m_blocksHandler.size() ; ++blocksHandlerIdx )
  {
    blockPtr = m_blocksHandler[ blocksHandlerIdx ];
    if( blockPtr )
    {
      delete[]( blockPtr );
    }
  }
  m_blocksHandler.clear();
  
  m_blocksFifo.clear();
  
  initState();
}

void* te::rst::SynchronizedBandBlocksManager::getBlockPointer(unsigned int band, 
  unsigned int x, unsigned int y )
{
  assert( m_syncPtr );
  assert( band < m_syncPtr->m_raster.getNumberOfBands() );
  assert( x < m_globalBlocksNumberX );
  assert( y < m_globalBlocksNumberY );
  assert( x < (unsigned int)m_syncPtr->m_raster.getBand( band )->getProperty()->m_nblocksx );
  assert( y < (unsigned int)m_syncPtr->m_raster.getBand( band )->getProperty()->m_nblocksy );
  
  m_getBlockPointer_BlkPtr = m_blocksPointers[ band ][ y ][ x ];
  
  if( m_getBlockPointer_BlkPtr == 0 )
  {
    // Is swapp necessary ?
    if( m_blocksHandler.size() < m_maxNumberOfCacheBlocks )
    {
      m_getBlockPointer_BlkPtr = new unsigned char[ m_globalBlockSizeBytes ];
      m_blocksHandler.push_back( m_getBlockPointer_BlkPtr );
      
      // add FIFO information of the new block
      BlockIndex newBlockFifoIndex;
      newBlockFifoIndex.m_b = band;
      newBlockFifoIndex.m_y = y;
      newBlockFifoIndex.m_x = x; 
      m_blocksFifo.push_back( newBlockFifoIndex );
    }
    else
    {
      BlockIndex& choosedSwapBlockIndex = m_blocksFifo[ 
        m_blocksFifoNextSwapBlockIndex ];   
      
      m_getBlockPointer_BlkPtr = m_blocksPointers[ choosedSwapBlockIndex.m_b ][ 
        choosedSwapBlockIndex.m_y ][ choosedSwapBlockIndex.m_x ];
      assert( m_getBlockPointer_BlkPtr );
      m_blocksPointers[ choosedSwapBlockIndex.m_b ][ 
        choosedSwapBlockIndex.m_y ][ choosedSwapBlockIndex.m_x ] = 0;
        
      // writing the block choosed for swap, if necessary

      if( !m_syncPtr->releaseBlock( choosedSwapBlockIndex.m_b, 
        choosedSwapBlockIndex.m_x, choosedSwapBlockIndex.m_y, m_getBlockPointer_BlkPtr ) )
      {
        throw Exception(TE_TR("Block release error") );
      }        
      
      // advances the next swap block fifo index
      choosedSwapBlockIndex.m_b = band;
      choosedSwapBlockIndex.m_y = y;
      choosedSwapBlockIndex.m_x = x;    
      m_blocksFifoNextSwapBlockIndex = ( m_blocksFifoNextSwapBlockIndex + 1 ) % 
        ((unsigned int)m_blocksFifo.size());        
    }
    
    // reading the required block
    
    if( !m_syncPtr->acquireBlock( band, x, y, m_getBlockPointer_BlkPtr ) )
    {
      throw Exception(TE_TR("Block release error") );
    }      

    m_blocksPointers[ band ][ y ][ x ] = m_getBlockPointer_BlkPtr;
      
  }
  
  return m_getBlockPointer_BlkPtr;  
}

te::rst::Raster* te::rst::SynchronizedBandBlocksManager::getRaster() const
{
  if( m_syncPtr )
  {
    return &(m_syncPtr->m_raster);
  }
  else
  {
    return 0;
  }
};
       

