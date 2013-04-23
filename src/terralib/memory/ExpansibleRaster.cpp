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
  \file terralib/memory/ExpansibleRaster.cpp

  \brief A raster (stored in memory and eventually swapped to disk) where it is possible to dynamically add and remove lines/columns.
*/

// TerraLib

#include "Exception.h"
#include "ExpansibleRaster.h"
#include "../raster/Utils.h"
#include "../common/Translator.h"
#include "../common/PlatformUtils.h"

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
  const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() ) / 
    2.0;
  const double freeVMem = ( ((double)maxMemPercentUsed) / 100.0 ) *
    std::min( totalPhysMem, ( ( totalVMem <= usedVMem ) ? 0.0 : ( totalVMem - usedVMem ) ) );  
  const unsigned int maxNumberOfBlocks = (unsigned int)
    std::max( 1.0, std::ceil( freeVMem / ((double)maxBlockSizeBytes) ) );    
  
  if( ! m_blocksManager.initialize( maxNumberOfBlocks, numbersOfBlocksX,
    numbersOfBlocksY, blocksSizesBytes, 
    TLINTERNAL_EXPANSIBLERASTER_MAXDISKFILESSIZE ) )
    throw Exception(TR_MEMORY("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < bandsProperties.size() ; 
    ++bandsIdx )
  {
    m_bands.push_back( new te::mem::ExpansibleBand( m_blocksManager, *this,
      *(bandsProperties[ bandsIdx ]) , bandsIdx ) );
    delete ( bandsProperties[ bandsIdx ] );
  }
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
    throw Exception(TR_MEMORY("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < bandsProperties.size() ; 
    ++bandsIdx )
  {
    m_bands.push_back( new te::mem::ExpansibleBand( m_blocksManager, *this,
      *(bandsProperties[ bandsIdx ]) , bandsIdx ) );
    delete ( bandsProperties[ bandsIdx ] );
  }
}

te::mem::ExpansibleRaster::~ExpansibleRaster()
{
  free();
}

void te::mem::ExpansibleRaster::open(const std::map<std::string, std::string>& rinfo, 
  te::common::AccessPolicy p)
{
}

std::map<std::string, std::string> te::mem::ExpansibleRaster::getInfo() const
{
  return std::map<std::string, std::string>();
};

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
  if( number )
  {
   
    
  }
  
  return false;
}

bool te::mem::ExpansibleRaster::addBottomLines( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::addLeftColumns( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::addRightColumns( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::addTopBands( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::addBottomBands( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::removeTopLines( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::removeBottomLines( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::removeLeftColumns( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::removeRightColumns( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::removeTopBands( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

bool te::mem::ExpansibleRaster::removeBottomBands( const unsigned int number )
{
  if( number )
  {
   
    
  }
    
  return false;
}

void te::mem::ExpansibleRaster::free()
{
  if( m_bands.size() > 0 )
  {
    for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ; ++bandsIdx )
      delete m_bands[ bandsIdx ];
    m_bands.clear();
  }
  
  m_blocksManager.free();
}
