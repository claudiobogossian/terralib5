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
  \file te/geometry/MultiPolygon.cpp

  \brief MultiPolygon is a MultiSurface whose elements are Polygons.
*/

// TerraLib
#include "MultiPolygon.h"

// STL
#include <cassert>

const std::string te::gm::MultiPolygon::sm_typeName("MultiPolygon");

te::gm::MultiPolygon::MultiPolygon(std::size_t nGeom, GeomType t, int srid, Envelope* mbr)
  : MultiSurface(nGeom, t, srid, mbr)
{
  assert(t == MultiPolygonType || t == MultiPolygonZType || t == MultiPolygonMType || t == MultiPolygonZMType);
}

te::gm::MultiPolygon::MultiPolygon(const MultiPolygon& rhs)
  : MultiSurface(rhs) 
{
}

te::gm::MultiPolygon& te::gm::MultiPolygon::operator=(const MultiPolygon& rhs)
{
  MultiSurface::operator=(rhs);

  return *this;
}

te::dt::AbstractData* te::gm::MultiPolygon::clone() const
{
  return new MultiPolygon(*this);
}

const std::string& te::gm::MultiPolygon::getGeometryType() const throw()
{
  return sm_typeName;
}

