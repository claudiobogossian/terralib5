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
  \file terralib/edit/Repository.cpp

  \brief This class represents a repository of geometries and features.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/ObjectId.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/Point.h"
#include "../geometry/Utils.h"
#include "IdGeometry.h"
#include "Repository.h"

// STL
#include <cassert>
#include <memory>

te::edit::Repository::Repository(const std::string& source, int srid)
  : m_source(source),
    m_srid(srid)
{
}

te::edit::Repository::~Repository()
{
  clear();
}

void te::edit::Repository::add(te::da::ObjectId* id, te::gm::Geometry* geom)
{
  assert(id);
  assert(geom);

  // Try find the given identifier
  std::size_t pos = getPosition(id);

  if(pos == std::string::npos) // Not found! Insert
  {
    m_geoms.push_back(new IdGeometry(id, geom));

    buildIndex(m_geoms.size() - 1, geom);

    return;
  }

  // Else, set the new values
  set(pos, id, geom);
}

void te::edit::Repository::set(te::da::ObjectId* id, te::gm::Geometry* geom)
{
  assert(id);
  assert(geom);

  // Try find the given identifier
  std::size_t pos = getPosition(id);

  if(pos == std::string::npos)
    throw te::common::Exception(TE_TR("Identifier not found!"));

  set(pos, id, geom);
}

void te::edit::Repository::remove(te::da::ObjectId* id)
{
  assert(id);

  // Try find the given identifier
  std::size_t pos = getPosition(id);

  if(pos == std::string::npos)
    throw te::common::Exception(TE_TR("Identifier not found!"));

  // Cleaning...
  delete m_geoms[pos];

  // Removing...
  m_geoms.erase(m_geoms.begin() + pos);

  // Indexing...
  buildIndex();
}

std::size_t te::edit::Repository::getPosition(te::da::ObjectId* id)
{
  assert(id);

  for(std::size_t i = 0; i < m_geoms.size(); ++i)
    if(m_geoms[i]->isEquals(id))
      return i;

  return std::string::npos;
}

bool te::edit::Repository::hasIdentifier(te::da::ObjectId* id)
{
  return getPosition(id) != std::string::npos;
}

const std::string& te::edit::Repository::getSource() const
{
  return m_source;
}

const std::vector<te::edit::IdGeometry*>& te::edit::Repository::getGeometries() const
{
  return m_geoms;
}

std::vector<te::edit::IdGeometry*> te::edit::Repository::getGeometries(const te::gm::Envelope& e, int /*srid*/) const
{
  assert(e.isValid());

  std::vector<te::edit::IdGeometry*> result;

  // Search on rtree
  std::vector<std::size_t> report;
  m_rtree.search(e, report);

  for(std::size_t i = 0; i < report.size(); ++i)
  {
    std::size_t pos = report[i];

    assert(pos < m_geoms.size());

    result.push_back(m_geoms[pos]);
  }

  return result;
}

te::edit::IdGeometry* te::edit::Repository::getGeometry(const te::gm::Envelope& e, int srid) const
{
  std::vector<te::edit::IdGeometry*> candidates = getGeometries(e, srid);

  if(candidates.empty())
    return 0;

  // Generates a geometry from the given extent. It will be used to refine the results
  std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&e, srid));

  // The restriction point. It will be used to refine the results
  te::gm::Coord2D center = e.getCenter();
  te::gm::Point point(center.x, center.y, srid);

  IdGeometry* result = 0;

  for(std::size_t i = 0; i < candidates.size(); ++i)
  {
    te::gm::Geometry* g = candidates[i]->getGeometry();

    if(g->contains(&point) || g->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(g))  // Geometry found!
      return candidates[i];
  }

  return 0;
}

void te::edit::Repository::clear()
{
  te::common::FreeContents(m_geoms);

  clearIndex();
}

void te::edit::Repository::set(const std::size_t& pos, te::da::ObjectId* id, te::gm::Geometry* geom)
{
  assert(pos != std::string::npos);
  assert(id);
  assert(geom);
  assert(pos < m_geoms.size());

  // Cleaning...
  delete m_geoms[pos];

  // Set the new values
  m_geoms[pos] = new IdGeometry(id, geom);

  // Indexing...
  buildIndex();
}

void te::edit::Repository::clearIndex()
{
  m_rtree.clear();
}

void te::edit::Repository::buildIndex()
{
  clearIndex();

  for(std::size_t i = 0; i < m_geoms.size(); ++i)
    buildIndex(i, m_geoms[i]->getGeometry());
}

void te::edit::Repository::buildIndex(const std::size_t& pos, te::gm::Geometry* geom)
{
   assert(pos != std::string::npos);
   assert(geom);

   te::gm::Envelope mbr(*geom->getMBR());

   // Indexing...
   m_rtree.insert(mbr, pos);
}
