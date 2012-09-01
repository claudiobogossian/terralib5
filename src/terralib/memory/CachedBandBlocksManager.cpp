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
  const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
  const double usedVMem = (double)te::common::GetUsedVirtualMemory();
  const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() ) / 
    2.0;
  const double freeVMem = ( ((double)maxMemPercentUsed) / 100.0 ) *
    std::min( totalPhysMem, ( ( totalVMem <= usedVMem ) ? 0.0 : ( totalVMem - usedVMem ) ) );  
  const unsigned int maxNumberOfCacheBlocks = (unsigned int)
    std::max( 1.0, std::ceil( freeVMem / ((double)m_globalBlockSizeBytes) ) );
    
  return initialize( maxNumberOfCacheBlocks, externalRaster, dataPrefetchThreshold );
}

bool te::mem::CachedBandBlocksManager::initialize( 
  const unsigned int maxNumberOfCacheBlocks, 
  const te::rst::Raster& externalRaster, 
  const unsigned int dataPrefetchThreshold)
{
  free();
  
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
    
  // Allocating the internal structures
  
  m_rasterPtr = (te::rst::Raster*)&externalRaster;
  m_dataPrefetchThreshold = dataPrefetchThreshold;
  m_maxNumberOfCacheBlocks = std::min( maxNumberOfCacheBlocks, numberOfRasterBlocks );
  
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
  
  if( m_dataPrefetchThreshold )
  {
    m_threadParams.m_rasterPtr = (te::rst::Raster*)&externalRaster;
    m_threadParams.m_dataPrefetchThreshold = dataPrefetchThreshold;
    m_threadParams.m_keepRunning = true;
    m_threadParams.m_taskFinished = false;
    m_threadParams.m_task = ThreadParameters::InvalidTaskT;
    m_threadParams.m_blockPtr= 0;
    m_threadParams.m_exchangeBlockPtr= 0;
    m_threadParams.m_blockB = 0;
    m_threadParams.m_blockX = 0;
    m_threadParams.m_blockY = 0;
    m_threadParams.m_threadDataBlockHandler.reset( new unsigned char[
      m_globalBlockSizeBytes ] );
    
    m_threadHandler.reset( new boost::thread( threadEntry, &m_threadParams ) );
  }
  
  return true;
}

