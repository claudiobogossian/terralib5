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
  \file terralib/memory/ExpansibleRaster.cpp

  \brief A raster (stored in memory and eventually swapped to disk) where it is possible to dynamically add lines/columns/bands.
*/

// TerraLib
#include "../common/Translator.h"
#include "../common/PlatformUtils.h"
#include "../geometry/Envelope.h"
#include "../raster/Utils.h"
#include "../raster/BlockUtils.h"
#include "Exception.h"
#include "ExpansibleRaster.h"


#ifndef TLINTERNAL_EXPANSIBLERASTER_MAXDISKFILESSIZE
  #define TLINTERNAL_EXPANSIBLERASTER_MAXDISKFILESSIZE 2147483648ul
#endif

te::mem::ExpansibleRaster::ExpansibleRaster()
{
}

te::mem::ExpansibleRaster::ExpansibleRaster( const Raster& rhs )
: te::rst::Raster( rhs )
{
}

te::mem::ExpansibleRaster::ExpansibleRaster( te::rst::Grid* grid, te::common::AccessPolicy p )
: te::rst::Raster( grid, p )
{
}

te::mem::ExpansibleRaster::ExpansibleRaster( const unsigned char maxMemPercentUsed, 
  te::rst::Grid* grid, 
  const std::vector<te::rst::BandProperty*> bandsProperties )
  : te::rst::Raster( grid, te::common::RWAccess )
{
  // Finding the global block dimensions
  
  unsigned int maxBlockSizeBytes = 0;
  std::vector< unsigned int> numbersOfBlocksX;
  std::vector< unsigned int> numbersOfBlocksY;
  std::vector< unsigned int> blocksSizesBytes;
  
  {
    for( unsigned int bandIdx = 0 ; bandIdx < bandsProperties.size() ;  ++bandIdx )
    {
      // Making all bands blocking equal
      bandsProperties[ bandIdx ]->m_nblocksx = bandsProperties[ 0 ]->m_nblocksx;
      bandsProperties[ bandIdx ]->m_nblocksy = bandsProperties[ 0 ]->m_nblocksy;
      bandsProperties[ bandIdx ]->m_blkw = bandsProperties[ 0 ]->m_blkw;
      bandsProperties[ bandIdx ]->m_blkh = bandsProperties[ 0 ]->m_blkh;     
      bandsProperties[ bandIdx ]->m_nblocksx = (int)std::ceil(
        ((double)grid->getNumberOfColumns()) / 
        ((double)bandsProperties[ bandIdx ]->m_blkw) );
      bandsProperties[ bandIdx ]->m_nblocksy = (int)std::ceil(
        ((double)grid->getNumberOfRows()) / 
        ((double)bandsProperties[ bandIdx ]->m_blkh) );
      
      const unsigned int blockSizeBytes = (unsigned int)( 
        bandsProperties[ bandIdx ]->m_blkw * 
        bandsProperties[ bandIdx ]->m_blkh * 
        te::rst::GetPixelSize( bandsProperties[ bandIdx ]->m_type ) );
      
      if( maxBlockSizeBytes < blockSizeBytes )
        maxBlockSizeBytes = blockSizeBytes;
          
      numbersOfBlocksX.push_back( (unsigned int)bandsProperties[ bandIdx ]->m_nblocksx );
      numbersOfBlocksY.push_back( (unsigned int)bandsProperties[ bandIdx ]->m_nblocksy );
      blocksSizesBytes.push_back( blockSizeBytes );
    }
  }
  
  const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
  const double usedVMem = (double)te::common::GetUsedVirtualMemory();
  const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() );
  const double freeVMem = ( ((double)maxMemPercentUsed) / 100.0 ) *
    std::min( totalPhysMem, ( totalVMem - usedVMem ) );  
  const unsigned int maxNumberOfBlocks = (unsigned int)
    std::max( 1.0, std::ceil( freeVMem / ((double)maxBlockSizeBytes) ) );    
  
  if( ! m_blocksManager.initialize( maxNumberOfBlocks, numbersOfBlocksX,
    numbersOfBlocksY, blocksSizesBytes, 
    TLINTERNAL_EXPANSIBLERASTER_MAXDISKFILESSIZE ) )
    throw Exception(TE_TR("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < bandsProperties.size() ; 
    ++bandsIdx )
  {
    m_bands.push_back( new te::mem::ExpansibleBand( m_blocksManager, *this,
      *(bandsProperties[ bandsIdx ]) , bandsIdx ) );
    delete ( bandsProperties[ bandsIdx ] );
  }
  
  dummyFillAllBlocks();
}

