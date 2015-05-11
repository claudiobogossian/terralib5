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
  \file terralib/geometry/Polygon.cpp

  \brief Polygon is a subclass of CurvePolygon whose rings are defined by linear rings.
*/

// TerraLib
#include "Polygon.h"

const std::string te::gm::Polygon::sm_typeName("Polygon");

te::gm::Polygon::Polygon(std::size_t nRings, GeomType t, int srid, Envelope* mbr)
  : CurvePolygon(nRings, t, srid, mbr)
{
}

te::gm::Polygon::Polygon(const Polygon& rhs)
  : CurvePolygon(rhs)
{
}

te::gm::Polygon::~Polygon()
{
}

te::gm::Polygon& te::gm::Polygon::operator=(const Polygon& rhs)
{
  CurvePolygon::operator=(rhs);

  return *this;
}

te::dt::AbstractData* te::gm::Polygon::clone() const
{
  return new Polygon(*this);
}

const std::string& te::gm::Polygon::getGeometryType() const throw()
{
  return sm_typeName;
}

