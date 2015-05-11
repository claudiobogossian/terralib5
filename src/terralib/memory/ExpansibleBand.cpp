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
  \file terralib/memory/ExpansibleBand.cpp
 
  \brief Expansible raster band.
*/

// TerraLib
#include "../raster/BandProperty.h"
#include "../raster/Utils.h"
#include "ExpansibleBand.h"

// STL
#include <cstring>

te::mem::ExpansibleBandBlocksManager te::mem::ExpansibleBand::dummyBlocksManager;

te::mem::ExpansibleBand::ExpansibleBand( ExpansibleBandBlocksManager& blocksManager,
          te::rst::Raster& parentRaster, 
          const te::rst::BandProperty& bandProperty,  std::size_t idx )
: te::rst::Band( new te::rst::BandProperty( bandProperty ), idx ),
  m_blocksManager( blocksManager ),
  m_parentRasterPtr( &parentRaster ),
  m_blkWidth( (unsigned int)bandProperty.m_blkw ),
  m_blkHeight( (unsigned int)bandProperty.m_blkh ),
  m_blkSizeBytes( (unsigned int)( bandProperty.m_blkw * bandProperty.m_blkh * 
    te::rst::GetPixelSize( bandProperty.m_type ) ) )
{
  te::rst::SetBlockFunctions( &m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, 
    m_property->m_type );
}

te::mem::ExpansibleBand::ExpansibleBand()
: te::rst::Band( new te::rst::BandProperty( 0, 0 ), 0 ), 
  m_blocksManager( dummyBlocksManager )
{
  m_getBuff = 0;
  m_getBuffI = 0;
  m_setBuff = 0;
  m_setBuffI = 0;
}

te::mem::ExpansibleBand::~ExpansibleBand()
{
}

void te::mem::ExpansibleBand::getValue(unsigned int c, unsigned int r, double& value) const
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_getBuff(m_setGetPos, m_setGetBufPtr, &value );
}

void te::mem::ExpansibleBand::setValue(unsigned int c, unsigned int r, const double value)
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_setBuff(m_setGetPos, m_setGetBufPtr, &value );
}

void te::mem::ExpansibleBand::getIValue(unsigned int c, unsigned int r, double& value) const
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_getBuffI(m_setGetPos, m_setGetBufPtr, &value );
}

void te::mem::ExpansibleBand::setIValue(unsigned int c, unsigned int r, const double value)
{
  m_setGetBlkX = c / m_blkWidth;
  m_setGetBlkY = r / m_blkHeight;
  m_setGetPos = c % m_blkWidth + ((r % m_blkHeight) * m_blkWidth);
  assert(m_setGetPos < ( m_blkWidth * m_blkHeight ) );
  m_setGetBufPtr = m_blocksManager.getBlockPointer( m_idx, m_setGetBlkX, 
    m_setGetBlkY );
  m_setBuffI(m_setGetPos, m_setGetBufPtr, &value );
}

void te::mem::ExpansibleBand::read(int x, int y, void* buffer) const
{
  assert( m_blocksManager.isInitialized() );
  memcpy( buffer, m_blocksManager.getBlockPointer( m_idx, x, y ),
    m_blkSizeBytes );
}

void te::mem::ExpansibleBand::write(int x, int y, void* buffer)
{
  assert( m_blocksManager.isInitialized() );
  memcpy( m_blocksManager.getBlockPointer( m_idx, x, y ), buffer,
    m_blkSizeBytes );
}




