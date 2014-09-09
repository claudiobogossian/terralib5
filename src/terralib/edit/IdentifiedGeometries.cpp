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
  \file terralib/edt/IdentifiedGeometries.cpp

  \brief This is a class for identifying edited geometries.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/ObjectId.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/Point.h"
#include "../geometry/Utils.h"
#include "IdentifiedGeometries.h"
#include "IdGeom.h"

// STL
#include <cassert>
#include <memory>

te::edit::IdentifiedGeometries::IdentifiedGeometries(const std::string& source)
  : m_source(source)
{
}

te::edit::IdentifiedGeometries::~IdentifiedGeometries()
{
  clear();
}

void te::edit::IdentifiedGeometries::add(te::da::ObjectId* id, te::gm::Geometry* geom)
{
  assert(id);
  assert(geom);

  // Try find the given identifier
  std::size_t pos = getIdentifierPos(id);

  if(pos == std::string::npos) // Not found! Insert
  {
    m_ids.push_back(id);
    m_geoms.push_back(geom);

    buildIndex(m_geoms.size() - 1, geom);

    return;
  }

  // Else, set the new values
  set(pos, id, geom);
}

void te::edit::IdentifiedGeometries::set(te::da::ObjectId* id, te::gm::Geometry* geom)
{
  assert(id);
  assert(geom);

  // Try find the given identifier
  std::size_t pos = getIdentifierPos(id);

  if(pos == std::string::npos) // Not found!
    throw te::common::Exception(TE_TR("Identifier not found!"));

  set(pos, id, geom);
}

void te::edit::IdentifiedGeometries::remove(te::da::ObjectId* id)
{
  assert(id);

  // Try find the given identifier
  std::size_t pos = getIdentifierPos(id);

  if(pos == std::string::npos) // Not found!
    throw te::common::Exception(TE_TR("Identifier not found!"));

  // Cleaning...
  delete m_ids[pos];
  delete m_geoms[pos];

  // Removing...
  m_ids.erase(m_ids.begin() + pos);
  m_geoms.erase(m_geoms.begin() + pos);

  // Indexing...
  buildIndex();
}

std::size_t te::edit::IdentifiedGeometries::getIdentifierPos(te::da::ObjectId* id)
{
  assert(id);

  for(std::size_t i = 0; i < m_ids.size(); ++i)
    if(m_ids[i]->getValueAsString() == id->getValueAsString())
      return i;

  return std::string::npos;
}

bool te::edit::IdentifiedGeometries::hasIdentifier(te::da::ObjectId* id)
{
  return getIdentifierPos(id) != std::string::npos;
}

const std::string& te::edit::IdentifiedGeometries::getSource() const
{
  return m_source;
}

const std::vector<te::da::ObjectId*>& te::edit::IdentifiedGeometries::getIdentifiers() const
{
  return m_ids;
}

const std::vector<te::gm::Geometry*>& te::edit::IdentifiedGeometries::getGeometries() const
{
  return m_geoms;
}

std::vector<te::edit::IdGeom*> te::edit::IdentifiedGeometries::getGeometries(const te::gm::Envelope& e, int /*srid*/) const
{
  assert(e.isValid());

  std::vector<te::edit::IdGeom*> result;

  // Search on rtree
  std::vector<std::size_t> report;
  m_rtree.search(e, report);

  for(std::size_t i = 0; i < report.size(); ++i)
  {
    std::size_t pos = report[i];

    assert(report[i] < m_ids.size());
    assert(report[i] < m_geoms.size());

    result.push_back(new IdGeom(m_ids[pos], m_geoms[pos]));
  }

  return result;
}

te::edit::IdGeom* te::edit::IdentifiedGeometries::getGeometry(const te::gm::Envelope& e, int srid) const
{
  std::vector<te::edit::IdGeom*> candidates = getGeometries(e, srid);

  if(candidates.empty())
    return 0;

  // Generates a geometry from the given extent. It will be used to refine the results
  std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&e, srid));

  // The restriction point. It will be used to refine the results
  te::gm::Coord2D center = e.getCenter();
  te::gm::Point point(center.x, center.y, srid);

  IdGeom* result = 0;

  for(std::size_t i = 0; i < candidates.size(); ++i)
  {
    te::gm::Geometry* g = candidates[i]->m_geom;

    if(g->contains(&point) || g->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(g))
    {
      result = new IdGeom(candidates[i]->m_id, g); // Geometry found!
      break;
    }
  }

  te::common::FreeContents(candidates);

  return result;
}

void te::edit::IdentifiedGeometries::clear()
{
  te::common::FreeContents(m_ids);
  m_ids.clear();

  te::common::FreeContents(m_geoms);
  m_geoms.clear();

  clearIndex();
}

void te::edit::IdentifiedGeometries::set(const std::size_t& pos, te::da::ObjectId* id, te::gm::Geometry* geom)
{
  assert(pos != std::string::npos);
  assert(id);
  assert(geom);
  assert(pos < m_ids.size());
  assert(pos < m_geoms.size());

  // Cleaning...
  delete m_ids[pos];
  delete m_geoms[pos];

  // Set the new values
  m_ids[pos] = id;
  m_geoms[pos] = geom;

  // Indexing...
  buildIndex();
}

void te::edit::IdentifiedGeometries::clearIndex()
{
  m_rtree.clear();
}

void te::edit::IdentifiedGeometries::buildIndex()
{
  clearIndex();

  for(std::size_t i = 0; i < m_geoms.size(); ++i)
    buildIndex(i, m_geoms[i]);
}

void te::edit::IdentifiedGeometries::buildIndex(const std::size_t& pos, te::gm::Geometry* geom)
{
   assert(pos != std::string::npos);
   assert(geom);

   te::gm::Envelope mbr(*geom->getMBR());

   // Indexing...
   m_rtree.insert(mbr, pos);
}
