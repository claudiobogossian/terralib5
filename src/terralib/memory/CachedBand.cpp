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
  \file terralib/memory/CachedBand.cpp
 
  \brief RAM cached and tiled raster band.
*/

// TerraLib
#include "CachedBand.h"
#include "../raster/BandProperty.h"

te::mem::CachedBand::CachedBand( CachedBandBlocksManager& blocksManager,
  std::size_t idx )
: te::rst::Band( new te::rst::BandProperty( 
  *(getRaster()->getBand( idx )->getProperty()) ), idx ), 
  m_idx( idx ), m_blocksManager( blocksManager )
{
}

te::mem::CachedBand::CachedBand()
: te::rst::Band( new te::rst::BandProperty( 0, 0 ), 0 ),
  m_idx( 0 ), m_blocksManager( m_dummyBlocksManager )
{

}

te::mem::CachedBand::CachedBand(const CachedBand& )
: te::rst::Band( new te::rst::BandProperty( 0, 0 ), 0 ),
  m_idx( 0 ), m_blocksManager( m_dummyBlocksManager )
{
}

te::mem::CachedBand::~CachedBand()
{
}

void te::mem::CachedBand::getValue(unsigned int c, unsigned int r, double& value) const
{

}

void te::mem::CachedBand::setValue(unsigned int c, unsigned int r, const double value)
{

}

void te::mem::CachedBand::getIValue(unsigned int c, unsigned int r, double& value) const
{

}

void te::mem::CachedBand::setIValue(unsigned int c, unsigned int r, const double value)
{

}



