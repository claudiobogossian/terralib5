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
  \file terralib/edit/SnapVertex.cpp

  \brief This class implements a vertex search snap.
*/

// TerraLib
#include "SnapVertex.h"
#include "Utils.h"

// STL
#include <cassert>

te::edit::SnapVertex::SnapVertex(const std::string& source, int srid)
  : Snap(source, srid)
{
}

te::edit::SnapVertex::~SnapVertex()
{
}

void te::edit::SnapVertex::add(te::gm::Geometry* geom)
{
  assert(geom);

  if(m_maxGeometries > 0 && m_nGeometries >= m_maxGeometries)
    return;

  std::size_t lastPos = m_coords.size();

  // Adding the new coordinates
  GetCoordinates(geom, m_coords);

  // Indexing...
  for(std::size_t i = lastPos; i < m_coords.size(); ++i)
  {
    te::gm::Envelope e(m_coords[i].x, m_coords[i].y, m_coords[i].x, m_coords[i].y);
    m_rtree.insert(e, i);
  }

  ++m_nGeometries;
}

void te::edit::SnapVertex::clear()
{
  m_nGeometries = 0;
  m_coords.clear();
  m_rtree.clear();
}

std::string te::edit::SnapVertex::getName() const
{
  return "SnapVertex";
}

std::string te::edit::SnapVertex::getDescription() const
{
  return "Implements vertex search snap.";
}

te::edit::Snap* te::edit::SnapVertex::Builder(const std::string& source, int srid)
{
  return new SnapVertex(source, srid);
}

bool te::edit::SnapVertex::search(const te::gm::Envelope& e, te::gm::Coord2D& result)
{
  assert(e.isValid());

  std::vector<std::size_t> report;
  m_rtree.search(e, report);

  if(report.empty())
    return false;

  std::size_t snappedPos = report[0];

  if(report.size() > 1)
  {
    // Finds the nearest coordinate
    te::gm::Coord2D center = e.getCenter();
    double minDistance = std::numeric_limits<double>::max();
    for(std::size_t i = 1; i < report.size(); ++i)
    {
      const te::gm::Coord2D& foundCoord = m_coords[report[i]];
      double distance = GetDistance(center, foundCoord);
      if(distance < minDistance)
      {
        minDistance = distance;
        snappedPos = report[i];
      }
    }
  }

  assert(snappedPos < m_coords.size());

  // Gets the snapped coordinate
  const te::gm::Coord2D& snapped = m_coords[snappedPos];

  // Informs the output
  result.x = snapped.x;
  result.y = snapped.y;

  return true;
}