te::mem::ExpansibleRaster::ExpansibleRaster( te::rst::Grid* grid, 
   const std::vector<te::rst::BandProperty*> bandsProperties,
   const unsigned int maxNumberOfRAMBlocks )
  : te::rst::Raster( grid, te::common::RWAccess )
{
  // Finding the global block dimensions
  
  std::vector< unsigned int> numbersOfBlocksX;
  std::vector< unsigned int> numbersOfBlocksY;
  std::vector< unsigned int> blocksSizesBytes;
  
  {
    for( unsigned int bandIdx = 0 ; bandIdx < bandsProperties.size() ;  ++bandIdx )
    {
      // Making all bands blocking equal
      bandsProperties[ bandIdx ]->m_nblocksx = bandsProperties[ 0 ]->m_nblocksx;
      bandsProperties[ bandIdx ]->m_nblocksy = bandsProperties[ 0 ]->m_nblocksy;
      bandsProperties[ bandIdx ]->m_blkw = bandsProperties[ 0 ]->m_blkw;
      bandsProperties[ bandIdx ]->m_blkh = bandsProperties[ 0 ]->m_blkh;   
      bandsProperties[ bandIdx ]->m_nblocksx = (int)std::ceil(
        ((double)grid->getNumberOfColumns()) / 
        ((double)bandsProperties[ bandIdx ]->m_blkw) );
      bandsProperties[ bandIdx ]->m_nblocksy = (int)std::ceil(
        ((double)grid->getNumberOfRows()) / 
        ((double)bandsProperties[ bandIdx ]->m_blkh) );      
      
      const unsigned int blockSizeBytes = (unsigned int)( 
        bandsProperties[ bandIdx ]->m_blkw * 
        bandsProperties[ bandIdx ]->m_blkh * 
        te::rst::GetPixelSize( bandsProperties[ bandIdx ]->m_type ) );
      
      numbersOfBlocksX.push_back( (unsigned int)bandsProperties[ bandIdx ]->m_nblocksx );
      numbersOfBlocksY.push_back( (unsigned int)bandsProperties[ bandIdx ]->m_nblocksy );
      blocksSizesBytes.push_back( blockSizeBytes );
    }
  }
  
  if( ! m_blocksManager.initialize( maxNumberOfRAMBlocks, numbersOfBlocksX,
    numbersOfBlocksY, blocksSizesBytes, 
    TLINTERNAL_EXPANSIBLERASTER_MAXDISKFILESSIZE ) )
    throw Exception(TE_TR("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < bandsProperties.size() ; 
    ++bandsIdx )
  {
    m_bands.push_back( new te::mem::ExpansibleBand( m_blocksManager, *this,
      *(bandsProperties[ bandsIdx ]) , bandsIdx ) );
    delete ( bandsProperties[ bandsIdx ] );
  }
  
  dummyFillAllBlocks();
}

te::mem::ExpansibleRaster::~ExpansibleRaster()
{
  free();
}

void te::mem::ExpansibleRaster::open(const std::map<std::string, std::string>& /*rinfo*/, 
                                         te::common::AccessPolicy /*p*/)
{
}

std::map<std::string, std::string> te::mem::ExpansibleRaster::getInfo() const
{
  return std::map<std::string, std::string>();
}

te::dt::AbstractData* te::mem::ExpansibleRaster::clone() const
{
  std::vector<te::rst::BandProperty*> bandsProperties;
  for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ; ++bandsIdx )
  {
    bandsProperties.push_back( new te::rst::BandProperty( 
      *(m_bands[ bandsIdx ]->getProperty() ) ) );
  }
  
  return new te::mem::ExpansibleRaster( new te::rst::Grid( *m_grid ), bandsProperties,
    m_blocksManager.getMaxNumberOfRAMBlocks() );
}

