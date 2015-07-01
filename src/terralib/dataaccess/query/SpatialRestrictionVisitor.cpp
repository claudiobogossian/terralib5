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
  \file terralib/dataaccess/query/SpatialRestrictionVisitor.cpp

  \brief A visitor that retrieves spatial restrictions from a Query hierarchy.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/Utils.h"
#include "../Exception.h"
#include "Expression.h"
#include "Function.h"
#include "FunctionNames.h"
#include "LiteralEnvelope.h"
#include "LiteralGeom.h"
#include "PropertyName.h"
#include "Select.h"
#include "SpatialRestrictionVisitor.h"
#include "Where.h"

// STL
#include <cassert>

te::da::SpatialRestriction::SpatialRestriction()
  : m_index(0),
    m_type(te::gm::UNKNOWN_SPATIAL_RELATION),
    m_geometry(0),
    m_pname(""),
    m_isFromEnvelope(false),
    m_function(0)
{
}

te::da::SpatialRestriction::~SpatialRestriction()
{
  delete m_geometry;
}

te::da::SpatialRestrictionVisitor::SpatialRestrictionVisitor()
  : m_index(0)
{
  initialize();
}

te::da::SpatialRestrictionVisitor::~SpatialRestrictionVisitor()
{
  te::common::FreeContents(m_spatialRestrictions);
}

void te::da::SpatialRestrictionVisitor::visit(const Function& visited)
{
  if(isSpatialRestrictionFunction(visited))
  {
    addSpatialRestriction(visited);
    return;
  }

  std::size_t size = visited.getNumArgs();
  for(std::size_t i = 0; i < size; ++i)
  {
    assert(visited[i]);
    visited[i]->accept(*this);
  }
}

void te::da::SpatialRestrictionVisitor::visit(const Select& visited)
{
  Where* restriction = visited.getWhere();

  if(!restriction)
    return;

  Expression* e = restriction->getExp();
  if(e)
    e->accept(*this);
}

bool te::da::SpatialRestrictionVisitor::hasSpatialRestrictions() const
{
  return !m_spatialRestrictions.empty();
}

const std::vector<te::da::SpatialRestriction*>& te::da::SpatialRestrictionVisitor::getSpatialRestrictions() const
{
  return m_spatialRestrictions;
}

void te::da::SpatialRestrictionVisitor::initialize()
{
  // Initializes the map of spatial restriction functions
  m_spatialFunctions[FunctionNames::sm_ST_Intersects] = te::gm::INTERSECTS;
  m_spatialFunctions[FunctionNames::sm_ST_Disjoint  ] = te::gm::DISJOINT;
  m_spatialFunctions[FunctionNames::sm_ST_Touches   ] = te::gm::TOUCHES;
  m_spatialFunctions[FunctionNames::sm_ST_Overlaps  ] = te::gm::OVERLAPS;
  m_spatialFunctions[FunctionNames::sm_ST_Crosses   ] = te::gm::CROSSES;
  m_spatialFunctions[FunctionNames::sm_ST_Within    ] = te::gm::WITHIN;
  m_spatialFunctions[FunctionNames::sm_ST_Contains  ] = te::gm::CONTAINS;
  m_spatialFunctions[FunctionNames::sm_ST_Equals    ] = te::gm::EQUALS;

  /* TODO: not mapped values - UNKNOWN_SPATIAL_RELATION = 0, COVERS = 128, COVEREDBY = 256 */
}

bool te::da::SpatialRestrictionVisitor::isSpatialRestrictionFunction(const Function& f) const
{
  return m_spatialFunctions.find(f.getName()) != m_spatialFunctions.end();
}

te::gm::SpatialRelation te::da::SpatialRestrictionVisitor::getSpatialRelation(const Function& f) const
{
  assert(isSpatialRestrictionFunction(f));

  return m_spatialFunctions.find(f.getName())->second;
}

bool te::da::SpatialRestrictionVisitor::isFromEnvelope(const Function& f) const
{
  assert(f.getNumArgs() == 2);
  assert(f.getArg(0));
  assert(f.getArg(1));

  // Try the first argument
  te::da::LiteralEnvelope* lenv = dynamic_cast<te::da::LiteralEnvelope*>(f.getArg(0));
  if(lenv)
    return true;

  // Try the second argument
  lenv = dynamic_cast<te::da::LiteralEnvelope*>(f.getArg(1));
  if(lenv)
    return true;

  return false;
}

te::gm::Geometry* te::da::SpatialRestrictionVisitor::getGeometryRestriction(const Function& f) const
{
  assert(isSpatialRestrictionFunction(f));
  assert(f.getNumArgs() == 2);
  assert(f.getArg(0));
  assert(f.getArg(1));

  bool hasEnvelope = true;

  // Try the first argument
  te::da::LiteralEnvelope* lenv = dynamic_cast<te::da::LiteralEnvelope*>(f.getArg(0));
  if(lenv == 0)
  {
    // Try the second argument
    lenv = dynamic_cast<te::da::LiteralEnvelope*>(f.getArg(1));
    if(lenv == 0)
      hasEnvelope = false;
  }

  if(hasEnvelope)
  {
    te::gm::Envelope* e = lenv->getValue();
    assert(e);
    return te::gm::GetGeomFromEnvelope(e, lenv->getSRID());
  }

  // Here there is not a LiteralEnvelope. Try LiteralGeom!
  te::da::LiteralGeom* lgeom = dynamic_cast<te::da::LiteralGeom*>(f.getArg(0));
  if(lgeom == 0)
  {
    // Try the second argument
    lgeom = dynamic_cast<te::da::LiteralGeom*>(f.getArg(1));
    if(lgeom == 0)
      return 0;
  }

  te::dt::AbstractData* data = lgeom->getValue();
  assert(data);

  te::gm::Geometry* geom = dynamic_cast<te::gm::Geometry*>(data);
  assert(geom);

  return dynamic_cast<te::gm::Geometry*>(geom->clone());
}

std::string te::da::SpatialRestrictionVisitor::getPropertyName(const Function& f) const
{
  assert(f.getNumArgs() == 2);
  assert(f.getArg(0));
  assert(f.getArg(1));

  // Try the first argument
  te::da::PropertyName* pname = dynamic_cast<te::da::PropertyName*>(f.getArg(0));
  if(pname)
    return pname->getName();

  // Try the second argument
  pname = dynamic_cast<te::da::PropertyName*>(f.getArg(1));
  if(pname)
    return pname->getName();

  return "";
}

void te::da::SpatialRestrictionVisitor::addSpatialRestriction(const Function& f)
{
  // Gets the geometry restriction
  te::gm::Geometry* geom = getGeometryRestriction(f);
  if(geom == 0)
    throw Exception(TE_TR("The spatial function not have a literal geometry or literal envelope!"));

  // Gets the property name
  std::string pname = getPropertyName(f);
  if(pname.empty())
    throw Exception(TE_TR("The spatial function not have a property name!"));

  // Creates the spatial restriction
  SpatialRestriction* restriction = new SpatialRestriction;
  restriction->m_index = m_index++;
  restriction->m_geometry = geom;
  restriction->m_pname = pname;
  restriction->m_type = getSpatialRelation(f);
  restriction->m_isFromEnvelope = isFromEnvelope(f);
  restriction->m_function = dynamic_cast<const Function*>(&f);

  // Adds the spatial restriction
  m_spatialRestrictions.push_back(restriction);
}
