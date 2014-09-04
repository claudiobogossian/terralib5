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
  \file terralib/edit/Utils.cpp
   
  \brief Utility functions for TerraLib Edit module.
*/

// TerraLib
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/LineString.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../geometry/Utils.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

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

  if(IsSpecialRingVertex(l, index))
  {
    l->setPoint(0, x, y);
    l->setPoint(l->getNPoints() - 1, x, y);
  }
  else
    l->setPoint(index.m_pos, x, y);
}

void te::edit::RemoveVertex(std::vector<te::gm::LineString*>& lines, const VertexIndex& index)
{
  assert(index.isValid());
  assert(index.m_line < lines.size());

  te::gm::LineString* currentLine = lines[index.m_line];

  te::gm::LineString* newLine = new te::gm::LineString(currentLine->getNPoints() - 1, currentLine->getGeomTypeId(), currentLine->getSRID());

  std::size_t pos = 0;
  for(std::size_t i = 0; i < currentLine->getNPoints(); ++i)
  {
    // Skip the vertex
    if(i == index.m_pos)
      continue;

    if(IsSpecialRingVertex(currentLine, index) && ((i == 0) || (i == currentLine->getNPoints() - 1)))
      continue;

    newLine->setPoint(pos, currentLine->getX(i), currentLine->getY(i));
    ++pos;
  }

  if(currentLine->isClosed())
    newLine->setPoint(newLine->size() - 1, newLine->getX(0), newLine->getY(0));

  // Copy the new line
  *currentLine = *newLine;

  delete newLine;
}

void te::edit::AddVertex(std::vector<te::gm::LineString*>& lines, const double& x, const double& y, const te::gm::Envelope& env, int srid)
{
  VertexIndex index = FindSegment(lines, env, srid);
  assert(index.isValid());

  te::gm::LineString* currentLine = lines[index.m_line];

  te::gm::LineString* newLine = new te::gm::LineString(currentLine->getNPoints() + 1,
                                                       currentLine->getGeomTypeId(), currentLine->getSRID());

  std::size_t pos = 0;
  for(std::size_t i = 0; i < currentLine->getNPoints(); ++i)
  {
    newLine->setPoint(pos, currentLine->getX(i), currentLine->getY(i));
    ++pos;

    if(i == index.m_pos)
    {
      newLine->setPoint(pos, x, y);
      ++pos;
    }
  }

  // Copy the new line
  *currentLine = *newLine;

  delete newLine;
}

te::edit::VertexIndex te::edit::FindSegment(std::vector<te::gm::LineString*>& lines, const te::gm::Envelope& env, int srid)
{
  VertexIndex index;
  index.makeInvalid();

  bool found = false;

  std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&env, srid));

  for(std::size_t i = 0; i < lines.size(); ++i) // for each line
  {
    te::gm::LineString* line = lines[i];

    for(std::size_t j = 0; j < line->getNPoints() - 1; ++j) // for each vertex
    {
      // Build the segment
      std::auto_ptr<te::gm::LineString> segment(new te::gm::LineString(2, line->getGeomTypeId(), line->getSRID()));
      segment->setPoint(0, line->getX(j), line->getY(j));
      segment->setPoint(1, line->getX(j + 1), line->getY(j + 1));

      if(geometryFromEnvelope->intersects(segment.get()))
      {
        index.m_line = i;
        index.m_pos = j;
        found = true;
        break;
      }
    }

    if(found)
      break;
  }

  return index;
}

bool te::edit::IsSpecialRingVertex(te::gm::LineString* l, const VertexIndex& index)
{
  return l->isClosed() && ((index.m_pos == 0) || (index.m_pos == l->getNPoints() - 1));
}
