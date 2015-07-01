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
  \file GeometryOperands.cpp
  
  \brief Geometry operands.
 */

// TerraLib
#include "GeometryOperands.h"
#include "Globals.h"

// STL
#include <cassert>

std::set<const char*, te::common::LessCmp<const char*> >  te::fe::GeometryOperands::sm_validGeometryOperands;

te::fe::GeometryOperands::GeometryOperands()
{
}

te::fe::GeometryOperands::~GeometryOperands()
{
// we will not clear the vector items because they are just reference to an internal list of operands.
}

size_t te::fe::GeometryOperands::size() const
{
  return m_operands.size();
}

void te::fe::GeometryOperands::push_back(const char* g)
{
  assert(g);
  m_operands.push_back(g);
}

const char* te::fe::GeometryOperands::operator[](size_t i) const
{
  assert(i < m_operands.size());
  return m_operands[i];
}

const char* te::fe::GeometryOperands::findOperand(const char* o)
{
  assert(o);

  std::set<const char*, te::common::LessCmp<const char*> >::const_iterator it = sm_validGeometryOperands.find(o);

  if(it != sm_validGeometryOperands.end())
    return *it;

  return 0;
}

void te::fe::GeometryOperands::loadValidGeometryOperandsList()
{
  sm_validGeometryOperands.insert(Globals::sm_envelope);
  sm_validGeometryOperands.insert(Globals::sm_point);
  sm_validGeometryOperands.insert(Globals::sm_lineString);
  sm_validGeometryOperands.insert(Globals::sm_polygon);
  sm_validGeometryOperands.insert(Globals::sm_arcByCenterPoint);
  sm_validGeometryOperands.insert(Globals::sm_circleByCenterPoint);
  sm_validGeometryOperands.insert(Globals::sm_arc);
  sm_validGeometryOperands.insert(Globals::sm_circle);
  sm_validGeometryOperands.insert(Globals::sm_arcByBulge);
  sm_validGeometryOperands.insert(Globals::sm_bezier);
  sm_validGeometryOperands.insert(Globals::sm_clothoid);
  sm_validGeometryOperands.insert(Globals::sm_cubicSpline);
  sm_validGeometryOperands.insert(Globals::sm_geodesic);
  sm_validGeometryOperands.insert(Globals::sm_offsetCurve);
  sm_validGeometryOperands.insert(Globals::sm_triangle);
  sm_validGeometryOperands.insert(Globals::sm_polyhedralSurface);
  sm_validGeometryOperands.insert(Globals::sm_triangulatedSurface);
  sm_validGeometryOperands.insert(Globals::sm_tin);
  sm_validGeometryOperands.insert(Globals::sm_solid);
}

void te::fe::GeometryOperands::clearValidGeometryOperandsList()
{
  sm_validGeometryOperands.clear();
}



