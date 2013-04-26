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
  \file terralib/memory/ExpansibleBandBlocksManager.cpp
 
  \brief RAM cached and tiled raster band blocks manager.
*/

#include "ExpansibleBandBlocksManager.h"
#include "Exception.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../common/PlatformUtils.h"
#include "../common/Translator.h"

#include <algorithm>

void te::mem::ExpansibleBandBlocksManager::initState()
{
  m_isInitialized = false;
  m_maxNumberRAMBlocks = 0;
  m_maxBlockSizeBytes = 0;
  m_maxDiskFilesSize = 0;
  m_maxBlockSizeBytes = 0;
  m_nextFIFOPositionOverSwapFifo = 0;
  m_currSwapBlockPtr = 0;
}

te::mem::ExpansibleBandBlocksManager::ExpansibleBandBlocksManager()
{
  initState();
}

te::mem::ExpansibleBandBlocksManager::~ExpansibleBandBlocksManager()
{
  free();
}

bool te::mem::ExpansibleBandBlocksManager::initialize( 
  const unsigned int maxNumberRAMBlocks,
  const std::vector< unsigned int>& numbersOfBlocksX,
  const std::vector< unsigned int>& numbersOfBlocksY,
  const std::vector< unsigned int>& blocksSizesBytes,
  const unsigned long int maxDiskFilesSize )
{
  if( numbersOfBlocksX.size() != numbersOfBlocksY.size() ) return false;
  if( numbersOfBlocksY.size() != blocksSizesBytes.size() ) return false;
  
  free();
  
  m_maxNumberRAMBlocks = maxNumberRAMBlocks;
  m_numbersOfBlocksX = numbersOfBlocksX;
  m_numbersOfBlocksY = numbersOfBlocksY;
  m_blocksSizesBytes = blocksSizesBytes;
  m_maxDiskFilesSize = maxDiskFilesSize;
  
  // Discovering boundaries
  
  m_maxBlockSizeBytes = 0;  
  unsigned int maxNumbersOfBlocksX = 0;
  unsigned int maxNumbersOfBlocksY = 0;
  unsigned int totalRequiredBlocks = 0;
  
  {
    for( unsigned int blockBIdx = 0 ; blockBIdx < numbersOfBlocksX.size() ;  
      ++blockBIdx )
    {
      if( m_maxBlockSizeBytes < blocksSizesBytes[ blockBIdx ] )
        m_maxBlockSizeBytes = blocksSizesBytes[ blockBIdx ];
      
      if( maxNumbersOfBlocksX < numbersOfBlocksX[ blockBIdx ] )
        maxNumbersOfBlocksX = numbersOfBlocksX[ blockBIdx ];
      
      if( maxNumbersOfBlocksY < numbersOfBlocksY[ blockBIdx ] )
        maxNumbersOfBlocksY = numbersOfBlocksY[ blockBIdx ];  
      
      totalRequiredBlocks += ( numbersOfBlocksX[ blockBIdx ] *
        numbersOfBlocksY[ blockBIdx ] );
    }
  }
  
  if( m_maxBlockSizeBytes > maxDiskFilesSize ) return false;
  
  // Allocating RAM blocks
  
  unsigned int allocatedRAMBlocksNumber = 0;
  
  try
  {
    m_blocksPointers.resize( boost::extents[ numbersOfBlocksX.size() ][ maxNumbersOfBlocksY ][ maxNumbersOfBlocksX ] );
    
    for( unsigned int blockBIdx = 0 ; blockBIdx < numbersOfBlocksX.size() ;  
      ++blockBIdx )
    {
      for( unsigned int blockYIdx = 0 ; blockYIdx < numbersOfBlocksY[ blockBIdx ] ; 
        ++blockYIdx )
      {
        for( unsigned int blockXIdx = 0 ; blockXIdx < numbersOfBlocksX[ blockBIdx ] ; ++blockXIdx )
        {
          if( 
              ( allocatedRAMBlocksNumber < maxNumberRAMBlocks )
              &&
              ( blockYIdx < numbersOfBlocksY[ blockBIdx ] )
              &&
              ( blockXIdx < numbersOfBlocksX[ blockBIdx ] )
            )
          {
            boost::shared_ptr< RAMBlockHandler > newBlockHandlerPtr( new RAMBlockHandler );
            newBlockHandlerPtr->m_dataPtr = new unsigned char[ m_maxBlockSizeBytes ];
            m_activeRamBlocksHandler.push_back( newBlockHandlerPtr );
            
            m_blocksPointers[ blockBIdx ][ blockYIdx ][ blockXIdx ] = 
              newBlockHandlerPtr.get()->m_dataPtr;
              
            m_swapFifo.push_back( BlockIndex( blockBIdx, blockXIdx, blockYIdx ) );  
            
            ++allocatedRAMBlocksNumber;
          }
          else
          {
            m_blocksPointers[ blockBIdx ][ blockYIdx ][ blockXIdx ] = 0;
          }
        }
      }
    }
  }
  catch(...)
  {
    free();
    return false;
  }
  
  // disk blocks must be used ?
  
  if( allocatedRAMBlocksNumber < totalRequiredBlocks )
  {
    try
    {
      // allocating the swap block
      
      m_swapBlockHandler.m_dataPtr = new unsigned char[ m_maxBlockSizeBytes ];
      
      m_currSwapBlockPtr = m_swapBlockHandler.m_dataPtr;
      
      // Allocating disk blocks
      
      const unsigned int requiredDiskBlocksNumber = totalRequiredBlocks -
        allocatedRAMBlocksNumber;
      
      std::vector< DiskBlockInfo > diskBlocksInfos;                       
      if( ! allocateDiskBlocks( requiredDiskBlocksNumber, diskBlocksInfos, 
        m_diskFilesHandler ) )
      {
        free();
        return false;
      }
      assert( ((unsigned int)diskBlocksInfos.size()) == requiredDiskBlocksNumber );
      
      unsigned int diskBlocksInfosIdx = 0;
      m_diskBlocksInfo.resize( boost::extents[ numbersOfBlocksX.size() ][ maxNumbersOfBlocksY ][ maxNumbersOfBlocksX ] );
      for( unsigned int blockBIdx = 0 ; blockBIdx < numbersOfBlocksX.size() ;  
        ++blockBIdx )
      {
        for( unsigned int blockYIdx = 0 ; blockYIdx < numbersOfBlocksY[ blockBIdx ] ; 
          ++blockYIdx )
        {
          for( unsigned int blockXIdx = 0 ; blockXIdx < numbersOfBlocksX[ blockBIdx ] ; ++blockXIdx )
          {
            if( 
                ( m_blocksPointers[ blockBIdx ][ blockYIdx ][ blockXIdx ] == 0 )
                &&
                ( blockYIdx < numbersOfBlocksY[ blockBIdx ] )
                &&
                ( blockXIdx < numbersOfBlocksX[ blockBIdx ] )
              )
            {
              assert( diskBlocksInfosIdx < (unsigned int)diskBlocksInfos.size() );
              m_diskBlocksInfo[ blockBIdx ][ blockYIdx ][ blockXIdx ] =
                diskBlocksInfos[ diskBlocksInfosIdx ];
              ++diskBlocksInfosIdx;
            }
          }
        }
      }
    }
    catch(...)
    {
      free();
      return false;
    }    
  }
  
  // finalizing
  
  m_isInitialized = true;
  
  return true;
}

