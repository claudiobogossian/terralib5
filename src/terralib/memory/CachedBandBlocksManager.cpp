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
  
  for( unsigned int bandIdx = 0 ; bandIdx < externalRaster.getNumberOfBands() ;
    ++bandIdx )
  {
    if( m_globalBlocksNumberX < externalRaster.getBand( bandIdx )->getProperty()->m_nblocksx )
      m_globalBlocksNumberX = externalRaster.getBand( bandIdx )->getProperty()->m_nblocksx;
      
    if( m_globalBlocksNumberY < externalRaster.getBand( bandIdx )->getProperty()->m_nblocksy )
      m_globalBlocksNumberY = externalRaster.getBand( bandIdx )->getProperty()->m_nblocksy;
      
    if( m_globalBlockSizeBytes < externalRaster.getBand( bandIdx )->getBlockSize() )
      m_globalBlockSizeBytes = externalRaster.getBand( bandIdx )->getBlockSize();
  }
  
  // Finding the max number of memory blocks
  
  const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
  const double usedVMem = (double)te::common::GetUsedVirtualMemory();
  const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() ) / 
    2.0;
  const double freeVMem = ( ((double)maxMemPercentUsed) / 100.0 ) *
    std::min( totalPhysMem, ( ( totalVMem <= usedVMem ) ? 0.0 : ( totalVMem - usedVMem ) ) );  
  const unsigned int maxNumberOfMemBlocks = (unsigned int)std::max( 1.0, std::ceil( freeVMem /
    ((double)m_globalBlockSizeBytes) ) );
    
  // Allocating the internal structures
  
  unsigned int blockBIdx = 0;
  unsigned int blockYIdx = 0;
  unsigned int blockXIdx = 0; 
  
  m_blocksPointers.resize( externalRaster.getNumberOfBands() );
  
  for( blockBIdx = 0 ; blockBIdx < m_blocksPointers.size() ;  ++blockBIdx )
  {
    m_blocksPointers[ blockBIdx ].resize( m_globalBlocksNumberY );
    
    for( unsigned int blockYIdx = 0 ; blockYIdx < m_globalBlocksNumberY ;
      ++blockYIdx )
    {
      m_blocksPointers[ blockBIdx ][ blockYIdx ].resize( m_globalBlocksNumberX );
      
      for( blockXIdx = 0 ; blockXIdx < m_globalBlocksNumberX ; ++blockXIdx )
      {
        m_blocksPointers[ blockBIdx ][ blockYIdx ][ blockXIdx ] = 0;
      }
    }
  }
}

void te::mem::CachedBandBlocksManager::free()
{
  m_blocksPointers.clear();
  
  initState();
}

void* te::mem::CachedBandBlocksManager::getBlockPointer(unsigned int band, 
  unsigned int x, unsigned int y)
{
  
}