bool te::mem::ExpansibleRaster::addTopLines( const unsigned int number )
{
  if( m_bands.empty() ) return false;
  
  if( number )
  {
    const unsigned int blockExpansionSize = (unsigned int)std::ceil( 
      ((double)number) / ((double)m_bands[ 0 ]->getProperty()->m_blkh) );
      
    std::vector< ExpansibleBandBlocksManager::BlockIndex3D > addedBlocksCoords;
      
    for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ; ++bandsIdx )
    {
      std::vector< ExpansibleBandBlocksManager::BlockIndex3D > bandAddedBlocksCoords;
      if( ! m_blocksManager.addTopBlocks( blockExpansionSize, bandsIdx,
        bandAddedBlocksCoords ) ) 
        return false;
      
      addedBlocksCoords.insert( addedBlocksCoords.end(), bandAddedBlocksCoords.begin(),
        bandAddedBlocksCoords.end() );
      
      m_bands[ bandsIdx ]->getProperty()->m_nblocksy += blockExpansionSize;
    }
    
    dummyFillBlocks( addedBlocksCoords );
    
    const unsigned int newLinesNumber = 
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkh) *
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_nblocksy );
    const unsigned int newColsNumber = 
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkw) *
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_nblocksx );
      
    const double addedLinesNumber = (double)( blockExpansionSize *
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkh) );
      
    te::gm::Coord2D newULC( 
      m_grid->getExtent()->getLowerLeftX(), 
      m_grid->getExtent()->getUpperRightY() + ( addedLinesNumber * 
      m_grid->getResolutionY() ) );
    
    te::rst::Grid* newGridPtr( new te::rst::Grid( newColsNumber,
      newLinesNumber, m_grid->getResolutionX(), m_grid->getResolutionY(), 
      &newULC, m_grid->getSRID() ) );
    delete( m_grid );
    m_grid = newGridPtr;
  }
  
  return true;
}

bool te::mem::ExpansibleRaster::addBottomLines( const unsigned int number )
{
  if( m_bands.empty() ) return false;
  
  if( number )
  {
    const unsigned int currExtraLines =  
      ( (unsigned int)( m_bands[ 0 ]->getProperty()->m_blkh *
      m_bands[ 0 ]->getProperty()->m_nblocksy ) ) - 
      m_grid->getNumberOfRows();
      
    te::gm::Coord2D uLC( 
      m_grid->getExtent()->getLowerLeftX(), 
      m_grid->getExtent()->getUpperRightY() );      
      
    if( currExtraLines < number )
    {
      const unsigned int blockExpansionSize = (unsigned int)std::ceil(
        ((double)number) / ((double)m_bands[ 0 ]->getProperty()->m_blkh) );
        
      std::vector< ExpansibleBandBlocksManager::BlockIndex3D > addedBlocksCoords;
        
      for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ; ++bandsIdx )
      {
        std::vector< ExpansibleBandBlocksManager::BlockIndex3D > bandAddedBlocksCoords;
        if( ! m_blocksManager.addBottomBlocks( blockExpansionSize, bandsIdx,
          bandAddedBlocksCoords ) ) 
          return false;
        
        addedBlocksCoords.insert( addedBlocksCoords.end(), bandAddedBlocksCoords.begin(),
          bandAddedBlocksCoords.end() );
        
        m_bands[ bandsIdx ]->getProperty()->m_nblocksy += blockExpansionSize;
      }
      
      dummyFillBlocks( addedBlocksCoords );
      
      const unsigned int newLinesNumber = 
        ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkh) *
        ((unsigned int)m_bands[ 0 ]->getProperty()->m_nblocksy );
      const unsigned int newColsNumber = 
        ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkw) *
        ((unsigned int)m_bands[ 0 ]->getProperty()->m_nblocksx );
      
      te::rst::Grid* newGridPtr( new te::rst::Grid( newColsNumber,
        newLinesNumber, m_grid->getResolutionX(), m_grid->getResolutionY(), 
        &uLC, m_grid->getSRID() ) );
      delete( m_grid );
      m_grid = newGridPtr;
    }
    else
    {
      te::rst::Grid* newGridPtr( new te::rst::Grid( 
        m_grid->getNumberOfColumns(),
        m_grid->getNumberOfRows() + number, 
        m_grid->getResolutionX(), m_grid->getResolutionY(), 
        &uLC, m_grid->getSRID() ) );
      delete( m_grid );
      m_grid = newGridPtr;
    }
  }
  
  return true;
}

