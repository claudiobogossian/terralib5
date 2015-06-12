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
  \file terralib/edit/Snap.cpp

  \brief This class implements geometry snap concept.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Enums.h"
#include "../maptools/WorldDeviceTransformer.h"
#include "Snap.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::edit::Snap::Snap(const std::string& source, int srid)
  : m_source(source),
    m_srid(srid),
    m_nGeometries(0),
    m_maxGeometries(0),
    m_tolerance(16),
    m_transformer(0)
{
}

te::edit::Snap::~Snap()
{
  delete m_transformer;
}

std::string te::edit::Snap::getSource() const
{
  return m_source;
}

int te::edit::Snap::getSRID() const
{
  return m_srid;
}

std::size_t te::edit::Snap::getNGeometries() const
{
  return m_nGeometries;
}

std::size_t te::edit::Snap::getMaxGeometries() const
{
  return m_maxGeometries;
}

double te::edit::Snap::getTolerance() const
{
  return m_tolerance;
}

void te::edit::Snap::setTolerance(const double& t)
{
  m_tolerance = t;
}

void te::edit::Snap::setWorld(const double& llx, const double& lly,
                                      const double& urx, const double& ury,
                                      const std::size_t& width, const std::size_t& height)
{
  delete m_transformer;
  m_transformer = new te::map::WorldDeviceTransformer(llx, lly, urx, ury, width, height);
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

bool te::edit::Snap::search(const te::gm::Coord2D& coord, te::gm::Coord2D& result)
{
  te::gm::Envelope e = getSearchEnvelope(coord);

  return search(e, result);
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
  te::gm::Envelope e(dx - m_tolerance, dy + m_tolerance, dx + m_tolerance, dy - m_tolerance);

  // Convert the search envelope to world coordinates
  m_transformer->device2World(e.m_llx, e.m_lly);
  m_transformer->device2World(e.m_urx, e.m_ury);

  assert(e.isValid());

  return e;
}
