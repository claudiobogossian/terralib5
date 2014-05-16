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
  \file terralib/raster/TileIndexer.cpp

  \brief Polygon tile indexing class for optmized geometrical relational tests.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../geometry.h"
#include "TileIndexer.h"

// STL
#include <iostream>

void te::rst::TileIndexer::getTileIndex(const te::gm::Point& p1, const te::gm::Point& p2,
                                        unsigned int& firstTile, unsigned int& lastTile) const
{
// Invalid tile indexer Y resolution
  assert(m_dy > 0);

  double lowerY = ((p1.getY() < p2.getY())? p1.getY(): p2.getY());
  double upperY = ((p1.getY() > p2.getY())? p1.getY(): p2.getY());

  firstTile = int((lowerY - m_referencePolygon.getMBR()->getLowerLeftY()) / m_dy);
  lastTile  = int((upperY - m_referencePolygon.getMBR()->getLowerLeftY()) / m_dy);
}

void te::rst::TileIndexer::getTileIndex(const double& y, unsigned int& tileIndex) const
{
  assert(m_dy > 0);

  tileIndex = int( (y - m_referencePolygon.getMBR()->getLowerLeftY() ) / m_dy );
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
      addRing(i);
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

void te::rst::TileIndexer::addRing(const unsigned int& ri)
{
  assert(ri < m_referencePolygon.getNumRings());

  unsigned int num_points = m_referencePolygon[ri]->getNPoints();

  if (num_points > 0)
  {
    unsigned int numSegments = num_points - 1;

    for(unsigned int j = 0; j < numSegments; j++)
    {
// creates a pointer to the segment
      std::pair<unsigned int, unsigned int> segPointer(ri, j);

// finds the tiles that this segment intersects
      unsigned int firstTileIndex = 0;
      unsigned int lastTileIndex = 0;

      te::gm::Point* cj = (static_cast<te::gm::LinearRing*> (m_referencePolygon[ri]))->getPointN(j);
      te::gm::Point* cjp1 = (static_cast<te::gm::LinearRing*> (m_referencePolygon[ri]))->getPointN(j + 1);
      getTileIndex(*cj, *cjp1, firstTileIndex, lastTileIndex);

      assert(firstTileIndex < m_tileIndex.size());
      assert(lastTileIndex < m_tileIndex.size());

// associates the pointer segment to the tiles
      for(unsigned int k = firstTileIndex; k <= lastTileIndex; k++)
        m_tileIndex[k]->push_back(segPointer);
    }
  }
}

void te::rst::TileIndexer::getTile(const double& y, TileSegIndex** index) const
{
  unsigned int tidx;
  getTileIndex(y, tidx);

  if (tidx < m_tileIndex.size())
    (*index) = m_tileIndex[tidx];
  else
    (*index) = 0;
}

const te::gm::Polygon& te::rst::TileIndexer::getPolygon() const
{
  return m_referencePolygon;
}
