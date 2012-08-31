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
  \file terralib/memory/CachedRaster.cpp

  \brief A RAM cache adaptor to an external existent raster that must always be avaliable.
*/

// TerraLib
#include "../common/Translator.h"
// #include "../common/STLUtils.h"
// #include "../common/StringUtils.h"
// #include "../common/Translator.h"
// #include "../geometry/Envelope.h"
// #include "../raster/Grid.h"
// #include "../raster/RasterFactory.h"
// #include "../raster/Utils.h"
// #include "Band.h"
#include "Exception.h"
#include "CachedRaster.h"
#include "CachedBand.h"

// STL
//#include <cassert>
//#include <memory>

// Boost
//#include <boost/lexical_cast.hpp>

te::mem::CachedRaster::CachedRaster()
{
}

te::mem::CachedRaster::CachedRaster( te::rst::Grid* grid, te::common::AccessPolicy p )
: te::rst::Raster( grid, p )
{
}

te::mem::CachedRaster::CachedRaster( const Raster& rhs, 
  const unsigned char maxMemPercentUsed, 
  const unsigned int dataPrefetchThreshold )
: te::rst::Raster( rhs )
{
  if( ! m_blocksManager.initialize( rhs, maxMemPercentUsed, dataPrefetchThreshold) )
    throw Exception(TR_MEMORY("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < rhs.getNumberOfBands() ; 
    ++bandsIdx )
    m_bands.push_back( new te::mem::CachedBand( m_blocksManager, bandsIdx ) );
}

te::mem::CachedRaster::CachedRaster( const unsigned int maxNumberOfCacheBlocks,
  const Raster& rhs, 
  const unsigned int dataPrefetchThreshold )
: te::rst::Raster( rhs )
{
  if( ! m_blocksManager.initialize( maxNumberOfCacheBlocks, rhs, 
    dataPrefetchThreshold) )
    throw Exception(TR_MEMORY("Cannot initialize the blocks menager") );
  
  for( unsigned int bandsIdx = 0 ; bandsIdx < rhs.getNumberOfBands() ; 
    ++bandsIdx )
    m_bands.push_back( new te::mem::CachedBand( m_blocksManager, bandsIdx ) );
}

te::mem::CachedRaster::~CachedRaster()
{
  free();
}

void te::mem::CachedRaster::open(const std::map<std::string, std::string>& rinfo, 
  te::common::AccessPolicy p)
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
