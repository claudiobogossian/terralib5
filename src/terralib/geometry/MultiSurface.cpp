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
  \file terralib/geometry/MultiSurface.cpp

  \brief MultiSurface is a class that represents a 2-dimensional GeometryCollection whose elements are surfaces.
*/

// TerraLib
#include "MultiSurface.h"
#include "Surface.h"

// STL
#include <cassert>

te::gm::MultiSurface::MultiSurface(std::size_t nGeom, GeomType t, int srid, Envelope* mbr)
  : GeometryCollection(nGeom, t, srid, mbr)
{
}

te::gm::MultiSurface::MultiSurface(const MultiSurface& rhs)
  : GeometryCollection(rhs)
{
}

te::gm::MultiSurface& te::gm::MultiSurface::operator=(const MultiSurface& rhs)
{
  GeometryCollection::operator=(rhs);

  return *this;
}

te::dt::AbstractData* te::gm::MultiSurface::clone() const
{
  return new MultiSurface(*this);
}

double te::gm::MultiSurface::getArea() const
{
  double sum = 0.0;

  std::size_t n = m_geometries.size();

  for(std::size_t i = 0; i < n; ++i)
  {
    Surface* surface = static_cast<Surface*>(m_geometries[i]);
    assert(surface);

    sum += surface->getArea();
  }

  return sum;
}

te::gm::Point* te::gm::MultiSurface::getCentroid() const
{
  return 0;
}

te::gm::Coord2D* te::gm::MultiSurface::getCentroidCoord() const
{
  return 0;
}

te::gm::Point* te::gm::MultiSurface::getPointOnSurface() const
{
  return 0;
}

te::gm::Coord2D* te::gm::MultiSurface::getCoordOnSurface() const
{
  return 0;
}

double te::gm::MultiSurface::getPerimeter() const
{
  double sum = 0.0;

  std::size_t n = m_geometries.size();

  for(std::size_t i = 0; i < n; ++i)
  {
    Surface* surface = static_cast<Surface*>(m_geometries[i]);
    assert(surface);

    sum += surface->getPerimeter();
  }

  return sum;
}

te::gm::Dimensionality te::gm::MultiSurface::getDimension() const throw()
{
  return te::gm::A;
}

