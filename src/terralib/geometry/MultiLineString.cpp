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
  \file terralib/geometry/MultiLineString.cpp

  \brief MultiLineString is a MultiCurve whose elements are LineStrings.
*/

// TerraLib
#include "MultiLineString.h"

// STL
#include <cassert>

const std::string te::gm::MultiLineString::sm_typeName("MultiLineString");

te::gm::MultiLineString::MultiLineString(std::size_t nGeom, GeomType t, int srid, Envelope* mbr)
  : MultiCurve(nGeom, t, srid, mbr)
{
  assert(t == MultiLineStringType || t == MultiLineStringZType || t == MultiLineStringMType || t == MultiLineStringZMType);
}

te::gm::MultiLineString::MultiLineString(const MultiLineString& rhs)
  : MultiCurve(rhs)
{
}

te::gm::MultiLineString& te::gm::MultiLineString::operator=(const MultiLineString& rhs)
{
  MultiCurve::operator=(rhs);

  return *this;
}

te::dt::AbstractData* te::gm::MultiLineString::clone() const
{
  return new MultiLineString(*this);
}

const std::string& te::gm::MultiLineString::getGeometryType() const throw()
{
  return sm_typeName;
}

te::gm::Geometry* te::gm::MultiLineString::locateBetween(const double& /*mStart*/, const double& /*mEnd*/) const throw(Exception)
{
  return 0;
}

