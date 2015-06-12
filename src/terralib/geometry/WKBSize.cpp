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
  \file terralib/geometry/WKBSize.cpp

  \brief A class that serializes a geometry to the WKB format.
*/

// TerraLib
#include "GeometryCollection.h"
#include "LinearRing.h"
#include "MultiLineString.h"
#include "MultiPoint.h"
#include "MultiPolygon.h"
#include "PointM.h"
#include "PointZ.h"
#include "PointZM.h"
#include "Polygon.h"
#include "PolyhedralSurface.h"
#include "WKBSize.h"

// STL
#include <cassert>

te::gm::WKBSize::WKBSize()
  : m_size(0)
{
}

te::gm::WKBSize::~WKBSize()
{
}

std::size_t te::gm::WKBSize::size(const Geometry* geom)
{
  WKBSize w;

  geom->accept(w);

  return w.m_size;
}

std::size_t te::gm::WKBSize::size(const Geometry& geom)
{
  WKBSize w;

  geom.accept(w);

  return w.m_size;
}

void te::gm::WKBSize::visit(const GeometryCollection& visited)
{
  m_size += 9; // byte order + type + ngeoms

  std::size_t nGeoms = visited.getNumGeometries();
  
  for(std::size_t i = 0; i < nGeoms; ++i)
  {
    assert(visited.getGeometryN(i) != 0);
    visited.getGeometryN(i)->accept(*this);
  }
}

void te::gm::WKBSize::visit(const LinearRing& visited) 
{
  visit((const LineString&)visited);
}

void te::gm::WKBSize::visit(const LineString& visited) 
{
  const std::size_t m_nPts = visited.size();

// byte order + geom type + npts
  m_size += 9 + (16 * m_nPts);

  if(visited.getZ())
    m_size += (8 * m_nPts);

  if(visited.getM())
    m_size += (8 * m_nPts);
}

void te::gm::WKBSize::visit(const MultiLineString& visited)
{
  visit((const GeometryCollection&)visited);
}

void te::gm::WKBSize::visit(const MultiPoint& visited)
{
  visit((const GeometryCollection&)visited);
}

void te::gm::WKBSize::visit(const MultiPolygon& visited)
{
  visit((const GeometryCollection&)visited);
}

void te::gm::WKBSize::visit(const Point& /*visited*/) 
{
  m_size += 21;
}

void te::gm::WKBSize::visit(const PointM& /*visited*/) 
{
  m_size += 29;
}

void te::gm::WKBSize::visit(const PointZ& /*visited*/) 
{
  m_size += 29;
}

void te::gm::WKBSize::visit(const PointZM& /*visited*/) 
{
  m_size += 37;
}

void te::gm::WKBSize::visit(const Polygon& visited) 
{
  m_size += 9; // byte order + type + nrings

  std::size_t nComponents = visited.getCoordinateDimension();
  std::size_t nRings = visited.getNumRings();
  
  for(std::size_t i = 0; i < nRings; ++i)
    m_size += ((visited.getRingN(i)->getNPoints() * nComponents * 8) + 4);
}

void te::gm::WKBSize::visit(const PolyhedralSurface& visited) 
{
  m_size += 9; // byte order + type + npols

  std::size_t nPols = visited.getNumPatches();
  
  for(std::size_t i = 0; i < nPols; ++i)
    visit(*(visited.getPatchN(i)));
}

void te::gm::WKBSize::visit(const TIN& visited)
{
  visit((const PolyhedralSurface&)visited);
}

void te::gm::WKBSize::visit(const Triangle& visited)
{
  visit((const Polygon&)visited);
}

