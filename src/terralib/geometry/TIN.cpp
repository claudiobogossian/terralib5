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
  \file terralib/geometry/TIN.cpp

  \brief TIN (triangulated irregular network) is a PolyhedralSurface consisting only of Triangle patches.
*/

// TerraLib
#include "Polygon.h"
#include "TIN.h"

const std::string te::gm::TIN::sm_typeName("Tin");

te::gm::TIN::TIN(std::size_t nPolygons, GeomType t, int srid, Envelope* mbr)
  : PolyhedralSurface(nPolygons, t, srid, mbr)
{
}

te::gm::TIN::TIN(const TIN& rhs)
  : PolyhedralSurface(rhs)
{
}

te::gm::TIN& te::gm::TIN::operator=(const TIN& rhs)
{
  PolyhedralSurface::operator=(rhs);

  return *this;
}

const std::string& te::gm::TIN::getGeometryType() const throw()
{
  return sm_typeName;
}


void te::gm::TIN::setGeometryN(std::size_t i, te::gm::Polygon* g)
{
	assert((i < m_polygons.size()) && (m_polygons[i] == 0));
	delete m_polygons[i];
	m_polygons[i] = g;
}

void te::gm::TIN::add(te::gm::Polygon* g)
{
	m_polygons.push_back(g);
}