bool te::mem::ExpansibleRaster::addLeftColumns( const unsigned int number )
{
  if( m_bands.empty() ) return false;
  
  if( number )
  {
    const unsigned int oldColsNumber = 
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkw) *
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_nblocksx );
      
    const unsigned int blockExpansionSize = (unsigned int)std::ceil(
      ((double)number) / ((double)m_bands[ 0 ]->getProperty()->m_blkw) );
      
    std::vector< ExpansibleBandBlocksManager::BlockIndex3D > addedBlocksCoords;
      
    for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ; ++bandsIdx )
    {
      std::vector< ExpansibleBandBlocksManager::BlockIndex3D > bandAddedBlocksCoords;
      if( ! m_blocksManager.addLeftBlocks( blockExpansionSize, bandsIdx,
        bandAddedBlocksCoords ) ) 
        return false;
      
      addedBlocksCoords.insert( addedBlocksCoords.end(), bandAddedBlocksCoords.begin(),
        bandAddedBlocksCoords.end() );
      
      m_bands[ bandsIdx ]->getProperty()->m_nblocksx += blockExpansionSize;
    }
    
    dummyFillBlocks( addedBlocksCoords );
    
    const unsigned int newLinesNumber = 
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkh) *
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_nblocksy );
    const unsigned int newColsNumber = 
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkw) *
      ((unsigned int)m_bands[ 0 ]->getProperty()->m_nblocksx );
      
    const double addedColsNumber = (double)( newColsNumber - oldColsNumber );
      
    te::gm::Coord2D newULC( 
      m_grid->getExtent()->getLowerLeftX() - ( addedColsNumber * 
      m_grid->getResolutionX() ), 
      m_grid->getExtent()->getUpperRightY() );
    
    te::rst::Grid* newGridPtr( new te::rst::Grid( newColsNumber,
      newLinesNumber, m_grid->getResolutionX(), m_grid->getResolutionY(), 
      &newULC, m_grid->getSRID() ) );
    delete( m_grid );
    m_grid = newGridPtr;
  }
  
  return true;
}

bool te::mem::ExpansibleRaster::addRightColumns( const unsigned int number )
{
  if( m_bands.empty() ) return false;
  
  if( number )
  {
    const unsigned int currExtraCols =  
      ( (unsigned int)( m_bands[ 0 ]->getProperty()->m_blkw *
      m_bands[ 0 ]->getProperty()->m_nblocksx ) ) - 
      m_grid->getNumberOfColumns();  
      
    te::gm::Coord2D uLC( 
      m_grid->getExtent()->getLowerLeftX(), 
      m_grid->getExtent()->getUpperRightY() );  
      
    if( currExtraCols < number )
    {
      const unsigned int blockExpansionSize = (unsigned int)std::ceil(
        ((double)number) / ((double)m_bands[ 0 ]->getProperty()->m_blkw) );
        
      std::vector< ExpansibleBandBlocksManager::BlockIndex3D > addedBlocksCoords;
        
      for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ; ++bandsIdx )
      {
        std::vector< ExpansibleBandBlocksManager::BlockIndex3D > bandAddedBlocksCoords;
        if( ! m_blocksManager.addRightBlocks( blockExpansionSize, bandsIdx,
          bandAddedBlocksCoords ) ) 
          return false;
        
        addedBlocksCoords.insert( addedBlocksCoords.end(), bandAddedBlocksCoords.begin(),
          bandAddedBlocksCoords.end() );
        
        m_bands[ bandsIdx ]->getProperty()->m_nblocksx += blockExpansionSize;
      }
      
      dummyFillBlocks( addedBlocksCoords );
      
      const unsigned int newLinesNumber = 
        ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkh) *
        ((unsigned int)m_bands[ 0 ]->getProperty()->m_nblocksy );
      const unsigned int newColsNumber = 
        ((unsigned int)m_bands[ 0 ]->getProperty()->m_blkw) *
        ((unsigned int)m_bands[ 0 ]->getProperty()->m_nblocksx );
      
      te::rst::Grid* newGridPtr( new te::rst::Grid( newColsNumber,
        newLinesNumber, m_grid->getResolutionX(), m_grid->getResolutionY(), 
        &uLC, m_grid->getSRID() ) );
      delete( m_grid );
      m_grid = newGridPtr;
    }
    else
    {
      te::rst::Grid* newGridPtr( new te::rst::Grid( 
        m_grid->getNumberOfColumns() + number,
        m_grid->getNumberOfRows(), 
        m_grid->getResolutionX(), m_grid->getResolutionY(), 
        &uLC, m_grid->getSRID() ) );
      delete( m_grid );
      m_grid = newGridPtr;
    }
  }
  
  return true;
}

bool te::mem::ExpansibleRaster::addTopBands( const unsigned int number )
{
  if( m_bands.empty() ) return false;
  
  if( number )
  {
    std::vector< ExpansibleBandBlocksManager::BlockIndex3D > addedBlocksCoords;
      
    if( ! m_blocksManager.addTopBands( number, addedBlocksCoords ) ) 
        return false;

    m_bands.insert( m_bands.begin(), number, 0 );
    for( unsigned int bIdx = 0 ; bIdx < m_bands.size() ; ++bIdx )
    {
      if( m_bands[ bIdx ] )
      {
        te::mem::ExpansibleBand* oldBandPtr = m_bands[ bIdx ];
        m_bands[ bIdx ] = new te::mem::ExpansibleBand( m_blocksManager, *this,
          *( oldBandPtr->getProperty() ), bIdx );
        delete( oldBandPtr );
      }
      else
      {
        m_bands[ bIdx ] = new te::mem::ExpansibleBand( m_blocksManager, *this,
          *( m_bands[ number ]->getProperty() ), bIdx );
      }
    }
    
    dummyFillBlocks( addedBlocksCoords );
  }
  
  return true;
}

