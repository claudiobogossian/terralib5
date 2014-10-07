/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/edit/Snap.cpp

  \brief This class implements geometry snap concept.
*/

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Enums.h"
#include "../geometry/Coord2D.h"
#include "Snap.h"
#include "Utils.h"

// STL
#include <cassert>
#include <limits>
#include <memory>

te::edit::Snap::Snap(const std::string& source, int srid)
  : m_source(source),
    m_srid(srid),
    m_nGeometries(0),
    m_maxGeometries(0),
    m_tolerance(32)
{
}

te::edit::Snap::~Snap()
{
}

void te::edit::Snap::build(te::da::DataSet* dataset)
{
  assert(dataset);

  std::size_t gpos = te::da::GetFirstPropertyPos(dataset, te::dt::GEOMETRY_TYPE);

  if(gpos == std::string::npos)
    return;

  while(dataset->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> g(dataset->getGeometry(gpos));
    add(g.get());
  }
}

void te::edit::Snap::add(te::gm::Geometry* geom)
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

bool te::edit::Snap::search(const te::gm::Envelope& e, te::gm::Coord2D& coord)
{
  std::vector<std::size_t> report;
  m_rtree.search(e, report);

  if(report.empty())
    return false;

  std::size_t snappedPos = 0;

  if(report.size() > 1)
  {
    // Finds the nearest coordinate
    double minDistance = std::numeric_limits<double>::max();
    for(std::size_t i = 0; i < report.size(); ++i)
    {
      const te::gm::Coord2D& foundCoord = m_coords[report[i]];
      double distance = GetDistance(coord, foundCoord);
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
  coord.x = snapped.x;
  coord.y = snapped.y;

  return true;
}

std::size_t te::edit::Snap::getNGeometries() const
{
  return m_nGeometries;
}

double te::edit::Snap::getTolerance() const
{
  return m_tolerance;
}

void te::edit::Snap::setTolerance(const double& t)
{
  m_tolerance = t;
}

void te::edit::Snap::clear()
{
  m_nGeometries = 0;
  m_coords.clear();
  m_rtree.clear();
}
