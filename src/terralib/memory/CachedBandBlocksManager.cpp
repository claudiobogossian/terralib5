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

te::mem::CachedBandBlocksManager::CachedBandBlocksManager()
{
  m_rasterPtr = 0;
}

te::mem::CachedBandBlocksManager::CachedBandBlocksManager(
  const CachedBandBlocksManager& )
{
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

}

void te::mem::CachedBandBlocksManager::free()
{

}

void* te::mem::CachedBandBlocksManager::getBlockPointer(unsigned int band, 
  unsigned int x, unsigned int y)
{
  
}


