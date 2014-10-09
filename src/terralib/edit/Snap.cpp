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
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Enums.h"
#include "../geometry/Coord2D.h"
#include "../maptools/WorldDeviceTransformer.h"
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
    m_tolerance(32),
    m_transformer(0)
{
}

te::edit::Snap::~Snap()
{
  delete m_transformer;
}

void te::edit::Snap::build(te::da::DataSet* dataset)
{
  assert(dataset);

  clear();

  add(dataset);
}

void te::edit::Snap::add(te::da::DataSet* dataset)
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

bool te::edit::Snap::search(const te::gm::Coord2D& coord, te::gm::Coord2D& result)
{
  te::gm::Envelope e = getSearchEnvelope(coord);

  return search(e, result);
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

void te::edit::Snap::setWorld(const double& llx, const double& lly,
                              const double& urx, const double& ury,
                              const std::size_t& width, const std::size_t& height)
{
  delete m_transformer;
  m_transformer = new te::map::WorldDeviceTransformer(llx, lly, urx, ury, width, height);
}

te::gm::Envelope te::edit::Snap::getSearchEnvelope(const te::gm::Coord2D& coord) const
{
  if(m_transformer == 0)
    throw te::common::Exception(TE_TR("World/Device transformer not defined!"));

  // Device coordinates
  double dx = 0.0;
  double dy = 0.0;

  // Transform the given coordinate to device coordinate
  m_transformer->world2Device(coord.x, coord.y, dx, dy);

  // Build the search envelope (device coordinates) using the snap tolerance
  te::gm::Envelope e(dx - m_tolerance, dy - m_tolerance, dx + m_tolerance, dy + m_tolerance);

  // Convert the search envelope to world coordinates
  m_transformer->device2World(e.m_llx, e.m_lly);
  m_transformer->device2World(e.m_urx, e.m_ury);

  return e;
}

int te::edit::Snap::getSRID() const
{
  return m_srid;
}

bool te::edit::Snap::search(const te::gm::Envelope& e, te::gm::Coord2D& result)
{
  std::vector<std::size_t> report;
  m_rtree.search(e, report);

  if(report.empty())
    return false;

  std::size_t snappedPos = 0;

  if(report.size() > 1)
  {
    // Finds the nearest coordinate
    te::gm::Coord2D center = e.getCenter();
    double minDistance = std::numeric_limits<double>::max();
    for(std::size_t i = 0; i < report.size(); ++i)
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
