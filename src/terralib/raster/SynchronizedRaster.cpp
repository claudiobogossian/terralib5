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
  \file terralib/raster/ProxyRaster.cpp

  \brief A proxy class for bands of several rasters over the same region.
*/

// TerraLib
#include "SynchronizedRaster.h"
#include "../common/Translator.h"
#include "BandProperty.h"
#include "Exception.h"
#include "Grid.h"

// STL
#include <cassert>
#include <utility>

te::rst::SynchronizedRaster::SynchronizedRaster()
{
}

te::rst::SynchronizedRaster::SynchronizedRaster( te::rst::Grid* grid, te::common::AccessPolicy p )
: te::rst::Raster( grid, p )
{
}

te::rst::SynchronizedRaster::SynchronizedRaster( RasterSynchronizer& sync, 
  const unsigned char maxMemPercentUsed )
: te::rst::Raster()
{
  sync.m_mutex.lock();
  m_grid = new te::rst::Grid( *sync.m_raster.getGrid() );
  sync.m_mutex.unlock();
  
  if( ! m_blocksManager.initialize( sync, maxMemPercentUsed ) )
    throw Exception(TE_TR("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < sync.m_raster.getNumberOfBands() ; 
    ++bandsIdx )
    m_bands.push_back( new te::rst::SynchronizedBand( m_blocksManager, *this,
      bandsIdx ) );
}

te::rst::SynchronizedRaster::SynchronizedRaster( const unsigned int maxNumberOfCacheBlocks,
  RasterSynchronizer& sync )
: te::rst::Raster()
{
  sync.m_mutex.lock();
  m_grid = new te::rst::Grid( *sync.m_raster.getGrid() );
  sync.m_mutex.unlock();
  
  if( ! m_blocksManager.initialize( maxNumberOfCacheBlocks, sync ) )
    throw Exception(TE_TR("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < sync.m_raster.getNumberOfBands() ; 
    ++bandsIdx )
    m_bands.push_back( new te::rst::SynchronizedBand( m_blocksManager, *this,
      bandsIdx ) );
}

te::rst::SynchronizedRaster::~SynchronizedRaster()
{
  free();
}

std::map<std::string, std::string> te::rst::SynchronizedRaster::getInfo() const
{
  std::map<std::string, std::string> info;
  
  m_blocksManager.getSynchronizer()->m_mutex.lock();
  assert( m_blocksManager.getRaster() );
  info = m_blocksManager.getRaster()->getInfo();
  m_blocksManager.getSynchronizer()->m_mutex.unlock();
  
  return info;
}

int te::rst::SynchronizedRaster::getBandDataType(std::size_t i) const
{
  int returnValue = 0;
  
  m_blocksManager.getSynchronizer()->m_mutex.lock();
  assert( m_blocksManager.getRaster() );
  returnValue = m_blocksManager.getRaster()->getBandDataType( i );
  m_blocksManager.getSynchronizer()->m_mutex.unlock();
  
  return returnValue;
};

void te::rst::SynchronizedRaster::open(const std::map<std::string, std::string>& /*rinfo*/,
  te::common::AccessPolicy /*p*/)
{
}

te::dt::AbstractData* te::rst::SynchronizedRaster::clone() const
{
  assert( m_blocksManager.isInitialized() );
  return new SynchronizedRaster( m_blocksManager.getMaxNumberOfCacheBlocks(),
    *m_blocksManager.getSynchronizer() );
}

void te::rst::SynchronizedRaster::free()
{
  if( m_bands.size() > 0 )
  {
    for( unsigned int bandsIdx = 0 ; bandsIdx < m_bands.size() ; ++bandsIdx )
      delete m_bands[ bandsIdx ];
    m_bands.clear();
  }
  
  m_blocksManager.free();
}
