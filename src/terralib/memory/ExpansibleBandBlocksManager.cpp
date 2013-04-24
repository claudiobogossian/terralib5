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
  m_nextFIFOPositionOverRamBlocksHandler = 0;
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
  assert( numbersOfBlocksX.size() == numbersOfBlocksY.size() );
  assert( numbersOfBlocksY.size() == blocksSizesBytes.size() );
  
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
    }
  }
  
  // Allocating RAM blocks
  
  unsigned int totalRequiredBlocks = 0;
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
            newBlockHandlerPtr->m_bIndex = blockBIdx;
            newBlockHandlerPtr->m_xIndex = blockXIdx;
            newBlockHandlerPtr->m_yIndex = blockYIdx;  
            
            m_ramBlocksHandler.push_back( newBlockHandlerPtr );
            
            m_blocksPointers[ blockBIdx ][ blockYIdx ][ blockXIdx ] = 
              newBlockHandlerPtr.get()->m_dataPtr;
            
            ++allocatedRAMBlocksNumber;
          }
          else
          {
            m_blocksPointers[ blockBIdx ][ blockYIdx ][ blockXIdx ] = 0;
          }
          
          ++totalRequiredBlocks;
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
      std::vector< boost::shared_ptr< OpenDiskFileHandler > > diskFilesHandler;
      if( ! allocateDiskBlocks( requiredDiskBlocksNumber, diskBlocksInfos, 
        diskFilesHandler ) )
      {
        free();
        return false;
      }
      assert( ((unsigned int)diskBlocksInfos.size()) == requiredDiskBlocksNumber );
      
      m_diskFilesHandler.insert(  m_diskFilesHandler.end(), diskFilesHandler.begin(),
        diskFilesHandler.end() );
        
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
  m_ramBlocksHandler.clear();
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
  else
  {
    // defining the blocks that will manipulated
    
    assert( m_nextFIFOPositionOverRamBlocksHandler < m_ramBlocksHandler.size() );
    RAMBlockHandler& blockToSwapToDiskHandler = 
      (*(m_ramBlocksHandler[ m_nextFIFOPositionOverRamBlocksHandler ]));
    
    assert( band < m_diskBlocksInfo.shape()[ 0 ] );
    assert( y < m_diskBlocksInfo.shape()[ 1 ] );
    assert( x < m_diskBlocksInfo.shape()[ 2 ] );      
    DiskBlockInfo& blockToSwapFromDiskInfo = m_diskBlocksInfo[ band ][ y ][ x ];
    
    // reading the required block into m_currSwapBlockPtr
    
    if( 0 != fseek( blockToSwapFromDiskInfo.m_filePtr, 
      (long)( blockToSwapFromDiskInfo.m_fileOff ), SEEK_SET ) )
    {
      throw Exception(TR_MEMORY("File seek error") );
    }
      
    assert( m_currSwapBlockPtr != 0  );
    if( 1 != fread( (void*)m_currSwapBlockPtr, (size_t)( m_maxBlockSizeBytes ), 
      1, blockToSwapFromDiskInfo.m_filePtr ) )
    {
      throw Exception(TR_MEMORY("File read error") );
    }
    
    // Flushing the choosed tile to disk
    
    if( 0 != fseek( blockToSwapFromDiskInfo.m_filePtr, 
      (long)( blockToSwapFromDiskInfo.m_fileOff ), SEEK_SET ) )
    {
      throw Exception(TR_MEMORY("File seek error") );
    }
      
    assert( blockToSwapToDiskHandler.m_dataPtr != 0 );
    if( 1 != fwrite( (void*)blockToSwapToDiskHandler.m_dataPtr, 
      (size_t)( m_maxBlockSizeBytes ), 1, 
      blockToSwapFromDiskInfo.m_filePtr ) )
    {
      throw Exception(TR_MEMORY("File write error") );
    }
    
    // updating m_blocksPointers. m_currSwapBlockPtr, getBlockPointer_returnValue
    
    m_blocksPointers[ band ][ y ][ x ] = m_currSwapBlockPtr;
    
    getBlockPointer_returnValue = m_currSwapBlockPtr;
    
    assert( blockToSwapToDiskHandler.m_bIndex < m_blocksPointers.shape()[ 0 ] );
    assert( blockToSwapToDiskHandler.m_yIndex < m_blocksPointers.shape()[ 1 ] );
    assert( blockToSwapToDiskHandler.m_xIndex < m_blocksPointers.shape()[ 2 ] );    
    m_currSwapBlockPtr = m_blocksPointers[ blockToSwapToDiskHandler.m_bIndex ][ 
      blockToSwapToDiskHandler.m_yIndex ][ blockToSwapToDiskHandler.m_xIndex ];
    
    m_blocksPointers[ blockToSwapToDiskHandler.m_bIndex ][ 
      blockToSwapToDiskHandler.m_yIndex ][ blockToSwapToDiskHandler.m_xIndex ] = 0;
    
    // updating m_diskBlocksInfo
    
    assert( blockToSwapToDiskHandler.m_bIndex < m_diskBlocksInfo.shape()[ 0 ] );
    assert( blockToSwapToDiskHandler.m_yIndex < m_diskBlocksInfo.shape()[ 1 ] );
    assert( blockToSwapToDiskHandler.m_xIndex < m_diskBlocksInfo.shape()[ 2 ] );     
    DiskBlockInfo& blockSwappedToDiskInfo = m_diskBlocksInfo[ blockToSwapToDiskHandler.m_bIndex ][ 
      blockToSwapToDiskHandler.m_yIndex ][ blockToSwapToDiskHandler.m_xIndex ];
    blockSwappedToDiskInfo.m_filePtr = blockToSwapFromDiskInfo.m_filePtr;
    blockSwappedToDiskInfo.m_fileOff = blockToSwapFromDiskInfo.m_fileOff;
    
    blockToSwapFromDiskInfo.m_fileOff = 0;
    blockToSwapFromDiskInfo.m_filePtr = 0;
    
    // updating m_ramBlocksHandler
    
    blockToSwapToDiskHandler.m_bIndex = band;
    blockToSwapToDiskHandler.m_yIndex = y;
    blockToSwapToDiskHandler.m_xIndex = x;   
    
    // updating m_nextFIFOPositionOverRamBlocksHandler
    
    m_nextFIFOPositionOverRamBlocksHandler = ( ( m_nextFIFOPositionOverRamBlocksHandler + 1 ) % 
      m_ramBlocksHandler.size() );
      
    // returning the required block pointer 
      
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

  const unsigned int maxBlocksPerFile = m_maxDiskFilesSize / m_maxBlockSizeBytes;

  unsigned int remainnigBlocksNumber = blocksNumber;
    
  while( remainnigBlocksNumber )
  {
    const unsigned int fileBlocksNumber = std::min( maxBlocksPerFile, remainnigBlocksNumber );
    const unsigned long int fileSizeBytes = ((unsigned long int)fileBlocksNumber) *
      ((unsigned long int)m_maxBlockSizeBytes);
      
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
      
    remainnigBlocksNumber -= blocksNumber;
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

        

