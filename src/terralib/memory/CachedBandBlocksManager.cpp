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
  \file terralib/memory/CachedBandBlocksManager.cpp

  \brief RAM cached and tiled raster band blocks manager.
*/

// TerraLib
#include "../common/PlatformUtils.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "CachedBandBlocksManager.h"

// STL
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
  m_getBlockPointer_BlkPtr = 0;
}

te::mem::CachedBandBlocksManager::CachedBandBlocksManager()
{
  initState();
}

te::mem::CachedBandBlocksManager::~CachedBandBlocksManager()
{
  free();
}

bool te::mem::CachedBandBlocksManager::initialize( 
  const te::rst::Raster& externalRaster, const unsigned char maxMemPercentUsed, 
  const unsigned int dataPrefetchThreshold)
{
  free();
  
  // Finding the global block dimensions
  
  unsigned int maxBlockSizeBytes = 0;
  
  for( unsigned int bandIdx = 0 ; bandIdx < externalRaster.getNumberOfBands() ;
    ++bandIdx )
  {
    if( maxBlockSizeBytes < (unsigned int)externalRaster.getBand( bandIdx )->getBlockSize() )
      maxBlockSizeBytes = (unsigned int)externalRaster.getBand( bandIdx )->getBlockSize();
  }
  
  const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
  const double usedVMem = (double)te::common::GetUsedVirtualMemory();
  const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() );
  const double freeVMem = ( ((double)maxMemPercentUsed) / 100.0 ) *
    std::min( totalPhysMem, ( totalVMem - usedVMem ) );  
  const unsigned int maxNumberOfCacheBlocks = (unsigned int)
    std::max( 1.0, std::ceil( freeVMem / ((double)maxBlockSizeBytes) ) );
    
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
    if( m_globalBlocksNumberX < (unsigned int)externalRaster.getBand( bandIdx )->getProperty()->m_nblocksx )
      m_globalBlocksNumberX = (unsigned int)externalRaster.getBand( bandIdx )->getProperty()->m_nblocksx;
      
    if( m_globalBlocksNumberY < (unsigned int)externalRaster.getBand( bandIdx )->getProperty()->m_nblocksy )
      m_globalBlocksNumberY = (unsigned int)externalRaster.getBand( bandIdx )->getProperty()->m_nblocksy;
      
    if( m_globalBlockSizeBytes < (unsigned int)externalRaster.getBand( bandIdx )->getBlockSize() )
      m_globalBlockSizeBytes = (unsigned int)externalRaster.getBand( bandIdx )->getBlockSize();
    
    numberOfRasterBlocks +=
      ( externalRaster.getBand( bandIdx )->getProperty()->m_nblocksx *
      externalRaster.getBand( bandIdx )->getProperty()->m_nblocksy );    
  }
    
  // Allocating the internal structures
  
  m_rasterPtr = (te::rst::Raster*)&externalRaster;
  m_dataPrefetchThreshold = dataPrefetchThreshold;
  
  m_maxNumberOfCacheBlocks = std::min( maxNumberOfCacheBlocks, numberOfRasterBlocks );
  m_maxNumberOfCacheBlocks = std::max( m_maxNumberOfCacheBlocks, (unsigned int)1 );
  
  unsigned int blockBIdx = 0;  
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
  
  if( m_dataPrefetchThreshold )
  {
    m_threadParams.m_rasterPtr = (te::rst::Raster*)&externalRaster;
    m_threadParams.m_dataPrefetchThreshold = dataPrefetchThreshold;
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
    {
      boost::lock_guard<boost::mutex> lock(m_threadParams.m_doTaskMutex);
      m_threadParams.m_task = ThreadParameters::SuicideTastT;
      m_threadParams.m_doTaskCondVar.notify_one();
    }
    
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
  assert( x < (unsigned int)m_rasterPtr->getBand( band )->getProperty()->m_nblocksx );
  assert( y < (unsigned int)m_rasterPtr->getBand( band )->getProperty()->m_nblocksy );
  
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
            m_threadParams.m_exchangeBlockPtr = 0;
            m_threadParams.m_task = ThreadParameters::WriteTaskT;
            m_threadParams.m_taskFinished = false;
          }
          
          m_threadParams.m_doTaskCondVar.notify_one();
          
          // wait for the block writing request to finish
          while( ! m_threadParams.m_taskFinished )
          {
            boost::unique_lock<boost::mutex> lock(
              m_threadParams.m_taskFinishedMutex);
              
            while( ! m_threadParams.m_taskFinished )
            {
              m_threadParams.m_taskFinishedCondVar.wait( lock );
            }
          }
        }
        else
        {
          m_rasterPtr->getBand( choosedSwapBlockIndex.m_b )->write( 
            choosedSwapBlockIndex.m_x, choosedSwapBlockIndex.m_y,
            m_getBlockPointer_BlkPtr );
        }
      }
      
      // advances the next swap block fifo index
      choosedSwapBlockIndex.m_b = band;
      choosedSwapBlockIndex.m_y = y;
      choosedSwapBlockIndex.m_x = x;    
      m_blocksFifoNextSwapBlockIndex = ( m_blocksFifoNextSwapBlockIndex + 1 ) % 
        ((unsigned int)m_blocksFifo.size());        
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
        m_threadParams.m_blockPtr = 0;
        m_threadParams.m_exchangeBlockPtr = m_getBlockPointer_BlkPtr;
        m_threadParams.m_task = ThreadParameters::ReadTaskT;
        m_threadParams.m_taskFinished = false;
        
        m_threadParams.m_doTaskCondVar.notify_one();
      }
      
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
    
      
  }
  
  return m_getBlockPointer_BlkPtr;  
}

