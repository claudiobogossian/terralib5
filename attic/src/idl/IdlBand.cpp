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
  \file terralib/idl/IdlBand.cpp
 
  \brief A adaptor to an external existent raster in a idl array form.
*/

// TerraLib
#include "IdlBand.h"
#include "Exception.h"
#include "../raster/BandProperty.h"
#include "../raster/Utils.h"

#include <cstring>

te::idl::IdlBand::IdlBand( const te::rst::Raster* rasterPtr, 
  const void* allBandsDataPtr, std::size_t idx, int dataType,
  const unsigned int nBands, const unsigned int nLines, const unsigned int nCols )
: te::rst::Band( new te::rst::BandProperty( idx, dataType, "" ), idx )
{
  m_property->m_blkh = nLines;
  m_property->m_blkw = nCols;
  m_property->m_colorInterp = te::rst::GrayIdxCInt;
  m_property->m_nblocksx = 1;
  m_property->m_nblocksy = 1;
  
  m_allBandsDataPtr = (void*)allBandsDataPtr;
  m_nLines = nLines;
  m_nCols = nCols;
  m_nBands = nBands;
  
  te::rst::SetBlockFunctions( &m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, 
    dataType );
}

te::idl::IdlBand::IdlBand()
: te::rst::Band( new te::rst::BandProperty( 0, 0 ), 0 )
{
  throw te::idl::Exception( "Not implemented" );
}

te::idl::IdlBand::IdlBand(const IdlBand& )
: te::rst::Band( new te::rst::BandProperty( 0, 0 ), 0 )
{
  throw te::idl::Exception( "Not implemented" );
}

te::idl::IdlBand::~IdlBand()
{
}

void te::idl::IdlBand::getValue(unsigned int c, unsigned int r, double& value) const
{
  m_bufferOffset = m_nBands * ( ( m_nCols * r ) + c ) + m_idx;
  m_getBuff(m_bufferOffset, m_allBandsDataPtr, &value );
}

void te::idl::IdlBand::setValue(unsigned int c, unsigned int r, const double value)
{
  m_bufferOffset = m_nBands * ( ( m_nCols * r ) + c ) + m_idx;
  m_setBuff(m_bufferOffset, m_allBandsDataPtr, &value );
}

void te::idl::IdlBand::getIValue(unsigned int c, unsigned int r, double& value) const
{
  m_bufferOffset = m_nBands * ( ( m_nCols * r ) + c ) + m_idx;
  m_getBuffI(m_bufferOffset, m_allBandsDataPtr, &value );
}

void te::idl::IdlBand::setIValue(unsigned int c, unsigned int r, const double value)
{
  m_bufferOffset = m_nBands * ( ( m_nCols * r ) + c ) + m_idx;
  m_setBuffI(m_bufferOffset, m_allBandsDataPtr, &value );
}

void te::idl::IdlBand::read(int x, int y, void* buffer) const
{
  assert( x == 0 );
  assert( y == 0 );
  memcpy( buffer, m_allBandsDataPtr, te::rst::GetPixelSize( m_property->m_type ) *
    m_nLines * m_nCols * m_nBands );
}

void te::idl::IdlBand::write(int x, int y, void* buffer)
{
  assert( x == 0 );
  assert( y == 0 );  
  memcpy( m_allBandsDataPtr, buffer, te::rst::GetPixelSize( m_property->m_type ) *
    m_nLines * m_nCols * m_nBands );
}