void te::mem::CachedBandBlocksManager::free()
{
  // Stopping the thread
  
  if( m_dataPrefetchThreshold )
  {
    m_threadParams.m_keepRunning = false;
    m_threadParams.m_doTaskCondVar.notify_one();
    m_threadHandler->join();
    m_threadHandler.reset();
  }  
  
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
  m_threadHandler.reset();
  m_threadParams.m_threadDataBlockHandler.reset();
  
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
    BlockIndex& choosedSwapBlockIndex = m_blocksFifo[ 
      m_blocksFifoNextSwapBlockIndex ];      
      
    // Is swapp necessary ?
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
        if( m_dataPrefetchThreshold )
        {
          // request a block writing to the thread
          {
            boost::lock_guard<boost::mutex> lock(m_threadParams.m_doTaskMutex);
            
            m_threadParams.m_blockB = choosedSwapBlockIndex.m_b;
            m_threadParams.m_blockX = choosedSwapBlockIndex.m_x;
            m_threadParams.m_blockY = choosedSwapBlockIndex.m_y;
            m_threadParams.m_blockPtr = m_getBlockPointer_BlkPtr;             
            m_threadParams.m_task = ThreadParameters::WriteTaskT;
          }
          
          m_threadParams.m_doTaskCondVar.notify_one();
          
          // wait for the block writing request to finish
          {
            boost::unique_lock<boost::mutex> lock(
              m_threadParams.m_taskFinishedMutex);
              
            while( ! m_threadParams.m_taskFinished )
            {
              m_threadParams.m_taskFinishedCondVar.wait( lock );
            }
          }
          
          m_threadParams.m_taskFinished = false;
        }
        else
        {
          m_rasterPtr->getBand( choosedSwapBlockIndex.m_b )->write( 
            choosedSwapBlockIndex.m_x, choosedSwapBlockIndex.m_y,
            m_getBlockPointer_BlkPtr );
        }
      }
    }
    
    // reading the required block
    if( m_dataPrefetchThreshold )
    {
      // request a block reading to the thread
      {
        boost::lock_guard<boost::mutex> lock(m_threadParams.m_doTaskMutex);
        
        m_threadParams.m_blockB = band;
        m_threadParams.m_blockX = x;
        m_threadParams.m_blockY = y;
        m_threadParams.m_exchangeBlockPtr = m_getBlockPointer_BlkPtr;
        m_threadParams.m_task = ThreadParameters::ReadTaskT;
      }
      
      m_threadParams.m_doTaskCondVar.notify_one();
      
      // wait for the block reading request to finish
      {
        boost::unique_lock<boost::mutex> lock(
          m_threadParams.m_taskFinishedMutex);
          
        while( ! m_threadParams.m_taskFinished )
        {
          m_threadParams.m_taskFinishedCondVar.wait( lock );
        }
      }
      
      m_threadParams.m_taskFinished = false;
      
      m_blocksPointers[ band ][ y ][ x ] = m_threadParams.m_blockPtr;
      m_getBlockPointer_BlkPtr = m_threadParams.m_blockPtr;
    }
    else
    {    
      m_rasterPtr->getBand( band )->read( x, y, m_getBlockPointer_BlkPtr );
      m_blocksPointers[ band ][ y ][ x ] = m_getBlockPointer_BlkPtr;
    }
    
    // advances the next swap block fifo index
    choosedSwapBlockIndex.m_b = band;
    choosedSwapBlockIndex.m_y = y;
    choosedSwapBlockIndex.m_x = x;    
    m_blocksFifoNextSwapBlockIndex = ( m_blocksFifoNextSwapBlockIndex + 1 ) % 
      m_maxNumberOfCacheBlocks;        
  }
  
  return m_getBlockPointer_BlkPtr;  
}