void te::mem::ExpansibleBandBlocksManager::free()
{
  m_numbersOfBlocksX.clear();
  m_numbersOfBlocksY.clear();
  m_blocksSizesBytes.clear();
  m_blocksPointers.resize( boost::extents[0][0][0] );
  m_swapFifo.clear();
  m_activeRamBlocksHandler.clear();
  m_inactiveRamBlocksHandler.clear();
  m_diskBlocksInfo.resize( boost::extents[0][0][0] );
  m_diskFilesHandler.clear();
  
  if( m_swapBlockHandler.m_dataPtr )
  {
    delete[] ( m_swapBlockHandler.m_dataPtr );
    m_swapBlockHandler.m_dataPtr = 0;
  }
  
  initState();
}

void* te::mem::ExpansibleBandBlocksManager::getBlockPointer(unsigned int band, 
  unsigned int x, unsigned int y )
{
  assert( m_isInitialized );
  assert( band < m_blocksPointers.shape()[ 0 ] );
  assert( y < m_blocksPointers.shape()[ 1 ] );
  assert( x < m_blocksPointers.shape()[ 2 ] );
  
  getBlockPointer_returnValue = m_blocksPointers[ band ][ y ][ x ];
  
  if( getBlockPointer_returnValue )
  {
    return getBlockPointer_returnValue;
  }
  else if( m_inactiveRamBlocksHandler.empty() )
  {
    // defining the blocks that will manipulated
    
    assert( m_nextFIFOPositionOverSwapFifo < m_swapFifo.size() );
    BlockIndex& swapIndex = m_swapFifo[ m_nextFIFOPositionOverSwapFifo ];
    
    assert( swapIndex.m_bIndex < m_diskBlocksInfo.shape()[ 0 ] );
    assert( swapIndex.m_yIndex < m_diskBlocksInfo.shape()[ 1 ] );
    assert( swapIndex.m_xIndex < m_diskBlocksInfo.shape()[ 2 ] );     
    DiskBlockInfo& outDiskInfo = m_diskBlocksInfo[ swapIndex.m_bIndex ][ 
      swapIndex.m_yIndex ][ swapIndex.m_xIndex ];    
    
    assert( band < m_diskBlocksInfo.shape()[ 0 ] );
    assert( y < m_diskBlocksInfo.shape()[ 1 ] );
    assert( x < m_diskBlocksInfo.shape()[ 2 ] );      
    DiskBlockInfo& inDiskInfo = m_diskBlocksInfo[ band ][ y ][ x ];
    
    // reading the required block into m_currSwapBlockPtr
    
    if( 0 != fseek( inDiskInfo.m_filePtr, 
      (long)( inDiskInfo.m_fileOff ), SEEK_SET ) )
    {
      throw Exception(TR_MEMORY("File seek error") );
    }
      
    assert( m_currSwapBlockPtr != 0  );
    if( 1 != fread( (void*)m_currSwapBlockPtr, (size_t)( m_maxBlockSizeBytes ), 
      1, inDiskInfo.m_filePtr ) )
    {
      throw Exception(TR_MEMORY("File read error") );
    }
    
    // Flushing the choosed tile to disk
    
    if( 0 != fseek( inDiskInfo.m_filePtr, 
      (long)( inDiskInfo.m_fileOff ), SEEK_SET ) )
    {
      throw Exception(TR_MEMORY("File seek error") );
    }
      
    assert( m_blocksPointers[ swapIndex.m_bIndex ][ swapIndex.m_yIndex ][ swapIndex.m_xIndex ] != 0 );
    if( 1 != fwrite( (void*)m_blocksPointers[ swapIndex.m_bIndex ][ swapIndex.m_yIndex ][ swapIndex.m_xIndex ], 
      (size_t)( m_maxBlockSizeBytes ), 1, 
      inDiskInfo.m_filePtr ) )
    {
      throw Exception(TR_MEMORY("File write error") );
    }
    
    // updating the indexing structures
    
    getBlockPointer_returnValue = m_currSwapBlockPtr;
    
    m_currSwapBlockPtr = m_blocksPointers[ swapIndex.m_bIndex ][ swapIndex.m_yIndex ][ swapIndex.m_xIndex ];
    
    m_blocksPointers[ band ][ y ][ x ] = getBlockPointer_returnValue;
    
    m_blocksPointers[ swapIndex.m_bIndex ][ swapIndex.m_yIndex ][ swapIndex.m_xIndex ] = 0;
    
    swapIndex.m_bIndex = band;
    swapIndex.m_xIndex = x;
    swapIndex.m_yIndex = y;
    
    outDiskInfo.m_filePtr = inDiskInfo.m_filePtr;
    outDiskInfo.m_fileOff = inDiskInfo.m_fileOff;
    
    inDiskInfo.m_filePtr = 0;
    inDiskInfo.m_fileOff = 0;
    
    m_nextFIFOPositionOverSwapFifo = ( ( m_nextFIFOPositionOverSwapFifo + 1 ) % 
      m_activeRamBlocksHandler.size() );
      
    // returning the required block pointer 
      
    return getBlockPointer_returnValue;
  }
  else
  {
    // Activating one inactive block
    
    m_activeRamBlocksHandler.push_back( m_inactiveRamBlocksHandler.back() );
    m_inactiveRamBlocksHandler.pop_back();
    
    getBlockPointer_returnValue = m_activeRamBlocksHandler.back()->m_dataPtr;
    
    // reading the required block into m_currSwapBlockPtr
    
    assert( band < m_diskBlocksInfo.shape()[ 0 ] );
    assert( y < m_diskBlocksInfo.shape()[ 1 ] );
    assert( x < m_diskBlocksInfo.shape()[ 2 ] );      
    DiskBlockInfo& inDiskInfo = m_diskBlocksInfo[ band ][ y ][ x ];     
    
    if( 0 != fseek( inDiskInfo.m_filePtr, 
      (long)( inDiskInfo.m_fileOff ), SEEK_SET ) )
    {
      throw Exception(TR_MEMORY("File seek error") );
    }
      
    assert( getBlockPointer_returnValue != 0  );
    if( 1 != fread( (void*)getBlockPointer_returnValue, (size_t)( m_maxBlockSizeBytes ), 
      1, inDiskInfo.m_filePtr ) )
    {
      throw Exception(TR_MEMORY("File read error") );
    }    
    
    // updating the indexing structures
    
    m_swapFifo.push_back( BlockIndex( band, x, y ) );
    
    m_blocksPointers[ band ][ y ][ x ] = getBlockPointer_returnValue;
    
    inDiskInfo.m_fileOff = 0;
    inDiskInfo.m_filePtr = 0;

    return getBlockPointer_returnValue;
  }  
}

