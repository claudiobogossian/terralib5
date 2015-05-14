/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/memory/CachedRaster.cpp

  \brief A RAM cache adaptor to an external existent raster that must always be avaliable.
*/

// TerraLib
#include "../common/Translator.h"
#include "CachedBand.h"
#include "CachedRaster.h"
#include "Exception.h"
#include "../raster/Grid.h"

te::mem::CachedRaster::CachedRaster()
{
}

te::mem::CachedRaster::CachedRaster( te::rst::Grid* grid, te::common::AccessPolicy p )
: te::rst::Raster( grid, p )
{
}

te::mem::CachedRaster::CachedRaster( const te::rst::Raster& rhs, 
  const unsigned char maxMemPercentUsed, 
  const unsigned int dataPrefetchThreshold )
: te::rst::Raster( new te::rst::Grid( *rhs.getGrid() ), rhs.getAccessPolicy() )
{
  if( ! m_blocksManager.initialize( rhs, maxMemPercentUsed, dataPrefetchThreshold) )
    throw Exception(TE_TR("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < rhs.getNumberOfBands() ; 
    ++bandsIdx )
    m_bands.push_back( new te::mem::CachedBand( m_blocksManager, bandsIdx ) );
}

te::mem::CachedRaster::CachedRaster( const unsigned int maxNumberOfCacheBlocks,
  const te::rst::Raster& rhs, 
  const unsigned int dataPrefetchThreshold )
: te::rst::Raster( new te::rst::Grid( *rhs.getGrid() ), rhs.getAccessPolicy() )
{
  if( ! m_blocksManager.initialize( maxNumberOfCacheBlocks, rhs, 
    dataPrefetchThreshold) )
    throw Exception(TE_TR("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < rhs.getNumberOfBands() ; 
    ++bandsIdx )
    m_bands.push_back( new te::mem::CachedBand( m_blocksManager, bandsIdx ) );
}

te::mem::CachedRaster::~CachedRaster()
{
  free();
}

void te::mem::CachedRaster::open(const std::map<std::string, std::string>& /*rinfo*/,
                                     te::common::AccessPolicy /*p*/)
{
}

te::dt::AbstractData* te::mem::CachedRaster::clone() const
{
  assert( m_blocksManager.isInitialized() );
  return new CachedRaster( m_blocksManager.getMaxNumberOfCacheBlocks(),
    *m_blocksManager.getRaster(), m_blocksManager.getDataPrefetchThreshold() );
}

void te::mem::CachedRaster::free()
{
  if( m_bands.size() > 0 )
  {
    for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ; ++bandsIdx )
      delete m_bands[ bandsIdx ];
    m_bands.clear();
  }
  
  m_blocksManager.free();
}