void te::mem::CachedBandBlocksManager::threadEntry(ThreadParameters* paramsPtr)
{
  assert( paramsPtr );
  assert( paramsPtr->m_rasterPtr );
  
  // internal data block to exchange
  unsigned char* internalExchangeBlockPtr = paramsPtr->m_threadDataBlockHandler.get();
  assert( internalExchangeBlockPtr );
  
  int readXDirectionVector = 0;
  int readYDirectionVector = 0;
  int readBDirectionVector = 0;
  int lastReadBlkX = 0;
  int lastReadBlkY = 0;
  int lastReadBlkB = 0;
  bool hasReadedAheadBlk = false;
  bool doReadAhead = false;
  int readAheadBlkB = 0;
  int readAheadBlkX = 0;
  int readAheadBlkY = 0;
  
  while( paramsPtr->m_keepRunning )
  {
    // wait for a task
    {
      boost::unique_lock<boost::mutex> lock( paramsPtr->m_doTaskMutex );
      
      while( paramsPtr->m_task == ThreadParameters::InvalidTaskT )
      {
        paramsPtr->m_doTaskCondVar.wait( lock );
      }
      
      if( paramsPtr->m_task == ThreadParameters::ReadTaskT )
      {
        assert( paramsPtr->m_exchangeBlockPtr );
        
        if( hasReadedAheadBlk && ( readAheadBlkB == paramsPtr->m_blockB )
          && ( readAheadBlkX == paramsPtr->m_blockX )
          && ( readAheadBlkY == paramsPtr->m_blockY ) )
        { 
          // use the read-ahed block
          paramsPtr->m_blockPtr = internalExchangeBlockPtr;
          internalExchangeBlockPtr = paramsPtr->m_exchangeBlockPtr;
          hasReadedAheadBlk = false;
        }
        else
        { 
          // read from the external raster
          paramsPtr->m_rasterPtr->getBand( paramsPtr->m_blockB )->read( paramsPtr->m_blockX, 
            paramsPtr->m_blockY, paramsPtr->m_blockPtr );
        }
        
        // defining if read-ahed must be done
        
        if( paramsPtr->m_dataPrefetchThreshold )
        {
          readXDirectionVector += ( paramsPtr->m_blockX - lastReadBlkX );
          readYDirectionVector += ( paramsPtr->m_blockY - lastReadBlkY );
          readBDirectionVector += ( paramsPtr->m_blockB - lastReadBlkB );
          
          doReadAhead = false;
          
          if( std::abs( readXDirectionVector )  > paramsPtr->m_dataPrefetchThreshold )
          {
            if( readXDirectionVector > 0 )
            {
              readAheadBlkX = ((int)paramsPtr->m_blockX) + 1;
              --readXDirectionVector;
            }
            else
            {
              readAheadBlkX = ((int)paramsPtr->m_blockX) - 1;
              ++readXDirectionVector;
            }
              
            if( ( readAheadBlkX >= 0 ) &&
              ( readAheadBlkX < paramsPtr->m_rasterPtr->getNumberOfColumns() ) )
              doReadAhead = true;
            else
              readAheadBlkX = ((int)paramsPtr->m_blockX);
          }
          
          if( std::abs( readYDirectionVector )  > paramsPtr->m_dataPrefetchThreshold )
          {
            if( readYDirectionVector > 0 )
            {
              readAheadBlkY = ((int)paramsPtr->m_blockY) + 1;
              --readYDirectionVector;
            }
            else
            {
              readAheadBlkY = ((int)paramsPtr->m_blockY) - 1;
              ++readYDirectionVector;
            }
              
            if( ( readAheadBlkY >= 0 ) &&
              ( readAheadBlkY < paramsPtr->m_rasterPtr->getNumberOfRows() ) )
              doReadAhead = true;
            else
              readAheadBlkY = ((int)paramsPtr->m_blockY);
          }
         
          if( std::abs( readBDirectionVector )  > paramsPtr->m_dataPrefetchThreshold )
          {
            if( readBDirectionVector > 0 )
            {
              readAheadBlkB = ((int)paramsPtr->m_blockB) + 1;
              --readBDirectionVector;
            }
            else
            {
              readAheadBlkB = ((int)paramsPtr->m_blockB) - 1;
              ++readBDirectionVector;
            }
              
            if( ( readAheadBlkB >= 0 ) &&
              ( readAheadBlkB < paramsPtr->m_rasterPtr->getNumberOfBands() ) )
              doReadAhead = true;
            else
              readAheadBlkB = ((int)paramsPtr->m_blockB);
          }
          
          lastReadBlkX = paramsPtr->m_blockX;
          lastReadBlkY = paramsPtr->m_blockY;
          lastReadBlkB = paramsPtr->m_blockB;
        }
        
        // notifying the task finishment
        
        paramsPtr->m_taskFinished = true;
        paramsPtr->m_task = ThreadParameters::InvalidTaskT;
        paramsPtr->m_taskFinishedCondVar.notify_one();
      }
      else if( paramsPtr->m_task == ThreadParameters::WriteTaskT )
      {
        paramsPtr->m_rasterPtr->getBand( paramsPtr->m_blockB )->write( paramsPtr->m_blockX, 
          paramsPtr->m_blockY, paramsPtr->m_blockPtr );
          
        // notifying the task finishment
        
        paramsPtr->m_taskFinished = true;
        paramsPtr->m_task = ThreadParameters::InvalidTaskT;
        paramsPtr->m_taskFinishedCondVar.notify_one();          
      }
    }
    
    // do read-ahead if necessary
    
    if( doReadAhead )
    {
      doReadAhead = false;
      paramsPtr->m_rasterPtr->getBand( readAheadBlkB )->read( readAheadBlkX, 
        readAheadBlkY, internalExchangeBlockPtr );      
        
      hasReadedAheadBlk = true;
    }
  }
}
        