bool te::mem::ExpansibleBandBlocksManager::allocateDiskBlocks( 
  const unsigned int blocksNumber,
  std::vector< DiskBlockInfo >& diskBlocksInfos,                       
  std::vector< boost::shared_ptr< OpenDiskFileHandler > >& diskFilesHandler )
{
  assert( m_maxDiskFilesSize > 0 );
  assert( m_maxBlockSizeBytes > 0 );
  assert( m_maxDiskFilesSize >= m_maxBlockSizeBytes );
  
  diskBlocksInfos.clear();
  diskFilesHandler.clear();

  const unsigned int maxBlocksPerFile = m_maxDiskFilesSize / m_maxBlockSizeBytes;

  unsigned int remainnigBlocksNumber = blocksNumber;
    
  while( remainnigBlocksNumber )
  {
    const unsigned int fileBlocksNumber = std::min( maxBlocksPerFile, remainnigBlocksNumber );
    const unsigned long int fileSizeBytes = ((unsigned long int)fileBlocksNumber) *
      ((unsigned long int)m_maxBlockSizeBytes);
    assert( fileSizeBytes <= m_maxDiskFilesSize );
      
    boost::shared_ptr< OpenDiskFileHandler > newFileHandlerPtr( new OpenDiskFileHandler );
    if( ! createNewDiskFile( fileSizeBytes, &( newFileHandlerPtr->m_filePtr ) ) )
    {
      return false;
    }
    diskFilesHandler.push_back( newFileHandlerPtr );
    
    for( unsigned int fileBlockIdx = 0 ; fileBlockIdx < fileBlocksNumber ;
      ++fileBlockIdx )
    {
      diskBlocksInfos.push_back( DiskBlockInfo() );
      diskBlocksInfos.back().m_filePtr = newFileHandlerPtr->m_filePtr;
      diskBlocksInfos.back().m_fileOff = ((unsigned long int)fileBlockIdx) * 
        ((unsigned long int)m_maxBlockSizeBytes);
    }
      
    remainnigBlocksNumber -= fileBlocksNumber;
  }
    
  return true;
}

bool te::mem::ExpansibleBandBlocksManager::createNewDiskFile( unsigned long int size,
  FILE** fileptr ) const
{
  //(*fileptr) = fopen( filename.c_str(), "wb+" );
  (*fileptr) = tmpfile();
  if( (*fileptr) == 0 )
    return false;
  
  long seekoff = (long)( size - 1 );
  
  if( 0 != fseek( (*fileptr), seekoff, SEEK_SET ) )
  {
    fclose( (*fileptr) );
    return false;
  }

  unsigned char c = '\0';
  if( 1 != fwrite( &c, 1, 1, (*fileptr) ) )
  {
    fclose( (*fileptr) );
    return false;
  }
    
  return true;
}

        

