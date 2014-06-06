/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/query/ST_ConvexHull.cpp

  \brief Spatial convex hull operator.
*/

// TerraLib
#include "../../geometry/Geometry.h"
#include "LiteralGeom.h"
#include "LiteralBool.h"
#include "PropertyName.h"
#include "ST_ConvexHull.h"

te::da::ST_ConvexHull::ST_ConvexHull(const te::gm::Geometry& g)
  : Function("st_convexhull")
{
  LiteralGeom* geom = new LiteralGeom(g);
  m_args.push_back(geom);
}

te::da::ST_ConvexHull::ST_ConvexHull(const PropertyName& prop)
  : Function("st_convexhull")
{
  m_args.push_back(prop.clone());
}

te::da::ST_ConvexHull::ST_ConvexHull(const Expression& e)
  : Function("st_convexhull")
{
  m_args.push_back(e.clone());
}

te::da::ST_ConvexHull::~ST_ConvexHull()
{
}

te::da::ST_ConvexHull& te::da::ST_ConvexHull::operator=(const ST_ConvexHull& rhs)
{
  Function::operator=(rhs);
  return *this;
}

te::da::Expression* te::da::ST_ConvexHull::clone() const
{
  return new ST_ConvexHull(*this);
}

