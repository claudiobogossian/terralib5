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
  \file terralib/geometry/LinearRing.cpp

  \brief A LinearRing is a LineString that is both closed and simple.
*/

// TerraLib
#include "LinearRing.h"

// STL
#include <cassert>

te::gm::LinearRing::LinearRing(GeomType t, int srid, Envelope* mbr)
  : LineString(t, srid, mbr)
{
}

te::gm::LinearRing::LinearRing(std::size_t size, GeomType t, int srid, Envelope* mbr)
  : LineString(size, t, srid, mbr)
{
}

te::gm::LinearRing::LinearRing(const LinearRing& rhs)
  : LineString(rhs)
{
}

te::gm::LinearRing& te::gm::LinearRing::operator=(const LinearRing& rhs)
{
  LineString::operator=(rhs);

  return *this;
}

te::dt::AbstractData* te::gm::LinearRing::clone() const
{
  return new LinearRing(*this);
}

