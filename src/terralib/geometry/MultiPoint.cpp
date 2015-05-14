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
  \file terralib/geometry/MultiPoint.cpp

  \brief MultiPoint is a GeometryCollection whose elements are restricted to points.
*/

// TerraLib
#include "MultiPoint.h"

// STL
#include <cassert>
#include <ostream>

const std::string te::gm::MultiPoint::sm_typeName("MultiPoint");

te::gm::MultiPoint::MultiPoint(std::size_t nGeom, GeomType t, int srid, Envelope* mbr)
  : GeometryCollection(nGeom, t, srid, mbr)
{
  assert(t == MultiPointType || t == MultiPointZType || t == MultiPointMType || t == MultiPointZMType);
}

te::gm::MultiPoint::MultiPoint(const MultiPoint& rhs)
  : GeometryCollection(rhs)
{
}

te::gm::MultiPoint& te::gm::MultiPoint::operator=(const MultiPoint& rhs)
{
  GeometryCollection::operator=(rhs);

  return *this;
}

te::dt::AbstractData* te::gm::MultiPoint::clone() const
{
  return new MultiPoint(*this);
}

te::gm::Dimensionality te::gm::MultiPoint::getDimension() const throw()
{
  return te::gm::P;
}

const std::string& te::gm::MultiPoint::getGeometryType() const throw()
{
  return sm_typeName;
}

te::gm::Geometry* te::gm::MultiPoint::locateBetween(const double& /*mStart*/, const double& /*mEnd*/) const throw(Exception)
{
  return 0;
}

