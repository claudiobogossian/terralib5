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
  \file terralib/raster/SynchronizedBand.cpp
 
  \brief Syncrhonized raster band.
*/

// TerraLib
#include "../raster/BandProperty.h"
#include "SynchronizedBand.h"

// STL
#include <cstring>

// initiating the dummy blocks manager
te::rst::SynchronizedBandBlocksManager te::rst::SynchronizedBand::dummyBlocksManager;

te::rst::SynchronizedBand::SynchronizedBand( SynchronizedBandBlocksManager& blocksManager, 
  SynchronizedRaster& raster, std::size_t idx )
  : te::rst::Band( new te::rst::BandProperty( 0, 0 ), idx ),
  m_blocksManager( blocksManager )
{
  blocksManager.getSynchronizer()->m_mutex.lock();
  
  m_syncRasterPtr = &raster;

  te::rst::Band::m_property = new te::rst::BandProperty( 
    *(blocksManager.getRaster()->getBand( idx )->getProperty()) );
  
  m_blkHeight = blocksManager.getRaster()->getBand( idx )->getProperty()->m_blkh;
  m_blkWidth = blocksManager.getRaster()->getBand( idx )->getProperty()->m_blkw;
  m_blkSizeBytes = blocksManager.getRaster()->getBand( idx )->getBlockSize();
  
  te::rst::SetBlockFunctions( &m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, 
    blocksManager.getRaster()->getBand( idx )->getProperty()->getType());
    
  blocksManager.getSynchronizer()->m_mutex.unlock();
}

te::rst::SynchronizedBand::SynchronizedBand()
  : te::rst::Band( new te::rst::BandProperty( 0, 0 ), 0 ),
  m_blocksManager( dummyBlocksManager )
{
  m_syncRasterPtr = 0;
  m_blkWidth = 0;
  m_blkHeight = 0;
  m_blkSizeBytes = 0;
  m_getBuff = 0;
  m_getBuffI = 0;
  m_setBuff = 0;
  m_setBuffI = 0;
}

te::rst::SynchronizedBand::~SynchronizedBand()
{
}

void te::rst::SynchronizedBand::getValue(unsigned int c, unsigned int r, double& value) const
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_getBuff(m_setGetPos, m_setGetBufPtr, &value );
}

void te::rst::SynchronizedBand::setValue(unsigned int c, unsigned int r, const double value)
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_setBuff(m_setGetPos, m_setGetBufPtr, &value );
}

void te::rst::SynchronizedBand::getIValue(unsigned int c, unsigned int r, double& value) const
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_getBuffI(m_setGetPos, m_setGetBufPtr, &value );
}

void te::rst::SynchronizedBand::setIValue(unsigned int c, unsigned int r, const double value)
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_setBuffI(m_setGetPos, m_setGetBufPtr, &value );
}

void te::rst::SynchronizedBand::read(int x, int y, void* buffer) const
{
  assert( m_blocksManager.isInitialized() );
  memcpy( buffer, m_blocksManager.getBlockPointer( m_idx, x, y ),
    m_blkSizeBytes );
}

void* te::rst::SynchronizedBand::read(int x, int y)
{
  assert( m_blocksManager.isInitialized() );
  return m_blocksManager.getBlockPointer( m_idx, x, y );  
}

void te::rst::SynchronizedBand::write(int x, int y, void* buffer)
{
  assert( m_blocksManager.isInitialized() );
  memcpy( m_blocksManager.getBlockPointer( m_idx, x, y ), buffer,
          m_blkSizeBytes );
}


