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
  \file terralib/geometry/MultiCurve.cpp

  \brief MultiCurve is a class that represents a 1-dimensional GeometryCollection whose elements are curves.
*/

// TerraLib
#include "Curve.h"
#include "MultiCurve.h"

// STL
#include <cassert>

te::gm::MultiCurve::MultiCurve(std::size_t nGeom, GeomType t, int srid, Envelope* mbr)
  : GeometryCollection(nGeom, t, srid, mbr)
{
}

te::gm::MultiCurve::MultiCurve(const MultiCurve& rhs)
  : GeometryCollection(rhs)
{
}

te::gm::MultiCurve& te::gm::MultiCurve::operator=(const MultiCurve& rhs)
{
  GeometryCollection::operator=(rhs);

  return *this;
}

te::dt::AbstractData* te::gm::MultiCurve::clone() const
{
  return new MultiCurve(*this);
}

bool te::gm::MultiCurve::isClosed() const
{
  std::size_t n = m_geometries.size();

  for(std::size_t i = 0; i < n; ++i)
  {
    Curve* curve = static_cast<Curve*>(m_geometries[i]);
    assert(curve);

    if(curve->isClosed() == false)
      return false;
  }

  return true;
}

double te::gm::MultiCurve::getLength() const
{
  double sum = 0.0;

  std::size_t n = m_geometries.size();

  for(std::size_t i = 0; i < n; ++i)
  {
    Curve* curve = static_cast<Curve*>(m_geometries[i]);
    assert(curve);

    sum += curve->getLength();
  }

  return sum;
}

te::gm::Dimensionality te::gm::MultiCurve::getDimension() const throw()
{
  return te::gm::L;
}

