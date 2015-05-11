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
  \file terralib/memory/CachedBand.cpp
 
  \brief RAM cached and tiled raster band.
*/

// TerraLib
#include "../raster/BandProperty.h"
#include "CachedBand.h"

// STL
#include <cstring>

te::mem::CachedBandBlocksManager te::mem::CachedBand::dummyBlocksManager;

te::mem::CachedBand::CachedBand( CachedBandBlocksManager& blocksManager,
                                     std::size_t idx )
  : te::rst::Band( new te::rst::BandProperty( 
  *(blocksManager.getRaster()->getBand( idx )->getProperty()) ), idx ),
  m_blocksManager( blocksManager )
{
  m_blkHeight = blocksManager.getRaster()->getBand( idx )->getProperty()->m_blkh;
  m_blkWidth = blocksManager.getRaster()->getBand( idx )->getProperty()->m_blkw;
  m_blkSizeBytes = blocksManager.getRaster()->getBand( idx )->getBlockSize();
  
  te::rst::SetBlockFunctions( &m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, 
    blocksManager.getRaster()->getBand( idx )->getProperty()->getType());
}

te::mem::CachedBand::CachedBand()
  : te::rst::Band( new te::rst::BandProperty( 0, 0 ), 0 ), m_blocksManager( dummyBlocksManager )
{
  m_blkWidth = 0;
  m_blkHeight = 0;
  m_blkSizeBytes = 0;
  m_getBuff = 0;
  m_getBuffI = 0;
  m_setBuff = 0;
  m_setBuffI = 0;
}

te::mem::CachedBand::~CachedBand()
{
}

void te::mem::CachedBand::getValue(unsigned int c, unsigned int r, double& value) const
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_getBuff(m_setGetPos, m_setGetBufPtr, &value );
}

void te::mem::CachedBand::setValue(unsigned int c, unsigned int r, const double value)
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_setBuff(m_setGetPos, m_setGetBufPtr, &value );
}

void te::mem::CachedBand::getIValue(unsigned int c, unsigned int r, double& value) const
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_getBuffI(m_setGetPos, m_setGetBufPtr, &value );
}

void te::mem::CachedBand::setIValue(unsigned int c, unsigned int r, const double value)
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_setBuffI(m_setGetPos, m_setGetBufPtr, &value );
}

void te::mem::CachedBand::read(int x, int y, void* buffer) const
{
  assert( m_blocksManager.isInitialized() );
  memcpy( buffer, m_blocksManager.getBlockPointer( m_idx, x, y ),
    m_blkSizeBytes );
}

void te::mem::CachedBand::write(int x, int y, void* buffer)
{
  assert( m_blocksManager.isInitialized() );
  memcpy( m_blocksManager.getBlockPointer( m_idx, x, y ), buffer,
          m_blkSizeBytes );
}


