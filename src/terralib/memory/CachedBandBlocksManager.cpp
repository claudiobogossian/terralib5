/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/memory/CachedBandBlocksManager.cpp
 
  \brief RAM cached and tiled raster band blocks manager.
*/

#include "CachedBandBlocksManager.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../common/PlatformUtils.h"

#include <algorithm>

void te::mem::CachedBandBlocksManager::initState()
{
  m_rasterPtr = 0;
  m_maxMemPercentUsed = 0;
  m_dataPrefetchThreshold = 0;
  m_globalBlocksNumberX = 0;
  m_globalBlocksNumberY = 0;
  m_globalBlockSizeBytes = 0;
  m_maxNumberOfCacheBlocks = 0;
  m_blocksFifoNextSwapBlockIndex = 0;
}

te::mem::CachedBandBlocksManager::CachedBandBlocksManager()
{
  initState();
}

te::mem::CachedBandBlocksManager::CachedBandBlocksManager(
  const CachedBandBlocksManager& )
{
  initState();
}

te::mem::CachedBandBlocksManager::~CachedBandBlocksManager()
{
  free();
}

const  te::mem::CachedBandBlocksManager& 
  te::mem::CachedBandBlocksManager::operator=( 
  const CachedBandBlocksManager& rhs )
{
  return rhs;
}

bool te::mem::CachedBandBlocksManager::initialize( 
  const te::rst::Raster& externalRaster, const unsigned char maxMemPercentUsed, 
  const unsigned int dataPrefetchThreshold)
{
  free();
  
  m_rasterPtr = (te::rst::Raster*)&externalRaster;
  m_maxMemPercentUsed = maxMemPercentUsed;
  m_dataPrefetchThreshold = dataPrefetchThreshold;
  
  // Finding the global block dimensions
  
  unsigned int numberOfRasterBlocks = 0;
  
  for( unsigned int bandIdx = 0 ; bandIdx < externalRaster.getNumberOfBands() ;
    ++bandIdx )
  {
    if( m_globalBlocksNumberX < externalRaster.getBand( bandIdx )->getProperty()->m_nblocksx )
      m_globalBlocksNumberX = externalRaster.getBand( bandIdx )->getProperty()->m_nblocksx;
      
    if( m_globalBlocksNumberY < externalRaster.getBand( bandIdx )->getProperty()->m_nblocksy )
      m_globalBlocksNumberY = externalRaster.getBand( bandIdx )->getProperty()->m_nblocksy;
      
    if( m_globalBlockSizeBytes < externalRaster.getBand( bandIdx )->getBlockSize() )
      m_globalBlockSizeBytes = externalRaster.getBand( bandIdx )->getBlockSize();
    
    numberOfRasterBlocks +=
      ( externalRaster.getBand( bandIdx )->getProperty()->m_nblocksx *
      externalRaster.getBand( bandIdx )->getProperty()->m_nblocksy );
  }
  
  // Finding the max number of memory blocks
  
  const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
  const double usedVMem = (double)te::common::GetUsedVirtualMemory();
  const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() ) / 
    2.0;
  const double freeVMem = ( ((double)maxMemPercentUsed) / 100.0 ) *
    std::min( totalPhysMem, ( ( totalVMem <= usedVMem ) ? 0.0 : ( totalVMem - usedVMem ) ) );  
  m_maxNumberOfCacheBlocks = std::min( numberOfRasterBlocks,
    (unsigned int)std::max( 1.0, std::ceil( freeVMem /
    ((double)m_globalBlockSizeBytes) ) ) );
    
  // Allocating the internal structures
  
  unsigned int blockBIdx = 0;
  unsigned int blockYIdx = 0;
  
  m_blocksPointers.resize( externalRaster.getNumberOfBands() );
  
  for( blockBIdx = 0 ; blockBIdx < m_blocksPointers.size() ;  ++blockBIdx )
  {
    m_blocksPointers[ blockBIdx ].resize( m_globalBlocksNumberY );
    
    for( unsigned int blockYIdx = 0 ; blockYIdx < m_globalBlocksNumberY ;
      ++blockYIdx )
    {
      m_blocksPointers[ blockBIdx ][ blockYIdx ].resize( m_globalBlocksNumberX, 0 );
    }
  }
  
  m_blocksFifo.resize( m_maxNumberOfCacheBlocks );
  
  return true;
}

void te::mem::CachedBandBlocksManager::free()
{
  // flushing the ram data, if necessary
  
  if( ( m_rasterPtr != 0 ) &&
    ( m_rasterPtr->getAccessPolicy() & te::common::WAccess ) )
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
            m_rasterPtr->getBand( blockBIdx )->write( blockXIdx, blockYIdx,
              blockPtr );
          }
        }
      }
    }
  }
  
  m_blocksPointers.clear();
  m_blocksHandler.clear();
  m_blocksFifo.clear();
  
  initState();
}

void* te::mem::CachedBandBlocksManager::getBlockPointer(unsigned int band, 
  unsigned int x, unsigned int y )
{
  assert( m_rasterPtr );
  assert( band < m_rasterPtr->getNumberOfBands() );
  assert( x < m_globalBlocksNumberX );
  assert( y < m_globalBlocksNumberY );
  
  m_getBlockPointer_BlkPtr = m_blocksPointers[ band ][ y ][ x ];
  
  if( m_getBlockPointer_BlkPtr == 0 )
  {
    if( m_dataPrefetchThreshold )
    {
      
    }
    else
    {
      BlockIndex& choosedSwapBlockIndex = m_blocksFifo[ 
        m_blocksFifoNextSwapBlockIndex ];      
        
      if( m_blocksHandler.size() < m_maxNumberOfCacheBlocks )
      {
        m_getBlockPointer_BlkPtr = new unsigned char[ m_globalBlockSizeBytes ];
        m_blocksHandler.push_back( boost::shared_array< unsigned char >( 
          (unsigned char*)m_getBlockPointer_BlkPtr ) );
      }
      else
      {
        m_getBlockPointer_BlkPtr = m_blocksPointers[ choosedSwapBlockIndex.m_b ][ 
          choosedSwapBlockIndex.m_y ][ choosedSwapBlockIndex.m_x ];
        assert( m_getBlockPointer_BlkPtr );
        
        m_blocksPointers[ choosedSwapBlockIndex.m_b ][ 
          choosedSwapBlockIndex.m_y ][ choosedSwapBlockIndex.m_x ] = 0;
          
        // writing the block choosed for swap, if necessary
        if( m_rasterPtr->getAccessPolicy() & te::common::WAccess )
        {
          m_rasterPtr->getBand( choosedSwapBlockIndex.m_b )->write( 
            choosedSwapBlockIndex.m_x, choosedSwapBlockIndex.m_y,
            m_getBlockPointer_BlkPtr );
        }
      }
      
      m_blocksPointers[ band ][ y ][ x ] = m_getBlockPointer_BlkPtr;
      
      // reading the required block
      m_rasterPtr->getBand( band )->read( x, y, m_getBlockPointer_BlkPtr );
        
      choosedSwapBlockIndex.m_b = band;
      choosedSwapBlockIndex.m_y = y;
      choosedSwapBlockIndex.m_x = x;
      
      // advances the next swap block fifo index
      m_blocksFifoNextSwapBlockIndex = ( m_blocksFifoNextSwapBlockIndex + 1 ) % 
        m_maxNumberOfCacheBlocks;        
    }
  }
  
  return m_getBlockPointer_BlkPtr;  
}