bool te::mem::ExpansibleRaster::addBottomBands( const unsigned int number )
{
  if( m_bands.empty() ) return false;
  
  if( number )
  {
    std::vector< ExpansibleBandBlocksManager::BlockIndex3D > addedBlocksCoords;
      
    if( ! m_blocksManager.addBottomBands( number, addedBlocksCoords ) ) 
        return false;

    unsigned int lastBandIdx = (unsigned int)m_bands.size() - 1;
    m_bands.insert( m_bands.end(), number, 0 );
    for( unsigned int bIdx = lastBandIdx + 1 ; bIdx < m_bands.size() ; ++bIdx )
    {
      m_bands[ bIdx ] = new te::mem::ExpansibleBand( m_blocksManager, *this,
        *( m_bands[ lastBandIdx ]->getProperty() ), bIdx );
    }
    
    dummyFillBlocks( addedBlocksCoords );
  }
  
  return true;
}

void te::mem::ExpansibleRaster::free()
{
  if( m_bands.size() > 0 )
  {
    for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ; ++bandsIdx )
      delete (m_bands[ bandsIdx ]);
    m_bands.clear();
  }
  
  m_blocksManager.free();
}

void te::mem::ExpansibleRaster::dummyFillAllBlocks()
{
  for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ;
    ++bandsIdx )
  {
    te::mem::ExpansibleBand& band = *( m_bands[ bandsIdx ] );
    
    te::rst::GetBufferValueFPtr gb = 0;
    te::rst::GetBufferValueFPtr gbi = 0;
    te::rst::SetBufferValueFPtr sb = 0;
    te::rst::SetBufferValueFPtr sbi = 0;
    te::rst::SetBlockFunctions( &gb, &gbi, &sb, &sbi, band.getProperty()->m_type );
    
    const int elementsNumber = band.getProperty()->m_blkh *
       band.getProperty()->m_blkw;      
    double* noDataValuePtr = &( band.getProperty()->m_noDataValue );
    const unsigned int nBlocksX = band.getProperty()->m_nblocksx;
    const unsigned int nBlocksY = band.getProperty()->m_nblocksy;
    unsigned int blockXIdx = 0;
    unsigned int blockYIdx = 0;
    int eIdx = 0;
    void* blockPtr = 0;
    
    for( blockXIdx = 0 ; blockXIdx < nBlocksX ; ++blockXIdx )
    {
      for( blockYIdx = 0 ; blockYIdx < nBlocksY ; ++blockYIdx )
      {
        blockPtr = m_blocksManager.getBlockPointer( bandsIdx, blockXIdx, 
          blockYIdx );
        assert( blockPtr );
        
        for( eIdx = 0 ; eIdx < elementsNumber ; ++eIdx )
          sb( eIdx, blockPtr, noDataValuePtr );              
      }
    }
  }  
}

void te::mem::ExpansibleRaster::dummyFillBlocks( 
  const std::vector< ExpansibleBandBlocksManager::BlockIndex3D >& blocksCoords )
{
  for( unsigned int coodsIdx = 0 ; coodsIdx < blocksCoords.size() ;
    ++coodsIdx )
  {
    const ExpansibleBandBlocksManager::BlockIndex3D& block3DIdx = blocksCoords[ coodsIdx ];
    
    assert( block3DIdx.m_dim0Index < m_bands.size() );
    te::mem::ExpansibleBand& band = *( m_bands[ block3DIdx.m_dim0Index ] );
    
    te::rst::GetBufferValueFPtr gb = 0;
    te::rst::GetBufferValueFPtr gbi = 0;
    te::rst::SetBufferValueFPtr sb = 0;
    te::rst::SetBufferValueFPtr sbi = 0;
    
    te::rst::SetBlockFunctions( &gb, &gbi, &sb, &sbi, band.getProperty()->m_type );
    
    void* blockPtr = m_blocksManager.getBlockPointer( block3DIdx.m_dim0Index,
      block3DIdx.m_dim2Index, block3DIdx.m_dim1Index );
    
    const int elementsNumber = band.getProperty()->m_blkh *
      band.getProperty()->m_blkw;
      
    double* noDataValuePtr = &( band.getProperty()->m_noDataValue );
      
    for( int eIdx = 0 ; eIdx < elementsNumber ; ++eIdx )
      sb( eIdx, blockPtr, noDataValuePtr );
  }  
}


  
