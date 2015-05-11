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
  \file terralib/sqlite/EWKBSize.cpp

  \brief A class that helps to determine the size of a SpatiaLite geometry.
*/

// TerraLib
#include "../common/Translator.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/LinearRing.h"
#include "../geometry/LineString.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/PointM.h"
#include "../geometry/PointZ.h"
#include "../geometry/PointZM.h"
#include "../geometry/Polygon.h"
#include "EWKBSize.h"

te::sqlite::EWKBSize::EWKBSize()
  : m_size(0)
{
}

te::sqlite::EWKBSize::~EWKBSize()
{
}

 std::size_t te::sqlite::EWKBSize::getEWKBSize(const te::gm::Geometry* geom)
{
  EWKBSize ws;

  ws.m_size = 44;

  geom->accept(ws);

  return ws.m_size;
}

void te::sqlite::EWKBSize::visit(const te::gm::GeometryCollection& visited)
{
  std::size_t ngeoms = visited.getNumGeometries();

  m_size += 4 + (5 * ngeoms);

  for(unsigned int i = 0; i < ngeoms; ++i)
    visited.getGeometryN(i)->accept(*this);
}

void te::sqlite::EWKBSize::visit(const te::gm::LinearRing& visited) 
{
  int dim = 2;

  if(visited.getZ())
    ++dim;

  if(visited.getM())
    ++dim;

  m_size += visited.getNPoints() * 8 * dim + 4;
}

void te::sqlite::EWKBSize::visit(const te::gm::LineString & visited) 
{
  int dim = 2;

  if(visited.getZ())
    ++dim;

  if(visited.getM())
    ++dim;

  m_size += visited.getNPoints() * 8 * dim + 4;
}

void te::sqlite::EWKBSize::visit(const te::gm:: MultiLineString& visited)
{
  visit((const te::gm::GeometryCollection&)visited);
}

void te::sqlite::EWKBSize::visit(const te::gm::MultiPoint& visited)
{
  visit((const te::gm::GeometryCollection&)visited);
}

void te::sqlite::EWKBSize::visit(const te::gm::MultiPolygon& visited)
{
  visit((const te::gm::GeometryCollection&)visited);
}

void te::sqlite::EWKBSize::visit(const te::gm::Point& /*visited*/) 
{
  m_size += 16;
}

void te::sqlite::EWKBSize::visit(const te::gm::PointM& /*visited*/) 
{
  m_size += 24;
}

void te::sqlite::EWKBSize::visit(const te::gm::PointZ& /*visited*/) 
{
  m_size += 24;
}

void te::sqlite::EWKBSize::visit(const te::gm::PointZM& /*visited*/) 
{
  m_size += 32;
}

void te::sqlite::EWKBSize::visit(const te::gm::Polygon& visited) 
{
  m_size += 4;

  std::size_t nrings = visited.getNumRings();

  for(std::size_t i = 0; i < nrings; ++i)
    visited.getRingN(i)->accept(*this);
}