void te::mem::CachedBandBlocksManager::threadEntry(ThreadParameters* paramsPtr)
{
//  std::cout << std::endl << "Thread started" << std::endl;
  
  assert( paramsPtr );
  assert( paramsPtr->m_rasterPtr );
  
  // internal data block to exchange
  unsigned char* internalExchangeBlockPtr = paramsPtr->m_threadDataBlockHandler.get();
  assert( internalExchangeBlockPtr );
  
  int globalReadAheadDirectionVectorX = 0;
  int globalReadAheadDirectionVectorY = 0;
  int globalReadAheadDirectionVectorB = 0;
  bool globalReadAheadBlockReady = false;
  int globalReadAheadBlockB = 0;
  int globalReadAheadBlockX = 0;
  int globalReadAheadBlockY = 0;  
  int lastReadBlockX = 0;
  int lastReadBlockY = 0;
  int lastReadBlockB = 0;
  
  int currentReadDirectionVectorX = 0;
  int currentReadDirectionVectorY = 0;
  int currentReadDirectionVectorB = 0;
  int nextReadAheadBlkB = 0;
  int nextReadAheadBlkX = 0;
  int nextReadAheadBlkY = 0;
  bool doReadAhead = false;
  
  while( true )
  {
    // wait for a task
    boost::unique_lock<boost::mutex> lock( paramsPtr->m_doTaskMutex );
    while( paramsPtr->m_task == ThreadParameters::InvalidTaskT )
    {
      paramsPtr->m_doTaskCondVar.wait( lock );
    }
    
    if( paramsPtr->m_task == ThreadParameters::ReadTaskT )
    {
//      std::cout << std::endl << "Starting a read task" << std::endl;
      
      assert( paramsPtr->m_blockPtr == 0 );
      assert( paramsPtr->m_exchangeBlockPtr );
      
      if( globalReadAheadBlockReady 
        && ( globalReadAheadBlockB == (int)paramsPtr->m_blockB )
        && ( globalReadAheadBlockX == (int)paramsPtr->m_blockX )
        && ( globalReadAheadBlockY == (int)paramsPtr->m_blockY ) )
      { 
//        std::cout << std::endl << "Read-ahead block is avaliable, it will be used" << std::endl;
        
        // use the read-ahed block
        paramsPtr->m_blockPtr = internalExchangeBlockPtr;
        globalReadAheadBlockReady = false;
        internalExchangeBlockPtr = paramsPtr->m_exchangeBlockPtr;
        
//        std::cout << std::endl << "Read-ahead block used." << std::endl;
      }
      else
      { 
//        std::cout << std::endl << "Read-ahead block not avaliable, block will be read from raster" << std::endl;
        
        // read from the external raster
        paramsPtr->m_rasterPtr->getBand( paramsPtr->m_blockB )->read( paramsPtr->m_blockX, 
          paramsPtr->m_blockY, paramsPtr->m_exchangeBlockPtr );
        paramsPtr->m_blockPtr = paramsPtr->m_exchangeBlockPtr;
          
//        std::cout << std::endl << "Block readed from raster." << std::endl;
      }
      
      // notifying the task finishment
      
      {
        boost::lock_guard<boost::mutex> lock( paramsPtr->m_taskFinishedMutex );
        
        paramsPtr->m_taskFinished = true;
        paramsPtr->m_task = ThreadParameters::InvalidTaskT;
        
        paramsPtr->m_taskFinishedCondVar.notify_one();
      }
      
      // defining if read-ahed must be done
      
      if( paramsPtr->m_dataPrefetchThreshold )
      {
        currentReadDirectionVectorX = ( ((int)paramsPtr->m_blockX) - lastReadBlockX );
        currentReadDirectionVectorY = ( ((int)paramsPtr->m_blockY) - lastReadBlockY );
        currentReadDirectionVectorB = ( ((int)paramsPtr->m_blockB) - lastReadBlockB );
        
        doReadAhead = false;
        
        if( ( std::abs( currentReadDirectionVectorX ) < 2 ) && 
          ( std::abs( currentReadDirectionVectorY ) < 2 )
          && ( std::abs( currentReadDirectionVectorB ) < 2 ) )
        {
          globalReadAheadDirectionVectorX += currentReadDirectionVectorX;
          globalReadAheadDirectionVectorY += currentReadDirectionVectorY;
          globalReadAheadDirectionVectorB += currentReadDirectionVectorB;
          
          if( ((unsigned int)std::abs( globalReadAheadDirectionVectorB ))  > 
            paramsPtr->m_dataPrefetchThreshold )
          {
            if( globalReadAheadDirectionVectorB > 0 )
            {
              nextReadAheadBlkB = ((int)paramsPtr->m_blockB) + 1;
              --globalReadAheadDirectionVectorB;
            }
            else
            {
              nextReadAheadBlkB = ((int)paramsPtr->m_blockB) - 1;
              ++globalReadAheadDirectionVectorB;
            }
              
            if( ( nextReadAheadBlkB >= 0 ) &&
              ( nextReadAheadBlkB < (int)paramsPtr->m_rasterPtr->getNumberOfBands() ) )
              doReadAhead = true;
            else
              nextReadAheadBlkB = ((int)paramsPtr->m_blockB);
          }           
          
          if( ((unsigned int)std::abs( globalReadAheadDirectionVectorX ))  > 
            paramsPtr->m_dataPrefetchThreshold )
          {
            if( globalReadAheadDirectionVectorX > 0 )
            {
              nextReadAheadBlkX = ((int)paramsPtr->m_blockX) + 1;
              --globalReadAheadDirectionVectorX;
            }
            else
            {
              nextReadAheadBlkX = ((int)paramsPtr->m_blockX) - 1;
              ++globalReadAheadDirectionVectorX;
            }
              
            if( ( nextReadAheadBlkX >= 0 ) &&
              ( nextReadAheadBlkX < 
              paramsPtr->m_rasterPtr->getBand( nextReadAheadBlkB )->getProperty()->m_nblocksx ) )
              doReadAhead = true;
            else
              nextReadAheadBlkX = ((int)paramsPtr->m_blockX);
          }
                    
          if( ((unsigned int)std::abs( globalReadAheadDirectionVectorY ))  > 
            paramsPtr->m_dataPrefetchThreshold )
          {
            if( globalReadAheadDirectionVectorY > 0 )
            {
              nextReadAheadBlkY = ((int)paramsPtr->m_blockY) + 1;
              --globalReadAheadDirectionVectorY;
            }
            else
            {
              nextReadAheadBlkY = ((int)paramsPtr->m_blockY) - 1;
              ++globalReadAheadDirectionVectorY;
            }
              
            if( ( nextReadAheadBlkY >= 0 ) &&
              ( nextReadAheadBlkY < 
              paramsPtr->m_rasterPtr->getBand( nextReadAheadBlkB )->getProperty()->m_nblocksy ) )
              doReadAhead = true;
            else
              nextReadAheadBlkY = ((int)paramsPtr->m_blockY);
          }
        }
        else
        {
          globalReadAheadDirectionVectorX = 0;
          globalReadAheadDirectionVectorY = 0;
          globalReadAheadDirectionVectorB = 0;
        }
    
        if( doReadAhead )
        {
//          std::cout << std::endl << "Reading-ahead a new block" << std::endl;

          assert( nextReadAheadBlkB < (int)paramsPtr->m_rasterPtr->getNumberOfBands() );
          assert( nextReadAheadBlkX < 
            paramsPtr->m_rasterPtr->getBand( nextReadAheadBlkB )->getProperty()->m_nblocksx );
          assert( nextReadAheadBlkY < 
            paramsPtr->m_rasterPtr->getBand( nextReadAheadBlkB )->getProperty()->m_nblocksy );
          
          paramsPtr->m_rasterPtr->getBand( nextReadAheadBlkB )->read( nextReadAheadBlkX, 
            nextReadAheadBlkY, internalExchangeBlockPtr );      
          
          globalReadAheadBlockX = nextReadAheadBlkX;
          globalReadAheadBlockY = nextReadAheadBlkY;
          globalReadAheadBlockB = nextReadAheadBlkB;
          globalReadAheadBlockReady = true;
          
//          std::cout << std::endl << "Read-ahead block reading completed." << std::endl;
        }
      }
      
      lastReadBlockX = (int)paramsPtr->m_blockX;
      lastReadBlockY = (int)paramsPtr->m_blockY;
      lastReadBlockB = (int)paramsPtr->m_blockB;         
    }
    else if( paramsPtr->m_task == ThreadParameters::WriteTaskT )
    {
//      std::cout << std::endl << "Starting a write task" << std::endl;
      
      assert( paramsPtr->m_blockPtr );
      assert( paramsPtr->m_exchangeBlockPtr == 0 );
      
      paramsPtr->m_rasterPtr->getBand( paramsPtr->m_blockB )->write( paramsPtr->m_blockX, 
        paramsPtr->m_blockY, paramsPtr->m_blockPtr );
        
      // notifying the task finishment
      
      paramsPtr->m_taskFinished = true;
      paramsPtr->m_task = ThreadParameters::InvalidTaskT;
      paramsPtr->m_taskFinishedCondVar.notify_one();          
    }
    else if( paramsPtr->m_task == ThreadParameters::SuicideTastT )
    {
//      std::cout << std::endl << "Thread ending" << std::endl;
      return;
    }
    
//    std::cout << std::endl << "Task ended" << std::endl;
  }
}
        

