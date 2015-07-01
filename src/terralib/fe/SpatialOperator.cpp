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
  \file SpatialOperator.cpp
  
  \brief Spatial operator.
 */

// TerraLib
#include "GeometryOperands.h"
#include "Globals.h"
#include "SpatialOperator.h"

// STL
#include <cassert>

std::set<const char*, te::common::LessCmp<const char*> >  te::fe::SpatialOperator::sm_validSpatialOperators;

te::fe::SpatialOperator::SpatialOperator(const char* opName)
  : m_name(opName),
    m_geometryOperands(0)
{
  assert(opName);
}

te::fe::SpatialOperator::~SpatialOperator()
{
  delete m_geometryOperands;
}

const char* te::fe::SpatialOperator::getOpName() const
{
  return m_name;
}

void te::fe::SpatialOperator::setGeometryOperands(GeometryOperands* gOps)
{
  delete m_geometryOperands;
  m_geometryOperands = gOps;
}

const te::fe::GeometryOperands* te::fe::SpatialOperator::getGeometryOperands() const
{
  return m_geometryOperands;
}

const char* te::fe::SpatialOperator::findOperator(const char* o)
{
  assert(o);

  std::set<const char*, te::common::LessCmp<const char*> >::const_iterator it = sm_validSpatialOperators.find(o);

  if(it != sm_validSpatialOperators.end())
    return *it;

  return 0;
}

void te::fe::SpatialOperator::loadValidOperatorList()
{
  sm_validSpatialOperators.insert(Globals::sm_bbox);
  sm_validSpatialOperators.insert(Globals::sm_equals);
  sm_validSpatialOperators.insert(Globals::sm_disjoint);
  sm_validSpatialOperators.insert(Globals::sm_touches);
  sm_validSpatialOperators.insert(Globals::sm_within);
  sm_validSpatialOperators.insert(Globals::sm_overlaps);
  sm_validSpatialOperators.insert(Globals::sm_crosses);
  sm_validSpatialOperators.insert(Globals::sm_intersects);
  sm_validSpatialOperators.insert(Globals::sm_contains);
  sm_validSpatialOperators.insert(Globals::sm_dWithin);
  sm_validSpatialOperators.insert(Globals::sm_beyond);
}

void te::fe::SpatialOperator::clearValidOperatorList()
{
  sm_validSpatialOperators.clear();
}

