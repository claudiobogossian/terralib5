/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/edit/core/Utils.cpp
   
  \brief Utility functions for TerraLib Edit module.
*/

// TerraLib
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/LineString.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "Utils.h"

// STL
#include <cassert>

void te::edit::GetLines(te::gm::Geometry* geom, std::vector<te::gm::LineString*>& lines)
{
  if(geom == 0)
    return;

  switch(geom->getGeomTypeId())
  {
    case te::gm::MultiPolygonType:
    case te::gm::MultiLineStringType:
    case te::gm::MultiPointType:
      GetLines(dynamic_cast<te::gm::GeometryCollection*>(geom), lines);
    break;

    case te::gm::PolygonType:
      GetLines(dynamic_cast<te::gm::Polygon*>(geom), lines);
    break;

    case te::gm::LineStringType:
      GetLines(dynamic_cast<te::gm::LineString*>(geom), lines);
    break;

    default:
      return;
  }
}

void te::edit::GetLines(te::gm::GeometryCollection* gc, std::vector<te::gm::LineString*>& lines)
{
  assert(gc);

  for(std::size_t i = 0; i < gc->getNumGeometries(); ++i)
    GetLines(gc->getGeometryN(i), lines);
}

void te::edit::GetLines(te::gm::Polygon* p, std::vector<te::gm::LineString*>& lines)
{
  assert(p);

  std::vector<te::gm::Curve*>& rings = p->getRings();

  for(std::size_t i = 0; i < rings.size(); ++i) // for each ring
    GetLines(dynamic_cast<te::gm::LineString*>(rings[i]), lines);
}

void te::edit::GetLines(te::gm::LineString* l, std::vector<te::gm::LineString*>& lines)
{
  assert(l);
  lines.push_back(l);
}

void te::edit::MoveVertex(std::vector<te::gm::LineString*>& lines, const VertexIndex& index, const double& x, const double& y)
{
  assert(index.isValid());
  assert(index.m_line < lines.size());

  te::gm::LineString* l = lines[index.m_line];

  assert(index.m_pos < l->getNPoints());

  l->setPoint(index.m_pos, x, y);
}

void te::edit::RemoveVertex(std::vector<te::gm::LineString*>& lines, const VertexIndex& index)
{
  assert(index.isValid());
  assert(index.m_line < lines.size());

  te::gm::LineString* currentLine = lines[index.m_line];

  te::gm::LineString* newLine = new te::gm::LineString(currentLine->getNPoints() - 1,
                                                       currentLine->getGeomTypeId(), currentLine->getSRID());

  std::size_t pos = 0;
  for(std::size_t i = 0; i < currentLine->getNPoints(); ++i)
  {
    if(i == index.m_pos)
      continue;

    newLine->setPoint(pos, currentLine->getX(i), currentLine->getY(i));
    ++pos;
  }

  // Copy the new line
  *currentLine = *newLine;

  delete newLine;
}
