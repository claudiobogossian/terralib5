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
  \file terralib/raster/TileIndexer.cpp

  \brief Polygon tile indexing class for optmized geometrical relational tests.
*/

// TerraLib
#include "TileIndexer.h"
#include "Exception.h"
#include "../geometry.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"

// STL
#include <iostream>
#include <memory>

bool te::rst::TileIndexer::getTileIndex(const te::gm::Point& p1, const te::gm::Point& p2,
                                        unsigned int& firstTile, unsigned int& lastTile) const
{
// Invalid tile indexer Y resolution
  assert(m_dy > 0);

  double lowerY = ((p1.getY() < p2.getY())? p1.getY(): p2.getY());
  double upperY = ((p1.getY() > p2.getY())? p1.getY(): p2.getY());
  
  if( ( lowerY < m_referencePolygon.getMBR()->getLowerLeftY() ) ||
    ( upperY > m_referencePolygon.getMBR()->getUpperRightY() ) )
  {
    return false;
  }
  else
  {
    firstTile = (unsigned int)((lowerY - m_referencePolygon.getMBR()->getLowerLeftY()) / m_dy);
    lastTile  = (unsigned int)((upperY - m_referencePolygon.getMBR()->getLowerLeftY()) / m_dy);
    return true;
  }
}

bool te::rst::TileIndexer::getTileIndex(const double& y, unsigned int& tileIndex) const
{
  assert(m_dy > 0);
  
  if( ( y < m_referencePolygon.getMBR()->getLowerLeftY() ) ||
    ( y > m_referencePolygon.getMBR()->getUpperRightY() ) )
  {
    return false;
  }
  else
  {
    tileIndex = (unsigned int)( (y - m_referencePolygon.getMBR()->getLowerLeftY() ) / m_dy );
    return true;
  }
}

void te::rst::TileIndexer::init()
{
  m_dy = 0;
}

te::rst::TileIndexer::TileIndexer(const te::gm::Polygon& pol, const double& dy)
  : m_referencePolygon(pol)
{
  assert(dy > 0);

  init();
  m_dy = dy;

// Building new index
  if (m_referencePolygon.getNumRings() > 0)
  {
    unsigned int total_tiles_number = 1 +
      ((unsigned int) floor(m_referencePolygon.getMBR()->getHeight() / m_dy));

    for(unsigned int i = 0; i < total_tiles_number; i++)
      m_tileIndex.push_back(new TileSegIndex);

// for each polygon ring, we need to index its segments
    for(unsigned int i = 0; i < m_referencePolygon.getNumRings(); i++)
    {
      if( ! addRing(i) )
      {
        throw Exception(TE_TR("Error adding ring") );
      }
    }
  }
}

void te::rst::TileIndexer::clear()
{
  te::common::FreeContents(m_tileIndex);
  m_tileIndex.clear();
  init();
}

te::rst::TileIndexer::~TileIndexer()
{
  clear();
}

bool te::rst::TileIndexer::addRing(const unsigned int& ri)
{
  if( ri >= m_referencePolygon.getNumRings() )
  {
    return false;
  }

  unsigned int num_points = m_referencePolygon[ri]->getNPoints();
   if (num_points < 2)
  {
    return false;
  }
  
  unsigned int numSegments = num_points - 1;

  for(unsigned int j = 0; j < numSegments; j++)
  {
// creates a pointer to the segment
    std::pair<unsigned int, unsigned int> segPointer(ri, j);

// finds the tiles that this segment intersects
    unsigned int firstTileIndex = 0;
    unsigned int lastTileIndex = 0;

    std::auto_ptr< te::gm::Point > cj( 
      (static_cast<te::gm::LinearRing*> (m_referencePolygon[ri]))->getPointN(j) );
    std::auto_ptr< te::gm::Point > cjp1(
      (static_cast<te::gm::LinearRing*> (m_referencePolygon[ri]))->getPointN(j + 1) );
    
    if( getTileIndex(*cj, *cjp1, firstTileIndex, lastTileIndex) )
    {
      // associates the pointer segment to the tiles
      for(unsigned int k = firstTileIndex; k <= lastTileIndex; k++)
        m_tileIndex[k]->push_back(segPointer);
    }
    else
    {
      return false;
    }
  }
    
  return true;
}

bool te::rst::TileIndexer::getTile(const double& y, TileSegIndex** index) const
{
  unsigned int tidx;
  if( getTileIndex(y, tidx) )
  {
    assert( tidx < m_tileIndex.size() );
    
    (*index) = m_tileIndex[tidx];
    return true;
  }
  else
  {
    return false;
  }
}

bool te::rst::TileIndexer::within_or_touches(const te::gm::Point& geometry) const
{
  m_withinTileY = geometry.getY();
  
  if( getTile( m_withinTileY, &m_withinTileIndexPtr ) )
  {
    assert( m_withinTileIndexPtr );
    
    m_withinTileX = geometry.getX();
    m_withinIsInside = false;
    
    for( unsigned int i = 0 ; i < m_withinTileIndexPtr->size() ; ++i ) 
    {
      assert( ( ( *m_withinTileIndexPtr )[ i ].first < m_referencePolygon.getNumRings() ) );
      assert( dynamic_cast< te::gm::LinearRing const* >( m_referencePolygon[ 
        ( *m_withinTileIndexPtr )[ i ].first ] ) );      
      m_withinRingPtr = (te::gm::LinearRing const*)m_referencePolygon[ ( *m_withinTileIndexPtr )[ i ].first ];
      
      assert( ( *m_withinTileIndexPtr )[ i ].second  < m_withinRingPtr->getNPoints() );    
      m_withinVtx0.x = m_withinRingPtr->getX( ( *m_withinTileIndexPtr )[ i ].second );
      m_withinVtx0.y = m_withinRingPtr->getY( ( *m_withinTileIndexPtr )[ i ].second );
      m_withinVtx1.x = m_withinRingPtr->getX( ( *m_withinTileIndexPtr )[ i ].second + 1 );
      m_withinVtx1.y = m_withinRingPtr->getY( ( *m_withinTileIndexPtr )[ i ].second + 1 );
      
      m_withinYFlag0 = (m_withinVtx0.y >= m_withinTileY);
      m_withinYFlag1 = (m_withinVtx1.y >= m_withinTileY);
      m_withinYEquals = (m_withinVtx0.y == m_withinTileY) && (m_withinVtx1.y == m_withinTileY);
    
      double m_within_coef1 = (m_withinVtx1.y - m_withinTileY) * (m_withinVtx0.x - m_withinVtx1.x);
      double m_within_coef2 = (m_withinVtx1.x - m_withinTileX) * (m_withinVtx0.y - m_withinVtx1.y);
      
      if(m_withinYFlag0 != m_withinYFlag1)
      {
        if (m_within_coef1 - m_within_coef2 == 0)
        {
          return true;
        }
        else if ((m_within_coef1 >= m_within_coef2) == m_withinYFlag1) {
          m_withinIsInside = !m_withinIsInside;
        }
      }
      else if (m_withinYEquals)
      {
        if ((m_withinVtx0.x >= m_withinTileX) == (m_withinVtx1.x <= m_withinTileX)) {
          return true;
        }
      }
    }
    
    return m_withinIsInside;
  }
  else
  {
    return false;
  }
}

